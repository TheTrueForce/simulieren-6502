// 65c02 simulator, originally intended for the Arduino Due, but it should be
//  platform-agnostic.
// This simulator covers _only_ the 65c02 microprocessor. Any peripherals will
//  need to be handled by other code.
// This simulator is intended to be mostly, but not totally faithful to the
//  W65C02 processor. A few alterations have been made to make it slightly
//  easier to use, and in some cases these are optional.
//
// A list of the changes is below:
//  - RTI restores the old BRK bit. Real hardware does not.
//  - The default reset routine sets A, X, and Y to 0x00, and the stack pointer
//     to 0xFF. None of these registers are initialised on real hardware.
//    reset6502() takes a boolean parameter that determines whether or not it
//     does a faithful reset.
//  - Decimal mode is unreliable, but at least gets it right sometimes.
//     BCD inputs are assumed, and are converted to binary. Addition is
//      performed on the binary versions, and the result converted to BCD,
//      truncating if an overflow occurs. Addition appears to be correct, but
//      Klaus' test suite complains about decimal sbc, possibly due to the fact
//      that sbc is implemented as an adc with a complemented input.
//     Zero, Negative and Overflow function exactly the same way as in binary mode.
//      For Zero, this is fine. negative BCD numbers are a pain to represent, and the Overflow flag is a signed thing anyway.

#include "simulieren-6502.h"
#include "opcodes.h"

#include <stdio.h>

/*************
 * Registers *
 *************/
uint8_t A;
uint8_t X, Y;
uint8_t stackPointer;
uint16_t programCounter;

/*************************
 * Status register flags *
 *************************/
bool flagNegative;  // Indicates result negative(bit 7 set).
bool flagOverflow;  // Indicates two's-complement arithmetic overflow.
bool flagBRK;       // Indicates that the last interrupt was caused by a BRK instruction. (this isn't actually a bit in the hardware)
bool flagDecimal;   // Indicates whether decimal mode is set.
bool flagIRQdisable;// Indicates whether the emulated 6502 responds to IRQs. NMIs are unaffected.
bool flagZero;      // Indicates result zero.
bool flagCarry;     // Indicates carry out of an addition or borrow out of a subtraction.

/******************
 * Internal flags *
 ******************/

// Set when a WAI instruction is executed, cleared when an IRQ, NMI, or reset
//  hits.
bool hitWAI = false;

// Set when a STP instruction is executed, cleared when a reset occurs.
bool hitSTP = false;

// Set when an IRQ is registered using raiseIRQ(), or by a BRK instruction,
//  cleared when lowerIRQ() is called.
// NOT CLEARED BY RESET!
bool IRQraised = false;

// Set when an NMI is raised using raiseNMI(), cleared when the the NMI is
//  serviced.
// NOT CLEARED BY RESET!
bool NMIraised = false;


/**************************
 * Memory access routines *
 **************************/
//  This exists to enable the emulating device to handle memory-mapped hardware
extern uint8_t readByte(uint16_t address);
extern void writeByte(uint16_t address, uint8_t data);

uint16_t readShort(uint16_t address) {
    return readByte(address) + (readByte(address+1) << 8);
}

// stack manipulation routines
uint8_t pullByte() {
    stackPointer++;
    return readByte(0x0100 + stackPointer);
}
void pushByte(uint8_t data) {
    writeByte(0x0100 + stackPointer, data);
    stackPointer--;
}

// program counter stacking and unstacking
void pullPC() {
    programCounter = pullByte();
    programCounter += pullByte() << 8;
}
void pushPC() {
    // push high byte, then low byte, to maintain low byte = low address
    pushByte((programCounter & 0xFF00) >> 8);
    pushByte(programCounter & 0x00FF);
}

#define MASK_FLAG_NEGATIVE  0x80
#define MASK_FLAG_OVERFLOW  0x40
#define MASK_FLAG_UNUSED    0x20
#define MASK_FLAG_BREAK     0x10
#define MASK_FLAG_DECIMAL   0x08
#define MASK_FLAG_INTERRUPT 0x04
#define MASK_FLAG_ZERO      0x02
#define MASK_FLAG_CARRY     0x01

// status register stacking and unstacking
void pullStatus() {
    flagCarry = false;
    flagZero = false;
    flagIRQdisable = false;
    flagDecimal = false;
    flagBRK = false;
    flagOverflow = false;
    flagNegative = false;
    
    uint8_t temp = pullByte();
    if ((temp & 0x01) != 0) flagCarry = true;
    if ((temp & 0x02) != 0) flagZero = true;
    if ((temp & 0x04) != 0) flagIRQdisable = true;
    if ((temp & 0x08) != 0) flagDecimal = true;
    if ((temp & 0x10) != 0) flagBRK = true;
    // 0x20 - unused bit
    if ((temp & 0x40) != 0) flagOverflow = true;
    if ((temp & 0x80) != 0) flagNegative = true;
}
void pushStatus() {
    uint8_t temp = 0;
    
    if (flagNegative) temp += MASK_FLAG_NEGATIVE;
    if (flagOverflow) temp += MASK_FLAG_OVERFLOW;
    // insert a one in the unused spot.
    temp += MASK_FLAG_UNUSED;
    ///*if (flagBRK)*/ temp++; // Klaus' test suite(and everything else I've seen) seems to imply that BRK is always set.
    temp += MASK_FLAG_BREAK;
    if (flagDecimal) temp += MASK_FLAG_DECIMAL;
    if (flagIRQdisable) temp += MASK_FLAG_INTERRUPT;
    if (flagZero) temp += MASK_FLAG_ZERO;
    if (flagCarry) temp += MASK_FLAG_CARRY;
    
    pushByte(temp);
}


/**********************
 * Interface routines *
 **********************/
// This function indicates to the emulated processor that an interrupt is
//  awaiting service.
void raiseIRQ() {
    IRQraised = true;
}

// This function indicates to the emulated processor that no interrupts are
//  awaiting service.
void lowerIRQ() {
    IRQraised = false;
}

// This function indicates to the emulated processor that a non-maskable
//  interrupt has occurred. There is no lowerNMI() because on a real 65c02, the
//  NMI input is active-edge-sensitive.
void raiseNMI() {
    NMIraised = true;
}

// Sets the Overflow(V) flag, in much the same way the /SO (Set Overflow) pin on
//  real hardware would.
void setOverflow() {
    flagOverflow = true;
}


/**********************
 * Emulation routines *
 **********************/

// resets the emulated 6502.
// Parameters:
//  bool faithful   If true, reset the simulated procesor in the same way as
//                   real hardware.
//                  If false, initialise the user registers to zero, the stack
//                   pointer to 0xFF(the top of the stack region), and all the
//                   flags(N=0, V=0, B=0, Z=1, C=0).
void reset6502(bool faithful) {
    // A, X, Y, and the stack pointer are not changed through a reset on real
    //  hardware.
    if (!faithful) {
        // if we're not interested in a faithful recreation of the reset
        //  procedure, initialize the registers and flags.
        A = 0x00;
        X = 0x00;
        Y = 0x00;
        stackPointer = 0xFF;
        flagNegative = false;
        flagOverflow = false;
        flagBRK = false;
        flagZero = true;
        flagCarry = false;
    }
    
    // Decimal mode is disabled, and IRQs are disabled.
    flagDecimal = false;
    flagIRQdisable = true;
    
    // Load up the reset vector, ready for execution.
    programCounter = readShort(RESET_VEC);
    // the effects of STP and WAI are ended by a reset.
    hitSTP = false;
    hitWAI = false;
}

void doInterrupt(uint16_t vector) {
    hitWAI = false;
    pushPC();
    pushStatus();
    flagDecimal = false;
    flagIRQdisable = true;
    programCounter = readShort(vector);
}

void branch(int8_t displacement) {
    programCounter += displacement;
}
void branchIf(bool flag) {
    if (flag) {
        branch((int8_t) readByte(programCounter++));
    } else {
        programCounter++;
    }
}

// addressing mode resolvers
uint16_t getABSAddr() {
    uint16_t ret = readShort(programCounter);
    programCounter += 2;
    return ret;
}
uint16_t getABS_XAddr() {
    return getABSAddr() + X;
}
uint16_t getABS_YAddr() {
    return getABSAddr() + Y;
}
uint16_t getABS_INDAddr() {
    uint16_t addrSrc = getABSAddr();
    return readShort(addrSrc);
}
uint16_t getABS_X_INDAddr() {
    uint16_t addrSrc = getABS_XAddr();
    return readShort(addrSrc);
}
uint16_t getZPAddr() {
    return 0x0000 | readByte(programCounter++);
}
uint16_t getZP_XAddr() {
    // ZP addressing stays within ZP
    return (getZPAddr() + X) & 0x00FF;
}
uint16_t getZP_YAddr() {
    // ZP addressing stays within ZP
    return (getZPAddr() + Y) & 0x00FF;
}
uint16_t getZP_INDAddr() {
    uint16_t addrSrc = getZPAddr();
    return readShort(addrSrc);
}
uint16_t getZP_X_INDAddr() {
    uint16_t addrSrc = getZP_XAddr();
    return readShort(addrSrc);
}
uint16_t getZP_IND_YAddr() {
    return getZP_INDAddr() + Y;
}

void adc(uint8_t value) {
    uint16_t intermediateResult = 0; // this assignment is temporary
    uint8_t oldA = A;
    
    if (!flagDecimal) {
        // binary mode
        intermediateResult = A + value + (flagCarry ? 1 : 0);
        flagCarry = (intermediateResult > 0xFF) ? true : false;
        A = (uint8_t)intermediateResult;
    } else {
            //printf("decimal mode adc:\n");
        //decimal mode
        // add low nybble
        intermediateResult = (A & 0x0F) + (value & 0x0F) + (flagCarry ? 1 : 0);
            //printf("low nybble = %02X\n", intermediateResult);
        // correct for BCD
        if (intermediateResult > 0x09) {
            intermediateResult += 0x06;
        }
            //printf("corrected = %02X\n", intermediateResult);
        
        // add high nybble
        intermediateResult = (A & 0xF0) + (value & 0xF0) + intermediateResult;
            //printf("high nybble = %02X\n", intermediateResult & 0xFF0);
        // correct for BCD
        if ((intermediateResult & 0xFF0) > 0x90) {
            intermediateResult += 0x60;
            flagCarry = true;
        }
            //printf("corrected = %02X\n", intermediateResult & 0xFF0);
        A = intermediateResult;
            //printf("decimal mode adc: %i + %02X + %02X = %02X\n", flagCarry, oldA, value, A);
    }
    
    // Set V if the sign of both inputs is the same, and that sign is different to the sign of the result.
    //  Else, clear it.
    // I'm not sure if I really need to use a cast in this.
    // Source for this formula: http://www.righto.com/2012/12/the-6502-overflow-flag-explained.html
    // not valid for decimal mode
    flagOverflow = (value^(uint8_t)intermediateResult)&(oldA^(uint8_t)intermediateResult)&0x80;
    
    // unchanged from binary mode.
    flagZero = (A == 0x00);
    flagNegative = (A & 0x80);
}
void sbc(uint8_t value) {
    // sbc == adc( value XOR $FF) == adc(~value).
    // this does not hold for decimal mode.
    if (!flagDecimal) {
        adc(~value);
    } else {
        
            //printf("decimal sbc: %02X - %02X. C = %i\n", A, value, flagCarry);
        // I do not understand this properly...
        uint16_t intermediateResult = 0;
        uint8_t oldA = A;
        // subtract low nybble
        intermediateResult = (A & 0x0F) - (value & 0x0F) - (!flagCarry ? 1 : 0);
            //printf("low nybble: %04X\n", intermediateResult);
        // correct for BCD
        if (intermediateResult > 0x09) {
            intermediateResult -= 0x06;
        }
            //printf("corrected: %04X\n", intermediateResult);
        // subtract high nybble
        intermediateResult = (A & 0xF0) - (value & 0xF0) + intermediateResult;// - (!flagCarry ? 1 : 0);
            //printf("high nybble: %04X\n", intermediateResult);
        // correct for BCD
        if ((intermediateResult & 0xF0) > 0x90) {
            intermediateResult -= 0x60;
        }
        flagCarry = (intermediateResult < 0x8000);
            //printf("corrected: %04X\n", intermediateResult);
        A = intermediateResult;
            //printf("final: %02X, C = %s\n", A, (flagCarry ? "true" : "false"));
        
        flagOverflow = (value^(uint8_t)intermediateResult)&(oldA^(uint8_t)intermediateResult)&0x80;
        flagZero = (A == 0x00);
        flagNegative = (A & 0x80);
    }
    
    
}
void cmp(uint8_t value) {
    uint8_t result = A - value;
    flagCarry = (A >= value) ? true : false;
    flagZero = (result == 0x00);
    flagNegative = (result & 0x80);
}
void cpx(uint8_t value) {
    uint8_t result = X - value;
    flagNegative = (result & 0x80);
    flagZero = result == 0x00;
    flagCarry = X >= value;
}
void cpy(uint8_t value) {
    uint8_t result = Y - value;
    flagNegative = (result & 0x80);
    flagZero = result == 0x00;
    flagCarry = Y >= value;
}
void inc(uint16_t address) {
    uint8_t result = readByte(address)+1;
    flagNegative = (result & 0x80);
    flagZero = result == 0;
    writeByte(address, result);
}
void dec(uint16_t address) {
    uint8_t result = readByte(address)-1;
    flagNegative = (result & 0x80);
    flagZero = result == 0;
    writeByte(address, result);
}

void op_and(uint8_t value) {
    A &= value;
    flagNegative = (A & 0x80);
    flagZero = A == 0;
}
void ora(uint8_t value) {
    A |= value;
    flagNegative = (A & 0x80);
    flagZero = A == 0;
}
void eor(uint8_t value) {
    A ^= value;
    flagNegative = (A & 0x80);
    flagZero = A == 0;
}
void op_bit(uint8_t value) {
    flagZero = (A & value) == 0x00;
    flagNegative = (value & 0x80);
    flagOverflow = (value & 0x40);     // next to highest bit
}
void asl(uint16_t address) {
    uint8_t value = readByte(address);
    // shift left by one place, and mask the bottom bit out to ensure it's zero.
    uint8_t ret = (value << 1) & 0xFE;
    // set carry if high bit was set, clear it otherwise.
    // coud do this before shift, but there is no particular benefit.
    flagCarry = (value & 0x80);
    flagNegative = (ret & 0x80);
    flagZero = ret == 0x00;
    writeByte(address, ret);
}
void lsr(uint16_t address) {
    uint8_t value = readByte(address);
    // shift left by one place, and mask the bottom bit out to ensure it's zero.
    uint8_t ret = (value >> 1) & 0x7F;
    // set carry if low bit was set, clear it otherwise.
    flagCarry = (value & 0x01);
    // zero is shifted in, so N is always cleared
    flagNegative = false;
    flagZero = ret == 0x00;
    writeByte(address, ret);
}
void rol(uint16_t address) {
    uint8_t value = readByte(address);
    // shift left by one place, mask the bottom bit out, and add in the carry
    uint8_t ret = ((value << 1) & 0xFE) + (flagCarry?1:0);
    // set carry if high bit was set, clear it otherwise
    flagCarry = (value & 0x80);
    flagNegative = (ret & 0x80);
    flagZero = ret == 0x00;
    writeByte(address, ret);
}
void ror(uint16_t address) {
    uint8_t value = readByte(address);
    // shift left by one place, mask the bottom bit out, and add in the carry
    uint8_t ret = ((value >> 1) & 0x7F) + (flagCarry?0x80:0x00);
    // set carry if high bit was set, clear it otherwise
    flagCarry = (value & 0x01);
    flagNegative = (ret & 0x80);
    flagZero = ret == 0x00;
    writeByte(address, ret);
}
void trb(uint16_t address) {
    uint8_t value = readByte(address);
    flagZero = (A & value) == 0x00;
    value = ~A & value;
    writeByte(address, value);
}
void tsb(uint16_t address) {
    uint8_t value = readByte(address);
    flagZero = (A & value) == 0x00;
    value = A | value;
    writeByte(address, value);
}
void rmb(int bit) {
    uint16_t address = getZPAddr();
    
    // read the byte at the memory location, and mask out the bit specified.
    uint8_t data = readByte(address) & ~(0x01 << bit);
    writeByte(address, data);
}
void smb(int bit) {
    uint16_t address = getZPAddr();
    
    // read the byte at the memory location, and mask in the bit specified.
    uint8_t data = readByte(address) | (0x01 << bit);
    writeByte(address, data);
}

void bbs(int bit) {
    uint8_t data = readByte(getZPAddr()) & (0x01 << bit);
    if (data != 0) {
        branch(readByte(programCounter++));
    } else {
        programCounter++;
    }
}
void bbr(int bit) {
    uint8_t data = readByte(getZPAddr()) & (0x01 << bit);
    if (data == 0) {
        branch(readByte(programCounter++));
    } else {
        programCounter++;
    }
}

void lda(uint8_t value) {
    A = value;
    flagZero = (A == 0x00);
    flagNegative = (A & 0x80);
}
void ldx(uint8_t value) {
    X = value;
    flagZero = (X == 0x00);
    flagNegative = (X & 0x80);
}
void ldy(uint8_t value) {
    Y = value;
    flagZero = (Y == 0x00);
    flagNegative = (Y & 0x80);
}
// Register stores are implemented directly.

// Processes a single 6502 instruction.
void do6502() {
    // Deal with the special case first:
    //  A previously-executed STP instruction.
    // Interrupts and coming out of a WAI are handled at the end of this function
    //  A raised NMI or IRQ, or
    //  A previously-executed WAI instruction.
    
    // If a STP has previously been executed, do nothing.
    //  IRQ and NMI have no effect on STP.
    if (hitSTP) return;
        
    uint8_t opcode = readByte(programCounter++); // move PC to the byte after the instruction
    
    switch (opcode) {
        // Control instructions
        case OP_BRK:
            // BRK is not affected by the I flag.
          //printf("BRK @ PC=$%04X\n", programCounter);
            programCounter++; // skip the signature byte.
            flagBRK = true;
            doInterrupt(IRQ_VEC);
          //printf("PC changed by BRK to %04X\n", programCounter);
            break;
        case OP_RTI:    // Ignores the BRK bit in real hardware
            pullStatus();
            pullPC();
          //printf("PC changed by RTI to %04X\n", programCounter);
            break;
        case OP_JSR:
            // PC points to the byte after the instruction, so the second byte of the JSR.
            // JSR pushes the address of the third byte.
            programCounter++;
            pushPC();
            programCounter = readShort(programCounter-1);
            break;
        case OP_RTS:
            pullPC();
            programCounter++;
            break;
        case OP_WAI:
            hitWAI = true;
            break;
        case OP_STP:
            hitSTP = true;
            break;
        case OP_NOP:
            // perhaps insert a deliberate time-delay here if on an arduino.
            break;
        /**********************
         * Stack instructions *
         **********************/
        case OP_PHA:
            pushByte(A);
            break;
        case OP_PLA:
            A = pullByte();
            flagNegative = (A & 0x80);
            flagZero = A == 0x00;
            break;
        case OP_PHX:
            pushByte(X);
            break;
        case OP_PLX:
            X = pullByte();
            flagNegative = (X & 0x80);
            flagZero = X == 0x00;
            break;
        case OP_PHY:
            pushByte(Y);
            break;
        case OP_PLY:
            Y = pullByte();
            flagNegative = (Y & 0x80);
            flagZero = Y == 0x00;
            break;
        case OP_PHP:
            pushStatus();
            break;
        case OP_PLP:
            pullStatus();
            break;
        /****************
         * Flag-setting *
         ****************/
        case OP_SEC:
            flagCarry = true;
            break;
        case OP_CLC:
            flagCarry = false;
            break;
        case OP_SEI:
            flagIRQdisable = true;
            break;
        case OP_CLI:
            flagIRQdisable = false;
            break;
        case OP_CLV:
            flagOverflow = false;
            break;
        case OP_SED:
            flagDecimal = true;
            break;
        case OP_CLD:
            flagDecimal = false;
            break;
        #include "branches-jumps.h"
        #include "load-store.h"
        #include "add-subtract.h"
        #include "logic-ops.h"
        #include "undefined.h"
    }
    
    if (NMIraised) {
        // process NMI
        NMIraised = false;
        doInterrupt(NMI_VEC);
      //printf("PC changed by NMI to %04X\n", programCounter);
    } else if (IRQraised && !flagIRQdisable) {
        // process IRQ
        // BRK is handled seperatedly, bypassing this handler.
        doInterrupt(IRQ_VEC);
      //printf("PC changed by IRQ to %04X\n", programCounter);
    }
    
    // If we are currently in a WAI instruction, and neither an IRQ or an NMI
    //  have occurred, do nothing. (Hitting an IRQ or NMI will clear this)
    if (hitWAI) {
        return;
    }
}

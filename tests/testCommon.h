// testCommon.h

#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <stdio.h>
#include <stdbool.h>

#include "../simulieren-6502.h"
#include "../opcodes.h"

uint16_t resetVal;
uint16_t IRQVal;
uint16_t NMIVal;

// allocate 512 bytes of RAM for zero page and the stack.
#define RAM_SIZE 512
uint8_t memory[RAM_SIZE];

// 6502 registers and flags
extern uint8_t A;
extern uint8_t X, Y;
extern uint8_t stackPointer;
extern uint16_t programCounter;

extern bool flagNegative;  // Indicates result negative(bit 7 set).
extern bool flagOverflow;  // Indicates two's-complement arithmetic overflow.
extern bool flagBRK;       // Indicates that the last interrupt was caused by a BRK instruction.
extern bool flagDecimal;   // Indicates whether decimal mode is set.
extern bool flagIRQdisable;// Indicates whether the emulated 6502 responds to IRQs. NMIs are unaffected.
extern bool flagZero;      // Indicates result zero.
extern bool flagCarry;     // Indicates carry out of an addition or borrow out of a subtraction.

extern bool hitWAI;
extern bool hitSTP;
extern bool IRQraised;
extern bool NMIraised;

// this is the primary purpose of having this module...
bool logging = false;

#ifndef OVERRIDE_MEM_ACCESS
uint8_t readByte(uint16_t address){
    uint8_t ret = 0;
    if(logging) printf("[6502] Read from address $%04X", address);
    
    if (address < RAM_SIZE) {
        ret = memory[address];
        if(logging) printf(" - returned $%02X from memory\n", ret);
    } else if (address == RESET_VEC) {
        ret = resetVal & 0x00FF;
        if(logging) printf(" - returned $%02X from Reset Vector Low\n", ret);
    } else if (address == (RESET_VEC + 1)) {
        ret = (resetVal >> 8) & 0x00FF;
        if(logging) printf(" - returned $%02X from Reset Vector High\n", ret);
    } else if (address == IRQ_VEC) {
        ret = IRQVal & 0x00FF;
        if(logging) printf(" - returned $%02X from Reset Vector Low\n", ret);
    } else if (address == (IRQ_VEC + 1)) {
        ret = (IRQVal >> 8) & 0x00FF;
        if(logging) printf(" - returned $%02X from Reset Vector High\n", ret);
    } else if (address == NMI_VEC) {
        ret = NMIVal & 0x00FF;
        if(logging) printf(" - returned $%02X from Reset Vector Low\n", ret);
    } else if (address == (NMI_VEC + 1)) {
        ret = (NMIVal >> 8) & 0x00FF;
        if(logging) printf(" - returned $%02X from Reset Vector High\n", ret);
    } else {
        if(logging) printf(" - returned $%02X from default value\n", ret);
    }
	
    return ret;
}
void writeByte(uint16_t address, uint8_t data){
    if(logging) printf("[6502] Write to address %04X with data %02X", address, data);
    if (address < RAM_SIZE) {
        if(logging) printf(" - Success!\n");
        memory[address] = data;
    } else {
        if(logging) printf(" - FAILURE: Memory location does not exist.\n");
    }
}
#endif //#ifndef OVERRIDE_MEM_ACCESS

#endif //ifndef TEST_COMMON_H

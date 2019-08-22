// testAddrModes.c - addressing mode resolver tests
// These functions pull an address from the address pointed to by the program
//  counter, incrementing it as they go. These are approximately half of each
//  instruction. There are a few special cases that do not use these resolvers,
//  but these are usually things like immediate and accumulator addressing
//  modes.

#include <stdio.h>  // for printf

#include "testCommon.h"

// expose the addressing mode resolvers in here so we can call them.
extern uint16_t getABSAddr();       //0
extern uint16_t getABS_XAddr();     //1
extern uint16_t getABS_YAddr();     //2
extern uint16_t getABS_INDAddr();   //3
extern uint16_t getABS_X_INDAddr(); //4
extern uint16_t getZPAddr();        //5
extern uint16_t getZP_XAddr();      //6
extern uint16_t getZP_YAddr();      //7
extern uint16_t getZP_INDAddr();    //8
extern uint16_t getZP_X_INDAddr();  //9
extern uint16_t getZP_IND_YAddr();  //10

#define NUM_TESTS 11

#define RESET_TARGET 0x0000
#define ABS_TARGET   0x0100
#define IND_TARGET   0x0050
#define ZP_TARGET    0x00F0
#define XY_VAL 0x20

bool addrResults[NUM_TESTS];   // whether or not the correct address was generated
bool stateResults[NUM_TESTS];  // whether or not the state was altered in the correct manner

bool checkState(uint8_t X_val = 0x00, uint8_t Y_val = 0x00) {
    // Check that no state is altered, except PC
    bool state = true;
    
    // Some of these might need to be altered
    if (X != X_val)             state = false;
    if (Y != Y_val)             state = false;
    
    // A, the flags, and the stack pointer should never be changed
    if (A != 0x00)              state = false;
    if (stackPointer != 0xFF)   state = false;
    if (flagNegative != false)  state = false;
    if (flagOverflow != false)  state = false;
    if (flagBRK != false)       state = false;
    if (flagZero != true)       state = false;
    if (flagCarry != false)     state = false;
    if (flagDecimal != false)   state = false;
    if (flagIRQdisable != true) state = false;
    return state;
}

void doTest(int testNum, uint16_t resolver(), uint16_t target, uint8_t Xval = 0x00, uint8_t Yval = 0x00) {
    // Run an addressing mode resolver
    uint16_t addr = resolver();
    addrResults[testNum] = (addr == target);
    stateResults[testNum] = checkState(Xval, Yval);
}

void printResults(bool addrs[], bool states[]) {
    printf("Addr Mode\tPC \tstate \n");
    printf("ABS \t\t%i \t%i\n", addrs[0], states[0]);
    printf("ABS, X \t\t%i \t%i\n", addrs[1], states[1]);
    printf("ABS, Y \t\t%i \t%i\n", addrs[2], states[2]);
    printf("(ABS) \t\t%i \t%i\n", addrs[3], states[3]);
    printf("(ABS, X) \t%i \t%i\n", addrs[4], states[4]);
    printf("ZP \t\t%i \t%i\n", addrs[5], states[5]);
    printf("ZP, X \t\t%i \t%i\n", addrs[6], states[6]);
    printf("ZP, Y \t\t%i \t%i\n", addrs[7], states[7]);
    printf("(ZP) \t\t%i \t%i\n", addrs[8], states[8]);
    printf("(ZP, X) \t%i \t%i\n", addrs[9], states[9]);
    printf("(ZP), Y \t%i \t%i\n", addrs[10], states[10]);
    
    //printf("Key: 1 = pass, 0 = fail\n");
    
    bool overallResult = true;
    for (int i = 0; i < NUM_TESTS; i++) {
        if (!(addrs[i] && states[i])) {
            overallResult = false;
        }
    }
    if (overallResult) {
        printf("All tests passed!\n");
    } else {
        printf("Falied tests!\n");
    }
}

int main() {
    resetVal = RESET_TARGET;
    
    //For each test:
    // Init 6502 to predictable state.
    // This predictable state is:
    // PC == resetVal;
    // A == 0x00;
    // X == 0x00;
    // Y == 0x00;
    // stackPointer == 0xFF;
    // flagNegative == false;
    // flagOverflow == false;
    // flagBRK == false;
    // flagZero == true;
    // flagCarry == false;
    // flagDecimal == false;
    // flagIRQdisable == true;
    
    /* ABS */
    reset6502(false);
    // set up memory (and registers if necessary) with the target address
    memory[RESET_TARGET]   = ABS_TARGET & 0x00FF;  // basic absolute address
    memory[RESET_TARGET+1] = (ABS_TARGET >> 8) & 0x00FF;
    doTest(0, getABSAddr, ABS_TARGET);
    //ABS Address stays in place for all the absolute addresses.
    
    /* ABS, X */
    reset6502(false);
    X = XY_VAL;
    // Run an addressing mode resolver
    doTest(1, getABS_XAddr, ABS_TARGET + XY_VAL, XY_VAL);
    
    /* ABS, Y */
    reset6502(false);
    Y = XY_VAL;
    // Run an addressing mode resolver
    doTest(2, getABS_YAddr, ABS_TARGET + XY_VAL, 0x00, XY_VAL);
    
    /* (ABS) */
    reset6502(false);
    // set up memory (and registers if necessary) with the target address
    memory[ABS_TARGET]   = IND_TARGET & 0x00FF;
    memory[ABS_TARGET+1] = (IND_TARGET >> 8) & 0x00FF;
    doTest(3, getABS_INDAddr, IND_TARGET);
    // prevent subsequent indirect addressing modes from cheating
    memory[ABS_TARGET]   = 0x00;
    memory[ABS_TARGET+1] = 0x00;
    
    /* (ABS, X) */
    reset6502(false);
    // set up memory (and registers if necessary) with the target address
    X = XY_VAL;
    memory[ABS_TARGET+XY_VAL]   = IND_TARGET & 0x00FF;
    memory[ABS_TARGET+XY_VAL+1] = (IND_TARGET >> 8) & 0x00FF;
    doTest(4, getABS_X_INDAddr, IND_TARGET, XY_VAL);
    
    memory[ABS_TARGET+XY_VAL]   = 0x00;
    memory[ABS_TARGET+XY_VAL+1] = 0x00;
    
    // prevent ZP addressing modes from picking up the ABS address.
    memory[RESET_TARGET]   = 0x00;
    memory[RESET_TARGET+1] = 0x00;
    
    /* ZP */
    reset6502(false);
    // set up memory (and registers if necessary) with the target address
    memory[RESET_TARGET] = ZP_TARGET & 0x00FF;
    doTest(5, getZPAddr, ZP_TARGET);
    
    /* ZP, X */
    reset6502(false);
    X = XY_VAL;
    // Run an addressing mode resolver
    doTest(6, getZP_XAddr, ZP_TARGET + XY_VAL, XY_VAL);
    
    /* ZP, Y */
    reset6502(false);
    Y = XY_VAL;
    // Run an addressing mode resolver
    doTest(7, getZP_YAddr, ZP_TARGET + XY_VAL, 0x00, XY_VAL);
    
    /* (ZP) */
    reset6502(false);
    // set up memory (and registers if necessary) with the target address
    memory[ZP_TARGET]   = IND_TARGET & 0x00FF;
    memory[ZP_TARGET+1] = (IND_TARGET >> 8) & 0x00FF;
    doTest(8, getZP_INDAddr, IND_TARGET);
    
    /* (ZP, X) */
    reset6502(false);
    // set up memory (and registers if necessary) with the target address
    X = XY_VAL;
    memory[ZP_TARGET+XY_VAL]   = IND_TARGET & 0x00FF;
    memory[ZP_TARGET+XY_VAL+1] = (IND_TARGET >> 8) & 0x00FF;
    doTest(9, getZP_X_INDAddr, IND_TARGET, XY_VAL);
    
    /* (ZP), Y */
    reset6502(false);
    // set up memory (and registers if necessary) with the target address
    Y = XY_VAL;
    doTest(10, getZP_IND_YAddr, IND_TARGET + XY_VAL, 0x00, XY_VAL);
    
    printResults(addrResults, stateResults);
}

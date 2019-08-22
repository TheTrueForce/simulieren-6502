#ifndef H6502SIM_H
#define H6502SIM_H

#include <stdbool.h>
#include <stdint.h>

#define IRQ_VEC 0xFFFE
#define RESET_VEC 0xFFFC
#define NMI_VEC 0xFFFA

// resets the emulated 6502.
// Parameters:
//  bool faithful   If true, reset the simulated procesor in the same way as
//                   real hardware.
//                  If false, initialise the user registers to zero, the stack
//                   pointer to 0xFF(the top of the stack region), and all the
//                   flags(N=0, V=0, B=0, Z=1, C=0).
void reset6502(bool faithful);

// Processes a single 6502 instruction.
void do6502();

// This function indicates to the simulated processor that an interrupt is
//  awaiting service.
// The processor will execute the next instruction, and then begin executing the
//  ISR on the instruction after that.
void raiseIRQ();

// This function indicates to the simulated processor that no interrupts are
//  awaiting service.
void lowerIRQ();

// This function indicates to the simulated processor that a non-maskable
//  interrupt has occurred.
// There is no lowerNMI() because on a real 65c02, the NMI input is
//  active-edge-sensitive. In this simulation, when the NMI is picked up by the
//  65c02(as with the IRQ), an NMI is no longer considered to be waiting.
void raiseNMI();

// Sets the Overflow(V) flag, in much the same way the /SO (Set Overflow) pin on
//  real hardware would.
void setOverflow();

#endif // ifndef H6502SIM_H

#ifndef OPCODES_H
#define OPCODES_H

//key to codes:
//  I - Implemented
//  S - needs Special testing
//  N - needs No specific testing
//  T - Tested

// Add with Carry
#define OP_ADC_IMM      0x69 /* I */
#define OP_ADC_ABS      0x6D /* I */
#define OP_ADC_ZP       0x65 /* I */
#define OP_ADC_ZP_IND   0x72 /* I */
#define OP_ADC_ABS_X    0x7D /* I */
#define OP_ADC_ABS_Y    0x79 /* I */
#define OP_ADC_ZP_X     0x75 /* I */
#define OP_ADC_ZP_X_IND 0x61 /* I */
#define OP_ADC_ZP_IND_Y 0x71 /* I */

// And Accumulator with Memory
#define OP_AND_IMM      0x29 /* I */
#define OP_AND_ABS      0x2D /* I */
#define OP_AND_ZP       0x25 /* I */
#define OP_AND_ZP_IND   0x32 /* I */
#define OP_AND_ABS_X    0x3D /* I */
#define OP_AND_ABS_Y    0x39 /* I */
#define OP_AND_ZP_X     0x35 /* I */
#define OP_AND_ZP_X_IND 0x21 /* I */
#define OP_AND_ZP_IND_Y 0x31 /* I */

// Shift Memory or Accumulator Left
#define OP_ASL_ACC      0x0A /* IS */
#define OP_ASL_ABS      0x0E /* I */
#define OP_ASL_ZP       0x06 /* I */
#define OP_ASL_ABS_X    0x1E /* I */
#define OP_ASL_ZP_X     0x16 /* I */

// Test Memory Bits against Accumulator
#define OP_BIT_IMM      0x89 /* IS */
#define OP_BIT_ABS      0x2C /* I */
#define OP_BIT_ZP       0x24 /* I */
#define OP_BIT_ABS_X    0x3C /* I */
#define OP_BIT_ZP_X     0x34 /* I */

// Compare Accumulator with Memory
#define OP_CMP_IMM      0xC9 /* I */
#define OP_CMP_ABS      0xCD /* I */
#define OP_CMP_ZP       0xC5 /* I */
#define OP_CMP_ABS_X    0xDD /* I */
#define OP_CMP_ABS_Y    0xD9 /* I */
#define OP_CMP_ZP_X     0xD5 /* I */
#define OP_CMP_ZP_X_IND 0xC1 /* I */
#define OP_CMP_ZP_IND_Y 0xD1 /* I */

// Compare Index Register X with Memory
#define OP_CPX_IMM      0xE0 /* I */
#define OP_CPX_ABS      0xEC /* I */
#define OP_CPX_ZP       0xE4 /* I */

// Compare Index Register X with Memory
#define OP_CPY_IMM      0xC0 /* I */
#define OP_CPY_ABS      0xCC /* I */
#define OP_CPY_ZP       0xC4 /* I */

// Decrement
#define OP_DEC_ACC      0x3A /* IS */
#define OP_DEC_ABS      0xCE /* I */
#define OP_DEC_ZP       0xC6 /* I */
#define OP_DEC_ABS_X    0xDE /* I */
#define OP_DEC_ZP_X     0xD6 /* I */

// Index register inc/dec
#define OP_INX          0xE8 /* IS */
#define OP_DEX          0xCA /* IS */
#define OP_INY          0xC8 /* IS */
#define OP_DEY          0x88 /* IS */

// Exclusive-OR Accumulator with Memory
#define OP_EOR_IMM      0x49 /* I */
#define OP_EOR_ABS      0x4D /* I */
#define OP_EOR_ZP       0x45 /* I */
#define OP_EOR_ZP_IND   0x52 /* I */
#define OP_EOR_ABS_X    0x5D /* I */
#define OP_EOR_ABS_Y    0x59 /* I */
#define OP_EOR_ZP_X     0x55 /* I */
#define OP_EOR_ZP_X_IND 0x41 /* I */
#define OP_EOR_ZP_IND_Y 0x51 /* I */

// Increment
#define OP_INC_ACC      0x1A /* IS */
#define OP_INC_ABS      0xEE /* I */
#define OP_INC_ZP       0xE6 /* I */
#define OP_INC_ABS_X    0xFE /* I */
#define OP_INC_ZP_X     0xF6 /* I */

// Jump
#define OP_JMP_ABS      0x4C /* I */
#define OP_JMP_ABS_IND  0x6C /* I */
#define OP_JMP_ABS_X_IND 0x7C /* I */

// Load Accumulator from Memory
#define OP_LDA_IMM      0xA9 /* I */
#define OP_LDA_ABS      0xAD /* I */
#define OP_LDA_ZP       0xA5 /* I */
#define OP_LDA_ABS_X    0xBD /* I */
#define OP_LDA_ABS_Y    0xB9 /* I */
#define OP_LDA_ZP_X     0xB5 /* I */
#define OP_LDA_ZP_X_IND 0xA1 /* I */
#define OP_LDA_ZP_IND_Y 0xB1 /* I */

// Load Index Register X from Memory
#define OP_LDX_IMM      0xA2 /* I */
#define OP_LDX_ABS      0xAE /* I */
#define OP_LDX_ZP       0xA6 /* I */
#define OP_LDX_ABS_Y    0xBE /* I */
#define OP_LDX_ZP_Y     0xB6 /* I */

// Load Index Register Y from Memory
#define OP_LDY_IMM      0xA0 /* I */
#define OP_LDY_ABS      0xAC /* I */
#define OP_LDY_ZP       0xA4 /* I */
#define OP_LDY_ABS_X    0xBC /* I */
#define OP_LDY_ZP_X     0xB4 /* I */

// Logical Shift Memory or Accumulator Right
#define OP_LSR_ACC      0x4A /* IS */
#define OP_LSR_ABS      0x4E /* I */
#define OP_LSR_ZP       0x46 /* I */
#define OP_LSR_ABS_X    0x5E /* I */
#define OP_LSR_ZP_X     0x56 /* I */

// OR Accumulator with Memory
#define OP_ORA_IMM      0x09 /* I */
#define OP_ORA_ABS      0x0D /* I */
#define OP_ORA_ZP       0x05 /* I */
#define OP_ORA_ZP_IND   0x12 /* I */
#define OP_ORA_ABS_X    0x1D /* I */
#define OP_ORA_ABS_Y    0x19 /* I */
#define OP_ORA_ZP_X     0x15 /* I */
#define OP_ORA_ZP_X_IND 0x01 /* I */
#define OP_ORA_ZP_IND_Y 0x11 /* I */

// Rotate Memory or Accumulator Left
#define OP_ROL_ACC      0x2A /* IS */
#define OP_ROL_ABS      0x2E /* I */
#define OP_ROL_ZP       0x26 /* I */
#define OP_ROL_ABS_X    0x3E /* I */
#define OP_ROL_ZP_X     0x36 /* I */

// Rotate Memory or Accumulator Right
#define OP_ROR_ACC      0x6A /* IS */
#define OP_ROR_ABS      0x6E /* I */
#define OP_ROR_ZP       0x66 /* I */
#define OP_ROR_ABS_X    0x7E /* I */
#define OP_ROR_ZP_X     0x76 /* I */

// Subtract with Borrow from Accumulator
#define OP_SBC_IMM      0xE9 /* I */
#define OP_SBC_ABS      0xED /* I */
#define OP_SBC_ZP       0xE5 /* I */
#define OP_SBC_ZP_IND   0xF2 /* I */
#define OP_SBC_ABS_X    0xFD /* I */
#define OP_SBC_ABS_Y    0xF9 /* I */
#define OP_SBC_ZP_X     0xF5 /* I */
#define OP_SBC_ZP_X_IND 0xE1 /* I */
#define OP_SBC_ZP_IND_Y 0xF1 /* I */

// Store Accumulator to Memory
#define OP_STA_ABS      0x8D /* IN */
#define OP_STA_ZP       0x85 /* IN */
#define OP_STA_ZP_IND   0x92 /* IN */
#define OP_STA_ABS_X    0x9D /* IN */
#define OP_STA_ABS_Y    0x99 /* IN */
#define OP_STA_ZP_X     0x95 /* IN */
#define OP_STA_ZP_X_IND 0x81 /* IN */
#define OP_STA_ZP_IND_Y 0x91 /* IN */

// Store Index Register X to Memory
#define OP_STX_ABS      0x8E /* IN */
#define OP_STX_ZP       0x86 /* IN */
#define OP_STX_ZP_Y     0x96 /* IN */

// Store Index Register Y to Memory
#define OP_STY_ABS      0x8C /* IN */
#define OP_STY_ZP       0x84 /* IN */
#define OP_STY_ZP_X     0x94 /* IN */

// Store Zero to Memory
#define OP_STZ_ABS      0x9C /* IN */
#define OP_STZ_ZP       0x64 /* IN */
#define OP_STZ_ABS_X    0x9E /* IN */
#define OP_STZ_ZP_X     0x74 /* IN */

// Test and Set/Reset Bits
#define OP_TRB_ZP       0x04 /* I */
#define OP_TRB_ABS      0x0C /* I */
#define OP_TSB_ZP       0x14 /* I */
#define OP_TSB_ABS      0x1C /* I */

// Branch instructions
#define OP_BPL          0x10 /* I */
#define OP_BMI          0x30 /* I */
#define OP_BVC          0x50 /* I */
#define OP_BVS          0x70 /* I */
#define OP_BCC          0x90 /* I */
#define OP_BCS          0xB0 /* I */
#define OP_BNE          0xD0 /* I */
#define OP_BEQ          0xF0 /* I */

// Control instructions
#define OP_BRK          0x00 /* I */
#define OP_JSR          0x20 /* I */
#define OP_RTI          0x40 /* I */
#define OP_RTS          0x60 /* I */
#define OP_BRA          0x80 /* I */
#define OP_WAI          0xCB /* IS */
#define OP_STP          0xDB /* IS */
#define OP_NOP          0xEA /* I */

// Flag-setting instructions
#define OP_CLC          0x18 /* I */
#define OP_SEC          0x38 /* I */
#define OP_CLI          0x58 /* I */
#define OP_SEI          0x78 /* I */
#define OP_CLV          0xB8 /* I */
#define OP_CLD          0xD8 /* I */
#define OP_SED          0xF8 /* I */

// Register-Transfer instructions
#define OP_TYA          0x98 /* I */
#define OP_TAY          0xA8 /* I */
#define OP_TXS          0x9A /* I */
#define OP_TSX          0xBA /* I */
#define OP_TAX          0xAA /* I */
#define OP_TXA          0x8A /* I */

// Stack instructions
#define OP_PHA          0x48 /* I */
#define OP_PLA          0x68 /* I */
#define OP_PHP          0x08 /* I */
#define OP_PLP          0x28 /* I */
#define OP_PHX          0xDA /* I */
#define OP_PLX          0xFA /* I */
#define OP_PHY          0x5A /* I */
#define OP_PLY          0x7A /* I */

/***************************
 * Rockwell R65C02 opcodes *
 ***************************/
// Branch on Bit Reset
#define OP_BBR0         0x0F /* I */
#define OP_BBR1         0x1F /* I */
#define OP_BBR2         0x2F /* I */
#define OP_BBR3         0x3F /* I */
#define OP_BBR4         0x4F /* I */
#define OP_BBR5         0x5F /* I */
#define OP_BBR6         0x6F /* I */
#define OP_BBR7         0x7F /* I */

// Branch on Bit Set
#define OP_BBS0         0x8F /* I */
#define OP_BBS1         0x9F /* I */
#define OP_BBS2         0xAF /* I */
#define OP_BBS3         0xBF /* I */
#define OP_BBS4         0xCF /* I */
#define OP_BBS5         0xDF /* I */
#define OP_BBS6         0xEF /* I */
#define OP_BBS7         0xFF /* I */

// Reset Memory Bits
#define OP_RMB0         0x07 /* I */
#define OP_RMB1         0x17 /* I */
#define OP_RMB2         0x27 /* I */
#define OP_RMB3         0x37 /* I */
#define OP_RMB4         0x47 /* I */
#define OP_RMB5         0x57 /* I */
#define OP_RMB6         0x67 /* I */
#define OP_RMB7         0x77 /* I */

// Set Memory Bits
#define OP_SMB0         0x87 /* I */
#define OP_SMB1         0x97 /* I */
#define OP_SMB2         0xA7 /* I */
#define OP_SMB3         0xB7 /* I */
#define OP_SMB4         0xC7 /* I */
#define OP_SMB5         0xD7 /* I */
#define OP_SMB6         0xE7 /* I */
#define OP_SMB7         0xF7 /* I */

#endif // ifndef OPCODES_H

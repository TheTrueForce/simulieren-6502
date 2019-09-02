// logic-ops.cfragment - AND, ORA, EOR

/*******************************
 * AND Accumulator with Memory *
 *******************************/
case OP_AND_IMM:
    op_and(readByte(programCounter++));
    break;
case OP_AND_ABS:
    op_and(readByte(getABSAddr()));
    break;
case OP_AND_ZP:
    op_and(readByte(getZPAddr()));
    break;
case OP_AND_ZP_IND:
    op_and(readByte(getZP_INDAddr()));
    break;
case OP_AND_ABS_X:
    op_and(readByte(getABS_XAddr()));
    break;
case OP_AND_ABS_Y:
    op_and(readByte(getABS_YAddr()));
    break;
case OP_AND_ZP_X:
    op_and(readByte(getZP_XAddr()));
    break;
case OP_AND_ZP_X_IND:
    op_and(readByte(getZP_X_INDAddr()));
    break;
case OP_AND_ZP_IND_Y:
    op_and(readByte(getZP_IND_YAddr()));
    break;

/******************************
 * OR Accumulator with Memory *
 ******************************/
case OP_ORA_IMM:
    ora(readByte(programCounter++));
    break;
case OP_ORA_ABS:
    ora(readByte(getABSAddr()));
    break;
case OP_ORA_ZP:
    ora(readByte(getZPAddr()));
    break;
case OP_ORA_ZP_IND:
    ora(readByte(getZP_INDAddr()));
    break;
case OP_ORA_ABS_X:
    ora(readByte(getABS_XAddr()));
    break;
case OP_ORA_ABS_Y:
    ora(readByte(getABS_YAddr()));
    break;
case OP_ORA_ZP_X:
    ora(readByte(getZP_XAddr()));
    break;
case OP_ORA_ZP_X_IND:
    ora(readByte(getZP_X_INDAddr()));
    break;
case OP_ORA_ZP_IND_Y:
    ora(readByte(getZP_IND_YAddr()));
    break;

/*******************************
 * XOR Accumulator with Memory *
 *******************************/
case OP_EOR_IMM:
    eor(readByte(programCounter++));
    break;
case OP_EOR_ABS:
    eor(readByte(getABSAddr()));
    break;
case OP_EOR_ZP:
    eor(readByte(getZPAddr()));
    break;
case OP_EOR_ZP_IND:
    eor(readByte(getZP_INDAddr()));
    break;
case OP_EOR_ABS_X:
    eor(readByte(getABS_XAddr()));
    break;
case OP_EOR_ABS_Y:
    eor(readByte(getABS_YAddr()));
    break;
case OP_EOR_ZP_X:
    eor(readByte(getZP_XAddr()));
    break;
case OP_EOR_ZP_X_IND:
    eor(readByte(getZP_X_INDAddr()));
    break;
case OP_EOR_ZP_IND_Y:
    eor(readByte(getZP_IND_YAddr()));
    break;
/****************************************
 * Test memory bits against accumulator *
 ****************************************/
case OP_BIT_IMM:
    // Immediate mode only affects the Z flag, and bit() does N,V, and Z
    flagZero = (A & readByte(programCounter++)) == 0x00;
    break;
case OP_BIT_ABS:
    op_bit(readByte(getABSAddr()));
    break;
case OP_BIT_ZP:
    op_bit(readByte(getZPAddr()));
    break;
case OP_BIT_ABS_X:
    op_bit(readByte(getABS_XAddr()));
    break;
case OP_BIT_ZP_X:
    op_bit(readByte(getZP_XAddr()));
    break;
/*************************
 * Arithmetic Shift Left *
 *************************/
case OP_ASL_ACC:
    {
    uint8_t value = A;
    // shift left by one place, and mask the bottom bit out to ensure it's zero.
    A = (value << 1) & 0xFE;
    // set carry if high bit was set, clear it otherwise
    flagCarry = (value & 0x80) != 0;
    flagNegative = (A & 0x80) != 0;
    flagZero = A == 0x00;
    }
    break;
case OP_ASL_ABS:
    asl(getABSAddr());
    break;
case OP_ASL_ZP:
    asl(getZPAddr());
    break;
case OP_ASL_ABS_X:
    asl(getABS_XAddr());
    break;
case OP_ASL_ZP_X:
    asl(getZP_XAddr());
    break;
/***********************
 * Logical Shift Right *
 ***********************/
case OP_LSR_ACC:
    {
    uint8_t value = A;
    // shift left by one place, and mask out the top bit
    A = (value >> 1) & 0x7F;
    // low bit moves into carry
    flagCarry = (value & 0x01) != 0;
    // zero is shifted in, so N is always cleared
    flagNegative = false;
    flagZero = A == 0x00;
    }
    break;
case OP_LSR_ABS:
    lsr(getABSAddr());
    break;
case OP_LSR_ZP:
    lsr(getZPAddr());
    break;
case OP_LSR_ABS_X:
    lsr(getABS_XAddr());
    break;
case OP_LSR_ZP_X:
    lsr(getZP_XAddr());
    break;
/***************
 * Rotate Left *
 ***************/
case OP_ROL_ACC:
    {
    uint8_t value = A;
    // shift left by one place, mask the bottom bit out, and add in the carry
    A = ((value << 1) & 0xFE) + (flagCarry?1:0);
    // set carry if high bit was set, clear it otherwise
    flagCarry = (value & 0x80) != 0;
    flagNegative = (A & 0x80) != 0;
    flagZero = A == 0x00;
    }
    break;
case OP_ROL_ABS:
    rol(getABSAddr());
    break;
case OP_ROL_ZP:
    rol(getZPAddr());
    break;
case OP_ROL_ABS_X:
    rol(getABS_XAddr());
    break;
case OP_ROL_ZP_X:
    rol(getZP_XAddr());
    break;
/****************
 * Rotate Right *
 ****************/
case OP_ROR_ACC:
    {
    uint8_t value = A;
    // shift right by one place, mask the bottom bit out, and add in the carry
    A = ((value >> 1) & 0x7F) + (flagCarry?0x80:0x00);
    // set carry if high bit was set, clear it otherwise
    flagCarry = (value & 0x01) != 0;
    flagNegative = (A & 0x80) != 0;
    flagZero = A == 0x00;
    }
    break;
case OP_ROR_ABS:
    ror(getABSAddr());
    break;
case OP_ROR_ZP:
    ror(getZPAddr());
    break;
case OP_ROR_ABS_X:
    ror(getABS_XAddr());
    break;
case OP_ROR_ZP_X:
    ror(getZP_XAddr());
    break;
/***************************
 * Test and Set/Reset Bits *
 ***************************/
case OP_TRB_ZP:
    trb(getZPAddr());
    break;
case OP_TRB_ABS:
    trb(getABSAddr());
    break;
case OP_TSB_ZP:
    tsb(getZPAddr());
    break;
case OP_TSB_ABS:
    tsb(getABSAddr());
    break;

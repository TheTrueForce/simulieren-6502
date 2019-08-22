// add-subtract.cfragment - handles opcodes that add and subtract from the user
//  registers.

/******************
 * Add with Carry *
 ******************/
case OP_ADC_IMM:
    adc(readByte(programCounter++));
    break;
case OP_ADC_ABS:
    adc(readByte(getABSAddr()));
    break;
case OP_ADC_ZP:
    adc(readByte(getZPAddr()));
    break;
case OP_ADC_ZP_IND:
    adc(readByte(getZP_INDAddr()));
    break;
case OP_ADC_ABS_X:
    adc(readByte(getABS_XAddr()));
    break;
case OP_ADC_ABS_Y:
    adc(readByte(getABS_YAddr()));
    break;
case OP_ADC_ZP_X:
    adc(readByte(getZP_XAddr()));
    break;
case OP_ADC_ZP_X_IND:
    adc(readByte(getZP_X_INDAddr()));
    break;
case OP_ADC_ZP_IND_Y:
    adc(readByte(getZP_IND_YAddr()));
    break;

/***********************
 * Subtract with Carry *
 ***********************/
case OP_SBC_IMM:
    sbc(readByte(programCounter++));
    break;
case OP_SBC_ABS:
    sbc(readByte(getABSAddr()));
    break;
case OP_SBC_ZP:
    sbc(readByte(getZPAddr()));
    break;
case OP_SBC_ZP_IND:
    sbc(readByte(getZP_INDAddr()));
    break;
case OP_SBC_ABS_X:
    sbc(readByte(getABS_XAddr()));
    break;
case OP_SBC_ABS_Y:
    sbc(readByte(getABS_YAddr()));
    break;
case OP_SBC_ZP_X:
    sbc(readByte(getZP_XAddr()));
    break;
case OP_SBC_ZP_X_IND:
    sbc(readByte(getZP_X_INDAddr()));
    break;
case OP_SBC_ZP_IND_Y:
    sbc(readByte(getZP_IND_YAddr()));
    break;

/*************
 * Increment *
 *************/
// Increment
case OP_INC_ACC:
    A++;
    flagNegative = (A & 0x80) != 0x00;
    flagZero = A == 0;
    break;
case OP_INC_ABS:
    inc(getABSAddr());
    break;
case OP_INC_ZP:
    inc(getZPAddr());
    break;
case OP_INC_ABS_X:
    inc(getABS_XAddr());
    break;
case OP_INC_ZP_X:
    inc(getZP_XAddr());
    break;

case OP_INX:
    X++;
    flagNegative = (X & 0x80) != 0x00;
    flagZero = X == 0;
    break;
case OP_INY:
    Y++;
    flagNegative = (Y & 0x80) != 0x00;
    flagZero = Y == 0;
    break;
/*************
 * Decrement *
 *************/
case OP_DEC_ACC:
    A--;
    flagNegative = (A & 0x80) != 0x00;
    flagZero = A == 0;
    break;
case OP_DEC_ABS:
    dec(getABSAddr());
    break;
case OP_DEC_ZP:
    dec(getZPAddr());
    break;
case OP_DEC_ABS_X:
    dec(getABS_XAddr());
    break;
case OP_DEC_ZP_X:
    dec(getZP_XAddr());
    break;

case OP_DEX:
    X--;
    flagNegative = (X & 0x80) != 0x00;
    flagZero = X == 0;
    break;
case OP_DEY:
    Y--;
    flagNegative = (Y & 0x80) != 0x00;
    flagZero = Y == 0;
    break;
/***********************************
 * Compare accumulator with memory *
 ***********************************/
case OP_CMP_IMM:
    cmp(readByte(programCounter++));
    break;
case OP_CMP_ABS:
    cmp(readByte(getABSAddr()));
    break;
case OP_CMP_ZP:
    cmp(readByte(getZPAddr()));
    break;
case OP_CMP_ABS_X:
    cmp(readByte(getABS_XAddr()));
    break;
case OP_CMP_ABS_Y:
    cmp(readByte(getABS_YAddr()));
    break;
case OP_CMP_ZP_X:
    cmp(readByte(getZP_XAddr()));
    break;
case OP_CMP_ZP_X_IND:
    cmp(readByte(getZP_X_INDAddr()));
    break;
case OP_CMP_ZP_IND_Y:
    cmp(readByte(getZP_IND_YAddr()));
    break;
/*************************
 * Compare X with memory *
 *************************/
case OP_CPX_IMM:
    cpx(readByte(programCounter++));
    break;
case OP_CPX_ABS:
    cpx(readByte(getABSAddr()));
    break;
case OP_CPX_ZP:
    cpx(readByte(getZPAddr()));
    break;
/*************************
 * Compare X with memory *
 *************************/
case OP_CPY_IMM:
    cpy(readByte(programCounter++));
    break;
case OP_CPY_ABS:
    cpy(readByte(getABSAddr()));
    break;
case OP_CPY_ZP:
    cpy(readByte(getZPAddr()));
    break;

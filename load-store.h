// load-store.cfragment
// Contains the implementations of the load/store instructions.
        /**********
         * Load A *
         **********/
        case OP_LDA_IMM:
            lda(readByte(programCounter++));
            break;
        case OP_LDA_ABS:
            lda(readByte(getABSAddr()));
            break;
        case OP_LDA_ZP:
            lda(readByte(getZPAddr()));
            break;
        case OP_LDA_ABS_X:
            lda(readByte(getABS_XAddr()));
            break;
        case OP_LDA_ABS_Y:
            lda(readByte(getABS_YAddr()));
            break;
        case OP_LDA_ZP_IND:
            lda(readByte(getZP_INDAddr()));
            break;
        case OP_LDA_ZP_X:
            lda(readByte(getZP_XAddr()));
            break;
        case OP_LDA_ZP_X_IND:
            lda(readByte(getZP_X_INDAddr()));
            break;
        case OP_LDA_ZP_IND_Y:
            lda(readByte(getZP_IND_YAddr()));
            break;
        /***********
         * Store A *
         ***********/
        case OP_STA_ABS:
            writeByte(getABSAddr(), A);
            break;
        case OP_STA_ZP:
            writeByte(getZPAddr(), A);
            break;
        case OP_STA_ZP_IND:
            writeByte(getZP_INDAddr(), A);
            break;
        case OP_STA_ABS_X:
            writeByte(getABS_XAddr(), A);
            break;
        case OP_STA_ABS_Y:
            writeByte(getABS_YAddr(), A);
            break;
        case OP_STA_ZP_X:
            writeByte(getZP_XAddr(), A);
            break;
        case OP_STA_ZP_X_IND:
            writeByte(getZP_X_INDAddr(), A);
            break;
        case OP_STA_ZP_IND_Y:
            writeByte(getZP_IND_YAddr(), A);
            break;
        /**********
         * Load X *
         **********/
        case OP_LDX_IMM:
            ldx(readByte(programCounter++));
            break;
        case OP_LDX_ABS:
            ldx(readByte(getABSAddr()));
            break;
        case OP_LDX_ZP:
            ldx(readByte(getZPAddr()));
            break;
        case OP_LDX_ABS_Y:
            ldx(readByte(getABS_YAddr()));
            break;
        case OP_LDX_ZP_Y:
            ldx(readByte(getZP_YAddr()));
            break;
        /***********
         * Store X *
         ***********/
        case OP_STX_ABS:
            writeByte(getABSAddr(), X);
            break;
        case OP_STX_ZP:
            writeByte(getZPAddr(), X);
            break;
        case OP_STX_ZP_Y:
            writeByte(getZP_YAddr(), X);
            break;
        /**********
         * Load Y *
         **********/
        case OP_LDY_IMM:
            ldy(readByte(programCounter++));
            break;
        case OP_LDY_ABS:
            ldy(readByte(getABSAddr()));
            break;
        case OP_LDY_ZP:
            ldy(readByte(getZPAddr()));
            break;
        case OP_LDY_ABS_X:
            ldy(readByte(getABS_XAddr()));
            break;
        case OP_LDY_ZP_X:
            ldy(readByte(getZP_XAddr()));
            break;
        /***********
         * Store Y *
         ***********/
        case OP_STY_ABS:
            writeByte(getABSAddr(), Y);
            break;
        case OP_STY_ZP:
            writeByte(getZPAddr(), Y);
            break;
        case OP_STY_ZP_X:
            writeByte(getZP_XAddr(), Y);
            break;
        /**************
         * Store Zero *
         **************/
        case OP_STZ_ABS:
            writeByte(getABSAddr(), 0x00);
            break;
        case OP_STZ_ZP:
            writeByte(getZPAddr(), 0x00);
            break;
        case OP_STZ_ABS_X:
            writeByte(getABS_XAddr(), 0x00);
            break;
        case OP_STZ_ZP_X:
            writeByte(getZP_XAddr(), 0x00);
            break;
        /**********************
         * Register Transfers *
         **********************/
         case OP_TYA:
            A = Y;
            flagNegative = (A & 0x80) != 0;
            flagZero = A == 0;
            break;
         case OP_TAY:
            Y = A;
            flagNegative = (Y & 0x80) != 0;
            flagZero = Y == 0;
            break;
         case OP_TXS:
            stackPointer = X;
            break;
         case OP_TSX:
            X = stackPointer;
            flagNegative = (X & 0x80) != 0;
            flagZero = X == 0;
            break;
         case OP_TAX:
            X = A;
            flagNegative = (X & 0x80) != 0;
            flagZero = X == 0;
            break;
         case OP_TXA:
            A = X;
            flagNegative = (A & 0x80) != 0;
            flagZero = A == 0;
            break;
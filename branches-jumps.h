// branches-jumps.cfragment - contains branch and jump instructions.
// Does not contain JSR or RTS
        /************
         * Branches *
         ************/
        case OP_BRA:
            // the cast to int8_t makes it a signed type, so that the offset
            //  works correctly.
            branch((int8_t) readByte(programCounter++));
            break;
        case OP_BPL:
            branchIf(!flagNegative);
            break;
        case OP_BMI:
            branchIf(flagNegative);
            break;
        case OP_BVC:
            branchIf(!flagOverflow);
            break;
        case OP_BVS:
            branchIf(flagOverflow);
            break;
        case OP_BCC:
            branchIf(!flagCarry);
            break;
        case OP_BCS:
            branchIf(flagCarry);
            break;
        case OP_BNE:
            branchIf(!flagZero);
            break;
        case OP_BEQ:
            branchIf(flagZero);
            break;
        /*********
         * Jumps *
         *********/
        case OP_JMP_ABS:
            programCounter = getABSAddr();
            break;
        case OP_JMP_ABS_IND:
            programCounter = getABS_INDAddr();
            break;
        case OP_JMP_ABS_X_IND:
            programCounter = getABS_X_INDAddr();
            break;
        /***********************
         * Branch if Bit Reset *
         ***********************/
        case OP_BBR0:
            bbr(0);
            break;
        case OP_BBR1:
            bbr(1);
            break;
        case OP_BBR2:
            bbr(2);
            break;
        case OP_BBR3:
            bbr(3);
            break;
        case OP_BBR4:
            bbr(4);
            break;
        case OP_BBR5:
            bbr(5);
            break;
        case OP_BBR6:
            bbr(6);
            break;
        case OP_BBR7:
            bbr(7);
            break;
        /*********************
         * Branch if Bit Set *
         *********************/
        case OP_BBS0:
            bbs(0);
            break;
        case OP_BBS1:
            bbs(1);
            break;
        case OP_BBS2:
            bbs(2);
            break;
        case OP_BBS3:
            bbs(3);
            break;
        case OP_BBS4:
            bbs(4);
            break;
        case OP_BBS5:
            bbs(5);
            break;
        case OP_BBS6:
            bbs(6);
            break;
        case OP_BBS7:
            bbs(7);
            break;
        /*********************
         * Reset Memory Bits *
         *********************/
        case OP_RMB0:
            rmb(0);
            break;
        case OP_RMB1:
            rmb(1);
            break;
        case OP_RMB2:
            rmb(2);
            break;
        case OP_RMB3:
            rmb(3);
            break;
        case OP_RMB4:
            rmb(4);
            break;
        case OP_RMB5:
            rmb(5);
            break;
        case OP_RMB6:
            rmb(6);
            break;
        case OP_RMB7:
            rmb(7);
            break;
        /*********************
         * Set Memory Bits *
         *********************/
        case OP_SMB0:
            smb(0);
            break;
        case OP_SMB1:
            smb(1);
            break;
        case OP_SMB2:
            smb(2);
            break;
        case OP_SMB3:
            smb(3);
            break;
        case OP_SMB4:
            smb(4);
            break;
        case OP_SMB5:
            smb(5);
            break;
        case OP_SMB6:
            smb(6);
            break;
        case OP_SMB7:
            smb(7);
            break;

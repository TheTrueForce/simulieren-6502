// undefined.h - undefined opcodes of the W65C02

case OP_UNDEF_02:   // 2-byte NOPs
case OP_UNDEF_22:
case OP_UNDEF_42:
case OP_UNDEF_62:
case OP_UNDEF_82:
case OP_UNDEF_C2:
case OP_UNDEF_E2:
case OP_UNDEF_44:
case OP_UNDEF_54:
case OP_UNDEF_D4:
case OP_UNDEF_F4:
    programCounter++;
    break;
case OP_UNDEF_5C:   // 3-byte NOPs
case OP_UNDEF_DC:
case OP_UNDEF_FC:
    programCounter += 2;
    break;
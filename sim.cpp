#include "simulieren-6502.h"
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

extern uint8_t A, X, Y, stackPointer;
extern uint16_t programCounter;
extern bool flagNegative, flagOverflow, flagBRK, flagDecimal, flagIRQdisable, flagZero, flagCarry;

#define BUF_SIZE 1024

#define MEMORY_SIZE 0x10000
#define OUTPUT_ADDR 0x7FFF

uint8_t memory[MEMORY_SIZE];

uint16_t breakpoint = 0000;

void printRegs() {
    printf("]A = $%02X\tX = $%02X\tY = $%02X\n", A, X, Y);
    printf("]PC = $%04X\tSP = $%02X\n", programCounter, stackPointer);
    printf("]Status register: %c%c-%c%c%c%c%c\n",    flagNegative?'N':'n',
           flagOverflow?'V':'v',   flagBRK?'B':'b',  flagDecimal?'D':'d',
           flagIRQdisable?'I':'i', flagZero?'Z':'z', flagCarry?'C':'c');
}

uint8_t readByte(uint16_t address) {
    if (address == OUTPUT_ADDR) {
        printf(">");
        memory[address] = getc(stdin);
    }
    return memory[address];
}

void writeByte(uint16_t address, uint8_t data){
    if (address == OUTPUT_ADDR) {
        printf("%c", data);
    }
    memory[address] = data;
}

// loads an Intel Hex (I8HEX) file
void loadHexFile() {
    //printf("hex loading not yet implemented.\n");
    //return;
    
    char filename[BUF_SIZE];
    // get file name
    printf("]Filename: ");
    fgets(filename, BUF_SIZE, stdin);
    filename[strcspn(filename, "\r\n")] = 0;
    
    // open file
    FILE *hexFile = fopen(filename, "r");
    if (hexFile == NULL) {
        printf("]Error opening hex file: %s", filename);
        perror("");
        return;
    }
    
    char lineBuf[BUF_SIZE];
    unsigned char byteCount, recType;   // number of bytes stored in the line and the record type
    unsigned char checksum;             // stored checksum
    int accumulate;                     // the running total of the calculated checksum
    unsigned short address;             // target address of a data line
    unsigned char data[255];            // a data line may contain 0xFF bytes
    int linecount = 0;                  // how many lines have been processed
    unsigned int i;                     // loop counter. value is dropped through and the variable is re-used.
    
    // while dataLeft
    while (fgets(lineBuf, BUF_SIZE, hexFile) != NULL) {
        // parse in a hex record
        sscanf(lineBuf, ":%02hhX%04hX%02hhX", &byteCount, &address, &recType);
        for (i = 0; i < byteCount; i++) {
            sscanf(&lineBuf[2 * i + 9], "%02hhX", &data[i]);
        }
        // i is left one higher
        sscanf(&lineBuf[2 * i + 9], "%02hhX", (unsigned char *)&checksum);
        
        // calculate checksum
        accumulate = byteCount + ((address & 0xFF00) >> 8) + (address & 0x00FF) + recType;
        for (i = 0; i < byteCount; i++) {
            accumulate += data[i];
        }
        
        // checksum should == -accumulate
        if ((-accumulate & 0x00FF) != checksum) {
            printf("]Checksum error on line %i!\n", linecount);
            printf("]Stored checksum: %02X. Calculated checksum: %02X.\n", checksum, (-accumulate & 0x00FF));
        }
        linecount++;
        
        // type 0x00 = data with 16-bit start addr
        if (recType == 0x00) {
            // write to memory
            for (i = 0; i < byteCount; i++) {
                memory[address+i] = data[i];
                //printf("%04X %02X\n", i, data[i]);
            }
        } else { // type 0x01 = EOF
            break;
        }
        
    }
    // close file
    fclose(hexFile);
    printf("]%i lines loaded.\n", linecount);
}

/* commands:
    view registers              v
    reset                       r
    load Intel Hex file         l           - prompts for filename
    set PC                      s aaaa
    set breakpoint              b aaaa
    write byte                  w aaaa dd
    read byte                   r aaaa
    run for n instructions      x nnnn
    run for one instruction     x
    free-run                    f           - stop this mode with ^A
    quit                        q
*/
int main() {
    char cmd;
    uint8_t data;
    uint16_t address;
    char buf[BUF_SIZE];
    int matched;
    
    // set up the non-blocking copy of stdin for the free-running mode
    //int stdin_NB_FD = dup(STDIN_FILENO);
    //fcntl(stdin_NB_FD, F_SETFL, (fcntl(stdin_NB_FD, F_GETFL, 0)|O_NONBLOCK));
    //FILE *stdin_NB = fdopen(stdin_NB_FD, "r");
    
    printf("6502 sim\n");
    
    // main program loop
    while (true) {
        // print registers
        //printRegs();
        printf("\n]>");
        // get input
        if(!fgets(buf, BUF_SIZE, stdin)) {
            perror("Error reading STDIN");
            printf("Terminating...\n");
            exit(EXIT_FAILURE);
        }
        matched = sscanf(buf, "%c %hx %hhx", &cmd, &address, &data);
        
        // act on it
        if (matched == 0) {
            printf("]Doing nothing.\n");
        } else if (matched == 1) {
            // r, x, q, l
            if (tolower(cmd) == 'q') {
                break;
            }
            switch (tolower(cmd)) {
                case 'r':
                    //reset6502(false);
                    reset6502(true);
                    break;
                case 'x':   // execute one
                    do6502();
                    printRegs();
                    break;
                case 'l':
                    loadHexFile();
                    break;
                case 'v':
                    printRegs();
                    break;
                case 'f':
                    while (true) {
                        do6502();
                        if (programCounter == breakpoint) {
                            printf("]Breakpoint hit!\n");
                            printRegs();
                            break;
                        }
                    }
                    break;
                default:
                    printf("]Unrecognized command\n");
                    break;
            }
        } else if (matched == 2) {
            // s, r, x
            switch (tolower(cmd)) {
                case 's':
                    programCounter = (uint16_t) address;
                    break;
                case 'r':
                    printf("]$%04X: $%02X\n", address, readByte(address));
                    break;
                case 'x':
                    printf("]Executing $%X(%i) instructions\n", address, address);
                    for (unsigned int i = 0; i < address; i++){
                        do6502();
                        if (programCounter == breakpoint) {
                            printf("]Breakpoint hit!\n");
                            printRegs();
                            break;
                        }
                    }
                    printRegs();
                    break;
                case 'b':
                    breakpoint = address;
                    printf("]Set breakpoint at address %04X\n", breakpoint);
                    break;
                default:
                    printf("]Unrecognized command\n");
                    break;
            }
        } else if (matched == 3) {
            // w
            if (tolower(cmd) == 'w') {
                writeByte(address, data);
            } else {
                printf("]Unrecognized command\n");
            }
        }
    }
    exit(EXIT_SUCCESS);
}
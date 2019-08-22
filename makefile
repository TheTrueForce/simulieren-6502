COMPILER = gcc
FLAGS = -Wall -pedantic
TARGETS = tests simulieren-6502.o
TESTS = tests/testAddrmodes.out
TESTMODULES = simulieren-6502.o


all: ${TARGETS}

simulieren-6502.o: simulieren-6502.cpp simulieren-6502.h opcodes.h add-subtract.h branches-jumps.h load-store.h logic-ops.h
	${COMPILER} -c simulieren-6502.cpp ${FLAGS} -o simulieren-6502.o

tests: ${TESTS}

tests/testAddrmodes.out: ${TESTMODULES} tests/testCommon.h tests/testAddrModes.cpp
	${COMPILER} ${TESTMODULES} tests/testAddrModes.cpp ${FLAGS} -o tests/testAddrModes.out

clean:
	rm ${TESTS} ${TESTMODULES}
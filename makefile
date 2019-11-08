COMPILER = gcc
FLAGS = -Wall -pedantic
TARGETS = tests simulieren-6502.o sim autoSim
TESTS = tests/testAddrmodes.out
TESTMODULES = simulieren-6502.o


all: ${TARGETS}

simulieren-6502.o: simulieren-6502.cpp simulieren-6502.h opcodes.h add-subtract.h branches-jumps.h load-store.h logic-ops.h
	${COMPILER} -c simulieren-6502.cpp ${FLAGS} -o simulieren-6502.o

autoSim: autoSim.cpp simulieren-6502.h simulieren-6502.o
	${COMPILER} autoSim.cpp simulieren-6502.o ${FLAGS} -o autoSim

sim: sim.cpp simulieren-6502.h simulieren-6502.o
	${COMPILER} sim.cpp simulieren-6502.o ${FLAGS} -o sim

tests: ${TESTS}

tests/testAddrmodes.out: ${TESTMODULES} tests/testCommon.h tests/testAddrModes.cpp
	${COMPILER} ${TESTMODULES} tests/testAddrModes.cpp ${FLAGS} -o tests/testAddrModes.out

clean:
	rm ${TESTS} ${TESTMODULES} sim

# simulieren-6502
W65C02 simulator, written in C/C++, and intended to be usable as an Arduino library.

This project is a result of my wanting to build a 65c02-based handheld. I did design a few iterations of hardware for the purpose, and that was fun, but very time-consuming, and I'd rather not do it again.  
In the end, I decided to build a 6502 simulator, run it on an Arduino Due(it has the memory, and definitely the horsepower), and construct the hardware around that.

The \/tests directory is no longer used. I have since switched to using Klaus2m5's test suite([here](https://github.com/Klaus2m5/6502_65C02_functional_tests)) to avoid writing my own and possibly getting it wrong. Simulieren-6502 passes nearly all the 6502 tests, and I am working on getting it to pass the 65C02 ones as well.
The 6502 tests that it fails are:
 - The decimal mode ADC/SBC test($2A). As it stands now, ADC and SBC should function correctly in decimal mode.

The 65C02 tests that it fails are:
 - TRB/TSB zp/abs($10)
 - anything that follows does not execute, for some reason.

**Important:** This project is intended to simulate the W65C02, as laid out in _Programming the 65816, including the 6502, 65C02, and 65802_ by David Eyes and Ron Lichty. However, this simulation is not perfect, and this is called out in the source files where relevant.

I am aware that there is at least one other 6502 simulator ([on the Arduino forum](https://forum.arduino.cc/index.php?topic=193216.0)), and I did take a look into that, but in my opinion, it makes a few design decisions that make it hard to call a library, as opposed to a straight-up program.

**I have looked at its code, but not closely, and all of the code in this repository is my own work.**

I would prefer to be able to include a library that just handles the 6502 simulation, allowing(and requiring) the user to provide memory, peripherals, etc. external to the simulation itself, and this is what I have built.

Criticisms aside, I did take a few design cues from that project. In particular, separating addressing modes from the actual operations.  
For example, `SBC Immediate` is implemented as `sbc(readByte(programCounter++);`.

At one stage, I did try to decode the opcodes as a real 6502 would, but I found it too difficult to wrap my head around, so I just used a massive switch statement. I spread this switch across a few files in order to break it up, which may or may not have been a mistake.

A very nice side-effect of building this as a pure simulation is that it's not limited to the Arduino platform. This compiles with GCC under WSL perfectly fine, and should work just fine under a real Linux install(although this is not tested). Getting it to work under MSVC might take a bit of work, but I don't know, since I don't use it.
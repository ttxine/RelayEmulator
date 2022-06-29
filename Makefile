all: memory.o cpu.o emulator.o main.o
	g++ memory.o cpu.o emulator.o main.o

main.o: main.cc
	g++ -c -Wall main.cc

emulator.o: src/emulator.cc
	g++ -c -Wall src/emulator.cc

cpu.o: src/cpu.cc
	g++ -c -Wall src/cpu.cc

memory.o: src/memory.cc
	g++ -c -Wall src/memory.cc

clear:
	rm -rf *.o *.out

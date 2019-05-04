.PHONY: clean

simulator : main.o pipeline.o latchRegister.o ALU.o memory.o decoder.o
	g++ -g -o simulator main.o pipeline.o latchRegister.o ALU.o memory.o decoder.o

main.o : main.cpp memory.h pipeline.h
	g++ -g -c main.cpp

decoder.o : decoder.cpp decoder.h 
	g++ -g -c decoder.cpp

latchRegister.o : latchRegister.cpp latchRegister.h
	g++ -g -c latchRegister.cpp

memory.o : memory.cpp memory.h
	g++ -g -c memory.cpp

pipeline.o : pipeline.cpp pipeline.h decoder.h ALU.h
	g++ -g -c pipeline.cpp

ALU.o : ALU.cpp ALU.h decoder.h
	g++ -g -c ALU.cpp

clean:
	rm -rf *.o


Target = MyStack
Compiler = g++

$(Target) : Main.o Debug.o
	$(Compiler) Main.o Debug.o -o $(Target)

Main.o : Main.cpp
	$(Compiler) -c Main.cpp -o Main.o

Debug.o : Debug.cpp
	$(Compiler) -c Debug.cpp -o Debug.o

clean :
	rm -f *.o

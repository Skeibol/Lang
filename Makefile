all: crust crust.o run

build: crust crust.o

run: ./crust
	./crust ./code.cro # -------- Program starts here --------
	
crust: ./src/main.cpp ./src/tokenizer.hpp ./src/parser.hpp ./src/generator.hpp ./src/tokenizer.cpp
	g++ -fdiagnostics-color=always -g ./src/*.cpp -o ./crust

crust.o: ./src/main.cpp ./src/tokenizer.hpp ./src/parser.hpp ./src/generator.hpp ./src/tokenizer.cpp
	g++ ./src/*.cpp -o ./crust.o



clean:
	rm -f ./crust ./crust.o # Removed files ./crust.o , ./crust

blabla:
	echo 2
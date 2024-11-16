all: lang lang.o run

build: lang lang.o

run: ./lang
	./lang ./code.cro # -------- Program starts here --------
	
lang: ./src/main.cpp ./src/tokenizer.hpp ./src/parser.hpp ./src/generator.hpp ./src/tokenizer.cpp
	g++ -fdiagnostics-color=always -g ./src/*.cpp -o ./lang

lang.o: ./src/main.cpp ./src/tokenizer.hpp ./src/parser.hpp ./src/generator.hpp ./src/tokenizer.cpp
	g++ ./src/*.cpp -o ./lang.o



clean:
	rm -f ./lang ./lang.o # Removed files ./lang.o , ./lang

blabla:
	echo 2
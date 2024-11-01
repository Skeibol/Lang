

all: ./build/crust ./build/crust.o run

run: ./build/crust
	@echo
	./build/crust code.cro # -------- Program starts here -------- 
	
./build/crust: ./src/main.cpp
	g++ -fdiagnostics-color=always -g ./src/main.cpp -o ./build/crust

./build/crust.o: ./src/main.cpp
	g++ ./src/main.cpp -o ./build/crust.o



clean:
	rm -f ./build/crust ./build/crust.o # Removed files ./build/crust.o , ./build/crust
#include "./generator.hpp"
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>

std::string readFileContents(std::string codePath) {
    std::string code;
    std::stringstream content_stream;
    std::fstream input(codePath, std::ios::in);
    content_stream << input.rdbuf();
    code = content_stream.str();

    return code;
}

void writeToFile(std::string fileContents) {
    std::fstream file("./build/out.asm", std::ios::out);
    file << fileContents;
    std::cout << fileContents;
}

int main(int argc, char const *argv[]) {
    if (argc != 2)
    {
        std::cerr << "Incorrect usage" << "\n";
        std::cerr << "cro <input.cro>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string fileContents = readFileContents(argv[1]);

    Tokenizer tokenizer(std::move(fileContents));
    std::vector<Token> tokens = tokenizer.tokenize();

    Parser parser(std::move(tokens));
    std::optional<node::Program> program = parser.parseProgram();

    if (!program.has_value())
    {
        std::cerr << "Invalid program" << std::endl;
    }

    Generator generator(program.value());
    writeToFile(generator.generateProgram());




    system("nasm -felf64 ./build/out.asm");
    system("ld -o ./build/out ./build/out.o");
    return EXIT_SUCCESS;
}

#include "./generator.hpp"
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>
#include <dirent.h>


struct dirent;

std::string readFileContents(std::string const &codePath) {
    std::string code;
    std::stringstream content_stream;
    std::fstream input(codePath, std::ios::in);
    content_stream << input.rdbuf();
    code = content_stream.str();

    return code;
}

void writeToFile(std::string const &fileContents) {
    std::fstream file("./build/out.asm", std::ios::out);
    file << fileContents;
}


void test() {
    // Very fucking unsafe, expect errors
    DIR *dp = nullptr;
    std::vector<std::string> foundTestFiles{};

    dp = opendir("./testing/");
    if (dp != nullptr) {
        dirent *entry = nullptr;
        while ((entry = readdir(dp))) {
            std::string dNameString = entry->d_name;
            if (entry->d_type == DT_DIR || dNameString == "." || dNameString == ".." || dNameString.find(".test.cro") ==
                std::string::npos) {
                std::cout << "discarded file ->" << dNameString << '\n';
                continue;
            }

            std::cout << "found test file -> " << dNameString << '\n';

            foundTestFiles.push_back("./testing/" + dNameString);
        }
    }
    std::cout << "\n======== " << foundTestFiles.size() << " tests found ========" << '\n';

    std::vector<std::string> nasmStr{};
    std::vector<std::string> linkerStr{};
    for (int i = 0; i < foundTestFiles.size(); i++) {
        std::string fileContents = readFileContents(foundTestFiles[i]);
        std::string programName = foundTestFiles[i].erase(foundTestFiles[i].length() - 4);
        std::cout << "Try compile program -> " << programName << '\n';
        programName = programName.erase(0, 10);
        auto outputName = programName;
        outputName.erase(outputName.length() - 5);
        std::fstream file("./testing/testAsm/" + programName + ".asm", std::ios::out);

        auto *tokenizer = new Tokenizer(std::move(fileContents));

        std::vector<Token> tokens = tokenizer->tokenize();
        auto *parser = new Parser(std::move(tokens));

        std::optional<node::Program*> program = parser->parseProgram();

        if (!program.has_value()) {
            std::cerr << "Invalid program" << std::endl;
        }
        auto *generator = new Generator(program.value());
        file << generator->generateProgram();

        /*
        nasm -felf64 ./test/testAsm/code.test.asm

        ld -o ./test/code ./test/testAsm/code.test.o
         */
        nasmStr.push_back("nasm -felf64 ./testing/testAsm/" + programName + ".asm");
        linkerStr.push_back("ld -o ./testing/" + outputName + " ./testing/testAsm/" + programName + ".o");
        std::cout << generator << " <- gen address " << parser << " <- parser address " << tokenizer << " <- tokenizer address\n";
        std::cout << "Compiled program -> " << programName << '\n';

    }
    for (int i = 0; i < nasmStr.size(); i++) {
        int linkResult = system(nasmStr[i].c_str());
        int nasmResult = system(linkerStr[i].c_str()); // Linker/Assembler error checking
        if (linkResult) {
            std::cout << linkResult << " - link error" << '\n';
        }
        if (nasmResult) {
            std::cout << nasmResult << " - nasm error" << '\n';
        }

        std::cout << foundTestFiles[i] << " - File succesfuly compiled" << '\n';
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        std::string const testFolder = "./testing/";
        std::cerr << "No input given, testing mode" << "\n";
        test();
        return EXIT_FAILURE;
    }

    std::string fileContents = readFileContents(argv[1]);

    Tokenizer tokenizer(std::move(fileContents));
    std::vector<Token> tokens = tokenizer.tokenize();

    Parser parser(std::move(tokens));
    std::optional<node::Program*> program = parser.parseProgram();

    if (!program.has_value()) {
        std::cerr << "Invalid program" << std::endl;
    }

    Generator generator(program.value());
    writeToFile(generator.generateProgram());


    system("nasm -felf64 ./build/out.asm");
    system("ld -o ./build/out ./build/out.o");
    return EXIT_SUCCESS;
}

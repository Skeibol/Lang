#include "./tokenizer.hpp"
#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>
std::string tokensToAssembly(const std::vector<Token> tokens)
{
  std::stringstream output;
  output << "global _start\n_start:\n";

  for (int i = 0; i < tokens.size(); i++)
  {
    int a;
    const Token token = tokens.at(i);
    if (token.type == TokenType::_exit)
    {
      if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::_int_lit)
      {
        if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::_semi)
        {
          output << "    mov rax, 60\n";
          output << "    mov rdi, " << tokens.at(i + 1).value.value() << "\n";
          output << "    syscall\n";
        }
      }
    }
  }

  return output.str();
}
std::string readFileContents(std::string codePath)
{
  std::string code;
  std::stringstream content_stream;

  std::fstream input(codePath, std::ios::in);
  content_stream << input.rdbuf();
  code = content_stream.str();

  return code;
}
void writeToFile(std::string fileContents)
{
  std::fstream file("./build/out.asm", std::ios::out);
  file << fileContents;
  std::cout << fileContents;
}

int main(int argc, char const *argv[])
{
  if (argc != 2)
  {
    std::cerr << "Incorrect usage" << "\n";
    std::cerr << "cro <input.cro>" << std::endl;
    return EXIT_FAILURE;
  }

  std::string fileContents = readFileContents(argv[1]);

  Tokenizer tokenizer(std::move(fileContents));
  std::vector<Token> tokens = tokenizer.tokenize();

  std::string assemblyCode = tokensToAssembly(tokens);
  writeToFile(assemblyCode);

  system("nasm -felf64 ./build/out.asm");
  system("ld -o ./build/out ./build/out.o");
  return EXIT_SUCCESS;
}

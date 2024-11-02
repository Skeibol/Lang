#include "./tokenization.hpp"
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

int main(int argc, char const *argv[])
{
  if (argc != 2)
  {
    std::cerr << "Incorrect usage" << "\n";
    std::cerr << "cro <input.cro>" << std::endl;
    return EXIT_FAILURE;
  }
  std::string fileContents;

  {
    std::stringstream content_stream;
    std::fstream input(argv[1], std::ios::in);
    content_stream << input.rdbuf();
    fileContents = content_stream.str();
  }

  Tokenizer tokenizer(std::move(fileContents));
  std::vector<Token> tokenz = tokenizer.tokenize();

  {
    std::fstream file("./out.asm", std::ios::out);
    file << tokensToAssembly(tokenz);
    std::cout << tokensToAssembly(tokenz);
  }
 
  system("nasm -felf64 ./out.asm");
  system("ld -o ./out ./out.o");
  return EXIT_SUCCESS;
}

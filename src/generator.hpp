#pragma once
#include "parser.hpp"
#include <sstream>

class Generator
{
public:
  inline explicit Generator(node::Exit root) : m_rootNode(std::move(root))
  {
  }

  [[nodiscard]] std::string generate() const
  {
    std::stringstream output;
    output << "global _start\n_start:\n";
    output << "    mov rax, 60\n";
    output << "    mov rdi, " << m_rootNode.expression.int_lit.value.value() << "\n";
    output << "    syscall\n";
    
    return output.str();
  }

private:
  const node::Exit m_rootNode;
};
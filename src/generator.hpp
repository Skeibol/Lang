#pragma once
#include "parser.hpp"
#include <sstream>

class Generator
{
public:
  inline explicit Generator(node::Program root) : m_program(std::move(root))
  {
  }

  [[nodiscard]] std::string generateExpression(const node::Expression expression) const
  {
    struct StatementVisitor
    {
      void operator()(const node::ExpressionIntLit &expr_int_lit)
      {
    
      }
      void operator()(const node::ExpressionIdentifier &expr_identifier)
      {
      }
    };

    StatementVisitor visitor;
    std::visit(visitor, expression.type);
  }
  [[nodiscard]] std::string generateStatement(const node::Statement statement) const
  {
    struct StatementVisitor
    {
      void operator()(const node::StatementExit &stmt_exit)
      {
 
      }
      void operator()(const node::StatementExit &stmt_let)
      {
      }
    };

    StatementVisitor visitor;
    std::visit(visitor, statement.type);
  }

  [[nodiscard]] std::string generateProgram() const
  {
    std::stringstream output;
    output << "global _start\n_start:\n";

    for (const node::Statement stmt : m_program.statements)
    {
      output << generateStatement(stmt);
    }

    output << "    mov rax, 60\n";
    output << "    mov rdi, 0" << "\n";
    output << "    syscall\n";

    return output.str();
  }

private:
  const node::Program m_program;
};
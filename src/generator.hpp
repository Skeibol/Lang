#pragma once
#include "parser.hpp"
#include <sstream>
#include <unordered_map>

class Generator
{
public:
  inline explicit Generator(node::Program root) : m_program(std::move(root))
  {
  }

  void generateExpression(const node::Expression &expression)
  {
    struct ExpressionVisitor
    {
      Generator *gen;

      void operator()(const node::ExpressionIntLit &expr_int_lit) const
      {

        gen->m_output << "    mov rax, " << expr_int_lit.int_lit.value.value() << "\n";
        gen->push("rax");
      }

      void operator()(const node::ExpressionIdentifier &expr_identifier) const
      {
        if (gen->m_variables.count(expr_identifier.identifier.value.value()) == 0)
        {
          std::cerr << "Variable(identifier) not declared :" << expr_identifier.identifier.value.value();
          exit(EXIT_FAILURE);
        }
        auto &_variable = gen->m_variables.at(expr_identifier.identifier.value.value());

        std::stringstream registerString;
        registerString << "QWORD [rsp + " << (gen->m_stackPointerOffset - _variable.stackLocation - 1) * 8 << "]";
        gen->push(registerString.str());
      }
    };

    ExpressionVisitor visitor({.gen = this});
    std::visit(visitor, expression.type);
  }

  void generateStatement(const node::Statement &statement)
  {
    struct StatementVisitor
    {
      Generator *gen;

      void operator()(const node::StatementExit &stmt_exit) const
      {
        gen->m_output << "    ;; Exit statement start\n"; // TODO cmp rdx, rax
        gen->generateExpression(stmt_exit.expression);

        gen->m_output << "    mov rax, 60\n"; // TODO
        gen->pop("rdi");
        gen->m_output << "    syscall\n";
        gen->m_output << "    ;; Exit statement end - rax calls exit, rdi provides arguement(exit code)\n"; // TODO cmp rdx, rax
      }
      void operator()(const node::StatementEquality &stmt_equality) const
      {
        gen->m_output << "    ;; equality start\n"; // TODO
        gen->generateExpression(stmt_equality.expressionLeft);
        gen->pop("rbx");
        gen->generateExpression(stmt_equality.expressionRight);
        gen->pop("rcx");
        gen->m_output << "    cmp rbx, rcx\n";
        gen->m_output << "    ;; equality end compare rax 1 == rax 2 - use jl flag\n";
      }

      void operator()(const node::StatementLet &stmt_let) const
      {
        if (gen->m_variables.count(stmt_let.identifier.value.value()))
        {
          std::cerr << "Identifier already declared :" << stmt_let.identifier.value.value();
          exit(EXIT_FAILURE);
        }
        gen->m_variables.insert({stmt_let.identifier.value.value(), Variable{gen->m_stackPointerOffset}});
        gen->generateExpression(stmt_let.expression);
      }
    };

    StatementVisitor visitor{.gen = this};
    std::visit(visitor, statement.type);
  }

  std::string generateProgram()
  {
    m_output << "global _start\n_start:\n";

    for (const node::Statement &stmt : m_program.statements)
    {
      generateStatement(stmt);
    }

    m_output << "    ; safety exit... program probably out\n";
    m_output << "    mov rax, 60\n";
    m_output << "    mov rdi, 0" << "\n";
    m_output << "    syscall\n";

    return m_output.str();
  }

private:
  void push(const std::string &_register)
  {
    m_output << "    push " << _register << "\n";
    m_stackPointerOffset++;
  }

  void pop(const std::string &_register)
  {
    m_output << "    pop " << _register << "\n";
    m_stackPointerOffset--;
  }

  struct Variable
  {
    size_t stackLocation;
  };

  const node::Program m_program;
  std::stringstream m_output;

  size_t m_stackPointerOffset = 0;
  std::unordered_map<std::string, Variable> m_variables{};
};

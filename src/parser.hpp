#pragma once
#include "tokenizer.hpp"
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace node
{
struct Expression
{
  Token int_lit;
};
struct Exit
{
  Expression expression;
};
} // namespace node

class Parser
{
public:
  inline explicit Parser(std::vector<Token> token) : m_tokens(std::move(token))
  {
  }

  std::optional<node::Expression> parseExpression()
  {
    if (peek().has_value() && peek().value().type == TokenType::_int_lit)
    {
      return node::Expression{consume()};
    }
    else
    {
      return {};
    }
  }

  std::optional<node::Exit> parse()
  {
    
    std::optional<node::Exit> exitNode;

    while (peek().has_value())
    {
      if (peek().value().type == TokenType::_exit)
      {
        consume();
        if (auto nodeExpression = parseExpression()) // Auto je skracenica za VAR.Kinda. Ako parseExpression(optional) vraca nešto, evaluate TRUE. Ako ne FALSE. Istovremeno assigna nodeExpression (implicit type conversion)
        {
          exitNode = node::Exit{nodeExpression.value()};
        }
        else
        {
          std::cerr << "Invalid expression";
          exit(EXIT_FAILURE);
        }

        if (peek().has_value() || peek().value().type != TokenType::_semi)
        {
          std::cerr << "Invalid expression";
          exit(EXIT_FAILURE);
        }
      }
    }
    m_currentIndex = 0;
    return exitNode;
  }

private:
  size_t m_currentIndex = 0;
  const std::vector<Token> m_tokens;

  [[nodiscard]] std::optional<Token> peek(int amount = 0) const
  {
    if (m_currentIndex + amount >= m_tokens.size())
    {
      return {};
    }
    else
    {
      return m_tokens.at(m_currentIndex);
    }
  }

  Token consume()
  {
    m_currentIndex++;
    return m_tokens.at(m_currentIndex - 1);
  }
};
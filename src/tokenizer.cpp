#include "tokenizer.hpp"
#include <iostream>

Tokenizer::Tokenizer(std::string src)
    : m_source(std::move(src))
{
  // Constructor implementation (if needed)
}

std::vector<Token> Tokenizer::tokenize()
{

  std::string buffer;
  std::vector<Token> tokens = {};

  while (peek().has_value())
  {
    if (std::isalpha(peek().value()))
    {
      do
      {
        buffer.push_back(consume());
      } while (peek().has_value() && std::isalnum(peek().value()));

      if (buffer == "return")
      {
        tokens.push_back({TokenType::_exit});
        buffer.clear();
      }
      else if(buffer == "let") {
        tokens.push_back({TokenType::_let});
        buffer.clear();
      }
      else
      {
        tokens.push_back({TokenType::_identifier, buffer});
        buffer.clear();
      }
    }
    else if (std::isdigit(peek().value()))
    {
      do
      {
        buffer.push_back(consume());
      } while (peek().has_value() && std::isdigit(peek().value()));

      tokens.push_back({TokenType::_int_lit, buffer});
      buffer.clear();
    }
    else if (peek().value() == '(')
    {
      consume();
      tokens.push_back({TokenType::_paren_open});
    }
    else if (peek().value() == ')')
    {
      consume();
      tokens.push_back({TokenType::_paren_close});
    }
    else if (peek().value() == '+')
    {
      buffer.push_back(consume());
      tokens.push_back({TokenType::_plus,buffer});
      buffer.clear();
    }
    else if (peek().value() == ';')
    {
      consume();
      tokens.push_back({TokenType::_semi});
    }
    else if (peek().value() == '=')
    {
      consume();
      tokens.push_back({TokenType::_equals});
    }
    else if (std::isspace(peek().value()))
    {
      consume();
    }
    else
    {
      std::cout << m_source.at(m_currentIndex) << std::endl;
      std::cerr << "Unrecognized token" << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  m_currentIndex = 0;
  return tokens;
}

std::optional<char> Tokenizer::peek(int amount) const
{
  if (m_currentIndex + amount >= m_source.length())
  {
    return {};
  }
  else
  {
    return m_source.at(m_currentIndex);
  }
}

char Tokenizer::consume()
{
  m_currentIndex++;
  return m_source.at(m_currentIndex - 1);
}

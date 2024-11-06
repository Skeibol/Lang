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
        continue;
      }
      else if(buffer == "let") {
        tokens.push_back({TokenType::_let});
        buffer.clear();
        continue;
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
      continue;
    }
    else if (peek().value() == ')')
    {
      consume();
      tokens.push_back({TokenType::_paren_close});
      continue;
    }
    else if (peek().value() == ';')
    {
      consume();
      tokens.push_back({TokenType::_semi});
      continue;
    }
    else if (peek().value() == '=')
    {
      consume();
      tokens.push_back({TokenType::_equals});
      continue;
    }
    else if (std::isspace(peek().value()))
    {
      consume();
      continue;
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

#pragma once
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

enum class TokenType
{
  _exit,
  _int_lit,
  _semi
};

struct Token
{
  TokenType type;
  std::optional<std::string> value{};
};

class Tokenizer
{
public:
  inline explicit Tokenizer(std::string src)
      : m_source(std::move(src))
  {
    /* Something here */
  }

  inline std::vector<Token> tokenize()
  {
    std::string buffer = "";
    std::vector<Token> tokens = {};
    while (peek().has_value())
    {
      if (std::isalpha(peek().value()))
      {

        do
        {
          buffer.push_back(consume()); // Incrementer
        } while (peek().has_value() && std::isalnum(peek().value()));
        if (buffer == "return")
        {
          tokens.push_back({TokenType::_exit});
          buffer.clear();
          continue;
        }
        else
        {
          std::cout << peek().value() << std::endl;
          std::cerr << "Unrecognized token " << std::endl;
          exit(EXIT_FAILURE);
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
      else if (peek().value() == 59) /* 59 = semicolon */
      {
        consume();
        tokens.push_back({TokenType::_semi});
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

private:
  [[nodiscard]] std::optional<char> peek(int amount = 0) const
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

  char consume()
  {
    m_currentIndex++;
    return m_source.at(m_currentIndex - 1);
  }
  const std::string m_source;
  size_t m_currentIndex = 0;
};
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
        if (isComment)
        {
            if (peek().value() == '\n')
            {
                isComment = false;
            }
            consume();
            continue;
        }
        if (std::isalpha(peek().value()))
        {

            do
            {
                buffer.push_back(consume());
            } while (peek().has_value() && std::isalnum(peek().value()));

            if (buffer == "izlaz")
            {
                tokens.push_back({TokenType::_exit});
                buffer.clear();
            }
            else if (buffer == "neka")
            {
                tokens.push_back({TokenType::_let});
                buffer.clear();
            }
            else if (buffer == "bude")
            {
                tokens.push_back({TokenType::_equals});
                buffer.clear();
            }
            else if (buffer == "izbaci")
            {
                tokens.push_back({TokenType::_print});
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
        else if (peek().value() == '*')
        {
            buffer.push_back(consume());
            tokens.push_back({TokenType::_multiply, buffer, 4});
            buffer.clear();
        }
        else if (peek().value() == '-')
        {
            buffer.push_back(consume());
            tokens.push_back({TokenType::_subtract, buffer, 3});
            buffer.clear();
        }

        else if (peek().value() == '/')
        {
            if (peek(1).value() == '/')
            {
                isComment = true;
                consume();
                continue;
            }
            buffer.push_back(consume());
            tokens.push_back({TokenType::_divide, buffer, 5});
            buffer.clear();
        }
        else if (peek().value() == '+')
        {
            buffer.push_back(consume());
            tokens.push_back({TokenType::_plus, buffer, 2});
            buffer.clear();
        }
        else if (peek().value() == ';')
        {
            consume();
            tokens.push_back({TokenType::_semi});
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

std::optional<char> Tokenizer::peek(int const amount) const
{
    if (m_currentIndex + amount >= m_source.length())
    {
        return {};
    }
    else
    {
        return m_source.at(m_currentIndex + amount);
    }
}

char Tokenizer::consume()
{
    m_currentIndex++;
    return m_source.at(m_currentIndex - 1);
}

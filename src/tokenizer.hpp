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
    _semi,
    _paren_open,
    _paren_close,
    _identifier,
    _let,
    _equals,
};

struct Token
{
    TokenType type;
    std::optional<std::string> value;
};

class Tokenizer
{
public:
    explicit Tokenizer(std::string src);
    std::vector<Token> tokenize();

private:
    const std::string m_source;
    size_t m_currentIndex = 0;
    
    [[nodiscard]] std::optional<char> peek(int amount = 0) const;
    char consume();

};

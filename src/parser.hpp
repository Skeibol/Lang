#pragma once
#include "tokenizer.hpp"
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace node {
    struct ExpressionIntLit {
        Token int_lit;
    };

    struct ExpressionIdentifier {
        Token identifier;
    };

    struct Expression {
        std::variant<ExpressionIntLit, ExpressionIdentifier> type;
    };

    struct StatementExit {
        Expression expression;
    };

    struct StatementEquality {
        Expression expressionLeft;
        Expression expressionRight;
    };


    struct StatementLet {
        Token identifier;
        Expression expression;
    };

    struct Statement {
        std::variant<StatementExit, StatementLet, StatementEquality> type;
    };

    struct Program {
        std::vector<Statement> statements;
    };
} // namespace node

class Parser {
public:
    inline explicit Parser(std::vector<Token> token) : m_tokens(std::move(token)) {
    }

    std::optional<node::Expression> parseExpression() {
        if (checkTokenType(TokenType::_int_lit)) {
            return node::Expression{
                .type = node::ExpressionIntLit{
                    .int_lit = consume()
                }
            };
        } else if (checkTokenType(TokenType::_identifier)) {
            return node::Expression{
                .type = node::ExpressionIdentifier{
                    .identifier = consume()
                }
            };
        } else {
            return {};
        }
    }

    std::optional<node::Statement> parseStatement() {
        node::StatementExit statementExit;

        if (checkTokenType(TokenType::_exit) && checkTokenType(TokenType::_paren_open, 1)) {
            consumeMultiple(2);
            if (auto nodeExpression = parseExpression()) {
                statementExit = {.expression = nodeExpression.value()};
            } else {
                std::cerr << "Invalide expression" << std::endl;
                exit(EXIT_FAILURE);
            }

            if (checkTokenType(TokenType::_paren_close)) {
                consume();
            } else {
                std::cerr << "Expected ')'" << std::endl;
                exit(EXIT_FAILURE);
            }

            if (checkTokenType(TokenType::_semi)) {
                consume();
            } else {
                std::cerr << "Expected ';'" << std::endl;
                exit(EXIT_FAILURE);
            }

            return node::Statement{.type = statementExit};
        }
        if (checkTokenType(TokenType::_let) && checkTokenType(TokenType::_identifier, 1) && checkTokenType(
                TokenType::_equals, 2)) {
            consume(); // Consume 'let'
            node::StatementLet statementLet = node::StatementLet{
                .identifier = consume() // Consume variable name
            };
            consume(); // Consume '='
            if (auto expr = parseExpression()) {
                statementLet.expression = expr.value();
            } else {
                std::cerr << "invalid expressionc" << std::endl;
                exit(EXIT_FAILURE);
            }

            if (checkTokenType(TokenType::_semi)) {
                consume();
            } else {
                std::cerr << "expected token ;" << std::endl;
                exit(EXIT_FAILURE);
            }

            return node::Statement{.type = statementLet};
        }
        if ((checkTokenType(TokenType::_int_lit) || checkTokenType(TokenType::_identifier))
            && checkTokenType(TokenType::_equals, 1) &&
            (checkTokenType(TokenType::_int_lit, 2) || checkTokenType(TokenType::_identifier, 2))) {
            std::cout << "expected once";
            node::StatementEquality statementEquality = node::StatementEquality{};
            if (auto nodeEquality = parseExpression()) {
                statementEquality.expressionLeft = nodeEquality.value();
            } else {
                std::cerr << "Invalid expression on leftside" << std::endl;
                exit(EXIT_FAILURE);
            }
            consume();
            if (auto nodeEquality = parseExpression()) {
                statementEquality.expressionRight = nodeEquality.value();
            } else {
                std::cerr << "Invalid expression on rightside" << std::endl;
                exit(EXIT_FAILURE);
            }
            return node::Statement{.type = statementEquality};
        }

        return {};
    }

    std::optional<node::Program> parseProgram() {
        node::Program program;

        while (peek().has_value()) {
            if (auto stmt = parseStatement()) {
                program.statements.push_back(stmt.value());
            } else {
                consume();
            }
        }

        return program;
    }

private:
    size_t m_currentIndex = 0;
    const std::vector<Token> m_tokens{};

    [[nodiscard]] std::optional<Token> peek(int const amount = 0) const {
        if (m_currentIndex + amount >= m_tokens.size()) {
            return {};
        } else {
            return m_tokens.at(m_currentIndex + amount);
        }
    }

    Token consume() {
        m_currentIndex++;
        return m_tokens.at(m_currentIndex - 1);
    }

    std::vector<Token> consumeMultiple(int const amount) {
        std::vector<Token> consumedTokens;

        for (size_t i = 1; i <= amount; i++) {
            consumedTokens.push_back(consume());
        }

        return consumedTokens;
    }

    [[nodiscard]] bool checkTokenType(TokenType const typeToCheck, int const amount = 0) const {
        return peek(amount).has_value() && peek(amount).value().type == typeToCheck;
    }
};

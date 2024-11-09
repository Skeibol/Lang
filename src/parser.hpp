#pragma once
#include "tokenizer.hpp"
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "arena.hpp"

namespace node {
    struct ExpressionIntLit {
        Token int_lit;
    };

    struct ExpressionIdentifier {
        Token identifier;
    };

    struct Expression {
        std::variant<ExpressionIntLit *, ExpressionIdentifier *> type;
    };

    struct StatementExit {
        Expression *expression;
    };



    struct StatementLet {
        Token identifier;
        Expression *expression;
    };

    struct Statement {
        std::variant<StatementExit *, StatementLet *> type;
    };

    struct Program {
        std::vector<Statement*> statements;
    };
} // namespace node

class Parser {
public:
    inline explicit Parser(std::vector<Token> token) : m_tokens(std::move(token)), m_arena(1024 * 1024 * 4) {
    }

    std::optional<node::Expression*> parseExpression() {
        if (checkTokenType(TokenType::_int_lit)) {
            auto *expressionIntLit = m_arena.allocate<node::ExpressionIntLit>();
            expressionIntLit->int_lit = consume();
            auto *expression = m_arena.allocate<node::Expression>();
            expression->type = expressionIntLit;
            return expression;
        } else if (checkTokenType(TokenType::_identifier)) {
            auto *expressionIdentifier = m_arena.allocate<node::ExpressionIdentifier>();
            expressionIdentifier->identifier = consume();
            auto *expression = m_arena.allocate<node::Expression>();
            expression->type = expressionIdentifier;
            return expression;
        } else {
            return {};
        }
    }

    std::optional<node::Statement*> parseStatement() {


        if (checkTokenType(TokenType::_exit) && checkTokenType(TokenType::_paren_open, 1)) {
            consumeMultiple(2);
            auto *statementExit = m_arena.allocate<node::StatementExit>();
            if (auto nodeExpression = parseExpression()) {
                statementExit->expression = nodeExpression.value();
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
            auto nodeStatement = m_arena.allocate<node::Statement>();
            nodeStatement->type = statementExit;
            return nodeStatement;
        }
        if (checkTokenType(TokenType::_let) && checkTokenType(TokenType::_identifier, 1) && checkTokenType(
                TokenType::_equals, 2)) {
            consume(); // Consume 'let'
            auto statementLet = m_arena.allocate<node::StatementLet>();
            statementLet->identifier = consume(); // consume variable name
            consume(); // Consume '='
            if (auto expr = parseExpression()) {
                statementLet->expression = expr.value();
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

            auto nodeStatement = m_arena.allocate<node::Statement>();
            nodeStatement->type = statementLet;
            return nodeStatement;
        }


        return {};
    }

    std::optional<node::Program*> parseProgram() {
        auto programNode = m_arena.allocate<node::Program>();



        while (peek().has_value()) {
            if (auto stmt = parseStatement()) {
                programNode->statements.push_back(stmt.value());
            } else {
                consume();
            }
        }

        return programNode;
    }


private:
    const std::vector<Token> m_tokens{};
    ArenaAllocator m_arena;
    size_t m_currentIndex = 0;

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

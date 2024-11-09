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
    struct TermIntLit {
        Token int_lit;
    };

    struct TermIdentifier {
        Token identifier;
    };

    struct TermOperand {
        Token op;
    };

    struct Term {
        std::variant<TermIntLit *, TermIdentifier *, TermOperand *> type;
    };

    struct ExpressionEquation {
        std::vector<Term *> terms;
    };

    struct Expression {
        std::variant<ExpressionEquation *> type;
    };

    struct StatementExit {
        Expression *expression;
    };

    struct StatementPrint {
        Expression *expression;
    };


    struct StatementLet {
        Token identifier;
        Expression *expression;
    };

    struct Statement {
        std::variant<StatementExit *, StatementLet *, StatementPrint *> type;
    };

    struct Program {
        std::vector<Statement *> statements;
    };
} // namespace node

class Parser {
public:
    inline explicit Parser(std::vector<Token> token) : m_tokens(std::move(token)), m_arena(1024 * 1024 * 4) {
    }

    std::optional<node::Term *> parseTerm() {
        if (checkTokenType(TokenType::_int_lit)) {
            auto *termIntLit = m_arena.allocate<node::TermIntLit>();
            termIntLit->int_lit = consume();
            auto *term = m_arena.allocate<node::Term>();
            term->type = termIntLit;
            return term;
        } else if (checkTokenType(TokenType::_identifier)) {
            auto *termIdentifier = m_arena.allocate<node::TermIdentifier>();
            termIdentifier->identifier = consume();
            auto *term = m_arena.allocate<node::Term>();
            term->type = termIdentifier;
            return term;
        } else if (checkTokenType(TokenType::_plus)) {
            auto *termOperand = m_arena.allocate<node::TermOperand>();
            termOperand->op = consume();
            auto *term = m_arena.allocate<node::Term>();
            term->type = termOperand;
            return term;
        } else {
            return {};
        }
    }

    std::optional<node::Expression *> parseExpression() {
        if (checkTokenType(TokenType::_int_lit) || checkTokenType(TokenType::_identifier)) {
            auto nodeExpression = m_arena.allocate<node::Expression>();
            auto nodeExpressionEquation = m_arena.allocate<node::ExpressionEquation>();
            while (checkTokenType(TokenType::_int_lit) || checkTokenType(TokenType::_identifier)) {
                if (auto term = parseTerm()) {
                    nodeExpressionEquation->terms.push_back(term.value());
                } else {
                    printErrorMessage("Error parsing integer", "parseExpression , int_lit checking");
                    exit(EXIT_FAILURE);
                }
                if (checkTokenType(TokenType::_plus)) {
                    if (auto term = parseTerm()) {
                        if (handleEquationTerms(term.value()).has_value()) {
                            nodeExpressionEquation->terms.push_back(term.value());
                        } else {
                            std::cout << "Returned empty, implement";
                        }
                    }
                    if (!checkTokenType(TokenType::_int_lit) && !checkTokenType(TokenType::_identifier)) {
                        printErrorMessage("Expected int_lit or identifier after operand", "parseExpression");
                    }
                }
            }
            for (int i = 0; i < operatorStack.size(); ++i) {
                nodeExpressionEquation->terms.push_back(&operatorStack.at(i));
            }

            resetOperatorStack();
            nodeExpression->type = nodeExpressionEquation;
            return nodeExpression;
        }

        return {};
    }


    std::optional<node::Statement *> parseStatement() {
        if (checkTokenType(TokenType::_exit) && checkTokenType(TokenType::_paren_open, 1)) {
            consumeMultiple(2);
            auto *statementExit = m_arena.allocate<node::StatementExit>();
            if (auto nodeExpression = parseExpression()) {
                statementExit->expression = nodeExpression.value();
            } else {
                printErrorMessage("Expected expression in return", "parseStatement, exit checking");
                exit(EXIT_FAILURE);
            }

            if (checkTokenType(TokenType::_paren_close)) {
                consume();
            } else {
                printErrorMessage("Missing parenthesis ) in return", "parseStatement, exit checking");
                exit(EXIT_FAILURE);
            }

            if (checkTokenType(TokenType::_semi)) {
                consume();
            } else {
                printErrorMessage("Expected semicolon in return", "parseStatement, exit checking");
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
                printErrorMessage("Invalid expression in let", "parseStatement, let checking");
                exit(EXIT_FAILURE);
            }

            if (checkTokenType(TokenType::_semi)) {
                consume();
            } else {
                printErrorMessage("Expected semi in let", "parseStatement, exit checking");
                exit(EXIT_FAILURE);
            }

            auto nodeStatement = m_arena.allocate<node::Statement>();
            nodeStatement->type = statementLet;
            return nodeStatement;
        }
        if (checkTokenType(TokenType::_print) && checkTokenType(TokenType::_paren_open, 1)) {
            consume(); // Consume 'print'
            consume(); // Consume 'paren'
            auto statementPrint = m_arena.allocate<node::StatementPrint>();
            if (auto expr = parseExpression()) {
                statementPrint->expression = expr.value();
            } else {
                printErrorMessage("Invalid expression in print", "parseStatement, printing");
                exit(EXIT_FAILURE);
            }
            if (checkTokenType(TokenType::_paren_close)) {
                consume();

            } else {
                printErrorMessage("No closed parens in print", "parseStatement, printing");
                exit(EXIT_FAILURE);
            }

            if (checkTokenType(TokenType::_semi)) {
                consume();
            } else {
                printErrorMessage("Expected semi in print", "parseStatement, printing");
                exit(EXIT_FAILURE);
            }

            auto nodeStatement = m_arena.allocate<node::Statement>();
            nodeStatement->type = statementPrint;
            return nodeStatement;
        }

        return {};
    }

    std::optional<node::Program *> parseProgram() {
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
    std::vector<node::Term> operatorStack{};
    ArenaAllocator m_arena;
    size_t m_currentIndex = 0;

    void pushOperatorStack(const node::Term &term) {
        operatorStack.push_back(term);
    }

    void popOperatorStack() {
    }

    void resetOperatorStack() {
        operatorStack.clear();
    }

    std::optional<node::Term *> handleEquationTerms(node::Term *term) {
        if (operatorStack.empty()) {
            pushOperatorStack(*term); // Push a copy of *term onto the stack
            return {}; // Return an empty optional if operatorStack was empty
        }

        return term; // Return the same term pointer if operatorStack was not empty
    }

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


    void printErrorMessage(std::string const &message, std::string const &functionName) const {
        std::cerr << message << std::endl;
        std::cerr << "In file parser.hpp, function " << functionName << "\n";
        std::cerr << "At index > " << m_currentIndex << "\n" << std::endl;

        exit(EXIT_FAILURE);
    }

    [[nodiscard]] bool checkTokenType(TokenType const typeToCheck, int const amount = 0) const {
        return peek(amount).has_value() && peek(amount).value().type == typeToCheck;
    }
};

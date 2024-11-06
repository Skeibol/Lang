#pragma once
#include "parser.hpp"
#include <sstream>

class Generator {
public:
    inline explicit Generator(node::Program root) : m_program(std::move(root)) {
    }

    void generateExpression(const node::Expression expression) {
        struct ExpressionVisitor {
            Generator *gen;

            void operator()(const node::ExpressionIntLit &expr_int_lit) const {
                gen->m_output << "    mov rax, " << expr_int_lit.int_lit.value.value() << "\n";
                gen->m_output << "    push rax\n";
            }

            void operator()(const node::ExpressionIdentifier &expr_identifier) const {
                // TODO
            }
        };

        ExpressionVisitor visitor({.gen = this});
        std::visit(visitor, expression.type);
    }


    void generateStatement(const node::Statement statement) {
        struct StatementVisitor {
            Generator *gen;

            void operator()(const node::StatementExit &stmt_exit) const {
                gen->generateExpression(stmt_exit.expression);

                gen->m_output << "    mov rax, 60\n"; // TODO
                gen->m_output << "    pop rdi\n";
                gen->m_output << "    syscall\n";
            }

            void operator()(const node::StatementLet &stmt_let) const {
            }
        };

        StatementVisitor visitor{.gen = this};
        std::visit(visitor, statement.type);
    }

    std::string generateProgram() {
        m_output << "global _start\n_start:\n";

        for (const node::Statement &stmt: m_program.statements)
        {
            generateStatement(stmt);
        }

        m_output << "    mov rax, 60\n";
        m_output << "    mov rdi, 0" << "\n";
        m_output << "    syscall\n";

        return m_output.str();

    }

private:
    const node::Program m_program;
    std::stringstream m_output;
};

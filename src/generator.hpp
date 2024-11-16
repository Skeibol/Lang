#pragma once
#include <assert.h>

#include "parser.hpp"
#include <sstream>
#include <unordered_map>

class Generator {
public:
    inline explicit Generator(const node::Program *root) : m_program(root) {
    }

    void generateTerm(const node::Term *term) {
        struct TermVisitor {
            Generator *gen;

            void operator()(const node::TermIntLit *term_int_lit) const {
                gen->m_output << "    mov rax, " << term_int_lit->int_lit.value.value() << "\n";
                gen->push("rax");
            }

            void operator()(const node::TermOperand *term_operand) const {
                if (term_operand->op.value.value() == "+")
                {
                    gen->pop("rax");
                    gen->pop("rbx");
                    gen->m_output << "    add rax,rbx\n";
                    gen->push("rax");
                }
                if (term_operand->op.value.value() == "*")
                {
                    gen->pop("rax");
                    gen->pop("rbx");
                    gen->m_output << "    mul rbx\n";
                    gen->push("rax");
                }
                 if (term_operand->op.value.value() == "-")
                {
                    gen->pop("rax");//3
                    gen->pop("rbx");//14
                    gen->m_output << "    sub rbx,rax\n";
                    gen->push("rbx");
                }
                   if (term_operand->op.value.value() == "/")
                {
                    gen->m_generatedDivisions++;
                    gen->pop("rax");//6
                    gen->pop("rbx");//20
                    gen->m_output << "    xor rcx,rcx\n";
                    gen->m_output << "    xor r8,r8\n";
                    gen->m_output << ".div" << std::to_string(gen->m_generatedDivisions) << ":\n";
                    gen->m_output << "    sub rbx,rax\n";     //rbx 14
                    gen->m_output << "    mov rcx,rbx\n";        //rcx 20  
                    gen->m_output << "    test rbx, rbx \n"; //2 2
                    //gen->m_output << "    cmp rcx,0\n";
                    gen->m_output << "    js .divb" << std::to_string(gen->m_generatedDivisions) << "\n";
                    gen->m_output << "    inc r8\n";
                    gen->m_output << "    cmp rbx,0\n";
                    gen->m_output << "    jne .div" << std::to_string(gen->m_generatedDivisions) << "\n";
                    gen->m_output << ".divb" << std::to_string(gen->m_generatedDivisions) << ":\n";
                    gen->push("r8");
                    

                }
            }

            void operator()(const node::TermIdentifier *term_identifier) const {
                if (gen->m_variables.count(term_identifier->identifier.value.value()) == 0)
                {
                    std::cerr << "Variable(identifier) not declared :" << term_identifier->identifier.value.value();
                    exit(EXIT_FAILURE);
                }
                auto &_variable = gen->m_variables.at(term_identifier->identifier.value.value());

                std::stringstream registerString;
                registerString << "QWORD [rsp + " << (gen->m_stackPointerOffset - _variable.stackLocation - 1) * 8 <<
                        "]";
                gen->push(registerString.str());
            }
        };

        TermVisitor visitor({.gen = this});
        std::visit(visitor, term->type);
    }

    void generateExpression(const node::Expression *expression) {
        struct ExpressionVisitor {
            Generator *gen;

            void operator()(const node::ExpressionEquation *equation) const {
                for (const node::Term *term: equation->terms)
                {
                    gen->generateTerm(term);
                }
            }
        };
        ExpressionVisitor visitor({.gen = this});
        std::visit(visitor, expression->type);
    }


    void generateStatement(const node::Statement *statement) {
        struct StatementVisitor {
            Generator *gen;

            void operator()(const node::StatementExit *stmt_exit) const {
                gen->m_output << "    ;; Exit statement start\n"; // TODO cmp rdx, rax
                gen->generateExpression(stmt_exit->expression);

                gen->m_output << "    mov rax, 60\n"; // TODO
                gen->pop("rdi");
                gen->m_output << "    syscall\n";
                gen->m_output << "    ;; Exit statement end - rax calls exit, rdi provides arguement(exit code)\n";
            }

            void operator()(const node::StatementLet *stmt_let) const {
                if (gen->m_variables.count(stmt_let->identifier.value.value()))
                {
                    std::cerr << "Identifier already declared :" << stmt_let->identifier.value.value();
                    exit(EXIT_FAILURE);
                }
                gen->m_variables.insert({stmt_let->identifier.value.value(), Variable{gen->m_stackPointerOffset}});
                gen->generateExpression(stmt_let->expression);
            }

            void operator()(const node::StatementPrint *stmt_print) const {
                gen->m_output << "    ;; Print statement start\n"; // TODO cmp rdx, rax

                gen->generateExpression(stmt_print->expression);
                gen->pop("rdi");
                gen->m_output << "    call dump\n";
                gen->m_output << "    ;; Print end - rax calls print, rdi provides arguement(what to print)\n";
            }
        };

        StatementVisitor visitor{.gen = this};
        std::visit(visitor, statement->type);
    }


    std::string generateProgram() {
        m_output << R"(dump:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 64
    mov     QWORD [rbp-56], rdi
    mov     QWORD [rbp-8], 1
    mov     eax, 32
    sub     rax, QWORD [rbp-8]
    mov     BYTE [rbp-48+rax], 10
.L2:
    mov     rcx, QWORD [rbp-56]
    mov  rdx, -3689348814741910323
    mov     rax, rcx
    mul     rdx
    shr     rdx, 3
    mov     rax, rdx
    sal     rax, 2
    add     rax, rdx
    add     rax, rax
    sub     rcx, rax
    mov     rdx, rcx
    mov     eax, edx
    lea     edx, [rax+48]
    mov     eax, 31
    sub     rax, QWORD [rbp-8]
    mov     BYTE [rbp-48+rax], dl
    add     QWORD [rbp-8], 1
    mov     rax, QWORD [rbp-56]
    mov     rdx, -3689348814741910323
    mul     rdx
    mov     rax, rdx
    shr     rax, 3
    mov     QWORD [rbp-56], rax
    cmp     QWORD [rbp-56], 0
    jne     .L2
    mov     eax, 32
    sub     rax, QWORD [rbp-8]
    lea     rdx, [rbp-48]
    lea     rcx, [rdx+rax]
    mov     rax, QWORD [rbp-8]
    mov     rdx, rax
    mov     rsi, rcx
    mov     edi, 1
    mov     eax, 0
    mov     rax, 1
    syscall
    nop
    leave
    ret
)";
        m_output << "global _start\n_start:\n";

        for (const node::Statement *stmt: m_program->statements)
        {
            generateStatement(stmt);
        }

        m_output << "    ; safety exit... program probably out\n";
        m_output << "    mov rax, 60\n";
        m_output << "    mov rdi, 0" << "\n";
        m_output << "    syscall\n";

        return m_output.str();
    }

private:
    void push(const std::string &_register) {
        m_output << "    push " << _register << "\n";
        m_stackPointerOffset++;
    }

    void pop(const std::string &_register) {
        m_output << "    pop " << _register << "\n";
        m_stackPointerOffset--;
    }

    struct Variable {
        size_t stackLocation;
    };

    const node::Program *m_program;
    std::stringstream m_output;

    size_t m_stackPointerOffset = 0;
    size_t m_generatedDivisions = 0;
    std::unordered_map<std::string, Variable> m_variables{};
};

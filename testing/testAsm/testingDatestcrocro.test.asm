global _start
_start:
    ; safety exit... program probably out
    mov rax, 60
    mov rdi, 0
    syscall

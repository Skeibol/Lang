global _start
_start:
    mov rax, 25
    push rax
    mov rax, 30
    push rax
    ;; Exit statement start
    mov rax, 23
    push rax
    mov rax, 60
    pop rdi
    syscall
    ;; Exit statement end - rax calls exit, rdi provides arguement(exit code)
    ; safety exit... program probably out
    mov rax, 60
    mov rdi, 0
    syscall

global _start
_start:
    mov rax, 23
    push rax
    push QWORD [rsp + 0]
    ;; Exit statement start
    mov rax, 32
    push rax
    mov rax, 23
    push rax
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 50
    push rax
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 60
    pop rdi
    syscall
    ;; Exit statement end - rax calls exit, rdi provides arguement(exit code)
    ; safety exit... program probably out
    mov rax, 60
    mov rdi, 0
    syscall

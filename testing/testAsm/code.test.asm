global _start
_start:
    mov rax, 20
    push rax
    ;; Exit statement start
    push QWORD [rsp + 0]
    mov rax, 60
    pop rdi
    syscall
    ;; Exit statement end - rax calls exit, rdi provides arguement(exit code)
    ; safety exit... program probably out
    mov rax, 60
    mov rdi, 0
    syscall

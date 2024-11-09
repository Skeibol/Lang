dump:
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
global _start
_start:
    ;; Print statement start
    mov rax, 4
    push rax
    mov rax, 23
    push rax
    pop rax
    pop rbx
    add rax,rbx
    push rax
    pop rdi
    call dump
    ;; Print end - rax calls print, rdi provides arguement(what to print)
    ;; Print statement start
    mov rax, 1
    push rax
    mov rax, 2
    push rax
    pop rax
    pop rbx
    add rax,rbx
    push rax
    pop rdi
    call dump
    ;; Print end - rax calls print, rdi provides arguement(what to print)
    ;; Print statement start
    mov rax, 1
    push rax
    mov rax, 3
    push rax
    pop rax
    pop rbx
    add rax,rbx
    push rax
    pop rdi
    call dump
    ;; Print end - rax calls print, rdi provides arguement(what to print)
    ;; Print statement start
    mov rax, 1
    push rax
    mov rax, 4
    push rax
    pop rax
    pop rbx
    add rax,rbx
    push rax
    pop rdi
    call dump
    ;; Print end - rax calls print, rdi provides arguement(what to print)
    ;; Print statement start
    mov rax, 1
    push rax
    mov rax, 5
    push rax
    pop rax
    pop rbx
    add rax,rbx
    push rax
    pop rdi
    call dump
    ;; Print end - rax calls print, rdi provides arguement(what to print)
    ;; Exit statement start
    mov rax, 2
    push rax
    mov rax, 60
    pop rdi
    syscall
    ;; Exit statement end - rax calls exit, rdi provides arguement(exit code)
    ; safety exit... program probably out
    mov rax, 60
    mov rdi, 0
    syscall

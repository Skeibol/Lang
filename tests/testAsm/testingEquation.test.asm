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
    mov rax, 18
    push rax
    mov rax, 2
    push rax
    ;; Print statement start
    mov rax, 10
    push rax
    mov rax, 5
    push rax
    pop rax
    pop rbx
    mul rbx
    push rax
    mov rax, 20
    push rax
    mov rax, 15
    push rax
    pop rax
    pop rbx
    sub rbx,rax
    push rbx
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 12
    push rax
    mov rax, 4
    push rax
    pop rax
    pop rbx
    xor rcx,rcx
    xor r8,r8
.div1:
    sub rbx,rax
    mov rcx,rbx
    test rbx, rbx 
    js .divb1
    inc r8
    cmp rbx,0
    jne .div1
.divb1:
    push r8
    pop rax
    pop rbx
    add rax,rbx
    push rax
    push QWORD [rsp + 16]
    mov rax, 6
    push rax
    push QWORD [rsp + 24]
    pop rax
    pop rbx
    mul rbx
    push rax
    pop rax
    pop rbx
    sub rbx,rax
    push rbx
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 30
    push rax
    mov rax, 6
    push rax
    pop rax
    pop rbx
    xor rcx,rcx
    xor r8,r8
.div2:
    sub rbx,rax
    mov rcx,rbx
    test rbx, rbx 
    js .divb2
    inc r8
    cmp rbx,0
    jne .div2
.divb2:
    push r8
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 7
    push rax
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 8
    push rax
    mov rax, 3
    push rax
    pop rax
    pop rbx
    mul rbx
    push rax
    mov rax, 9
    push rax
    pop rax
    pop rbx
    sub rbx,rax
    push rbx
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 36
    push rax
    mov rax, 6
    push rax
    pop rax
    pop rbx
    xor rcx,rcx
    xor r8,r8
.div3:
    sub rbx,rax
    mov rcx,rbx
    test rbx, rbx 
    js .divb3
    inc r8
    cmp rbx,0
    jne .div3
.divb3:
    push r8
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 14
    push rax
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 24
    push rax
    mov rax, 3
    push rax
    pop rax
    pop rbx
    xor rcx,rcx
    xor r8,r8
.div4:
    sub rbx,rax
    mov rcx,rbx
    test rbx, rbx 
    js .divb4
    inc r8
    cmp rbx,0
    jne .div4
.divb4:
    push r8
    mov rax, 16
    push rax
    pop rax
    pop rbx
    sub rbx,rax
    push rbx
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 5
    push rax
    push QWORD [rsp + 16]
    pop rax
    pop rbx
    mul rbx
    push rax
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 12
    push rax
    mov rax, 4
    push rax
    pop rax
    pop rbx
    mul rbx
    push rax
    push QWORD [rsp + 24]
    pop rax
    pop rbx
    sub rbx,rax
    push rbx
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 48
    push rax
    mov rax, 6
    push rax
    pop rax
    pop rbx
    xor rcx,rcx
    xor r8,r8
.div5:
    sub rbx,rax
    mov rcx,rbx
    test rbx, rbx 
    js .divb5
    inc r8
    cmp rbx,0
    jne .div5
.divb5:
    push r8
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 11
    push rax
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 56
    push rax
    mov rax, 7
    push rax
    pop rax
    pop rbx
    xor rcx,rcx
    xor r8,r8
.div6:
    sub rbx,rax
    mov rcx,rbx
    test rbx, rbx 
    js .divb6
    inc r8
    cmp rbx,0
    jne .div6
.divb6:
    push r8
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 13
    push rax
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 20
    push rax
    push QWORD [rsp + 16]
    pop rax
    pop rbx
    mul rbx
    push rax
    mov rax, 15
    push rax
    pop rax
    pop rbx
    sub rbx,rax
    push rbx
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 9
    push rax
    mov rax, 3
    push rax
    pop rax
    pop rbx
    xor rcx,rcx
    xor r8,r8
.div7:
    sub rbx,rax
    mov rcx,rbx
    test rbx, rbx 
    js .divb7
    inc r8
    cmp rbx,0
    jne .div7
.divb7:
    push r8
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 15
    push rax
    push QWORD [rsp + 16]
    pop rax
    pop rbx
    mul rbx
    push rax
    mov rax, 25
    push rax
    pop rax
    pop rbx
    sub rbx,rax
    push rbx
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 42
    push rax
    mov rax, 6
    push rax
    pop rax
    pop rbx
    xor rcx,rcx
    xor r8,r8
.div8:
    sub rbx,rax
    mov rcx,rbx
    test rbx, rbx 
    js .divb8
    inc r8
    cmp rbx,0
    jne .div8
.divb8:
    push r8
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 28
    push rax
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 40
    push rax
    mov rax, 8
    push rax
    pop rax
    pop rbx
    xor rcx,rcx
    xor r8,r8
.div9:
    sub rbx,rax
    mov rcx,rbx
    test rbx, rbx 
    js .divb9
    inc r8
    cmp rbx,0
    jne .div9
.divb9:
    push r8
    pop rax
    pop rbx
    add rax,rbx
    push rax
    mov rax, 8
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

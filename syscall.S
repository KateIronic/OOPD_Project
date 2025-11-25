    .text
    .globl syscall3
    .type syscall3, @function
syscall3:
    /* syscall3(long number, long arg1, long arg2, long arg3) */
    movq %rdi, %rax
    movq %rsi, %rdi
    movq %rdx, %rsi
    movq %rcx, %rdx
    syscall
    ret

    .size syscall3, .-syscall3
    .section .note.GNU-stack,"",@progbits

    .file "enterkernel.S"

    .text

    .globl EnterKernel
    .type EnterKernel, @function
EnterKernel:
    move $a1, $a0
    move $a2, $sp
    move $a0, $a1

    move $a1, $ra
    ret

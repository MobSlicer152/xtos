    .file "enterkernel.S"
    .text
    .globl EnterKernel
EnterKernel:
    movq %rcx, %rax
    movq %rdx, %rsp
    movq %r8, %rcx

    # Put entry point as return address
    pushq %rax
    ret

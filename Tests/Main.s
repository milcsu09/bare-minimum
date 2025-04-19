	.text
	.file	"Main"
	.globl	node_create
	.p2align	4, 0x90
	.type	node_create,@function
node_create:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movl	$1, %edi
	movl	$16, %esi
	callq	calloc@PLT
	movq	%rbx, (%rax)
	movq	$0, 8(%rax)
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	node_create, .Lfunc_end0-node_create
	.cfi_endproc

	.globl	__main
	.p2align	4, 0x90
	.type	__main,@function
__main:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%r13
	.cfi_def_cfa_offset 32
	pushq	%r12
	.cfi_def_cfa_offset 40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -48
	.cfi_offset %r12, -40
	.cfi_offset %r13, -32
	.cfi_offset %r14, -24
	.cfi_offset %r15, -16
	movl	$12, %edi
	callq	node_create@PLT
	movq	%rax, %r12
	movl	$34, %edi
	callq	node_create@PLT
	movq	%rax, %r14
	movl	$56, %edi
	callq	node_create@PLT
	movq	%rax, %r15
	movq	%rax, 8(%r14)
	movq	%r14, 8(%r12)
	testq	%r12, %r12
	je	.LBB1_3
	leaq	.Lstr(%rip), %r13
	movq	%r12, %rbx
	.p2align	4, 0x90
.LBB1_2:
	movq	(%rbx), %rsi
	movq	%r13, %rdi
	xorl	%eax, %eax
	callq	printf@PLT
	movq	8(%rbx), %rbx
	testq	%rbx, %rbx
	jne	.LBB1_2
.LBB1_3:
	movq	%r15, %rdi
	callq	free@PLT
	movq	%r14, %rdi
	callq	free@PLT
	movq	%r12, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	jmp	free@PLT
.Lfunc_end1:
	.size	__main, .Lfunc_end1-__main
	.cfi_endproc

	.type	.Lstr,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr:
	.asciz	"%d\n"
	.size	.Lstr, 4

	.section	".note.GNU-stack","",@progbits

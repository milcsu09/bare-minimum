	.text
	.file	"Main"
	.globl	__main
	.p2align	4, 0x90
	.type	__main,@function
__main:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -16
	movl	%edi, 20(%rsp)
	movq	%rsi, 24(%rsp)
	movl	$32, %edi
	callq	malloc@PLT
	movq	%rax, %rbx
	movq	%rax, 8(%rsp)
	movl	$32, %edi
	callq	malloc@PLT
	movq	%rax, (%rbx)
	movq	8(%rsp), %rbx
	movl	$32, %edi
	callq	malloc@PLT
	movq	%rax, 8(%rbx)
	movq	8(%rsp), %rbx
	movl	$32, %edi
	callq	malloc@PLT
	movq	%rax, 16(%rbx)
	movq	8(%rsp), %rbx
	movl	$32, %edi
	callq	malloc@PLT
	movq	%rax, 24(%rbx)
	movq	8(%rsp), %rax
	movq	(%rax), %rdi
	callq	free@PLT
	movq	8(%rsp), %rax
	movq	8(%rax), %rdi
	callq	free@PLT
	movq	8(%rsp), %rax
	movq	16(%rax), %rdi
	callq	free@PLT
	movq	8(%rsp), %rax
	movq	24(%rax), %rdi
	callq	free@PLT
	movq	8(%rsp), %rdi
	callq	free@PLT
	addq	$32, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	__main, .Lfunc_end0-__main
	.cfi_endproc

	.section	".note.GNU-stack","",@progbits

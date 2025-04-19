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

	.globl	node_free
	.p2align	4, 0x90
	.type	node_free,@function
node_free:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	8(%rdi), %rdi
	testq	%rdi, %rdi
	je	.LBB1_2
	callq	node_free@PLT
.LBB1_2:
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	free@PLT
.Lfunc_end1:
	.size	node_free, .Lfunc_end1-node_free
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
	pushq	%r12
	.cfi_def_cfa_offset 32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	pushq	%rax
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -40
	.cfi_offset %r12, -32
	.cfi_offset %r14, -24
	.cfi_offset %r15, -16
	movl	$12, %edi
	callq	node_create@PLT
	movq	%rax, %r14
	movl	$34, %edi
	callq	node_create@PLT
	movq	%rax, %rbx
	movl	$56, %edi
	callq	node_create@PLT
	movq	%rax, 8(%rbx)
	movq	%rbx, 8(%r14)
	testq	%r14, %r14
	je	.LBB2_6
	leaq	.Lstr(%rip), %r12
	leaq	.Lstr.1(%rip), %r15
	movq	%r14, %rbx
	jmp	.LBB2_2
	.p2align	4, 0x90
.LBB2_4:
	movl	$10, %edi
	callq	putchar@PLT
	testq	%rbx, %rbx
	je	.LBB2_6
.LBB2_2:
	movq	(%rbx), %rsi
	movq	%r12, %rdi
	xorl	%eax, %eax
	callq	printf@PLT
	movq	8(%rbx), %rbx
	testq	%rbx, %rbx
	je	.LBB2_4
	movq	%r15, %rdi
	xorl	%eax, %eax
	callq	printf@PLT
	testq	%rbx, %rbx
	jne	.LBB2_2
.LBB2_6:
	movq	%r14, %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	jmp	node_free@PLT
.Lfunc_end2:
	.size	__main, .Lfunc_end2-__main
	.cfi_endproc

	.type	.Lstr,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr:
	.asciz	"%d"
	.size	.Lstr, 3

	.type	.Lstr.1,@object
.Lstr.1:
	.asciz	" -> "
	.size	.Lstr.1, 5

	.type	.Lstr.2,@object
.Lstr.2:
	.asciz	"\n"
	.size	.Lstr.2, 2

	.section	".note.GNU-stack","",@progbits

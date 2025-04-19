	.text
	.file	"Main"
	.globl	lexerNext
	.p2align	4, 0x90
	.type	lexerNext,@function
lexerNext:
	.cfi_startproc
	movq	(%rdi), %rcx
	movb	(%rcx), %dl
	movq	$-1, %rax
	addb	$-40, %dl
	cmpb	$7, %dl
	ja	.LBB0_3
	movzbl	%dl, %edx
	leaq	.LJTI0_0(%rip), %rsi
	movslq	(%rsi,%rdx,4), %rdx
	addq	%rsi, %rdx
	jmpq	*%rdx
.LBB0_2:
	xorl	%eax, %eax
	cmpq	$-1, %rax
	jne	.LBB0_4
	jmp	.LBB0_5
.LBB0_6:
	movl	$1, %eax
	cmpq	$-1, %rax
	jne	.LBB0_4
	jmp	.LBB0_5
.LBB0_9:
	movl	$4, %eax
	cmpq	$-1, %rax
	jne	.LBB0_4
	jmp	.LBB0_5
.LBB0_7:
	movl	$2, %eax
	cmpq	$-1, %rax
	jne	.LBB0_4
	jmp	.LBB0_5
.LBB0_8:
	movl	$3, %eax
.LBB0_3:
	cmpq	$-1, %rax
	je	.LBB0_5
.LBB0_4:
	incq	%rcx
	movq	%rcx, (%rdi)
.LBB0_5:
	retq
.LBB0_10:
	movl	$5, %eax
	cmpq	$-1, %rax
	jne	.LBB0_4
	jmp	.LBB0_5
.Lfunc_end0:
	.size	lexerNext, .Lfunc_end0-lexerNext
	.cfi_endproc
	.section	.rodata,"a",@progbits
	.p2align	2
.LJTI0_0:
	.long	.LBB0_2-.LJTI0_0
	.long	.LBB0_6-.LJTI0_0
	.long	.LBB0_9-.LJTI0_0
	.long	.LBB0_7-.LJTI0_0
	.long	.LBB0_3-.LJTI0_0
	.long	.LBB0_8-.LJTI0_0
	.long	.LBB0_3-.LJTI0_0
	.long	.LBB0_10-.LJTI0_0

	.text
	.globl	__main
	.p2align	4, 0x90
	.type	__main,@function
__main:
	.cfi_startproc
	pushq	%r14
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	pushq	%rax
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -24
	.cfi_offset %r14, -16
	leaq	.Lstr(%rip), %rax
	movq	%rax, (%rsp)
	movq	%rsp, %rdi
	callq	lexerNext@PLT
	cmpq	$-1, %rax
	je	.LBB1_3
	leaq	.Lstr.1(%rip), %r14
	movq	%rsp, %rbx
	.p2align	4, 0x90
.LBB1_2:
	movq	%r14, %rdi
	movq	%rax, %rsi
	xorl	%eax, %eax
	callq	printf@PLT
	movq	%rbx, %rdi
	callq	lexerNext@PLT
	cmpq	$-1, %rax
	jne	.LBB1_2
.LBB1_3:
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	__main, .Lfunc_end1-__main
	.cfi_endproc

	.type	.Lstr,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr:
	.asciz	"(+-*/)"
	.size	.Lstr, 7

	.type	.Lstr.1,@object
.Lstr.1:
	.asciz	"%d\n"
	.size	.Lstr.1, 4

	.section	".note.GNU-stack","",@progbits

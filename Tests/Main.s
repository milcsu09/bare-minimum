	.text
	.file	"Main"
	.globl	my_strlen
	.p2align	4, 0x90
	.type	my_strlen,@function
my_strlen:
	.cfi_startproc
	cmpb	$0, (%rdi)
	je	.LBB0_1
	xorl	%ecx, %ecx
	.p2align	4, 0x90
.LBB0_3:
	leaq	1(%rcx), %rax
	cmpb	$0, 1(%rdi,%rcx)
	movq	%rax, %rcx
	jne	.LBB0_3
	retq
.LBB0_1:
	xorl	%eax, %eax
	retq
.Lfunc_end0:
	.size	my_strlen, .Lfunc_end0-my_strlen
	.cfi_endproc

	.globl	__main
	.p2align	4, 0x90
	.type	__main,@function
__main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	leaq	.Lstr.1(%rip), %rdi
	callq	my_strlen@PLT
	leaq	.Lstr(%rip), %rdi
	movq	%rax, %rsi
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.Lfunc_end1:
	.size	__main, .Lfunc_end1-__main
	.cfi_endproc

	.type	.Lstr,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr:
	.asciz	"%ld\n"
	.size	.Lstr, 5

	.type	.Lstr.1,@object
.Lstr.1:
	.asciz	"Test"
	.size	.Lstr.1, 5

	.section	".note.GNU-stack","",@progbits

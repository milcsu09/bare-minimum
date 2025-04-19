	.text
	.file	"Main"
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3
.LCPI0_0:
	.quad	0x3ff8000000000000
.LCPI0_1:
	.quad	0x4004000000000000
	.text
	.globl	__main
	.p2align	4, 0x90
	.type	__main,@function
__main:
	.cfi_startproc
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movl	%edi, 28(%rsp)
	movq	%rsi, 32(%rsp)
	movabsq	$4609434218613702656, %rax
	movq	%rax, 8(%rsp)
	movabsq	$4612811918334230528, %rax
	movq	%rax, 16(%rsp)
	leaq	.Lstr(%rip), %rdi
	movsd	.LCPI0_0(%rip), %xmm0
	movsd	.LCPI0_1(%rip), %xmm1
	movb	$2, %al
	callq	printf@PLT
	leaq	8(%rsp), %rax
	movq	%rax, (%rsp)
	movsd	8(%rsp), %xmm0
	movsd	16(%rsp), %xmm1
	leaq	.Lstr.1(%rip), %rdi
	movb	$2, %al
	callq	printf@PLT
	movq	(%rsp), %rax
	movabsq	$4621819117588971520, %rcx
	movq	%rcx, (%rax)
	movq	(%rsp), %rax
	movabsq	$4626322717216342016, %rcx
	movq	%rcx, 8(%rax)
	movsd	8(%rsp), %xmm0
	movsd	16(%rsp), %xmm1
	leaq	.Lstr.2(%rip), %rdi
	movb	$2, %al
	callq	printf@PLT
	movq	(%rsp), %rax
	movsd	(%rax), %xmm0
	movsd	8(%rax), %xmm1
	leaq	.Lstr.3(%rip), %rdi
	movb	$2, %al
	callq	printf@PLT
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	__main, .Lfunc_end0-__main
	.cfi_endproc

	.type	.Lstr,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr:
	.asciz	"Vec2 [%g, %g]\n"
	.size	.Lstr, 15

	.type	.Lstr.1,@object
.Lstr.1:
	.asciz	"*Vec2 [%g, %g]\n"
	.size	.Lstr.1, 16

	.type	.Lstr.2,@object
.Lstr.2:
	.asciz	"Vec2 [%g, %g]\n"
	.size	.Lstr.2, 15

	.type	.Lstr.3,@object
.Lstr.3:
	.asciz	"*Vec2 [%g, %g]\n"
	.size	.Lstr.3, 16

	.section	".note.GNU-stack","",@progbits

	.text
	.file	"Main"
	.globl	vec2_create
	.p2align	4, 0x90
	.type	vec2_create,@function
vec2_create:
	.cfi_startproc
	retq
.Lfunc_end0:
	.size	vec2_create, .Lfunc_end0-vec2_create
	.cfi_endproc

	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3
.LCPI1_0:
	.quad	0x4024000000000000
.LCPI1_1:
	.quad	0x4034000000000000
	.text
	.globl	__main
	.p2align	4, 0x90
	.type	__main,@function
__main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movsd	.LCPI1_0(%rip), %xmm0
	movsd	.LCPI1_1(%rip), %xmm1
	callq	vec2_create@PLT
	leaq	.Lstr(%rip), %rdi
	movb	$2, %al
	popq	%rcx
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
.Lfunc_end1:
	.size	__main, .Lfunc_end1-__main
	.cfi_endproc

	.type	.Lstr,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr:
	.asciz	"%f, %f\n"
	.size	.Lstr, 8

	.section	".note.GNU-stack","",@progbits

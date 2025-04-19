	.text
	.file	"Main"
	.globl	__main
	.p2align	4, 0x90
	.type	__main,@function
__main:
	.cfi_startproc
	leaq	.Lstr(%rip), %rdi
	movl	$16, %esi
	xorl	%eax, %eax
	jmp	printf@PLT
.Lfunc_end0:
	.size	__main, .Lfunc_end0-__main
	.cfi_endproc

	.type	.Lstr,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr:
	.asciz	"%ld\n"
	.size	.Lstr, 5

	.section	".note.GNU-stack","",@progbits

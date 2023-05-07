	.section	.rodata
.LC0:
	.string	"test_init!\n"
	.section	.init,"ax",@progbits
#	.globl	_Z9test_initv
#	.type	_Z9test_initv, @function
#_Z9test_initv:
#.LFB0:
#	endbr64
#	pushq	%rbp
#	movq	%rsp, %rbp
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
#	popq	%rbp
#	ret
#.LFE0:
#	.size	_Z9test_initv, .-_Z9test_initv
	.section	.rodata
.LC1:
	.string	"test_fini!\n"
	.section	.fini,"ax",@progbits
#	.globl	_Z9test_finiv
#	.type	_Z9test_finiv, @function
#_Z9test_finiv:
#.LFB1:
#	endbr64
#	pushq	%rbp
#	movq	%rsp, %rbp
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
#	popq	%rbp
#	ret
#.LFE1:
#	.size	_Z9test_finiv, .-_Z9test_finiv

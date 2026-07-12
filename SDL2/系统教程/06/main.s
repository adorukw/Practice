	.file	"main.c"
	.text
	.section	.rodata
.LC0:
	.string	"rb"
.LC1:
	.string	"\346\227\240\346\263\225\345\212\240\350\275\275 %s\357\274\232%s\n"
.LC2:
	.string	"\346\227\240\346\263\225\345\210\233\345\273\272\347\272\271\347\220\206\357\274\232%s\n"
	.text
	.globl	LoadBMPTexture
	.type	LoadBMPTexture, @function
LoadBMPTexture:
.LFB5023:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-32(%rbp), %rax
	leaq	.LC0(%rip), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	SDL_RWFromFile@PLT
	movl	$1, %esi
	movq	%rax, %rdi
	call	SDL_LoadBMP_RW@PLT
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L2
	call	SDL_GetError@PLT
	movq	%rax, %rdx
	movq	-32(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	jmp	.L3
.L2:
	movq	-8(%rbp), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	SDL_CreateTextureFromSurface@PLT
	movq	%rax, -16(%rbp)
	cmpq	$0, -16(%rbp)
	jne	.L4
	call	SDL_GetError@PLT
	movq	%rax, %rsi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
.L4:
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	SDL_FreeSurface@PLT
	movq	-16(%rbp), %rax
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5023:
	.size	LoadBMPTexture, .-LoadBMPTexture
	.section	.rodata
.LC3:
	.string	"\347\272\271\347\220\206\347\244\272\344\276\213"
.LC4:
	.string	"test.bmp"
.LC5:
	.string	"\347\272\271\347\220\206\345\260\272\345\257\270\357\274\232%dx%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB5024:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$262368, %rsp
	movl	%edi, -262356(%rbp)
	movq	%rsi, -262368(%rbp)
	movl	$32, %edi
	call	SDL_Init@PLT
	movl	$0, %r9d
	movl	$600, %r8d
	movl	$800, %ecx
	movl	$536805376, %edx
	movl	$536805376, %esi
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	call	SDL_CreateWindow@PLT
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	movl	$2, %edx
	movl	$-1, %esi
	movq	%rax, %rdi
	call	SDL_CreateRenderer@PLT
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rax
	leaq	.LC4(%rip), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	LoadBMPTexture
	movq	%rax, -48(%rbp)
	movl	$0, -64(%rbp)
	movl	$0, -68(%rbp)
	cmpq	$0, -48(%rbp)
	je	.L6
	leaq	-68(%rbp), %rcx
	leaq	-64(%rbp), %rdx
	movq	-48(%rbp), %rax
	movq	%rcx, %r8
	movq	%rdx, %rcx
	movl	$0, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	SDL_QueryTexture@PLT
	movl	-68(%rbp), %edx
	movl	-64(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
.L6:
	movq	-40(%rbp), %rax
	movl	$256, %r8d
	movl	$256, %ecx
	movl	$1, %edx
	movl	$373694468, %esi
	movq	%rax, %rdi
	call	SDL_CreateTexture@PLT
	movq	%rax, -56(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L7
.L10:
	movl	$0, -8(%rbp)
	jmp	.L8
.L9:
	movl	-8(%rbp), %eax
	movb	%al, -57(%rbp)
	movl	-4(%rbp), %eax
	movb	%al, -58(%rbp)
	movl	-8(%rbp), %edx
	movl	-4(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, %edx
	shrl	$31, %edx
	addl	%edx, %eax
	sarl	%eax
	notl	%eax
	movb	%al, -59(%rbp)
	movzbl	-57(%rbp), %eax
	sall	$24, %eax
	movl	%eax, %edx
	movzbl	-58(%rbp), %eax
	sall	$16, %eax
	orl	%eax, %edx
	movzbl	-59(%rbp), %eax
	sall	$8, %eax
	orl	%edx, %eax
	orb	$-1, %al
	movl	%eax, %ecx
	movl	-4(%rbp), %eax
	sall	$8, %eax
	movl	%eax, %edx
	movl	-8(%rbp), %eax
	addl	%edx, %eax
	movl	%ecx, %edx
	cltq
	movl	%edx, -262224(%rbp,%rax,4)
	addl	$1, -8(%rbp)
.L8:
	cmpl	$255, -8(%rbp)
	jle	.L9
	addl	$1, -4(%rbp)
.L7:
	cmpl	$255, -4(%rbp)
	jle	.L10
	leaq	-262224(%rbp), %rdx
	movq	-56(%rbp), %rax
	movl	$1024, %ecx
	movl	$0, %esi
	movq	%rax, %rdi
	call	SDL_UpdateTexture@PLT
	movl	$1, -12(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -24(%rbp)
	jmp	.L11
.L13:
	movl	-262352(%rbp), %eax
	cmpl	$256, %eax
	jne	.L12
	movl	$0, -12(%rbp)
.L12:
	leaq	-262352(%rbp), %rax
	movq	%rax, %rdi
	call	SDL_PollEvent@PLT
	testl	%eax, %eax
	jne	.L13
	movq	-40(%rbp), %rax
	movl	$255, %r8d
	movl	$0, %ecx
	movl	$0, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	SDL_SetRenderDrawColor@PLT
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	SDL_RenderClear@PLT
	cmpq	$0, -48(%rbp)
	je	.L14
	movl	$50, -262288(%rbp)
	movl	$50, -262284(%rbp)
	movl	-64(%rbp), %eax
	movl	%eax, -262280(%rbp)
	movl	-68(%rbp), %eax
	movl	%eax, -262276(%rbp)
	leaq	-262288(%rbp), %rdx
	movq	-48(%rbp), %rsi
	movq	-40(%rbp), %rax
	movq	%rdx, %rcx
	movl	$0, %edx
	movq	%rax, %rdi
	call	SDL_RenderCopy@PLT
.L14:
	movl	$500, -262240(%rbp)
	movl	$50, -262236(%rbp)
	movl	$200, -262232(%rbp)
	movl	$200, -262228(%rbp)
	leaq	-262240(%rbp), %rdx
	movq	-56(%rbp), %rsi
	movq	-40(%rbp), %rax
	movq	%rdx, %rcx
	movl	$0, %edx
	movq	%rax, %rdi
	call	SDL_RenderCopy@PLT
	movl	$300, -262256(%rbp)
	movl	$350, -262252(%rbp)
	movl	$150, -262248(%rbp)
	movl	$150, -262244(%rbp)
	movl	$75, -262264(%rbp)
	movl	$75, -262260(%rbp)
	leaq	-262264(%rbp), %rdi
	movq	-24(%rbp), %rcx
	leaq	-262256(%rbp), %rdx
	movq	-48(%rbp), %rsi
	movq	-40(%rbp), %rax
	movl	$0, %r9d
	movq	%rdi, %r8
	movq	%rcx, %xmm0
	movq	%rdx, %rcx
	movl	$0, %edx
	movq	%rax, %rdi
	call	SDL_RenderCopyEx@PLT
	movsd	-24(%rbp), %xmm1
	movsd	.LC7(%rip), %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -24(%rbp)
	movsd	-24(%rbp), %xmm0
	comisd	.LC8(%rip), %xmm0
	jb	.L15
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -24(%rbp)
.L15:
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	SDL_RenderPresent@PLT
	movl	$16, %edi
	call	SDL_Delay@PLT
.L11:
	cmpl	$0, -12(%rbp)
	jne	.L12
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	SDL_DestroyTexture@PLT
	movq	-56(%rbp), %rax
	movq	%rax, %rdi
	call	SDL_DestroyTexture@PLT
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	SDL_DestroyRenderer@PLT
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	SDL_DestroyWindow@PLT
	call	SDL_Quit@PLT
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5024:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC7:
	.long	0
	.long	1071644672
	.align 8
.LC8:
	.long	0
	.long	1081507840
	.ident	"GCC: (Debian 14.2.0-19) 14.2.0"
	.section	.note.GNU-stack,"",@progbits

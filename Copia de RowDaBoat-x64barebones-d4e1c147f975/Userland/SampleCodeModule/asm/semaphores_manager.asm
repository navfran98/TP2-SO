GLOBAL _xchg
GLOBAL _yield

_xchg:
    mov rax, rsi
    xchg [rdi], eax
    ret

_yield:
	push rbp
    mov rbp, rsp
	yield
    mov rsp, rbp
    pop rbp
	ret
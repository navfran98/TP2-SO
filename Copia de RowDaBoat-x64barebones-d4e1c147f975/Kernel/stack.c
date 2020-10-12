#include <stack.h>


stackStruct initialize_stack(void * rip, void * base) {
    stackStruct stack;

	// stack.r8 = 0x0;
	// stack.r9 = 0x0;
	// stack.r10 = 0x0;
	// stack.r11 = 0x0;
	// stack.r12 = 0x0;
	// stack.r13 = 0x0;
	// stack.r14 = 0x0;
    // stack.r15 = 0x0;
	// stack.rsi = 0x0;
	// stack.rdi = 0x0;
	// stack.rbp = 0x0;
	// stack.rax = 0x0;
	// stack.rbx = 0x0;
	// stack.rcx = 0x0;
	// stack.rdx = 0x0;

	stack.ss = 0x0;
    stack.cs = 0x8;
    stack.eflags = 0x202;
	stack.rip = (uint64_t) rip;
    stack.rsp = (uint64_t) base;
    // stack.returnAddress = ;

    return stack;
}
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <time.h>   // ya que necesita la funcion timer_handler()
#include <keyboard.h>  // ya que necesita la funcion keyboard_handler()
#include <stdint.h>

static void int_20();
static void int_21();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_20();
			break;
		case 1:
			int_21();
			break;
	}
	return;
}

static void int_20() {
	timer_handler();
}

static void int_21() {
	keyboard_handler();
}

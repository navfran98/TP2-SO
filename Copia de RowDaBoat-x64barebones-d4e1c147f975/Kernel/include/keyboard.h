#ifndef KEYBOARD_H
#define KEYBOARD_H

//#include "stdlib.h"

void keyboard_handler(void);
char get_keyboard_buffer(void);

//info de osdev.wiki
//el make-code y break-code de cada tecla que podria modificar el comportamiento de las demas
#define L_SHIFT                 0x2A
#define L_SHIFT_RELEASED        0xAA
#define R_SHIFT                 0x36
#define R_SHIFT_RELEASED        0xB6
#define L_CTRL                  0x1D
#define L_CTRL_RELEASED         0x9D
#define L_ALT                   0x38
#define L_ALT_RELEASED          0xB8
#define CAPSLOCK                0x3A
#define CAPSLOCK_RELEASED       0xBA
#define TAB                     0x0F
#define TAB_RELEASED            0x8F        
#define UP                      0x48
#define UP_RELEASED             0xC8
#define DOWN                    0x50
#define DOWN_RELEASED           0xD0
#define LEFT                    0x4B
#define LEFT_RELEASED           0xCB    
#define RIGHT                   0x4D
#define RIGHT_RELEASED          0xCD    
#define ESC                     0x01
#define ESC_RELEASED            0x81        
#define F1                      0x3B
#define F1_RELEASED             0xBB
#define F2                      0x3C
#define F2_RELEASED             0xBC
#define F3                      0x3D
#define F3_RELEASED             0xBD
#define F4                      0x3E
#define F4_RELEASED             0xBE
#define F5                      0x3F
#define F5_RELEASED             0xBF
#define F6                      0x40
#define F6_RELEASED             0xC0
#define F7                      0x41
#define F7_RELEASED             0xC1
#define F8                      0x42
#define F8_RELEASED             0xC2
#define F9                      0x43
#define F9_RELEASED             0xC3
#define F10                     0x44
#define F10_RELEASED            0xC4
#define F11                     0x57
#define F11_RELEASED            0xD7
#define F12                     0x58
#define F12_RELEASED            0xD8
#define TAB                     0x0F
#define TAB_R                   0x8F       

#endif
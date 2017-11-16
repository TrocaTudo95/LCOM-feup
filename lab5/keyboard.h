#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_

#include <stdio.h>
#include <stdbool.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
#include "i8254.h"

int kbd_subscribe_int();
int kbd_print_scancodes(bool *isMakecode,
		unsigned long scanCode,
		unsigned long prevScanCode,
		unsigned long prevPrevScanCode,
		unsigned long prevPrevPrevScanCode,
		bool *is4Byte);
int kbd_pressed_esc();
int kbd_int_handler(bool *isMakecode,
		unsigned long *prevScanCode,
		unsigned long *prevPrevScanCode,
		unsigned long *prevPrevPrevScanCode,
		bool *is4Byte);
int kbd_unsubscribe_int();
int kbd_sendCmdToKBD(unsigned long command, unsigned long arg1);
int waitByKey(unsigned long key);
/*
int asm_wrapper_kbd_int_handler(bool *isMakecode,
		unsigned long *prevScanCode,
		unsigned long *prevPrevScanCode,
		unsigned long *prevPrevPrevScanCode,
		bool *is4Byte);
*/

#endif

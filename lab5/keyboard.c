#include "keyboard.h"

static int kbd_hook_id = 2;
unsigned short scanCode; // Used by the ASM implementation of the keyboard handler.

int kbd_subscribe_int() {
	int temp_hook_id = kbd_hook_id;
	if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &temp_hook_id)
			!= OK)
		return -1;
	if (sys_irqenable(&temp_hook_id) != OK)
		return -1;
	return kbd_hook_id;
}

// Also updates isMakecode and is4Byte.
int kbd_print_scancodes(bool *isMakecode, unsigned long scanCode,
		unsigned long prevScanCode, unsigned long prevPrevScanCode,
		unsigned long prevPrevPrevScanCode, //prevPrevPrevPrevPrevPrevPrevPrevPrevPrevPrev
		bool *is4Byte) {
	if (scanCode == 0x2a || scanCode == 0xaa) {
		*is4Byte = !(*is4Byte);
	}

	if ((!(*is4Byte) && prevScanCode != scanCode && prevScanCode != 0xe0
			&& prevScanCode != 0xe1 && prevPrevScanCode != 0xe1)
			|| (*is4Byte
					&& (prevPrevScanCode == 0x2a || prevPrevScanCode == 0xaa))) {
		*isMakecode = !(*isMakecode);
	}

	// Detects Enter's breakcode when execution is started by the user.
	if (scanCode == ENTER_BREAK)
		*isMakecode = false;
	else if (scanCode == ENTER_MAKE)
		*isMakecode = true;

	// Assumes the first byte of 2-byte scancodes is always 0xe0.
	// Assumes the first byte of 3-byte scancodes is always 0xe1.
	if ((!(*is4Byte) && prevScanCode != 0xe0 && prevScanCode != 0xe1
			&& prevPrevScanCode != 0xe1)
			|| (*is4Byte && scanCode != 0x2a && scanCode != 0xaa
					&& prevScanCode != 0xe0 && prevScanCode != 0x2a
					&& prevScanCode != 0xaa
					&& (prevPrevPrevScanCode == 0x2a || prevPrevScanCode == 0xaa))) {
		if (*isMakecode)
			printf("Makecode: ");
		else
			printf("Breakcode: ");
	}

	printf("%x ", scanCode);

	if ((!(*is4Byte) && scanCode != 0xe0 && scanCode != 0xe1
			&& prevScanCode != 0xe1)
			|| (*is4Byte && scanCode != 0x2a && scanCode != 0xaa
					&& scanCode != 0xe0
					&& (prevPrevScanCode == 0x2a || prevPrevScanCode == 0xaa))) {
		printf("\n");
	}
}

int kbd_pressed_esc() {
	unsigned long scanCode;
	sys_inb(OUT_BUF, &scanCode);

	if (scanCode == ESC_BREAK)
		return ESC_RELEASED;
	return OK;
}

int kbd_int_handler(bool *isMakecode, unsigned long *prevScanCode,
		unsigned long *prevPrevScanCode, unsigned long *prevPrevPrevScanCode, //prevPrevPrevPrevPrevPrevPrevPrevPrevPrevPrev
		bool *is4Byte) {
	unsigned long scanCode;
	sys_inb(OUT_BUF, &scanCode);

	kbd_print_scancodes(isMakecode, scanCode, *prevScanCode, *prevPrevScanCode,
			*prevPrevPrevScanCode, is4Byte);

	*prevPrevPrevScanCode = *prevPrevScanCode;
	*prevPrevScanCode = *prevScanCode;
	*prevScanCode = scanCode;
	if (scanCode == ESC_BREAK)
		return ESC_RELEASED;
	return OK;
}

int kbd_unsubscribe_int() {
	int temp_hook_id = kbd_hook_id;
	if (sys_irqdisable(&temp_hook_id) != OK)
		return -1;
	if (sys_irqrmpolicy(&temp_hook_id) != OK)
		return -1;
	return OK;
}

int kbd_sendCmdToKBD(unsigned long command, unsigned long arg1) {
	unsigned long response, stat;

	// Send command.
	while (true) {
		sys_inb(STAT_REG, &stat);
		if ((stat & IBF) == 0) {
			sys_outb(IN_BUF, command);
			break;
		}
	}
	waitByTimer0(20.0 / 1000);
	sys_inb(OUT_BUF, &response);

	if (response == RESEND) {
		printf("Response = RESEND\n");
		while (true) {
			sys_inb(STAT_REG, &stat);
			if ((response & IBF) == 0) {
				sys_outb(KBC_CMD_REG, command);

				break;
			}
		}
		waitByTimer0(20.0 / 1000);
		sys_inb(OUT_BUF, &response);

		if (response == ERROR) {
			printf("Response = ERROR\n");
			return -1;
		}
	}

	// Send argument.
	while (true) {
		sys_inb(STAT_REG, &stat);
		if ((stat & IBF) == 0) {
			sys_outb(IN_BUF, arg1);
			break;
		}
	}
	waitByTimer0(20.0 / 1000);
	sys_inb(OUT_BUF, &response);
	if (response == RESEND) {
		printf("Response = RESEND\n");
		while (true) {
			if ((response & IBF) == 0) {
				sys_outb(IN_BUF, arg1);
				break;
			}
		}
		waitByTimer0(20.0 / 1000);
		sys_inb(OUT_BUF, &response);

		if (response == ERROR) {
			printf("Response = ERROR\n");
			return -1;
		}
	}
	return OK;
}

int waitByKey(unsigned long key) {
	unsigned long scanCode;
	unsigned long irq_kbd_set = BIT(kbd_subscribe_int());
	int r, ipc_status;
	message msg;

	while (true) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_kbd_set) {
					sys_inb(OUT_BUF, &scanCode);
					if (scanCode == key) {
						if (kbd_unsubscribe_int() == -1)
							return -1;
						return OK;
					}
				}
				break;
			}
		}
	}
}

/*
int asm_wrapper_kbd_int_handler(bool *isMakecode, unsigned long *prevScanCode,
		unsigned long *prevPrevScanCode, unsigned long *prevPrevPrevScanCode,
		bool *is4Byte) {
	asm_kbd_int_handler();

	kbd_print_scancodes(isMakecode, scanCode, *prevScanCode, *prevPrevScanCode,
			*prevPrevPrevScanCode, is4Byte);

	*prevPrevPrevScanCode = *prevPrevScanCode;
	*prevPrevScanCode = *prevScanCode;
	*prevScanCode = scanCode;

	if (scanCode == ESC_BREAK)
		return ESC_RELEASED;
	return OK;
}
*/

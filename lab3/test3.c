#include "test3.h"

int kbd_test_scan(unsigned short ass) {
	bool isMakecode = true;
	unsigned long irq_kbd_set = BIT(kbd_subscribe_int());
	int r, ipc_status;
	message msg;
	unsigned long scanCode = 0x00, prevScanCode = 0x00, prevPrevScanCode = 0x00;
	bool is4Byte = false;

	while (true) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive() failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_kbd_set) {
					if (ass == 0) {
						if (ESC_RELEASED == kbd_int_handler(&isMakecode, &scanCode,
															&prevScanCode, &prevPrevScanCode,
															&is4Byte)) {
							if (kbd_unsubscribe_int() != OK)
								return -1;
							else
								return OK;
						}
					} else if (ESC_RELEASED == asm_wrapper_kbd_int_handler(&isMakecode, &scanCode,
																			&prevScanCode, &prevPrevScanCode,
																			&is4Byte)) {
							if (kbd_unsubscribe_int() != OK)
								return -1;
							else
								return OK;
					}
				}
				break;
			default:
				break;
			}
		}
	}

	return OK;
}

// Assumes all LED's are off.
int kbd_test_leds(unsigned short n, unsigned short *leds) {
	// 0 means turned off.
	int ledStatus = 0;

	{
		unsigned short i = 0;
		while (i < n) {
			waitByTimer0(1.0);

			ledStatus ^= LED(leds[i]);
			if (kbd_sendCmdToKBD(SET_KBD_LEDS, ledStatus) != OK) {
				printf("Command error - kbd_sendCmdToKBD()\n");
			}

			++i;
		}
	}
	return OK;
}

int kbd_test_timed_scan(unsigned short n) {
	bool isMakecode = true;
	unsigned long irq_timer_set = BIT(timer_subscribe_int());
	unsigned long irq_kbd_set = BIT(kbd_subscribe_int());
	int r, ipc_status;
	message msg;
	unsigned long scanCode = 0x00, previousScanCode = 0x00, prevPrevScanCode = 0x00;
	bool is4Byte = false;
	unsigned long idleSeconds = 0, intCounter = 0;

	while (idleSeconds < n) {
		if (r = driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive() failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_kbd_set) {
					if (ESC_RELEASED == kbd_int_handler(&isMakecode, &scanCode,
														&previousScanCode, &prevPrevScanCode,
														&is4Byte)) {
						if (kbd_unsubscribe_int() != OK)
							return -1;
						else
							return 0;
					}
					else
						idleSeconds = 0;
				}
				if (msg.NOTIFY_ARG & irq_timer_set) {
					++intCounter;
					if (intCounter % 60 == 0)
						++idleSeconds;
				}

				break;
			default:
				break;
			}
		}
	}
	printf("Execution ended after %d idle seconds.\n", n);
	if (kbd_unsubscribe_int() != OK)
		return -1;
	if (timer_unsubscribe_int() != OK)
		return -1;
	return OK;
}

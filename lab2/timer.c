#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"

// Used in cycle of timer_test_int().
unsigned long g_counter = 0;

static int timer_hook_id = 1;

int timer_set_square(unsigned long timer, unsigned long freq) {
	if (timer > 2)
		return 1;
	unsigned long readBackCmd = TIMER_RB_CMD | TIMER_RB_COUNT_
			| TIMER_RB_SEL(timer);
	unsigned long readBackStatus;
	if (sys_outb(TIMER_CTRL, readBackCmd))
		return 1;
	if (sys_inb(TIMER_0 + timer, &readBackStatus))
		return 1;
	readBackStatus &= 0x1;
	readBackStatus |= (timer << 6) | TIMER_LSB_MSB | TIMER_SQR_WAVE;
	if (sys_outb(TIMER_CTRL, readBackStatus))
		return 1;
	unsigned long timer0Value = TIMER_FREQ / freq;
	if (sys_outb(TIMER_0 + timer, timer0Value & 0xFF))
		return 1;
	if (sys_outb(TIMER_0 + timer, (timer0Value & 0xFF00) >> 8))
		return 1;

	return 0;
}

int timer_subscribe_int(void) {
	int temp_hook_id = timer_hook_id;
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != OK)
		return -1;
	if (sys_irqenable(&timer_hook_id) != OK)
		return -1;
	return temp_hook_id;
}

int timer_unsubscribe_int() {
	int temp_hook_id = timer_hook_id;
	if (sys_irqdisable(&temp_hook_id) != OK) {
		return -1;
	}
	if (sys_irqrmpolicy(&temp_hook_id) != OK) {
		return -1;
	}
	return OK;
}

void timer_int_handler() {
	++g_counter;
}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	unsigned long readBack = TIMER_RB_CMD | TIMER_RB_COUNT_;
	readBack |= TIMER_RB_SEL(timer);

	if (sys_outb(TIMER_CTRL, readBack))
		return 1;
	if (sys_inb(TIMER_0 + timer, (unsigned long *) st))
		return 1;

	return 0;
}

int timer_display_conf(unsigned char conf) {
	printf("Read-back status: %x\n", conf);
	printf("Output: %d\n", (conf & BIT(7)) >> 7);
	printf("Type of access: ");
	switch ((conf & (BIT(5) | BIT(4))) >> 4) {
	case 0:
		printf("0\n");
		break;
	case 1:
		printf("LSB\n");
		break;
	case 2:
		printf("MSB\n");
		break;
	case 3:
		printf("LSB followed by MSB\n");
		break;
	default:
		printf("Returned, case %d\n\n", (conf & (BIT(5) | BIT(4))) >> 4);
		return 1;
	}
	printf("Programmed Mode: %d\n", (conf & (BIT(3) | BIT(2) | BIT(1))) >> 1);
	printf("Counting Mode: ");
	switch (conf & BIT(0)) {
	case 0:
		printf("Binary\n");
		break;
	case 1:
		printf("BCD\n");
		break;
	default:
		printf("Returned, case %d\n\n", conf & BIT(0));
		return 1;
	}

	printf("\n");
	return 0;
}

int timer_test_square(unsigned long freq) {
	if (timer_set_square(0, freq))
		return 1;

	return 0;
}

int timer_test_int(unsigned long time) {
	g_counter = 0;
	unsigned long irq_set = BIT(timer_subscribe_int());
	int ipc_status, r;
	message msg;
	int message_counter = 1;
	time=time*60;

	while (g_counter < time) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */

				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					timer_int_handler();
					if (g_counter % 60 == 0) {
						printf("%d of %u messages, each message is presented each second\n", message_counter, time / 60);
						message_counter++;
					}
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		}
	}

	if (timer_unsubscribe_int())
		return 1;

	return 0;
}

int timer_test_config(unsigned long timer) {
	unsigned char readBackStatus;
	if (timer_get_conf(timer, &readBackStatus))
		return 1;
	if (timer_display_conf(readBackStatus))
		return 1;

	return 0;
}

// Serves the same purpose as the sleep() function.
// n is the number of seconds to wait for.
int waitByTimer0(double n) {
	unsigned long irq_timer_set = BIT(timer_subscribe_int());
	unsigned long intCounter = 0;
	message msg;
	int r, ipc_status;
	while (intCounter < n * 60) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_timer_set) {
					++intCounter;
				}
				break;
			}
		}
	}

	if (timer_unsubscribe_int() != OK) {
		return -1;
	}
	return OK;
}

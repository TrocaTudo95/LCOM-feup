#include "mouse.h"

static int mouse_hook_id = 4;
static bool isSubscribed = false;

int sendCmdToKBC(unsigned long command, unsigned long arg1) {
	unsigned long status, response;

	while (true) {
		if (sys_inb(STAT_REG, &status) != OK)
			return -1;
		if ((status & IBF) == 0) {
			if (sys_outb(KBC_CMD_REG, command) != OK)
				return -1;
			if (sys_inb(OUT_BUF, &response) != OK)
				return -1;
			if (response == RESEND)
				continue;
			if (response == ERROR)
				return -1;
			break;
		}
	}

	while (true) {
		if (sys_inb(STAT_REG, &status) != OK)
			return -1;
		if ((status & IBF) == 0) {
			if (sys_outb(IN_BUF, arg1) != OK)
				return -1;
			if (sys_inb(OUT_BUF, &response) != OK)
				return -1;
			if (response == RESEND)
				continue;
			if (response == ERROR)
				return -1;
			break;
		}
	}

	return OK;
}

int mouse_getConfig(unsigned long *packet) {
	if (sendCmdToKBC(WRITE_BYTE_MOUSE, STAT_REQ) != OK)
		return -1;

	if (sys_inb(OUT_BUF, &(packet[0])) != OK)
		return -1;
	if (sys_inb(OUT_BUF, &(packet[1])) != OK)
		return -1;
	if (sys_inb(OUT_BUF, &(packet[2])) != OK)
		return -1;

	return OK;
}

int mouse_displayConfig(unsigned long *packet) {
	if (packet[0] & BIT(6))
		printf("Mode: Remote\n");
	else {
		printf("Mode: Stream\n");
		if (packet[0] & BIT(5))
			printf("Data reporting: Enabled\n");
		else
			printf("Data reporting: Disabled\n");
	}

	if (packet[0] & BIT(4))
		printf("Scaling: 2:1\n");
	else {
		printf("Scaling: 1:1\n");
	}

	if (packet[0] & BIT(0))
		printf("Left button is currently pressed\n");
	else {
		printf("Left button is currently released\n");
	}

	if (packet[0] & BIT(2))
		printf("Middle button is currently pressed\n");
	else {
		printf("Middle button is currently released\n");
	}

	if (packet[0] & BIT(1))
		printf("Right button is currently pressed\n");
	else {
		printf("Right button is currently released\n");
	}

	printf("Resolution: ");
	switch (packet[1]) {
	case 0:
		printf("1 count/mm");
		break;
	case 1:
		printf("2 counts/mm");
		break;
	case 2:
		printf("4 counts/mm");
		break;
	case 3:
		printf("8 counts/mm");
		break;
	default:
		printf("Invalid resolution value.");
		return -1;
	}
	printf("\n");

	printf("Sample rate: %d\n\n", packet[2]);

	return OK;
}

int mouse_subscribe_int() {
	if (isSubscribed) {
		printf("Subscribing already-subscribed mouse\n");
		return -1;
	}
	int temp_hook_id = mouse_hook_id;
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,
			&mouse_hook_id) != OK) {
		printf("mouse_subscribe_int(): irq set policy error\n");
		return -1;
	}
	if (sys_irqenable(&mouse_hook_id) != OK) {
		printf("mouse_subscribe_int(): irq enable error\n");
		return -1;
	}
	isSubscribed = true;
	return temp_hook_id;
}

int mouse_int_handler(unsigned long *mouseByte) {
	return sys_inb(OUT_BUF, mouseByte);
}

int mouse_get_packet(unsigned long packet[], unsigned long irq_timer_set, size_t *timeIntCounter) {
	bool detectedFirstByte = false;
	unsigned long mouseByte;
	unsigned int counter = 0;

	int mouseSub = mouse_subscribe_int();

	if (-1 == mouseSub) {
		printf("mouse_get_packet() 0: Mouse sub error\n");
		return -1;
	}

	unsigned long irq_mouse_set = BIT(mouseSub);
	int r, ipc_status;
	message msg;

	if (OK != sendCmdToKBC(WRITE_BYTE_MOUSE, SET_STREAM)) {
		printf("mouse_get_packet(): set stream command error\n");
		if (mouse_unsubscribe_int() == -1) {
			printf("mouse_get_packet() stream: Mouse unsub error\n");
		}
		return -1;
	}

	if (OK != sendCmdToKBC(WRITE_BYTE_MOUSE, ENABLE_DATA_REPORT)) {
		printf("mouse_get_packet(): enable data report command error\n");
		if (mouse_unsubscribe_int() == -1) {
			printf("mouse_get_packet() enable data rep: Mouse unsub error\n");
		}
		return -1;
	}


	size_t maxIters = 3;
	for (size_t iter = 0; iter < maxIters; ++iter) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("mouse_get_packet(): driver_receive() failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_timer_set) {
					++(*timeIntCounter);
				}
				if (msg.NOTIFY_ARG & irq_mouse_set) {
					if (mouse_int_handler(&mouseByte) != OK) {
						if (mouse_unsubscribe_int() == -1) {
							printf("mouse_get_packet() 0: Mouse unsub error\n");
						}
						return -1;
					}

					if (mouseByte == ACK) {
						--iter;
						continue;
					}

					if (iter == 0 && !(mouseByte & BIT(3))) {
						if (mouse_unsubscribe_int() == -1) {
							printf("mouse_get_packet() 2: Mouse unsub error\n");
						}
						return NO_PACKETS;
					}

					if (!detectedFirstByte && mouseByte & BIT(3) && mouseByte != ACK) {
						detectedFirstByte = true;
					}
					if (detectedFirstByte) {
						packet[counter] = mouseByte;
						++counter;
					}

					if (counter == 3) {
						if (OK != sendCmdToKBC(WRITE_BYTE_MOUSE, DISABLE_DATA_REPORT)) {
							printf("mouse_get_packet(): disable data report command error\n");
							if (mouse_unsubscribe_int() == -1) {
								printf("mouse_get_packet() disable data rep: Mouse unsub error\n");
							}
							return -1;
						}
						if (mouse_unsubscribe_int() == -1) {
							printf("mouse_get_packet() 3: Mouse unsub error\n");
							return -1;
						}
						return OK;
					}
				}
				break;
			default:
				break;
			}
		}
	}

	if (mouse_unsubscribe_int() == -1) {
		printf("mouse_get_packet() 4: Mouse unsub error\n");
		return -1;
	}

	return NO_PACKETS;
}

void printMousePacket(short packet[]) {
	short firstByte = packet[0];
	short secondByte = packet[1];
	short thirdByte = packet[2];
	long x = 0xFFFFFF00, y = 0xFFFFFF00;

	printf("B1=0x%x ", firstByte);
	printf("B2=0x%x ", secondByte);
	printf("B3=0x%x ", thirdByte);

	printf("LB=%d ", firstByte & BIT(0));
	printf("MB=%d ", (firstByte & BIT(2)) >> 2);
	printf("RB=%d ", (firstByte & BIT(1)) >> 1);

	printf("XOV=%d ", (firstByte & BIT(6)) >> 6);
	printf("YOV=%d ", (firstByte & BIT(7)) >> 7);

	if (firstByte & BIT(4)) {
		x |= secondByte;
	}
	else
		x = secondByte;

	if (firstByte & BIT(5)) {
		y |= thirdByte;
	}
	else
		y = thirdByte;

	printf("X=%d ", x);
	printf("Y=%d\n", y);
}

int mouse_unsubscribe_int() {
	if (!isSubscribed) {
		printf("Unsubscribing already-unsubscribed mouse\n");
		return -1;
	}
	int temp_hook_id = mouse_hook_id;
	mouse_hook_id = 4;
	if (sys_irqdisable(&temp_hook_id) != OK) {
		return -1;
	}
	if (sys_irqrmpolicy(&temp_hook_id) != OK) {
		return -1;
	}
	isSubscribed = false;
	return OK;
}

bool mouse_overflow(unsigned long packet[]) {
	return (packet[0] & X_OVF || packet[0] & Y_OVF);
}

bool mouse_pressedLMB(unsigned long packet[]) {
	return (packet[0] & LMB);
}

bool mouse_pressedMMB(unsigned long packet[]) {
	return (packet[0] & MMB);
}

bool mouse_pressedRMB(unsigned long packet[]) {
	return (packet[0] & RMB);
}

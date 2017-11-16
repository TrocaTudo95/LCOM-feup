#include "test4.h"
#include "mouse.h"

static int mouse_hook_id = 4;

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
	int temp_hook_id = mouse_hook_id;
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,
			&mouse_hook_id) != OK)
		return -1;
	if (sys_irqenable(&mouse_hook_id) != OK)
		return -1;
	return temp_hook_id;
}

int mouse_int_handler(unsigned long *mouseByte) {
	return sys_inb(OUT_BUF, mouseByte);
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

gest_transition getTransition(gest_state st, short *packet, short *currentLength, short length) {
	static int wasMovingRight = 2;
	if (wasMovingRight == 2) {
		wasMovingRight = !((packet[0] & BIT(4)) * (packet[0] & BIT(5)));
	}
	switch (st) {
	case INIT:
		if (packet[0] & BIT(1)) {
			return RDOWN;
		}
		break;
	case DRAW:
		if ((packet[0] & BIT(1)) == 0) {
			return RUP;
		}

		long x = 0xFFFFFF00, y = 0xFFFFFF00;
		if (packet[0] & BIT(4)) {
			x |= packet[1];
		} else
			x = packet[1];

		if (packet[0] & BIT(5)) {
			y |= packet[2];
		} else
			y = packet[2];

		if ((length > 0 && wasMovingRight && x > 0 && y > 0)
				|| (length < 0 && !wasMovingRight && x < 0 && y < 0)) {
			*currentLength += y;
			return POS_SLOPE_MOV;
		}
		else if (x * y < 0) {
			return NEG_SLOPE_MOV;
		}
		else if (x == 0 || y == 0) {
			return ZERO_COMPONENT_MOV;
		}
		else {
			wasMovingRight = !wasMovingRight;
			return INV_DIR;
		}
		break;
	default:
		printf("Invalid st state.\n");
		break;
	}
}

gest_state updateState(short *packet, short length) {
	static gest_state st = INIT;
	static short currentLength = 0;
	switch (st) {
	case INIT:
		if (getTransition(INIT, packet, &currentLength, length) == RDOWN)
			st = DRAW;
		break;
	case DRAW:
		switch (getTransition(DRAW, packet, &currentLength, length)) {
		case RUP:
		case NEG_SLOPE_MOV:
		case ZERO_COMPONENT_MOV:
		case INV_DIR:
			currentLength = 0;
			st = INIT;
			break;
		case POS_SLOPE_MOV:
			if (abs(currentLength) >= abs(length)) {
				st = COMP;
				return st;
			}
			break;
		default:
			printf("Invalid transition from DRAW state.\n");
			break;
		}
		break;
	default:
		printf("Invalid st state: %d.\n", st);
		break;
	}
	return st;
}

int mouse_unsubscribe_int() {
	int temp_hook_id = mouse_hook_id;
	if (sys_irqdisable(&temp_hook_id) != OK) {
		return -1;
	}
	if (sys_irqrmpolicy(&temp_hook_id) != OK) {
		return -1;
	}
	return OK;
}

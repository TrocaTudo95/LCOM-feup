#include "test4.h"
#include "mouse.h"


int test_packet(unsigned short cnt) {
	bool detectedFirstByte = false;
	short packet[3] = { 0, 0, 0 };
	unsigned long mouseByte;
	unsigned int counter = 0, packetCounter = 0;
	unsigned long trash;

	unsigned long irq_mouse_set = BIT(mouse_subscribe_int());
	int r, ipc_status;
	message msg;

	if (OK != sendCmdToKBC(WRITE_BYTE_MOUSE, SET_STREAM))
		return -1;
	if (OK != sendCmdToKBC(WRITE_BYTE_MOUSE, ENABLE_DATA_REPORT))
		return -1;

	while (true) {
		if (packetCounter == cnt) {
			sendCmdToKBC(WRITE_BYTE_MOUSE, DISABLE_DATA_REPORT);
			if (mouse_unsubscribe_int() != OK)
				return -1;
			return OK;
		}
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive() failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_mouse_set) {
					if (mouse_int_handler(&mouseByte) != OK) {
						mouse_unsubscribe_int();
						return -1;
					}
					if (mouseByte & BIT(3)) {
						detectedFirstByte = true;
					}
					if (detectedFirstByte) {
						packet[counter] = mouseByte;
						++counter;
					}

					if (counter == 3) {
						counter = 0;
						detectedFirstByte = false;
						++packetCounter;
						printMousePacket(packet);
						size_t i = 0;
						while (i < 3) {
							packet[i] = 0;
							++i;
						}
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

int test_async(unsigned short idle_time) {
	bool detectedFirstByte = false;
	unsigned short numTimerInts = 0;
	short packet[3] = { 0, 0, 0 };
	unsigned long mouseByte;
	unsigned int counter = 0;

	unsigned long irq_mouse_set = BIT(mouse_subscribe_int());
	unsigned long irq_timer_set = BIT(timer_subscribe_int());
	int r, ipc_status;
	message msg;

	if (OK != sendCmdToKBC(WRITE_BYTE_MOUSE, SET_STREAM))
		return -1;
	if (OK != sendCmdToKBC(WRITE_BYTE_MOUSE, ENABLE_DATA_REPORT))
		return -1;

	while (numTimerInts < idle_time * 60) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive() failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_mouse_set) {
					numTimerInts = 0;
					if (mouse_int_handler(&mouseByte) != OK) {
						mouse_unsubscribe_int();
						return -1;
					}
					if (mouseByte & BIT(3)) {
						detectedFirstByte = true;
					}
					if (detectedFirstByte) {
						packet[counter] = mouseByte;
						++counter;
					}

					if (counter == 3) {
						counter = 0;
						detectedFirstByte = false;
						printMousePacket(packet);
						size_t i = 0;
						while (i < 3) {
							packet[i] = 0;
							++i;
						}
					}
				}
				if (msg.NOTIFY_ARG & irq_timer_set) {
					++numTimerInts;
				}
				break;
			default:
				break;
			}
		}
	}

	if (OK != sendCmdToKBC(WRITE_BYTE_MOUSE, DISABLE_DATA_REPORT))
		return -1;
	if (mouse_unsubscribe_int() != OK) {
		return -1;
	}
	if (timer_unsubscribe_int() != OK) {
		return -1;
	}

	printf("Function returned after %d idle seconds.\n", idle_time);
	return OK;
}

int test_config(void) {
	unsigned long packet[3];

	if (mouse_subscribe_int() == -1) {
		return -1;
	}

	if (mouse_getConfig(packet) != OK)
		return -1;
	if (mouse_displayConfig(packet) != OK)
		return -1;

	if (mouse_unsubscribe_int() != OK) {
		return -1;
	}

	return OK;
}

int test_gesture(short length) {
	bool reachedFinalState = false;
	bool detectedFirstByte = false;
	short packet[3] = { 0, 0, 0 };
	unsigned long mouseByte, trash;
	unsigned int counter = 0;

	unsigned long irq_mouse_set = BIT(mouse_subscribe_int());
	int r, ipc_status;
	message msg;

	if (OK != sendCmdToKBC(WRITE_BYTE_MOUSE, SET_STREAM))
		return -1;
	if (OK != sendCmdToKBC(WRITE_BYTE_MOUSE, ENABLE_DATA_REPORT))
		return -1;

	while (!reachedFinalState) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive() failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_mouse_set) {
					if (mouse_int_handler(&mouseByte) != OK) {
						mouse_unsubscribe_int();
						return -1;
					}
					if (mouseByte & BIT(3)) {
						if (mouseByte != ACK)
						detectedFirstByte = true;
					}
					if (detectedFirstByte) {
						packet[counter] = mouseByte;
						++counter;
					}

					if (counter == 3) {
						counter = 0;
						detectedFirstByte = false;
						printMousePacket(packet);
						if (updateState(packet, length) == COMP) {
							reachedFinalState = true;
							break;
						}
						size_t i = 0;
						while (i < 3) {
							packet[i] = 0;
							++i;
						}
					}
				}
				break;
			default:
				break;
			}
		}
	}

	printf("Moved %d units vertically on a positive slope with the RMB pressed.\n", length);
		if (OK != sendCmdToKBC(WRITE_BYTE_MOUSE, DISABLE_DATA_REPORT)) {
			printf("DISABLE_DATA_REPORT command failed.\n");
			return -1;

		}
	if (mouse_unsubscribe_int() != OK) {
		printf("Mouse unsubscription failed.\n");
		return -1;
	}
	printf("\n");
	return OK;
}

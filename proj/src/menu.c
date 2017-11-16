#include "menu.h"
#include "game.h"
#include "keyboard.h"
#include "timer.h"
#include "bitmap.h"

void menu_options(unsigned long irq_kbd_set, unsigned *difficulty, unsigned short *mode);
int menu_difficulty(unsigned long irq_kbd_set);
void menu_resolution(unsigned long irq_kbd_set, unsigned short *mode);
void game_over();

int initial_menu() {
	initLowArea();
	Bitmap* menu = loadBitmap("/home/lcom/proj/res/images/menu_600.bmp");
	if (menu == NULL) {
		printf("initial_menu(): Invalid bmp\n");
		return -1;
	}
	int menuHeight = menu->bitmapInfoHeader.height;

	unsigned short mode = MODE_768_64K;
	unsigned difficulty = DIFF_NORMAL;
	unsigned int points = 0;

	unsigned long scanCode;
	int kbdSubRes = kbd_subscribe_int();
	if (kbdSubRes == -1) {
		printf("initial_menu(): Keyboard sub error\n");
		return -1;

	}
	unsigned long irq_kbd_set = BIT(kbdSubRes);
	int r, ipc_status;
	message msg;

	vg_init(mode);
	while (true) {
		int menuY = (getVertRes() - menuHeight) / 2;
		drawBitmap(menu, getHorRes() / 2, menuY, ALIGN_CENTER);
		vg_updateMainBuffer();

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_kbd_set) {
					sys_inb(OUT_BUF, &scanCode);
					if (scanCode == KEY_3_MAKE) {
						vg_exit();
						if (kbd_unsubscribe_int() == -1) {
							printf("initial_menu(): Keyboard unsub error\n");
							return -1;
						}
						return OK;
					}
					if (scanCode == KEY_2_MAKE) {
						menu_options(irq_kbd_set, &difficulty, &mode);
					}
					if (scanCode == KEY_1_MAKE) {
						vg_exit();
						vg_init(MODE_768_256);
						points = 0;
						switch (play(difficulty, &points)) {
						case GAME_SUCCESS:
							vg_init(mode);
							break;
						case GAME_LOST:
							vg_init(mode);
							game_over(&points);
							break;
						case GAME_EXIT:
							vg_init(mode);
							break;
						default:
							break;
						}
					}
					break;
				}
			}
		}
	}
}

void menu_options(unsigned long irq_kbd_set, unsigned *difficulty, unsigned short *mode) {
	Bitmap *menu = loadBitmap("/home/lcom/proj/res/images/options.bmp");
	if (menu == NULL) {
		printf("menu_options(): Invalid bmp\n");
		return;
	}
	int menuHeight = menu->bitmapInfoHeader.height;
	int menuY = (getVertRes() - menuHeight) / 2;

	unsigned long scanCode;
	int r, ipc_status;
	message msg;

	while (true) {
		drawBitmap(menu, getHorRes() / 2, menuY, ALIGN_CENTER);
		vg_updateMainBuffer();

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_kbd_set) {
					sys_inb(OUT_BUF, &scanCode);
					switch (scanCode) {
					case KEY_1_MAKE:
						deleteBitmap(menu);
						*difficulty = menu_difficulty(irq_kbd_set);
						return;
					case KEY_2_MAKE:
						deleteBitmap(menu);
						unsigned short prev_mode = *mode;
						menu_resolution(irq_kbd_set, mode);
						if (*mode != prev_mode) {
							vg_exit();
							vg_init(*mode);
						}
						return;
					case KEY_3_MAKE:
						deleteBitmap(menu);
						return;
					}
				}
			}
		}
	}
}

int menu_difficulty(unsigned long irq_kbd_set) {
	Bitmap *menu = loadBitmap("/home/lcom/proj/res/images/difficulty.bmp");
	if (menu == NULL) {
		printf("menu_difficulty(): Invalid bmp\n");
		return -1;
	}
	int menuHeight = menu->bitmapInfoHeader.height;
	int menuY = (getVertRes() - menuHeight) / 2;

	unsigned long scanCode;
	int r, ipc_status;
	message msg;

	while (true) {
		drawBitmap(menu, getHorRes() / 2, menuY, ALIGN_CENTER);
		vg_updateMainBuffer();

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_kbd_set) {
					sys_inb(OUT_BUF, &scanCode);
					if (scanCode == KEY_1_MAKE) {
						deleteBitmap(menu);
						return DIFF_EASY;
					}
					if (scanCode == KEY_2_MAKE) {
						deleteBitmap(menu);
						return DIFF_NORMAL;
					}

					if (scanCode == KEY_3_MAKE) {
						deleteBitmap(menu);
						return DIFF_HARD;
					}
					break;
				}
			}
		}
	}
}

void menu_resolution(unsigned long irq_kbd_set, unsigned short *mode) {
	Bitmap *menu = loadBitmap("/home/lcom/proj/res/images/resolution.bmp");
	if (menu == NULL) {
		printf("menu_options(): Invalid bmp\n");
		return;
	}
	int menuHeight = menu->bitmapInfoHeader.height;
	int menuY = (getVertRes() - menuHeight) / 2;

		unsigned long scanCode;
	int r, ipc_status;
	message msg;

	while (true) {
		drawBitmap(menu, getHorRes() / 2, menuY, ALIGN_CENTER);
		vg_updateMainBuffer();

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_kbd_set) {
					sys_inb(OUT_BUF, &scanCode);
					switch (scanCode) {
					case KEY_1_MAKE:
						deleteBitmap(menu);
						*mode = MODE_600_64K;
						return;
					case KEY_2_MAKE:
						deleteBitmap(menu);
						*mode = MODE_768_64K;
						return;
					case KEY_3_MAKE:
						deleteBitmap(menu);
						*mode = MODE_1024_64K;
						return;
					case KEY_4_MAKE:
						return;
					}
					break;
				}
			}
		}
	}
}



void draw_digit_bmp(size_t num, short x, short y) {
	Bitmap *number;
	switch (num) {
	case 0:
		number=loadBitmap("/home/lcom/proj/res/images/numbers/num_0.bmp");
		break;
	case 1:
		number=loadBitmap("/home/lcom/proj/res/images/numbers/num_1.bmp");
		break;
	case 2:
		number=loadBitmap("/home/lcom/proj/res/images/numbers/num_2.bmp");
		break;
	case 3:
		number=loadBitmap("/home/lcom/proj/res/images/numbers/num_3.bmp");
		break;
	case 4:
		number=loadBitmap("/home/lcom/proj/res/images/numbers/num_4.bmp");
		break;
	case 5:
		number=loadBitmap("/home/lcom/proj/res/images/numbers/num_5.bmp");
		break;
	case 6:
		number=loadBitmap("/home/lcom/proj/res/images/numbers/num_6.bmp");
		break;
	case 7:
		number=loadBitmap("/home/lcom/proj/res/images/numbers/num_7.bmp");
		break;
	case 8:
		number=loadBitmap("/home/lcom/proj/res/images/numbers/num_8.bmp");
		break;
	case 9:
		number=loadBitmap("/home/lcom/proj/res/images/numbers/num_9.bmp");
		break;
	default:
		printf("draw_digit(): Invalid num (%u)\n", num);
	}
	drawBitmap(number, x, y, ALIGN_CENTER);
	deleteBitmap(number);
}

void game_over(unsigned short *points) {
	Bitmap *game_over = loadBitmap("/home/lcom/proj/res/images/game_over.bmp");
	if (game_over == NULL) {
		printf("game_over(): Invalid bmp\n");
		return;
	}

	Bitmap *exampleDigit = loadBitmap("/home/lcom/proj/res/images/numbers/num_0.bmp");
	int digitWidth = exampleDigit->bitmapInfoHeader.width - 10;
	deleteBitmap(exampleDigit);

	unsigned int digits = getNumDecimalSlots(*points);
	int overHeight = game_over->bitmapInfoHeader.height;
	int menuY = (getVertRes() - overHeight) / 2;
	drawBitmap(game_over, getHorRes() / 2, menuY, ALIGN_CENTER);

	unsigned x = getHorRes() / 2 + 70;
	unsigned y = menuY + 405;

	for (size_t i = 0; i < digits; i++) {
		draw_digit_bmp((*points) % 10, x, y);
		*points /= 10;
		x -= digitWidth;
	}
	vg_updateMainBuffer();

	while (!pressedEsc()) {	}

	deleteBitmap(game_over);
	return;

}

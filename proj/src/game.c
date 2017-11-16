#include "game.h"
#include <minix/driver.h>
#include <stdlib.h>
#include "MiscData.h"
#include "Snake.h"
#include "RTC.h"
#include "timer.h"
#include "keyboard.h"


bool belongsInRange(short elem, short leftBound, short rightBound) {
	return (elem >= leftBound && elem <= rightBound);
}

bool rangeIntersect(short x1, short x2, short y1, short y2) {
	return (belongsInRange(x1, y1, y2) || belongsInRange(x2, y1, y2)
			|| belongsInRange(y1, x1, x2) || belongsInRange(y2, x1, x2));
}

int getDirection(unsigned long packet[], bool *isHorizontal, short *delta, unsigned long irq_timer_set, size_t *timeIntCounter, Rectangle *eatableDim) {
	int mousePacketResult = mouse_get_packet(packet, irq_timer_set, timeIntCounter);
	if (NO_PACKETS == mousePacketResult || -1 == mousePacketResult) {
		return INV_PACKETS;
	}
	unsigned long miscInfo = packet[0];
	long x_delta = 0xFFFFFF00;
	long y_delta = 0xFFFFFF00;

	if (miscInfo & BIT(4))
		x_delta |= packet[1];
	else
		x_delta = packet[1];

	if (miscInfo & BIT(5))
		y_delta |= packet[2];
	else {
		y_delta = packet[2];
	}

	y_delta = -y_delta;

//	printf("x_delta = %d\ny_delta = %d\n", x_delta, y_delta);

	//Horizontal or vertical.
	*isHorizontal = (abs(x_delta) > abs(y_delta));

	//Positive or negative.
	if (*isHorizontal) {
		if (x_delta < 0) {
			*delta = -abs(*delta);
		} else {
			*delta = abs(*delta);
		}
	} else {
		if (y_delta < 0) {
			if (*delta > 0) {
				*delta = -(*delta);
			}
		} else if (*delta < 0) {
			*delta = -(*delta);
		}
	}

	return OK;
}

bool canEat(bool isHorizontal, short delta, Rectangle snakeDim, Rectangle eatDim) {
	bool ate = false;
	short x_snake = snakeDim.xi;
	short y_snake = snakeDim.yi;
	short headLength;
	short headWidth;
	if (isHorizontal) {
		headLength = snakeDim.width;
		headWidth = snakeDim.height;
	}
	else {
		headLength = snakeDim.height;
		headWidth = snakeDim.width;
	}
	short x_eat = eatDim.xi;
	short y_eat = eatDim.yi;
	short eatableWidth = eatDim.width;
	short eatableHeight = eatDim.height;


	if (isHorizontal) {
		if (delta > 0) {
			if (x_snake + delta + headLength - 1 >= x_eat
					&& x_snake <= x_eat + eatableWidth - 1
					&& rangeIntersect(y_snake, y_snake + headWidth - 1, y_eat,
							y_eat + eatableHeight - 1)) {
				ate = true;
			}
		} else {
			if (x_snake + delta <= x_eat + eatableWidth - 1
					&& x_snake + headLength - 1 >= x_eat
					&& rangeIntersect(y_snake, y_snake + headWidth - 1, y_eat,
							y_eat + eatableHeight - 1)) {
				ate = true;
			}
		}
	} else {
		if (delta > 0) {
			if (y_snake + delta + headLength - 1 >= y_eat
					&& y_snake <= y_eat + eatableHeight - 1
					&& rangeIntersect(x_snake, x_snake + headWidth - 1, x_eat,
							x_eat + eatableWidth - 1)) {
				ate = true;
			}
		} else {
			if (y_snake + delta <= y_eat + eatableHeight - 1
					&& y_snake + headLength - 1 >= y_eat
					&& rangeIntersect(x_snake, x_snake + headWidth - 1, x_eat,
							x_eat + eatableWidth - 1)) {
				ate = true;
			}
		}
	}

	return ate;
}

//TODO: Detect ESC makes.
// Commented out ESC make detection until we figure out why ESC makes appear when not pressing ESC.
bool pressedEsc() {
	int res = kbd_pressed_esc();
	/*if (ESC_PRESSED == res) {
		printf("ESC make\n");
	} else*/ if (ESC_RELEASED == res) {
		printf("ESC break\n");
	}
	return (/*ESC_PRESSED == res ||*/ ESC_RELEASED == res);
}

int updateSnakePos(bool prev_isHorizontal, short prev_delta, bool isHorizontal, short *delta, Rectangle *snakeDim) {
	if (isHorizontal == prev_isHorizontal) {
		if ((prev_delta > 0 && *delta < 0) || (prev_delta < 0 && *delta > 0))
			*delta = prev_delta;
	}

	if (isHorizontal)
		snakeDim->xi += *delta;
	else
		snakeDim->yi += *delta;

	if (prev_isHorizontal != isHorizontal) {
		short t_width = snakeDim->width;
		snakeDim->width = snakeDim->height;
		snakeDim->height = t_width;
	}

	if (snakeDim->xi < 55 || snakeDim->xi >= (getHorRes() - snakeDim->width)-55
			|| snakeDim->yi < 55 || snakeDim->yi >= (getVertRes() - snakeDim->height)-55) {
		return OUT_BOUNDS;
	}

	return OK;
}

void spawnEat(Snake *snake, Rectangle *eatDim, char *eatable[]) {
	do {
		eatDim->xi = 100 + (rand() % (getHorRes() - eatDim->width + 1 - 200)); //100 = border.
		eatDim->yi = 100 + (rand() % (getVertRes() - eatDim->height + 1 - 200)); //100 = border.
	} while (positionConflict(snake->head, *eatDim) || positionConflictArray(*eatDim, snake->bodyCoords, snake->length, getXpmWidth(snake->xpmBody), getXpmHeight(snake->xpmBody)));

	vg_draw_xpm(eatDim->xi, eatDim->yi, eatable);
}

void draw_digit(size_t num, short x, short y) {
	switch (num) {
	case 0:
		vg_draw_xpm(x, y, numZero);
		break;
	case 1:
		vg_draw_xpm(x, y, numOne);
		break;
	case 2:
		vg_draw_xpm(x, y, numTwo);
		break;
	case 3:
		vg_draw_xpm(x, y, numThree);
		break;
	case 4:
		vg_draw_xpm(x, y, numFour);
		break;
	case 5:
		vg_draw_xpm(x, y, numFive);
		break;
	case 6:
		vg_draw_xpm(x, y, numSix);
		break;
	case 7:
		vg_draw_xpm(x, y, numSeven);
		break;
	case 8:
		vg_draw_xpm(x, y, numEight);
		break;
	case 9:
		vg_draw_xpm(x, y, numNine);
		break;
	default:
		printf("draw_digit(): Invalid num (%u)\n", num);
	}
}

unsigned getNumDecimalSlots(size_t num) {
	unsigned numSlots = 0;

	do {
		++numSlots;
		num /= 10;
	} while (num != 0);

	return numSlots;
}

unsigned getDigit(size_t num, unsigned decimalSlot, unsigned numDecimalSlots) {
	unsigned firstOp = num / (size_t) pow(10, numDecimalSlots - decimalSlot);
	unsigned secOp = (num / (size_t) pow(10, numDecimalSlots - decimalSlot + 1)) * 10;
	return firstOp - secOp;
}

// lastX and lastY are the top-left coordinates of the number's first digit.
void drawNumber(size_t num, short firstX, short firstY) {
	short x = firstX, y = firstY;
	unsigned numDecimalSlots = getNumDecimalSlots(num);
//	printf("x = %d | y = %d | numDecimalSlots = %d\n", x, y, numDecimalSlots);
	unsigned decimalSlot = 1;

	do {
		size_t dig = getDigit(num, decimalSlot, numDecimalSlots);
		draw_digit(dig, x, y);
		++decimalSlot;
		x += DIGIT_WIDTH;
	} while (decimalSlot <= numDecimalSlots);
}

// HH:MM:SS format.
// firstX and firstY are the top-left coordinates of time.
void drawTime(Time *time, short firstX, short firstY) {
	short x = firstX, y = firstY;

	if (time->hour < 10) {
		drawNumber(0, x, y);
		x += DIGIT_WIDTH;
	}
	drawNumber(time->hour, x, y);
	x += 2 * DIGIT_WIDTH;

	vg_draw_xpm(x, y, twoPoints);
	x += DIGIT_WIDTH;

	if (time->minute < 10) {
		drawNumber(0, x, y);
		x += DIGIT_WIDTH;
	}
	drawNumber(time->minute, x, y);
	x += 2 * DIGIT_WIDTH;

	vg_draw_xpm(x, y, twoPoints);
	x += DIGIT_WIDTH;

	if (time->second < 10) {
		drawNumber(0, x, y);
		x += DIGIT_WIDTH;
	}
	drawNumber(time->second, x, y);
}

void draw_borders() {
	vg_line(55, 55, 974, 55, 63);
	vg_line(55, 55, 55, 718, 63);
	vg_line(55, 718, 974, 718, 63);
	vg_line(974, 718, 974, 55, 63);
}

void treatMouseButtons(unsigned long packet[], Snake *snake, Rectangle *eatableDim, char *eatXpm[]) {
	if (mouse_overflow(packet)) {
		return;
	}
	if (mouse_pressedLMB(packet) || mouse_pressedMMB(packet) || mouse_pressedRMB(packet)) {
		vg_clear_xpm(eatableDim->xi, eatableDim->yi, eatXpm);
		spawnEat(snake, eatableDim, eatXpm);
	}
}

int play(unsigned int difficulty, unsigned int *points) {
	bool ate = false;
	bool isHorizontal = true;
	*points = 0;

	bool prev_isHorizontal;
	short delta; // Pixels moved per frame.
	short prev_delta;
	switch (difficulty) {
	case 1:
		delta = 4;
		break;
	case 2:
		delta = 6;
		break;
	case 3:
		delta = 9;
		break;
	}

	Snake snake = newSnake();
	draw_borders();
	getHeadRot(isHorizontal, delta, &snake);

	snake.head.xi = 200;
	snake.head.yi = 100;
	short headWidth = 23, headLength = 30;
	if (isHorizontal) {
		snake.head.width = headLength;
		snake.head.height = headWidth;
	} else {
		snake.head.width = headWidth;
		snake.head.height = headLength;
	}
	char **eatable = apple;
	Rectangle eatableDim;
	eatableDim.width = 48;
	eatableDim.height = 49;

	vg_draw_xpm(snake.head.xi, snake.head.yi, snake.xpmHead);
	spawnEat(&snake, &eatableDim, eatable);

	// Draw score.
	vg_draw_xpm(SCORE_X - SCORE_MAX_DIGITS * (DIGIT_WIDTH + 1) - getXpmWidth(scoreXpm), SCORE_LAST_DIG_Y, scoreXpm);
	vg_draw_xpm(SCORE_X, SCORE_LAST_DIG_Y, numZero);

	unsigned long packet[3];

	int r, ipc_status;
	message msg;
	int timerSubRes = timer_subscribe_int();
	if (timerSubRes == -1) {
		printf("play(): Timer sub error\n");
		return -1;
	}
	unsigned long irq_timer_set = BIT(timerSubRes);

	Time time = getTime();

	size_t intCounter = 0;
	while (true) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive() failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_timer_set) {
					++intCounter;
					ate = false;

					// Save previous snake for its body coordinates and its head's position and orientation.
					Snake prevSnake = copySnake(&snake);

					// Update coordinates.
					prev_isHorizontal = isHorizontal;
					prev_delta = delta;
					if (INV_PACKETS != getDirection(packet, &isHorizontal, &delta, irq_timer_set, &intCounter, &eatableDim)) {
						treatMouseButtons(packet, &snake, &eatableDim, eatable);
					}
					switch (updateSnakePos(prev_isHorizontal, prev_delta,
							isHorizontal, &delta, &(snake.head))) {
					case OUT_BOUNDS:
						if (timer_unsubscribe_int() == -1) {
							printf("play(): Timer unsub error\n");
						}
						return GAME_LOST;
					case OK:
						break;
					}
					getHeadRot(isHorizontal, delta, &snake);
					updateBody(&snake, &prevSnake, delta);
				//	for (unsigned index = 0; index < snake.length; ++index) {
				//		printf("body[%d] = (%d, %d)\n", index, snake.bodyCoords[index].x, snake.bodyCoords[index].y);
				//	}

					if (canEat(isHorizontal, delta, snake.head, eatableDim)) {
						vg_clear_xpm(eatableDim.xi, eatableDim.yi, eatable);
						ate = true;
					}

					while (intCounter >= 60) {
						intCounter -= 60;
						incrementTime(&time);
					//	printTime(&time);
						if (intCounter < 60) {
							intCounter = 0;
						}
					}

					// Draw on secondary frame buffer.
					// - Snake and time always drawn.
					// - Eatable and score only drawn when ate.
					vg_clear_xpm(prevSnake.head.xi, prevSnake.head.yi, prevSnake.xpmHead);
					if (ate) {
						spawnEat(&snake, &eatableDim, eatable);
						addBody(&snake);
						(*points)++;
						drawNumber(*points, SCORE_X, SCORE_LAST_DIG_Y);
					}
				//	printSnakeInfo(&prevSnake);
//					if (snake.length >= 2) {
//						waitByTimer0(0.5);
//					}
					undrawBody(&prevSnake);
//					if (snake.length >= 2) {
//						vg_updateMainBuffer();
//						waitByTimer0(0.5);
//					}
					drawBody(&snake);
					vg_draw_xpm(snake.head.xi, snake.head.yi, snake.xpmHead);
					drawTime(&time, TIME_X, TIME_Y);

					// Move to main frame buffer.
					vg_updateMainBuffer();

//					if (positionConflictArray(snake.head, snake.bodyCoords, snake.length, getXpmWidth(snake.xpmBody), getXpmHeight(snake.xpmBody))) {
//						if (timer_unsubscribe_int() == -1) {
//							printf("play(): Timer unsub error\n");
//							return -1;
//						}
//						return GAME_LOST;
//					}

					if (pressedEsc()) {
						printf("Pressed ESC\n");
						if (timer_unsubscribe_int() == -1) {
							printf("play(): Timer unsub error\n");
							return -1;
						}
						return GAME_EXIT;
					}
				}
			}
		}
	}

	if (timer_unsubscribe_int() == -1) {
		printf("play(): Timer unsub error\n");
		return -1;
	}
	return GAME_SUCCESS;
}

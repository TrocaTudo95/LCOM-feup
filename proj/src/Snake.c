#include <stdio.h>
#include <stdlib.h>
#include "pixmap.h"
#include "i8042.h"
#include "video_gr.h"
#include "Snake.h"
#include "MiscData.h"

// Undefined start position, orientation and head xpm. Reserved memory for body length of 10.
Snake newSnake() {
	Snake snake;
	snake.length = 0;
	snake.capacity = 10;
	snake.bodyCoords = malloc(10 * sizeof(Point2D));
	snake.xpmBody = body;
	return snake;
}

Snake copySnake(Snake *snake) {
	Snake copySnake = *snake;
	Point2D *copyBodyCoords = malloc(copySnake.capacity * sizeof(Point2D));
	for (unsigned index = 0; index < copySnake.length; ++index) {
		copyBodyCoords[index] = snake->bodyCoords[index];
	}
	copySnake.bodyCoords = copyBodyCoords;
	return copySnake;
}

int addBody(Snake *snake) {
	++(snake->length);
	if (snake->length > snake->capacity) {
		Point2D *temp = realloc(snake->bodyCoords, (snake->capacity + 10) * sizeof(Point2D));
		if (temp == NULL) {
			printf("addBody(): Memory reallocation failure\n");
			return -1;
		}
		snake->bodyCoords = temp;
		snake->capacity += 10;
	}
	Point2D newSection;
	switch (snake->length - 1) {
	case 0: // Only had head.
		switch (snake->orientation) {
		case UP:
			newSection.x = snake->head.xi;
			newSection.y = snake->head.yi + getXpmHeight(snake->xpmHead);
			break;
		case DOWN:
			newSection.x = snake->head.xi;
			newSection.y = snake->head.yi - getXpmHeight(snake->xpmHead);
			break;
		case LEFT:
			newSection.x = snake->head.xi + getXpmWidth(snake->xpmHead);
			newSection.y = snake->head.yi;
			break;
		case RIGHT:
			newSection.x = snake->head.xi - getXpmWidth(snake->xpmHead);
			newSection.y = snake->head.yi;
			break;
		default:
			printf("addBody(): Invalid snake head orientation\n");
			return -1;
		}
		break;
	case 1: // Only had one body section.
		newSection.x = snake->bodyCoords[snake->length - 2].x
				+ (snake->bodyCoords[snake->length - 2].x - snake->head.xi);
		newSection.y = snake->bodyCoords[snake->length - 2].y
				+ (snake->bodyCoords[snake->length - 2].y - snake->head.yi);
		break;
	default:
		newSection.x = snake->bodyCoords[snake->length - 2].x
				+ (snake->bodyCoords[snake->length - 2].x - snake->bodyCoords[snake->length - 3].x);
		newSection.y = snake->bodyCoords[snake->length - 2].y
				+ (snake->bodyCoords[snake->length - 2].y - snake->bodyCoords[snake->length - 3].y);
		break;
	}
	printf("New body[%d] = (%d, %d)\n", snake->length - 1, newSection.x, newSection.y);
	snake->bodyCoords[snake->length - 1] = newSection;
	return OK;
}

// Update to next frame.
int updateBody(Snake *snake, Snake *prevSnake, short deltaPerFrame) {
	if (snake->length <= 0) {
		return OK;
	}

	switch (getPointDirection(&prevSnake->bodyCoords[0], getPoint2D(&(prevSnake->head)))) {
	case UP:
		snake->bodyCoords[0].y -= abs(deltaPerFrame);
		break;
	case DOWN:
		snake->bodyCoords[0].y += abs(deltaPerFrame);
		break;
	case LEFT:
		snake->bodyCoords[0].x -= abs(deltaPerFrame);
		break;
	case RIGHT:
		snake->bodyCoords[0].x += abs(deltaPerFrame);
		break;
		//TODO
	case UP_LEFT:
		snake->bodyCoords[0].y -= abs(deltaPerFrame);
		snake->bodyCoords[0].x -= abs(deltaPerFrame);
		break;
	case UP_RIGHT:
		snake->bodyCoords[0].y -= abs(deltaPerFrame);
		snake->bodyCoords[0].x += abs(deltaPerFrame);
		break;
	case DOWN_LEFT:
		snake->bodyCoords[0].y += abs(deltaPerFrame);
		snake->bodyCoords[0].x -= abs(deltaPerFrame);
		break;
	case DOWN_RIGHT:
		snake->bodyCoords[0].y += abs(deltaPerFrame);
		snake->bodyCoords[0].x += abs(deltaPerFrame);
		break;
	default:
		printf("updateBody(): Invalid direction\n");
		return -1;
	}

	for (size_t bodyInd = 1; bodyInd < snake->length; ++bodyInd) {
		switch (getPointDirection(&prevSnake->bodyCoords[bodyInd], &prevSnake->bodyCoords[bodyInd - 1])) {
		case UP:
			snake->bodyCoords[bodyInd].y -= abs(deltaPerFrame);
			break;
		case DOWN:
			snake->bodyCoords[bodyInd].y += abs(deltaPerFrame);
			break;
		case LEFT:
			snake->bodyCoords[bodyInd].x -= abs(deltaPerFrame);
			break;
		case RIGHT:
			snake->bodyCoords[bodyInd].x += abs(deltaPerFrame);
			break;
			//TODO
		case UP_LEFT:
			snake->bodyCoords[bodyInd].y -= abs(deltaPerFrame);
			snake->bodyCoords[bodyInd].x -= abs(deltaPerFrame);
			break;
		case UP_RIGHT:
			snake->bodyCoords[bodyInd].y -= abs(deltaPerFrame);
			snake->bodyCoords[bodyInd].x += abs(deltaPerFrame);
			break;
		case DOWN_LEFT:
			snake->bodyCoords[bodyInd].y += abs(deltaPerFrame);
			snake->bodyCoords[bodyInd].x -= abs(deltaPerFrame);
			break;
		case DOWN_RIGHT:
			snake->bodyCoords[bodyInd].y += abs(deltaPerFrame);
			snake->bodyCoords[bodyInd].x += abs(deltaPerFrame);
			break;
		default:
			printf("updateBody(): Invalid direction\n");
			return -1;
		}
	}
	return OK;
}

void getHeadRot(bool isHorizontal, short delta, Snake *snake) {
	if (isHorizontal) {
		if (delta > 0) {
			snake->orientation = RIGHT;
			snake->xpmHead = snakeHeadRight;
		} else {
			snake->orientation = LEFT;
			snake->xpmHead = snakeHeadLeft;
		}
	} else {
		if (delta > 0) {
			snake->orientation = DOWN;
			snake->xpmHead = snakeHeadDown;
		} else {
			snake->orientation = UP;
			snake->xpmHead = snakeHeadUp;
		}
	}
}

void drawBody(Snake *snake) {
	for (unsigned index = 0; index < snake->length; ++index) {
	//	printf("Drawn body[%d]: (%d, %d)\n", index, snake->bodyCoords[index].x, snake->bodyCoords[index].y);
		vg_draw_xpm(snake->bodyCoords[index].x, snake->bodyCoords[index].y, snake->xpmBody);
	}
}

void undrawBody(Snake *snake) {
	for (unsigned index = 0; index < snake->length; ++index) {
	//	printf("Undrawn body[%d]: (%d, %d)\n", index, snake->bodyCoords[index].x, snake->bodyCoords[index].y);
		vg_clear_xpm(snake->bodyCoords[index].x, snake->bodyCoords[index].y, snake->xpmBody);
	}
}

void undrawLastBodySection(Snake *snake) {
	vg_clear_xpm(snake->bodyCoords[snake->length - 1].x, snake->bodyCoords[snake->length - 1].y, snake->xpmBody);
}

void printSnakeInfo(Snake *snake) {
	printf("Length = %d | Capacity = %d\n", snake->length, snake->capacity);
}

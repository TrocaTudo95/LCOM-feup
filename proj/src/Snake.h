/**
 * @file Snake.h
 */

#ifndef SNAKE_H
#define SNAKE_H

#include "MiscData.h"

/**
 * @brief Definition of a Snake.
 */
typedef struct {
	Rectangle head; /**< @brief Rectangle with the head. */
	enum Direction orientation; /**< @brief Orientation of the Snake's head. */
	unsigned int length; /**< @brief Number of body sections. */
	unsigned int capacity; /**< @brief Number of body sections with memory reserved. */
	Point2D *bodyCoords; /**< @brief Array of top-left coordinates of body sections. */
	char **xpmHead; /**< @brief xpm representation of the head. Changes with orientation. */
	char **xpmBody; /**< @brief xpm representation of a body section. */
} Snake;

/**
 * @brief Creates a new Snake.
 * Undefined start position, orientation and head xpm. Reserved memory for body length of 10.
 * @return New Snake.
 */
Snake newSnake();

/**
 * @brief Creates a copy of a given Snake.
 * New Snake is independent of the previous one.
 * @param snake Pointer to Snake to copy.
 * @return New Snake.
 */
Snake copySnake(Snake *snake);

/**
 * @brief Adds a body section to a given Snake.
 * If Snake has no body, it is added behind the head.
 * Otherwise, it is added on the opposite side of the second last body or head.
 * @param snake Pointer to Snake to increase.
 * @return -1 if body reallocation failure or invalid snake head orientation, OK otherwise.
 */
int addBody(Snake *snake);

/**
 * @brief Updates Snake head and body orientations for the next frame.
 * @param snake Pointer to Snake to update.
 * @param prevSnake Pointer to current Snake for reference. It is not altered.
 * @param deltaPerFrame Distance per frame pixels. Positive if downwards, negative otherwise.
 * @return -1 if invalid direction between snake and body or between bodies, OK otherwise.
 */
int updateBody(Snake *snake, Snake *prevSnake, short deltaPerFrame);

/**
 * @brief Rotates Snake head to the required orientation.
 * @param isHorizontal True if required orientation is horizontal, false othwise.
 * @param delta Distance per frame pixels. Positive if downwards, negative otherwise.
 * @param snake Pointer to Snake to rotate.
 */
void getHeadRot(bool isHorizontal, short delta, Snake *snake);

/**
 * @brief Draws a Snake's body.
 * @param snake Pointer to Snake to draw.
 */
void drawBody(Snake *snake);

/**
 * @brief Clears a Snake's body.
 * @param snake Pointer to Snake to clear.
 */
void undrawBody(Snake *snake);

/**
 * @brief Clears a Snake's last body section.
 * @param snake Pointer to Snake to clear.
 */
void undrawLastBodySection(Snake *snake);

/**
 * @brief Prints a Snake's length and capacity.
 * @param snake Pointer to Snake to print about.
 */
void printSnakeInfo(Snake *snake);

#endif

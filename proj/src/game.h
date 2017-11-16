/**
 * @file game.h
 */

#ifndef GAME_H
#define GAME_H

#include "pixmap.h"
#include "mouse.h"
#include "vbe.h"
#include "video_gr.h"
#include "RTC.h"
#include "MiscData.h"
#include <string.h>

#define GAME_SUCCESS 0
#define GAME_LOST 1
#define GAME_EXIT 2

#define DIFF_EASY 1
#define DIFF_NORMAL 2
#define DIFF_HARD 3

#define SPEED_BOOST 1.5 // When a mouse button is pressed.

#define DIGIT_HEIGHT 20 // Equal to the height of the digit xpm's.
#define DIGIT_WIDTH 20 // Equal to the width of the digit xpm's.

#define SCORE_X 800
#define SCORE_LAST_DIG_Y 30
#define SCORE_MAX_DIGITS 3 // Determines the space between "Score:" and the last digit.

#define TIME_X 500
#define TIME_Y (getVertRes() - DIGIT_HEIGHT - 2)

/**
 * @brief Detects if user released ESC.
 * @return True if user released ESC, false otherwise.
 */
bool pressedEsc();

/**
 * @brief Gets the number of decimal slots of an unsigned integer.
 * @param num Unsigned integer.
 * @return Number of decimal slots.
 */
unsigned getNumDecimalSlots(size_t num);

/**
 * @brief Gets the direction from the mouse's movement.
 * @param packet Mouse packet initialized here by mouse_get_packet().
 * @param isHorizontal Pointer to bool. True if horizontal, false otherwise.
 * @param delta Pointer to distance per frame in pixels. Positive if downwards, negative otherwise. Only the sign may be changed.
 * @param irq_timer_set Timer flag used to detect timer interrupts.
 * @param timeIntCounter Pointer to counter of timer interrupts. Used to keep track of time inside function.
 * @param eatableDim Pointer to Rectangle representing a consumable.
 * @return OK.
 */
int getDirection(unsigned long packet[], bool *isHorizontal, short *delta, unsigned long irq_timer_set, size_t *timeIntCounter, Rectangle *eatableDim);

/**
 * @brief Starts the game, whole game is spent inside this function.
 * @param difficulty Game's difficulty. Dictates snake's speed.
 * @param points Pointer to points counter.
 * @return -1 if a device fails to (un)subscribe, OK otherwise.
 */
int play(unsigned int difficulty, unsigned int *points);



#endif

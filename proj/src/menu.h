/**
 * @file menu.h
 */

#ifndef MENU_H
#define MENU_H

#include "game.h"

#define BMP_DIGIT_WIDTH 40

/**
 * @brief Displays the main menu.
 * @return -1 if keyboard (un)subscription fails, OK otherwise.
 */
int initial_menu();

/**
 * @brief Displays the options menu.
 * Uses keyboard interruptions to select an option. User may go to the difficulty or resolution settings, or exit the menu.
 */
void menu_options();

/**
 * @brief  Displays the menu to choose the difficulty.
 * Uses keyboard interruptions to select an option.
 * @param irq_kbd_set Keyboard IRQ.
 * @return -1 if menu bitmap load failed, OK otherwise.
 */
int menu_difficulty(unsigned long irq_kbd_set);

/**
 * @brief Displays the menu to choose the resolution.
 * Uses keyboard interruptions to select an option. VBE mode may be altered.
 * @param irq_kbd_set Keyboard IRQ.
 * @param mode Pointer to the current VBE mode.
 */
void menu_resolution(unsigned long irq_kbd_set, unsigned short *mode);

/**
 * @brief Draws a given number in a (x, y) position.
 * Uses bitmaps for each digit.
 * @param num Number to draw.
 * @param x x coordinate to draw the number.
 * @param y y coordinate to draw the number.
 */
void draw_digit_bmp(size_t num, short x, short y);

#endif

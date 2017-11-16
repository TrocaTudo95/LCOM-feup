/**
 * @file keyboard.h
 */

#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_

#include <stdio.h>
#include <stdbool.h>
#include <minix/syslib.h>
#include <minix/driver.h>
#include "i8042.h"
#include "i8254.h"

/**
 * @brief Subscribes the keyboard.
 * If it's already subscribed, it prints an error message and returns -1.
 * @return Original hook ID of the keyboard if success, -1 otherwise.
 */
int kbd_subscribe_int();

/**
 * @brief Unsubscribes the keyboard.
 * If it's not initially subscribed, it prints an error message and returns -1.
 * @return OK if success, -1 otherwise.
 */
int kbd_unsubscribe_int();

/**
 * @brief Prints a scan code one byte at a time.
 * Takes into account multi-byte scan codes. If it detects they're part of the same key stroke,
 * they remain on the same line.
 * @param isMakecode Pointer to bool to distinguish make codes from break codes.
 * @param scanCode Current byte to print.
 * @param prevScanCode Previous printed byte.
 * @param prevPrevScanCode Previous previous printed byte.
 * @param prevPrevPrevScanCode Previous previous previous printed byte.
 * @param is4Byte Pointer to bool used for 4-byte scan codes.
 * @return OK.
 */
int kbd_print_scancodes(bool *isMakecode,
		unsigned long scanCode,
		unsigned long prevScanCode,
		unsigned long prevPrevScanCode,
		unsigned long prevPrevPrevScanCode,
		bool *is4Byte);

/**
 * @brief Detects if ESC was used.
 * Distinguishes between pressing and releasing ESC,
 * and checks for errors with the status register.
 * @return ESC_PRESSED if ESC was pressed, ESC_RELEASED if ESC was released, OK otherwise.
 */
int kbd_pressed_esc();

/**
 * @brief Gets a keyboard scan code and prints it.
 * Handles parameters of kbd_print_scancodes().
 * @param isMakecode Pointer to bool to distinguish make codes from break codes.
 * @param prevScanCode Previous printed byte.
 * @param prevPrevScanCode Previous previous printed byte.
 * @param prevPrevPrevScanCode Previous previous previous printed byte.
 * @param is4Byte Pointer to bool used for 4-byte scan codes.
 * @return ESC_RELEASED if ESC was released, OK otherwise.
 */
int kbd_int_handler(bool *isMakecode,
		unsigned long *prevScanCode,
		unsigned long *prevPrevScanCode,
		unsigned long *prevPrevPrevScanCode,
		bool *is4Byte);

/**
 * @brief Sends a command to the keyboard.
 * Allows one argument. If keyboard responds with RESEND, the command is sent once again.
 * If keyboard responds with ERROR, the function returns.
 * @param command Command to send.
 * @param arg1 Command's argument.
 * @return OK if successful command delivery, -1 otherwise.
 */
int kbd_sendCmdToKBD(unsigned long command, unsigned long arg1);

/**
 * @brief Waits until user presses a given key.
 * @param key Scan code of the wanted key.
 * @return OK if success, -1 if keyboard unsubscription failure.
 */
int waitByKey(unsigned long key);

#endif

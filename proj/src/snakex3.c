/**
 * @file snakex3.c
 */

#include <minix/syslib.h>
#include <minix/drivers.h>
#include "game.h"
#include "menu.h"
#include "RTC.h"

/**
 * @brief Main function.
 * @return 0.
 */
int main() {
	sef_startup();

	{
		DateTime dt = getDateTime();
		srand(getSeconds(&dt));
	}

	initial_menu();

	return 0;
}

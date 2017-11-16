/**
 * @file mouse.h
 */

#ifndef __MOUSE_H_
#define __MOUSE_H_

#include <stdio.h>
#include <stdbool.h>
#include <minix/syslib.h>
#include <minix/driver.h>
#include "i8042.h"
#include "RTC.h"

#define NO_PACKETS 2
#define INV_PACKETS 3

#define LMB BIT(0)
#define MMB BIT(2)
#define RMB BIT(1)
#define X_OVF BIT(6)
#define Y_OVF BIT(7)

typedef enum {INIT, DRAW, COMP} gest_state;
typedef enum {RDOWN, RUP, POS_SLOPE_MOV, NEG_SLOPE_MOV, ZERO_COMPONENT_MOV, INV_DIR} gest_transition;

/*
 * @brief send a command to KBC.
 * @param command to the KBC.
 * @param
 * @return OK if success, -1 otherwise.
 */
int sendCmdToKBC(unsigned long command, unsigned long arg1);

/**
 * @brief get a mouse packet from the OUT_BUF.
 * @param packet pointer to a array to get the 3 bytes from the mouse.
  *@return OK if success, -1 otherwise.
 */
int mouse_getConfig(unsigned long *packet);

/**
 * @brief displays the mouse packet in a human friendly way
 * @param *packet pointer to a array to get the 3 bytes from the mouse.
  *@return OK if success, -1 otherwise.
 */
int mouse_displayConfig(unsigned long *packet);

/**
 * If it's already subscribed, it prints an error message and returns -1.
 * @brief Subscribe the mouse.
 * @return Original hook ID of the mouse if success, -1 otherwise.
 */
int mouse_subscribe_int();

/**
 * @brief get a byte form the OUT_BUF.
 * @param *mouseByte pointer to put the byte from the OUT_BUF.
 * @return OK if success, -1 otherwise.
 */
int mouse_int_handler(unsigned long *mouseByte);

/**
 * @brief Gets mouse packet.
 * @param packet Mouse packet initialized here.
 * @param irq_timer_set IRQ Timer flag.
 * @param timeIntCounter Pointer to counter of timer interruptions. Increased by function if there are timer interruptions during its execution.
 */
int mouse_get_packet(unsigned long packet[], unsigned long irq_timer_set, size_t *timeIntCounter);

/**
 * @brief print a mouse packet showing the 3 bytes, the x and y movement and the buttons.
 * @param packet[] packet to print.
 */
void printMousePacket(short packet[]);


/**
 * If it's not initially subscribed, it prints an error message and returns -1.
 * @brief Unsubscribe the mouse.
 * @return OK if success, -1 otherwise.
 */
int mouse_unsubscribe_int();

/**
 * @brief Detects movement overflow in a packet.
 * @param packet 3-byte mouse packet.
 * @return True if there was overflow in X or Y movement, false otherwise.
 */
bool mouse_overflow(unsigned long packet[]);

/**
 * @brief Detects if the left mouse button was pressed.
 * @param packet 3-byte mouse packet.
 * @return True if the LMB was pressed, false otherwise.
 */
bool mouse_pressedLMB(unsigned long packet[]);

/**
 * @brief Detects if the middle mouse button was pressed.
 * @param packet 3-byte mouse packet.
 * @return True if the MMB was pressed, false otherwise.
 */
bool mouse_pressedMMB(unsigned long packet[]);

/**
 * @brief Detects if the right mouse button was pressed.
 * @param packet 3-byte mouse packet.
 * @return True if the RMB was pressed, false otherwise.
 */
bool mouse_pressedRMB(unsigned long packet[]);

#endif

#ifndef __MOUSE_H_
#define __MOUSE_H_

#include <stdio.h>
#include <stdbool.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"

typedef enum {INIT, DRAW, COMP} gest_state;
typedef enum {RDOWN, RUP, POS_SLOPE_MOV, NEG_SLOPE_MOV, ZERO_COMPONENT_MOV, INV_DIR} gest_transition;

/*
 * Function return values:
 * 		OK if success
 * 		-1 otherwise
 */

int sendCmdToKBC(unsigned long command, unsigned long arg1);
int mouse_getConfig(unsigned long *packet);
int mouse_displayConfig(unsigned long *packet);
int mouse_subscribe_int();
int mouse_int_handler(unsigned long *mouseByte);
void printMousePacket(short packet[]);
gest_transition getTransition(gest_state st, short *packet, short *currentLength, short length);
gest_state updateState(short *packet, short length);
int mouse_unsubscribe_int();

#endif

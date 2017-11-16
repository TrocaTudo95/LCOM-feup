/**
 * @file i8042.h
 */

#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Timer. Needs to be completed.
 */

#define TIMER_FREQ  1193182    /**< @brief clock frequency for timer in PC and AT */

#define BIT(n) (0x01<<(n))		/**< @brief Select a bit. */

#define KBC_IRQ 1	/**< @brief KBC IRQ line */
#define MOUSE_IRQ 12	/**< @brief Mouse IRQ line */

/* I/O port addresses */

#define IN_BUF 0x60 /**< @brief Input Buffer */
#define OUT_BUF 0x60 /**< @brief Output Buffer */
#define KBC_CMD_REG 0x64 /**< @brief KBC Command Register */
#define STAT_REG 0x64/**< @brief Status Register */

// KBC Commands.

#define WRITE_BYTE_MOUSE 0xD4

// KBC Arguments.

#define SET_STREAM 0xEA
#define SET_REMOTE 0xF0
#define DISABLE_DATA_REPORT 0xF5
#define ENABLE_DATA_REPORT 0xF4
#define STAT_REQ 0xE9

/* KEYBOARD COMMANDS */

#define RESET_KBD 0xFF
#define SET_KBD_LEDS 0xED

// Status register.
#define IBF BIT(1)
#define OBF BIT(0)
#define ESC 0x81
// Keyboard arguments.
#define LED(n) BIT(n)
#define TWO_BYTES 0xe0

/* KEYBOARD SCANCODES */

#define ENTER_MAKE 0x1c
#define ENTER_BREAK 0x9c
#define ESC_MAKE 0x01
#define ESC_BREAK 0x81
#define KEY_1_MAKE 0x02
#define KEY_2_MAKE 0x03
#define KEY_3_MAKE 0x04
#define KEY_4_MAKE 0x05

/* FUNCTION RETURN VALUES */

#define ESC_PRESSED 1
#define ESC_RELEASED 2
#define OK 0
#define ACK 0xFA
#define RESEND 0xFE
#define ERROR 0xFC
#define IBF BIT(1)
#define OBF BIT(0)

/**@}*/

#endif /* _LCOM_I8042_H */


#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Timer. Needs to be completed.
 */

#define TIMER_FREQ  1193182    /**< @brief clock frequency for timer in PC and AT */

#define BIT(n) (0x01<<(n))

#define KBC_IRQ	        1

/* I/O port addresses */

#define IN_BUF 0x60
#define OUT_BUF 0x60
#define KBC_CMD_REG 0x64
#define STAT_REG 0x64

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
#define ESC_BREAK 0x81

/* FUNCTION RETURN VALUES */

#define ESC_RELEASED 1
#define OK 0
#define ACK 0xFA
#define RESEND 0xFE
#define ERROR 0xFC
#define IBF BIT(1)
#define OBF BIT(0)

/**@}*/

#endif /* _LCOM_I8042_H */


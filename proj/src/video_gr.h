/**
 * @file video_gr.h
 */

#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include <stdbool.h>
#include "i8042.h"
#include <math.h>

#define H_RES 1024
#define V_RES 768
//#define BITS_PER_PIXEL 8
#define FRAMES_PER_SEC 60
#define OUT_BOUNDS 2


/**
 * @brief Maps the first Megabyte.
 * @return Virtual address to the first Megabyte.
 */
void* initLowArea();

/**
 * @brief Gets the current screen width in pixels.
 * @return Screen width in pixels.
 */
unsigned getHorRes();

/**
 * @brief Gets the current screen height in pixels.
 * @return Screen height in pixels.
 */
unsigned getVertRes();

/**
 * @brief Gets the current address of the secondary framebuffer.
 * @return Address of the secondary framebuffer.
 */
char* getSecBuffer();

/**
 * @brief Moves the content of the secondary framebuffer to the primary one.
 */
void vg_updateMainBuffer();

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

/**
 * @brief Draws a square on the secondary framebuffer.
 * @param x Top-left x coordinate.
 * @param y Top-left y coordinate.
 * @param size Side length in pixels.
 * @param color Square's color.
 * @return OK.
 */
int vg_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color);

/**
 * @brief Draws a line on the secondary framebuffer.
 * @param xi Starting point's x coordinate.
 * @param yi Starting point's y coordinate.
 * @param xf End point's x coordinate.
 * @param yf End point's y coordinate.
 * @param color Line's color.
 * @return OK.
 */
int vg_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, unsigned long color);

/**
 * @brief Draws a xpm on the secondary framebuffer.
 * VBE mode must be 0x105 (1024x768, 256 colors).
 * @param xi Top-left x coordinate of the xpm.
 * @param yi Top-left y coordinate of the xpm.
 * @param xpm xpm to draw.
 * @return OK.
 */
int vg_draw_xpm(short xi, short yi, char *xpm[]);

/**
 * @brief Paints the screen with the background color.
 * @return OK.
 */
int vg_clear_screen();

/**
 * @brief Replaces drawn xpm with the background color.
 * @param xi Top-left x coordinate of the xpm.
 * @param yi Top-left y coordinate of the xpm.
 * @param xpm xpm to clear.
 * @return OK.
 */
int vg_clear_xpm(short xi, short yi, char *xpm[]);

/**
 * @brief Move xpm gradually from one position to another.
 * @param xi Starting position's top-left x coordinate.
 * @param yi Starting position's top-left y coordinate.
 * @param xpm xpm to move.
 * @param hor 1 if horizontal movement, 0 if vertical.
 * @param delta Distance to move in pixels.
 * @param time Time which the xpm takes to move in seconds.
 * @return ESC_RELEASED if ESC is released during the movement, OK otherwise.
 */
int vg_move_xpm(short xi, short yi, char *xpm[],
		unsigned short hor, short delta, double time);

/**
 * @brief Gets palette data.
 * @return Palette array, 6 bits per primary color.
 */
long* vg_getPalette();

/**
 * @brief Prints the 6-bit/color RGB value associated with each color index.
 * @param colors Array of RGB values.
 * @param numColors Number of colors to print.
 */
void vg_printPalette(long *colors, int numColors);

 /** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */

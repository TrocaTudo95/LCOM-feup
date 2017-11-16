/**
 * @file MiscData.h
 */

#ifndef MISCDATA_H
#define MISCDATA_H

#include <stdbool.h>

typedef struct {
	short x;
	short y;
} Point2D;

typedef struct {
	short xi, yi;
	short width, height;
} Rectangle;

enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UP_LEFT,
	UP_RIGHT,
	DOWN_LEFT,
	DOWN_RIGHT
};

/**
 * @brief Gets top-left Point2D from a Rectangle.
 * @param r Pointer to Rectangle.
 * @return Top-left Point2D.
 */
Point2D* getPoint2D(Rectangle *r);

/**
 * @brief Checks if 2 Rectangles overlap.
 * @param r1 Fist Rectangle.
 * @param r2 Second Rectangle.
 * @return True if Rectangles overlap, false otherwise.
 */
bool positionConflict(Rectangle r1, Rectangle r2);

/**
 * @brief Checks if a Rectangle overlaps with any of an array of Rectangles.
 * Rectangles from the array are obtained from the given array of Point2D and the width and height parameters.
 * @param r1 First Rectangle.
 * @param points Array of Point2D indicating top-left positions of array of Rectangles.
 * @param pointLength Number of elements of points.
 * @param pointWidth Width of the Rectangles in the array.
 * @param pointHeight Height of the Rectangles in the array.
 * True if r1 overlaps with any of the Rectangles in the array, false otherwise.
 */
bool positionConflictArray(Rectangle r1, Point2D *points, unsigned pointLength, short pointWidth, short pointHeight);

/**
 * @brief Gets the xpm's height in pixels.
 * @param xpm xpm to measure.
 * @return xpm's height.
 */
unsigned long getXpmHeight(char *xpm[]);

/**
 * @brief Gets the xpm's width in pixels.
 * @param xpm xpm to measure.
 * @return xpm's width.
 */
unsigned long getXpmWidth(char *xpm[]);

/**
 * @brief Gets the Direction from a point to another.
 * @param p1 Starting point.
 * @param p2 End point.
 * @return Direction from p1 to p2.
 */
enum Direction getPointDirection(Point2D *p1, Point2D *p2);

/**
 * @brief Gets the rough Direction from a point to another.
 * If direction deviates upto 3 pixels from being vertical or horizontal,
 * it returns vertical or horizontal.
 * @param p1 Starting point.
 * @param p2 End point.
 * @return Direction from p1 to p2.
 */
enum Direction getPointDirectionMargin(Point2D *p1, Point2D *p2);

#endif

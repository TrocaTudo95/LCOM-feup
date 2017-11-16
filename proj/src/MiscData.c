#include <stdio.h>
#include <stdlib.h>
#include "MiscData.h"

Point2D* getPoint2D(Rectangle *r) {
	Point2D *point = malloc(sizeof(Point2D));
	point->x = r->xi;
	point->y = r->yi;
	return point;
}

bool positionConflict(Rectangle r1, Rectangle r2) {
	return (r1.xi < (r2.xi +r2.width) && (r1.xi +r1.width) > r2.xi
			&& r1.yi <(r2.yi+ r2.height) && (r1.yi + r1.height)> r2.yi);
}

bool positionConflictArray(Rectangle r1, Point2D *points, unsigned pointLength, short pointWidth, short pointHeight) {
	for (unsigned index = 0; index < pointLength; ++index) {
		Rectangle r2;
		r2.xi = points[index].x;
		r2.yi = points[index].y;
		r2.width = pointWidth;
		r2.height = pointHeight;
		if (positionConflict(r1, r2)) {
			return true;
		}
	}
	return false;
}

unsigned long getXpmHeight(char *xpm[]) {
	strtoul(xpm[0], NULL, 10);
	return strtoul(xpm[0], NULL, 10);
}

unsigned long getXpmWidth(char *xpm[]) {
	return strtoul(xpm[0], NULL, 10);
}

// Returns direction from p1 to p2.
enum Direction getPointDirection(Point2D *p1, Point2D *p2) {
	if (p1->x == p2->x) {
		if (p1->y > p2->y) {
			return UP;
		} else {
			return DOWN;
		}
	} else if (p1->y == p2->y) {
		if (p1->x > p2->x) {
			return LEFT;
		} else {
			return RIGHT;
		}
	} else {
		if (p1->y > p2->y) {
			if (p1->x > p2->x) {
				return UP_LEFT;
			} else {
				return UP_RIGHT;
			}
		} else {
			if (p1->x > p2->x) {
				return DOWN_LEFT;
			} else {
				return DOWN_RIGHT;
			}
		}
	}
}

// Returns direction from p1 to p2.
enum Direction getPointDirectionMargin(Point2D *p1, Point2D *p2) {
	if (p1->x == p2->x || abs(p1->x - p2->x) <= 3) {
		if (p1->y > p2->y) {
			return UP;
		} else {
			return DOWN;
		}
	} else if (p1->y == p2->y || abs(p1->y - p2->y) <= 3) {
		if (p1->x > p2->x) {
			return LEFT;
		} else {
			return RIGHT;
		}
	} else {
		if (p1->y > p2->y) {
			if (p1->x > p2->x) {
				return UP_LEFT;
			} else {
				return UP_RIGHT;
			}
		} else {
			if (p1->x > p2->x) {
				return DOWN_LEFT;
			} else {
				return DOWN_RIGHT;
			}
		}
	}
}

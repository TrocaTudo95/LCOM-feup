#include "test5.h"
#include "pixmap.h"

static int proc_args(int argc, char **argv);
static long parse_long(char *str, int base);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);

static char* xpmTest[] = { "32 13 4", /* número de pixels horizontais, verticais, e cores */
					". 0", /* símbolo ’.’ representa pixel de cor 0 */
"x 2", /* símbolo ’x’ representa pixel de cor 2 */
"o 14",
"+ 4",  "................................", /* pixels ... */
		"..............xxx...............",
		"............xxxxxxx.............",
		".........xxxxxx+xxxxxx..........",
		"......xxxxxxx+++++xxxxxxx.......",
		"....xxxxxxx+++++++++xxxxxxx.....",
		"....xxxxxxx+++++++++xxxxxxx.....",
		"......xxxxxxx+++++xxxxxxx.......",
		".........xxxxxx+xxxxxx..........",
		"..........ooxxxxxxxoo...........",
		".......ooo...........ooo........",
		".....ooo...............ooo......",
		"...ooo...................ooo...." };

int main(int argc, char *argv[]) {
	sef_startup();

	if (argc == 1) { /* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	} else
		return proc_args(argc, argv);

	return 0;
}

static void print_usage(char **argv) {
	printf(
			"Usage: one of the following:\n"
					"\t service run %s -args \"init <mode (hex)> <delay (secs)>\"\n"
					"\t service run %s -args \"square <xi coord> <yi coord> <side length> <color>\"\n"
					"\t service run %s -args \"line <xi coord> <yi coord> <xf coord> <yf coord> <color>\"\n"
					"\t service run %s -args \"xpm <xi coord> <yi coord>\"\n"
					"\t service run %s -args \"move <xi coord> <yi coord> <direction> <delta> <time>\"\n"
					"\t service run %s -args \"controller\"\n\n", argv[0],
			argv[0], argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv) {
	short delta;
	unsigned short mode, delay, xi, yi, xf, yf, horizontal, time;
	unsigned long color;
	short length;
	if (strncmp(argv[1], "init", strlen("init")) == 0) {
		if (argc != 4) {
			printf("test5: wrong no. of arguments for test_init()\n");
			return 1;
		}
		mode = parse_ulong(argv[2], 16);
		delay = parse_ulong(argv[3], 10);
		if (mode == ULONG_MAX || delay == ULONG_MAX)
			return 1;
		printf("test5::test_init(%x, %u)\n", mode, delay);
		if (NULL == test_init(mode, delay))
			return 1;
		else
			return 0;
	} else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		if (argc != 6) {
			printf("test5: wrong no. of arguments for test_square()\n");
			return 1;
		}
		xi = parse_ulong(argv[2], 10);
		yi = parse_ulong(argv[3], 10);
		xf = parse_ulong(argv[4], 10);
		yf = parse_ulong(argv[5], 10);
		if (xi == ULONG_MAX || yi == ULONG_MAX || xf == ULONG_MAX
				|| yf == ULONG_MAX)
			return 1;

		printf("test5::test_square(%u, %u, %u, %u)\n", xi, yi, xf, yf);
		return test_square(xi, yi, xf, yf);
	} else if (strncmp(argv[1], "line", strlen("line")) == 0) {
		if (argc != 7) {
			printf("test5: wrong no of arguments for test_line()\n");
			return 1;
		}
		xi = parse_ulong(argv[2], 10);
		yi = parse_ulong(argv[3], 10);
		xf = parse_ulong(argv[4], 10);
		yf = parse_ulong(argv[5], 10);
		color = parse_ulong(argv[6], 10);
		if (xi == ULONG_MAX || yi == ULONG_MAX || xf == ULONG_MAX
				|| yf == ULONG_MAX || color == ULONG_MAX)
			return 1;

		printf("test5::test_line(%u, %u, %u, %u, %u)\n", xi, yi, xf, yf, color);
		return test_line(xi, yi, xf, yf, color);
	} else if (strncmp(argv[1], "xpm", strlen("xpm")) == 0) {
		if (argc != 4) {
			printf("test5: wrong no. of arguments for test_xpm()\n");
			return 1;
		}
		xi = parse_ulong(argv[2], 10);
		yi = parse_ulong(argv[3], 10);
		if (xi == ULONG_MAX || yi == ULONG_MAX)
			return 1;

		printf("test5::test_xpm(%u, %u, xpmTest)\n", xi, yi);
		return test_xpm(xi, yi, penguin);
	} else if (strncmp(argv[1], "move", strlen("move")) == 0) {
		if (argc != 7) {
			printf("test5: wrong no. of arguments for test_move()\n");
			return 1;
		}
		xi = parse_ulong(argv[2], 10);
		yi = parse_ulong(argv[3], 10);
		horizontal = parse_ulong(argv[4], 10);
		delta = parse_long(argv[5], 10);
		time = parse_ulong(argv[6], 10);
		if (xi == ULONG_MAX || yi == ULONG_MAX || horizontal == ULONG_MAX
				|| delta == LONG_MAX || time == ULONG_MAX)
			return 1;

		printf("test5::test_move(%u, %u, xpmTest, %u, %d, %u)\n", xi, yi,
				horizontal, delta, time);
		return test_move(xi, yi, penguin, horizontal, delta, time);
	} else if (strncmp(argv[1], "controller", strlen("controller")) == 0) {
		if (argc != 2) {
			printf("test5: wrong no. of arguments for test_controller()\n");
			return 1;
		}

		printf("test5::test_controller()\n");
		return test_controller();
	} else {
		printf("test5: %s - no valid function!\n", argv[1]);
		return 1;
	}
}

static long parse_long(char *str, int base) {
	char *endptr;
	long val;

	/* Convert string to long */
	val = strtol(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
			|| (errno != 0 && val == 0)) {
		perror("strtol");
		return LONG_MAX;
	}

	if (endptr == str) {
		printf("parse_long(): no digits were found in %s\n", str);
		return LONG_MAX;
	}

	/* Successful conversion */
	return val;
}

static unsigned long parse_ulong(char *str, int base) {
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}

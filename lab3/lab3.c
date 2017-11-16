#include "test3.h"
#include <limits.h>
#include <string.h>
#include <errno.h>

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char **argv)
{
	sef_startup();
	sys_enable_iop(SELF);

	if (argc == 1) {					/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"scan <decimal no.- assembly>\"\n"
			"\t service run %s -args \"leds <decimal no. - array size> <space-separated decimal nos. - array elements>\"\n"
			"\t service run %s -args \"timed_scan <decimal no. - max idle seconds>\"\n",
			argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	// kbd_test_leds(): 1st param = number of leds; 2nd param = array of leds ([0-2]) to modify.
	unsigned long ass, arraySize, time;
	unsigned short *ledArray;
	if (strncmp(argv[1], "scan", strlen("scan")) == 0) {
		if (argc != 3) {
			printf("test3: wrong no. of arguments for kbd_test_scan()\n");
			return 1;
		}
		ass = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (ass == ULONG_MAX)
			return 1;
		printf("test3::kbd_test_scan(%lu)\n", ass);
		return kbd_test_scan(ass);
	}
	else if (strncmp(argv[1], "leds", strlen("leds")) == 0) {
		if (argc < 3) {
			printf("test3: wrong no. of arguments for kbd_test_leds()\n");
			return 1;
		}
		arraySize = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (arraySize == ULONG_MAX)
			return 1;
		ledArray = (unsigned short *) malloc(arraySize * sizeof(unsigned long));

			unsigned int i = 0;
			while (i < arraySize) {
				ledArray[i] =parse_ulong(argv[3 + i],10);
				++i;
			}

		printf("test3::kbd_test_leds(%lu, ledArray)\n", arraySize);
		return kbd_test_leds(arraySize, ledArray);
	}
	else if (strncmp(argv[1], "timed_scan", strlen("timed_scan")) == 0) {
		if (argc != 3) {
			printf("test3: wrong no of arguments for kbd_test_timed_scan()\n");
			return 1;
		}
		time = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (time == ULONG_MAX)
			return 1;
		printf("test3::kbd_test_timed_scan(%lu)\n", time); //TODO
		return kbd_test_timed_scan(time); //TODO
	}
	else {
		printf("test3: %s - no valid function!\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base)
{
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


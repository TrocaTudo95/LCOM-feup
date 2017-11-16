#include "mouse.h"
#include "test4.h"

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char *argv[]) {
	sef_startup();

	if (argc == 1) { /* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	} else
		return proc_args(argc, argv);

	return 0;
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"packet <decimal no. - number of packets>\"\n"
			"\t service run %s -args \"async <decimal no. - number of seconds>\"\n"
			"\t service run %s -args \"config\"\n"
			"\t service run %s -args \"gesture <decimal no. - min. Y length\"\n",
			argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	unsigned short cnt, idle_time;
	short length;
	if (strncmp(argv[1], "packet", strlen("packet")) == 0) {
		if (argc != 3) {
			printf("test4: wrong no. of arguments for test_packet()\n");
			return 1;
		}
		cnt = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (cnt == ULONG_MAX)
			return 1;
		printf("test4::test_packet(%u)\n", cnt);
		return test_packet(cnt);
	}
	else if (strncmp(argv[1], "async", strlen("async")) == 0) {
		if (argc < 3) {
			printf("test4: wrong no. of arguments for test_async()\n");
			return 1;
		}
		idle_time = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (idle_time == ULONG_MAX)
			return 1;

		printf("test4::test_async(%u)\n", idle_time);
		return test_async(idle_time);
	}
	else if (strncmp(argv[1], "config", strlen("config")) == 0) {
		if (argc > 3) {
			printf("test4: wrong no of arguments for test_config\n");
			return 1;
		}

		printf("test4::test_config()\n\n");
		return test_config();
	}

	else if (strncmp(argv[1], "gesture", strlen("gesture")) == 0) {
			if (argc < 3) {
				printf("test4: wrong no. of arguments for test_gesture()\n");
				return 1;
			}
			length = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
			if (length == ULONG_MAX)
				return 1;

			printf("test4::test_gesture(%d)\n", length);
			return test_gesture(length);
		}
	else {
		printf("test4: %s - no valid function!\n", argv[1]);
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

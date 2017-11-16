/**
 * @file RTC.h
 */

#ifndef RTC_H
#define RTC_H

#define RTC_IRQ 8	/**< @brief RTC IRQ line */

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 0x04
#define DAY 0x07
#define DAY_WEEK 0x06
#define MONTH 0x08
#define YEAR 0x09
#define REG_A 10
#define REG_B 11
#define REG_C 12
#define REG_D 13

// Control register bits.
//Register A.
#define UIP BIT(7)
#define DV BIT(6) | BIT(5) | BIT(4)
#define RS BIT(3) | BIT(2) | BIT(1) | BIT(0)
//Register B.
#define SET BIT(7)
#define PIE BIT(6)
#define AIE BIT(5)
#define UIE BIT(4)
#define SQWE BIT(3)
#define DM BIT(2)
#define PMAM BIT(1)
#define DSE BIT(0)
//Register C.
#define IRQF BIT(7)
#define PF BIT(6)
#define AF BIT(5)
#define UF BIT(4)
//Register D.
#define VRT BIT(7)

typedef struct {
	unsigned hour;
	unsigned minute;
	unsigned second;
} Time;

typedef struct {
	unsigned day;
	unsigned month;
	unsigned year;
	unsigned hour;
	unsigned minute;
	unsigned second;
} DateTime;

typedef struct {
	char regA;
	char regB;
	char regC;
	char regD;
} rtcCtrlRegs;

/**
 * @brief Subscribes the RTC.
 * If it's already subscribed, it prints an error message and returns -1.
 * @return Original hook ID of the RTC if success, -1 otherwise.
 */
int rtc_subscribe_int();

/**
 * @brief Unsubscribes the RTC.
 * If it's not initially subscribed, it prints an error message and returns -1.
 * @return OK if success, -1 otherwise.
 */
int rtc_unsubscribe_int();

/**
 * It checks the UIP flag of Register A to not read while it updates time
 * and also the VRT flag of Register D to warn if RTC battery ran out of energy
 * which indicates questionable results.
 * @brief Get the current time.
 * @return Time with the current hour, minute and second.
 */
Time getTime();

/**
 * It checks the UIP flag of Register A to not read while it updates time
 * and also the VRT flag of Register D to warn if RTC battery ran out of energy
 * which indicates questionable results.
 * @brief Get the current date and time.
 * @return Date and time with the current year, month, day, hour, minute and second.
 */
DateTime getDateTime();

/**
 * Useful as a seed for srand().
 * @brief Gets the number of seconds since 0h00 of the first day of the given month.
 * @param time Date and time.
 * @return Seconds.
 */
unsigned getSeconds(DateTime *time);

/**
 * @brief Adds one second to a given time.
 * @param time Pointer to Time to be incremented.
 */
void incrementTime(Time *time);

/**
 * @brief Prints a given time in the format HH:MM:SS.
 * @param t Pointer to Time to be printed.
 */
void printTime(Time *t);

/**
 * @brief Prints a given date and time in the format DD/MM/AA HH:MM:SS.
 * @param dt Pointer to DateTime to be printed.
 */
void printDateTime(DateTime *dt);

/**
 * @brief Prints the content of the 4 RTC status registers.
 */
void rtc_printConf();

/**
 * @brief Gets the content of the 4 RTC status registers.
 * @return Struct with the registers' contents.
 */
rtcCtrlRegs rtc_getConf();

/**
 * @brief Converts a number from BCD to decimal.
 * @param bcdNum Number in BCD.
 * @return Number in decimal.
 */
unsigned bcdToDecimal(unsigned bcdNum);

#endif

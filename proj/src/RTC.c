#include "RTC.h"
#include <math.h>
#include <minix/driver.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "i8042.h"
#include "timer.h"

static int rtc_hook_id = 16;
static bool isSubscribed = false;

int rtc_subscribe_int() {
	if (isSubscribed) {
		printf("rtc_subscribe_int(): Subscribing already-subscribed RTC\n");
		return -1;
	}
	int temp_hook_id = rtc_hook_id;

	if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,
			&rtc_hook_id) != OK) {
		printf("RTC_subscribe_int(): irq set policy error\n");
		return -1;
	}

	if (sys_irqenable(&rtc_hook_id) != OK) {
		printf("RTC_subscribe_int(): irq enable error\n");
		return -1;
	}

	isSubscribed = true;
	return temp_hook_id;
}

int rtc_unsubscribe_int() {
	if (!isSubscribed) {
		printf("rtc_unsubscribe_int(): Unsubscribing non-subscribed RTC\n");
		return -1;
	}

	int temp_hook_id = rtc_hook_id;
	rtc_hook_id = 8;
	if (sys_irqdisable(&temp_hook_id) != OK) {
		printf("RTC_unsubscribe_int(): irq disable error\n");
		return -1;
	}
	if (sys_irqrmpolicy(&temp_hook_id) != OK) {
		printf("RTC_unsubscribe_int(): irq rm policy error\n");
		return -1;
	}

	isSubscribed = false;
	return OK;
}

//TODO: Disable and enable interrupts to prevent DD preemption.
Time getTime() {
	rtcCtrlRegs conf;
	do {
		conf = rtc_getConf();
		if (conf.regD & VRT == 0) {
			printf("getDateTime(): RTC battery out of energy, questionable results\n");
		}
		if (conf.regA & UIP) {
			waitByTimer0(pow(10, -5));
		}
	} while (conf.regA & UIP);

	unsigned long reg = 0;
	Time t;

	sys_outb(RTC_ADDR_REG, SECONDS);
	sys_inb(RTC_DATA_REG, &reg);
	t.second = bcdToDecimal(reg);

	sys_outb(RTC_ADDR_REG, MINUTES);
	sys_inb(RTC_DATA_REG, &reg);
	t.minute = bcdToDecimal(reg);

	sys_outb(RTC_ADDR_REG, HOURS);
	sys_inb(RTC_DATA_REG, &reg);
	t.hour = bcdToDecimal(reg);

	return t;
}

//TODO: Disable and enable interrupts to prevent DD preemption.
DateTime getDateTime() {
	rtcCtrlRegs conf;
	do {
		conf = rtc_getConf();
		if (conf.regD & VRT == 0) {
			printf("getDateTime(): RTC battery out of energy, questionable results\n");
		}
		if (conf.regA & UIP) {
			waitByTimer0(pow(10,-5));
		}
	} while (conf.regA & UIP);

	unsigned long reg = 0;
	DateTime dt;

	sys_outb(RTC_ADDR_REG, SECONDS);
	sys_inb(RTC_DATA_REG, &reg);
	dt.second = bcdToDecimal(reg);

	sys_outb(RTC_ADDR_REG, MINUTES);
	sys_inb(RTC_DATA_REG, &reg);
	dt.minute = bcdToDecimal(reg);

	sys_outb(RTC_ADDR_REG, HOURS);
	sys_inb(RTC_DATA_REG, &reg);
	dt.hour = bcdToDecimal(reg);

	sys_outb(RTC_ADDR_REG, YEAR);
	sys_inb(RTC_DATA_REG, &reg);
	dt.year = bcdToDecimal(reg);

	sys_outb(RTC_ADDR_REG, MONTH);
	sys_inb(RTC_DATA_REG, &reg);
	dt.month = bcdToDecimal(reg);

	sys_outb(RTC_ADDR_REG, DAY);
	sys_inb(RTC_DATA_REG, &reg);
	dt.day = bcdToDecimal(reg);

	return dt;
}

// Seconds since 00h00 of the first day of the current month.
unsigned getSeconds(DateTime *dt) {
	return (dt->day - 1) * 24 * 3600 + dt->hour * 3600 + dt->minute * 60 + dt->second;
}

void incrementTime(Time *time) {
	time->second++;
	if (time->second == 60) {
		time->minute++;
		time->second = 0;
	}
	if (time->minute == 60) {
		time->hour++;
		time->minute = 0;
	}
	if (time->hour == 24) {
		time->hour = 0;
	}
}

void printTime(Time *t) {
	printf("HH:MM:SS - %u:%u:%u\n", t->hour, t->minute, t->second);
}

//TODO: Force 2-digit prints.
void printDateTime(DateTime *dt) {
	printf("DD/MM/AA HH:MM:SS - %u/%u/%u %u:%u:%u\n", dt->day, dt->month, dt->year, dt->hour, dt->minute, dt->second);
}

void rtc_printConf() {
	rtcCtrlRegs conf = rtc_getConf();
	printf("Register A = 0x%x\n", conf.regA);
	printf("Register B = 0x%x\n", conf.regB);
	printf("Register C = 0x%x\n", conf.regC);
	printf("Register D = 0x%x\n", conf.regD);
}

rtcCtrlRegs rtc_getConf() {
	rtcCtrlRegs conf;

	unsigned long reg = 0;

	sys_outb(RTC_ADDR_REG, REG_A);
	sys_inb(RTC_DATA_REG, &reg);
	conf.regA = reg;

	sys_outb(RTC_ADDR_REG, REG_B);
	sys_inb(RTC_DATA_REG, &reg);
	conf.regB = reg;

	sys_outb(RTC_ADDR_REG, REG_C);
	sys_inb(RTC_DATA_REG, &reg);
	conf.regC = reg;

	sys_outb(RTC_ADDR_REG, REG_D);
	sys_inb(RTC_DATA_REG, &reg);
	conf.regD = reg;

	return conf;
}

unsigned bcdToDecimal(unsigned bcdNum) {
	unsigned long decimalRes = 0;
	unsigned bitLength = sizeof(bcdNum) * 8;

	for (size_t bit = 0; bit < bitLength; bit += 4) {
		unsigned temp = bcdNum >> bit;
		temp &= 0xF;
		decimalRes += temp * pow(10, bit / 4);
	}

	return decimalRes;
}

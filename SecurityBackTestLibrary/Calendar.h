#pragma once
#include "SysType.h"
#include "SysConst.h"
#include "SysOperator.h"

class Calendar
{
public:
	Calendar(Date startDate);
	Calendar(RawDate startDate);

	//===== Static Methods =====
	static RawDate convertToRawDate(Date date);
	static Date convertToDate(RawDate);
	static bool isDateValid(Date date);

	//===== Time Flow =====
	void operator++();

	//===== Time Event =====
	bool isFirstDayOfMonth();
	bool isFirstDayOfYear();

	//===== Get/Set =====
	void setToday(Date td);
	void setToday(RawDate rd);
	Date getToday();
	RawDate getRawToday();

private:
	//===== Data =====
	RawDate rawToday{};
	Date today{};

	//===== calculations =====
	static int calcYear(int& t);
	static int calcMonth(int& t, int y);

	void oneDayPass();
	void oneMonthPass();
	void oneYearPass();
};
#include "Calendar.h"
#include "ErrorPacket.h"
#include <cmath>
#include <iostream>

Calendar::Calendar(Date startDate)
	:rawToday{ convertToRawDate(startDate) }, today{ startDate.d, startDate.m, startDate.y }
{

}

Calendar::Calendar(RawDate startDate)
	: rawToday{ startDate }
{
	today = convertToDate(startDate);
}

///////////////////////////////////////////////////////////////////
// Date Conversion
///////////////////////////////////////////////////////////////////
RawDate Calendar::convertToRawDate(Date date)
{
	if (!Calendar::isDateValid(date))
	{
		throw(ErrorPacket{ ErrorCode::INVALID_DATE,date });
	}

	int tempRaw = 0;

	for (int i = 1980; i < date.y; i++)
	{
		bool isLeap = (i % 4 == 0);
		isLeap ? tempRaw += 366 : tempRaw += 365;
	}

	int monthLength[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	bool isLeap = (date.y % 4 == 0);
	if (isLeap) { monthLength[1] = 29; };
	for (int i = 1; i < date.m; i++)
	{
		tempRaw += monthLength[i - 1];
	}

	tempRaw += date.d - 1;

	return tempRaw;
}

Date Calendar::convertToDate(RawDate t)
{
	if (t < 0)
	{
		throw(ErrorPacket{ ErrorCode::INVALID_RAWDATE,t });
	}

	Date date{};
	int remain{ t };

	date.y = calcYear(remain);
	date.m = calcMonth(remain, date.y);
	date.d = remain + 1;

	return date;
}

int Calendar::calcYear(int& tempDate)
{
	int tempYear = 1980;
	int leapYear{ 4 };
	int leapCount{ 0 };
	bool isLeap = (tempYear % 4 == 0);

	int yearLength = (isLeap) ? 366 : 365;

	while (tempDate >= yearLength)
	{
		if (leapCount % leapYear == 0)
		{
			tempDate -= 366;
			leapCount = 0;
		}
		else
		{
			tempDate -= 365;
		}

		leapCount++;
		tempYear++;
		isLeap = (tempYear % 4 == 0);
		yearLength = (isLeap) ? 366 : 365;
	}
	return tempYear;
}

int Calendar::calcMonth(int& tempDate, int year)
{
	int thisMonth{ 1 };
	int monthLength[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

	bool isLeap = (year % 4 == 0);
	if (isLeap) {monthLength[1] = 29;}

	for (int i = 0; i < 12; i++)
	{
		if (tempDate < monthLength[i])
		{
			thisMonth = i + 1;
			break;
		}
		tempDate -= monthLength[i];
	}

	return thisMonth;
}

///////////////////////////////////////////////////////////////////
// time Flow
///////////////////////////////////////////////////////////////////
void Calendar::operator++()
{
	oneDayPass();
}

void Calendar::oneDayPass()
{
	rawToday++;
	if (today.d == 28)
	{
		bool isLeap = today.y % 4 == 0;
		if (!isLeap && today.m == 2)
		{
			oneMonthPass();
			return;
		}
	}
	else if (today.d == 29)
	{
		bool isLeap = today.y % 4 == 0;
		if (isLeap && today.m == 2)
		{
			oneMonthPass();
			return;
		}
	}
	else if (today.d == 30)
	{
		if (today.m == 4 || today.m == 6 || today.m == 9 || today.m == 11)
		{
			oneMonthPass();
			return;
		}
	}
	else if (today.d == 31)
	{
		if (today.m == 12)
		{
			oneYearPass();
			return;
		}
		oneMonthPass();
		return;
	}
	today.d++;
}

void Calendar::oneMonthPass()
{
	today.m++;
	today.d = 1;
}

void Calendar::oneYearPass()
{
	today.y++;
	today.m = 1;
	today.d = 1;
}

///////////////////////////////////////////////////////////////////
// Time Flow
///////////////////////////////////////////////////////////////////
bool Calendar::isFirstDayOfMonth()
{
	return (today.d == 1) ? true : false;
}

bool Calendar::isFirstDayOfYear()
{
	return (today.d == 1 && today.m == 1) ? true : false;
}

bool Calendar::isDateValid(Date date)
{
	int y = date.y;
	int m = date.m;
	int d = date.d;

	if ((y < 1980 || m < 1 || m > 12 || d < 1 || d > 31))
	{
		return false;
	}
	if ((m == 4 || m == 6 || m == 9 || m == 11) && d > 30)
	{
		return false;
	}
	if ((m == 2 && (y % 4 == 0)) && d > 29)
	{
		return false;
	}
	if ((m == 2 && (y % 4 != 0)) && d > 28)
	{
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////
// Get/Set
///////////////////////////////////////////////////////////////////
void Calendar::setToday(Date td)
{
	if (!Calendar::isDateValid(td))
	{
		throw(ErrorPacket{ ErrorCode::INVALID_DATE,td });
	}

	today = td;
	rawToday = convertToRawDate(td);
}

void Calendar::setToday(RawDate rd)
{
	if (rd < 0)
	{
		throw(ErrorPacket{ ErrorCode::INVALID_RAWDATE,rd });
	}
	rawToday = rd;
	today = convertToDate(rd);
}

Date Calendar::getToday()
{
	return today;
}

RawDate Calendar::getRawToday()
{
	return rawToday;
}
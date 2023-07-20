#include "ControlPanel.h"
#include "CalendarTest.h"
#include <array>

///////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////

bool CalendarTest::test()
{
	std::cout << "========Test Calendar Class=======\n";
	bool isPass{ true };

	isPass &= testDateValidation();
	isPass &= testDateConvertion();
	isPass &= testDatePass();
	isPass &= testDateEvent();


	if (isPass)
		std::cout << "--<Pass>--\n\n";
	else
		std::cout << "--<Fail>--\n\n";

	return isPass;
}

bool CalendarTest::testDateValidation()
{
	std::cout << "[Date Validation]:\n";
	bool isPass{ false };

	bool b1 = Calendar::isDateValid(Date{ 1,1,1980 });
	std::cout << "\t1/1/1980: " << (b1 ? "Valid\n" : "Not Valid\n");
	bool b2 = Calendar::isDateValid(Date{ 29,2,1992 });
	std::cout << "\t29/2/1992: " << (b2 ? "Valid\n" : "Not Valid\n");
	bool b3 = Calendar::isDateValid(Date{ 31,12,2980 });
	std::cout << "\t31/12/2980: " << (b3 ? "Valid\n" : "Not Valid\n");
	bool b4 = Calendar::isDateValid(Date{ 0,1,1980 });
	std::cout << "\t0/1/1980: " << (b4 ? "Valid\n" : "Not Valid\n");
	bool b5 = Calendar::isDateValid(Date{ 32,4,1985 });
	std::cout << "\t32/4/1985: " << (b5 ? "Valid\n" : "Not Valid\n");
	bool b6 = Calendar::isDateValid(Date{ 1,5,1979 });
	std::cout << "\t1/5/1979: " << (b6 ? "Valid\n" : "Not Valid\n");
	bool b7 = Calendar::isDateValid(Date{ 1,13,1980 });
	std::cout << "\t1/13/1980: " << (b7 ? "Valid\n" : "Not Valid\n");
	bool b8 = Calendar::isDateValid(Date{ 30,2,1980 });
	std::cout << "\t30/2/1980: " << (b8 ? "Valid\n" : "Not Valid\n");
	bool b9 = Calendar::isDateValid(Date{ 29,2,1981 });
	std::cout << "\t29/2/1981: " << (b9 ? "Valid\n" : "Not Valid\n");

	isPass = b1 && b2 && b3 && !b4 && !b5 && !b6 && !b7 && !b8 && !b9;

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

///////////////////////////////////////////////////////////////////
// Raw Date and Date Conversion
///////////////////////////////////////////////////////////////////

bool CalendarTest::testDateConvertion()
{
	std::cout << "[Date Convertion]:\n";
	bool isPass{ true };

	isPass &= testKeyDateConversion();
	isPass &= testAllDateConversion();

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool CalendarTest::testKeyDateConversion()
{
	bool isPass{ false };

	const int fixTestSize = 5;
	std::array<Date, fixTestSize> dateArr{ Date{1,1,1980},Date{29,2,1984},Date{31,12,1999},Date{1,1,2000},Date{1,3,2023} };
	std::array<RawDate, fixTestSize> rawDateArr{ 0,1520,7304,7305,15765 };

	for (int i = 0; i < fixTestSize; i++)
	{
		bool isCorrect = dateArr[i] == Calendar::convertToDate(rawDateArr[i]) && rawDateArr[i] == Calendar::convertToRawDate(dateArr[i]);
	
		std::cout << "\t" << dateArr[i] << " => " << Calendar::convertToDate(Calendar::convertToRawDate(dateArr[i])) << "\n";
		
		if (isCorrect)
		{
			isPass = true;
		}
	}
	return isPass;
}

bool CalendarTest::testAllDateConversion()
{
	bool isPass{ false };
	RawDate rd{ 0 };
	Date d1{ 1,1,1980 };

	int testIter{ 5000 };
	int correctIter{ 0 };

	for (RawDate rd = 0; rd < testIter; rd++)
	{
		d1 = Calendar::convertToDate(rd);
		if (rd == Calendar::convertToRawDate(d1))
		{
			correctIter++;
		}
	}

	if (testIter == correctIter)
	{
		isPass = true;
	}
	std::cout << "\tFull Test:" << correctIter << "/" << testIter << "\n";
	return isPass;
}

///////////////////////////////////////////////////////////////////
// Date Counter
///////////////////////////////////////////////////////////////////

bool CalendarTest::testDatePass()
{
	std::cout << "[Date Pass Counter]:\n";

	bool isPass{ false };

	bool isStartDayPass = testDatePassFrom(Date{ 1,1,1980 });
	bool isLeapDayPass = testDatePassFrom(Date{ 29,2,2000 });
	bool isNonLeapDayPass = testDatePassFrom(Date{ 28,2,2012 });

	isPass = isStartDayPass && isLeapDayPass && isNonLeapDayPass;

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;	
}

bool CalendarTest::testDatePassFrom(Date startDay)
{
	bool isPass{ false };
	Calendar calendar{ startDay };
	RawDate startDate{ Calendar::convertToRawDate(startDay) };
	int testIter = 5000;
	int correctIter = 0;

	for (RawDate rd = startDate; rd < startDate + testIter; rd++)
	{
		bool isCorrect = (rd == calendar.getRawToday());
		if (isCorrect)
		{
			correctIter++;
		}
		++calendar;
	}

	if (testIter == correctIter)
	{
		isPass = true;
	}
	std::cout << "\tStart from " << startDay << ": " << correctIter << "/" << testIter << "\n";

	return isPass;
}

///////////////////////////////////////////////////////////////////
// Date Events
///////////////////////////////////////////////////////////////////

bool CalendarTest::testDateEvent()
{
	std::cout << "[Date Event]:\n";
	bool isPass{ true };

	isPass &= testFirstDayOfAPeriodFix();
	isPass &= testFirstDayOfAPeriodAll();

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool CalendarTest::testFirstDayOfAPeriodFix()
{
	bool isPass{ true };

	Calendar calendar{ Date{1,1,1995} };
	std::cout << "\t" << calendar.getToday() << " Month start=" << calendar.isFirstDayOfMonth() << " Year start=" << calendar.isFirstDayOfYear() << '\n';
	if (calendar.isFirstDayOfMonth() && calendar.isFirstDayOfYear())
	{
		isPass = true;
	}

	calendar.setToday(Date{ 1,7,2005 });
	std::cout << "\t" << calendar.getToday() << " Month start=" << calendar.isFirstDayOfMonth() << " Year start=" << calendar.isFirstDayOfYear() << '\n';
	if (calendar.isFirstDayOfMonth() && calendar.isFirstDayOfYear())
	{
		isPass = true;
	}

	calendar.setToday(Date{ 28,11,2017 });
	std::cout << "\t" << calendar.getToday() << " Month start=" << calendar.isFirstDayOfMonth() << " Year start=" << calendar.isFirstDayOfYear() << '\n';
	if (calendar.isFirstDayOfMonth() && calendar.isFirstDayOfYear())
	{
		isPass = true;
	}

	return isPass;
}

bool CalendarTest::testFirstDayOfAPeriodAll()
{
	bool isPass{ false };

	Calendar calendar{ Date{1,1,1980} };
	int monthStartCount{ 0 };
	int yearStartCount{ 0 };
	for (RawDate rd{ 0 }; rd < 2000; rd++)
	{
		if (calendar.isFirstDayOfMonth()) monthStartCount++;
		if (calendar.isFirstDayOfYear()) yearStartCount++;
		++calendar;
	}

	int monthCountAns{ 66 };
	int yearCountAns{ 6 };
	std::cout << "\tMonth Count: " << monthStartCount << " (" << monthCountAns
		<< ") Year Count: " << yearStartCount << " (" << yearCountAns << ")\n";
	bool isEventCountCorrect = monthStartCount == monthCountAns && yearStartCount == yearCountAns;
	if (isEventCountCorrect)
	{
		isPass = true;
	}

	return isPass;
}
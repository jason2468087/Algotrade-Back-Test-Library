#include "ShareTest.h"

///////////////////////////////////////////////////////////////////
// Read Price File
///////////////////////////////////////////////////////////////////

bool ShareTest::test()
{
	std::cout << "========Test Share Class=======\n";
	bool isPass{ true };
	Share share{ "SPY" };

	isPass &= testShareConstruction(share);
	isPass &= testAccessSharePrice(share);


	if (isPass)
		std::cout << "--<Pass>--\n\n";
	else
		std::cout << "--<Fail>--\n\n";

	return isPass;
}

///////////////////////////////////////////////////////////////////
// Share Construction
///////////////////////////////////////////////////////////////////

bool ShareTest::testShareConstruction(Share share)
{
	std::cout << "[Test Share Construction]\n";
	bool isPass{ false };

	RawDate startDate{ 4777 };
	RawDate endDate{ Calendar::convertToRawDate(Date{21,12,2020}) };
	bool isCorrect = share.getStartDate() == startDate && share.getEndDate() == 15876
		&& share.getClose(startDate) == 43.94_USD && share.getLastDividend(endDate) == 158;
	if (isCorrect)
	{
		isPass = true;
	}

	std::cout << "\tSPY start date = " << share.getStartDate() << "(4777)\n";
	std::cout << "\tSPY end date = " << share.getEndDate() << "(15876)\n";
	std::cout << "\tSPY initial price = " << ptos(share.getClose(startDate)) << "(43.94)\n";
	std::cout << "\tSPY Last Dividend at 21/12/2020 = " << ptos(share.getLastDividend(endDate)) << "(1.58)\n";

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

///////////////////////////////////////////////////////////////////
// Access Share Price
///////////////////////////////////////////////////////////////////

bool ShareTest::testAccessSharePrice(Share share)
{
	std::cout << "[Test Share Construction]\n";
	bool isPass{ true };

	isPass &= testAccessTradeDayPrice(share);
	isPass &= testAccessRestDayPrice(share);

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool ShareTest::testAccessTradeDayPrice(Share share)
{
	bool isPass{ false };

	RawDate date1{ Calendar::convertToRawDate(Date{26,10,1995}) };
	if (share.getClose(date1) == 57.75_USD)
	{
		isPass = true;
	}
	std::cout << "\tSPY 26/10/1995 Close = " << ptos(share.getClose(date1)) << "(57.75)\n";

	RawDate date2{ Calendar::convertToRawDate(Date{27,10,1995}) };
	if (share.getClose(date2) == 58.19_USD)
	{
		isPass = true;
	}
	std::cout << "\tSPY 27/10/1995 Close = " << ptos(share.getClose(date2)) << "(58.19)\n";

	return isPass;
}

bool ShareTest::testAccessRestDayPrice(Share share)
{
	bool isPass{ false };

	RawDate date{ Calendar::convertToRawDate(Date{3,1,2004}) };
	if (share.getClose(date) == 111.23_USD)
	{
		isPass = true;
	}
	std::cout << "\tSPY 3/1/2004 Close = " << ptos(share.getClose(date)) << "(111.23)\n";

	return isPass;
}
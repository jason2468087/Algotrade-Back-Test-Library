#include "IndexTest.h"

bool IndexTest::test()
{
	std::cout << "========Test Index Class=======\n";
	bool isPass{ true };

	isPass &= testMovingAverage();

	if (isPass)
		std::cout << "--<Pass>--\n\n";
	else
		std::cout << "--<Fail>--\n\n";

	return isPass;
}

bool IndexTest::testMovingAverage()
{
	bool isPass{ false };

	Market market{};
	market.observe("VOO");

	Index index{ market };

	Price mAvg1{ index.movingAverage(Date{28,9,2010},14,"VOO") };
	double rsi{ index.relativeStrength(Date{18,1,2011},14,"VOO") };

	std::cout << "\tVOO 14 Day MA at 28/9/2010 " << ptos(mAvg1) << "($103.61)\n";
	std::cout << "\tVOO 14 Day RSI at 18/1/2011 " << rsi << "(81.7368)\n";

	isPass = mAvg1 == 103.61_USD && dtop(rsi) == 81.7368_USD;

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}
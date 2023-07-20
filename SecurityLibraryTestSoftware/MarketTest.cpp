#include "MarketTest.h"

///////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////
bool MarketTest::test()
{
	std::cout << "========Test Market Class=======\n";
	bool isPass{ true };

	Market market{};
	market.update(Date{ 1,3,2015 });

	isPass &= testObserve(market);
	isPass &= testUpdate(market);

	if (isPass)
		std::cout << "--<Pass>--\n\n";
	else
		std::cout << "--<Fail>--\n\n";

	return isPass;
}

bool MarketTest::testObserve(Market& market)
{
	std::cout << "[Observe Stock]:\n";
	bool isPass{ false };

	market.observe("VOO");
	market.observe("MSFT");

	Price vooClose{ market.getClose("VOO",Date{1,3,2015}) };
	bool msftIsTrade{ market.getIsTraded("MSFT",Date{4,2,2006}) };

	isPass = vooClose == 193.2_USD && msftIsTrade == false;

	std::cout << "\tVOO Close at 1/3/2015: " << ptos(vooClose) << "($193.2)\n";
	std::cout << "\tMSFT is traded at 4/2/2006: " << msftIsTrade << "(0)\n";

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool MarketTest::testUpdate(Market& market)
{
	std::cout << "[Update Market]:\n";
	bool isPass{ false };

	market.update(Date{ 26,9,2018 });
	market.update(Date{ 22,10,2018 });

	Price vooPrice{ market.getTodayOpen("VOO")};

	isPass = vooPrice == 254.55_USD;

	std::cout << "\tVOO Close at 1/3/2015: " << ptos(vooPrice) << "($254.55)\n";

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}
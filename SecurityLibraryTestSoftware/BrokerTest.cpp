#include "BrokerTest.h"

bool BrokerTest::test()
{
	bool isPass{ true };
	std::cout << "========Test Broker Class=======\n";
	Market market{};

	isPass &= testHSBCBroker(market);

	if (isPass)
		std::cout << "--<Pass>--\n\n";
	else
		std::cout << "--<Fail>--\n\n";

	return isPass;
}

bool BrokerTest::testHSBCBroker(Market& market)
{
	std::cout << "[HSBC Broker]:\n";
	bool isPass{ true };

	Portfolio portfolio{ market };
	portfolio.setBroker(BrokerName::HSBC);

	isPass &= testHSBCOver1000(portfolio);
	isPass &= testHSBCUnder1000(portfolio);

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool BrokerTest::testHSBCOver1000(Portfolio& portfolio)
{
	bool isPass{ false };

	int amount{ 3001 };
	Price price{ 45_USD * amount };
	Price tradeFee{ portfolio.getBroker().getTradeFee(price,amount) };
	Price monthlyFee{ portfolio.getBroker().getMonthlyFee() };
	Price yearlyFee{ portfolio.getBroker().getYearlyFee() };

	std::cout << "\tTrade 3001 stock with $45\n";
	std::cout << "\tTrade Fee = " << ptos(tradeFee) << "($48.02)\n";
	std::cout << "\tMonth Fee = " << ptos(monthlyFee) << "($0)\n";
	std::cout << "\tYearly Fee = " << ptos(yearlyFee) << "($0)\n\n";

	if (tradeFee == 48.02_USD)
	{
		isPass = true;
	}

	return isPass;
}

bool BrokerTest::testHSBCUnder1000(Portfolio& portfolio)
{
	bool isPass{ false };

	int amount{ 500 };
	Price price{ 45_USD * amount };
	Price tradeFee{ portfolio.getBroker().getTradeFee(price,amount) };
	Price monthlyFee{ portfolio.getBroker().getMonthlyFee() };
	Price yearlyFee{ portfolio.getBroker().getYearlyFee() };

	std::cout << "\tTrade 500 stock with $45\n";
	std::cout << "\tTrade Fee = " << ptos(tradeFee) << "($18)\n";
	std::cout << "\tMonth Fee = " << ptos(monthlyFee) << "($0)\n";
	std::cout << "\tYearly Fee = " << ptos(yearlyFee) << "($0)\n\n";

	if (tradeFee == 18_USD)
	{
		isPass = true;
	}

	return isPass;
}
#include "PortfolioTest.h"

///////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////

bool PortfolioTest::test()
{
	std::cout << "========Test Portfolio Class=======\n";
	bool isPass{ true };
	Market market{};

	isPass &= testMarketOrder(market);
	isPass &= testFixedPriceOrder(market);
	isPass &= testReceiveSalary(market);
	isPass &= testReceiveDividend(market);

	if (isPass)
		std::cout << "--<Pass>--\n\n";
	else
		std::cout << "--<Fail>--\n\n";

	return isPass;
}

///////////////////////////////////////////////////////////////////
// Market Orders
///////////////////////////////////////////////////////////////////

bool PortfolioTest::testMarketOrder(Market& market)
{
	std::cout << "[Market Order]:\n";
	bool isPass{ true };

	Portfolio portfolio{ market };

	isPass &= testMarketOrderBuy(portfolio, market);
	isPass &= testMarketOrderSell(portfolio, market);

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool PortfolioTest::testMarketOrderBuy(Portfolio& portfolio, Market& market)
{
	bool isPass{ false };

	market.observe("HSBC");
	market.update(Date{ 3,1,2000 });
	std::cout << market.getOpen("HSBC", Date{ 3,1,2000 });
	portfolio.setToday(Date{ 3,1,2000 });
	portfolio.buyStock("HSBC", 5);
	portfolio.buyStock("HSBC", 5);
	portfolio.buyStock("HSBC", 10);
	bool isCorrect = portfolio.getCash() == 97840.0_USD && portfolio.getShareHolded("HSBC") == 20;
	if (isCorrect)
	{
		isPass = true;
	}
	std::cout << "\tBuy 20 HSBC at 3/1/2000\n";
	std::cout << "\tCash = " << ptos(portfolio.getCash()) << "($97840) HSBC share = " << portfolio.getShareHolded("HSBC") << "\n";

	return isPass;
}

bool PortfolioTest::testMarketOrderSell(Portfolio& portfolio, Market& market)
{
	bool isPass{ false };

	market.update(Date{ 14,2,2001 });
	portfolio.setToday(Date{ 14,2,2001 });
	portfolio.sellStock("HSBC", 5);
	portfolio.sellStock("HSBC", 5);
	portfolio.sellStock("HSBC", 10);
	bool isCorrect = portfolio.getCash() == 100250.0_USD && portfolio.getShareHolded("HSBC") == 0;
	if (isCorrect)
	{
		isPass = true;
	}
	std::cout << "\tSell 20 HSBC at 14/2/2001\n";
	std::cout << "\tCash = " << ptos(portfolio.getCash()) << "(100250) HSBC share = " << portfolio.getShareHolded("HSBC") << "\n";

	return isPass;
}

///////////////////////////////////////////////////////////////////
// Fixed Price Orders
///////////////////////////////////////////////////////////////////

bool PortfolioTest::testFixedPriceOrder(Market& market)
{
	std::cout << "[Fixed Price Order]:\n";
	bool isPass{ true };

	Portfolio portfolio{ market };

	isPass &= testFixedPriceOrderBuy(portfolio, market);
	isPass &= testFixedPriceOrderSell(portfolio, market);

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool PortfolioTest::testFixedPriceOrderBuy(Portfolio& portfolio, Market& market)
{
	bool isPass{ false };

	portfolio.buyStock("VOO", 21, 397.63_USD);
	portfolio.buyStock("VOO", 18, 399.23_USD);
	portfolio.buyStock("VOO", 11, 417.81_USD);
	bool isCorrect = portfolio.getCash() == 79867.72_USD && portfolio.getShareHolded("VOO") == 50;
	if (isCorrect)
	{
		isPass = true;
	}
	std::cout << "\tBuy VOO 3 time in pre-set price\n";
	std::cout << "\tCash = " << ptos(portfolio.getCash()) << "(79867.72) VOO share = " << portfolio.getShareHolded("VOO") << "(50)\n";

	return isPass;
}

bool PortfolioTest::testFixedPriceOrderSell(Portfolio& portfolio, Market& market)
{
	bool isPass{ false };

	portfolio.sellStock("VOO", 21, 397.63_USD);
	portfolio.sellStock("VOO", 18, 399.23_USD);
	portfolio.sellStock("VOO", 11, 417.81_USD);
	bool isCorrect = portfolio.getCash() == 100000.0_USD && portfolio.getShareHolded("VOO") == 0;
	if (isCorrect)
	{
		isPass = true;
	}
	std::cout << "\tSell VOO 3 time in pre-set price\n";
	std::cout << "\tCash = " << ptos(portfolio.getCash()) << "(100000) VOO share = " << portfolio.getShareHolded("VOO") << "(0)\n";

	return isPass;
}

///////////////////////////////////////////////////////////////////
// Receive Salary & Dividend
///////////////////////////////////////////////////////////////////

bool PortfolioTest::testReceiveSalary(Market& market)
{
	std::cout << "[Receive salary]:\n";
	bool isPass{ true };

	Portfolio portfolio{ market };
	portfolio.receiveSalary(10000.0_USD);
	bool isCorrect = portfolio.getCash() == 110000.0_USD;
	if (isCorrect)
	{
		isPass = true;
	}
	std::cout << "\tCash after $10000 salary = " << ptos(portfolio.getCash()) << "(110000)\n";

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool PortfolioTest::testReceiveDividend(Market& market)
{
	std::cout << "[Receive dividend]:\n";
	bool isPass{ false };

	Portfolio portfolio{ market };
	portfolio.receiveDividend("BND",500.0_USD);
	bool isCorrect = portfolio.getCash() == 100500.0_USD;
	if (isCorrect)
	{
		isPass = true;
	}
	std::cout << "\tCash after $500 dividend = " << ptos(portfolio.getCash()) << "(100500)\n";

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}
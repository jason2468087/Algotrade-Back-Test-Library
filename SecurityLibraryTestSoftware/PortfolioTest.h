#pragma once

#include "ControlPanel.h"

namespace PortfolioTest
{
	bool test();

	bool testMarketOrder(Market& market);
	bool testMarketOrderBuy(Portfolio& portfolio, Market& market);
	bool testMarketOrderSell(Portfolio& portfolio, Market& market);

	bool testFixedPriceOrder(Market& market);
	bool testFixedPriceOrderBuy(Portfolio& portfolio, Market& market);
	bool testFixedPriceOrderSell(Portfolio& portfolio, Market& market);

	bool testReceiveSalary(Market& market);
	bool testReceiveDividend(Market& market);
}
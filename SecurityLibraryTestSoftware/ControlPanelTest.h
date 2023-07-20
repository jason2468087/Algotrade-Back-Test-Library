#pragma once

#include "ControlPanel.h"

namespace ControlPanelTest
{
	bool test();

	bool testRepeatEvent();
	bool testDefaultRepeatEvent();
	bool testFixedDateRepeatEvent();
	void dailyTimeFlowAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market,Index& index);
	void monthTimeFlowAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index);
	void yearTimeFlowAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index);

	bool testSingleEvent();
	void singleAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index);

	bool testReceiveDividend();
	void initDividendAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index);

	bool testRunTrade();
	void initTradeAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index);
	void monthTradeAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index);

	bool testOrder();
	bool testLimitBuyOrder();
	void initLimitBuyAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index);
	bool testStopBuyOrder();
	void initStopBuyAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index);
	bool testLimitSellOrder();
	void initLimitSellAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index);
	bool testStopSellOrder();
	void initStopSellAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index);

}
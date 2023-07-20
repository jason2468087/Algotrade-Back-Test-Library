#pragma once

#include "ControlPanel.h"

namespace BrokerTest
{
	bool test();

	bool testHSBCBroker(Market& market);
	bool testHSBCOver1000(Portfolio& portfolio);
	bool testHSBCUnder1000(Portfolio& portfolio);
}
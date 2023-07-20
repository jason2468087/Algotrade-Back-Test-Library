#pragma once

#include "ControlPanel.h"

namespace ShareTest
{
	bool test();

	bool testShareConstruction(Share);

	bool testAccessSharePrice(Share);
	bool testAccessTradeDayPrice(Share);
	bool testAccessRestDayPrice(Share);
}
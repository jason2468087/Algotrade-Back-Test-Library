#pragma once

#include "ControlPanel.h"

namespace CalendarTest
{
	bool test();

	bool testDateValidation();

	bool testDateConvertion();
	bool testKeyDateConversion();
	bool testAllDateConversion();

	bool testDatePass();
	bool testDatePassFrom(Date d);

	bool testDateEvent();
	bool testFirstDayOfAPeriodFix();
	bool testFirstDayOfAPeriodAll();
}
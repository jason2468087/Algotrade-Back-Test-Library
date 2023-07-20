#pragma once

#include "ControlPanel.h"

namespace ProfitLogTest
{
	bool test();

	bool testLogCapitalGain();
	bool testLogFixIncome();

	bool verifySavedFile(std::string ans, Path path);
}
#pragma once

#include "ControlPanel.h"

namespace AssetLogTest
{
	bool test();

	bool testLogTotalWorth();
	bool testLogStockWorth();
	bool testStockRatio();

	bool verifySavedFile(std::string ans, Path path);
}
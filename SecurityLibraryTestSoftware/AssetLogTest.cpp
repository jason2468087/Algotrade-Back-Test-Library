#include "AssetLogTest.h"
#include "Share.h"
#include "SysOperator.h"

#include <string>
#include <fstream>

bool AssetLogTest::test()
{
	std::cout << "========Test Asset Log Class=======\n";
	bool isPass{ true };

	isPass &= testLogTotalWorth();
	isPass &= testLogStockWorth();
	isPass &= testStockRatio();

	if (isPass)
		std::cout << "--<Pass>--\n\n";
	else
		std::cout << "--<Fail>--\n\n";

	return isPass;
}

bool AssetLogTest::testLogTotalWorth()
{
	bool isPass{ false };
	std::cout << "[Test Log Total Worth]:\n";

	AssetLogHandler assetLogHandler{};
	assetLogHandler.logTotalWorth(Date{ 24,6,2015 }, 1200_USD);
	assetLogHandler.logTotalWorth(Date{ 30,6,2015 }, 800_USD);

	assetLogHandler.saveTotalWorthLog();

	std::string ans{ R"(24/6/2015,$1200.00
30/6/2015,$800.00
)" };

	isPass = verifySavedFile(ans, TOTAL_WORTH_LOG_PATH);

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool AssetLogTest::testLogStockWorth()
{
	bool isPass{ false };
	std::cout << "[Test Log Stock Worth]:\n";

	AssetLogHandler assetLogHandler{};
	assetLogHandler.logStockWorth(Date{ 24,6,2015 }, std::vector<Price>{134_USD,2500_USD,1833_USD});
	assetLogHandler.logStockWorth(Date{ 30,6,2015 }, std::vector<Price>{89.45_USD,25700_USD,0.58_USD});

	assetLogHandler.setHeaderSymbolRow(std::vector<Symbol>{"AAA", "BBB", "CCC"});
	assetLogHandler.saveStockWorthLog();

std::string ans{ R"(,AAA,BBB,CCC,Cash,
24/6/2015,$134.00,$2500.00,$1833.00,
30/6/2015,$89.45,$25700.00,$0.58,
)" };

	isPass = verifySavedFile(ans, STOCK_WORTH_LOG_PATH);

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool AssetLogTest::testStockRatio()
{
	bool isPass{ false };
	std::cout << "[Test Log Stock Ratio]:\n";

	AssetLogHandler assetLogHandler{};
	assetLogHandler.logStockRatio(Date{ 24,6,2015 }, std::vector<double>{0.27, 0.175, 0.555});
	assetLogHandler.logStockRatio(Date{ 30,6,2015 }, std::vector<double>{0.422,0.18,0.398});

	assetLogHandler.setHeaderSymbolRow(std::vector<Symbol>{"AAA", "BBB", "CCC"});
	assetLogHandler.saveStockRatioLog();

	std::string ans{ R"(,AAA,BBB,CCC,Cash,
24/6/2015,0.27,0.175,0.555,
30/6/2015,0.422,0.18,0.398,
)" };

	isPass = verifySavedFile(ans, STOCK_RATIO_LOG_PATH);

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool AssetLogTest::verifySavedFile(std::string ans, Path path)
{
	bool isPass{ false };

	std::ifstream file{};
	file.open(path);
	if (file.is_open())
	{
		std::string str{ std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>() };
		if (str == ans)
		{
			isPass = true;
		}
	}
	std::cout << "\tSaved content correct? " << isPass << " \n";

	return isPass;
}
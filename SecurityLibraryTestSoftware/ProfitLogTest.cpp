#include "ProfitLogTest.h"

#include <string>
#include <fstream>

bool ProfitLogTest::test()
{
	std::cout << "========Test Profit Log Class=======\n";
	bool isPass{ true };

	isPass &= testLogCapitalGain();
	isPass &= testLogFixIncome();

	if (isPass)
		std::cout << "--<Pass>--\n\n";
	else
		std::cout << "--<Fail>--\n\n";

	return isPass;
}

bool ProfitLogTest::testLogCapitalGain()
{
	bool isPass{ false };

	std::cout << "[Test Log Capital Gain]:\n";
	ProfitLogHandler profitLogHandler{};

	std::unordered_map<Symbol, Price> worthArr{};
	worthArr.emplace("ANF", 37.5_USD);
	worthArr.emplace("BBRE", 126_USD);
	worthArr.emplace("CVD", 162_USD);
	profitLogHandler.updateTradeBalance("ANF", 42.6_USD);
	profitLogHandler.updateTradeBalance("BBRE", 85.7_USD);
	profitLogHandler.updateTradeBalance("CVD", 174_USD);

	profitLogHandler.logCapitalGain(Date{ 19,12,2004 }, worthArr);
	profitLogHandler.logCapitalGain(Date{ 23,12,2004 }, worthArr);
	profitLogHandler.setHeaderSymbolRow(std::vector<Symbol>{"ANF", "BBRE", "CVD"});
	profitLogHandler.saveCapitalGainLog();

	std::string ans{ R"(,CVD,BBRE,ANF,
19/12/2004,-$12.00,$40.30,-$5.10,
23/12/2004,-$12.00,$40.30,-$5.10,
)" };

	isPass = verifySavedFile(ans, CAPITAL_GAIN_LOG_PATH);

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool ProfitLogTest::testLogFixIncome()
{
	bool isPass{ false };

	std::cout << "[Test Log Fixed Income]:\n";

	ProfitLogHandler profitLogHandler{};
	std::vector<Price> divArr1{ 0_USD,3.26_USD,0_USD };
	std::vector<Price> divArr2{ 0.74_USD,30_USD,0_USD };
	std::vector<Price> divArr3{ 0_USD,1.85_USD,0_USD };

	profitLogHandler.logDividend(Date{ 7,10,2004 },divArr1);
	profitLogHandler.logDividend(Date{ 7,11,2004 }, divArr2);
	profitLogHandler.logDividend(Date{ 7,12,2004 }, divArr3);
	profitLogHandler.logSalary(Date{ 7,12,2004 }, 10_USD);
	profitLogHandler.setHeaderSymbolRow(std::vector<Symbol>{"ANF", "BBRE", "CVD"});
	profitLogHandler.saveFixedIncomeLog();

	std::string ans{ R"(,ANF,BBRE,CVD,Salary,
7/10/2004,$0,$3.26,$0,$0,
7/11/2004,$0.74,$33.26,$0,$0,
7/12/2004,$0.74,$35.11,$0,$10.00,
)" };

	isPass = verifySavedFile(ans, FIXED_INCOME_LOG_PATH);

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool ProfitLogTest::verifySavedFile(std::string ans, Path path)
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
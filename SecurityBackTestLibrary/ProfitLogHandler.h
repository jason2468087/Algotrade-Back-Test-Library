#pragma once
#include "SysType.h"

#include <utility>
#include <vector>

class ProfitLogHandler
{
public:
	//===== Log Event =====
	void logCapitalGain(Date, std::unordered_map<Symbol, Price>);
	void logDividend(Date, std::vector<Price>);
	void logSalary(Date, Price);
	void updateTradeBalance(Symbol, Price);

	//===== Save Log =====
	void saveCapitalGainLog();
	void saveFixedIncomeLog();
	void setHeaderSymbolRow(std::vector<Symbol>);

private:
	//===== Data =====
	std::vector<Symbol> headerSymbolRow{};
	std::unordered_map<Symbol, Price> currentBalanceArr{};
	std::vector<MultiPriceRow> capitalGainLogTable{};
	std::vector<MultiPriceRow> dividendLogTable{};
	std::vector<PriceRow> salaryLogTable{};
	std::vector<MultiPriceRow> profitLogTable{};

	//===== Operation =====
	void mergeDividendAndSalaryLog();
};
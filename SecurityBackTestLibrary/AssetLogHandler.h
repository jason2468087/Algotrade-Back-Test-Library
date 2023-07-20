#pragma once
#include "SysType.h"

#include <vector>
#include <utility>

class AssetLogHandler
{
public:
	//===== Log Event =====
	void logTotalWorth(Date, Price);
	void logStockWorth(Date, std::vector<Price>);
	void logStockRatio(Date, std::vector<double>);

	//===== Save Log =====
	void saveTotalWorthLog();
	void saveStockWorthLog();
	void saveStockRatioLog();

	//===== Get/Set =====
	void setHeaderSymbolRow(std::vector<Symbol>);

private:
	//===== Data =====
	std::vector<Symbol> headerSymbolRow{};
	std::vector<PriceRow> totalWorthLogTable{};
	std::vector<MultiPriceRow> stockWorthLogTable{};
	std::vector<MultiFloatRow> stockRatioLogTable{};
};
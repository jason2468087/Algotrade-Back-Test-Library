#include "ProfitLogHandler.h"
#include "ErrorPacket.h"
#include "SysOperator.h"
#include "SysConst.h"
#include "Calendar.h"
#include <fstream>

///////////////////////////////////////////////////////////////////
// Log Event
///////////////////////////////////////////////////////////////////

void ProfitLogHandler::logCapitalGain(Date date, std::unordered_map<Symbol, Price> stockWorthMap)
{
	std::vector<Price> capitalGainArr{};

	for (auto pair : currentBalanceArr)
	{
		Symbol symbol{ pair.first };
		Price curBalance{ pair.second };
		if (stockWorthMap.size() == 0)
		{
			capitalGainArr.push_back(curBalance);
		}
		else
		{
			capitalGainArr.push_back(curBalance + stockWorthMap.at(symbol));
		}
	}

	MultiPriceRow row{ std::make_pair(date,capitalGainArr) };
	capitalGainLogTable.push_back(row);
}

void ProfitLogHandler::updateTradeBalance(Symbol symbol, Price tradeVolume)
{
	bool isPurchased = currentBalanceArr.find(symbol) != currentBalanceArr.end();

	if (!isPurchased)
	{
		currentBalanceArr.emplace(symbol, 0);
	}
	currentBalanceArr.at(symbol) -= tradeVolume;
}

void ProfitLogHandler::logDividend(Date date, std::vector<Price> dividendArr)
{
	MultiPriceRow row{ std::make_pair(date,dividendArr) };

	bool isFirstLog{ dividendLogTable.size() == 0 };
	if (!isFirstLog)
	{
		MultiPriceRow prevRow{ dividendLogTable.back() };

		for (int i = 0; i < dividendArr.size(); i++)
		{
			if (prevRow.second.size() == dividendArr.size())
			{
				row.second[i] += prevRow.second[i];
			}
		}
	}

	dividendLogTable.push_back(row);
}

void ProfitLogHandler::logSalary(Date date, Price salary)
{
	PriceRow row(std::make_pair(date, salary));

	bool isFirstLog{ salaryLogTable.size() == 0 };
	if (!isFirstLog)
	{
		PriceRow prevRow{ salaryLogTable.back() };
		row.second += prevRow.second;
	}

	salaryLogTable.push_back(row);
}

///////////////////////////////////////////////////////////////////
// Save/Clear/Check Log
///////////////////////////////////////////////////////////////////

void ProfitLogHandler::saveCapitalGainLog()
{
	std::ofstream logFile{ CAPITAL_GAIN_LOG_PATH };
	if (logFile.is_open())
	{
		logFile << ",";
		for (auto pair : currentBalanceArr)
		{
			logFile << pair.first << ",";
		}
		logFile << "\n";

		for (auto pair : capitalGainLogTable)
		{
			Date date{ pair.first };
			std::vector<Price> worthArr{ pair.second };

			logFile << date << ",";

			for (Price worth : worthArr)
			{
				logFile << ptos(worth) << ",";
			}
			logFile << "\n";
		}
		logFile.close();
	}
	else
	{
		throw(ErrorPacket{ ErrorCode::CANNOT_WRITE_FILE,CAPITAL_GAIN_LOG_PATH });
	}
}

void ProfitLogHandler::saveFixedIncomeLog()
{
	mergeDividendAndSalaryLog();

	std::ofstream logFile{ FIXED_INCOME_LOG_PATH };
	if (logFile.is_open())
	{
		logFile << ",";
		for (Symbol symbol : headerSymbolRow)
		{
			logFile << symbol << ",";
		}
		logFile << "\n";

		for (auto pair : dividendLogTable)
		{
			Date date{ pair.first };
			std::vector<Price> divArr{ pair.second };

			logFile << date << ",";

			for (Price worth : divArr)
			{
				logFile << ptos(worth) << ",";
			}
			logFile << "\n";
		}
		logFile.close();
	}
	else
	{
		throw(ErrorPacket{ ErrorCode::CANNOT_WRITE_FILE,FIXED_INCOME_LOG_PATH });
	}
}

///////////////////////////////////////////////////////////////////
// Operation
///////////////////////////////////////////////////////////////////

void ProfitLogHandler::mergeDividendAndSalaryLog()
{
	if (salaryLogTable.size() == 0)
	{
		return;
	}

	int salaryRowIdx{};
	Price prevSalary{};

	for (int i = 0; i < dividendLogTable.size(); i++)
	{
		Date thisDivRowDate{ dividendLogTable[i].first };
		Date thisSalaryRowDate{ salaryLogTable[salaryRowIdx].first };
		if (thisDivRowDate == thisSalaryRowDate)
		{
			Price thisSalary{ salaryLogTable[salaryRowIdx].second };
			dividendLogTable[i].second.push_back(thisSalary);

			prevSalary = thisSalary;
			if (salaryRowIdx < salaryLogTable.size() - 1)
			{
				salaryRowIdx++;
			}
		}
		else
		{
			dividendLogTable[i].second.push_back(prevSalary);
		}
	}
}

///////////////////////////////////////////////////////////////////
// Get/Set
///////////////////////////////////////////////////////////////////

void ProfitLogHandler::setHeaderSymbolRow(std::vector<Symbol> symbolArr)
{
	headerSymbolRow = symbolArr;
	headerSymbolRow.push_back("Salary");
}
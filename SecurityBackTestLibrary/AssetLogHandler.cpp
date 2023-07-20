#include "AssetLogHandler.h"
#include "ErrorPacket.h"
#include "SysConst.h"
#include "SysOperator.h"
#include <fstream>

///////////////////////////////////////////////////////////////////
// Log Event
///////////////////////////////////////////////////////////////////

void AssetLogHandler::logTotalWorth(Date date, Price totalWorth)
{
	PriceRow row{ std::make_pair(date,totalWorth) };
	totalWorthLogTable.emplace_back(date, totalWorth);
}


void AssetLogHandler::logStockWorth(Date date, std::vector<Price> stockWorthArr)
{
	MultiPriceRow row{ std::make_pair(date, stockWorthArr) };
	stockWorthLogTable.push_back(row);
}

void AssetLogHandler::logStockRatio(Date date, std::vector<double> stockRatioArr)
{
	MultiFloatRow row{ std::make_pair(date,stockRatioArr) };
	stockRatioLogTable.push_back(row);
}

///////////////////////////////////////////////////////////////////
// Save/Clear/Check Log
///////////////////////////////////////////////////////////////////

void AssetLogHandler::saveTotalWorthLog()
{
	std::ofstream logFile(TOTAL_WORTH_LOG_PATH);
	if(logFile.is_open())
	{
		for (auto pair : totalWorthLogTable)
		{
			logFile << pair.first << "," << ptos(pair.second) << "\n";
		}
		logFile.close();
	}
	else
	{
		throw(ErrorPacket{ ErrorCode::CANNOT_WRITE_FILE,TOTAL_WORTH_LOG_PATH });
	}
}

void AssetLogHandler::saveStockWorthLog()
{
	std::ofstream logFile(STOCK_WORTH_LOG_PATH);
	if (logFile.is_open())
	{
		logFile << ",";
		for (Symbol symbol : headerSymbolRow)
		{
			logFile << symbol << ",";
		}
		logFile << "\n";

		for (auto pair : stockWorthLogTable)
		{
			Date date{ pair.first };
			logFile << date << ",";

			for (Price worth : pair.second)
			{
				logFile << ptos(worth) << ",";
			}
			logFile << "\n";
		}
		logFile.close();
	}
	else
	{
		throw(ErrorPacket{ ErrorCode::CANNOT_WRITE_FILE,STOCK_WORTH_LOG_PATH });
	}
}

void AssetLogHandler::saveStockRatioLog()
{
	std::ofstream logFile(STOCK_RATIO_LOG_PATH);
	if (logFile.is_open())
	{
		logFile << ",";
		for (Symbol symbol : headerSymbolRow)
		{
			logFile << symbol << ",";
		}
		logFile << "\n";

		for (auto pair : stockRatioLogTable)
		{
			logFile << pair.first << ",";

			for (double ratio : pair.second)
			{
				logFile << ratio << ",";
			}
			logFile << "\n";
		}
		logFile.close();
	}
	else
	{
		throw(ErrorPacket{ ErrorCode::CANNOT_WRITE_FILE,STOCK_RATIO_LOG_PATH });
	}
}

///////////////////////////////////////////////////////////////////
// Get/Set
///////////////////////////////////////////////////////////////////

void AssetLogHandler::setHeaderSymbolRow(std::vector<Symbol> symbolArr)
{
	headerSymbolRow = symbolArr;
	headerSymbolRow.push_back("Cash");
}
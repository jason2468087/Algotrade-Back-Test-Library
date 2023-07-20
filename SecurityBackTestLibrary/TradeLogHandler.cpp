#include "TradeLogHandler.h"
#include "SysOperator.h"
#include "SysConst.h"
#include "ErrorPacket.h"
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>

TradeLogHandler::TradeLogHandler()
{

}

///////////////////////////////////////////////////////////////////
// Save/Clear/Check Log
///////////////////////////////////////////////////////////////////

void TradeLogHandler::saveLog()
{
	std::ofstream logFile{ TRADE_LOG_PATH };
	if (logFile.is_open())
	{
		logFile << logStream.str();
		logFile.close();
	}
	else
	{
		throw(ErrorPacket{ ErrorCode::CANNOT_WRITE_FILE,TRADE_LOG_PATH });
	}
}

void TradeLogHandler::clearLog()
{
	logStream.str("");
	logTable.clear();
}

bool TradeLogHandler::checkLogExist(Date date, LogType type, int amount, Symbol sym, Price price)
{
	bool isMatch{ false };
	for (size_t i = 0; i < logTable.size(); i++)
	{
		isMatch = logTable.at(i) == std::make_tuple(date, type, amount, sym, price);
		if (isMatch)
		{
			break;
		}
	}
	return isMatch;
}

///////////////////////////////////////////////////////////////////
// Log Event
///////////////////////////////////////////////////////////////////

void TradeLogHandler::logBuy(Date d, Symbol sym, Price price, int amount)
{
	TradeRow newLine = std::make_tuple(d, LogType::BUY, amount, sym, price);
	logTable.push_back(newLine);

	logStream << d << " " << "Buy " << amount << " " << sym << " in " << ptos(price) << "\n";
}

void TradeLogHandler::logSell(Date d, Symbol sym, Price price, int amount)
{
	TradeRow newLine = std::make_tuple(d, LogType::SELL, amount, sym, price);
	logTable.push_back(newLine);

	logStream << d << " " << "Sell " << amount << " " << sym << " in " << ptos(price) << "\n";
}

void TradeLogHandler::logDividend(Date d, Symbol sym, Price dividend)
{
	TradeRow newLine = std::make_tuple(d, LogType::DIVIDEND, 0, sym, dividend);
	logTable.push_back(newLine);

	logStream << d << " " << "Dividend " << " " << sym << " of " << ptos(dividend) << "\n";
}

void TradeLogHandler::logSalary(Date d, Price salary)
{
	TradeRow newLine = std::make_tuple(d, LogType::SALARY, 0, std::string{}, salary);
	logTable.push_back(newLine);

	logStream << d << " " << "Salary " << ptos(salary) << "\n";
}

void TradeLogHandler::logMessage(Date d, std::string message)
{
	TradeRow newLine = std::make_tuple(d, LogType::MESSAGE, 0, message, 0);
	logTable.push_back(newLine);

	logStream << d << " " << message << "\n";
}

///////////////////////////////////////////////////////////////////
// Get/Set
///////////////////////////////////////////////////////////////////

TradeLogTable TradeLogHandler::getLogTable()
{
	return logTable;
}

std::string TradeLogHandler::getContentString()
{
	return logStream.str();
}

///////////////////////////////////////////////////////////////////
// Print Log
///////////////////////////////////////////////////////////////////

void TradeLogHandler::printLogLine(int lineNo)
{
	auto line{ logTable.at(lineNo) };

	std::cout << std::get<0>(line) << " ";
	std::cout << printLogType(std::get<1>(line)) << " ";
	std::cout << std::get<2>(line) << " ";
	std::cout << std::get<3>(line) << " ";
	std::cout << ptos(std::get<4>(line)) << "\n";
}

void TradeLogHandler::printLog(int tap)
{
	for (size_t i = 0; i < logTable.size(); i++)
	{
		for (int t = 0; t < tap; t++)
		{
			std::cout << "\t";
		}
		printLogLine(static_cast<int>(i));
	}
}

std::string TradeLogHandler::printLogType(LogType type)
{
	switch (type)
	{
	case LogType::BUY:
		return "BUY";
	case LogType::SELL:
		return "SELL";
	case LogType::DIVIDEND:
		return "DIVIDEND";
	case LogType::SALARY:
		return "SALARY";
	case LogType::MESSAGE:
		return "MESSAGE";
	}
	return "NAN";
}
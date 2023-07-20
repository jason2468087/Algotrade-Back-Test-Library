#include "ErrorPacket.h"
#include "SysOperator.h"


///////////////////////////////////////////////////////////////////
// Construction
///////////////////////////////////////////////////////////////////

ErrorPacket::ErrorPacket(ErrorCode code, Date date)
	:errorCode(code)
{
	if (errorCode == ErrorCode::INVALID_DATE)
	{
		errDate = date;
		errorStream << errDate << " is an invalid date!";
		errorString = errorStream.str();
		errorStream.clear();
	}
	else if (errorCode == ErrorCode::DATE_OUT_OF_RECORD)
	{
		errDate = date;
		errorStream << errDate << " is out of database record!";
		errorString = errorStream.str();
		errorStream.clear();
	}
}

ErrorPacket::ErrorPacket(ErrorCode code, int number)
	:errorCode(code)
{
	if (errorCode == ErrorCode::INVALID_RAWDATE)
	{
		errRawDate = number;
		errorStream << errRawDate << " is an invalid raw date!";
		errorString = errorStream.str();
		errorStream.clear();
	}
	else if (errorCode == ErrorCode::NEGATIVE_SALARY)
	{
		errPrice = number;
		errorStream << "Receive Negative Salary "<<ptos(errPrice)<<"!";
		errorString = errorStream.str();
		errorStream.clear();
	}
}

ErrorPacket::ErrorPacket(ErrorCode code, int number, std::string str)
	:errorCode(code)
{
	if (errorCode == ErrorCode::NOT_ENOUGH_CASH)
	{
		errSymbol = str;
		errPrice = number;
		errorStream << "Not Enough Cash To Buy " << errPrice << " " << errSymbol << "!";
		errorString = errorStream.str();
		errorStream.clear();
	}
	else if (errorCode == ErrorCode::SELL_AMOUNT_EXCEEDED)
	{
		errSymbol = str;
		errAmount = number;
		errorStream << "Try To Sell " << errAmount<<" "<< errSymbol << " Which Exceed The Amount Owned In Portfolio!";
		errorString = errorStream.str();
		errorStream.clear();
	}
	else if (errorCode == ErrorCode::NEGATIVE_DIVIDEND)
	{
		errSymbol = str;
		errPrice = number;
		errorStream << "Receive Negative Dividend " << ptos(errPrice) << " From " << errSymbol << "!";
		errorString = errorStream.str();
		errorStream.clear();
	}
}

ErrorPacket::ErrorPacket(ErrorCode code, std::string str)
	:errorCode(code)
{
	if (errorCode == ErrorCode::CANNOT_WRITE_FILE)
	{
		errPath = str;
		errorStream << "Can Not Write Log To Path " << errPath << "!";
		errorString = errorStream.str();
		errorStream.clear();
	}
	else if (errorCode == ErrorCode::SELL_UNOWNED_STOCK)
	{
		errSymbol = str;
		errorStream << "Try To Sell " << errSymbol << " Which Is Not Owned In Portfolio!";
		errorString = errorStream.str();
		errorStream.clear();
	}
}

ErrorPacket::ErrorPacket(ErrorCode code, std::string str1, std::string str2)
	:errorCode(code)
{
	if (errorCode == ErrorCode::FILE_NOT_EXIST)
	{
		errPath = str1;
		errSymbol = str2;
		errorStream << "Can Not Find " << errSymbol<<" File From Path " << errPath<<"!";
		errorString = errorStream.str();
		errorStream.clear();
	}
}

ErrorPacket::ErrorPacket(const ErrorPacket& src)
	:errorCode(src.errorCode), errRawDate{src.errRawDate}, errSymbol{src.errSymbol}
	,errPath{src.errPath},errPrice{src.errPrice},errAmount{src.errAmount}
	,errorString{src.errorString}
{
	if (errorCode == ErrorCode::CANNOT_WRITE_FILE)
	{
		errDate.d = src.errDate.d;
		errDate.m = src.errDate.m;
		errDate.y = src.errDate.y;
	}
}

///////////////////////////////////////////////////////////////////
// Get/Set
///////////////////////////////////////////////////////////////////

std::string ErrorPacket::getErrorString()
{
	return errorString;
}
#pragma once
#include <sstream>
#include "SysConst.h"

class ErrorPacket
{
public:
	//===== Constructors =====
	ErrorPacket(ErrorCode code, Date date);
	ErrorPacket(ErrorCode code, int number);
	ErrorPacket(ErrorCode code, int number, std::string str);
	ErrorPacket(ErrorCode code, std::string str);
	ErrorPacket(ErrorCode code, std::string str1, std::string str2);

	ErrorPacket(const ErrorPacket&);

	//===== Get/Set =====
	std::string getErrorString();

private:
	//===== Data =====
	ErrorCode errorCode{ ErrorCode::NONE };
	Date errDate{};
	RawDate errRawDate{};
	Symbol errSymbol{};
	Path errPath{};
	Price errPrice{};
	int errAmount{};

	//===== Error Message =====
	std::string errorString{};
	std::ostringstream errorStream;
};
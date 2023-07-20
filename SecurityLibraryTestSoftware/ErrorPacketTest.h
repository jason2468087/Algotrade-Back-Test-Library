#pragma once
#include "ControlPanel.h"

namespace ErrorPacketTest
{
	bool test();

	bool testErrDate();
	bool testErrInt();
	bool testErrIntNStr();
	bool testErrStr();
	bool testErrStrNStr();

	bool tryCatchException(ErrorPacket errorPacket, std::string ans);
}
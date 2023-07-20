// SecurityLibraryTestSoftware.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include "AssetLogTest.h"
#include "BrokerTest.h"
#include "ControlPanelTest.h"
#include "CalendarTest.h"
#include "DataReaderTest.h"
#include "ErrorPacketTest.h"
#include "IndexTest.h"
#include "MarketTest.h"
#include "PortfolioTest.h"
#include "ProfitLogTest.h"
#include "ShareTest.h"
#include "TradeLogTest.h"
#include <iostream>

int main()
{
	bool isPass{ true };

	try 
	{
		isPass &= ErrorPacketTest::test();
		isPass &= DataReaderTest::test();
		isPass &= CalendarTest::test();
		isPass &= ShareTest::test();
		isPass &= MarketTest::test();
		isPass &= PortfolioTest::test();
		isPass &= BrokerTest::test();
		isPass &= IndexTest::test();
		isPass &= AssetLogTest::test();
		isPass &= ProfitLogTest::test();
		isPass &= TradeLogTest::test();
		isPass &= ControlPanelTest::test();
	}
	catch(ErrorPacket& ep)
	{
		std::cerr << "\nError!!!" << ep.getErrorString() << "\n";
		isPass = false;
	}

	if (isPass)
	{
		std::cout << R"(///////////////////////////////////////////////////////////////////)" << std::endl;
		std::cout << R"(//)" << std::endl;
		std::cout << R"(// PASS!!!)" << std::endl;
		std::cout << R"(//)" << std::endl;
		std::cout << R"(///////////////////////////////////////////////////////////////////)";
	}
	else
	{
		std::cout << R"(///////////////////////////////////////////////////////////////////)" << std::endl;
		std::cout << R"(//)" << std::endl;
		std::cout << R"(// FAIL!!!)" << std::endl;
		std::cout << R"(//)" << std::endl;
		std::cout << R"(///////////////////////////////////////////////////////////////////)";
	}

	std::cin.get();
	return 0;
}

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案

#include "DataReaderTest.h"
#include "ControlPanel.h"
#include "DataReader.h"

///////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////

bool DataReaderTest::test()
{
	std::cout << "========Test Data Reader Class=======\n";
	bool isPass{ true };

	isPass &= testReadPrice();
	isPass &= testReadDividend();
	isPass &= testTradeDay();


	if (isPass)
		std::cout << "--<Pass>--\n\n";
	else
		std::cout << "--<Fail>--\n\n";

	return isPass;
}

///////////////////////////////////////////////////////////////////
// Read Price File
///////////////////////////////////////////////////////////////////

bool DataReaderTest::testReadPrice()
{
	std::cout << "[Test Reading Price Data]\n";
	bool isPass{ true };

	isPass &= testReadStandardPrice();
	isPass &= testReadNullPrice();

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool DataReaderTest::testReadStandardPrice()
{
	bool isPass{ true };
	DataTable dataTable{};
	DataReader::readPriceData(dataTable, "SPY");

	isPass = dataTable.size() == 11100 && dataTable[0][OPEN] == 43.97 && dataTable[1000][CLOSE] == 57.75;
	
	std::cout << "\tSPY: Length = " << dataTable.size() << "(11100)\n"
		<< "\t     First Open = " << dataTable[0][OPEN] << "(43.97)\n"
		<< "\t     Thousand Data = " << dataTable[1000][CLOSE] << "(57.75)\n";

	return isPass;
}

bool DataReaderTest::testReadNullPrice()
{
	bool isPass{ true };
	DataTable dataTable{};
	DataReader::readPriceData(dataTable, "CKH");

	isPass = dataTable.size() == 8110 && dataTable[0][OPEN] == 71.47 && dataTable[1000][CLOSE] == 35.24;

	std::cout << "\tCKH: Length = " << dataTable.size() << "(8110)\n"
		<< "\t     First Open = " << dataTable[0][OPEN] << "(71.47)\n"
		<< "\t     Thousand Data = " << dataTable[1000][CLOSE] << "(35.24)\n";

	return isPass;
}

bool DataReaderTest::testReadDividend()
{
	std::cout << "[Test Reading Dividend Data]\n";
	bool isPass{ true };
	DataTable dataTable{};
	DataReader::readPriceData(dataTable, "BND");
	DataReader::readDividendData(dataTable, "BND");

	isPass = dataTable[4223][DIVIDEND] == 0.194 && dataTable[4224][DIVIDEND] == 0 && dataTable[357][DIVIDEND] == 0.301;

	std::cout << "\tBND dividend at 1/11/2008: " << dataTable[4223][DIVIDEND] << "(0.194)\n";
	std::cout << "\tBND dividend at 2/11/2008: " << dataTable[4224][DIVIDEND] << "(0)\n";
	std::cout << "\tBND dividend at 1/4/2008: " << dataTable[357][DIVIDEND] << "(0.301)\n";

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool DataReaderTest::testTradeDay()
{
	std::cout << "[Test Trade Day Handling]\n";
	bool isPass{ true };
	DataTable dataTable{};
	DataReader::readPriceData(dataTable, "VOO");

	isPass =
		dataTable[1][TRADE_DAY] == 1 && dataTable[1][CLOSE] == 101.78 &&
		dataTable[2][TRADE_DAY] == 0 && dataTable[2][CLOSE] == 101.78 &&
		dataTable[3][TRADE_DAY] == 0 && dataTable[3][CLOSE] == 101.78 &&
		dataTable[4][TRADE_DAY] == 1 && dataTable[4][CLOSE] == 103.06;

	std::cout << "\tIs Trade Day:" << dataTable[1][TRADE_DAY] << "(1) Close Price:" << dataTable[1][CLOSE] << "(101.78)" << "\n";
	std::cout << "\tIs Trade Day:" << dataTable[2][TRADE_DAY] << "(0) Close Price:" << dataTable[2][CLOSE] << "(101.78)" << "\n";
	std::cout << "\tIs Trade Day:" << dataTable[3][TRADE_DAY] << "(0) Close Price:" << dataTable[3][CLOSE] << "(101.78)" << "\n";
	std::cout << "\tIs Trade Day:" << dataTable[4][TRADE_DAY] << "(1) Close Price:" << dataTable[4][CLOSE] << "(103.06)" << "\n";
	
	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}
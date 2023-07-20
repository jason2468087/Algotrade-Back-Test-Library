#include "ErrorPacketTest.h"
#include "ErrorPacket.h"

bool ErrorPacketTest::test()
{
	std::cout << "========Test Error Packet Class=======\n";
	bool isPass{ true };

	isPass &= testErrDate();
	isPass &= testErrInt();
	isPass &= testErrIntNStr();
	isPass &= testErrStr();
	isPass &= testErrStrNStr();


	if (isPass)
		std::cout << "--<Pass>--\n\n";
	else
		std::cout << "--<Fail>--\n\n";

	return isPass;
}

bool ErrorPacketTest::testErrDate()
{
	std::cout << "[Test Throw Invalid Date]\n";
	bool isPass{ false };

	bool isInvalidDatePass = tryCatchException(
		ErrorPacket{ ErrorCode::INVALID_DATE,Date{-1,13,1897} }, std::string{ "-1/13/1897 is an invalid date!" });
	
	bool isOutOfRecordDatePass = tryCatchException(
		ErrorPacket{ ErrorCode::DATE_OUT_OF_RECORD,Date{24,3,1981} }, std::string{ "24/3/1981 is out of database record!" });

	isPass = isInvalidDatePass && isOutOfRecordDatePass;

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool ErrorPacketTest::testErrInt()
{
	std::cout << "[Test Throw Invalid Interger]\n";
	bool isPass{ false };

	bool isInvalidRawDatePass = tryCatchException(
		ErrorPacket{ ErrorCode::INVALID_RAWDATE,-500 }, std::string{ "-500 is an invalid raw date!" });

	bool isNegativeSalaryPass = tryCatchException(
		ErrorPacket{ ErrorCode::NEGATIVE_SALARY,-120000 }, std::string{ "Receive Negative Salary -$1200.00!" });

	isPass = isInvalidRawDatePass && isNegativeSalaryPass;

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool ErrorPacketTest::testErrIntNStr()
{
	std::cout << "[Test Throw Invalid Interger and String]\n";
	bool isPass{ false };

	bool isNotEnoughCashPass = tryCatchException(
		ErrorPacket{ ErrorCode::NOT_ENOUGH_CASH,30,"SPY"}, std::string{"Not Enough Cash To Buy 30 SPY!"});

	bool isSellAmountExceededPass = tryCatchException(
		ErrorPacket{ ErrorCode::SELL_AMOUNT_EXCEEDED,85,"QQQ"}, std::string{"Try To Sell 85 QQQ Which Exceed The Amount Owned In Portfolio!"});
	
	bool isNegativeDividendPass = tryCatchException(
		ErrorPacket{ ErrorCode::NEGATIVE_DIVIDEND,-562,"HSBC" }, std::string{ "Receive Negative Dividend -$5.62 From HSBC!" });

	isPass = isNotEnoughCashPass && isSellAmountExceededPass && isNegativeDividendPass;

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool ErrorPacketTest::testErrStr()
{
	std::cout << "[Test Throw Invalid String]\n";
	bool isPass{ false };

	bool isConnotWriteFilePass = tryCatchException(
		ErrorPacket{ ErrorCode::CANNOT_WRITE_FILE,"../SecurityBackTestLibrary/Historial Data/Price/MSFT.csv"}
	, std::string{"Can Not Write Log To Path ../SecurityBackTestLibrary/Historial Data/Price/MSFT.csv!"});

	bool isSellUnownedStockPass = tryCatchException(
		ErrorPacket{ ErrorCode::SELL_UNOWNED_STOCK,"AIA" }, std::string{ "Try To Sell AIA Which Is Not Owned In Portfolio!" });

	isPass = isConnotWriteFilePass && isSellUnownedStockPass;

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool ErrorPacketTest::testErrStrNStr()
{
	std::cout << "[Test Throw Invalid String and String]\n";
	bool isPass{ false };

	bool isFileNotExistPass = tryCatchException(
		ErrorPacket{ ErrorCode::FILE_NOT_EXIST,"../SecurityBackTestLibrary/Historial Data/Price/AIA.csv","AIA"}
	, std::string{ "Can Not Find AIA File From Path ../SecurityBackTestLibrary/Historial Data/Price/AIA.csv!" });

	isPass = isFileNotExistPass;

	if (isPass)
		std::cout << "\t<Pass>\n";
	else
		std::cout << "\t<Fail>\n";

	return isPass;
}

bool ErrorPacketTest::tryCatchException(ErrorPacket errorPacket, std::string ans)
{
	bool isPass{ false };

	try
	{
		throw(errorPacket);
	}
	catch (ErrorPacket& ep)
	{
		std::cout << "\tAns: " << ans << "\n";
		std::cerr << "\tErr: " << ep.getErrorString() << "\n";
		isPass = ep.getErrorString() == ans;
	}

	return isPass;
}
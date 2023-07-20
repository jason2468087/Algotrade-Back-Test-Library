#pragma once
#include "SysType.h"
#include "Calendar.h"

class DataReader {
public:
	static void readPriceData(DataTable& histPrice, Symbol sym);
	static void readDividendData(DataTable& histPrice, Symbol sym);

private:
	//===== Read Price =====
	static void readPriceDataLine(std::string& currentLine, DataTable& histPrice);
	static RawDate readDataLineDate(std::stringstream& lineStream);
	static void skipTableRow(DataTable& histPrice, RawDate thisDate);
	static void writePriceDataRow(DataTable& histPrice, std::stringstream& lineStream, RawDate thisDate);

	//===== Read Dividend =====
	static void readDividendDataLine(std::string& currentLine, DataTable& histPrice);
	static void createDividendColumn(DataTable& histPrice);
};
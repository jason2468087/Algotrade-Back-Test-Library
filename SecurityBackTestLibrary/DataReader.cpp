#include "DataReader.h"
#include "SysConst.h"
#include "ErrorPacket.h"

#include <iostream>
#include <fstream>
#include <sstream>


///////////////////////////////////////////////////////////////////
// Read Price
///////////////////////////////////////////////////////////////////
void DataReader::readPriceData(DataTable& histPrice, Symbol sym)
{
	Path path{ HIST_PRICE_PATH + sym + ".csv" };

	std::ifstream dataFile;
	dataFile.open(path);

	if (dataFile.is_open())
	{
		std::string line;

		std::getline(dataFile, line); // Skip first line (Header line)
		while (std::getline(dataFile, line))
		{
			readPriceDataLine(line, histPrice);
		}
		dataFile.close();
	}
	else
	{
		throw(ErrorPacket{ ErrorCode::FILE_NOT_EXIST,path,sym });
	}
}

void DataReader::readPriceDataLine(std::string& currentLine, DataTable& histPrice)
{
	std::stringstream lineStream(currentLine);

	RawDate thisDate = readDataLineDate(lineStream);
	skipTableRow(histPrice, thisDate);

	writePriceDataRow(histPrice, lineStream, thisDate);
}

RawDate DataReader::readDataLineDate(std::stringstream& lineStream)
{
	std::string cell("");
	Date date{};

	std::getline(lineStream, cell, '-');
	date.y = std::stoi(cell);
	std::getline(lineStream, cell, '-');
	date.m = std::stoi(cell);
	std::getline(lineStream, cell, ',');
	date.d = std::stoi(cell);
	return RawDate{ Calendar::convertToRawDate(date) };
}

void DataReader::skipTableRow(DataTable& histPrice, RawDate thisDate)
{
	if (histPrice.size() == 0)
	{
		return;
	}

	int dayToSkip{ thisDate - static_cast<int>(histPrice.back()[0]) - 1 };
	for (int i = 0; i < dayToSkip; i++)
	{
		DataRow copyRow{ histPrice.back() };
		copyRow[DATE]++;
		copyRow[TRADE_DAY] = 0;
		copyRow[VOLUMN] = 0;
		histPrice.push_back(copyRow);
	}
}

void DataReader::writePriceDataRow(DataTable& histPrice, std::stringstream& lineStream, RawDate thisDate)
{
	std::string cell("");
	DataRow dataRow{};

	dataRow.push_back(thisDate);
	dataRow.push_back(1.0);
	while (std::getline(lineStream, cell, ','))
	{
		if (cell != "null")
		{
			double dataValue = std::stod(cell);
			double roudValue = std::round(dataValue * 100.0) / 100.0;
			dataRow.push_back(roudValue);
		}
		else
		{
			dataRow = histPrice.back();
			dataRow[DATE]++;
			dataRow[TRADE_DAY] = 0;
			dataRow[VOLUMN] = 0;
			break;
		}
	}
	histPrice.push_back(dataRow);
}

///////////////////////////////////////////////////////////////////
// Read Dividend
///////////////////////////////////////////////////////////////////
void DataReader::readDividendData(DataTable& histPrice, Symbol sym)
{
	createDividendColumn(histPrice);

	Path path{ HIST_DIV_PATH + sym + ".csv" };

	std::ifstream dataFile;
	dataFile.open(path);

	if (dataFile.is_open())
	{
		std::string line;

		std::getline(dataFile, line); // Skip first line (Header line)
		while (std::getline(dataFile, line))
		{
			readDividendDataLine(line, histPrice);
		}
		dataFile.close();
	}
	else
	{
		throw(ErrorPacket{ ErrorCode::FILE_NOT_EXIST,path,sym });
	}
}

void DataReader::createDividendColumn(DataTable& histPrice)
{
	RawDate startDate = histPrice[0][0];

	for (size_t r = 0; r < histPrice.size(); r++)
	{
		histPrice[r].push_back(0.0);
	}
}

void DataReader::readDividendDataLine(std::string& currentLine, DataTable& histPrice)
{
	std::stringstream lineStream(currentLine);

	RawDate thisDate = readDataLineDate(lineStream);
	int thisRow = thisDate - histPrice[0][0];

	std::string dividend{ "" };
	std::getline(lineStream, dividend, '-');

	double dataValue = std::stod(dividend);
	histPrice[thisRow][DIVIDEND] = dataValue;
}
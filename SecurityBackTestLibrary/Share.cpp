#include "Share.h"
#include "DataReader.h"
#include "SysConst.h"
#include "ErrorPacket.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

///////////////////////////////////////////////////////////////////
// Construction
///////////////////////////////////////////////////////////////////

Share::Share(Symbol sym)
	:symbol{ sym }
{
	importHistoricalData();
	verifyDataTableSize();
	generateLastDividend();
}

void Share::importHistoricalData()
{
	DataReader::readPriceData(historicalData, symbol);
	DataReader::readDividendData(historicalData, symbol);
	startDate = static_cast<int> (historicalData[0][DATE]);
	endDate = static_cast<int>(historicalData.back()[DATE]);
}

void Share::verifyDataTableSize()
{
	unsigned int correctRowSize = DIVIDEND + 1;
	for (auto& row : historicalData)
	{
		while (row.size() < correctRowSize)
		{
			row.push_back(0.0);
		}
	}
}

void Share::generateLastDividend()
{
	double lastDividend{};

	for (auto& row : historicalData)
	{
		if (row[DIVIDEND] != 0)
		{
			lastDividend = row[DIVIDEND];
		}
		row.push_back(lastDividend);
	}
}

///////////////////////////////////////////////////////////////////
// Access Data Table
///////////////////////////////////////////////////////////////////

DataRow& Share::getDataRowRef(RawDate rDate)
{
	if (rDate<startDate || rDate>endDate)
	{
		throw(ErrorPacket{ ErrorCode::DATE_OUT_OF_RECORD,Calendar::convertToDate(rDate) });
	}

	int row{ rDate - startDate };
	return historicalData[row];
}

///////////////////////////////////////////////////////////////////
// Get/Set (Date)
///////////////////////////////////////////////////////////////////

RawDate Share::getStartDate()
{
	return startDate;
}

RawDate Share::getEndDate()
{
	return endDate;
}

///////////////////////////////////////////////////////////////////
// Get/Set (Data Element)
///////////////////////////////////////////////////////////////////

Price Share::getOpen(RawDate rDate)
{
	DataRow& dataRow{ getDataRowRef(rDate) };
	return dtop(dataRow[OPEN]);
}

Price Share::getHigh(RawDate rDate)
{
	DataRow& dataRow{ getDataRowRef(rDate) };
	return dtop(dataRow[HIGH]);
}

Price Share::getLow(RawDate rDate)
{
	DataRow& dataRow{ getDataRowRef(rDate) };
	return dtop(dataRow[LOW]);
}

Price Share::getClose(RawDate rDate)
{
	DataRow& dataRow{ getDataRowRef(rDate) };
	return dtop(dataRow[CLOSE]);
}

double Share::getDividend(RawDate rDate)
{
	DataRow& dataRow{ getDataRowRef(rDate) };
	return dataRow[DIVIDEND];
}

Price Share::getVolumn(RawDate rDate)
{
	DataRow& dataRow{ getDataRowRef(rDate) };
	return dtop(dataRow[VOLUMN]);
}

bool Share::getIsTradedToday(RawDate rDate)
{
	DataRow& dataRow{ getDataRowRef(rDate) };
	return dtop(dataRow[TRADE_DAY]);
}

double Share::getLastDividend(RawDate rDate)
{
	DataRow& dataRow{ getDataRowRef(rDate) };
	return dtop(dataRow[LAST_DIVIDEND]);
}

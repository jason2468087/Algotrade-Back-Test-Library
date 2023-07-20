#pragma once

#include "SysType.h"
#include <vector>

class Share
{
public:
	Share(Symbol symbol);

	//===== Get/Set (Data Element) =====
	Price getOpen(RawDate rDate);
	Price getClose(RawDate rDate);
	Price getHigh(RawDate rDate);
	Price getLow(RawDate rDate);
	double getDividend(RawDate rDate);
	Price getVolumn(RawDate rDate);
	bool getIsTradedToday(RawDate rDate);
	double getLastDividend(RawDate rDate);

	//===== Get/Set (Date) =====
	RawDate getStartDate();
	RawDate getEndDate();

private:
	//===== Data =====
	Symbol symbol{};
	RawDate startDate{};
	RawDate endDate{};

	DataTable historicalData{};

	//===== Construction =====
	void importHistoricalData();
	void verifyDataTableSize();
	void generateLastDividend();

	//===== Access Data Table =====
	DataRow& getDataRowRef(RawDate date);
	void verifyAccessDate(RawDate rDate);
};
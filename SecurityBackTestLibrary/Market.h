#pragma once
#include "Share.h"

class Market
{
public:
	//===== Data Updating =====
	void observe(Symbol sym);
	void update(Date date);
	void update(RawDate rDate);

	//===== Get/Set (Today) =====
	void setIsMarketOpen(bool isOpen);
	bool getIsMarketOpen();
	Price getPrice(Symbol sym);
	Price getTodayOpen(Symbol sym);
	Price getTodayClose(Symbol sym);
	double getTodayDividend(Symbol sym);
	double getTodayDividendYield(Symbol sym);

	//===== Get/Set (Historical) =====
	Price getOpen(Symbol sym, Date date);
	Price getHigh(Symbol sym, Date date);
	Price getLow(Symbol sym, Date);
	Price getClose(Symbol sym, Date);
	Price getVolumn(Symbol sym, Date);
	double getDividend(Symbol sym, Date);
	double getLastDividend(Symbol sym, Date);
	bool getIsTraded(Symbol sym, Date);
	Date getStartDate(Symbol sym);

	//===== Get/Set (Raw Historical) =====
	Price getClose(Symbol sym, RawDate date);
	bool getIsTraded(Symbol sym, RawDate date);
	RawDate getRawStartDate(Symbol sym);

private:
	//===== Data =====
	std::unordered_map<Symbol, Share> shareMarketData{};
	RawDate rawToday{};
	Date today{};
	bool isMarketOpen{ true };
};
#include "Market.h"
#include "Calendar.h"
#include "SysOperator.h"
#include "SysConst.h"

///////////////////////////////////////////////////////////////////
// Data Updating
///////////////////////////////////////////////////////////////////

void Market::observe(Symbol symbol)
{
	bool isSymbolAbsent = shareMarketData.find(symbol) == shareMarketData.end();
	if (isSymbolAbsent)
	{
		shareMarketData.emplace(symbol, Share{ symbol });
	}
}

void Market::update(Date date)
{
	today = date;
	rawToday = Calendar::convertToRawDate(date);
}

void Market::update(RawDate rDate)
{
	rawToday = rDate;
	today = Calendar::convertToDate(rDate);
}

///////////////////////////////////////////////////////////////////
// Get/Set (Today)
///////////////////////////////////////////////////////////////////

void Market::setIsMarketOpen(bool isOpen)
{
	isMarketOpen = isOpen;
}

bool Market::getIsMarketOpen()
{
	return isMarketOpen;
}

Price Market::getPrice(Symbol sym)
{
	if (isMarketOpen)
	{
		return getOpen(sym, Calendar::convertToDate(rawToday));
	}
	else
	{
		return getClose(sym, Calendar::convertToDate(rawToday));
	}
}

Price Market::getTodayOpen(Symbol sym)
{
	return getOpen(sym, Calendar::convertToDate(rawToday));
}

Price Market::getTodayClose(Symbol sym)
{
	return getClose(sym, Calendar::convertToDate(rawToday));
}

double Market::getTodayDividend(Symbol sym)
{
	return getDividend(sym, Calendar::convertToDate(rawToday));
}

double Market::getTodayDividendYield(Symbol sym)
{
	Date today{ Calendar::convertToDate(rawToday) };
	return getLastDividend(sym, Calendar::convertToDate(rawToday));
}

///////////////////////////////////////////////////////////////////
// Get/Set (Historical)
///////////////////////////////////////////////////////////////////

Price Market::getOpen(Symbol sym, Date date)
{
	RawDate rDate{ Calendar::convertToRawDate(date) };
	return shareMarketData.at(sym).getOpen(rDate);
}

Price Market::getHigh(Symbol sym, Date date)
{
	RawDate rDate{ Calendar::convertToRawDate(date) };
	return shareMarketData.at(sym).getHigh(rDate);
}

Price Market::getLow(Symbol sym, Date date)
{
	RawDate rDate{ Calendar::convertToRawDate(date) };
	return shareMarketData.at(sym).getLow(rDate);
}

Price Market::getClose(Symbol sym, Date date)
{
	RawDate rDate{ Calendar::convertToRawDate(date) };
	return shareMarketData.at(sym).getClose(rDate);
}

Price Market::getVolumn(Symbol sym, Date date)
{
	RawDate rDate{ Calendar::convertToRawDate(date) };
	return shareMarketData.at(sym).getVolumn(rDate);
}

double Market::getDividend(Symbol sym, Date date)
{
	RawDate rDate{ Calendar::convertToRawDate(date) };
	return shareMarketData.at(sym).getDividend(rDate);
}

double Market::getLastDividend(Symbol sym, Date date)
{
	RawDate rDate{ Calendar::convertToRawDate(date) };
	return shareMarketData.at(sym).getLastDividend(rDate);
}

bool Market::getIsTraded(Symbol sym, Date date)
{
	RawDate rDate{ Calendar::convertToRawDate(date) };
	return shareMarketData.at(sym).getIsTradedToday(rDate);
}

Date Market::getStartDate(Symbol sym)
{
	return Calendar::convertToDate(getRawStartDate(sym));
}

///////////////////////////////////////////////////////////////////
// Get/Set (Historical Raw)
///////////////////////////////////////////////////////////////////

RawDate Market::getRawStartDate(Symbol sym)
{
	return shareMarketData.at(sym).getStartDate();
}

RawDate Market::getClose(Symbol sym, RawDate rDate)
{
	return shareMarketData.at(sym).getClose(rDate);
}

bool Market::getIsTraded(Symbol sym, RawDate rDate)
{
	return shareMarketData.at(sym).getIsTradedToday(rDate);
}
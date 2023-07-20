#pragma once
#include "SysType.h"

class Index
{
public:
	Index(Market& marketRef);
	Price movingAverage(Date today, int duration, Symbol symbol);
	double relativeStrength(Date today, int duration, Symbol symbol);

	void setMarketReference(Market& mkt);

private:
	Market& marketRef;

	double prevRSGain{NULL};
	double prevRSLoss{NULL };
};
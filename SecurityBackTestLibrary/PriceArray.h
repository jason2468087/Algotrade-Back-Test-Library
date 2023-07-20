#pragma once

#include "SysType.h"
#include "Calendar.h"

class PriceArray
{
public:
	PriceArray(RawDate);
	PriceArray(Date);

	Price at(RawDate rDate);
	Price at(Date date);

private:
	RawDate start{};
	std::vector<Price> arr{};
};
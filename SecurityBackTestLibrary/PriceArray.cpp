#include "PriceArray.h"

PriceArray::PriceArray(RawDate rDate)
	:start{rDate}
{

}

PriceArray::PriceArray(Date date)
	: start{ Calendar::convertToRawDate(date) }
{

}

Price PriceArray::at(RawDate rDate)
{
	int idx{ rDate - start };
	return arr[idx];
}

Price PriceArray::at(Date date)
{
	int idx{ Calendar::convertToRawDate(date) - start };
	return arr[idx];
}
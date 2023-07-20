#include "Broker.h"
#include "SysOperator.h"

///////////////////////////////////////////////////////////////////
// Construction
///////////////////////////////////////////////////////////////////

Broker::Broker(BrokerName broker)
	: brokerName{ broker }
{
	switch (brokerName)
	{ 
		case BrokerName::NONE:
			monthlyFee = 0;
			yearlyFee = 0;
			break;
		case BrokerName::HSBC:
			monthlyFee = 0;
			yearlyFee = 0;
	}
}

///////////////////////////////////////////////////////////////////
// Construction
///////////////////////////////////////////////////////////////////
Price Broker::calcTradeFee_HSBC(Price price, int amount)
{
	if (amount > 1000)
	{
		return static_cast<int> (round(1800 + (amount - 1000) * 1.5));
	}
	else
	{
		return 18.0_USD;
	}
}

///////////////////////////////////////////////////////////////////
// Get/Set
///////////////////////////////////////////////////////////////////
Price Broker::getTradeFee(Price price, int amount)
{
	switch (brokerName)
	{
	case BrokerName::NONE:
		return 0;
		break;
	case BrokerName::HSBC:
		return calcTradeFee_HSBC(price, amount);
		break;
	}
	return 0;
}

Price Broker::getMonthlyFee()
{
	return monthlyFee;
}

Price Broker::getYearlyFee()
{
	return yearlyFee;
}

void Broker::setBroker(BrokerName broker)
{
	brokerName = broker;
}
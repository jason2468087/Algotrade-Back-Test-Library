#pragma once
#include "SysConst.h"
#include "SysType.h"
#include <array>

class Broker
{
public:
	Broker(BrokerName broker);

	//===== Get/Set =====
	void setBroker(BrokerName broker);
	Price getTradeFee(Price price, int amount);
	Price getMonthlyFee();
	Price getYearlyFee();

private:
	//===== Data =====
	BrokerName brokerName{};
	Price monthlyFee{};
	Price yearlyFee{};

	//===== Calculations =====
	Price calcTradeFee_HSBC(Price price, int amount);
};
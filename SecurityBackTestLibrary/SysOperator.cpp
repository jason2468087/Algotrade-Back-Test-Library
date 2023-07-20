#include "SysOperator.h"

std::ostream& operator<<(std::ostream& os, const Date& dt)
{
	os << dt.d << '/' << dt.m << '/' << dt.y;
	return os;
}

bool operator==(const Date& d1, const Date& d2)
{
	return (d1.d == d2.d) && (d1.m == d2.m) && (d1.y == d2.y);
}

Price operator""_USD(long double price)
{
	return static_cast<int>(round(price * 100));
}

Price operator""_USD(unsigned long long price)
{
	return static_cast<int>(price * 100);
}

std::string ptos(Price price)
{
	if (price > 0)
	{
		return "$" + std::to_string(price / 100) + "." + std::to_string(price / 10 % 10) + std::to_string(price % 10);
	}
	else if (price == 0)
	{
		return "$0";
	}
	else
	{
		return "-$" + std::to_string(-price / 100) + "." + std::to_string(-price / 10 % 10) + std::to_string(-price % 10);
	}
}

Price dtop(double dPrice)
{
	return static_cast<int>(round(dPrice * 100));
}
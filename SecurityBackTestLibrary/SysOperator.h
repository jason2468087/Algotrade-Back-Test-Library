#pragma once
#include "SysType.h"

#include <string>
#include <cmath>

std::string ptos(Price price);
Price dtop(double price);
extern Price operator"" _USD(long double price);
extern Price operator"" _USD(unsigned long long price);
extern bool operator==(const Date& d1, const Date& d2);
extern std::ostream& operator<<(std::ostream& os, const Date& dt);
#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>

class Share;
class Portfolio;
class ControlPanel;
class Calendar;
class Market;
class Index;
struct Date;
struct Order;
enum class LogType;

using Symbol = std::string;
using Price = int;
using RawDate = int;
using Path = std::string;
using Action = void(*)(Portfolio&, ControlPanel*, Market&, Index&);
using PortfolioName = std::string;

using DataTable = std::vector<std::vector<double>>;
using DataRow = std::vector<double>;
using ShareList = std::unordered_map<Symbol, int>;
using PortfolioList = std::unordered_map <PortfolioName, Portfolio>;

using PriceRow = std::pair<Date,Price>;
using MultiPriceRow = std::pair<Date, std::vector<Price>>;
using MultiFloatRow = std::pair<Date, std::vector<double>>;
using TradeRow = std::tuple<Date, LogType, int, Symbol, Price>;
using TradeLogTable = std::vector<TradeRow>;

struct Date
{
	Date() :d{ 1 }, m{ 1 }, y{ 1980 }{}
	Date(int d_, int m_, int y_) :d{ d_ }, m{ m_ }, y{ y_ }{}
	Date(const Date& date) :d{ date.d }, m{ date.m }, y{ date.y } {}

	int d;
	int m;
	int y;

	friend std::ostream& operator<<(std::ostream& os, const Date& dt);
	friend bool operator==(const Date& d1, const Date& d2);
};

struct Order
{
	Order(int oType, Symbol sym, double pct) :orderType(oType), symbol{ sym }, percentage{ pct }{};
	Order(int oType, Symbol sym, int amt) :orderType(oType), symbol{ sym }, amount{ amt }{};
	Order(int oType, Symbol sym, int amt, Price p, Date exDate) :orderType(oType), symbol{ sym }, amount{ amt }, price{ p }{expiryDate = exDate; }

	int orderType{};
	Symbol symbol{};
	int amount{};
	double percentage{};
	Price price{};
	Date expiryDate{};
};

struct Statistic
{
	Price initialNetWorth{};
	Price finalNetWorth{};
	Price maxNetWorth{};
	Price minNetWorth{ 2147483647 };

	Price totalPriceBought{};
	Price totalPriceSold{};
	int totalStockBought{};
	int totalStockSold{};

	Price totalGrowth{};
	double totalGrowthPercent{};
	Price priceGrowth{};
	Price totalDividend{};
	Price totalSalary{};
};

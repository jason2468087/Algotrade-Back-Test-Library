// AssetBalancePortfolio.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//
#include "ControlPanel.h"
#include <iostream>
#include <string>
#include <algorithm>

void balanceBuy(int idx, Portfolio& portfolio, ControlPanel* controlPanel);
void balanceSell(int idx, Portfolio& portfolio, ControlPanel* controlPanel);

int calcTradeAmount(Symbol symbol, Price tradeVol);
int calcLowerTradeAmount(Symbol symbol, Price tradeVol);

Date start{ 1,6,2010 };
Date end{ 20,6,2023 };

ControlPanel testPanel;

Price initWealth{ 100000_USD };
Price salary{ 10000_USD };

int stockCnt = 5;
enum { SPY, VT, BND, GLD, TQQQ };
std::string sym[] = {"SPY","VT","BND","GLD","TQQQ"};
double percent[] = { 1.0,0.0,0.0,0.0,0.0 };
double threashold[] = { 0.01,0.01,0.01,0.01,0.01 };
double deviation[] = { 0,0,0,0,0 };

void initialAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
    for (int i = 0; i < stockCnt; i++)
    {
        market.observe(sym[i]);
    }

    for (int i = 0; i < stockCnt; i++)
    {
        int amount{};
        if (i < stockCnt - 1)
        {
            amount = calcTradeAmount(sym[i],initWealth * percent[i]);
        }
        else
        {
            amount = calcLowerTradeAmount(sym[i], initWealth * percent[i]);
        }
        controlPanel->buyStock(Order{ MARKET_ORDER,sym[i],amount });
    }
}

void buyLowestWithSalary(Portfolio& portfolio, ControlPanel* controlPanel, Market& market)
{
    for (int i = 0; i << stockCnt; i++)
    {
        deviation[i] = (percent[i] - portfolio.getStockProportion(sym[i])) / percent[i];
    }

    double max{ 0 };
    int maxIdx{ 0 };
    for (int i = 0; i < stockCnt; i++)
    {
        if (deviation[i] > max)
        {
            max = deviation[i];
            maxIdx = i;
        }
    }

    Price maxTradeVol{ (int)(portfolio.getTotalWorth() * (percent[maxIdx] - portfolio.getStockProportion(sym[maxIdx]))) };
    maxTradeVol = std::min(maxTradeVol, portfolio.getCash());
    int amount{ calcLowerTradeAmount(sym[maxIdx],maxTradeVol) };
    if (amount > 0)
    {
        controlPanel->buyStock(Order{ MARKET_ORDER,sym[maxIdx],amount });
    }
}

void rebalance(Portfolio& portfolio, ControlPanel* controlPanel, Market& market)
{
    bool isSell[] = { 0,0,0,0,0 };
    bool isBuy[] = { 0,0,0,0,0 };

    for (int i = 0; i < stockCnt; i++)
    {
        isSell[i] = portfolio.getStockProportion(sym[i]) > percent[i] + threashold[i];
        isBuy[i] = portfolio.getStockProportion(sym[i]) < percent[i] + threashold[i];
    }

    if (isSell[TQQQ])
        balanceSell(TQQQ, portfolio, controlPanel);
    if (isSell[GLD])
        balanceSell(GLD, portfolio, controlPanel);
    if (isSell[BND])
        balanceSell(BND, portfolio, controlPanel);
    if (isSell[VT])
        balanceSell(VT, portfolio, controlPanel);
    if (isSell[SPY])
        balanceSell(SPY, portfolio, controlPanel);

    if (isBuy[SPY])
        balanceBuy(SPY, portfolio, controlPanel);
    if (isBuy[VT])
        balanceBuy(VT, portfolio, controlPanel);
    if (isBuy[BND])
        balanceBuy(BND, portfolio, controlPanel);
    if (isBuy[GLD])
        balanceBuy(GLD, portfolio, controlPanel);
    if (isBuy[TQQQ])
        balanceBuy(TQQQ, portfolio, controlPanel);
}

void dailyAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
    rebalance(portfolio, controlPanel, market);
}

void monthlyAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
    controlPanel->receiveSalary(salary);

    buyLowestWithSalary(portfolio, controlPanel, market);
}

int main()
{
    testPanel.setDuration(start, end);

    testPanel.setInitAction(initialAction);
    testPanel.setDailyOpenAction(dailyAction);
    testPanel.setMonthlyAction(monthlyAction);

    testPanel.getPortfolioRef().setCash(initWealth);

    try
    {
        testPanel.run();
    }
    catch (ErrorPacket ep)
    {
        std::cerr << ep.getErrorString() << "\n";
    }

    testPanel.getPortfolioRef().printPortfolio();
    testPanel.getPortfolioRef().printStatistic();

    return 0;
}

void balanceBuy(int idx, Portfolio& portfolio, ControlPanel* controlPanel)
{
    double percentNow{ portfolio.getStockProportion(sym[idx]) };
    Price buyVolume = portfolio.getTotalWorth() * (percent[idx] - percentNow);

    if (portfolio.getCash() < buyVolume)
    {
        int amount{ calcTradeAmount(sym[idx],portfolio.getCash()) };
        if (amount > 0)
        {
            controlPanel->buyStock(Order{ MARKET_ORDER,sym[idx],amount });
        }
    }
    else
    {
        int amount{ calcTradeAmount(sym[idx],buyVolume) };
        if (amount > 0)
        {
            controlPanel->buyStock(Order{ MARKET_ORDER, sym[idx],amount });
        }
    }
}

void balanceSell(int idx, Portfolio& portfolio, ControlPanel* controlPanel)
{
    double percentNow{ portfolio.getStockProportion(sym[idx]) };
    Price sellVolume = portfolio.getTotalWorth() * (percentNow - percent[idx]);
    int amount{ calcTradeAmount(sym[idx],sellVolume) };

    controlPanel->sellStock(Order{ MARKET_ORDER,sym[idx],amount });
}

int calcTradeAmount(Symbol symbol, Price tradeVol)
{
    Price stockPrice{ testPanel.getMarketRef().getTodayOpen(symbol)};
    return round(tradeVol / stockPrice);
}

int calcLowerTradeAmount(Symbol symbol, Price tradeVol)
{
    Price stockPrice{ testPanel.getMarketRef().getTodayOpen(symbol) };
    return round(tradeVol / stockPrice);
}


// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案

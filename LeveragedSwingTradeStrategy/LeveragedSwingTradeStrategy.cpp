// MovingAverageStrategy.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include "ControlPanel.h"

ControlPanel testPanel;

Date start{ 1,4,2018 };
Date end{ 1,1,2023 };

Price initWealth{ 100000_USD };
Price salary{ 0_USD };

bool isEnteredMarket{ false };

void initAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
    market.observe("SPY");
    market.observe("TQQQ");
    market.observe("SQQQ");
}

void NowxMAAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
    Price mAvg{ index.movingAverage(controlPanel->getToday(),20,"TQQQ") };
    Price tqqqPrice{ market.getPrice("TQQQ") };

    if (tqqqPrice > mAvg)
    {
        int amount{ portfolio.getCash() / tqqqPrice };
        if (amount > 0)
            controlPanel->buyStock(Order{ MARKET_ORDER,"TQQQ",amount });
    }
    else if (tqqqPrice < mAvg)
    {
        int amount{ portfolio.getShareHolded("TQQQ") };
        if (amount > 0)
            controlPanel->sellStock(Order{ MARKET_ORDER,"TQQQ",amount });
    }
}

void MASwingAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
    Price tqqq20MA{ index.movingAverage(controlPanel->getToday(),20,"TQQQ") };
    Price tqqq10MA{ index.movingAverage(controlPanel->getToday(),10,"TQQQ") };
    Price tqqqPrice{ market.getPrice("TQQQ") };
    int amount{};

    bool isLongMode{ tqqqPrice > tqqq20MA };
    if (isLongMode)
    {
        amount = portfolio.getShareHolded("SQQQ");
        if (amount > 0)
            controlPanel->sellStock(Order{ MARKET_ORDER,"SQQQ",amount });

        amount = portfolio.getCash() / tqqqPrice;
        if (amount > 0)
            controlPanel->buyStock(Order{ MARKET_ORDER,"TQQQ",amount });
    }
    else
    {
        amount = portfolio.getShareHolded("TQQQ");
        if (amount > 0)
            controlPanel->sellStock(Order{ MARKET_ORDER,"TQQQ",amount });

        Price sqqqMA{ index.movingAverage(controlPanel->getToday(),5,"SQQQ") };
        Price sqqqPrice{ market.getPrice("SQQQ") };
        bool isShortMode{ sqqqPrice > sqqqMA };
        if (isShortMode)
        {
            amount = portfolio.getCash() / sqqqPrice ;
            if (amount > 0)
                controlPanel->buyStock(Order{ MARKET_ORDER,"SQQQ",amount });
        }
        else
        {
            amount = portfolio.getShareHolded("SQQQ") ;
            if (amount > 0)
                controlPanel->sellStock(Order{ MARKET_ORDER,"SQQQ",amount });
        }
    }
}

void MACrossAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
    Price mAvgLong{ index.movingAverage(controlPanel->getToday(),20,"TQQQ") };
    Price mAvgShort{ index.movingAverage(controlPanel->getToday(),5,"TQQQ") };
    Price tqqqPrice{ market.getPrice("TQQQ") };

    if (mAvgShort > mAvgLong)
    {
        int amount{ portfolio.getCash() / tqqqPrice };
        if (amount > 0)
            controlPanel->buyStock(Order{ MARKET_ORDER,"TQQQ",amount });
    }
    else if (mAvgShort < mAvgLong)
    {
        int amount{ portfolio.getShareHolded("TQQQ") };
        if (amount > 0)
            controlPanel->sellStock(Order{ MARKET_ORDER,"TQQQ",amount });
    }
}

void StrongHoldAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
    Price tqqqPrice{ market.getPrice("TQQQ") };
    int amount{ portfolio.getCash() / tqqqPrice };
    if (amount > 0)
    {
        controlPanel->buyStock(Order{ MARKET_ORDER,"TQQQ",amount });
    }
    std::cout << tqqqPrice/100.0 << "\n";
}

const int rsiMAWin = 10;
double rsiArr[rsiMAWin] = {};
double prevRsiMA[3] = {};
void calcRsiMA(double rsi_)
{
    // Shift rsi record
    for (int i = 1; i < rsiMAWin; i++)
    {
        rsiArr[i - 1] = rsiArr[i];
    }
    rsiArr[rsiMAWin - 1] = rsi_;

    // Calc RSI MA
    double rsiMA{};
    for (int i = 0 ; i < rsiMAWin; i++)
    {
        rsiMA += rsiArr[i];
    }
    rsiMA /= rsiMAWin;

    // Shift rsi MA record
    prevRsiMA[0] = prevRsiMA[1];
    prevRsiMA[1] = prevRsiMA[2];
    prevRsiMA[2] = rsiMA;
}
void CompositeIndicatorAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
    Price mAvg20{ index.movingAverage(controlPanel->getToday(),20,"TQQQ") };
    Price mAvg10{ index.movingAverage(controlPanel->getToday(),10,"TQQQ") };
    double rsi{ index.relativeStrength(controlPanel->getToday(),10,"TQQQ")};
    calcRsiMA(rsi);

    //std::cout << rsi << " " << prevRsiMA[2] << "\n";

    Price tqqqPrice{ market.getPrice("TQQQ") };

    bool isAboveMA20{ tqqqPrice > mAvg20 };
    bool isAboveMA10{ tqqqPrice > mAvg10 };
    bool isAboveRsiMA{ rsi > prevRsiMA[2] };
    bool isRSIMAPeak{ prevRsiMA[0] < prevRsiMA[1] && prevRsiMA[1] > prevRsiMA[2] };
    bool isRSIMADip{ prevRsiMA[0] > prevRsiMA[1] && prevRsiMA[1] < prevRsiMA[2] };
    bool isRSIMAFall{ prevRsiMA[0] > prevRsiMA[1] && prevRsiMA[1] > prevRsiMA[2] };
    bool isRSIMARise{ prevRsiMA[0] < prevRsiMA[1] && prevRsiMA[1] < prevRsiMA[2] };

    if (isAboveMA20 && (isRSIMARise || isRSIMADip))
    {
        int amount{ portfolio.getCash() / tqqqPrice };
        if (amount > 0)
        {
            controlPanel->buyStock(Order{ MARKET_ORDER,"TQQQ",amount });
        }
    }
    else if (!isAboveMA20 && (isRSIMAFall || isRSIMAPeak) )
    {
        int amount{ portfolio.getShareHolded("TQQQ") };
        if (amount > 0)
        {
            controlPanel->sellStock(Order{ MARKET_ORDER,"TQQQ",amount });
        }
    }
}

int main()
{
    testPanel.setDuration(start, end);

    testPanel.setInitAction(initAction);
    testPanel.setDailyCloseAction(NowxMAAction);

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

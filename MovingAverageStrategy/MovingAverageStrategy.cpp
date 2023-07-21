// MovingAverageStrategy.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include "ControlPanel.h"
#include <iostream>
#include <string>
#include <algorithm>

ControlPanel testPanel;

Date start{ 1,1,2010 };
Date end{ 1,1,2022 };

Price initWealth{ 25000_USD };
Price salary{ 1200_USD };

bool isEnteredMarket{ false };

void initAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
    market.observe("SPY");
}

void dailyAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
    Price mAvg50{index.movingAverage(controlPanel->getToday(),50,"SPY")};
    Price mAvg5{ index.movingAverage(controlPanel->getToday(),5,"SPY") };
    Price spyPrice{ market.getPrice("SPY") };

    if (!isEnteredMarket && mAvg5 > mAvg50)
    {
        int amount{ portfolio.getCash() / spyPrice };
        controlPanel->buyStock(Order{ MARKET_ORDER,"SPY",amount });

        isEnteredMarket = true;
    }
    else if (isEnteredMarket && mAvg5 < mAvg50)
    {
        int amount{ portfolio.getShareHolded("SPY") };
        controlPanel->sellStock(Order{ MARKET_ORDER,"SPY",amount });

        isEnteredMarket = false;
    }
}

int main()
{
    testPanel.setDuration(start, end);

    testPanel.setInitAction(initAction);
    testPanel.setDailyOpenAction(dailyAction);

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

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
// AssetBalancePortfolio.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//
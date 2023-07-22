# Algotrade-Back-Test-Library

## Introduction
This is a C++ library for conducting Algo-trade backtesting. It can compute the expected earning from a trading strategy by simulating stock market with real life market data. To use the library, just put your daily decision routine in a function and parse the function into the given api as input. After a second, the output is computed. A brief result will be print on console window. Moreover, trading record and daily portfolio status are written into CSV files in log folder. This library also provided moving average and relative strength index to assist your trade.

## Dependencies
- C++ 17

## Computation Output
The computed output are:
* Final status of your portfolio - - - - - (Console output)
* Buy and Sell Record - - - - - (trade_log.txt)
* Daily change of total asset - - - - - (total_worth_log.csv)
* Daily change of portfolio earning - - - - - (capital_gain_log.csv & fixed_income_log.csv)
* Daily change of portfolio composition - - - - - (stock_ratio_log.csv & stock_worth_log.csv)

(All output are located in path: ..\SecurityBackTestLibrary\Log)

Console Output of the library.

![alt text](https://github.com/jason2468087/Algotrade-Back-Test-Library/blob/master/img/console%20output.png?raw=true)

Buy and Sell Record

![alt text](https://github.com/jason2468087/Algotrade-Back-Test-Library/blob/master/img/trade%20log%20output.png?raw=true)

Daily Portfolio Worth

![alt text](https://github.com/jason2468087/Algotrade-Back-Test-Library/blob/master/img/total%20worth%20output.png?raw=true)

## Tutorial
### How to use the library

Here is a basic template of the library.
```C++
#include "ControlPanel.h"
#include <iostream>
#include <string>
#include <algorithm>

ControlPanel testPanel;

Date start{ /*...*/ }; // Start date of simulation, e.g. 1,1,2010
Date end{ /*...*/ }; // End date of simulation, e.g. 1,1,2022

Price initWealth{ /*...*/ }; // Starting Capital in USD, e.g. 25000_USD
Price salary{ /*...*/ }; // Monthly Salary in USD, e.g. 1200_USD

void initAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
    // Trading Strategy: Action when entering market
}

void dailyAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
    // Trading Strategy: Action done every day
}

int main()
{
    testPanel.setDuration(start, end);

    testPanel.setInitAction(initAction);
    testPanel.setDailyOpenAction(dailyAction);

    testPanel.getPortfolioRef().setCash(initWealth);

    try
    {
        testPanel.run(); // <= Start Simulation !!!
    }
    catch (ErrorPacket ep)
    {
        std::cerr << ep.getErrorString() << "\n";
    }

    testPanel.getPortfolioRef().printPortfolio();
    testPanel.getPortfolioRef().printStatistic();
    
    return 0;
}
```
To run the simulation, setup the library by providing parameters and "Action" function through setter function. Then call run().

As you can see, we define our trading strategy inside a function and parse our strategy into the library as an input argument. These strategy function must follow the format given in template as they are a specific function pointer type called "Action".
___
Within "Action" function, we can buy/sell stocks, get market information, get portfolio status and use financial indexes. These features are provided by the classes in input argument of the function.

The functionalities of the 4 classes are:
| Class         | Description                                                              |
| ------------- |:---------------------:                                                   |
| Portfolio     | Contain investment portfolio data (e.g. stocks holded, cash amount, ...) |
| Control Panel | Use this class to perform trading action                                 |
| Market        | Store historical market data                                             |
| Index         | A class of math function about financial indexes                         |

```C++
void dailyAction(Portfolio& portfolio, ControlPanel* controlPanel, Market& market, Index& index)
{
    // Trading Strategy: Action done every day
}
```

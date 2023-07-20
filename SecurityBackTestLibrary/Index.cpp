#include "SysOperator.h"
#include "SysConst.h"
#include "Index.h"
#include "Market.h"
#include "Calendar.h"

Index::Index(Market& market)
	: marketRef{ market }
{

}

Price Index::movingAverage(Date today, int duration, Symbol symbol)
{
	Price sum{};
	RawDate rawToday{ Calendar::convertToRawDate(today) };

	int dayCnt{ 0 };
	RawDate iterDate{ rawToday };
	while (dayCnt < duration)
	{
		if (marketRef.getIsTraded(symbol, iterDate))
		{
			sum += marketRef.getClose(symbol, iterDate);
			dayCnt++;
		}
		iterDate--;
	}

	return sum / duration;
}

double Index::relativeStrength(Date today, int duration, Symbol symbol)
{
	std::vector<Price> priceArr{};
	RawDate rawtoday{ Calendar::convertToRawDate(today) };

	int dayCnt{ 0 };
	RawDate iterDate{ rawtoday };

	while (dayCnt < (duration+1))
	{
		if (marketRef.getIsTraded(symbol, iterDate))
		{
			Price todayPrice{ marketRef.getClose(symbol,iterDate) };
			priceArr.push_back(todayPrice);

			dayCnt++;
		}
		iterDate--;
	}

	double RS{};
	double sumGain{};
	double sumLoss{};
	int gainCnt{};
	int lossCnt{};

	if (prevRSGain == NULL || prevRSLoss == NULL)
	{
		for (int i = 0; i < priceArr.size() - 1; i++)
		{
			Price todayPrice{ priceArr[i] };
			Price ytdPrice{ priceArr[i + 1] };

			bool isRise{ todayPrice > ytdPrice };
			double changePercent{ (double)(todayPrice - ytdPrice) / (double)ytdPrice };

			if (isRise)
			{
				sumGain += changePercent;
				gainCnt++;
			}
			else
			{
				sumLoss += -changePercent;
				lossCnt++;
			}
		}
		double gain{ sumGain / duration };
		double loss{ sumLoss / duration };
		RS = gain / loss;

		prevRSGain = gain;
		prevRSLoss = loss;
	}
	else
	{
		Price todayPrice{ priceArr[0] };
		Price ytdPrice{ priceArr[1] };

		bool isRise{ todayPrice > ytdPrice };
		double changePercent{ (double)(todayPrice - ytdPrice) / (double)ytdPrice };

		double todayGain{ isRise ? changePercent : 0 };
		double todayloss{ isRise ? 0 : changePercent * -1 };


		double gain{ (prevRSGain * (duration - 1) + todayGain) / duration };
		double loss{ (prevRSLoss * (duration - 1) + todayloss) / duration };

		RS = gain / loss;

		prevRSGain = gain;
		prevRSLoss = loss;
	}

	return 100 - (100 / (1 + RS));
}

void Index::setMarketReference(Market& mkt)
{
	marketRef = mkt;
}
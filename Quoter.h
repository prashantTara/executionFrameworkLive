#ifndef __QUOTER_H__
#define __QUOTER_H__

#include "memory"
#ifndef __LIVEMODE__
	#include "/home/infra/InternalSimulatorUsers/Backtester/strategyConnector.h"
#endif
#ifdef __LIVEMODE__
	#include "/home/dev/StrategyBox/strategyConnector.h"
	#include "/home/dev/StrategyBox/structures.h"
#endif
#include "portfolio.h"

class Portfolio;
class Quoter
{
    public:
        Quoter();
        Quoter(std::string symbol , std::string quoterType , quoterParams quoterParams_ , Portfolio * portfolio_);
        ~Quoter();
        void setParams(quoterParams quoterParams_);
        void calQuotes(int leaderLag , double fairBuyPrice_ , double fairSellPrice_ , int64_t bidPrice , int64_t askPrice);
        void calQuotesABS(double fairBuyPrice_ , double fairSellPrice_);
        void calQuotesBPS(double fairBuyPrice_ , double fairSellPrice_);
        void calQuotesQty();
        void calPnlSkew();
        void adjustQuoteWithBook();
        int priceCheck(int price , bool type);
        void setPrevFairSellPrice();
        void setPrevFairBuyPrice();
        void quoteOrders();
        void quoteBuyOrder();
        void quoteSellOrder();
        bool canModifyBuyOrder();
        bool canModifyBuyOrderABS();
        bool canModifyBuyOrderBPS();
        bool canModifySellOrder();
        bool canModifySellOrderABS();
        bool canModifySellOrderBPS();
        void cancelOpenBuyOrder();
        void cancelOpenSellOrder();
        void cancelOpenOrderOnEndTime();
        void cancelOpenOrder();
        void onResponse(char responseType, long long int orderID, std::string symbol, char side, int fillQuantity,int price, int16_t errorCode, char orderType, int openQuantity);
        void stopPortfolio();
        void startPortfolio();
        void endPortfolio(int bid , int ask);
        //yet To implement
        void stopBuying();
        void stopSelling();
        void startBuying();
        void startSelling();
        void squareOffQuoter();

        void trendSkewQuotesBPS();
        void trendSkewQuotesABS();


    public:
        bool pnlSkewOn = false;
        double lastBuyOrderFillPrice = 0;
        double lastSellOrderFillPrice = 0;

        double fairBuyPrice = 0;
        double fairSellPrice = 0;

        double lastBuyOrderFairPrice = 0;
        double lastSellOrderFairPrice = 0;
   
        
        int64_t lagBidPrice = 0;
        int64_t lagAskPrice = 0;
        std::string _symbol;
        std::string _quoterType;

        int position = 0;


        Portfolio * _portfolio = nullptr;


        int wait = 0;
        int buyWait = 0;
        int sellWait = 0;
        long long int lastBuyOrderId = 0;
        long long int lastSellOrderId = 0;

        std::pair<long long int, ORDER> buyOrderHandler;
        std::pair<long long int, ORDER> sellOrderHandler;
        int buyFillQuantity = 0;
        int sellFillQuantity = 0;
        
        bool portfolioStart = false;

        bool useAbsoluteOffset = false;
        double bidSpread_BPS = 0;
        double askSpread_BPS = 0;
        double bidSpread_ABS = 0;
        double askSpread_ABS = 0;
        bool useAbsoluteModifyThreshold = false;
        double quoteModifyPassiveThreshold_Bid_BPS = 0;
        double quoteModifyPassiveThreshold_Ask_BPS = 0;
        double quoteModifyAggresiveThreshold_Bid_BPS = 0;
        double quoteModifyAggresiveThreshold_Ask_BPS = 0;
        double quoteModifyPassiveThreshold_Bid_ABS = 0;
        double quoteModifyPassiveThreshold_Ask_ABS = 0;
        double quoteModifyAggresiveThreshold_Bid_ABS = 0;
        double quoteModifyAggresiveThreshold_Ask_ABS = 0;
        bool quoterSwitch = true;
        bool buy_switch = true;
        bool original_buy_switch = true;
        bool sell_switch = true;
        bool original_sell_switch = true;
        int originalBuyQuoteQty = 1;
        int originalSellQuoteQty = 1;
        int buyQuoteQty = 1;
        int sellQuoteQty = 1;
        double quoteQtySkewFactorBuy = 1;
        double quoteQtySkewFactorSell = 1;
        bool useLongTrendSkew = false;
        bool useShortTrendSkew = false;
        bool useTrendOffsetABS = false;
        bool trendSkewInversion = false;
        double longTrendIncreaseSkewOffset_BPS = 0;
        double longTrendDecreaseSkewOffset_BPS = 0;
        double shortTrendIncreaseSkewOffset_BPS = 0;
        double shortTrendDecreaseSkewOffset_BPS = 0;
        double longTrendIncreaseSkewOffset_ABS = 0;
        double longTrendDecreaseSkewOffset_ABS = 0;
        double shortTrendIncreaseSkewOffset_ABS = 0;
        double shortTrendDecreaseSkewOffset_ABS = 0;
        int maxShortTrendShifts = 1;
        int maxLongTrendShifts = 1;
        double longTrendQualifyPercent = 1;
        double shortTrendQualifyPercent = 1;
        int minQuotePassiveGap = 0;
        int maxQuotePassiveGap = 0;

        bool useLtpFilter = false;        
        
        double directionShortWindow = 0;
        double directionLongWindow = 0;

        int64_t alphaBuyPrice = 0;
        int64_t alphaSellPrice = 0;
        int64_t alpha2BuyPrice = 0;
        int64_t alpha2SellPrice = 0;

};

#endif
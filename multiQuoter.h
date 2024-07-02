#ifndef __MULTIQUOTER_H__
#define __MULTIQUOTER_H__

#include "memory"
#ifndef __LIVEMODE__
	#include "/home/infra/InternalSimulatorUsers/Backtester/strategyConnector.h"
#endif
#ifdef __LIVEMODE__
	#include "/home/dev/StrategyBox/strategyConnector.h"
	#include "/home/dev/StrategyBox/structures.h"
#endif

#include <stack>
#include "portfolio.h"

class Portfolio;
class MultiQuoter
{
    public :
        MultiQuoter();
        MultiQuoter(std::string symbol , std::string multiQuoterType , multiQuoterParams multiQuoterParams_ , Portfolio * portfolio_);
        ~MultiQuoter();
        void setParams(multiQuoterParams multiQuoterParams_);
        void calQuotes(int leaderLag , double fairBuyPrice_ , double fairSellPrice_ , int64_t bidPrice , int64_t askPrice);
        void calQuotesABS(double fairBuyPrice_ , double fairSellPrice_);
        void calQuotesBPS(double fairBuyPrice_ , double fairSellPrice_);
        int priceCheck(int price , bool type);
        void adjustQuoteWithBook();
        void adjustQuoteWithBookQty();
        int findBuyPriceLevelQty(int64_t price);
        int findSellPriceLevelQty(int64_t price);
        void trendSkewQuotesABS();
        void trendSkewQuotesBPS();
        void setPrevFairBuyPrice();
        void setPrevFairSellPrice();
        void quoteOrders();
        void calQuotesQty();
        bool canModifyBuyOrder();
        bool canModifySellOrder();
        bool canModifyBuyOrderABS();
        bool canModifyBuyOrderBPS();
        bool canModifySellOrderABS();
        bool canModifySellOrderBPS();
        void calBuyPriceVector();
        void calBuyPriceVectorABS();
        void calBuyPriceVectorBPS();
        void calSellPriceVector();
        void calSellPriceVectorABS();
        void calSellPriceVectorBPS();
        void calQtyVectors();
        void quoteBuyOrder();
        void quoteSellOrder();
        void refillQuoteBuyOrder();
        void refillQuoteSellOrder();
        void stopBuying();
        void startBuying();
        void stopSelling();
        void startSelling();
        void squareOffQuoter();
        void cancelOpenBuyOrder();
        void cancelOpenSellOrder();
        void cancelOpenOrderOnEndTime();
        void cancelOpenOrder();
        void endPortfolio(int64_t bid , int64_t ask);
        void onResponse(char responseType, long long int orderID, std::string symbol, char side, int fillQuantity,int price, int16_t errorCode, char orderType, int openQuantity);        
        void removeBuyOrderIdFromStack(long long orderId , int64_t price);
        void removeSellOrderIdFromStack(long long orderId , int64_t price);

    
    public : 
        bool useMultiQuoter = false;
        bool useAbsoluteOffset = false;
        int levels = 0;
        double bidSpread_BPS = 0;
        double askSpread_BPS = 0;
        double bidSpread_ABS = 0;
        double askSpread_ABS = 0;
        double bidSpreadIncrement_BPS = 0;
        double askSpreadIncrement_BPS = 0;
        double bidSpreadIncrement_ABS = 0;
        double askSpreadIncrement_ABS = 0;
        double quoteModifyPassiveThreshold_Bid_BPS = 0;
        double quoteModifyPassiveThreshold_Ask_BPS = 0;
        double quoteModifyAggresiveThreshold_Bid_BPS = 0;
        double quoteModifyAggresiveThreshold_Ask_BPS = 0;
        double quoteModifyPassiveThreshold_Bid_ABS = 0;
        double quoteModifyPassiveThreshold_Ask_ABS = 0;
        double quoteModifyAggresiveThreshold_Bid_ABS = 0;
        double quoteModifyAggresiveThreshold_Ask_ABS = 0;
        int buyQuoteQty = 1;
        int sellQuoteQty = 1;
        int originalBuyQuoteQty = 1;
        int originalSellQuoteQty = 1;
        double quoteQtySkewFactorBuy = 1;
        double quoteQtySkewFactorSell = 1;
        int buyQuoteQtyIncrement = 1;
        int sellQuoteQtyIncrement = 1;
        double buyFillPercentage = 0.5;
        double sellFillPercentage = 0.5;
        int maxBuyPosition = 0;
        int maxSellPosition = 0;
        int orderLimit = 0;
        bool multiQuoterSwitch = true;
        bool buy_switch = true;
        bool sell_switch = true;
        bool onlyPassive = false;
        bool useLtpFilter = false;
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
        double shortTrendQualifyPercent = 1;
        int maxShortTrendShifts = 1;
        double longTrendQualifyPercent = 1;
        int maxLongTrendShifts = 1;
        int minQuotePassiveGap = 0;
        int maxQuotePassiveGap = 0;
        bool topLevelMinQtyFilter = false;
        int topLevelMinQtyBid = 0;
        int topLevelMinQtyAsk = 0;
        int originalTopLevelMinQtyBid = 0;
        int originalTopLevelMinQtyAsk = 0;
        int topLevelBidQtySkewFactor = 0;
        int topLevelAskQtySkewFactor = 0;
        double topLevelBidQtySkewFactorPosNetQty = 1;
        double topLevelBidQtySkewFactorNegNetQty = 1;
        double topLevelAskQtySkewFactorPosNetQty = 1;
        double topLevelAskQtySkewFactorNegNetQty = 1;
        std::string _symbol;
        bool original_buy_switch;
        bool original_sell_switch;
        Portfolio * _portfolio;
        std::string _multiQuoterType;
        int64_t lagBidPrice;
        int64_t lagAskPrice;
        double fairBuyPrice = 0;
        double fairSellPrice = 0;
        double lastBuyOrderFairPrice = 0;
        double lastSellOrderFairPrice = 0;
        std::unordered_map<long long ,int> orderIdToBuyFillQuantity;
        std::unordered_map<long long ,int64_t> orderIdToPrice;
        std::unordered_map<long long ,int> orderIdToBuyOpenQuantity;
        std::unordered_map<long long ,int> orderIdToSellFillQuantity;
        std::unordered_map<long long ,int> orderIdToSellOpenQuantity;
        std::vector<double> buyOrderPrice;
        std::vector<double> prevBuyOrderPrice;
        std::vector<bool> prevBuyOrderHandled;
        std::vector<double> buyOrderQty;
        std::vector<double> sellOrderPrice;
        std::vector<double> prevSellOrderPrice;
        std::vector<bool> prevSellOrderHandled;
        std::vector<double> sellOrderQty;
        std::unordered_map<int64_t , std::stack<long long>> buyPriceToOrderId;
        std::unordered_map<int64_t , std::stack<long long>> sellPriceToOrderId;
        std::unordered_map<long long, int> buyPriceToOpenQty;
        std::unordered_map<long long, int> buyPriceToFillQty;
        std::unordered_map<long long, int> sellPriceToOpenQty;
        std::unordered_map<long long, int> sellPriceToFillQty;
        int buyWait = 0;
        int sellWait = 0;
        int position = 0;

        double directionShortWindow = 0;
        double directionLongWindow = 0;
};

#endif
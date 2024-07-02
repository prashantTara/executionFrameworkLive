#ifndef __JUMPER_H__
#define __JUMPER_H__

#include "memory"
#include "portfolio.h"

class Portfolio;
class Jumper
{

    public:
        Jumper();
        Jumper(jumperParams jumperParams_ , Portfolio * portfolio_);
        ~Jumper();
        void setParams(jumperParams jumperParams_);
        void updateData(long long int time , double fairBuyPrice_ , double fairSellPrice_ );
        void calJumperQuotesABS(double fairBuyPrice_ , double fairSellPrice_);
        void calJumperQuotesBPS(double fairBuyPrice_ , double fairSellPrice_);
        void quoteJumpOrders();
        void findBuyLevelPrice();
        void findSellLevelPrice();
        void findBuyPriceCummQty();
        void findSellPriceCummQty();
        void findBuyPriceTopToBottom();
        void findBuyPriceBottomToTop();
        void findSellPriceTopToBottom();
        void findSellPriceBottomToTop();
        void findNearestBuyLevel();
        void findNearestSellLevel();
        void calQuotesQty();
        void setPrevFairBuyPrice();
        void setPrevFairSellPrice();
        bool canModifyBuyOrder();
        bool canModifySellOrder();
        void quoteBuyOrder();
        void quoteSellOrder();
        void cancelOpenBuyOrder();
        void cancelOpenSellOrder();
        void cancelOpenOrderOnEndTime();
        void cancelOpenOrder();
        void stopPortfolio();
        void startPortfolio();
        void endPortfolio(int bid , int ask);
        void onResponse(char responseType, long long int orderID, std::string symbol, char side, int fillQuantity,int price, int16_t errorCode, char orderType, int openQuantity);
        void stopBuying();
        void stopSelling();
        void startBuying();
        void startSelling();
        void squareOffQuoter();
        void calBuyBookAvg();
        void calSellBookAvg();
        int priceCheck(int price , bool type);

    public:
        std::string _jumperType = "Jumper";
        Portfolio * _portfolio = nullptr;
        bool useJumper = false;
        int buyJoinQty = 0;
        int sellJoinQty = 0;
        int buyJumpQty = 0;
        int sellJumpQty = 0;
        bool useCummQty = false;
        int bidCummQty = 0;
        int askCummQty = 0;
        int bookLength = 5;
        int topToBottomPriority = 0;    
        int bottomToTopPriority = 0;
        bool useAbsOffset = false;
        double bidOffsetBPS = 0;    
        double askOffsetBPS = 0;    
        int bidOffsetABS = 0;    
        int askOffsetABS = 0;
        int buyQuoteQty = 0;
        int sellQuoteQty = 0;
        int buyMaxPosition = 0;
        int sellMaxPosition = 0;
        int modifyDelay = 0;
        bool jumperSwitch = false;
        bool jumperBuySwitch = false;
        bool jumperSellSwitch = false;

        double fairBuyPrice;
        double fairSellPrice;
        double lastBuyOrderFairPrice;
        double lastSellOrderFairPrice;
        double lastBuyOrderFillPrice = 0;
        double lastSellOrderFillPrice = 0;
        int jumperQuoteBookBuyLevel = -1;
        int jumperQuoteBookSellLevel = -1;

        int originalBuyQuoteQty = 0;
        int originalSellQuoteQty = 0;
        bool original_sell_switch = true;
        bool original_buy_switch = true;

        bool useAvgBook = false;
        double avgOrderToBookThreshold_BPS = 0;
        double avgOrderToBookThreshold_ABS = 0;
        int avgBookLevels = 0;
        std::vector<double> levelWeightageVector ;

        int wait = 0;
        int buyWait = 0;
        int sellWait = 0;
        long long int lastBuyOrderId = 0;
        long long int lastSellOrderId = 0;
        int position = 0;
        std::pair<long long int, ORDER> buyOrderHandler;
        std::pair<long long int, ORDER> sellOrderHandler;
        int buyFillQuantity = 0;
        int sellFillQuantity = 0;

        long long int lastJumperUpdateTime = 0;

        double buyBookAvgPrice = 0;
        double sellBookAvgPrice = 0;

};


#endif
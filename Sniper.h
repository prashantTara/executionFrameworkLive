#ifndef __SNIPER_H__
#define __SNIPER_H__

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
class Sniper
{
    public : 
        Sniper();
        Sniper(std::string symbol , std::string sniperType , sniperParams sniperParams_ , Portfolio * portfolio_ );
        ~Sniper();
        void setParams(sniperParams sniperParams_);
        void updateData(int leaderLag , double fairBuyPrice_ , double fairSellPrice_ , int64_t bidPrice , int64_t askPrice);
        void calSniperQuotesABS(double fairBuyPrice_ , double fairSellPrice_);
        void calSniperQuotesBPS(double fairBuyPrice_ , double fairSellPrice_);
        int priceCheck(int price , bool type);
        bool checkOpportunityBPS();
        bool checkOpportunityABS();
        bool checkBuyBookQtyAvail();
        bool checkSellBookQtyAvail();
        bool canModifyBuyOrderABS();
        bool canModifyBuyOrderBPS();
        bool canModifySellOrderABS();
        bool canModifySellOrderBPS();

        void quoteBuyOrderIOC();
        void quoteSellOrderIOC();

        void setPrevFairBuyPrice();
        void setPrevFairSellPrice();
        void onResponse(char responseType, long long int orderID, std::string symbol, char side, int fillQuantity,int price, int16_t errorCode, char orderType, int openQuantity);

        void stopBuying();
        void stopSelling();
        void startBuying();
        void startSelling();

    public :
        std::string _symbol;
        std::string _sniperType;
        Portfolio * _portfolio;

        bool useAbsoluteOffset = false;
        bool useSniper = false;
        double buyOpportunity_BPS = 0;
        double sellOpportunity_BPS = 0;
        double buyOpportunity_ABS = 0;
        double sellOpportunity_ABS = 0;
        double bidSpread_BPS = 0;
        double askSpread_BPS = 0;
        double bidSpread_ABS = 0;
        double askSpread_ABS = 0;
        double quoteModifyThreshold_Bid_BPS = 0;
        double quoteModifyThreshold_Ask_BPS = 0;
        double quoteModifyThreshold_Bid_ABS = 0;
        double quoteModifyThreshold_Ask_ABS = 0;
        bool sniperSwitch = true;
        bool buy_switch = true;
        bool sell_switch = true;
        int buyQuoteQty = 1;
        int sellQuoteQty = 1;
        int minBookBuyQtyToFire = 0;
        int minBookSellQtyToFire = 0;
        int buyShootCount = 0;
        int sellShootCount = 0;
        int totalShootCount = 0;
        int buyMaxPosition = 0;
        int sellMaxPosition = 0;
        int booklevel = 5;

        double fairBuyPrice = 0;
        double fairSellPrice = 0;
        int64_t lagBidPrice = 0;
        int64_t lagAskPrice = 0;

        bool captureBySell = false;
        bool captureByBuy = false;

        double lastBuyOrderFairPrice = 0;
        double lastSellOrderFairPrice = 0;

        int buyWait = 0;
        int sellWait = 0;

        bool original_buy_switch;
        bool original_sell_switch;

};

#endif
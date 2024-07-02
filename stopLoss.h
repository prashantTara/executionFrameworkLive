#ifndef __STOPLOSS_H__
#define __STOPLOSS_H__

#include "frameworkStructures.h"
#include "portfolio.h"
class Portfolio;

class stopLoss{

    public:
        stopLoss();
        stopLoss(stopLossParams stopLossParams_ , int lotSize, Portfolio * portfolio_);
        ~stopLoss();
        void setParams(stopLossParams stopLossParams_);
        void stopLossSquareOff();
        void stopLossSquareOff(int64_t bidPrice , int64_t askPrice);
        void quoteSellOrder();
        void quoteBuyOrder();
        bool checkStopLoss(int64_t bidPrice , int64_t askPrice);
        void updatePositionAndValue(int position_ , double buyPrice, double sellPrice);
        void onResponse(char responseType, long long int orderID, std::string symbol, char side, int fillQuantity,int price, int16_t errorCode, char orderType, int openQuantity);
        void calFairBuyPrice();
        void calFairSellPrice();
        void setPrevFairBuyPrice();
        void setPrevFairSellPrice();
        void calBuyBookAvg();
        void calSellBookAvg();
        int priceCheck(int price , bool type);

    public:
        double slAcThr = 0;
        double trailStopLoss = 0;
        double maxPortfolioLoss = 0;
        double maxTradeLoss = 0;
        bool useTradeJumper = false;
        double bookCrossThreshold = 0;
        int bookLevel = 0;
        int orderDivisionFactor = 1;
        bool useAvgBook = false;
        bool useAvgBookThresholdABS = false;

        double avgOrderToBookThreshold_BPS = 0;
        double avgOrderToBookThreshold_ABS = 0;

        int position = 0;
        double buyValue = 0;
        double sellValue = 0;
        double curBuyValue = 0;
        double curSellValue = 0;
        bool stopLossWait = false;
        bool sqauredOffTimePassed = false;
        bool clearPositionTimePassed = false;
        double curTradePnl = 0;
        double netPnl = 0;
        double lastPnl = 0;
        double currentPnl = 0;
        double lagAskPrice = 0;
        double lagBidPrice = 0;
        int _lotSize = 1;
        bool sqauredOff = false;
        bool slActivated = false;
        Portfolio * _portfolio = nullptr;

        int wait = 0;
        long long int lastBuyOrderId = 0;
        long long int lastSellOrderId = 0;
        int buyFillQuantity = 0;
        int sellFillQuantity = 0;
        std::pair<long long int, ORDER> buyOrderHandler;
        std::pair<long long int, ORDER> sellOrderHandler;

        int64_t lastBuyOrderFairPrice = 0;
        int64_t lastSellOrderFairPrice = 0;
        int64_t fairBuyPrice = 0;
        int64_t fairSellPrice = 0;
        double buyBookAvgPrice = 0;
        double sellBookAvgPrice = 0;
};


#endif
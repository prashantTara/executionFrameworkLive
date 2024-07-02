#ifndef __CALSUPPORT_H__
#define __CALSUPPORT_H__

#include <memory>
#include <cmath>
#include <numeric>
#include <deque>
#include "frameworkStructures.h"
#include "portfolio.h"
// #include "latency.h"
// class Latency
using namespace std;
class Portfolio;
class calSupport
{

    public:
        calSupport();
        calSupport(std::string symbol , supportParams supportParams_ , Portfolio * portfolio_);
        ~calSupport();
        void setParams(supportParams supportParams_);
        void calculateSupport(int leaderLag , uint8_t type , uint8_t side , int64_t price , int quantity, int64_t bidPrice , int64_t askPrice , long long int time);
        void calTrend(int64_t price , long long int time);
        void calPriceBigMove(int leaderLag ,int64_t price , long long int time);
        void calTradeBigMove(int leaderLag ,int64_t price , int quantity ,long long int time);
        void calLeadLag(int leaderLag , int64_t bidPrice , int64_t askPrice);
        void adjustTrend();
        void calPairRatio(int leaderLag , double midPrice);
        void calBidAskRatio(int leaderLag , int64_t bidPrice , int64_t askPrice);
        void checkCalPairRatio(double ratio);
        void checkCalBidAskRatio(int leaderLag , double ratio);
        bool supportSkewQuotes(double & fairBuyPrice , double & fairSellPrice , long long int time);
        void bigMoveSkewQuotesABS(double & fairBuyPrice , double & fairSellPrice);
        void bigMoveSkewQuotesBPS(double & fairBuyPrice , double & fairSellPrice);
    public:

        bool stopPortfolioBySupport = false;
        int checkRatio = 0;

        double lagMidPrice = 0;
        double leadMidPrice = 0;
        int calPairRatioTicks = 0;
        double calPairRatioSum = 0;

        double leadBidAskDiff = 0;
        double lagBidAskDiff = 0;
        double leadCalBidAskRatioSum = 0;
        double leadCalBidAskRatioTicks = 0;
        double lagCalBidAskRatioSum = 0;
        double lagCalBidAskRatioTicks = 0;

        bool usePairLimitRatio = false;
        double maxPairRatio = 0;
        double minPairRatio = 0;
        bool useBidAskLimitRatio = false;
        double primaryMaxBidAskRatio = 0;
        double primaryMinBidAskRatio = 0;
        double secondaryMaxBidAskRatio = 0;
        double secondaryMinBidAskRatio = 0;
        bool useTrendSkew = false;
        int longTermTrendTicks = 100;
        int shortTermTrendTicks = 10;
        bool useBigMove = true;
        bool usePriceChangeBigMove = true;
        int priceChangeTimeWindow = 0;
        int priceChangeWindowTicks = 0;
        double intervalForNextTick = 0;
        double priceChangeBigMove = 0;
        bool onBigMoveCancelOrder = false;

        bool useTradeBigMove = true;
        int tradeTimeWindow = 0;
        double tradeBigMoveQty = 0;
        bool changeOrderDelayONBigmove = false;
        bool turnOffQuotingBigMove = true;
        int turnOffQuotingBigMoveForTime = 0;
        bool widenOffSetOnBigMove = true;
        int widenOffSetOnBigMoveForTime = 0;
        bool useWidenOffSetOnBigMoveABS = false;
        double widenOffSetOnBigMoveByBPS = 0;
        double widenOffSetOnBigMoveByABS = 0;

        std::string _symbol;
        Portfolio * _portfolio;

        bool leadLagCalFlag = false;



//calTrend
    int prevTrendPrice = 0;
    std::vector<int> shortTrendRollingVector;
    std::vector<int> longTrendRollingVector;
    double longRollingTrendSum = 0;
    double shortRollingTrendSum = 0;
    double longRollingTrendSlope = 0;
    double shortRollingTrendSlope = 0;
    int directionShortWindow = 0;
    int directionLongWindow = 0;
    int trendDiff = 0;
    int currentLongRollingIndex = 0;
    int currentshortRollingIndex = 0;

//bigMoveTrades
    std::deque<std::pair<long long int , int>> leadTradeQuantityDeque;
    std::deque<std::pair<long long int , int>> lagTradeQuantityDeque;
    int leadTradeQty = 0;
    int lagTradeQty = 0;

    std::vector<std::pair<long long int , int>> leadPriceMoveVec;
    std::vector<std::pair<long long int , int>> lagPriceMoveVec;
    int prevBigMovePriceLead = 0;
    long long int prevBigMoveTimeLead = 0;
    int leadMovePrice = 0;
    int lagMovePrice = 0;
    int prevBigMovePriceLag = 0;
    long long int prevBigMoveTimeLag = 0;
    double bigMoveDetectedChange =  0;
    bool bigMoveDetected = false;
    long long int bigMoveDetectedTime = 0;

    int leadPriceMoveVec_StartIndex = 0;
    int leadPriceMoveVec_CurrentIndex = 0;
    int lagPriceMoveVec_StartIndex = 0;
    int lagPriceMoveVec_CurrentIndex = 0;

    // Latency _latencyMap;
};


 

#endif
#ifndef __FAIRPRICE_BIDASK_H__
#define __FAIRPRICE_BIDASK_H__

#include "../fairPrice.h"


class fairPriceBidAsk : public fairPrice
{
    public : 
        fairPriceBidAsk();
        fairPriceBidAsk(std::string symbol , fairPriceParams fairPriceParams_);
        ~fairPriceBidAsk();
        void fairPriceBidAskRatio(int leaderLag , int64_t bidPrice , int64_t askPrice);
        void calculateFairPrice(int leaderLag , uint8_t type , int64_t price , int64_t bidPrice , int64_t askPrice);
        void calFairPriceUsingABS();
        void calFairPriceUsingBPS();
    public :
        std::string _symbol;
        int fairPriceCalType = 0;
        double leadBidPrice = 0; 
        double leadAskPrice = 0;
        double lagBidPrice = 0; 
        double lagAskPrice = 0;
        double bidRatio = 0;
        double askRatio = 0;
        std::vector<double> bidRatioVector;
        std::vector<double> askRatioVector;
        

        bool takeNextLagTick = false; 
        int positionCount = 0;
        int rollingWindow = 0;
        int ratioVectorIndex = 0;
        double bidRatioSum = 0;
        double askRatioSum = 0;

        double fairBidRatio = 0;
        double fairAskRatio = 0;

        bool useAbsoluteOffset = false;
        double fairPriceBidOffsetBPS = 0;
        double fairPriceAskOffsetBPS = 0;
        double fairPriceBidOffsetABS = 0;
        double fairPriceAskOffsetABS = 0;


};


#endif
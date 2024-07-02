#ifndef __FAIRPRICE_MIDPRICE_H__
#define __FAIRPRICE_MIDPRICE_H__

#include "../fairPrice.h"


class fairPriceMidPrice : public fairPrice
{
    public : 
        fairPriceMidPrice();
        fairPriceMidPrice(std::string symbol , fairPriceParams fairPriceParams_);
        ~fairPriceMidPrice();
        void fairPriceMid(int leaderLag , int64_t bidPrice , int64_t askPrice);
        void calculateFairPrice(int leaderLag , uint8_t type , int64_t price , int64_t bidPrice , int64_t askPrice);
        void calFairPriceUsingABS();
        void calFairPriceUsingBPS();
    public :
        std::string _symbol;
        int fairPriceCalType = 0;
        double leadMidPrice = 0;
        double lagMidPrice = 0;
        double midRatio = 0;
        std::vector<double> midRatioVector;
        

        bool takeNextLagTick = false; 
        int positionCount = 0;
        int rollingWindow = 0;
        int ratioVectorIndex = 0;
        double midRatioSum = 0;

        double fairMidRatio = 0;

        bool useAbsoluteOffset = false;
        double fairPriceBidOffsetBPS = 0;
        double fairPriceAskOffsetBPS = 0;
        double fairPriceBidOffsetABS = 0;
        double fairPriceAskOffsetABS = 0;

        double fairPrice = 0;
        

};


#endif
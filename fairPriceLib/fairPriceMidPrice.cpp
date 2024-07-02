#include "fairPriceMidPrice.h"

fairPriceMidPrice::fairPriceMidPrice(){}

fairPriceMidPrice::fairPriceMidPrice(std::string symbol , fairPriceParams fairPriceParams_)
{
    std::cout<<"FairPrice:MidPrice"<<std::endl;
    _symbol = symbol;
    rollingWindow = fairPriceParams_.rollingWindow;
    useAbsoluteOffset = fairPriceParams_.useAbsoluteOffset;
    fairPriceBidOffsetBPS = fairPriceParams_.fairPriceBidOffsetBPS;
    fairPriceAskOffsetBPS = fairPriceParams_.fairPriceAskOffsetBPS;
    fairPriceBidOffsetABS = fairPriceParams_.fairPriceBidOffsetABS;
    fairPriceAskOffsetABS = fairPriceParams_.fairPriceAskOffsetABS;

    midRatioVector = std::vector<double>(rollingWindow , 0);
   
}


fairPriceMidPrice::~fairPriceMidPrice(){

}


void fairPriceMidPrice::calculateFairPrice(int leaderLag , uint8_t type , int64_t price , int64_t bidPrice , int64_t askPrice)
{
    fairPriceMid(leaderLag , bidPrice ,askPrice);
    if(useAbsoluteOffset)
    {
        calFairPriceUsingABS();
    }
    else
    {
        calFairPriceUsingBPS();
    }
    #ifdef __DEBUG__
        std::cout<<fairPriceCalType<<"|"<<leaderLag<<"|"<<type<<"|"<<price<<"|"<<bidPrice<<"|"<<askPrice<<std::endl;
    #endif
}

////change
void fairPriceMidPrice::fairPriceMid(int leaderLag , int64_t bidPrice , int64_t askPrice)
{
    if(bidPrice >= askPrice){
        return;
    }

    if(leaderLag == LEADLAG::LEAD){
        if(leadMidPrice == ((bidPrice + askPrice) * 0.5)){
            return;
        }
        leadMidPrice = (bidPrice + askPrice) * 0.5;
        takeNextLagTick = true;
    }
    else if(leaderLag == LEADLAG::LAG && takeNextLagTick){
        if(lagMidPrice == ((bidPrice + askPrice) * 0.5)){
            return;
        }
        takeNextLagTick = false;
        lagMidPrice = (bidPrice + askPrice) * 0.5;
        midRatio = lagMidPrice / leadMidPrice ; 
        if(positionCount < rollingWindow){
            midRatioVector[ratioVectorIndex] = midRatio;
            positionCount++;
            ratioVectorIndex++;
            midRatioSum += midRatio;
            if(positionCount == rollingWindow){
                ratioVectorIndex = 0;
                fairMidRatio = midRatioSum / rollingWindow;
                #ifdef __DEBUG__
                    std::cout<<"fairMidRatio:"<<fairMidRatio<<std::endl;
                #endif
            }
        }
        else{
            midRatioSum = midRatioSum - midRatioVector[ratioVectorIndex] + midRatio;
            midRatioVector[ratioVectorIndex] = midRatio;
            ratioVectorIndex++;
            if(ratioVectorIndex == rollingWindow){
                ratioVectorIndex = 0;
            }
            fairMidRatio = midRatioSum / rollingWindow;
            #ifdef __DEBUG__
                    std::cout<<"fairMidRatio:"<<fairMidRatio<<std::endl;
            #endif
        }
    }
}


void fairPriceMidPrice::calFairPriceUsingABS()
{
    if(fairMidRatio == 0){
        return;
    }
    fairPrice = leadMidPrice * fairMidRatio;
    fairBuyPrice = fairPrice  - fairPriceBidOffsetABS; 
    fairSellPrice = fairPrice  + fairPriceAskOffsetABS;
}


void fairPriceMidPrice::calFairPriceUsingBPS()
{
    if(fairMidRatio == 0){
        return;
    }
    fairPrice = leadMidPrice * fairMidRatio;
    fairBuyPrice = fairPrice * ( 1 - fairPriceBidOffsetBPS ); 
    fairSellPrice = fairPrice * (1 + fairPriceAskOffsetBPS );  
}
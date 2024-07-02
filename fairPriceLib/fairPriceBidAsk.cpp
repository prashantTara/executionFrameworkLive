#include "fairPriceBidAsk.h"

fairPriceBidAsk::fairPriceBidAsk(){}

fairPriceBidAsk::fairPriceBidAsk(std::string symbol , fairPriceParams fairPriceParams_)
{
    std::cout<<"FairPrice:BidAsk"<<std::endl;

    rollingWindow = fairPriceParams_.rollingWindow;
    useAbsoluteOffset = fairPriceParams_.useAbsoluteOffset;
    fairPriceBidOffsetBPS = fairPriceParams_.fairPriceBidOffsetBPS;
    fairPriceAskOffsetBPS = fairPriceParams_.fairPriceAskOffsetBPS;
    fairPriceBidOffsetABS = fairPriceParams_.fairPriceBidOffsetABS;
    fairPriceAskOffsetABS = fairPriceParams_.fairPriceAskOffsetABS;

    bidRatioVector = std::vector<double>(rollingWindow , 0);
    askRatioVector = std::vector<double>(rollingWindow , 0);
}


fairPriceBidAsk::~fairPriceBidAsk(){

}

void fairPriceBidAsk::calculateFairPrice(int leaderLag , uint8_t type , int64_t price , int64_t bidPrice , int64_t askPrice)
{
    fairPriceBidAskRatio(leaderLag , bidPrice , askPrice);  
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

void fairPriceBidAsk::fairPriceBidAskRatio(int leaderLag , int64_t bidPrice , int64_t askPrice)
{
     if(bidPrice >= askPrice){
        return;
    }

    if(leaderLag == LEADLAG::LEAD){
        if(leadBidPrice == bidPrice && leadAskPrice == askPrice){
            return;
        }
        leadBidPrice = bidPrice;
        leadAskPrice = askPrice;
        takeNextLagTick = true;
    }
    else if(leaderLag == LEADLAG::LAG && takeNextLagTick){
        if(lagBidPrice == bidPrice && lagAskPrice == askPrice){
            return;
        }
        takeNextLagTick = false;
        lagBidPrice = bidPrice;
        lagAskPrice = askPrice;
        bidRatio = lagBidPrice / leadBidPrice ; 
        askRatio = lagAskPrice / leadAskPrice ; 
        // std::cout<<lagBidPrice<<" "<<leadBidPrice<<std::endl;
        // std::cout<<lagAskPrice<<" "<<leadAskPrice<<std::endl;
        if(positionCount < rollingWindow){
            bidRatioVector[ratioVectorIndex] = bidRatio;
            askRatioVector[ratioVectorIndex] = askRatio;
            positionCount++;
            ratioVectorIndex++;
            bidRatioSum += bidRatio;
            askRatioSum += askRatio;
            if(positionCount == rollingWindow){
                ratioVectorIndex = 0;
                fairBidRatio = bidRatioSum / rollingWindow;
                fairAskRatio = askRatioSum / rollingWindow;
                #ifdef __DEBUG__
                    std::cout<<"fairRatio:"<<fairBidRatio<<"|"<<fairAskRatio<<std::endl;
                #endif
            }
        }
        else{
            bidRatioSum = bidRatioSum - bidRatioVector[ratioVectorIndex] + bidRatio;
            askRatioSum = askRatioSum - askRatioVector[ratioVectorIndex] + askRatio;
            bidRatioVector[ratioVectorIndex] = bidRatio;
            askRatioVector[ratioVectorIndex] = askRatio;
            ratioVectorIndex++;
            if(ratioVectorIndex == rollingWindow){
                ratioVectorIndex = 0;
            }
            fairBidRatio = bidRatioSum / rollingWindow;
            fairAskRatio = askRatioSum / rollingWindow;
             #ifdef __DEBUG__
                    std::cout<<"fairRatio:"<<fairBidRatio<<"|"<<fairAskRatio<<std::endl;
            #endif
        }
    }
}

void fairPriceBidAsk::calFairPriceUsingABS()
{
    if(fairBidRatio == 0 || fairAskRatio == 0){
        return;
    }

    fairBuyPrice = (leadBidPrice * fairBidRatio) - fairPriceBidOffsetABS;
    fairSellPrice = (leadAskPrice * fairAskRatio) + fairPriceAskOffsetABS;
}


void fairPriceBidAsk::calFairPriceUsingBPS()
{
    if(fairBidRatio == 0 || fairAskRatio == 0){
        return;
    }
    
    fairBuyPrice = (leadBidPrice * fairBidRatio) * ( 1 - fairPriceBidOffsetBPS );
    fairSellPrice = (leadAskPrice * fairAskRatio) * (1 + fairPriceAskOffsetBPS );
}
#include "calSupport.h"

calSupport::calSupport(){}
calSupport::~calSupport(){}

calSupport::calSupport(std::string symbol , supportParams supportParams_ , Portfolio * portfolio_)
{
    _symbol = symbol;
    _portfolio = portfolio_;
    setParams(supportParams_);
    leadPriceMoveVec = std::vector<std::pair<long long int , int>>(priceChangeWindowTicks , {0,0});
    lagPriceMoveVec = std::vector<std::pair<long long int , int>>(priceChangeWindowTicks , {0,0});
    shortTrendRollingVector = std::vector<int>(shortTermTrendTicks, 0);
    longTrendRollingVector = std::vector<int>(longTermTrendTicks,0);

}

void calSupport::setParams(supportParams supportParams_)
{

    usePairLimitRatio = supportParams_.usePairLimitRatio;
    maxPairRatio = supportParams_.maxPairRatio;
    minPairRatio = supportParams_.minPairRatio;
    useBidAskLimitRatio = supportParams_.useBidAskLimitRatio;
    primaryMaxBidAskRatio = supportParams_.primaryMaxBidAskRatio;
    primaryMinBidAskRatio = supportParams_.primaryMinBidAskRatio;
    secondaryMaxBidAskRatio = supportParams_.secondaryMaxBidAskRatio;
    secondaryMinBidAskRatio = supportParams_.secondaryMinBidAskRatio;
    useTrendSkew = supportParams_.useTrendSkew;
    longTermTrendTicks = supportParams_.longTermTrendTicks;
    shortTermTrendTicks = supportParams_.shortTermTrendTicks;
    useBigMove = supportParams_.useBigMove;
    usePriceChangeBigMove = supportParams_.usePriceChangeBigMove;
    priceChangeTimeWindow = supportParams_.priceChangeTimeWindow * 1000;
    priceChangeWindowTicks = supportParams_.priceChangeWindowTicks * 1000;
    intervalForNextTick = double(priceChangeTimeWindow)/(double)priceChangeWindowTicks;
    priceChangeBigMove = supportParams_.priceChangeBigMove;
    onBigMoveCancelOrder = supportParams_.onBigMoveCancelOrder;
    useTradeBigMove = supportParams_.useTradeBigMove;
    tradeTimeWindow = supportParams_.tradeTimeWindow * 1000;
    tradeBigMoveQty = supportParams_.tradeBigMoveQty;
    changeOrderDelayONBigmove = supportParams_.changeOrderDelayONBigmove;
    turnOffQuotingBigMove = supportParams_.turnOffQuotingBigMove;
    turnOffQuotingBigMoveForTime = supportParams_.turnOffQuotingBigMoveForTime * 1000;
    widenOffSetOnBigMove = supportParams_.widenOffSetOnBigMove;
    widenOffSetOnBigMoveForTime = supportParams_.widenOffSetOnBigMoveForTime * 1000;
    useWidenOffSetOnBigMoveABS = supportParams_.useWidenOffSetOnBigMoveABS;
    widenOffSetOnBigMoveByBPS = supportParams_.widenOffSetOnBigMoveByBPS;
    widenOffSetOnBigMoveByABS = supportParams_.widenOffSetOnBigMoveByABS;    

}

void calSupport::calculateSupport(int leaderLag , uint8_t type , uint8_t side , int64_t price, int quantity , int64_t bidPrice , int64_t askPrice , long long int time)
{
    // _latencyMap.start("overall");

    if(bidPrice > askPrice){
        if(useBigMove && useTradeBigMove ){
             if(type == 'T'){
                if(side =='B'){
                    calTradeBigMove(leaderLag ,price , quantity , time );
                }
                else if(side =='S'){
                    calTradeBigMove(leaderLag ,price , -quantity , time );
                }
            }
        }
        // _latencyMap.end("overall");
        return;
    }

    double midPrice = (bidPrice + askPrice)*0.5;
    checkRatio = PORTFOLIO_RATIO_CHECK::DO_NOTHING;
    if(usePairLimitRatio){
        calPairRatio(leaderLag , midPrice);
    }
    if(useBidAskLimitRatio && checkRatio != PORTFOLIO_RATIO_CHECK::STOP){
        calBidAskRatio(leaderLag , bidPrice , askPrice);
    }
    if(checkRatio == PORTFOLIO_RATIO_CHECK::STOP)
    {
        if(_portfolio->portfolioStart == true){
            _portfolio->stopPortfolio();
            stopPortfolioBySupport = true;
        }
    }
    else if(checkRatio == PORTFOLIO_RATIO_CHECK::START){
        if(stopPortfolioBySupport == true){
            _portfolio->startPortfolio();
            stopPortfolioBySupport = false;
        }
    }

    if(leaderLag == LEADLAG::LEAD && useTrendSkew){
            calTrend(midPrice , time);
    }
    
    if(useBigMove){
        if(usePriceChangeBigMove)
        {
            calPriceBigMove(leaderLag ,midPrice , time);

        }
        if(useTradeBigMove)
        {
            if(type == 'T'){
                if(side =='B'){
                    calTradeBigMove(leaderLag ,price , quantity , time );
                }
                else if(side =='S'){
                    calTradeBigMove(leaderLag ,price , -quantity , time );
                }
            }
        }
    }
    // _latencyMap.end("overall");

    // if(leadLagCalFlag){
    //     calLeadLag(leaderLag ,bidPrice , askPrice);
    // }

}

void calSupport::calPairRatio(int leaderLag , double midPrice ){

    if(calPairRatioTicks == 1000){
        checkCalPairRatio(calPairRatioSum/calPairRatioTicks);
        calPairRatioTicks = 0;
        calPairRatioSum = 0;
    }
    if(leaderLag == LEADLAG::LEAD){
        leadMidPrice = midPrice;
    }
    if(leaderLag == LEADLAG::LAG){
        lagMidPrice = midPrice;
    }

    calPairRatioTicks++;
    calPairRatioSum+=(lagMidPrice / leadMidPrice);

}

void calSupport::calBidAskRatio(int leaderLag , int64_t bidPrice , int64_t askPrice){

    if(leaderLag == LEADLAG::LEAD){
        leadBidAskDiff = askPrice - bidPrice;
        leadCalBidAskRatioSum += leadBidAskDiff;
        leadCalBidAskRatioTicks++;
        if(leadCalBidAskRatioTicks == 1000)
        {
            checkCalBidAskRatio(leaderLag , leadCalBidAskRatioSum/(leadCalBidAskRatioTicks* ((bidPrice + askPrice)*0.5)));
            leadCalBidAskRatioSum = 0;
            leadCalBidAskRatioTicks = 0;
        }
    }
    else if(leaderLag == LEADLAG::LAG){
        lagBidAskDiff = askPrice - bidPrice;
        lagCalBidAskRatioSum += lagBidAskDiff;
        lagCalBidAskRatioTicks++;
        if(lagCalBidAskRatioTicks == 1000)
        {
            checkCalBidAskRatio(leaderLag , lagCalBidAskRatioSum/(lagCalBidAskRatioTicks* ((bidPrice + askPrice)*0.5)));
            lagCalBidAskRatioSum = 0;
            lagCalBidAskRatioTicks = 0;
        }
    }
}

void calSupport::checkCalPairRatio(double ratio)
{
    checkRatio = PORTFOLIO_RATIO_CHECK::START;
    if(ratio > maxPairRatio || ratio < minPairRatio){
#ifdef __DEBUG__
        std::cout<<ratio<<" "<<maxPairRatio<<" "<<minPairRatio<<std::endl;
#endif
        checkRatio = PORTFOLIO_RATIO_CHECK::STOP;
    }
}

void calSupport::checkCalBidAskRatio(int leaderLag , double ratio)
{
    checkRatio = PORTFOLIO_RATIO_CHECK::START;
    if(leaderLag == LEADLAG::LEAD){
        if(ratio > primaryMaxBidAskRatio || ratio < primaryMinBidAskRatio){
            checkRatio = PORTFOLIO_RATIO_CHECK::STOP;   
#ifdef __DEBUG__
            std::cout<<"LEADER:checkCalBidAskRatio:"<<ratio<<std::endl;
 #endif
            return;
        }
    }
    else if(leaderLag == LEADLAG::LEAD){
        if(ratio > secondaryMaxBidAskRatio || ratio < secondaryMinBidAskRatio){
            checkRatio = PORTFOLIO_RATIO_CHECK::STOP;    
#ifdef __DEBUG__
            std::cout<<"LAGGER:checkCalBidAskRatio:"<<ratio<<std::endl;
#endif
            return;
        }
    }
}

void calSupport::calTrend(int64_t price , long long int time){
    
    if(prevTrendPrice == 0){
        prevTrendPrice = price;
        return;
    }
    if(abs(prevTrendPrice - price) <= 5){
        return;
    }
    if(currentLongRollingIndex >= longTermTrendTicks){
        currentLongRollingIndex = 0;
    }
    if(currentshortRollingIndex >= shortTermTrendTicks){
        currentshortRollingIndex = 0;
    }

    trendDiff = price - prevTrendPrice;
    longRollingTrendSum = longRollingTrendSum + trendDiff - longTrendRollingVector[currentLongRollingIndex];
    shortRollingTrendSum = shortRollingTrendSum + trendDiff - shortTrendRollingVector[currentshortRollingIndex];
    longRollingTrendSlope = longRollingTrendSum / longTermTrendTicks;
    shortRollingTrendSlope = shortRollingTrendSum / shortTermTrendTicks;
    longTrendRollingVector[currentLongRollingIndex++] = trendDiff;
    shortTrendRollingVector[currentshortRollingIndex++] = trendDiff;
    prevTrendPrice = price;

    shortRollingTrendSlope = shortRollingTrendSlope/price; 
    longRollingTrendSlope = longRollingTrendSlope/price; 
    // std::cout<<time<<","<<price<<","<<longRollingTrendSlope<<","<<shortRollingTrendSlope<<",";
    // adjustTrend();
}


void calSupport::adjustTrend()
{        
    
}

bool calSupport::supportSkewQuotes(double & fairBuyPrice , double & fairSellPrice , long long int time)
{
    if(bigMoveDetected == true)
    {
        if(time - bigMoveDetectedTime < turnOffQuotingBigMoveForTime)
        {
            return false;
        }
        if(time - bigMoveDetectedTime < widenOffSetOnBigMoveForTime)
        {
            if(useWidenOffSetOnBigMoveABS)
            {
                bigMoveSkewQuotesABS(fairBuyPrice , fairSellPrice);
            }
            else{
                bigMoveSkewQuotesBPS(fairBuyPrice , fairSellPrice);
            }
        }
        else{
            bigMoveDetected = false;
        }
    }
    return true;
}


void calSupport::bigMoveSkewQuotesABS(double & fairBuyPrice , double & fairSellPrice)
{
    if(bigMoveDetectedChange < 0){
        fairBuyPrice -= widenOffSetOnBigMoveByABS;
    }
    else{
        fairSellPrice += widenOffSetOnBigMoveByABS;
    }
}

void calSupport::bigMoveSkewQuotesBPS(double & fairBuyPrice , double & fairSellPrice)
{
    if(bigMoveDetectedChange < 0){
        fairBuyPrice -= (fairBuyPrice*widenOffSetOnBigMoveByBPS);
    }
    else{
        fairSellPrice += (fairSellPrice*widenOffSetOnBigMoveByBPS);
    }
}


void calSupport::calPriceBigMove(int leaderLag ,int64_t price , long long int time)
{
    if(leaderLag == LEADLAG::LEAD)
    {
        if(prevBigMovePriceLead == 0){
            prevBigMovePriceLead = price;
            prevBigMoveTimeLead = time;
            return;
        }
        int diff = price - prevBigMovePriceLead;
        long long int diffTime = time - prevBigMoveTimeLead;

        if(diff ==  0 || diffTime < intervalForNextTick ){
            return;
        }

        prevBigMovePriceLead = price;
        prevBigMoveTimeLead = time;
        
        
        leadMovePrice += diff;
        leadPriceMoveVec[leadPriceMoveVec_CurrentIndex++] = std::make_pair(time,leadMovePrice);
        if(leadPriceMoveVec_CurrentIndex == priceChangeWindowTicks)
        {
            leadPriceMoveVec_CurrentIndex = 0;
        }

        auto stTime = leadPriceMoveVec[leadPriceMoveVec_CurrentIndex].first;
           while(time - stTime > priceChangeTimeWindow)
            {
                leadPriceMoveVec_StartIndex++;
                if(leadPriceMoveVec_StartIndex == priceChangeWindowTicks){
                    leadPriceMoveVec_StartIndex = 0;
                }
                if(leadPriceMoveVec_StartIndex == leadPriceMoveVec_CurrentIndex){
                        leadPriceMoveVec_CurrentIndex = 0;
                        leadPriceMoveVec_StartIndex = 0;
                        leadPriceMoveVec[leadPriceMoveVec_CurrentIndex++] = std::make_pair(time,leadMovePrice);
                        return;
                }
                stTime = leadPriceMoveVec[leadPriceMoveVec_StartIndex].first;
            }
        double currentWindowPriceChange = leadMovePrice - leadPriceMoveVec[leadPriceMoveVec_StartIndex].second;
        if(abs(currentWindowPriceChange)/price > priceChangeBigMove)
        {
            if(onBigMoveCancelOrder){
                _portfolio->cancelAllOpenOrders();
            }
            bigMoveDetected = true;
            bigMoveDetectedTime = time;
            bigMoveDetectedChange = currentWindowPriceChange;
#ifdef __DEBUG__
            std::cout<<"LEADER:PRICE_CHANGE_BIG_MOVE:"<<time<<":"<<currentWindowPriceChange<<"|TotalPriceChange:"<<leadMovePrice<<std::endl;
#endif
        }
    }
    else{
        if(prevBigMovePriceLag == 0){
            prevBigMovePriceLag = price;
            prevBigMoveTimeLag = time;
            return;
        }
        int diff = price - prevBigMovePriceLag;
        long long int diffTime = time - prevBigMoveTimeLag;

        if(diff ==  0 || diffTime < intervalForNextTick ){
            return;
        }
        prevBigMovePriceLag = price;
        prevBigMoveTimeLag = time;
        
        
        lagMovePrice += (diff);
        lagPriceMoveVec[lagPriceMoveVec_CurrentIndex++] = std::make_pair(time,lagMovePrice);
        if(lagPriceMoveVec_CurrentIndex == priceChangeWindowTicks)
        {
            lagPriceMoveVec_CurrentIndex = 0;
        }
        auto stTime = lagPriceMoveVec[lagPriceMoveVec_StartIndex].first;
        while(time - stTime > priceChangeTimeWindow)
        {
            lagPriceMoveVec_StartIndex++;
            if(lagPriceMoveVec_StartIndex == priceChangeWindowTicks)
            {
                lagPriceMoveVec_StartIndex = 0;
            }
            if(lagPriceMoveVec_StartIndex == lagPriceMoveVec_CurrentIndex){
                lagPriceMoveVec_StartIndex = 0;
                lagPriceMoveVec_CurrentIndex = 0;
                lagPriceMoveVec[lagPriceMoveVec_CurrentIndex++] = std::make_pair(time,lagMovePrice);
                return;
            }
            stTime = lagPriceMoveVec[lagPriceMoveVec_StartIndex].first;
        }
        double currentWindowPriceChange = lagMovePrice - lagPriceMoveVec[lagPriceMoveVec_StartIndex].second;
        if(abs(currentWindowPriceChange)/price > priceChangeBigMove)
        {
            if(onBigMoveCancelOrder){
                _portfolio->cancelAllOpenOrders();
            }
            bigMoveDetected = true;
            bigMoveDetectedTime = time;
            bigMoveDetectedChange = currentWindowPriceChange;
#ifdef __DEBUG__
            std::cout<<"LAGGER:PRICE_CHANGE_BIG_MOVE:"<<time<<":"<<currentWindowPriceChange<<"|TotalPriceChange:"<<lagMovePrice<<std::endl;
#endif
        }
    }

}

void calSupport::calTradeBigMove(int leaderLag ,int64_t price , int quantity ,long long int time)
{
    if(leaderLag == LEADLAG::LEAD)
    {
        leadTradeQty += quantity;
        leadTradeQuantityDeque.push_back(make_pair(time,leadTradeQty));

        auto it = leadTradeQuantityDeque.front();
        auto stTime = (it).first;
        while(time - stTime > tradeTimeWindow)
        {
            leadTradeQuantityDeque.pop_front();
            it = leadTradeQuantityDeque.front();
            stTime = (it).first;
        }
        int currentWindowTradeQuantity = leadTradeQty - (it).second;
        if(abs(currentWindowTradeQuantity) > tradeBigMoveQty)
        {
            if(onBigMoveCancelOrder){
                _portfolio->cancelAllOpenOrders();
            }
            bigMoveDetected = true;
            bigMoveDetectedTime = time;
            bigMoveDetectedChange = currentWindowTradeQuantity;

#ifdef __DEBUG__
            std::cout<<"LEADER:BIG_MOVE:"<<time<<":"<<currentWindowTradeQuantity<<"|TotalTradeLot:"<<leadTradeQty<<std::endl;
#endif
        }
    }
    else{
        lagTradeQty += quantity;
        lagTradeQuantityDeque.push_back(make_pair(time , lagTradeQty));

        auto it = lagTradeQuantityDeque.front();
        auto stTime =  (it).first;
        while(time - stTime > tradeTimeWindow)
        {
            lagTradeQuantityDeque.pop_front();
            it = lagTradeQuantityDeque.front();
            stTime = (it).first;
        }
        int currentWindowTradeQuantity = lagTradeQty - (it).second;
        if(abs(currentWindowTradeQuantity) > tradeBigMoveQty)
        {
            if(onBigMoveCancelOrder){
                _portfolio->cancelAllOpenOrders();
            }
            bigMoveDetected = true;
            bigMoveDetectedTime = time;
            bigMoveDetectedChange = currentWindowTradeQuantity;

#ifdef __DEBUG__
            std::cout<<"LAGGER:BIG_MOVE:"<<time<<":"<<currentWindowTradeQuantity<<"|TotalTradeLot:"<<leadTradeQty<<std::endl;
#endif
        }
    }
}
        

void calSupport::calLeadLag(int leaderLag , int64_t bidPrice , int64_t askPrice)
{
    
}
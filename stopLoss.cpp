#include "stopLoss.h"

stopLoss::stopLoss(){}

stopLoss::stopLoss(stopLossParams stopLossParams_ , int lotSize , Portfolio * portfolio_)
{
        std::cout<<"STOPLOSS:ACTIVE"<<std::endl;
        _portfolio = portfolio_;
        _lotSize = lotSize;
        setParams(stopLossParams_);
}


stopLoss::~stopLoss(){}

void stopLoss::setParams(stopLossParams stopLossParams_){
        slAcThr = stopLossParams_.slAcThr / _lotSize;
        trailStopLoss = stopLossParams_.trailStopLoss / 100;
        maxPortfolioLoss = stopLossParams_.maxPortfolioLoss / _lotSize;
        maxTradeLoss = stopLossParams_.maxTradeLoss / _lotSize;
        useTradeJumper = stopLossParams_.useTradeJumper;
        bookCrossThreshold = stopLossParams_.bookCrossThreshold;
        bookLevel = stopLossParams_.bookLevel;
        orderDivisionFactor = stopLossParams_.orderDivisionFactor;
        useAvgBook = stopLossParams_.useAvgBook;
        useAvgBookThresholdABS = stopLossParams_.useAvgBookThresholdABS;

        avgOrderToBookThreshold_BPS = stopLossParams_.avgOrderToBookThreshold_BPS;
        avgOrderToBookThreshold_ABS = stopLossParams_.avgOrderToBookThreshold_ABS;
}

void stopLoss::updatePositionAndValue(int position_ , double buyPrice , double sellPrice)
{
        position += position_ ;
        curBuyValue += (buyPrice * position_ ) ;
        buyValue += (buyPrice * position_ ) ;
        curSellValue += (sellPrice * position_ ) ;
        sellValue += (sellPrice * position_ ) ;
        if(position == 0){
                curBuyValue = 0;
                curSellValue = 0;
                slActivated = false;
                stopLossWait = false;
                curTradePnl = 0;
                lastPnl = 0;
        }
}

bool stopLoss::checkStopLoss(int64_t bidPrice , int64_t askPrice)
{
        lagBidPrice = bidPrice;
        lagAskPrice = askPrice;
        if(position == 0){
                slActivated = false;
                stopLossWait = false;
                return false;
        }

        if(stopLossWait == true){
                stopLossSquareOff();
                return true;
        }
        int64_t midPrice = (lagBidPrice + lagAskPrice) * 0.5;
        curTradePnl = curSellValue - curBuyValue + (midPrice*position);
        currentPnl = sellValue - buyValue + (midPrice*position);
#ifdef debugStrat
        std::cout<<"PNL|SYM:"<<_portfolio->laggerSymbol<<"|currentPnl:"<<currentPnl<<"|curTradePnl:"<<curTradePnl<<"|Postion:"<<position<<std::endl;
#endif
        if( currentPnl < maxPortfolioLoss)
        {
                std::cout<<"SQ_1_MAX_PORTFOLIO_LOSS_RREACHED:"<<_portfolio->laggerSymbol<<std::endl;
                stopLossWait = true;
                stopLossSquareOff();
                sqauredOff= true;
                return true;
        } 
        else if(curTradePnl < maxTradeLoss)
        {
                std::cout<<"SQ_2_CURRENT_TRADE_SL_HIT:"<<_portfolio->laggerSymbol<<std::endl;
                stopLossWait = true;
                stopLossSquareOff();
                return true;
        }   
        else
        {
                if(slActivated == false && curTradePnl > slAcThr){
                        slActivated = true;
                }          
                if(lastPnl < curTradePnl)
                {
                        lastPnl = curTradePnl;
                        return false;
                }
                else if(slActivated && curTradePnl < 0 ){
                        slActivated = false;
                }
                else if(slActivated && curTradePnl - lastPnl < (lastPnl*trailStopLoss)){
                        std::cout<<"SQ_3_TRAILING_SL_HIT:"<<_portfolio->laggerSymbol<<std::endl;
                        stopLossSquareOff();
                        stopLossWait = true;
                        lastPnl = 0;
                        return true;
                }  
        }
        return false;
}

void stopLoss::calBuyBookAvg()
{
        buyBookAvgPrice = 0;
        int weight = 0;
        for(int i = 0; i < bookLevel ; i++)
        {
                if(_portfolio->lagBooklvl[i]._quantity > 0)
                {
                        buyBookAvgPrice += _portfolio->lagBooklvl[i]._price;
                        weight++;
                }
        }
        buyBookAvgPrice = buyBookAvgPrice/weight;
}

void stopLoss::calSellBookAvg()
{
        sellBookAvgPrice = 0;
        int weight = 0;
        for(int i = 5; i < 5 + bookLevel ; i++)
        {
                if(_portfolio->lagBooklvl[i]._quantity > 0)
                {
                        sellBookAvgPrice += _portfolio->lagBooklvl[i]._price;
                        weight++;
                }
        }
        sellBookAvgPrice = sellBookAvgPrice/weight;
}

int stopLoss::priceCheck(int price , bool type){
    if(price%5 != 0){
        int rem = price % 5;
        if(type==0){
            return (price - rem); 
        }                
        else{
            return (price + 5 - rem);
        }       
    }
    return price;
}

void stopLoss::calFairBuyPrice()
{
        if(lagAskPrice - lagBidPrice <= ((lagBidPrice + lagAskPrice)/2)*bookCrossThreshold){                
                fairBuyPrice = lagAskPrice;
        }
        else{
                if(useAvgBook){
                        calBuyBookAvg();
                        if(fairBuyPrice != lagBidPrice)
                        {
                                fairBuyPrice = lagBidPrice + 5;
                        }
                        if(useAvgBookThresholdABS)
                        {
                                if(fairBuyPrice >  buyBookAvgPrice + avgOrderToBookThreshold_ABS)
                                {
                                        fairBuyPrice = buyBookAvgPrice + avgOrderToBookThreshold_ABS;
                                        fairBuyPrice = priceCheck(fairBuyPrice , 1);
                                }
                        }
                        else
                        {
                                if(fairBuyPrice >  buyBookAvgPrice *(1 + avgOrderToBookThreshold_BPS))
                                {
                                        fairBuyPrice = buyBookAvgPrice * (1 + avgOrderToBookThreshold_BPS);
                                        fairBuyPrice = priceCheck(fairBuyPrice , 1);
                                }

                        } 
                }
                else{
                        if(fairBuyPrice == 0)
                        {
                                fairBuyPrice = lagBidPrice + 5;  
                        }
                        if(fairBuyPrice <= _portfolio->lagBooklvl[2]._price || fairBuyPrice >= lagBidPrice){
                                fairBuyPrice = lagBidPrice + 5;
                                if(fairBuyPrice > _portfolio->lagBooklvl[1]._price + 10)
                                {
                                        fairBuyPrice = _portfolio->lagBooklvl[1]._price + 5;
                                }
                        }
                }
        }
}

void stopLoss::calFairSellPrice()
{
        if(lagAskPrice - lagBidPrice <= ((lagBidPrice + lagAskPrice)/2)*bookCrossThreshold){
                fairSellPrice = lagBidPrice;
        }
        else
        {
                if(useAvgBook){
                        calSellBookAvg();
                        if(fairSellPrice != lagAskPrice)
                        {
                                fairSellPrice = lagAskPrice - 5;
                        }
                        if(useAvgBookThresholdABS)
                        {
                                if(fairSellPrice <  sellBookAvgPrice - avgOrderToBookThreshold_ABS)
                                {
                                        fairSellPrice = sellBookAvgPrice - avgOrderToBookThreshold_ABS;
                                        fairSellPrice = priceCheck(fairSellPrice , 0);
                                }
                        }
                        else
                        {
                                if(fairSellPrice <  sellBookAvgPrice *(1 - avgOrderToBookThreshold_BPS))
                                {
                                        fairSellPrice = sellBookAvgPrice * (1 - avgOrderToBookThreshold_BPS);
                                        fairSellPrice = priceCheck(fairSellPrice , 0);
                                }    
                        }
                }
                else{
                        if(fairSellPrice == 0)
                        {
                                fairSellPrice = lagAskPrice - 5;  
                        }
                        if(fairSellPrice >= _portfolio->lagBooklvl[7]._price || fairSellPrice <= lagAskPrice)
                        {
                                fairSellPrice = lagAskPrice - 5;
                                if(fairSellPrice < _portfolio->lagBooklvl[6]._price - 10)
                                {
                                        fairSellPrice = _portfolio->lagBooklvl[6]._price - 5;
                                }
                        }
                }
        }
}

void stopLoss::setPrevFairBuyPrice(){
        lastBuyOrderFairPrice = fairBuyPrice;
}

void stopLoss::setPrevFairSellPrice(){
        lastSellOrderFairPrice = fairSellPrice;
}

void stopLoss::stopLossSquareOff(int64_t bidPrice , int64_t askPrice)
{
        lagBidPrice = bidPrice;
        lagAskPrice = askPrice;
        if(position == 0 )
        {
                return;
        }
        _portfolio->squareOffQuoter();
        if(useTradeJumper == true && sqauredOffTimePassed == true && clearPositionTimePassed == false)
        {
                if(position > 0)
                {
                        calFairSellPrice();
                        if(fairSellPrice != lastSellOrderFairPrice)
                        {
                                quoteSellOrder();
                        }
                }
                if(position < 0)
                {
                        calFairBuyPrice();
                        if(fairBuyPrice != lastBuyOrderFairPrice)
                        {
                                quoteBuyOrder();
                        }
                }
        }
        else
        {
                if(position > 0)
                {
                        fairSellPrice = lagBidPrice;
                        quoteSellOrder();
                }
                if(position < 0)
                {
                        fairBuyPrice = lagAskPrice;
                        quoteBuyOrder();
                }
        }
}

void stopLoss::stopLossSquareOff()
{
        if(position == 0 )
        {
                return;
        }
        _portfolio->squareOffQuoter();
        if(position > 0)
        {
                fairSellPrice = lagBidPrice;
                quoteSellOrder();
        }
        if(position < 0)
        {
                fairBuyPrice = lagAskPrice;
                quoteBuyOrder();
        }
}

void stopLoss::quoteBuyOrder()
{        
        if(wait!=0)
        {
                return;
        }
        buyFillQuantity = buyOrderHandler.second.fillQuantity;
        {
                int quoteQty = std::min(-position , orderDivisionFactor);
                if(lastBuyOrderId == 0)
                {
                        lastBuyOrderId = OrderIDStartingNumber;
                        _portfolio->sendNewStopLoss('B', quoteQty,fairBuyPrice,OrderIDStartingNumber++);
                        setPrevFairBuyPrice(); 
                        wait++;
                }
                else 
                {
                        _portfolio->sendModify('B', quoteQty ,fairBuyPrice,lastBuyOrderId ,buyFillQuantity);
                        setPrevFairBuyPrice(); 
                        wait++;
                }
        }           
}

void stopLoss::quoteSellOrder()
{        
        if(wait!=0)
        {
                return;
        }
        sellFillQuantity = sellOrderHandler.second.fillQuantity;
        {
                int quoteQty = std::min(position , orderDivisionFactor);
                if(lastSellOrderId == 0)
                {
                        lastSellOrderId = OrderIDStartingNumber;
                        _portfolio->sendNewStopLoss('S', quoteQty,fairSellPrice,OrderIDStartingNumber++ );
                        setPrevFairSellPrice();
                        wait++;
                }
                else
                {
                        _portfolio->sendModify('S', quoteQty,fairSellPrice,lastSellOrderId ,sellFillQuantity); 
                        setPrevFairSellPrice();
                        wait++;
                }
        }           
}

void stopLoss::onResponse(char responseType, long long int orderID, std::string symbol, char side, int fillQuantity,int price, int16_t errorCode, char orderType, int openQuantity)
{
        responseType = std::toupper(responseType);
        if(side == 'B'){
                if(buyOrderHandler.first == orderID){
                        buyOrderHandler.second.fillQuantity += fillQuantity; 
                }
                else{
                        buyOrderHandler.first = orderID;
                        buyOrderHandler.second.fillQuantity = fillQuantity; 
                }
        }
        else if(side == 'S'){
                if(sellOrderHandler.first == orderID){
                        sellOrderHandler.second.fillQuantity += fillQuantity; 
                }
                else{
                        sellOrderHandler.first = orderID;
                        sellOrderHandler.second.fillQuantity = fillQuantity; 
                }
        }
        if(responseType == 'T' && side =='B'){
                std::cout<<"STOPLOSS:"<<"Trade:"<<symbol<<"|"<<side<<"|"<<fillQuantity<<"|"<<price<<"|"<<openQuantity<<std::endl;
                if(openQuantity == 0){
                        lastBuyOrderId = 0;
                        fairBuyPrice = 0;
                        lastBuyOrderFairPrice = 0;
                }
        }
        else if(responseType == 'T' && side =='S'){
                std::cout<<"STOPLOSS:"<<"Trade:"<<symbol<<"|"<<side<<"|"<<fillQuantity<<"|"<<price<<"|"<<openQuantity<<std::endl;
                if(openQuantity == 0){
                        lastSellOrderId = 0;
                        fairSellPrice = 0;
                        lastSellOrderFairPrice = 0;
                }
        }
        else if(responseType == 'R' && orderType == 'R'){
                std::cout<<"ORDER_REJECTED_INTERNAL_ERROR_CODE:"<<errorCode<<std::endl;
                return;
        }
        else if(responseType == 'R' && side =='S' && orderType == 'N'){
                std::cout<<"STOPLOSS:|REJECT_SELL_ORDER_NEW:"<<errorCode<<"|Symbol:"<<symbol<<"|Price:"<<price<<"|ORDER_ID:"<<orderID<<"|openQuantity:"<<openQuantity<<std::endl;
                lastSellOrderId = 0;
                sellOrderHandler.first = 0;

        }
        else if(responseType == 'R' && side =='B' && orderType == 'N'){
                std::cout<<"STOPLOSS|REJECT_BUY_ORDER_NEW:"<<errorCode<<"|Symbol:"<<symbol<<"|Price:"<<price<<"|ORDER_ID:"<<orderID<<"|openQuantity:"<<openQuantity<<std::endl;
                lastBuyOrderId = 0;
                buyOrderHandler.first = 0;

        }
        else if(responseType == 'R' && side =='B' && orderType == 'M'){
                std::cout<<"STOPLOSS|REJECT_BUY_ORDER_MODIFY:"<<errorCode<<"|Price:"<<price<<"|ORDER_ID:"<<orderID<<"|sellFillQuantity:"<<buyOrderHandler.second.fillQuantity<<std::endl;
                // lastBuyOrderId = 0;

        }
        else if(responseType == 'R' && side =='S' && orderType == 'M'){
                std::cout<<"STOPLOSS|REJECT_SELL_ORDER_MODIFY:"<<errorCode<<"|Price:"<<price<<"|ORDER_ID:"<<orderID<<"|sellFillQuantity:"<<sellOrderHandler.second.fillQuantity<<std::endl;
                // lastSellOrderId = 0;
        }
        else if(responseType == 'P' && side =='B'){
        }

        else if(responseType == 'P' && side =='S'){
        }
        else if(responseType == 'X' && side == 'B' ){
                lastBuyOrderId = 0;
                buyOrderHandler.first = 0;        
        }
        else if(responseType == 'X' && side == 'S' ){
                lastSellOrderId = 0;
                sellOrderHandler.first = 0;
        }

        if(side == 'B' &&responseType != 'T' && responseType != 'P' ){
                wait--;
        }

        if(side == 'S' &&responseType != 'T' && responseType != 'P' ){
                wait--;
        }
}

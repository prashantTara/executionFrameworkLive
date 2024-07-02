#include "Quoter.h"

Quoter::Quoter(){}
Quoter::~Quoter(){}

Quoter::Quoter(std::string symbol , std::string quoterType , quoterParams quoterParams_ , Portfolio * portfolio_)
{
        _portfolio = portfolio_;
        _symbol = symbol;
        _quoterType = quoterType;
        setParams(quoterParams_);

}

void Quoter::setParams(quoterParams quoterParams_)
{
        useAbsoluteOffset = quoterParams_.useAbsoluteOffset;
        bidSpread_BPS = quoterParams_.bidSpread_BPS;
        askSpread_BPS = quoterParams_.askSpread_BPS;
        bidSpread_ABS = quoterParams_.bidSpread_ABS;
        askSpread_ABS = quoterParams_.askSpread_ABS;
        useAbsoluteModifyThreshold = quoterParams_.useAbsoluteModifyThreshold;
        quoteModifyPassiveThreshold_Bid_BPS = quoterParams_.quoteModifyPassiveThreshold_Bid_BPS;
        quoteModifyPassiveThreshold_Ask_BPS = quoterParams_.quoteModifyPassiveThreshold_Ask_BPS;
        quoteModifyAggresiveThreshold_Bid_BPS = quoterParams_.quoteModifyAggresiveThreshold_Bid_BPS;
        quoteModifyAggresiveThreshold_Ask_BPS = quoterParams_.quoteModifyAggresiveThreshold_Ask_BPS;
        quoteModifyPassiveThreshold_Bid_ABS = quoterParams_.quoteModifyPassiveThreshold_Bid_ABS;
        quoteModifyPassiveThreshold_Ask_ABS = quoterParams_.quoteModifyPassiveThreshold_Ask_ABS;
        quoteModifyAggresiveThreshold_Bid_ABS = quoterParams_.quoteModifyAggresiveThreshold_Bid_ABS;
        quoteModifyAggresiveThreshold_Ask_ABS = quoterParams_.quoteModifyAggresiveThreshold_Ask_ABS;
        quoterSwitch = quoterParams_.quoterSwitch;
        buy_switch = quoterParams_.buy_switch;
        original_buy_switch = quoterParams_.buy_switch;
        sell_switch = quoterParams_.sell_switch;
        original_sell_switch = quoterParams_.sell_switch;
        originalBuyQuoteQty = quoterParams_.buyQuoteQty;
        originalSellQuoteQty = quoterParams_.sellQuoteQty;
        buyQuoteQty = quoterParams_.buyQuoteQty;
        sellQuoteQty = quoterParams_.sellQuoteQty;
        quoteQtySkewFactorBuy = quoterParams_.quoteQtySkewFactorBuy;
        quoteQtySkewFactorSell = quoterParams_.quoteQtySkewFactorSell;
        useLongTrendSkew = quoterParams_.useLongTrendSkew;
        useShortTrendSkew = quoterParams_.useShortTrendSkew;
        useTrendOffsetABS = quoterParams_.useTrendOffsetABS;
        trendSkewInversion = quoterParams_.trendSkewInversion;
        longTrendIncreaseSkewOffset_BPS = quoterParams_.longTrendIncreaseSkewOffset_BPS;
        longTrendDecreaseSkewOffset_BPS = quoterParams_.longTrendDecreaseSkewOffset_BPS;
        shortTrendIncreaseSkewOffset_BPS = quoterParams_.shortTrendIncreaseSkewOffset_BPS;
        shortTrendDecreaseSkewOffset_BPS = quoterParams_.shortTrendDecreaseSkewOffset_BPS;
        longTrendIncreaseSkewOffset_ABS = quoterParams_.longTrendIncreaseSkewOffset_ABS;
        longTrendDecreaseSkewOffset_ABS = quoterParams_.longTrendDecreaseSkewOffset_ABS;
        shortTrendIncreaseSkewOffset_ABS = quoterParams_.shortTrendIncreaseSkewOffset_ABS;
        shortTrendDecreaseSkewOffset_ABS = quoterParams_.shortTrendDecreaseSkewOffset_ABS;
        useLtpFilter = quoterParams_.useLtpFilter;       
        maxShortTrendShifts = quoterParams_.maxShortTrendShifts;
        maxLongTrendShifts = quoterParams_.maxLongTrendShifts;
        longTrendQualifyPercent = quoterParams_.longTrendQualifyPercent;
        shortTrendQualifyPercent = quoterParams_.shortTrendQualifyPercent;

}


void Quoter::calPnlSkew(){
    
}

void Quoter::calQuotes(int leaderLag , double fairBuyPrice_ , double fairSellPrice_ , int64_t bidPrice , int64_t askPrice ){

        if(pnlSkewOn){
            calPnlSkew();
        }
        if(leaderLag == LEADLAG::LAG ){
            lagBidPrice = bidPrice;
            lagAskPrice = askPrice;
        }
        if(lagBidPrice == 0 || lagAskPrice == 0)
        {
                return;
        }
        if(useAbsoluteOffset)
        {
                calQuotesABS(fairBuyPrice_ , fairSellPrice_);
                if(useLongTrendSkew || useShortTrendSkew)
                {
                       trendSkewQuotesABS(); 
                }
        }
        else
        {
                calQuotesBPS(fairBuyPrice_ , fairSellPrice_);
                if(useLongTrendSkew || useShortTrendSkew)
                {
                       trendSkewQuotesBPS(); 
                }
        }
        fairBuyPrice = priceCheck(fairBuyPrice , 0);
        fairSellPrice = priceCheck(fairSellPrice , 1);
        // std::cout<<"Before"<<_quoterType<<":"<<fairBuyPrice_<<" "<<fairBuyPrice<<" "<<fairSellPrice<<" "<<fairSellPrice_<<std::endl;
        adjustQuoteWithBook();
        // std::cout<<"After"<<_quoterType<<":"<<fairBuyPrice<<" "<<fairSellPrice<<std::endl;
        
        // return;
        quoteOrders();

}

void Quoter::calQuotesABS(double fairBuyPrice_ , double fairSellPrice_)
{
        fairBuyPrice = fairBuyPrice_ - bidSpread_ABS;
        fairSellPrice = fairSellPrice_  + askSpread_ABS;
}

void Quoter::calQuotesBPS(double fairBuyPrice_ , double fairSellPrice_)
{
        fairBuyPrice = fairBuyPrice_ * (1 - bidSpread_BPS);
        fairSellPrice = fairSellPrice_ * (1 + askSpread_BPS);
}

void Quoter::trendSkewQuotesABS()
{
        directionShortWindow = _portfolio->getDirectionShortWindow();
        directionLongWindow = _portfolio->getDirectionLongWindow();
        // std::cout<<_quoterType<<"|ABS:directionShort:"<<directionShortWindow<<"|directionLong:"<<directionLongWindow<<std::endl;
        if(trendSkewInversion)
        {
                if(useShortTrendSkew){
                        int times = abs(directionShortWindow) / shortTrendQualifyPercent;
                        times = std::min(times , maxShortTrendShifts);
                        do{
                                if(directionShortWindow > 0)
                                {
                                        fairBuyPrice -= shortTrendDecreaseSkewOffset_ABS;
                                        fairSellPrice -= shortTrendDecreaseSkewOffset_ABS;;
                                }
                                else if(directionShortWindow < 0)
                                {
                                        fairBuyPrice += shortTrendIncreaseSkewOffset_ABS;
                                        fairSellPrice += shortTrendIncreaseSkewOffset_ABS;
                                }
                        } while(--times > 0);
                }
                if(useLongTrendSkew){
                        int times = abs(directionLongWindow) / longTrendQualifyPercent;
                        times = std::min(times , maxLongTrendShifts);
                        do{
                                if(directionLongWindow > 0)
                                {
                                        fairBuyPrice -= longTrendDecreaseSkewOffset_ABS;
                                        fairSellPrice -= longTrendDecreaseSkewOffset_ABS;;
                                }
                                else if(directionLongWindow < 0)
                                {
                                        fairBuyPrice += longTrendIncreaseSkewOffset_ABS;
                                        fairSellPrice += longTrendIncreaseSkewOffset_ABS;
                                }
                        } while(--times > 0);
                }
        }
        else
        {
                if(useShortTrendSkew){
                        int times = abs(directionShortWindow) / shortTrendQualifyPercent;
                        times = std::min(times , maxShortTrendShifts);
                        do{
                                if(directionShortWindow > 0)
                                {
                                        fairBuyPrice += shortTrendIncreaseSkewOffset_ABS;
                                        fairSellPrice += shortTrendIncreaseSkewOffset_ABS;
                                }
                                else if(directionShortWindow < 0)
                                {
                                        fairBuyPrice -= shortTrendDecreaseSkewOffset_ABS;
                                        fairSellPrice -= shortTrendDecreaseSkewOffset_ABS;
                                }
                        } while(--times > 0);
                }
                if(useLongTrendSkew){
                        int times = abs(directionLongWindow) / longTrendQualifyPercent;
                        times = std::min(times , maxLongTrendShifts);
                        do{
                                if(directionLongWindow > 0)
                                {
                                        fairBuyPrice += longTrendIncreaseSkewOffset_ABS;
                                        fairSellPrice += longTrendIncreaseSkewOffset_ABS;
                                }
                                else if(directionLongWindow < 0)
                                {
                                        fairBuyPrice -= longTrendDecreaseSkewOffset_ABS;
                                        fairSellPrice -= longTrendDecreaseSkewOffset_ABS;
                                }
                        } while(--times > 0);
                }
        }
}

void Quoter::trendSkewQuotesBPS()
{
        directionShortWindow = _portfolio->getDirectionShortWindow();
        directionLongWindow = _portfolio->getDirectionLongWindow();
        // std::cout<<_quoterType<<"|BPS:directionShort:"<<directionShortWindow<<"|directionLong:"<<directionLongWindow<<std::endl;
        // std::cout<<"trendSkewInversion:"<<trendSkewInversion<<std::endl;
        if(trendSkewInversion)
        {
                if(useShortTrendSkew)
                {
                        int times = abs(directionShortWindow) / shortTrendQualifyPercent;
                        times = std::min(times , maxShortTrendShifts);
                        do{
                                if(directionShortWindow > 0)
                                {
                                        fairBuyPrice -= fairBuyPrice*shortTrendDecreaseSkewOffset_BPS;
                                        fairSellPrice -= fairSellPrice*shortTrendDecreaseSkewOffset_BPS;
                                }
                                else if(directionShortWindow < 0)
                                {
                                        fairBuyPrice += fairBuyPrice*shortTrendIncreaseSkewOffset_BPS;
                                        fairSellPrice += fairSellPrice*shortTrendIncreaseSkewOffset_BPS;
                                }
                        } while(--times > 0);
                }
                if(useLongTrendSkew){
                        int times = abs(directionLongWindow) / longTrendQualifyPercent;
                        times = std::min(times , maxLongTrendShifts);
                        do{
                                if(directionLongWindow > 0)
                                {
                                        fairBuyPrice -= fairBuyPrice*longTrendDecreaseSkewOffset_BPS;
                                        fairSellPrice -= fairSellPrice*longTrendDecreaseSkewOffset_BPS;
                                }
                                else if(directionLongWindow < 0)
                                {
                                        fairBuyPrice += fairBuyPrice*longTrendIncreaseSkewOffset_BPS;
                                        fairSellPrice += fairSellPrice*longTrendIncreaseSkewOffset_BPS;
                                }
                        } while(--times > 0);
                }
        }
        else
        {
                if(useShortTrendSkew){
                        int times = abs(directionShortWindow) / shortTrendQualifyPercent;
                        times = std::min(times , maxShortTrendShifts);
                        do{
                                if(directionShortWindow > 0)
                                {
                                        fairBuyPrice += fairBuyPrice*shortTrendIncreaseSkewOffset_BPS;
                                        fairSellPrice += fairSellPrice*shortTrendIncreaseSkewOffset_BPS;
                                }
                                else if(directionShortWindow < 0)
                                {
                                        fairBuyPrice -= fairBuyPrice*shortTrendDecreaseSkewOffset_BPS;
                                        fairSellPrice -= fairSellPrice*shortTrendDecreaseSkewOffset_BPS;
                                }
                        } while(--times > 0);
                }
                if(useLongTrendSkew){
                        int times = abs(directionLongWindow) / longTrendQualifyPercent;
                        times = std::min(times , maxLongTrendShifts);
                        do{
                                if(directionLongWindow > 0)
                                {
                                        fairBuyPrice += fairBuyPrice*longTrendIncreaseSkewOffset_BPS;
                                        fairSellPrice += fairSellPrice*longTrendIncreaseSkewOffset_BPS;
                                }
                                else if(directionLongWindow < 0)
                                {
                                        fairBuyPrice -= fairBuyPrice*longTrendDecreaseSkewOffset_BPS;
                                        fairSellPrice -= fairSellPrice*longTrendDecreaseSkewOffset_BPS;
                                }
                        } while(--times > 0);
                }
        }
}


void Quoter::calQuotesQty()
{
        if(position > 0){
                sellQuoteQty = originalSellQuoteQty * (1 + int(position/quoteQtySkewFactorSell));
                buyQuoteQty = originalBuyQuoteQty;
        }
        else if(position < 0){
                buyQuoteQty = originalBuyQuoteQty * (1 - int(position / quoteQtySkewFactorBuy));
                sellQuoteQty = originalSellQuoteQty;
        }
        else{
                buyQuoteQty = originalBuyQuoteQty;
                sellQuoteQty = originalSellQuoteQty;
        }
}


int Quoter::priceCheck(int price , bool type){
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

void Quoter::adjustQuoteWithBook()
{
        if(fairBuyPrice > lagBidPrice )
        {
                fairBuyPrice = lagBidPrice + 5;
        }
        if(fairBuyPrice > lagAskPrice)
        {
                fairBuyPrice = lagAskPrice - 10;
        }
        if(fairSellPrice < lagAskPrice )
        {
                fairSellPrice = lagAskPrice - 5;
        }
        if(fairSellPrice < lagBidPrice )
        {
                fairSellPrice = lagBidPrice + 10;
        }
        if(abs(fairBuyPrice - lagAskPrice) <= 10)
        {
                fairBuyPrice -=5;
        }
        if(abs(fairSellPrice - lagBidPrice) <= 10)
        {
                fairSellPrice +=5;
        }
}



void Quoter::setPrevFairBuyPrice(){
        lastBuyOrderFairPrice = fairBuyPrice;
}

void Quoter::setPrevFairSellPrice(){
        lastSellOrderFairPrice = fairSellPrice;
}

void Quoter::quoteOrders(){
        
        if(buy_switch)
        {
                if(useLtpFilter)
                {
                        if(_portfolio->maxSellLtp == 0 || (fairBuyPrice >= _portfolio->maxSellLtp || fairSellPrice >= _portfolio->maxSellLtp))
                        {
                                if(_portfolio->maxSellLtp != 0 && fairBuyPrice >= _portfolio->maxSellLtp && fairSellPrice >= _portfolio->maxSellLtp )
                                {
                                        if(alphaSellPrice != lagBidPrice)
                                        {        
                                                std::cout<<"ALPHA|SellOrder:"<<lagBidPrice<<std::endl;
                                                alphaSellPrice = lagBidPrice;                                
                                        }
                                }
                                if(lagAskPrice <= portfolio_->minBuyLtp)
                                {
                                        std::cout<<"ALPHA2|BuyOrder:"<<lagAskPrice<<std::endl;

                                }
                        }
                        else
                        {
                                quoteBuyOrder(); 
                        }
                }
                else
                {
                        quoteBuyOrder();
                }
        }
        if(sell_switch)
        {
                if(useLtpFilter)
                {
                        if(_portfolio->minBuyLtp == 0 || (fairSellPrice <= _portfolio->minBuyLtp || fairBuyPrice <= _portfolio->minBuyLtp))
                        {
                                if(_portfolio->minBuyLtp != 0 && fairSellPrice <= _portfolio->minBuyLtp && fairBuyPrice <= _portfolio->minBuyLtp && fairSellPrice <= _portfolio->minSellLtp && fairBuyPrice <= _portfolio->minSellLtp)
                                {
                                        if(alphaBuyPrice != lagAskPrice){
                                                std::cout<<"ALPHA|BuyOrder:"<<lagAskPrice<<std::endl;
                                                alphaBuyPrice = lagAskPrice;
                                        }
                                }
                                if(lagBidPrice >= portfolio_->maxSellLtp)
                                {
                                        std::cout<<"ALPHA2|Sell:"<<lagBidPrice<<std::endl;

                                }

                                // return;
                        }
                        else
                        {
                                quoteSellOrder();
                        }
                }
                else
                {
                        quoteSellOrder();
                }
        }
}

bool Quoter::canModifyBuyOrder()
{
        if(fairBuyPrice == lastBuyOrderFairPrice){
                return false;
        }
        if(useAbsoluteModifyThreshold)
        {
                return canModifyBuyOrderABS();
        }
        else
        {
                return canModifyBuyOrderBPS();
        }
}

bool Quoter::canModifySellOrder()
{
        if(fairSellPrice == lastSellOrderFairPrice){
                return false;
        }
        if(useAbsoluteModifyThreshold)
        {
                return canModifySellOrderABS();
        }
        else
        {
                return canModifySellOrderBPS();
        }
}

bool Quoter::canModifyBuyOrderABS()
{
        int changeInFairBuyPrice = fairBuyPrice - lastBuyOrderFairPrice;
         if(changeInFairBuyPrice > 0 && changeInFairBuyPrice > quoteModifyPassiveThreshold_Bid_ABS)
        {
                return true;
        }
        else if(changeInFairBuyPrice < 0 && -changeInFairBuyPrice > quoteModifyAggresiveThreshold_Bid_ABS)
        {
                return true;
        }
        return false;

}

bool Quoter::canModifyBuyOrderBPS()
{
        int changeInFairBuyPrice = fairBuyPrice - lastBuyOrderFairPrice;
        if(changeInFairBuyPrice > 0 && changeInFairBuyPrice > lastBuyOrderFairPrice*quoteModifyPassiveThreshold_Bid_BPS)
        {
                return true;
        }
        else if(changeInFairBuyPrice < 0 && -changeInFairBuyPrice > lastBuyOrderFairPrice*quoteModifyAggresiveThreshold_Bid_BPS)
        {
                return true;
        }
        return false;
        
}

bool Quoter::canModifySellOrderABS()
{
        int changeInFairSellPrice = lastSellOrderFairPrice - fairSellPrice ;
         if(changeInFairSellPrice > 0 && changeInFairSellPrice > quoteModifyPassiveThreshold_Ask_ABS)
        {
                return true;
        }
        else if(changeInFairSellPrice < 0 && -changeInFairSellPrice > quoteModifyAggresiveThreshold_Ask_ABS)
        {
                return true;
        }
        return false;

}

bool Quoter::canModifySellOrderBPS()
{
        int changeInFairSellPrice = lastSellOrderFairPrice - fairSellPrice;
        if(changeInFairSellPrice > 0 && changeInFairSellPrice > lastSellOrderFairPrice*quoteModifyPassiveThreshold_Ask_BPS)
        {
                return true;
        }
        else if(changeInFairSellPrice < 0 && -changeInFairSellPrice > lastSellOrderFairPrice*quoteModifyAggresiveThreshold_Ask_BPS)
        {
                return true;
        }
        return false;
        
}

void Quoter::quoteBuyOrder()
{        
        if(buyWait!=0)
        {
                return;
        }
        buyFillQuantity = buyOrderHandler.second.fillQuantity;
        {
                if(lastBuyOrderId == 0)
                {
                        lastBuyOrderId = OrderIDStartingNumber;
                        setPrevFairBuyPrice();
                        _portfolio->sendNew('B', buyQuoteQty,fairBuyPrice,OrderIDStartingNumber++ , _quoterType); 
                        buyWait++;
                }
                else if(canModifyBuyOrder() == true)
                {
                        _portfolio->sendModify('B', buyQuoteQty,fairBuyPrice,lastBuyOrderId ,buyFillQuantity); 
                        setPrevFairBuyPrice(); 
                        buyWait++;
                }
        }           
}

void Quoter::quoteSellOrder()
{        
        if(sellWait!=0)
        {
                return;
        }
        sellFillQuantity = sellOrderHandler.second.fillQuantity;
        {
                if(lastSellOrderId == 0)
                {
                        lastSellOrderId = OrderIDStartingNumber;
                        setPrevFairSellPrice();
                        _portfolio->sendNew('S', sellQuoteQty,fairSellPrice,OrderIDStartingNumber++ , _quoterType);
                        sellWait++;
                }
                else if(canModifySellOrder() == true)
                {
                        _portfolio->sendModify('S', sellQuoteQty,fairSellPrice,lastSellOrderId ,sellFillQuantity); 
                        setPrevFairSellPrice(); 
                        sellWait++;
                }
        }           
}


void Quoter::cancelOpenBuyOrder(){
        if(buyWait == 0){
                if(lastBuyOrderId !=0 ){
                        _portfolio->sendCancel('B',lastBuyOrderId); 
                        buyWait++;
                }
        }
}

void Quoter::cancelOpenSellOrder(){
        if(sellWait == 0){
                if(lastSellOrderId !=0 ){
                        _portfolio->sendCancel('S' ,lastSellOrderId); 
                        sellWait++;
                }
        }
}

void Quoter::cancelOpenOrderOnEndTime(){
        if(position >= 0){
               cancelOpenBuyOrder();
        }
        if(position <= 0){
               cancelOpenSellOrder();
        }
}

void Quoter::cancelOpenOrder(){
        cancelOpenBuyOrder();
        cancelOpenSellOrder();
}

void Quoter::stopPortfolio()
{
        std::cout<<"Portfolio_Stopped:"<<_symbol<<std::endl;
        cancelOpenOrder();
}

void Quoter::startPortfolio()
{
        std::cout<<"Portfolio_Started:"<<_symbol<<std::endl;
}

void Quoter::endPortfolio(int bid , int ask){
        lagBidPrice = bid;
        lagAskPrice = ask;
        // squareOff();
        // sqauredOff = true;
}


void Quoter::onResponse(char responseType, long long int orderID, std::string symbol, char side, int fillQuantity,int price, int16_t errorCode, char orderType, int openQuantity)
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
                std::cout<<_quoterType<<"|Trade:"<<symbol<<"|"<<side<<"|"<<fillQuantity<<"|"<<price<<"|"<<openQuantity<<"|FAIRBUYPRICE:"<<fairBuyPrice<<"|ORDER_ID:"<<orderID<<"|buyFillQuantity:"<<buyOrderHandler.second.fillQuantity<<std::endl;
           
                position += fillQuantity;
                if(openQuantity == 0){
                        lastBuyOrderId = 0;
                }
                if(position == 0){
                        lastBuyOrderFairPrice = 0;
                        lastSellOrderFairPrice = 0;
                        lastSellOrderFillPrice = 0;
                        lastBuyOrderFillPrice = 0;
                }
                else{
                   lastBuyOrderFillPrice = price;
                }
                calQuotesQty();

        }
        else if(responseType == 'T' && side =='S'){
                std::cout<<_quoterType<<"|Trade:"<<symbol<<"|"<<side<<"|"<<fillQuantity<<"|"<<price<<"|"<<openQuantity<<"|FAIRSELLPRICE:"<<fairSellPrice<<"|ORDER_ID:"<<orderID<<"|sellFillQuantity:"<<sellOrderHandler.second.fillQuantity<<std::endl;
                position -= fillQuantity;
                if(openQuantity == 0){
                        lastSellOrderId = 0;
                }
                if(position == 0){
                        lastBuyOrderFairPrice = 0;
                        lastSellOrderFairPrice = 0;
                        lastSellOrderFillPrice = 0;
                        lastBuyOrderFillPrice = 0;
                }
                else{
                        lastSellOrderFillPrice = price;
                }
                calQuotesQty();
        }
        else if(responseType == 'R' && orderType == 'R'){
                std::cout<<_quoterType<<":ORDER_REJECTED_INTERNAL_ERROR_CODE:"<<errorCode<<"|price:"<<price<<"|side:"<<side<<std::endl;
                return;
        }
        else if(responseType == 'R' && side =='S' && orderType == 'N'){
                std::cout<<_quoterType<<"|REJECT_SELL_ORDER_NEW:"<<errorCode<<"|Symbol:"<<symbol<<"|Price:"<<price<<"|ORDER_ID:"<<orderID<<"|openQuantity:"<<openQuantity<<std::endl;
                lastSellOrderId = 0;
                sellOrderHandler.first = 0;

        }
        else if(responseType == 'R' && side =='B' && orderType == 'N'){
                std::cout<<_quoterType<<"|REJECT_BUY_ORDER_NEW:"<<errorCode<<"|Symbol:"<<symbol<<"|Price:"<<price<<"|ORDER_ID:"<<orderID<<"|openQuantity:"<<openQuantity<<std::endl;
                lastBuyOrderId = 0;
                buyOrderHandler.first = 0;

        }
        else if(responseType == 'R' && side =='B' && orderType == 'M'){
                std::cout<<_quoterType<<"|REJECT_BUY_ORDER_MODIFY:"<<errorCode<<"|Price:"<<price<<"|ORDER_ID:"<<orderID<<"|sellFillQuantity:"<<buyOrderHandler.second.fillQuantity<<std::endl;
                // lastBuyOrderId = 0;

        }
        else if(responseType == 'R' && side =='S' && orderType == 'M'){
                std::cout<<_quoterType<<"|REJECT_SELL_ORDER_MODIFY:"<<errorCode<<"|Price:"<<price<<"|ORDER_ID:"<<orderID<<"|sellFillQuantity:"<<sellOrderHandler.second.fillQuantity<<std::endl;
                // lastSellOrderId = 0;
        }
        else if(responseType == 'P' && side =='B'){
                position += fillQuantity;
                calQuotesQty();
        }

        else if(responseType == 'P' && side =='S'){
                position -= fillQuantity;
                calQuotesQty();
        }
        else if(responseType == 'X' && side == 'B' ){
                lastBuyOrderId = 0;
                buyOrderHandler.first = 0;        
        }
        else if(responseType == 'X' && side == 'S' ){
                lastSellOrderId = 0;
                sellOrderHandler.first = 0;
        }

        if(side == 'B' && responseType != 'T' && responseType != 'P' ){
                buyWait--;
        }

        if(side == 'S' && responseType != 'T' && responseType != 'P' ){
                sellWait--;
        }
}


void Quoter::stopBuying()
{
        buy_switch = false;
        cancelOpenBuyOrder();
}

void Quoter::stopSelling()
{   
        sell_switch = false;
        cancelOpenSellOrder();
}

void Quoter::startBuying()
{
        buy_switch = original_buy_switch;
}

void Quoter::startSelling()
{       
        sell_switch = original_sell_switch;
}

void Quoter::squareOffQuoter()
{
        cancelOpenOrder();
        position = 0;
        buyQuoteQty = originalBuyQuoteQty; 
        sellQuoteQty = originalSellQuoteQty; 

}
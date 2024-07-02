#include "Sniper.h"

Sniper::Sniper(){}
Sniper::~Sniper(){}

Sniper::Sniper(std::string symbol , std::string sniperType , sniperParams sniperParams_ , Portfolio * portfolio_ )
{
     _portfolio = portfolio_;
    _symbol = symbol;
    _sniperType = sniperType;
    setParams(sniperParams_);

}

void Sniper::setParams(sniperParams sniperParams_)
{
        useAbsoluteOffset = sniperParams_.useAbsoluteOffset;
        useSniper = sniperParams_.useSniper;
        buyOpportunity_BPS = sniperParams_.buyOpportunity_BPS;
        sellOpportunity_BPS = sniperParams_.sellOpportunity_BPS;
        buyOpportunity_ABS = sniperParams_.buyOpportunity_ABS;
        sellOpportunity_ABS = sniperParams_.sellOpportunity_ABS;
        bidSpread_BPS = sniperParams_.bidSpread_BPS;
        askSpread_BPS = sniperParams_.askSpread_BPS;
        bidSpread_ABS = sniperParams_.bidSpread_ABS;
        askSpread_ABS = sniperParams_.askSpread_ABS;
        quoteModifyThreshold_Bid_BPS = sniperParams_.quoteModifyThreshold_Bid_BPS;
        quoteModifyThreshold_Ask_BPS = sniperParams_.quoteModifyThreshold_Ask_BPS;
        quoteModifyThreshold_Bid_ABS = sniperParams_.quoteModifyThreshold_Bid_ABS;
        quoteModifyThreshold_Ask_ABS = sniperParams_.quoteModifyThreshold_Ask_ABS;
        sniperSwitch = sniperParams_.sniperSwitch;
        buy_switch = sniperParams_.buy_switch;
        sell_switch = sniperParams_.sell_switch;
        buyQuoteQty = sniperParams_.buyQuoteQty;
        sellQuoteQty = sniperParams_.sellQuoteQty;
        minBookBuyQtyToFire = sniperParams_.minBookBuyQtyToFire;
        minBookSellQtyToFire = sniperParams_.minBookSellQtyToFire;
        buyShootCount = sniperParams_.buyShootCount;
        sellShootCount = sniperParams_.sellShootCount;
        totalShootCount = sniperParams_.totalShootCount;
        buyMaxPosition = sniperParams_.buyMaxPosition;
        sellMaxPosition = sniperParams_.sellMaxPosition;
        booklevel = sniperParams_.booklevel;

        original_buy_switch = buy_switch;
        original_sell_switch = sell_switch;
}


void Sniper::updateData(int leaderLag , double fairBuyPrice_ , double fairSellPrice_ , int64_t bidPrice , int64_t askPrice)
{
    if(!useSniper)
    {
        return;
    }
    if(leaderLag == LEADLAG::LAG ){
            lagBidPrice = bidPrice;
            lagAskPrice = askPrice;
    }
    if(lagBidPrice == 0 || lagAskPrice == 0 )
    {
        return;
    }
    if(useAbsoluteOffset){
        calSniperQuotesABS(fairBuyPrice_ , fairSellPrice_);
        if(!checkOpportunityABS())
        {
            return;
        }
    }
    else{
        calSniperQuotesBPS(fairBuyPrice_ , fairSellPrice_);
        if(!checkOpportunityBPS())
        {
            return;
        }
    }
    if(captureByBuy == true && buy_switch == true && buyShootCount > 0)
    {
        if(useAbsoluteOffset)
        {
            if(!canModifyBuyOrderABS())
            {
                return;
            }
        }
        else{
            if(!canModifyBuyOrderBPS())
            {
                return;
            }
        }

        if(checkBuyBookQtyAvail())
        {
            quoteBuyOrderIOC();
        }
    }
    else if(captureBySell == true && sell_switch == true && sellShootCount > 0)
    {
        if(useAbsoluteOffset)
        {
            if(!canModifySellOrderABS())
            {
                return;
            }
        }
        else{
            if(!canModifySellOrderBPS())
            {
                return;
            }
        }
        if(checkSellBookQtyAvail())
        {
            quoteSellOrderIOC();
        }
    }
    

    

}

void Sniper::calSniperQuotesABS(double fairBuyPrice_ , double fairSellPrice_)
{
        fairBuyPrice = fairBuyPrice_ - bidSpread_ABS;
        fairSellPrice = fairSellPrice_  + askSpread_ABS;
        fairBuyPrice = priceCheck(fairBuyPrice , 0);
        fairSellPrice = priceCheck(fairSellPrice , 1);
}

void Sniper::calSniperQuotesBPS(double fairBuyPrice_ , double fairSellPrice_)
{
        fairBuyPrice = fairBuyPrice_ * (1 - bidSpread_BPS);
        fairSellPrice = fairSellPrice_ * (1 + askSpread_BPS);
        fairBuyPrice = priceCheck(fairBuyPrice , 0);
        fairSellPrice = priceCheck(fairSellPrice , 1);
}

int Sniper::priceCheck(int price , bool type){
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

bool Sniper::checkOpportunityABS()
{
    captureBySell = false;
    captureByBuy = false;
    if(lagBidPrice - fairSellPrice >= sellOpportunity_ABS)
    {
        captureBySell = true;
        return true;
    }
    else if(fairBuyPrice - lagAskPrice >= buyOpportunity_ABS)
    {
        captureByBuy = true;
        return true;
    }
    return false;
}

bool Sniper::checkOpportunityBPS()
{
    captureBySell = false;
    captureByBuy = false;
    if(lagBidPrice - fairSellPrice >= lagBidPrice*sellOpportunity_BPS)
    {
        captureBySell = true;
        return true;
    }
    else if(fairBuyPrice - lagAskPrice >= lagAskPrice*buyOpportunity_BPS)
    {
        captureByBuy = true;
        return true;
    }
    return false;
}

bool Sniper::checkBuyBookQtyAvail()
{
    int qty = 0;
    int index = 5;
    while(index < 2*booklevel)
    {
        if(_portfolio->lagBooklvl[index]._price > fairBuyPrice){
            break;
        }
        qty += _portfolio->lagBooklvl[index]._quantity;
        index++;
    }
    if(qty >= minBookBuyQtyToFire)
    {
        return true;
    }
    return false;
}

bool Sniper::checkSellBookQtyAvail()
{
    int qty = 0;
    int index = 0;
    while(index < booklevel)
    {
        if(_portfolio->lagBooklvl[index]._price < fairSellPrice){
            break;
        }
        qty += _portfolio->lagBooklvl[index]._quantity;
        index++;
    }
    if(qty >= minBookSellQtyToFire)
    {
        return true;
    }
    return false;
    
}

bool Sniper::canModifyBuyOrderABS()
{
        int changeInFairBuyPrice = abs(fairBuyPrice - lastBuyOrderFairPrice);
        if(changeInFairBuyPrice > quoteModifyThreshold_Bid_ABS)
        {
                return true;
        }
        return false;

}

bool Sniper::canModifyBuyOrderBPS()
{
        int changeInFairBuyPrice = abs(fairBuyPrice - lastBuyOrderFairPrice);
        if(changeInFairBuyPrice > lastBuyOrderFairPrice*quoteModifyThreshold_Bid_BPS)
        {
                return true;
        }
        return false;
        
}

bool Sniper::canModifySellOrderABS()
{
        int changeInFairSellPrice = abs(lastSellOrderFairPrice - fairSellPrice) ;
         if(changeInFairSellPrice > quoteModifyThreshold_Ask_ABS)
        {
                return true;
        }
        return false;

}

bool Sniper::canModifySellOrderBPS()
{
        int changeInFairSellPrice = abs(lastSellOrderFairPrice - fairSellPrice);
        if(changeInFairSellPrice > lastSellOrderFairPrice*quoteModifyThreshold_Ask_BPS)
        {
                return true;
        }
        return false;
        
}

void Sniper::setPrevFairBuyPrice(){
        lastBuyOrderFairPrice = fairBuyPrice;
}

void Sniper::setPrevFairSellPrice(){
        lastSellOrderFairPrice = fairSellPrice;
}

void Sniper::quoteBuyOrderIOC()
{        
        if(buyWait!=0)
        {
                return;
        }
        if(fairBuyPrice >= _portfolio->lowPrice && ((_portfolio->IncreaseOnSidePositionsNearDPR == true && fairBuyPrice <= _portfolio->originalHighPrice) || (_portfolio->IncreaseOnSidePositionsNearDPR == false  && fairBuyPrice <= _portfolio->highPrice)))
        {

        }
        else
        {
            return;
        }
        setPrevFairBuyPrice();
        int totalOrders = buyMaxPosition/buyQuoteQty;
        {
            while(totalOrders-- && buyShootCount > 0)
            {
                    _portfolio->sendNewIOC('B', buyQuoteQty,fairBuyPrice,OrderIDStartingNumber++ , _sniperType); 
                    buyWait++;
                    buyShootCount--;
            }
        }           
}

void Sniper::quoteSellOrderIOC()
{        
        if(sellWait!=0)
        {
                return;
        }
        if(fairSellPrice <= _portfolio->highPrice && ((_portfolio->IncreaseOnSidePositionsNearDPR == true && fairSellPrice >= _portfolio->originalLowPrice) || (_portfolio->IncreaseOnSidePositionsNearDPR == false  && fairSellPrice >= _portfolio->lowPrice)))
        {

        }
        else
        {
            return;
        }
        setPrevFairSellPrice();
        int totalOrders = sellMaxPosition/sellQuoteQty;
        {
            while(totalOrders-- && sellShootCount > 0)
            {                    
                    _portfolio->sendNewIOC('S', sellQuoteQty,fairSellPrice,OrderIDStartingNumber++ , _sniperType);
                    sellWait++;
                    sellShootCount--;
            }
        }           
}

void Sniper::onResponse(char responseType, long long int orderID, std::string symbol, char side, int fillQuantity,int price, int16_t errorCode, char orderType, int openQuantity)
{
    responseType = std::toupper(responseType);
    if(responseType == 'T' && side =='B')
    {
        std::cout<<_sniperType<<"|Trade:"<<symbol<<"|"<<side<<"|"<<fillQuantity<<"|"<<price<<"|"<<openQuantity<<"|FAIRBUYPRICE:"<<fairBuyPrice<<"|ORDER_ID:"<<orderID<<std::endl;
        // if(openQuantity == 0){
        //     buyWait--;
        // }
    }
    else if(responseType == 'T' && side =='S')
    {
        std::cout<<_sniperType<<"|Trade:"<<symbol<<"|"<<side<<"|"<<fillQuantity<<"|"<<price<<"|"<<openQuantity<<"|FAIRSELLPRICE:"<<fairSellPrice<<"|ORDER_ID:"<<orderID<<std::endl;
        // if(openQuantity == 0){
        //         sellWait--;
        // }
    }
    if(side == 'B' && responseType != 'T'){
                buyWait--;
    }
    if(side == 'S' && responseType != 'T'){
                sellWait--;
    }
}

void Sniper::stopBuying()
{
        buy_switch = false;
}

void Sniper::stopSelling()
{   
        sell_switch = false;
}

void Sniper::startBuying()
{
        buy_switch = original_buy_switch;
}

void Sniper::startSelling()
{       
        sell_switch = original_sell_switch;
}

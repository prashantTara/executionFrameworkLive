#include "multiQuoter.h"

MultiQuoter::MultiQuoter(){}
MultiQuoter::~MultiQuoter(){}

MultiQuoter::MultiQuoter(std::string symbol , std::string multiQuoterType , multiQuoterParams multiQuoterParams_ , Portfolio * portfolio_)
{
    _symbol = symbol;
    _portfolio = portfolio_;
    _multiQuoterType = multiQuoterType;
    setParams(multiQuoterParams_);
}

void MultiQuoter::setParams(multiQuoterParams multiQuoterParams_)
{
    useMultiQuoter = multiQuoterParams_.useMultiQuoter;
    useAbsoluteOffset = multiQuoterParams_.useAbsoluteOffset;
    levels = multiQuoterParams_.levels;
    bidSpread_BPS = multiQuoterParams_.bidSpread_BPS;
    askSpread_BPS = multiQuoterParams_.askSpread_BPS;
    bidSpread_ABS = multiQuoterParams_.bidSpread_ABS;
    askSpread_ABS = multiQuoterParams_.askSpread_ABS;
    bidSpreadIncrement_BPS = multiQuoterParams_.bidSpreadIncrement_BPS;
    askSpreadIncrement_BPS = multiQuoterParams_.askSpreadIncrement_BPS;
    bidSpreadIncrement_ABS = multiQuoterParams_.bidSpreadIncrement_ABS;
    askSpreadIncrement_ABS = multiQuoterParams_.askSpreadIncrement_ABS;
    quoteModifyPassiveThreshold_Bid_BPS = multiQuoterParams_.quoteModifyPassiveThreshold_Bid_BPS;
    quoteModifyPassiveThreshold_Ask_BPS = multiQuoterParams_.quoteModifyPassiveThreshold_Ask_BPS;
    quoteModifyAggresiveThreshold_Bid_BPS = multiQuoterParams_.quoteModifyAggresiveThreshold_Bid_BPS;
    quoteModifyAggresiveThreshold_Ask_BPS = multiQuoterParams_.quoteModifyAggresiveThreshold_Ask_BPS;
    quoteModifyPassiveThreshold_Bid_ABS = multiQuoterParams_.quoteModifyPassiveThreshold_Bid_ABS;
    quoteModifyPassiveThreshold_Ask_ABS = multiQuoterParams_.quoteModifyPassiveThreshold_Ask_ABS;
    quoteModifyAggresiveThreshold_Bid_ABS = multiQuoterParams_.quoteModifyAggresiveThreshold_Bid_ABS;
    quoteModifyAggresiveThreshold_Ask_ABS = multiQuoterParams_.quoteModifyAggresiveThreshold_Ask_ABS;
    buyQuoteQty = multiQuoterParams_.buyQuoteQty;
    originalBuyQuoteQty = multiQuoterParams_.buyQuoteQty;
    sellQuoteQty = multiQuoterParams_.sellQuoteQty;
    originalSellQuoteQty = multiQuoterParams_.sellQuoteQty;
    quoteQtySkewFactorBuy = multiQuoterParams_.quoteQtySkewFactorBuy;
    quoteQtySkewFactorSell = multiQuoterParams_.quoteQtySkewFactorSell;
    buyQuoteQtyIncrement = multiQuoterParams_.buyQuoteQtyIncrement;
    sellQuoteQtyIncrement = multiQuoterParams_.sellQuoteQtyIncrement;
    buyFillPercentage = multiQuoterParams_.buyFillPercentage;
    sellFillPercentage = multiQuoterParams_.sellFillPercentage;
    maxBuyPosition = multiQuoterParams_.maxBuyPosition;
    maxSellPosition = multiQuoterParams_.maxSellPosition;
    orderLimit = multiQuoterParams_.orderLimit;
    multiQuoterSwitch = multiQuoterParams_.multiQuoterSwitch;
    buy_switch = multiQuoterParams_.buy_switch;
    sell_switch = multiQuoterParams_.sell_switch;
    onlyPassive = multiQuoterParams_.onlyPassive;
    original_buy_switch = buy_switch;
    original_sell_switch = sell_switch;
    useLtpFilter = multiQuoterParams_.useLtpFilter;

    useLongTrendSkew = multiQuoterParams_.useLongTrendSkew;
    useShortTrendSkew = multiQuoterParams_.useShortTrendSkew;
    useTrendOffsetABS = multiQuoterParams_.useTrendOffsetABS;
    trendSkewInversion = multiQuoterParams_.trendSkewInversion;
    longTrendIncreaseSkewOffset_BPS = multiQuoterParams_.longTrendIncreaseSkewOffset_BPS;
    longTrendDecreaseSkewOffset_BPS = multiQuoterParams_.longTrendDecreaseSkewOffset_BPS;
    shortTrendIncreaseSkewOffset_BPS = multiQuoterParams_.shortTrendIncreaseSkewOffset_BPS;
    shortTrendDecreaseSkewOffset_BPS = multiQuoterParams_.shortTrendDecreaseSkewOffset_BPS;
    longTrendIncreaseSkewOffset_ABS = multiQuoterParams_.longTrendIncreaseSkewOffset_ABS;
    longTrendDecreaseSkewOffset_ABS = multiQuoterParams_.longTrendDecreaseSkewOffset_ABS;
    shortTrendIncreaseSkewOffset_ABS = multiQuoterParams_.shortTrendIncreaseSkewOffset_ABS;
    shortTrendDecreaseSkewOffset_ABS = multiQuoterParams_.shortTrendDecreaseSkewOffset_ABS;
    shortTrendQualifyPercent = multiQuoterParams_.shortTrendQualifyPercent;
    maxShortTrendShifts = multiQuoterParams_.maxShortTrendShifts;
    longTrendQualifyPercent = multiQuoterParams_.longTrendQualifyPercent;
    maxLongTrendShifts = multiQuoterParams_.maxLongTrendShifts;
    minQuotePassiveGap = multiQuoterParams_.minQuotePassiveGap;
    maxQuotePassiveGap = multiQuoterParams_.maxQuotePassiveGap;
    topLevelMinQtyFilter = multiQuoterParams_.topLevelMinQtyFilter;
    topLevelMinQtyBid = multiQuoterParams_.topLevelMinQtyBid;
    topLevelMinQtyAsk = multiQuoterParams_.topLevelMinQtyAsk;
    originalTopLevelMinQtyBid = topLevelMinQtyBid;
    originalTopLevelMinQtyAsk = topLevelMinQtyAsk;
    topLevelBidQtySkewFactor = multiQuoterParams_.topLevelBidQtySkewFactor;
    topLevelAskQtySkewFactor = multiQuoterParams_.topLevelAskQtySkewFactor;
    topLevelBidQtySkewFactorPosNetQty = multiQuoterParams_.topLevelBidQtySkewFactorPosNetQty;
    topLevelBidQtySkewFactorNegNetQty = multiQuoterParams_.topLevelBidQtySkewFactorNegNetQty;
    topLevelAskQtySkewFactorPosNetQty = multiQuoterParams_.topLevelAskQtySkewFactorPosNetQty;
    topLevelAskQtySkewFactorNegNetQty = multiQuoterParams_.topLevelAskQtySkewFactorNegNetQty;


    buyOrderPrice = std::vector<double>(levels , 0);
    prevBuyOrderPrice = std::vector<double>(levels , 0);
    prevBuyOrderHandled = std::vector<bool>(levels , 0);
    buyOrderQty = std::vector<double>(levels , 0);
    sellOrderPrice = std::vector<double>(levels , 0);
    prevSellOrderPrice = std::vector<double>(levels , 0);
    prevSellOrderHandled = std::vector<bool>(levels , 0);
    sellOrderQty = std::vector<double>(levels , 0);
    calQtyVectors();
}

void MultiQuoter::calQtyVectors()
{
    int buyQty = buyQuoteQty;
    int sellQty = sellQuoteQty;
    for(int i = 0; i < levels ; i++){
        buyOrderQty[i] = buyQty;
        sellOrderQty[i] = sellQty;
        buyQty += buyQuoteQtyIncrement;
        sellQty += sellQuoteQtyIncrement;
    }
}

void MultiQuoter::calQuotesQty()
{
        if(position > 0){
                sellQuoteQty = originalSellQuoteQty * (1 + int(position/quoteQtySkewFactorSell));
                buyQuoteQty = originalBuyQuoteQty;
                if(topLevelMinQtyFilter)
                {
                        int fac1 = abs(position/topLevelBidQtySkewFactor);
                        int fac2 = abs(position/topLevelAskQtySkewFactor);
                        if(fac1 > 0)
                        {
                                double bidFactor = topLevelBidQtySkewFactorPosNetQty;
                                while(--fac1 > 0)
                                {
                                        bidFactor = bidFactor * topLevelBidQtySkewFactorPosNetQty;
                                }
                                topLevelMinQtyBid = originalTopLevelMinQtyBid * bidFactor;
                        }
                        else
                        {
                                topLevelMinQtyBid = originalTopLevelMinQtyBid;
                        }
                        if(fac2 > 0)
                        {
                                double askFactor = topLevelAskQtySkewFactorPosNetQty;
                                while(--fac2 > 0)
                                {
                                        askFactor = askFactor * topLevelAskQtySkewFactorPosNetQty;
                                }
                                topLevelMinQtyAsk = originalTopLevelMinQtyAsk * askFactor;
                        }
                        else
                        {
                                topLevelMinQtyAsk = originalTopLevelMinQtyAsk;
                        }
                }
        }
        else if(position < 0){
                buyQuoteQty = originalBuyQuoteQty * (1 - int(position / quoteQtySkewFactorBuy));
                sellQuoteQty = originalSellQuoteQty;
                if(topLevelMinQtyFilter)
                {
                        int fac1 = abs(position/topLevelBidQtySkewFactor);
                        int fac2 = abs(position/topLevelAskQtySkewFactor);
                        if(fac1 > 0)
                        {
                                double bidFactor = topLevelBidQtySkewFactorNegNetQty;
                                while(--fac1 > 0)
                                {
                                        bidFactor = bidFactor * topLevelBidQtySkewFactorNegNetQty;
                                }
                                topLevelMinQtyBid = originalTopLevelMinQtyBid * bidFactor;
                        }
                        else
                        {
                                topLevelMinQtyBid = originalTopLevelMinQtyBid;

                        }
                        if(fac2 > 0)
                        {
                                double askFactor = topLevelAskQtySkewFactorNegNetQty;
                                while(--fac2 > 0)
                                {
                                        askFactor = askFactor * topLevelAskQtySkewFactorNegNetQty;
                                }
                                topLevelMinQtyAsk = originalTopLevelMinQtyAsk * askFactor;
                        }
                        else
                        {
                                topLevelMinQtyAsk = originalTopLevelMinQtyAsk;

                        }
                }
        }
        else{
                buyQuoteQty = originalBuyQuoteQty;
                sellQuoteQty = originalSellQuoteQty;
                if(topLevelMinQtyFilter)
                {
                        topLevelMinQtyBid = originalTopLevelMinQtyBid;
                        topLevelMinQtyAsk = originalTopLevelMinQtyAsk;
                }
        }
        calQtyVectors();
}

void MultiQuoter::calQuotes(int leaderLag , double fairBuyPrice_ , double fairSellPrice_ , int64_t bidPrice , int64_t askPrice )
{
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
        // std::cout<<"Before"<<_multiQuoterType<<":"<<fairBuyPrice_<<" "<<fairBuyPrice<<" "<<fairSellPrice<<" "<<fairSellPrice_<<std::endl;
        if(onlyPassive){
            adjustQuoteWithBook();
        }

        if(topLevelMinQtyFilter)
        {
                adjustQuoteWithBookQty();
        }
        // std::cout<<"After"<<_multiQuoterType<<":"<<fairBuyPrice<<" "<<fairSellPrice<<std::endl;
        
        // return;
        quoteOrders();

#ifdef debugStrat
        std::cout<<"MultiQuoter|"<<_symbol<<"|BuyPriceToOpenQty:";
        for(int index = 0; index < levels ; index++){
                std::cout<<prevBuyOrderPrice[index]<<":"<<buyPriceToOpenQty[prevBuyOrderPrice[index]]<<"|";
        }  
        std::cout<<std::endl;
        std::cout<<"MultiQuoter|"<<_symbol<<"|SellPriceToOpenQty:";
        for(int index = 0; index < levels ; index++)
        {
                std::cout<<prevSellOrderPrice[index]<<":"<<sellPriceToOpenQty[prevSellOrderPrice[index]]<<"|";
        }   
        std::cout<<std::endl;
#endif

}

void MultiQuoter::calQuotesABS(double fairBuyPrice_ , double fairSellPrice_)
{
        fairBuyPrice = fairBuyPrice_ - bidSpread_ABS;
        fairSellPrice = fairSellPrice_  + askSpread_ABS;
}

void MultiQuoter::calQuotesBPS(double fairBuyPrice_ , double fairSellPrice_)
{
        fairBuyPrice = fairBuyPrice_ * (1 - bidSpread_BPS);
        fairSellPrice = fairSellPrice_ * (1 + askSpread_BPS);
}

int MultiQuoter::priceCheck(int price , bool type){
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

void MultiQuoter::adjustQuoteWithBook()
{
        if(lagAskPrice - fairBuyPrice < minQuotePassiveGap )
        {
                fairBuyPrice = lagAskPrice - minQuotePassiveGap;
        }
        if(fairBuyPrice - lagBidPrice > maxQuotePassiveGap)
        {
                fairBuyPrice = lagBidPrice + maxQuotePassiveGap;
        }
        if(fairSellPrice - lagBidPrice < minQuotePassiveGap)
        {
                fairSellPrice = lagBidPrice + minQuotePassiveGap;
        }
        if(lagAskPrice - fairSellPrice > maxQuotePassiveGap)
        {
                fairSellPrice = lagAskPrice - maxQuotePassiveGap;
        }
}

void MultiQuoter::adjustQuoteWithBookQty()
{
        if(fairBuyPrice >= lagBidPrice)
        {
                int qty = findBuyPriceLevelQty(lagBidPrice);
                if(qty >= topLevelMinQtyBid )
                {
                        fairBuyPrice = lagBidPrice;
                }
                else{
                        fairBuyPrice = lagBidPrice - 5;
                }
        }
        if(fairSellPrice <= lagAskPrice)
        {
                int qty = findSellPriceLevelQty(lagAskPrice);
                if(qty >= topLevelMinQtyAsk )
                {
                        fairSellPrice = lagAskPrice;
                }
                else{
                        fairSellPrice = lagAskPrice + 5;
                }
        }
}

int MultiQuoter::findBuyPriceLevelQty(int64_t price)
{
        for(int i = 0 ; i < 5; i++)
        {
                if(_portfolio->lagBooklvl[i]._price == price)
                {
                        return _portfolio->lagBooklvl[i]._quantity;
                }
        }
        return 0;

}

int MultiQuoter::findSellPriceLevelQty(int64_t price)
{
        for(int i = 5 ; i < 10; i++)
        {
                if(_portfolio->lagBooklvl[i]._price == price)
                {
                        return _portfolio->lagBooklvl[i]._quantity;
                }
        }
        return 0;

}

void MultiQuoter::trendSkewQuotesABS()
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

void MultiQuoter::trendSkewQuotesBPS()
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

void MultiQuoter::setPrevFairBuyPrice(){
        lastBuyOrderFairPrice = fairBuyPrice;
}

void MultiQuoter::setPrevFairSellPrice(){
        lastSellOrderFairPrice = fairSellPrice;
}

void MultiQuoter::calBuyPriceVector()
{
    if(useAbsoluteOffset){
        calBuyPriceVectorABS();
    }
    else{
        calBuyPriceVectorBPS();
    }
}

void MultiQuoter::calSellPriceVector()
{
    if(useAbsoluteOffset){
        calSellPriceVectorABS();
    }
    else{
        calSellPriceVectorBPS();
    }
}

void MultiQuoter::calBuyPriceVectorABS()
{
    int price = fairBuyPrice;
    for(int i = 0; i < levels ; i++)
    {
        if(price >= _portfolio->lowPrice && ((_portfolio->IncreaseOnSidePositionsNearDPR == true && price <= _portfolio->originalHighPrice) || (_portfolio->IncreaseOnSidePositionsNearDPR == false  && price <= _portfolio->highPrice)))
        {
                buyOrderPrice[i] = price;
        }
        else
        {
                buyOrderPrice[i] = 0;
        }
        price -= bidSpreadIncrement_ABS;
    }
}

void MultiQuoter::calBuyPriceVectorBPS()
{
    int price = fairBuyPrice;
    for(int i = 0; i < levels ; i++)
    {
        if(price >= _portfolio->lowPrice && ((_portfolio->IncreaseOnSidePositionsNearDPR == true && price <= _portfolio->originalHighPrice) || (_portfolio->IncreaseOnSidePositionsNearDPR == false  && price <= _portfolio->highPrice)))
        {
                buyOrderPrice[i] = price;
        }
        else
        {
                buyOrderPrice[i] = 0;
        }
        price = price*(1 - bidSpreadIncrement_BPS);
        price = priceCheck(price , 0);
    }
}

void MultiQuoter::calSellPriceVectorABS()
{
    int price = fairSellPrice;
    for(int i = 0; i < levels ; i++)
    {
        if(price <= _portfolio->highPrice && ((_portfolio->IncreaseOnSidePositionsNearDPR == true && price >= _portfolio->originalLowPrice) || (_portfolio->IncreaseOnSidePositionsNearDPR == false  && price >= _portfolio->lowPrice)))
        {
                sellOrderPrice[i] = price;
        }
        else
        {
                sellOrderPrice[i] = INT_MAX;
        }
        price += askSpreadIncrement_ABS;
    }
}

void MultiQuoter::calSellPriceVectorBPS()
{
    int price = fairSellPrice;
    for(int i = 0; i < levels ; i++)
    {
        if(price <= _portfolio->highPrice && ((_portfolio->IncreaseOnSidePositionsNearDPR == true && price >= _portfolio->originalLowPrice) || (_portfolio->IncreaseOnSidePositionsNearDPR == false  && price >= _portfolio->lowPrice)))
        {
                sellOrderPrice[i] = price;
        
        }
        else
        {
                sellOrderPrice[i] = INT_MAX;
        }
        price = price*(1 + askSpreadIncrement_BPS);
        price = priceCheck(price , 1);
    
    }
}

void MultiQuoter::quoteOrders()
{    
        if(buy_switch)
        {
                bool quote = true;
                if(useLtpFilter)
                {
                        if(_portfolio->maxSellLtp == 0 || (fairBuyPrice >= _portfolio->maxSellLtp || fairSellPrice >= _portfolio->maxSellLtp))
                        {
                                quote = false;
                        }
                }
                if(quote)
                {    
                        if(canModifyBuyOrder())
                        {
                                setPrevFairBuyPrice();
                                calBuyPriceVector();
                #ifdef __DEBUG__
                                std::cout<<std::endl;
                                for(int i =0 ; i < levels ; i++ ){
                                        std::cout<<buyOrderQty[i]<<" "<<buyOrderPrice[i]<<" "<<sellOrderPrice[i]<<" "<<sellOrderQty[i]<<std::endl;
                                }
                #endif
                                quoteBuyOrder();
                
                        }
                        else{
                                if(buyFillPercentage >= 0){
                                        refillQuoteBuyOrder();
                                }
                        }
                }   
        }
        if(sell_switch)
        {     
                bool quote = true;
                if(useLtpFilter)
                {
                        if(_portfolio->minBuyLtp == 0 || (fairSellPrice <= _portfolio->minBuyLtp || fairBuyPrice <= _portfolio->minBuyLtp))
                        {
                                quote = false;
                        }
                }
                if(quote)
                {
                        if(canModifySellOrder())
                        {     
                                setPrevFairSellPrice();
                                calSellPriceVector();
                #ifdef __DEBUG__
                                std::cout<<std::endl;
                                for(int i =0 ; i < levels ; i++ ){
                                        std::cout<<buyOrderQty[i]<<" "<<buyOrderPrice[i]<<" "<<sellOrderPrice[i]<<" "<<sellOrderQty[i]<<std::endl;
                                }
                #endif
                                quoteSellOrder();
                
                        }
                        else{
                                if(sellFillPercentage >= 0)
                                {
                                        refillQuoteSellOrder();
                                }
                        }
                }
        }
}

bool MultiQuoter::canModifyBuyOrder()
{
        if(fairBuyPrice == lastBuyOrderFairPrice){
                return false;
        }
        if(useAbsoluteOffset)
        {
                return canModifyBuyOrderABS();
        }
        else
        {
                return canModifyBuyOrderBPS();
        }
}

bool MultiQuoter::canModifySellOrder()
{
        if(fairSellPrice == lastSellOrderFairPrice){
                return false;
        }
        if(useAbsoluteOffset)
        {
                return canModifySellOrderABS();
        }
        else
        {
                return canModifySellOrderBPS();
        }
}

bool MultiQuoter::canModifyBuyOrderABS()
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

bool MultiQuoter::canModifyBuyOrderBPS()
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

bool MultiQuoter::canModifySellOrderABS()
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

bool MultiQuoter::canModifySellOrderBPS()
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

void MultiQuoter::quoteBuyOrder()
{        
        if(buyWait!=0)
        {
                return;
        }
        int prevIndex = 0;
        int lastIndex = levels - 1;
        if(prevBuyOrderPrice[0] < buyOrderPrice[0] && prevBuyOrderPrice[lastIndex] > buyOrderPrice[lastIndex])
        {
                return;
        }
        // std::cout<<"BUY SIDE"<<std::endl;
        // for(int i = 0; i < levels ; i++)
        // {
        //         std::cout<<prevBuyOrderPrice[i]<<"  "<<buyOrderPrice[i]<<std::endl;
        // }
        for(int i = 0 ; i < levels ; i++){
                if(prevBuyOrderPrice[i] > buyOrderPrice[0] || prevBuyOrderPrice[i] < buyOrderPrice[lastIndex])
                {
                        if(prevBuyOrderPrice[i] > buyOrderPrice[0]){
                                prevIndex++;
                        }
                        while(!buyPriceToOrderId[prevBuyOrderPrice[i]].empty() ){
                                long long int orderId = buyPriceToOrderId[prevBuyOrderPrice[i]].top();
                                buyPriceToOrderId[prevBuyOrderPrice[i]].pop();
                                _portfolio->sendCancel('B',orderId);
                                orderIdToBuyFillQuantity[orderId] = 0;
                                orderIdToBuyOpenQuantity[orderId] = 0;
                                buyWait++;
                        }
                        buyPriceToOpenQty[prevBuyOrderPrice[i]] = 0;
                        buyPriceToFillQty[prevBuyOrderPrice[i]] = 0;
                        prevBuyOrderHandled[i] = true;
                }
        }

        for(int i = 0 ; i < levels ; i++)
        {
                if(buyOrderPrice[i] > prevBuyOrderPrice[0] || buyOrderPrice[i] < prevBuyOrderPrice[lastIndex])
                {
                        if(orderLimit > 0 && buyOrderPrice[i] > 0){
                                buyPriceToOrderId[buyOrderPrice[i]].push(OrderIDStartingNumber);
                                orderIdToBuyFillQuantity[OrderIDStartingNumber] = 0;
                                orderIdToBuyOpenQuantity[OrderIDStartingNumber] = buyOrderQty[i];
                                buyPriceToOpenQty[buyOrderPrice[i]] = buyOrderQty[i];
                                buyPriceToFillQty[buyOrderPrice[i]] = 0;
                                orderIdToPrice[OrderIDStartingNumber] = buyOrderPrice[i];
                                _portfolio->sendNew('B', buyOrderQty[i],buyOrderPrice[i],OrderIDStartingNumber++ , _multiQuoterType); 
                                orderLimit--;
                                buyWait++;
                        }
                }
                else if(buyOrderPrice[i] > 0)
                {
                        prevBuyOrderHandled[prevIndex] = true;
                        int64_t price = 0;
                        if(prevIndex >= levels){
                                price = buyOrderPrice[i];
                        }else{
                                buyOrderPrice[i] = prevBuyOrderPrice[prevIndex++];
                                price = buyOrderPrice[i];
                        }
                        int qty = buyOrderQty[i];
                        if(qty > buyPriceToOpenQty[price])
                        {
                                if(orderLimit > 0){
                                        buyPriceToOrderId[price].push(OrderIDStartingNumber);
                                        orderIdToBuyFillQuantity[OrderIDStartingNumber] = 0;
                                        orderIdToBuyOpenQuantity[OrderIDStartingNumber] = qty - buyPriceToOpenQty[price];
                                        orderIdToPrice[OrderIDStartingNumber] = price;
                                        _portfolio->sendNew('B', qty - buyPriceToOpenQty[price],price,OrderIDStartingNumber++ , _multiQuoterType); 
                                        buyPriceToOpenQty[price] = qty;
                                        buyWait++;
                                        orderLimit--;
                                }
                        }
                        else if(qty < buyPriceToOpenQty[price])
                        {
                                int qtyClosed = buyPriceToOpenQty[price] - qty;
                                if(buyPriceToOrderId[price].size() > 0)
                                {
                                        while(qtyClosed > 0 && buyPriceToOrderId[price].size() > 0)
                                        {
                                                long long int orderId = buyPriceToOrderId[price].top();
                                                int qty = orderIdToBuyOpenQuantity[orderId];
                                                if(qty > qtyClosed)
                                                {
                                                                _portfolio->sendModify('B', qty - qtyClosed,price,orderId ,orderIdToBuyFillQuantity[orderId]); 
                                                                orderIdToBuyOpenQuantity[orderId] = qty - qtyClosed;
                                                                buyPriceToOpenQty[price] -= qtyClosed;
                                                                qtyClosed = 0;
                                                                buyWait++;
                                                }
                                                else if(qty <= qtyClosed)
                                                {
                                                        qtyClosed -= qty; 
                                                        buyPriceToOrderId[price].pop();
                                                        orderIdToBuyOpenQuantity[orderId] = 0;
                                                        orderIdToBuyFillQuantity[orderId] = 0;
                                                                _portfolio->sendCancel('B',orderId);
                                                                buyPriceToOpenQty[price] -= qty;
                                                                buyWait++;
                                                                
                                                }
                                        }
                                }
                                else{
                                        buyPriceToOpenQty[price] = 0;
                                }
                        }
                }
        }
        for(int i = 0 ; i < levels ; i++){
                if(prevBuyOrderHandled[i] == false)
                {
                        while(!buyPriceToOrderId[prevBuyOrderPrice[i]].empty() ){
                                long long int orderId = buyPriceToOrderId[prevBuyOrderPrice[i]].top();
                                buyPriceToOrderId[prevBuyOrderPrice[i]].pop();
                                _portfolio->sendCancel('B',orderId);
                                orderIdToBuyFillQuantity[orderId] = 0;
                                orderIdToBuyOpenQuantity[orderId] = 0;
                                buyWait++;
                        }
                        buyPriceToOpenQty[prevBuyOrderPrice[i]] = 0;
                        buyPriceToFillQty[prevBuyOrderPrice[i]] = 0;
                        prevBuyOrderHandled[i] = true;
                }
        }
        for(int index = 0; index < levels ; index++){
                prevBuyOrderPrice[index] = buyOrderPrice[index];
                prevBuyOrderHandled[index] = false;
        }  

}

void MultiQuoter::refillQuoteBuyOrder()
{
        if(buyWait!=0)
        {
                return;
        }
        for(int index = 0; index < levels ; index++){
                int64_t price = prevBuyOrderPrice[index];
                if(price <= fairBuyPrice && price > 0)
                {
                        int openQty = buyPriceToOpenQty[price];
                        int qty = buyOrderQty[index];
                        if(openQty <= buyFillPercentage*qty)
                        {
                                if(orderLimit > 0){
                                        buyPriceToOrderId[price].push(OrderIDStartingNumber);
                                        orderIdToBuyFillQuantity[OrderIDStartingNumber] = 0;
                                        orderIdToBuyOpenQuantity[OrderIDStartingNumber] = qty - openQty;
                                        orderIdToPrice[OrderIDStartingNumber] = price;
                                        _portfolio->sendNew('B', qty - openQty,price,OrderIDStartingNumber++ , _multiQuoterType); 
                                        buyPriceToOpenQty[price] = qty;
                                        buyWait++;
                                        orderLimit--;
                                }
                        }
                }
        } 
}

void MultiQuoter::refillQuoteSellOrder()
{
        if(sellWait!=0)
        {
                return;
        }
        for(int index = 0; index < levels ; index++){
                int64_t price = prevSellOrderPrice[index];
                if(price >= fairSellPrice && price < INT_MAX)
                {
                        int openQty = sellPriceToOpenQty[price];
                        int qty = sellOrderQty[index];
                        if(openQty <= sellFillPercentage*qty)
                        {
                                if(orderLimit > 0){
                                        sellPriceToOrderId[price].push(OrderIDStartingNumber);
                                        orderIdToSellFillQuantity[OrderIDStartingNumber] = 0;
                                        orderIdToSellOpenQuantity[OrderIDStartingNumber] = qty - openQty;
                                        orderIdToPrice[OrderIDStartingNumber] = price;
                                        _portfolio->sendNew('S', qty - openQty,price,OrderIDStartingNumber++ , _multiQuoterType); 
                                        sellPriceToOpenQty[price] = qty;
                                        sellWait++;
                                        orderLimit--;
                                }
                        }
                }
        } 
}

void MultiQuoter::quoteSellOrder()
{        
        if(sellWait!=0)
        {
                return;
        }
        int prevIndex = 0;
        int lastIndex = levels - 1;
        if(prevSellOrderPrice[0] > sellOrderPrice[0] && prevSellOrderPrice[lastIndex] < sellOrderPrice[lastIndex])
        {
                return;
        }
        // std::cout<<"SELL SIDE"<<std::endl;
        // for(int i = 0; i < levels ; i++)
        // {
        //         std::cout<<prevSellOrderPrice[i]<<"  "<<sellOrderPrice[i]<<std::endl;
        // }
        for(int i = 0; i < levels ; i++)
        {
                if(prevSellOrderPrice[i] < sellOrderPrice[0] || prevSellOrderPrice[i] > sellOrderPrice[lastIndex])
                {
                        if(prevSellOrderPrice[i] < sellOrderPrice[0]){
                                prevIndex++;
                        }
                        while(!sellPriceToOrderId[prevSellOrderPrice[i]].empty() ){
                                long long int orderId = sellPriceToOrderId[prevSellOrderPrice[i]].top();
                                sellPriceToOrderId[prevSellOrderPrice[i]].pop();
                                _portfolio->sendCancel('S',orderId);
                                orderIdToSellFillQuantity[orderId] = 0;
                                orderIdToSellOpenQuantity[orderId] = 0;
                                sellWait++;
                        }
                        sellPriceToOpenQty[prevSellOrderPrice[i]] = 0;
                        sellPriceToFillQty[prevSellOrderPrice[i]] = 0;
                        prevSellOrderHandled[i] = true;
                }
        }
        for(int i = 0; i < levels ; i++)
        {
                if(sellOrderPrice[i] < prevSellOrderPrice[0] || sellOrderPrice[i] > prevSellOrderPrice[lastIndex])
                {
                        if(orderLimit > 0 && sellOrderPrice[i] < INT_MAX)
                        {
                                sellPriceToOrderId[sellOrderPrice[i]].push(OrderIDStartingNumber);
                                orderIdToSellFillQuantity[OrderIDStartingNumber] = 0;
                                orderIdToSellOpenQuantity[OrderIDStartingNumber] = sellOrderQty[i];
                                sellPriceToOpenQty[sellOrderPrice[i]] = sellOrderQty[i];
                                sellPriceToFillQty[sellOrderPrice[i]] = 0;
                                orderIdToPrice[OrderIDStartingNumber] = sellOrderPrice[i];
                                _portfolio->sendNew('S', sellOrderQty[i],sellOrderPrice[i],OrderIDStartingNumber++ , _multiQuoterType); 
                                sellWait++;
                                orderLimit--;
                        }
                }
                else if(sellOrderPrice[i] < INT_MAX)
                {
                        prevSellOrderHandled[prevIndex] = true;
                        int64_t price = 0;
                        if(prevIndex >= levels){
                                price = sellOrderPrice[i];
                        }else{
                                sellOrderPrice[i] = prevSellOrderPrice[prevIndex++];
                                price = sellOrderPrice[i];
                        }
                        int qty = sellOrderQty[i];
                        if(qty > sellPriceToOpenQty[price])
                        {
                                if(orderLimit > 0)
                                {
                                        sellPriceToOrderId[price].push(OrderIDStartingNumber);
                                        orderIdToSellFillQuantity[OrderIDStartingNumber] = 0;
                                        orderIdToSellOpenQuantity[OrderIDStartingNumber] = qty - sellPriceToOpenQty[price];
                                        orderIdToPrice[OrderIDStartingNumber] = price;
                                        
                                        _portfolio->sendNew('S', qty - sellPriceToOpenQty[price],price,OrderIDStartingNumber++ , _multiQuoterType); 
                                        sellPriceToOpenQty[price] = qty;
                                        sellWait++;
                                        orderLimit--;
                                }
                        }
                        else if(qty < sellPriceToOpenQty[price]){

                                int qtyClosed = sellPriceToOpenQty[price] - qty;
                                if(sellPriceToOrderId[price].size() > 0)
                                {
                                        while(qtyClosed > 0 && sellPriceToOrderId[price].size() > 0){
                                                long long int orderId = sellPriceToOrderId[price].top();
                                                int qty = orderIdToSellOpenQuantity[orderId];
                                                if(qty > qtyClosed)
                                                {      
                                                                _portfolio->sendModify('S', qty - qtyClosed,price,orderId ,orderIdToSellFillQuantity[orderId]); 
                                                                orderIdToSellOpenQuantity[orderId] = qty - qtyClosed;
                                                                sellPriceToOpenQty[price] -= qtyClosed;
                                                                qtyClosed = 0;
                                                                sellWait++;
                                                }
                                                else if(qty <= qtyClosed)
                                                {
                                                        qtyClosed -= qty; 
                                                        sellPriceToOrderId[price].pop();
                                                        orderIdToSellOpenQuantity[orderId] = 0;
                                                        orderIdToSellFillQuantity[orderId] = 0;                                                 
                                                        _portfolio->sendCancel('S',orderId);
                                                        sellPriceToOpenQty[price] -= qty;                                                 
                                                        sellWait++;
                                                }
                                        }
                                }
                                else
                                {
                                        sellPriceToOpenQty[price] = 0;
                                }
                        }
                }
        }

        for(int i = 0; i < levels ; i++)
        {
                if(prevSellOrderHandled[i] == false)
                {
                        while(!sellPriceToOrderId[prevSellOrderPrice[i]].empty() ){
                                long long int orderId = sellPriceToOrderId[prevSellOrderPrice[i]].top();
                                sellPriceToOrderId[prevSellOrderPrice[i]].pop();
                                _portfolio->sendCancel('S',orderId);
                                orderIdToSellFillQuantity[orderId] = 0;
                                orderIdToSellOpenQuantity[orderId] = 0;
                                sellWait++;
                        }
                        sellPriceToOpenQty[prevSellOrderPrice[i]] = 0;
                        sellPriceToFillQty[prevSellOrderPrice[i]] = 0;
                        prevSellOrderHandled[i] = true;
                }
        }
        for(int index = 0; index < levels ; index++)
        {
                prevSellOrderPrice[index] = sellOrderPrice[index];
                prevSellOrderHandled[index] = false;

        }  
}

void MultiQuoter::removeBuyOrderIdFromStack(long long orderId , int64_t price)
{
        if(buyPriceToOrderId[price].size() == 0){
                return;
        }
        long long id = buyPriceToOrderId[price].top();
        buyPriceToOrderId[price].pop();
        if(id == orderId){
                return;
        }
        else{
                removeBuyOrderIdFromStack(orderId , price);
                buyPriceToOrderId[price].push(id);
        }
}

void MultiQuoter::removeSellOrderIdFromStack(long long orderId , int64_t price)
{
        if(sellPriceToOrderId[price].size() == 0){
                return;
        }
        long long id = sellPriceToOrderId[price].top();
        sellPriceToOrderId[price].pop();
        if(id == orderId){
                return;
        }
        else{
                removeSellOrderIdFromStack(orderId , price);
                sellPriceToOrderId[price].push(id);
        }
        
}

void MultiQuoter::onResponse(char responseType, long long int orderID, std::string symbol, char side, int fillQuantity,int price, int16_t errorCode, char orderType, int openQuantity)
{
        responseType = std::toupper(responseType);
        if(side == 'B'){
                orderIdToBuyFillQuantity[orderID] += fillQuantity;
                orderIdToBuyOpenQuantity[orderID] = openQuantity;
                buyPriceToOpenQty[orderIdToPrice[orderID]] -= fillQuantity;
                buyPriceToFillQty[orderIdToPrice[orderID]] += fillQuantity;
                if(buyPriceToOpenQty[orderIdToPrice[orderID]] == 0)
                {
                        buyPriceToFillQty[orderIdToPrice[orderID]] = 0;
                }
                if(orderIdToBuyOpenQuantity[orderID] == 0)
                {
                        orderIdToBuyFillQuantity[orderID] = 0;
                }
                // std::cout<<"OrderID:"<<orderID<<"|"<<orderIdToBuyOpenQuantity[orderID]<<"|"<<orderIdToBuyFillQuantity[orderID]<<"|Price:"<<price<<"|"<<buyPriceToOpenQty[orderIdToPrice[orderID]]<<"|"<<buyPriceToFillQty[orderIdToPrice[orderID]]<<std::endl;
        }
        else if(side == 'S'){
                orderIdToSellFillQuantity[orderID] += fillQuantity;
                orderIdToSellOpenQuantity[orderID] = openQuantity;
                sellPriceToOpenQty[orderIdToPrice[orderID]] -= fillQuantity;
                sellPriceToFillQty[orderIdToPrice[orderID]] += fillQuantity;
                if(sellPriceToOpenQty[orderIdToPrice[orderID]] == 0)
                {
                        sellPriceToFillQty[orderIdToPrice[orderID]] = 0;
                }
                if(orderIdToSellOpenQuantity[orderID] == 0)
                {
                        orderIdToSellFillQuantity[orderID] = 0;
                }
        }
        if(responseType == 'T' && side =='B'){
                std::cout<<_multiQuoterType<<"|Trade:"<<symbol<<"|"<<side<<"|"<<fillQuantity<<"|"<<price<<"|"<<openQuantity<<"|FAIRBUYPRICE:"<<fairBuyPrice<<"|ORDER_ID:"<<orderID<<"|buyFillQuantity:"<<orderIdToBuyFillQuantity[orderID]<<std::endl;
                position += fillQuantity;
                if(openQuantity == 0){
                        removeBuyOrderIdFromStack(orderID , orderIdToPrice[orderID]);
                }
                calQuotesQty();
                // if(position == 0){
                //         lastBuyOrderFairPrice = 0;
                //         lastSellOrderFairPrice = 0;
                // }
        }
        else if(responseType == 'T' && side =='S'){
                std::cout<<_multiQuoterType<<"|Trade:"<<symbol<<"|"<<side<<"|"<<fillQuantity<<"|"<<price<<"|"<<openQuantity<<"|FAIRSELLPRICE:"<<fairSellPrice<<"|ORDER_ID:"<<orderID<<"|sellFillQuantity:"<<orderIdToSellFillQuantity[orderID]<<std::endl;
                position -= fillQuantity;
                if(openQuantity == 0){
                        removeSellOrderIdFromStack(orderID , orderIdToPrice[orderID]);
                }
                calQuotesQty();
                // if(position == 0){
                //         lastBuyOrderFairPrice = 0;
                //         lastSellOrderFairPrice = 0;
                // }
        }
        // else if(responseType == 'R' || orderType == 'R'){
        //         std::cout<<_multiQuoterType<<":ORDER_REJECTED_INTERNAL_ERROR_CODE:"<<errorCode<<"|price:"<<price<<"|side:"<<side<<std::endl;
        //         // removeBuyOrderIdFromStack(orderID , price);
        //         return;
        // }
        else if(responseType == 'R' && side =='S' && orderType == 'N'){
                std::cout<<_multiQuoterType<<"|REJECT_SELL_ORDER_NEW:"<<errorCode<<"|Symbol:"<<symbol<<"|Price:"<<price<<"|ORDER_ID:"<<orderID<<"|openQuantity:"<<openQuantity<<std::endl;
                sellPriceToOpenQty[price] -= orderIdToSellOpenQuantity[orderID]; 
                orderIdToSellOpenQuantity[orderID] = 0;
                removeSellOrderIdFromStack(orderID , orderIdToPrice[orderID]);

        }
        else if(responseType == 'R' && side =='B' && orderType == 'N'){
                std::cout<<_multiQuoterType<<"|REJECT_BUY_ORDER_NEW:"<<errorCode<<"|Symbol:"<<symbol<<"|Price:"<<price<<"|ORDER_ID:"<<orderID<<"|openQuantity:"<<openQuantity<<std::endl;
                buyPriceToOpenQty[price] -= orderIdToBuyOpenQuantity[orderID]; 
                orderIdToBuyOpenQuantity[orderID] = 0;
                removeBuyOrderIdFromStack(orderID , orderIdToPrice[orderID]);

        }
        else if(responseType == 'R' && side =='B' && orderType == 'M'){
                std::cout<<_multiQuoterType<<"|REJECT_BUY_ORDER_MODIFY:"<<errorCode<<"|Price:"<<price<<"|ORDER_ID:"<<orderID<<"|buyFillQuantity:"<<orderIdToBuyFillQuantity[orderID]<<std::endl;

        }
        else if(responseType == 'R' && side =='S' && orderType == 'M'){
                std::cout<<_multiQuoterType<<"|REJECT_SELL_ORDER_MODIFY:"<<errorCode<<"|Price:"<<price<<"|ORDER_ID:"<<orderID<<"|sellFillQuantity:"<<orderIdToSellFillQuantity[orderID]<<std::endl;
        }
        else if(orderType == 'X' && responseType == 'R')
        {
                if(fillQuantity > 0 && openQuantity > 0){
                        _portfolio->sendCancel(side , orderID);
                        if(side == 'B')
                        {
                                buyWait++;
                        }
                        else if(side == 'S')
                        {
                                sellWait++;
                        }
                }
        }

        if(side == 'B' && responseType != 'T' && responseType != 'P' && errorCode != 17080 ){
                buyWait--;
        }

        if(side == 'S' && responseType != 'T' && responseType != 'P' && errorCode != 17080){
                sellWait--;
        }
}


void MultiQuoter::cancelOpenBuyOrder(){
        if(buyWait == 0){
                for(auto st : buyPriceToOrderId ){
                        while(st.second.size() > 0){
                                long long orderId = st.second.top();
                                st.second.pop();
                                _portfolio->sendCancel('B',orderId); 
                                buyWait++;
                        }
                }
                buyPriceToOrderId.clear();
        }
}

void MultiQuoter::cancelOpenSellOrder(){
        if(sellWait == 0){
                for(auto st : sellPriceToOrderId ){
                        while(st.second.size() > 0){
                                long long orderId = st.second.top();
                                st.second.pop();
                                _portfolio->sendCancel('S' ,orderId); 
                                sellWait++;
                        }
                }
                sellPriceToOrderId.clear();
        }
}

void MultiQuoter::endPortfolio(int64_t bid , int64_t ask){
        lagBidPrice = bid;
        lagAskPrice = ask;
        // squareOff();
        // sqauredOff = true;
}




void MultiQuoter::cancelOpenOrderOnEndTime(){
        if(position >= 0){
               cancelOpenBuyOrder();
        }
        if(position <= 0){
               cancelOpenSellOrder();
        }
}

void MultiQuoter::cancelOpenOrder(){
        cancelOpenBuyOrder();
        cancelOpenSellOrder();
}

void MultiQuoter::stopBuying()
{
        buy_switch = false;
        cancelOpenBuyOrder();
}

void MultiQuoter::stopSelling()
{   
        sell_switch = false;
        cancelOpenSellOrder();
}

void MultiQuoter::startBuying()
{
        buy_switch = original_buy_switch;
}

void MultiQuoter::startSelling()
{       
        sell_switch = original_sell_switch;
}

void MultiQuoter::squareOffQuoter()
{
        cancelOpenOrder();
        position = 0;
        calQuotesQty();
}
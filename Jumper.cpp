#include "Jumper.h"

Jumper::Jumper(){}
Jumper::~Jumper(){}


Jumper::Jumper(jumperParams jumperParams_ , Portfolio * portfolio_)
{
    _portfolio = portfolio_;
    setParams(jumperParams_);
}

void Jumper::setParams(jumperParams jumperParams_){

        useJumper = jumperParams_.useJumper;
        buyJoinQty = jumperParams_.buyJoinQty;
        sellJoinQty = jumperParams_.sellJoinQty;
        buyJumpQty = jumperParams_.buyJumpQty;
        sellJumpQty = jumperParams_.sellJumpQty;
        useCummQty = jumperParams_.useCummQty;
        bidCummQty = jumperParams_.bidCummQty;
        askCummQty = jumperParams_.askCummQty;
        bookLength = jumperParams_.bookLength;
        topToBottomPriority = jumperParams_.topToBottomPriority;    
        bottomToTopPriority = jumperParams_.bottomToTopPriority;
        useAbsOffset = jumperParams_.useAbsOffset;
        bidOffsetBPS = jumperParams_.bidOffsetBPS;    
        askOffsetBPS = jumperParams_.askOffsetBPS;    
        bidOffsetABS = jumperParams_.bidOffsetABS; 
        askOffsetABS = jumperParams_.askOffsetABS;
        originalBuyQuoteQty = jumperParams_.buyQuoteQty;
        originalSellQuoteQty = jumperParams_.sellQuoteQty;
        buyMaxPosition = jumperParams_.buyMaxPosition;
        sellMaxPosition = jumperParams_.sellMaxPosition;
        modifyDelay = jumperParams_.modifyDelay;
        jumperSwitch = jumperParams_.jumperSwitch;
        jumperBuySwitch = jumperParams_.jumperBuySwitch;
        jumperSellSwitch = jumperParams_.jumperSellSwitch;
        original_buy_switch = jumperBuySwitch;
        original_sell_switch = jumperSellSwitch;
        buyQuoteQty = originalBuyQuoteQty;
        sellQuoteQty = originalSellQuoteQty;
        useAvgBook = jumperParams_.useAvgBook;
        avgOrderToBookThreshold_BPS = jumperParams_.avgOrderToBookThreshold_BPS;
        avgOrderToBookThreshold_ABS = jumperParams_.avgOrderToBookThreshold_ABS;
        avgBookLevels = jumperParams_.avgBookLevels;
        levelWeightageVector = jumperParams_.levelWeightageVector;

}



void Jumper::updateData(long long int time , double fairBuyPrice_ , double fairSellPrice_ )
{
    if(time - lastJumperUpdateTime  < modifyDelay)
    {
        return;
    }
    lastJumperUpdateTime = time;   

     if(useAbsOffset)
    {
        calJumperQuotesABS(fairBuyPrice_ , fairSellPrice_);
    }
    else
    {
        calJumperQuotesBPS(fairBuyPrice_ , fairSellPrice_);
    } 

    quoteJumpOrders();

}

void Jumper::calJumperQuotesABS(double fairBuyPrice_ , double fairSellPrice_)
{
        fairBuyPrice = fairBuyPrice_ - bidOffsetABS;
        fairSellPrice = fairSellPrice_  + askOffsetABS;
}

void Jumper::calJumperQuotesBPS(double fairBuyPrice_ , double fairSellPrice_)
{
        fairBuyPrice = fairBuyPrice_ * (1 - bidOffsetBPS);
        fairSellPrice = fairSellPrice_ * (1 + askOffsetBPS);
}

void Jumper::calQuotesQty()
{
        if(position > 0){
                buyQuoteQty = std::min(originalBuyQuoteQty , buyMaxPosition - position);
        }
        else if(position < 0){
                sellQuoteQty = std::min(originalSellQuoteQty , sellMaxPosition + position);
        }
        else{
                buyQuoteQty = originalBuyQuoteQty;
                sellQuoteQty = originalSellQuoteQty;
        }
}

int Jumper::priceCheck(int price , bool type){
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

void Jumper::calBuyBookAvg()
{
        buyBookAvgPrice = 0;
        double totalBuyWeigthage = 0;
        for(int i = 0; i < avgBookLevels ; i++)
        {
                if(_portfolio->lagBooklvl[i]._price > 0)
                {
                        buyBookAvgPrice += (_portfolio->lagBooklvl[i]._price*levelWeightageVector[i]);
                        totalBuyWeigthage += levelWeightageVector[i];
                }
        }
        buyBookAvgPrice = buyBookAvgPrice/totalBuyWeigthage;
        if(useAbsOffset)
        {
                if(fairBuyPrice > buyBookAvgPrice + avgOrderToBookThreshold_ABS){
                        fairBuyPrice = buyBookAvgPrice + avgOrderToBookThreshold_ABS;
                        fairBuyPrice = priceCheck(fairBuyPrice,0);
                }
        }
        else
        {
                if(fairBuyPrice > buyBookAvgPrice * (1 + avgOrderToBookThreshold_BPS)){
                        fairBuyPrice = buyBookAvgPrice * (1 + avgOrderToBookThreshold_BPS);
                        fairBuyPrice = priceCheck(fairBuyPrice,0);
                }
        }
}

void Jumper::calSellBookAvg()
{
        sellBookAvgPrice = 0;
        double totalSellWeigthage = 0;

        for(int i = 5; i < 5 + avgBookLevels ; i++)
        {
                if(_portfolio->lagBooklvl[i]._price > 0)
                {
                        sellBookAvgPrice += (_portfolio->lagBooklvl[i]._price*levelWeightageVector[i - 5]);
                        totalSellWeigthage += levelWeightageVector[i - 5];
                }
        }
        sellBookAvgPrice = sellBookAvgPrice/totalSellWeigthage;
        if(useAbsOffset)
        {
                if(fairSellPrice < sellBookAvgPrice - avgOrderToBookThreshold_ABS){
                        fairSellPrice = sellBookAvgPrice - avgOrderToBookThreshold_ABS;
                        fairSellPrice = priceCheck(fairSellPrice,1);
                }
        }
        else
        {
                if(fairSellPrice < sellBookAvgPrice * (1 - avgOrderToBookThreshold_BPS)){
                        fairSellPrice = sellBookAvgPrice * (1 - avgOrderToBookThreshold_BPS);
                        fairSellPrice = priceCheck(fairSellPrice,1);
                }
        }
}

void Jumper::findNearestBuyLevel(){
    int iter = 0;
    jumperQuoteBookBuyLevel = -1;
    while(iter < bookLength){
        if(_portfolio->lagBooklvl[iter]._price <= fairBuyPrice){
            jumperQuoteBookBuyLevel = iter;
            break;
        }
        iter++;
    }
    if(jumperQuoteBookBuyLevel == -1){
        jumperQuoteBookBuyLevel = 4;
        //cancel open orders , dont know what to do or use totalCummulativeQty
    }
}

void Jumper::findNearestSellLevel(){
    int iter = bookLength;
    jumperQuoteBookSellLevel = -1;
    while(iter < 2*bookLength){
        if(_portfolio->lagBooklvl[iter]._price >= fairSellPrice){
            jumperQuoteBookSellLevel = iter;
            break;
        }
        iter++;
    }
    if(jumperQuoteBookSellLevel == -1){
        jumperQuoteBookSellLevel = 9; 
        //cancel open orders , dont know what to do or use totalCummulativeQty
    }
}

void Jumper::findBuyPriceTopToBottom(){

    int iter = jumperQuoteBookBuyLevel;
    int candidate = -1;
    int final = -1;
    while(iter < bookLength)
    {
        if(_portfolio->lagBooklvl[iter]._quantity <= buyJoinQty){
            final = iter;
            break;
        }
        else if(_portfolio->lagBooklvl[iter]._quantity >= buyJumpQty){
        //     continue;
        }
        else{
            if(candidate == -1){
                candidate = iter;
            }
        }
        iter++;
    }
    if(final == -1){
        if(candidate != -1){
            jumperQuoteBookBuyLevel = candidate;
        }
    }
    else{
        jumperQuoteBookBuyLevel = final;
    }
}

void Jumper::findBuyPriceBottomToTop()
{
    int iter = bookLength - 1;
    int candidate = -1;
    int final = -1;
    while(iter >= jumperQuoteBookBuyLevel){
        if(_portfolio->lagBooklvl[iter]._quantity <= buyJoinQty){
            final = iter;
            break;
        }
        else if(_portfolio->lagBooklvl[iter]._quantity >= buyJumpQty){
        //     continue;
        }
        else{
            if(candidate == -1){
                candidate = iter;
            }
        }
        iter--;
    }
    if(final == -1){
        if(candidate != -1){
            jumperQuoteBookBuyLevel = candidate;
        }
    }
    else{
        jumperQuoteBookBuyLevel = final;
    }
}
void Jumper::findSellPriceTopToBottom()
{
    int iter = jumperQuoteBookSellLevel;
    int candidate = -1;
    int final = -1;
    while(iter < 2*bookLength)
    {
        if(_portfolio->lagBooklvl[iter]._quantity <= sellJoinQty){
            final = iter;
            break;
        }
        else if(_portfolio->lagBooklvl[iter]._quantity >= sellJumpQty){
        //     continue;
        }
        else{
            if(candidate == -1){
                candidate = iter;
            }
        }
        iter++;
    }
    if(final == -1){
        if(candidate != -1){
            jumperQuoteBookSellLevel = candidate;
        }
    }
    else{
        jumperQuoteBookSellLevel = final;
    }
}
void Jumper::findSellPriceBottomToTop(){
    int iter = (2*bookLength) - 1;
    int candidate = -1;
    int final = -1;
    while(iter >= jumperQuoteBookSellLevel){
        if(_portfolio->lagBooklvl[iter]._quantity <= sellJoinQty){
            final = iter;
            break;
        }
        else if(_portfolio->lagBooklvl[iter]._quantity >= sellJumpQty){
        //     continue;
        }
        else{
            if(candidate == -1){
                candidate = iter;
            }
        }
        iter--;
    }
    if(final == -1){
        if(candidate != -1){
            jumperQuoteBookSellLevel = candidate;
        }
    }
    else{
        jumperQuoteBookSellLevel = final;
    }
}

void Jumper::findBuyPriceCummQty(){
        int iter = jumperQuoteBookBuyLevel;
        int qty = _portfolio->lagBooklvl[iter]._quantity;
        while(iter < bookLength && qty < bidCummQty)
        {
                iter++;
                qty += _portfolio->lagBooklvl[iter]._quantity;
        }
        jumperQuoteBookBuyLevel = iter;

}

void Jumper::findSellPriceCummQty(){
        int iter = jumperQuoteBookSellLevel;
        int qty = _portfolio->lagBooklvl[iter]._quantity;
        while(iter < 2*bookLength && qty < askCummQty)
        {
                iter++;
                qty += _portfolio->lagBooklvl[iter]._quantity;
                
        }
        jumperQuoteBookSellLevel = iter;
}

void Jumper::findBuyLevelPrice()
{
    findNearestBuyLevel();
    if(useCummQty){
        findBuyPriceCummQty();
    }
    else{
        if(topToBottomPriority){
                findBuyPriceTopToBottom();
        }
        else{
                findBuyPriceBottomToTop();
        }
    }
}

void Jumper::findSellLevelPrice()
{
    findNearestSellLevel();
    if(useCummQty){
        findSellPriceCummQty();
    }
    else{
        if(topToBottomPriority){
                findSellPriceTopToBottom();
        }
        else{
                findSellPriceBottomToTop();
        }
    }
}


void Jumper::quoteJumpOrders()
{
#ifdef __DEBUG__
    std::cout<<"Before:"<<fairBuyPrice<<"  "<<fairSellPrice<<std::endl;
#endif

    if(jumperBuySwitch)
    {
        findBuyLevelPrice();
        fairBuyPrice = _portfolio->lagBooklvl[jumperQuoteBookBuyLevel]._price;
        if(useAvgBook)
        {
                calBuyBookAvg();
        }
        if(fairBuyPrice >= _portfolio->lowPrice && ((_portfolio->IncreaseOnSidePositionsNearDPR == true && fairBuyPrice <= _portfolio->originalHighPrice) || (_portfolio->IncreaseOnSidePositionsNearDPR == false  && fairBuyPrice <= _portfolio->highPrice)))
        {
                quoteBuyOrder();
        }
    }
    if(jumperSellSwitch)
    {
        findSellLevelPrice();
        fairSellPrice = _portfolio->lagBooklvl[jumperQuoteBookSellLevel]._price;
        if(useAvgBook)
        {
                calSellBookAvg();
        }
        if(fairSellPrice <= _portfolio->highPrice && ((_portfolio->IncreaseOnSidePositionsNearDPR == true && fairSellPrice >= _portfolio->originalLowPrice) || (_portfolio->IncreaseOnSidePositionsNearDPR == false  && fairSellPrice >= _portfolio->lowPrice)))
        {
                quoteSellOrder();
        }
    }

#ifdef __DEBUG__
    _portfolio->printBook();
    std::cout<<"levelChoosed:"<<fairBuyPrice<<"  "<<fairSellPrice<<std::endl;
#endif
}

void Jumper::setPrevFairBuyPrice(){
        lastBuyOrderFairPrice = fairBuyPrice;
}

void Jumper::setPrevFairSellPrice(){
        lastSellOrderFairPrice = fairSellPrice;
}

bool Jumper::canModifyBuyOrder()
{
        if(abs(fairBuyPrice - lastBuyOrderFairPrice) <= 5){
                return false;
        }
        return true;
}

bool Jumper::canModifySellOrder()
{
        if(abs(fairSellPrice - lastSellOrderFairPrice) <= 5){
                return false;
        }
        return true;
}


void Jumper::quoteBuyOrder()
{        
        if(buyWait!=0 || buyQuoteQty <= 0)
        {
                return;
        }
        buyFillQuantity = buyOrderHandler.second.fillQuantity;
        {
                if(lastBuyOrderId == 0)
                {
                        lastBuyOrderId = OrderIDStartingNumber;
                        setPrevFairBuyPrice();
                        _portfolio->sendNew('B', buyQuoteQty,fairBuyPrice,OrderIDStartingNumber++ , _jumperType); 
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

void Jumper::quoteSellOrder()
{        
        if(sellWait!=0 || sellQuoteQty <= 0)
        {
                return;
        }
        sellFillQuantity = sellOrderHandler.second.fillQuantity;
        {
                if(lastSellOrderId == 0)
                {
                        lastSellOrderId = OrderIDStartingNumber;
                        setPrevFairSellPrice();
                        _portfolio->sendNew('S', sellQuoteQty,fairSellPrice,OrderIDStartingNumber++ , _jumperType);
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

void Jumper::cancelOpenBuyOrder(){
        if(buyWait == 0){
                if(lastBuyOrderId !=0 ){
                        _portfolio->sendCancel('B',lastBuyOrderId); 
                        buyWait++;
                }
        }
}

void Jumper::cancelOpenSellOrder(){
        if(sellWait == 0){
                if(lastSellOrderId !=0 ){
                        _portfolio->sendCancel('S' ,lastSellOrderId); 
                        sellWait++;
                }
        }
}


void Jumper::cancelOpenOrderOnEndTime(){
        if(position >= 0){
               cancelOpenBuyOrder();
        }
        if(position <= 0){
               cancelOpenSellOrder();
        }
}

void Jumper::cancelOpenOrder(){
        cancelOpenBuyOrder();
        cancelOpenSellOrder();
}

void Jumper::stopPortfolio()
{
        std::cout<<"Portfolio_Stopped:"<<_portfolio->laggerSymbol<<std::endl;
        cancelOpenOrder();
}

void Jumper::startPortfolio()
{
        std::cout<<"Portfolio_Started:"<<_portfolio->laggerSymbol<<std::endl;
}


void Jumper::endPortfolio(int bid , int ask){
        // lagBidPrice = bid;
        // lagAskPrice = ask;
        // squareOff();
        // sqauredOff = true;
}


void Jumper::onResponse(char responseType, long long int orderID, std::string symbol, char side, int fillQuantity,int price, int16_t errorCode, char orderType, int openQuantity)
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
                std::cout<<_jumperType<<"|Trade:"<<symbol<<"|"<<side<<"|"<<fillQuantity<<"|"<<price<<"|"<<openQuantity<<"|FAIRBUYPRICE:"<<fairBuyPrice<<"|ORDER_ID:"<<orderID<<"|buyFillQuantity:"<<buyOrderHandler.second.fillQuantity<<std::endl;
           
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
                std::cout<<_jumperType<<"|Trade:"<<symbol<<"|"<<side<<"|"<<fillQuantity<<"|"<<price<<"|"<<openQuantity<<"|FAIRSELLPRICE:"<<fairSellPrice<<"|ORDER_ID:"<<orderID<<"|sellFillQuantity:"<<sellOrderHandler.second.fillQuantity<<std::endl;
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
                std::cout<<_jumperType<<":ORDER_REJECTED_INTERNAL_ERROR_CODE:"<<errorCode<<"|price:"<<price<<"|side:"<<side<<std::endl;
                return;
        }
        else if(responseType == 'R' && side =='S' && orderType == 'N'){
                std::cout<<_jumperType<<"|REJECT_SELL_ORDER_NEW:"<<errorCode<<"|Symbol:"<<symbol<<"|Price:"<<price<<"|ORDER_ID:"<<orderID<<"|openQuantity:"<<openQuantity<<std::endl;
                lastSellOrderId = 0;
                sellOrderHandler.first = 0;

        }
        else if(responseType == 'R' && side =='B' && orderType == 'N'){
                std::cout<<_jumperType<<"|REJECT_BUY_ORDER_NEW:"<<errorCode<<"|Symbol:"<<symbol<<"|Price:"<<price<<"|ORDER_ID:"<<orderID<<"|openQuantity:"<<openQuantity<<std::endl;
                lastBuyOrderId = 0;
                buyOrderHandler.first = 0;

        }
        else if(responseType == 'R' && side =='B' && orderType == 'M'){
                std::cout<<_jumperType<<"|REJECT_BUY_ORDER_MODIFY:"<<errorCode<<"|Price:"<<price<<"|ORDER_ID:"<<orderID<<"|sellFillQuantity:"<<buyOrderHandler.second.fillQuantity<<std::endl;
                // lastBuyOrderId = 0;

        }
        else if(responseType == 'R' && side =='S' && orderType == 'M'){
                std::cout<<_jumperType<<"|REJECT_SELL_ORDER_MODIFY:"<<errorCode<<"|Price:"<<price<<"|ORDER_ID:"<<orderID<<"|sellFillQuantity:"<<sellOrderHandler.second.fillQuantity<<std::endl;
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

        if(side == 'B' && responseType != 'T' && responseType != 'P' && errorCode != 17080 ){
                buyWait--;
        }

        if(side == 'S' && responseType != 'T' && responseType != 'P' && errorCode != 17080){
                sellWait--;
        }
}


void Jumper::stopBuying()
{
        jumperBuySwitch = false;
        cancelOpenBuyOrder();
}

void Jumper::stopSelling()
{   
        jumperSellSwitch = false;
        cancelOpenSellOrder();
}

void Jumper::startBuying()
{
        jumperBuySwitch = original_buy_switch;
}

void Jumper::startSelling()
{       
        jumperSellSwitch = original_sell_switch;
}

void Jumper::squareOffQuoter()
{
        cancelOpenOrder();
        position = 0;
        buyQuoteQty = originalBuyQuoteQty; 
        sellQuoteQty = originalSellQuoteQty; 

}
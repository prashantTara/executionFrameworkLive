#include"portfolio.h"
#include"strategy.h"

Portfolio::Portfolio(){}

Portfolio::Portfolio(std::string exeConfigPath_  , portfolioSymbols portfolioSymbols_ , int portfolioId_ ,  Strategy * strat_){
        _portfolioId = portfolioId_;
        _portfolioSymbols = portfolioSymbols_;
        initaliseTime();      
        _exeConfigPath = exeConfigPath_ ;
        std::cout<<"_exeConfigPath:"<<_exeConfigPath<<std::endl;
        _exeConfig = std::make_shared<exeConfig>(_exeConfigPath);
        strat = strat_;
        leaderSymbol = _portfolioSymbols.leaderVector[0];
        laggerSymbol = _portfolioSymbols.lagger;
        std::pair<int64_t , int64_t> dpr = strat->getDPR(laggerSymbol);
        lowPrice = dpr.first;
        highPrice = dpr.second;
        originalLowPrice = lowPrice ;
        originalHighPrice = highPrice;
        PassiveStopfromDprBPS = _exeConfig->getPassiveStopfromDpr();
        if(PassiveStopfromDprBPS > 0){
                lowPrice = originalLowPrice * (1 + PassiveStopfromDprBPS);
                highPrice = originalHighPrice * (1 - PassiveStopfromDprBPS);
        }
        lowPrice = priceCheck(lowPrice , 1);
        highPrice = priceCheck(highPrice , 0);
        stopOnDprHit = _exeConfig->getStopOnDprHit();
        IncreaseOnSidePositionsNearDPR = _exeConfig->getIncreaseOnSidePositionsNearDPR();
        
        std::cout<<"laggerSymbol:"<<laggerSymbol<<"|lowPrice:"<<lowPrice<<"|highPrice:"<<highPrice<<std::endl;
        startTime = _exeConfig->getStartTime();
        endTime = _exeConfig->getEndTime();
        squareOffTime = _exeConfig->getSquareOffTime();
        clearPositionTime = _exeConfig->getClearPositionTime();
        useLtpFilter = _exeConfig->getLtpFilter();
        useJumper = _exeConfig->useJumper();
        useMultiQuoter = _exeConfig->useMultiQuoter();
        if(useLtpFilter == true)
        {
                ltpRollingWindow = _exeConfig->getLtpRollingWindow();
                bidLtpWindow = std::vector<int64_t>(ltpRollingWindow , 0);
                askLtpWindow = std::vector<int64_t>(ltpRollingWindow , 0);
        }
        std::string fairPriceType = _exeConfig->getFairPriceMethod();
        if(fairPriceType == "MID_PRICE")
        {
                _fairPrice = std::make_shared<fairPriceMidPrice>(laggerSymbol , _exeConfig->getFairPriceParams());
        }
        else if(fairPriceType == "BID_ASK")
        {
                _fairPrice = std::make_shared<fairPriceBidAsk>(laggerSymbol , _exeConfig->getFairPriceParams());
        }

        _calSupport = std::make_shared<calSupport>(laggerSymbol , _exeConfig->getSupportParams() , this);
        _jumper = std::make_shared<Jumper>( _exeConfig->getJumperParams() , this);
        _multiQuoter = std::make_shared<MultiQuoter>(laggerSymbol , _multiQuoterType , _exeConfig->getMultiQuoterParams() ,this );
        quoterParamsMap = _exeConfig->getQuoterParamsMap();
        for(auto q : quoterParamsMap){
                quoterMap[q.first] = std::make_shared<Quoter>(laggerSymbol , q.first , q.second ,this );
        }
        sniperParamsMap = _exeConfig->getSniperParamsMap();
        for(auto s : sniperParamsMap){
                sniperMap[s.first] = std::make_shared<Sniper>(laggerSymbol , s.first , s.second ,this );
        }
        { 
                std::ifstream sLConfigFileStream("lotsize.txt");
                if (sLConfigFileStream.is_open())
                {
                        std::string line;
                        while(std::getline(sLConfigFileStream, line)){
                                std::istringstream is_line(line);
                                std::string key;
                                if(std::getline(is_line, key, '='))
                                {       
                                        std::string value;
                                        if (std::getline(is_line, value))
                                                {
                                                        if(key == laggerSymbol){
                                                                lotSize = std::stoi(value);
                                                        }
                                                }
                                        }
                        }
                }
                std::cout<<"lotSize:"<<laggerSymbol<<"|"<<lotSize<<std::endl;
        }
        if(lotSize == 1){
                getLotSizeFromData = true;
        }

        _stopLoss = make_shared<stopLoss>(_exeConfig->getStopLossParams(), lotSize , this);
        _positionManager = make_shared<positionManager>(_exeConfig->getPositionManagerParams() , this);

        if(!_exeConfig->getMainSwitch())
        {
                if(!_exeConfig->getMainBuySwitch()){
                        stopBuying();
                }
                if(!_exeConfig->getMainSellSwitch()){
                        stopSelling();
                }
        }
}

int Portfolio::priceCheck(int price , bool type){
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

void Portfolio::resetParams()
{
        std::cout<<"ModifyRequested:"<<_portfolioId<<"|laggerSymbol:"<<laggerSymbol<<"|leaderSymbol:"<<leaderSymbol<<std::endl;
        _exeConfig = std::make_shared<exeConfig>(_exeConfigPath);
        startTime = _exeConfig->getStartTime();
        endTime = _exeConfig->getEndTime();
        squareOffTime = _exeConfig->getSquareOffTime();
        clearPositionTime = _exeConfig->getClearPositionTime();
        
        std::pair<int64_t , int64_t> dpr = strat->getDPR(laggerSymbol);
        lowPrice = dpr.first;
        highPrice = dpr.second;
        originalLowPrice = lowPrice ;
        originalHighPrice = highPrice;
        PassiveStopfromDprBPS = _exeConfig->getPassiveStopfromDpr();
        if(PassiveStopfromDprBPS > 0){
                lowPrice = originalLowPrice * (1 + PassiveStopfromDprBPS);
                highPrice = originalHighPrice * (1 - PassiveStopfromDprBPS);
        }
        lowPrice = priceCheck(lowPrice , 1);
        highPrice = priceCheck(highPrice , 0);
        stopOnDprHit = _exeConfig->getStopOnDprHit();
        IncreaseOnSidePositionsNearDPR = _exeConfig->getIncreaseOnSidePositionsNearDPR();
        useLtpFilter = _exeConfig->getLtpFilter();
        useJumper = _exeConfig->useJumper();
        useMultiQuoter = _exeConfig->useMultiQuoter();
        _calSupport->setParams(_exeConfig->getSupportParams());
        _jumper->setParams(_exeConfig->getJumperParams());
        _multiQuoter->setParams(_exeConfig->getMultiQuoterParams());
        quoterParamsMap = _exeConfig->getQuoterParamsMap();
        for(auto q : quoterParamsMap){
                quoterMap[q.first]->setParams(q.second);
        }
        sniperParamsMap = _exeConfig->getSniperParamsMap();
        for(auto s : sniperParamsMap){
                sniperMap[s.first]->setParams(s.second);
        }
        _stopLoss->setParams(_exeConfig->getStopLossParams());
        _stopLoss->sqauredOff = false;
        _positionManager->setParams(_exeConfig->getPositionManagerParams());
        if(!_exeConfig->getMainSwitch())
        {
                if(!_exeConfig->getMainBuySwitch()){
                        stopBuying();
                }
                if(!_exeConfig->getMainSellSwitch()){
                        stopSelling();
                }
        }

        std::cout<<"ModifyDone:"<<_portfolioId<<std::endl;

}

void Portfolio::printBook(){
        for(int i = 0;i<=4;i++){
                std::cout<<lagBooklvl[i]._quantity<<"    "<<lagBooklvl[i]._price<<"          "<<lagBooklvl[i+5]._price<<"   "<<lagBooklvl[i+5]._quantity<<std::endl;
        }
        std::cout<<std::endl;
}

void Portfolio::printBookDebug(){
        std::cout<<"|BBook|";
        for(int i = 0;i<=4;i++){
                std::cout<<lagBooklvl[i]._price<<","<<lagBooklvl[i]._quantity<<":";
        }
        std::cout<<"|ABook|";
        for(int i = 5;i<=9;i++){
                std::cout<<lagBooklvl[i]._price<<","<<lagBooklvl[i]._quantity<<":";
        }
        std::cout<<std::endl;
}


void Portfolio::initaliseTime(){

#ifdef __LIVEMODE__
        long long num = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        long long curDateEpoch = num - num%86400;
        curDateEpoch -= 315532800;
        curDateEpoch += 33300;
        currMinStartingEpoch = curDateEpoch*1e9;
#endif
#ifndef __LIVEMODE__
        struct tm t = {0};
        // t.tm_year = 90 + stoi(Date.substr(2,2));
        t.tm_year = 100 + stoi(Date.substr(2,2));
        t.tm_mon = stoi(Date.substr(4,2))-1;
        t.tm_mday = stoi(Date.substr(6));
        t.tm_hour = 14;
        t.tm_min = 45;
        t.tm_sec = 0;
        time_t timeSinceEpoch = mktime(&t);
        currMinStartingEpoch=((long long int)timeSinceEpoch-315532800)*1e9;
        std::cout<<"currMinStartingEpoch:"<<currMinStartingEpoch<<std::endl;
#endif
	
}

bool Portfolio::TBTMinutelyTimer(long long int curTime)
{
        if((currMinStartingEpoch+1000000000)<=curTime)
        {
                //while((currMinStartingEpoch+60000000000)<=curTime) 
                while((currMinStartingEpoch+1000000000)<=curTime) 
                {
                        //currMinStartingEpoch+=60000000000;
                        currMinStartingEpoch+=1000000000;
                        stategyInternalMinutelyTimer++;
                        if((stategyInternalMinutelyTimer%100)%60==0)
                        {
                                stategyInternalMinutelyTimer=(stategyInternalMinutelyTimer/100)*100 + 100;
                                if((((stategyInternalMinutelyTimer/100)%100)%60) == 0)
                                        stategyInternalMinutelyTimer=(stategyInternalMinutelyTimer/10000)*10000 + 10000;
                        }
                }
                return true;
        }
        return false;
}
 

void Portfolio::updateData(std::string symbol , uint8_t type, uint8_t side , int64_t price,int quantity, BookLevel booklvl[10] , long long int time , int lot, int bestBid , int bestAsk )
{
        TBTMinutelyTimer(time);
        if(stategyInternalMinutelyTimer >= clearPositionTime){
                _stopLoss->clearPositionTimePassed = true;        
        }
        if(stategyInternalMinutelyTimer >= squareOffTime){
                _stopLoss->sqauredOff = true;
                _stopLoss->stopLossWait = true;
                _stopLoss->sqauredOffTimePassed = true;
        }
        if(startTimePassed == false){
                if(stategyInternalMinutelyTimer >= startTime){
                        startTimePassed = true;
                }
        }
        if(stategyInternalMinutelyTimer >= endTime){
                setEndTimePassed();
        }

        if(stopOnDprHit)
        {
                if(symbol == laggerSymbol)
                {
                        if(bestBid >= highPrice || bestAsk <= lowPrice)
                        {
                                std::cout<<"HIT:"<<lowPrice<<"|"<<booklvl[0]._price<<"|"<<booklvl[5]._price<<"|"<<highPrice<<std::endl;
                                 _stopLoss->sqauredOff = true;
                                _stopLoss->stopLossWait = true;
                                _stopLoss->sqauredOffTimePassed = true;
                        }
                }
        }
        if(_stopLoss->sqauredOff == true)
        {
                if(symbol == laggerSymbol){
                        _stopLoss->stopLossSquareOff(bestBid , bestAsk);
                }
                return;
        }

        if(symbol == laggerSymbol){
                memcpy(lagBooklvl , booklvl , 10*sizeof(*booklvl));
                #ifdef __DEBUG__
                        printBook();
                #endif
                if(getLotSizeFromData){
                        lotSize = lot;
                }
        }

        int64_t bid = bestBid;
        int64_t ask = bestAsk;

        if(symbol == leaderSymbol){
                _fairPrice->calculateFairPrice(LEADLAG::LEAD , type , price, bid , ask);
                _calSupport->calculateSupport(LEADLAG::LEAD , type , side , price, quantity/lotSize, bid , ask , time);
        }
        if(symbol == laggerSymbol){
                _fairPrice->calculateFairPrice(LEADLAG::LAG , type , price, bid , ask);
                if(leaderSymbol != leaderSymbol){
                        _calSupport->calculateSupport(LEADLAG::LAG , type , side ,  price, quantity/lotSize, bid , ask , time);
                }
                if(useLtpFilter){
                        if(type == 'T'){
                                calLtpFiller(side ,price);
                        }
                }
        }
        double fairBuyPrice = _fairPrice->fairBuyPrice;
        double fairSellPrice = _fairPrice->fairSellPrice;

#ifdef __DEBUG__
        if(symbol == laggerSymbol)
        {
                std::cout<<"Before:"<<bid<<","<<fairBuyPrice<<","<<fairSellPrice<<","<<ask<<std::endl;
        }
#endif

        if(fairBuyPrice == 0 || fairSellPrice == 0){
                return;
        }

        if(!_calSupport->supportSkewQuotes(fairBuyPrice , fairSellPrice, time))
        {     
                return;
        }

        _positionManager->skewQuotes(fairBuyPrice , fairSellPrice);

        
#ifdef __DEBUG__
        if(symbol == laggerSymbol)
        {
                // std::cout<<"after:"<<bid<<","<<fairBuyPrice<<","<<fairSellPrice<<","<<ask<<std::endl;
                if(type == 'T'){
                        if(side == 'S')
                        {
                                tradeCount = tradeCount - (quantity/lotSize);
                        }
                        else{
                                tradeCount = tradeCount + (quantity/lotSize);
                        }
                        std::cout<<"T:"<<laggerSymbol<<"|"<<price<<"|"<<quantity<<"|"<<side<<" |TQ:"<<tradeCount<<"| "<<fairBuyPrice<<"| "<<fairSellPrice<<std::endl;
                }
        }
#endif
        if(symbol == laggerSymbol && bid < ask)
        {
                if(_stopLoss->checkStopLoss(bid , ask) == true)
                {
                        return;
                }                 
        }
        if(startTimePassed == true && _stopLoss->stopLossWait == false  && portfolioStart == true){
                for(auto sniper : sniperMap)
                {           
                        if(symbol == laggerSymbol)
                        {
                                sniper.second->updateData(LEADLAG::LAG ,fairBuyPrice , fairSellPrice, bid , ask);
                        }
                        else
                        {
                                sniper.second->updateData(LEADLAG::LEAD ,fairBuyPrice , fairSellPrice , bid , ask);
                        }               
                }
                for(auto quoter : quoterMap)
                {           
                        if(symbol == laggerSymbol)
                        {
                                quoter.second->calQuotes(LEADLAG::LAG ,fairBuyPrice , fairSellPrice, bid , ask);
                        }
                        else
                        {
                                quoter.second->calQuotes(LEADLAG::LEAD ,fairBuyPrice , fairSellPrice , bid , ask);
                        }               
                }
                if(symbol == laggerSymbol)
                {
                        if(useJumper)
                        {
                                _jumper->updateData(time , fairBuyPrice , fairSellPrice);
                        }
                }
                if(useMultiQuoter)
                {
                       if(symbol == laggerSymbol)
                        {
                                _multiQuoter->calQuotes(LEADLAG::LAG ,fairBuyPrice , fairSellPrice, bid , ask);
                        }
                        else
                        {
                                _multiQuoter->calQuotes(LEADLAG::LEAD ,fairBuyPrice , fairSellPrice , bid , ask);
                        }
                }
        }  
}

void Portfolio::setEndTimePassed()
{        
        endTimePassed = true;
        if(_positionManager->position > 0){
                stopBuying();
        }
        else if(_positionManager->position < 0){
                stopSelling();
        }
        else{
                stopBuying();
                stopSelling();
        }          
}

void Portfolio::stopPortfolio(){
       
        portfolioStart = false;
        cancelAllOpenOrders();

}


void Portfolio::startPortfolio(){
       
       portfolioStart = true;
}


void Portfolio::endAllPortfolio(std::string symbol , int bid , int ask){
        // if(symbol == laggerSymbol){
        //         for(auto quoter : quoterMap){
        //                 quoter.second->endPortfolio(bid ,ask);
        //         }
        // }

}

void Portfolio::onResponse(char responseType, long long int orderID, std::string symbol, char side, int fillQuantity,int price, int16_t errorCode, char orderType, int openQuantity)
{
        fillQuantity = fillQuantity/lotSize;
        openQuantity = openQuantity/lotSize;
        if(orderIdQuoterMap_.find(orderID) != orderIdQuoterMap_.end())
        {
                std::string module = orderIdQuoterMap_[orderID];
                if(module == _jumperType){
                        _jumper->onResponse(responseType , orderID , symbol, side, fillQuantity , price , errorCode , orderType , openQuantity);
                }
                else if(module == _multiQuoterType){
                        _multiQuoter->onResponse(responseType , orderID , symbol, side, fillQuantity , price , errorCode , orderType , openQuantity);
                }
                else{
                        quoterMap[module]->onResponse(responseType , orderID , symbol, side, fillQuantity , price , errorCode , orderType , openQuantity);
                }
        }
        else if(orderIdSniperMap_.find(orderID) != orderIdSniperMap_.end())
        {
                std::string module = orderIdSniperMap_[orderID];
                sniperMap[module]->onResponse(responseType , orderID , symbol, side, fillQuantity , price , errorCode , orderType , openQuantity);

        }
        else if(curStopLossOrderId == orderID){
                _stopLoss->onResponse(responseType , orderID , symbol, side, fillQuantity , price , errorCode , orderType , openQuantity);
        }

        if(responseType == 'T')
        {
                if(side == 'B')
                {
                        _positionManager->updatePosition(fillQuantity);
                        _stopLoss->updatePositionAndValue(fillQuantity , price , 0);
                }
                else if(side == 'S')
                {
                        _positionManager->updatePosition(-fillQuantity);
                        _stopLoss->updatePositionAndValue(-fillQuantity , 0 , -price);
                }
        }
}

void Portfolio::sendCancel(char side ,long long oid)
{
        send(_portfolioId , side, lotSize,0, laggerSymbol, 'X',oid, 0,0,0); 
}

void Portfolio::sendNew(char side , int qty , int px, long long oid , std::string quoterType )
{        
        orderIdQuoterMap_[oid] = quoterType;
        quoterOrderIdMap_[quoterType] = oid;
        send(_portfolioId , side, qty*lotSize ,px , laggerSymbol, 'N' ,oid , 0,0,0); 
}

void Portfolio::sendNewIOC(char side , int qty , int px, long long oid , std::string sniperType )
{        
        orderIdSniperMap_[oid] = sniperType;
        sniperOrderIdMap_[sniperType] = oid;
        send(_portfolioId , side, qty*lotSize ,px , laggerSymbol, 'N' ,oid , 0,0,1); 
}

void Portfolio::sendNewStopLoss(char side , int qty , int px, long long oid )
{     
        curStopLossOrderId = oid;
        send(_portfolioId , side, qty*lotSize ,px , laggerSymbol, 'N' ,oid , 0,0,0); 
}

void Portfolio::sendModify(char side , int qty , int px, long long oid  , int fillQuantity)
{     
        send(_portfolioId , side, qty*lotSize ,px , laggerSymbol, 'M' ,oid , 0,fillQuantity*lotSize,0); 
}

void Portfolio::send(int portfolioId , char side, int qty, int px, string symbol, char orderType,long long oid,bool matchRemainingInNeXtBarOverRide,int fillQuantity,int ioc)
{
        // #ifdef debugStrat
                std::cout<<"TS:"<<strat->currentTime<<"|Send|Side:"<<side<<"|Quantity:"<<qty<<"|Price:"<<px<<"|Symbol:"<<symbol<<"|OrderType:"<<orderType<<"|Oid:"<<oid<<"|FillQuantity:"<<fillQuantity<<"|IOC:"<<ioc<<"|PortfolioId:"<<portfolioId;
                printBookDebug();
        // #endif
        strat->send(portfolioId , side,qty,px,symbol,orderType,oid,matchRemainingInNeXtBarOverRide,fillQuantity,qty,ioc);
}


void Portfolio::stopBuying()
{
        for(auto quoter : quoterMap)
        {           
                quoter.second->stopBuying();
        }
        for(auto sniper : sniperMap)
        {           
                sniper.second->stopBuying();
        }
        if(useJumper){
                _jumper->stopBuying();
        }
        if(useMultiQuoter){
                _multiQuoter->stopBuying();
        }
}

void Portfolio::stopSelling()
{
        for(auto quoter : quoterMap)
        {           
                quoter.second->stopSelling();
        }
        for(auto sniper : sniperMap)
        {           
                sniper.second->stopSelling();
        }
        if(useJumper){
                _jumper->stopSelling();
        }
        if(useMultiQuoter){
                _multiQuoter->stopSelling();
        }
        
}

void Portfolio::startBuying()
{
        for(auto quoter : quoterMap)
        {           
                quoter.second->startBuying();
        }
        for(auto sniper : sniperMap)
        {           
                sniper.second->startBuying();
        }
        if(useJumper){
                _jumper->startBuying();
        }
        if(useMultiQuoter){
                _multiQuoter->startBuying();
        }
}

void Portfolio::startSelling()
{
        for(auto quoter : quoterMap)
        {           
                quoter.second->startSelling();
        }  
        for(auto sniper : sniperMap)
        {           
                sniper.second->startSelling();
        }
        if(useJumper){
                _jumper->startSelling();
        }
        if(useMultiQuoter){
                _multiQuoter->startSelling();
        }
}

void Portfolio::squareOffQuoter()
{
        for(auto quoter : quoterMap)
        {           
                quoter.second->squareOffQuoter();
        }
        if(useJumper){
                _jumper->squareOffQuoter();
        }
        if(useMultiQuoter){
                _multiQuoter->squareOffQuoter();
        }
}

void Portfolio::cancelAllOpenOrders()
{
        for(auto quoter : quoterMap)
        {           
                quoter.second->cancelOpenOrder();
        }
        if(useJumper){
                _jumper->cancelOpenOrder();
        }
        if(useMultiQuoter){
                _multiQuoter->cancelOpenOrder();
        }
}

double Portfolio::getDirectionShortWindow()
{
        return _calSupport->shortRollingTrendSlope;
}

double Portfolio::getDirectionLongWindow()
{
        return _calSupport->longRollingTrendSlope;
}


void Portfolio::calLtpFiller(uint8_t side , int64_t price)
{

        if(side == 'B'){
                if(price == prevTradeBidPrice){
                        return;
                }
                bidLtpWindow[bidLtpWindowIndex++] = price;
                prevTradeBidPrice = price;
                if(bidLtpWindowIndex == ltpRollingWindow){
                        bidLtpWindowIndex = 0;
                }
                findBidLtp();
        }
        if(side == 'S'){
                if(price == prevTradeAskPrice){
                        return;
                }
                askLtpWindow[askLtpWindowIndex++] = price;
                prevTradeAskPrice = price;
                if(askLtpWindowIndex == ltpRollingWindow){
                        askLtpWindowIndex = 0;
                }
                findAskLtp();
        }
}

void Portfolio::findBidLtp()
{
        minBuyLtp = INT_MAX;
        maxBuyLtp = 0;
        for(int i = 0; i < ltpRollingWindow ; i++){
                if(bidLtpWindow[i] == 0){
                        continue;
                }
                minBuyLtp = std::min(minBuyLtp , bidLtpWindow[i]);
                maxBuyLtp = std::max(maxBuyLtp , bidLtpWindow[i]);
        }
}
void Portfolio::findAskLtp()
{
        minSellLtp = INT_MAX;
        maxSellLtp = 0;
        for(int i = 0; i < ltpRollingWindow ; i++){
                if(askLtpWindow[i] == 0){
                        continue;
                }
                minSellLtp = std::min(minSellLtp , askLtpWindow[i]);
                maxSellLtp = std::max(maxSellLtp , askLtpWindow[i]);
        }
}

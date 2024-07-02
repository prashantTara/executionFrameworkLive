#include "strategy.h"
#include <fstream>
#include <sstream>
#include "portfolio.h"

int counter=0;
Strategy::Strategy(string filepath)
{
    std::cout<<filepath<<std::endl;
    _config = std::make_shared<Config>(filepath);          
#ifdef __LIVEMODE__
        contractFile = _config->getContractFile();
        readContractFile(contractFile);
#endif
    useUncrossedBook = _config->getUseUncrossedBook(); 
    symbolVector = _config->getSymbolVectorPair();
    symbolVectorPortfolioParams = _config->getSymbolVectorPortfolioParams();
    _portfolioIdMap = std::vector<Portfolio*>(symbolVectorPortfolioParams.size() , nullptr);
    initialiseProtfolio();
    // startCoutThread("logProcessingDump.txt");


}

void Strategy::startCoutThread(std::string logpath)
{
        _printingClass = std::make_shared<PrintingClass>(logpath);
        dumpOut = _printingClass->getDumpAddress();
#ifndef __LIVEMODE__
        // _asyncCout = std::async(std::launch::async, &PrintingClass::checkPrint, _printingClass.get());
#endif
}

void Strategy::printSpread(std::string symbol , int spread ,int buySpread , int sellSpread){
        spr spr_;
        spr_.symbol = symbol;
        spr_.sp = spread;
        spr_.mean = buySpread;
        spr_.sd = sellSpread;
        printString(spr_);
}

void Strategy::printString(spr s)
{
        mtx.lock();
        sQueue.push(s);
        mtx.unlock();
}

void Strategy::resetParams()
{

}

void Strategy::subscribeSymbols(){
        for(auto sym : subscribeSymbols_){ //pop too 
                std::cout<<sym<<std::endl;
                connector->subscribeToSymbol(sym);
        }
        subscribeSymbols_.clear();
}

void Strategy::uncrossBook(BookLevel booklvl[10]){
        
        int bestBid = booklvl[0]._price;
        int bestAsk = booklvl[5]._price;
        int bidQty = booklvl[0]._quantity;
        int askQty = booklvl[5]._quantity;
        int i = 1;
        int j = 6;
        while(1){
                if(bestBid < bestAsk || bestAsk == 0 || bestBid == 0){
                        break;
                }
                else if(askQty > bidQty){
                        askQty -= bidQty;
                        if(i == 5){
                                bestBid = 0;
                                break;
                        }
                        bestBid = booklvl[i]._price;
                        bidQty = booklvl[i]._quantity;
                        i++;
                }
                else if(askQty < bidQty){
                        bidQty -= askQty;
                        if(j == 10){
                                bestAsk = 0;
                                break;
                        }
                        bestAsk = booklvl[j]._price;
                        askQty = booklvl[j]._quantity;
                        j++;
                }
                else if(bidQty == askQty){
                        if(i == 5){
                                bestBid = 0;
                                break;
                        }
                        if(j == 10){
                                bestAsk = 0;
                                break;
                        }
                        bestBid = booklvl[i]._price;
                        bidQty = booklvl[i]._quantity;
                        i++;
                        bestAsk = booklvl[j]._price;
                        askQty = booklvl[j]._quantity;
                        j++;
                }
        }
        symbolbestBid = bestBid;
        symbolbestAsk = bestAsk;

#ifdef __DEBUG__
        std::cout<<symbolbestBid << " " << symbolbestAsk <<std::endl;
        for(int i = 0;i<=4;i++){
                std::cout<<booklvl[i]._quantity<<"    "<<booklvl[i]._price<<"          "<<booklvl[i+5]._price<<"   "<<booklvl[i+5]._quantity<<std::endl;
        }
        std::cout<<std::endl;
        std::cout<<std::endl;
#endif

}

void Strategy::printBookDebug(BookLevel booklvl[10]){
        std::cout<<"|BBook|";
        for(int i = 0;i<=4;i++){
                std::cout<<booklvl[i]._price<<";"<<booklvl[i]._quantity<<",";
        }
        std::cout<<"|ABook|";
        for(int i = 5;i<=9;i++){
                std::cout<<booklvl[i]._price<<";"<<booklvl[i]._quantity<<",";
        }
        std::cout<<std::endl;
}

// MINUTELY
void Strategy::updateData(std::string symbol, std::string date,long long int time,int open,int high, int low, int close, float volume,int OI, int lot, std::string expiry, std::string callput)
{
}

// TBT
void Strategy::updateData(std::string symbol,uint8_t type,long long int time,uint8_t side,long long int price,int quantity,std::string expiry,int strike,int lot,std::string callput,BookLevel booklvl[10], long long int orderId1, long long int orderId2)
{
        currentTime = time;
        symbolbestBid = booklvl[0]._price;
        symbolbestAsk = booklvl[5]._price;
        if(symbolbestBid == 0 && symbolbestAsk == 0)
        {
                return;
        }
        
        if(symbolbestBid== 0 || symbolbestAsk == 0){
                if(symbolbestBid == 0)
                {
                        symbolbestBid = symbolbestAsk;
                }
                else{
                        symbolbestAsk = symbolbestBid;
                }
                #ifdef debugStrat
                std::cout<<"Tick:"<<symbol<<"|"<<type<<"|"<<time<<"|"<<side<<"|"<<price<<"|"<<quantity<<"|"<<"uncrossBid:"<<symbolbestBid<<"|uncrossAsk:"<<symbolbestAsk;
                printBookDebug(booklvl);
                #endif
                for(auto id : symbolToPortfolioId[symbol]){
                        _portfolioIdMap[id]->updateData(symbol , type ,side , price , quantity, booklvl , time , lot , symbolbestBid , symbolbestAsk);
                }
                return;
        }

        if(useUncrossedBook && symbolbestBid >= symbolbestAsk){
                uncrossBook(booklvl);
                if(symbolbestBid == 0 || symbolbestAsk == 0){
                        return;
                }
        }
        #ifdef debugStrat
        std::cout<<"Tick:"<<symbol<<"|"<<type<<"|"<<time<<"|"<<side<<"|"<<price<<"|"<<quantity<<"|"<<"uncrossBid:"<<symbolbestBid<<"|uncrossAsk:"<<symbolbestAsk;
        printBookDebug(booklvl);
        #endif
        for(auto id : symbolToPortfolioId[symbol]){
                _portfolioIdMap[id]->updateData(symbol , type ,side , price , quantity, booklvl , time , lot , symbolbestBid , symbolbestAsk);

        }
}

void Strategy::initialiseProtfolio(){
        for(auto pr : symbolVectorPortfolioParams){
                Portfolio *pt = new Portfolio(pr.first , pr.second , portfolioId , this);
                for( auto sym : symbolVector[pr.first]){
                        symbolToPortfolioId[sym].insert(portfolioId);
                        subscribeSymbols_.insert(sym);
                }
                _portfolioIdMap[portfolioId] = pt;
                portfolioId++;

        }
}

#ifdef SimSnapshot
// SNAPSHOT
 void Strategy::updateData(std::string smbl,int open,int high,int low,int close,float volume,int LTP,int LTQ,int LTT,int ATP,int lot,std::string expiry,int bidPriceArray[5],int bidQuantityArray[5],int     askPriceArray[5],int askQuantityArray[5]/*std::vector<std::pair<int,int>> buyDepth,std::vector<std::pair<int,int>> sellDepth*/,long long int time)
{
}
#endif
void Strategy::processTopOfBook(std::string symbol, BookLevel booklvl[10])
{
    cout<<"Top Of The Book\n";
    for(int i=0; i<10; i++)
    {
        cout<<i+1<<" "<<booklvl[i]._price<<" "<<booklvl[i]._quantity<<"\n";
    }
}

// MINUTELY API
void Strategy::currMinuteBarsFinished()
{

}

void Strategy::checkSignal(string symbol)
{

}

void Strategy::checkSL(string symbol)
{

}

void Strategy::onResponse(char responseType, int16_t errorCode, long long int orderID, std::string symbol, char side, int openQuantity,int fillQuantity,int price, char orderType)
{
    //#ifdef debugStrat
		std::cout<<"TS:"<<currentTime<<"|StrategyResponse:"<<responseType<<"|"<<orderID<<"|"<<symbol<<"|"<<side<<"|"<<fillQuantity<<"|"<<price<<"|"<<errorCode<<"|"<<orderType<<"|"<<openQuantity<<"|PortfolioId:"<<internalOrderIdToPortfolioMap[orderID]<<std::endl;
//	#endif
    _portfolioIdMap[internalOrderIdToPortfolioMap[orderID]]->onResponse( responseType,  orderID,  symbol,  side,  fillQuantity, price,  errorCode,  orderType,  openQuantity);
}

void Strategy::setStrategyConnector(StrategyConnector *stc)
{
	connector = stc;
        subscribeSymbols();

}

void Strategy::send(int portfolioId, char side, int qty, int px, string symbol, char orderType,long long oid,bool matchRemainingInNeXtBarOverRide,int fillQuantity,int disclosedQuantity, int ioc)
{
	internalOrderIdToPortfolioMap[oid] = portfolioId;
	connector->send(side,qty,px,symbol,orderType,oid,matchRemainingInNeXtBarOverRide,fillQuantity,disclosedQuantity,ioc);
}

void Strategy::onSignal1(){}
void Strategy::onSignal2(){}
void Strategy::onSignal3(){}

void Strategy::onSignal4(){
        #ifdef __LIVEMODE__
                readContractFile(contractFile);
        #endif
        for(int i =0 ; i < _portfolioIdMap.size() ; i++){
                _portfolioIdMap[i]->resetParams();
        }
}


void Strategy::signalCallback(int signal){

        if(signal == 4)
        {
               onSignal4(); 
        }
        
}

void Strategy::TopOfBook(std::string symbol, BookLevel *booklvl){}
/*void Strategy::getTopFromBook(std::string symbol,int numOfLevels,std::vector<pair<long long,long long>> &topBuy,std::vector<pair<long long,long long>> &topSell)
{
        connector->getTopFromBook(symbol,numOfLevels,topBuy,topSell);
}*/
void Strategy::timerEvent(){}

// By Som
// void Strategy::getTopOfBook(std::string symbol)
// {
//     connector->getTopOfBook(symbol);
// }

// long long int Strategy::getOrderId()
// {
//     return connector->getOrderId();
// }

// long long int Strategy::getOriginalOrderId(long long int orderId)
// {
//     return connector->getOriginalOrderId(orderId);
// }

// long long int Strategy::getLatestOrderId(long long int orderId)
// {
//     return connector->getLatestOrderId(orderId);
// }



void Strategy::readContractFile(std::string file)
{
        std::ifstream myFile(file);
	if(!myFile.is_open())
	{
		// printCurrentTime();
		std::cout << "error opening contract file\n";
		return;
	}

	std::string tempString;
	getline(myFile,tempString);             // ignoring line 1 of securiy file
	while(!myFile.eof())
	{
		getline(myFile,tempString);             // extracting line from file
		if(tempString.length()==0)
			continue;

		std::vector<std::string> v;                       // contains delimiter seperated strings
		std::stringstream ss(tempString);

		while (ss.good()) {
			getline(ss, tempString, ',');
			v.push_back(tempString);
		}

//		if(v[2] != "EQ" && v[2] != "BE")	// consider only BE and EQ
//			continue;

		std::string symbol;
                int lowprice = 0 ;
                int highprice = 0 ;
#ifdef __CMCONFIG__
		symbol = v[1] + "_" + v[2];
                std::string inputString = v[17];
                std::stringstream sps(inputString); 
                std::string token; 
                std::vector<std::string> tokens; 
                char delimiter = '-';  
                while (getline(sps, token, delimiter)) 
                { 
                          tokens.push_back(token); 
                } 
                lowprice = (std::stod(tokens[0]) * 100);
                highprice = (std::stod(tokens[1]) * 100);
#endif
#ifdef __FOCONFIG__
		if (v[6] == "XX")
			symbol = v[3] + "_" + v[4] + "_" + v[2].substr(0, 3);
		else if (v[6] == "CE" || v[6] == "PE")
			symbol = v[3] + "_" + v[4] + "_" + v[5] + "_" + v[6];

                lowprice = std::stoi(v[25]);
                highprice = std::stoi(v[26]);
#endif
#ifdef __CDSCONFIG__
		if (v[2] == "FUTCUR")
			symbol = v[3] + "_" + v[4] + "_" + "FUT";
		else if (v[2] == "OPTCUR")
			symbol = v[3] + "_" + v[4] + "_" + v[5] + "_" + v[6];
#endif
		std::cout<<symbol<<"|"<<lowprice<<"|"<<highprice<<std::endl;
                symbolToDPR[symbol] = std::make_pair(lowprice , highprice);
    }
}


std::pair<int64_t , int64_t> Strategy::getDPR(std::string symbol)
{
        // symbolToDPR[symbol] = std::make_pair(1315,2020);
        if(symbolToDPR.find(symbol) == symbolToDPR.end())
        {
                return make_pair(INT_MIN , INT_MAX);
        }
        return symbolToDPR[symbol];
}
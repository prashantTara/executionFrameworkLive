#ifndef __STRATEGY_H__
#define __STRATEGY_H__
#include <bits/stdc++.h>
#ifndef __LIVEMODE__
	#include "/home/infra/InternalSimulatorUsers/Backtester/strategyConnector.h"
#endif
#ifdef __LIVEMODE__
	#include "/home/dev/StrategyBox/strategyConnector.h"
	#include "/home/dev/StrategyBox/structures.h"
#endif
#include <math.h>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <map>
#include <unordered_map>
#include <map>
#include <memory>
#include <future>
#include<string>
#include <sstream> 
#include"printingClass.h"
#include"globalVar.h"

#include "config.h"
#include "frameworkStructures.h"

class Portfolio;
class Config;
class Strategy{
	public:
		Strategy(string);
		// MINUTELY
        void updateData(std::string symbol,std::string date,long long int time,int open,int high, int low, int close, float volume,int OI, int lot, std::string expiry, std::string callput);
        // TBT
        void updateData(std::string symbol,uint8_t,long long int time,uint8_t side,long long int price,int quantity,std::string expiry,int strike,int lot,std::string callput,BookLevel booklvl[10], long long int orderId1, long long int orderId2);
		#ifdef SimSnapshot
		// SNAPSHOT
        void updateData(std::string smbl,int open,int high,int low,int close,float volume,int LTP,int LTQ,int LTT,int ATP,int lot,std::string expiry,int bidPriceArray[5],int bidQuantityArray[5],int askPriceArray[5],int askQuantityArray[5]/*std::vector<std::pair<int,int>> buyDepth,std::vector<std::pair<int,int>> sellDepth*/,long long int time);
	//	void updateData(std::string smbl,int open,int high,int low,int close,float volume,int LTP,int LTQ,std::string LTT,int ATP,int lot,std::string expiry,std::vector<std::pair<int,int>> buyDepth,std::vector<std::pair<int,int>> sellDepth,long long int time);
		#endif
		void checkSignal(std::string);
		void checkSL(std::string);
		
		//void onResponse(char responseType, long long int orderID, std::string symbol, char side, int fillQuantity,int price, int16_t errorCode, char orderType, int openQuantity);
		void onResponse(char responseType, int16_t errorCode, long long int orderID, std::string symbol, char side, int openQuantity,int fillQuantity,int price, char orderType);
        void setStrategyConnector(StrategyConnector *stc);
		void send(int portfolioId, char side, int qty, int px, string symbol, char orderType,long long oid,bool matchRemainingInNeXtBarOverRide,int fillQuantity,int disclosedQuantity,int ioc);
		void currMinuteBarsFinished();

		void onSignal1();
		void onSignal2();
		void onSignal3();
		void onSignal4();
		void signalCallback(int signal);
		void TopOfBook(std::string, BookLevel *);
		void timerEvent();
        //By Som
        void processTopOfBook(std::string symbol, BookLevel booklvl[]);
        void getTopOfBook(std::string symbol);
        long long int getOrderId();
        long long getOriginalOrderId(long long int);
        long long getLatestOrderId(long long int);

		void initialiseProtfolio();
		void resetParams();
		void startCoutThread(std::string logpath);
		void printString(spr s);
		void printSpread(std::string symbol , int spread, int buySpread , int sellSpread);
		
		void subscribeSymbols();
		void uncrossBook(BookLevel booklvl[10]);
		void printBookDebug(BookLevel booklvl[10]);
		void readContractFile(std::string file);
		std::pair<int64_t , int64_t> getDPR(std::string symbol);

	private:
		StrategyConnector *connector;
		std::string stratconfigfilepath;
		std::vector<Portfolio*> _portfolioIdMap;
		unordered_map<std::string , std::set<int>> symbolToPortfolioId;
		std::shared_ptr<PrintingClass> _printingClass;
		std::future<void> _asyncCout;
		std::ofstream* dumpOut;
		std::set<std::string> subscribeSymbols_;
		std::shared_ptr<Config> _config;
		std::string exeConfigPath;
		std::unordered_map<std::string,std::vector<std::string>> symbolVector;
		std::unordered_map<std::string, portfolioSymbols> symbolVectorPortfolioParams;
		int portfolioId = 0;
		std::unordered_map<long long , int> internalOrderIdToPortfolioMap;
		bool useUncrossedBook = false;
	public:
		long long currentTime = 0;
		std::unordered_map<std::string , std::pair<int64_t , int64_t>> symbolToDPR;
		int symbolbestBid = 0;
		int symbolbestAsk = 0;
		std::string contractFile;
};

#endif
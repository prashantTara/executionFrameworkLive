#ifndef __PORTFOLIO_H__
#define __PORTFOLIO_H__
#include<string.h>
#include<unordered_map>
#include<vector>
#include<memory>
#include"printingClass.h"
#ifndef __LIVEMODE__
	#include "/home/infra/InternalSimulatorUsers/Backtester/strategyConnector.h"
#endif
#ifdef __LIVEMODE__
	#include "/home/dev/StrategyBox/strategyConnector.h"
	#include "/home/dev/StrategyBox/structures.h"
#endif
#include "calSupport.h"
#include "exeConfig.h"
#include "fairPrice.h"
#include "fairPriceLib/fairPriceMidPrice.h"
#include "fairPriceLib/fairPriceBidAsk.h"
#include "enums.h"
#include "Quoter.h"
#include "Sniper.h"
#include "stopLoss.h"
#include "Jumper.h"
#include "positionManager.h"
#include "latency.h"
#include "multiQuoter.h"
using namespace std;
class exeConfig;
class fairPrice;
class Strategy;
class Quoter;
class stopLoss;
class positionManager;
class calSupport;
class Latency;
class Jumper;
class Sniper;
class MultiQuoter;
class Portfolio{

    public:
        Portfolio();

        Portfolio(std::string exeConfigPath_ , portfolioSymbols portfolioSymbols_ , int portfolioId_ , Strategy * strat_);
        void updateData(std::string symbol , uint8_t type, uint8_t side , int64_t price,int quantity, BookLevel booklvl[10] , long long int time , int lot , int bestBid , int bestAsk);
		void onResponse(char responseType, long long int orderID, std::string symbol, char side, int fillQuantity,int price, int16_t errorCode, char orderType, int openQuantity);
        void printBook();
        void printBookDebug();
        void cancelOrder(long long int id ,char side);
        void resetParams();
        void stopPortfolio();
        void startPortfolio();
        void endAllPortfolio(std::string symbol , int bid , int ask);
        void setEndTimePassed();
		void sendCancel(char side, long long oid);
        void sendNew( char side , int qty , int px, long long oid , std::string quoterType  );
        void sendNewIOC( char side , int qty , int px, long long oid , std::string sniperType  );
        void sendNewStopLoss( char side , int qty , int px, long long oid  );
        void sendModify( char side , int qty , int px, long long oid , int fillQuantity  );
        void send(int portfolioId , char side, int qty, int px, string symbol, char orderType,long long oid,bool matchRemainingInNeXtBarOverRide,int fillQuantity,int ioc);
        void stopBuying();
        void stopSelling();
        void startBuying();
        void startSelling();
        void squareOffQuoter();
        bool TBTMinutelyTimer(long long int curTime);
        void initaliseTime();
        void cancelAllOpenOrders();

        double getDirectionShortWindow();
        double getDirectionLongWindow();

        void calLtpFiller(uint8_t side , int64_t price);
        void findBidLtp();
        void findAskLtp();
        int priceCheck(int price , bool type);


    public:
        std::shared_ptr<exeConfig> _exeConfig;
        std::shared_ptr<fairPrice> _fairPrice;
        std::shared_ptr<stopLoss> _stopLoss;
        std::shared_ptr<positionManager> _positionManager;
        std::shared_ptr<calSupport> _calSupport;
        std::shared_ptr<Jumper> _jumper;
        std::shared_ptr<MultiQuoter> _multiQuoter;
        std::string leaderSymbol;
        std::string laggerSymbol;
        std::string baseAsset;
        std::vector<std::string> quoterVector;
        std::unordered_map<std::string , std::shared_ptr<Quoter>> quoterMap;
        std::unordered_map<std::string , std::shared_ptr<Sniper>> sniperMap;
        std::string fairPriceCalType;
        Strategy * strat = nullptr;
        int lotSize = 1;
        bool getLotSizeFromData = false;
        bool sqauredOff = false;
        long long int initialTime = 0 ;
        bool foundInParams = false;
        bool endTimePassed = false;
        bool startTimePassed = false;
        bool portfolioStart = true;
        BookLevel lagBooklvl[10];
        std::unordered_map<long long , std::string> orderIdQuoterMap_;
        std::unordered_map<long long , std::string> orderIdSniperMap_;
        std::unordered_map<std::string, long long> quoterOrderIdMap_;
        std::unordered_map<std::string, long long> sniperOrderIdMap_;
        std::string _exeConfigPath;
        std::unordered_map<std::string , quoterParams> quoterParamsMap;
        std::unordered_map<std::string , sniperParams> sniperParamsMap;
        long long int startTime = 91500;
		long long int endTime = 150000;
		long long int squareOffTime = 152000;
		long long int clearPositionTime = 152000;
		long long int currMinStartingEpoch;
		long long int stategyInternalMinutelyTimer = 91500;
        double PassiveStopfromDprBPS = 0;
        long long int curStopLossOrderId = 0;
        int tradeCount = 0;
        Latency _latencyMap;

        portfolioSymbols _portfolioSymbols;
        int _portfolioId = 0;

        bool useLtpFilter = false;
		int ltpRollingWindow = 5;
        std::vector<int64_t> bidLtpWindow;
        int bidLtpWindowIndex = 0;
        int askLtpWindowIndex = 0;
        std::vector<int64_t> askLtpWindow;
        int64_t minBuyLtp = 0;
        int64_t maxBuyLtp = 0;
        int64_t minSellLtp = 0;
        int64_t maxSellLtp = 0;

        int64_t prevTradeBidPrice = 0;
        int64_t prevTradeAskPrice = 0;

        bool useJumper = false;
        std::string _jumperType = "Jumper";

        bool useMultiQuoter = false;
        std::string _multiQuoterType = "MultiQuoter";

        int64_t lowPrice;
        int64_t highPrice;
        int64_t originalLowPrice;
        int64_t originalHighPrice;
        bool stopOnDprHit = false;
        bool IncreaseOnSidePositionsNearDPR = false;


};


#endif

//#include "/home/dev/StrategyBox/strategyConnector.h"
#include "strategy.h"

std::shared_ptr<Strategy> _strategy;
StrategyConnector::StrategyConnector(StrategyManager* strategyManager_, std::string strategyConfigFilePath_)
{
	_strategyManager = strategyManager_;
	_strategy = std::make_shared<Strategy>(strategyConfigFilePath_);
	_strategy-> setStrategyConnector(this);

}
void StrategyConnector::onSignal1()
{
	_strategy->onSignal1();
}
void StrategyConnector::onSignal2()
{
	_strategy->onSignal2();
}
void StrategyConnector::onSignal3()
{
	_strategy->onSignal3();
}
void StrategyConnector::onSignal4()
{
	_strategy->onSignal4();
}

void StrategyConnector::onResponse(char responseType, long long int orderID, std::string symbol, char side, int fillQuantity,int price, int16_t errorCode, char orderType, int openQuantity)
{
	//_strategy->onResponse(responseType, orderID, symbol, side, openQuantity, price, errorCode, orderType, fillQuantity);
	_strategy->onResponse(responseType, errorCode, orderID, symbol, side, openQuantity, fillQuantity, price, orderType);
}
void StrategyConnector::onMarketData(std::string symbol,uint8_t messageType, long long int time, uint8_t side, int price, int quantity, std::string expiry, int strike, int lot, std::string callput, BookLevel level[10],double orderId1,double orderId2)
{
	_strategy->updateData(symbol, messageType, time, side, price, quantity, expiry, strike, lot, callput, level,orderId1,orderId2);
}
void StrategyConnector::onMarketData(std::string symbol, std::string date,long long int time,int open,int high, int low, int close, float volume,int OI, int lot, std::string expiry, std::string callput)
{
	_strategy->updateData(symbol, date, time, open, high, low, close, volume, OI, lot, expiry, callput);
	_strategy->checkSignal(symbol);
	_strategy->checkSL(symbol);
}
		//void send(char side, int qty, int px, string symbol, char orderType,long long orderID, bool matchRemainingInNeXtBarOverRide, int fillQuantity=0);
//void StrategyConnector::send(char side, int qty, int px, string symbol, char orderType,long long orderId, bool nextBarMatching, int fillQuantity)
void StrategyConnector::send(char side, int qty, int px,std::string symbol, char orderType,long long orderId, bool matching, int fillQuantity, int disclosedQuantity, int iocFlag)
{
	//_strategyManager->sendOrder(side, qty, px, symbol, orderType, orderId, nextBarMatching, fillQuantity);
	_strategyManager->sendOrder(side, qty, px, symbol, orderType, orderId,matching, fillQuantity, disclosedQuantity, iocFlag);
}

void StrategyConnector::subscribeToSymbol(std::string symbol)
{
	_strategyManager->subscribeToSymbol(symbol);
}

void StrategyConnector::currMinuteBarsFinished()
{
	_strategy->currMinuteBarsFinished();
}
void StrategyConnector::processTimerEvent()
{
}
void StrategyConnector::processTopOfBook(std::string symbol,BookLevel records[])
{
}
void StrategyConnector::signalCallback(int signal)
{
	_strategy->signalCallback(signal);
}

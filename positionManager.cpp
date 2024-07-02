#include "positionManager.h"


positionManager::positionManager(){}
positionManager::~positionManager(){}


positionManager::positionManager(positionManagerParams positionManagerParams_ , Portfolio * portfolio_ )
{
    _portfolio = portfolio_;
    setParams(positionManagerParams_);
    std::cout<<"PORTFOLIO_MANAGER:ACTIVE"<<std::endl;
}

void positionManager::setParams(positionManagerParams positionManagerParams_){

    usePositionManager = positionManagerParams_.usePositionManager;
    useAbsoluteOffset = positionManagerParams_.useAbsoluteOffset;
    positionShiftQty = positionManagerParams_.positionShiftQty;
    maxPosition = positionManagerParams_.maxPosition;
    bidDecreaseSkewFactor_BPS = positionManagerParams_.bidDecreaseSkewFactor_BPS;
    bidIncreaseSkewFactor_BPS = positionManagerParams_.bidIncreaseSkewFactor_BPS;
    askDecreaseSkewFactor_BPS = positionManagerParams_.askDecreaseSkewFactor_BPS;
    askIncreaseSkewFactor_BPS = positionManagerParams_.askIncreaseSkewFactor_BPS;
    bidDecreaseSkewFactor_ABS = positionManagerParams_.bidDecreaseSkewFactor_ABS;
    bidIncreaseSkewFactor_ABS = positionManagerParams_.bidIncreaseSkewFactor_ABS;
    askDecreaseSkewFactor_ABS = positionManagerParams_.askDecreaseSkewFactor_ABS;
    askIncreaseSkewFactor_ABS = positionManagerParams_.askIncreaseSkewFactor_ABS;
    bidDecreaseMaxShift = positionManagerParams_.bidDecreaseMaxShift;
    bidIncreaseMaxShift = positionManagerParams_.bidIncreaseMaxShift;
    askDecreaseMaxShift = positionManagerParams_.askDecreaseMaxShift;
    askIncreaseMaxShift = positionManagerParams_.askIncreaseMaxShift;
    updateSkewFactors();

}


void positionManager::updateSkewFactors()
{
    if(useAbsoluteOffset)
    {
        updateSkewFactorsABS();
    }
    else{
        updateSkewFactorsBPS();
    }
}


void positionManager::skewQuotes(double & fairBuyPrice , double & fairSellPrice)
{
    if(useAbsoluteOffset)
    {
        fairBuyPrice += buySkewFactor;
        fairSellPrice += sellSkewFactor;
    }
    else{
        fairBuyPrice = fairBuyPrice * buySkewFactor;
        fairSellPrice = fairSellPrice * sellSkewFactor;
    }
}

void positionManager::updateSkewFactorsABS()
{
    if(position == 0){
        buySkewFactor = 0;
        sellSkewFactor = 0;
        return;
    }

    int level = (abs(position) + 1)/positionShiftQty;
    if(level > 0)
    {
        if(position > 0)
        {
            int buyDecreaseLevel = level;
            if(buyDecreaseLevel > bidDecreaseMaxShift){
                buyDecreaseLevel = bidDecreaseMaxShift;
            }
            buySkewFactor = 0;
            while(buyDecreaseLevel--){
                buySkewFactor = buySkewFactor - bidDecreaseSkewFactor_ABS;
            }
            int sellDecreaseLevel = level;
            if(sellDecreaseLevel > askDecreaseMaxShift){
                sellDecreaseLevel = askDecreaseMaxShift;
            }
            sellSkewFactor = 0;
            while(sellDecreaseLevel--){
                sellSkewFactor = sellSkewFactor - askDecreaseSkewFactor_ABS;
            }
        }
        else if(position < 0)
        {
            int buyIncreaseLevel = level;
            if(buyIncreaseLevel > bidIncreaseMaxShift){
                buyIncreaseLevel = bidIncreaseMaxShift;
            }
            buySkewFactor = 0;
            while(buyIncreaseLevel--){
                buySkewFactor = buySkewFactor + bidIncreaseSkewFactor_ABS;
            }
            int sellIncreaseLevel = level;
            if(sellIncreaseLevel > askIncreaseMaxShift){
                sellIncreaseLevel = askIncreaseMaxShift;
            }
            sellSkewFactor = 0;
            while(sellIncreaseLevel--){
                sellSkewFactor = sellSkewFactor + askIncreaseSkewFactor_ABS;
            }
        }
    }
}

void positionManager::updateSkewFactorsBPS()
{
     if(position == 0){
        buySkewFactor = 1;
        sellSkewFactor = 1;
        return;
    }

    int level = (abs(position) + 1)/positionShiftQty;
    if(level > 0)
    {
        if(position > 0)
        {
            int buyDecreaseLevel = level;
            if(buyDecreaseLevel > bidDecreaseMaxShift){
                buyDecreaseLevel = bidDecreaseMaxShift;
            }
            double buyFactor = 1 - bidDecreaseSkewFactor_BPS;
            buySkewFactor = 1;
            while(buyDecreaseLevel--){
                buySkewFactor = buySkewFactor * buyFactor;
            }
            int sellDecreaseLevel = level;
            if(sellDecreaseLevel > askDecreaseMaxShift){
                sellDecreaseLevel = askDecreaseMaxShift;
            }
            double sellFactor = 1 - askDecreaseSkewFactor_BPS;
            sellSkewFactor = 1;
            while(sellDecreaseLevel--){
                sellSkewFactor = sellSkewFactor * sellFactor;
            }
        }
        else if(position < 0)
        {
            int buyIncreaseLevel = level;
            if(buyIncreaseLevel > bidIncreaseMaxShift){
                buyIncreaseLevel = bidIncreaseMaxShift;
            }
            double buyFactor = 1 + bidIncreaseSkewFactor_BPS;
            buySkewFactor = 1;
            while(buyIncreaseLevel--){
                buySkewFactor = buySkewFactor * buyFactor;
            }
            int sellIncreaseLevel = level;
            if(sellIncreaseLevel > askIncreaseMaxShift){
                sellIncreaseLevel = askIncreaseMaxShift;
            }
            double sellFactor = 1 + askIncreaseSkewFactor_BPS;
            sellSkewFactor = 1;
            while(sellIncreaseLevel--){
                sellSkewFactor = sellSkewFactor * sellFactor;
            }
        }
    }
}



void positionManager::updatePosition(int pos){
    position += pos;
    updateSkewFactors();
    if(position >= maxPosition || position <= -maxPosition){
        maxPositionHit();
    }
    else if(buyingStopped == true || sellingStopped == true){
        startQuoters();
    }
}


void positionManager::maxPositionHit(){

    if(position >= maxPosition ){
        buyingStopped = true; 
        _portfolio->stopBuying();
    }
    else if(position <= -maxPosition ){
        sellingStopped = true; 
        _portfolio->stopSelling();
    }
}


void positionManager::startQuoters(){

    if(buyingStopped == true ){
        buyingStopped = false; 
        _portfolio->startBuying();
    }
    else if(sellingStopped == true ){
        sellingStopped = false; 
        _portfolio->startSelling();
    }
}






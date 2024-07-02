#ifndef __FAIRPRICE_H__
#define __FAIRPRICE_H__

#include <memory>
#include <vector>
#include <iostream>
#include "enums.h"
#include "frameworkStructures.h"

class fairPrice
{
    public : 
        fairPrice(){}
        fairPrice(std::string symbol , fairPriceParams fairPriceParams_){}
        virtual ~fairPrice(){}
        virtual void calculateFairPrice(int leaderLag , uint8_t type , int64_t price , int64_t bidPrice , int64_t askPrice) = 0;

    public :
        double fairBuyPrice = 0;
        double fairSellPrice = 0;

};


#endif
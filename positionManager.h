#ifndef __POSITIONMANAGER_H__
#define __POSITIONMANAGER_H__

#include "frameworkStructures.h"
#include "portfolio.h"
class Portfolio;

class positionManager
{
    public :
        positionManager();
        positionManager(positionManagerParams positionManagerParams_ , Portfolio * portfolio_ );
        ~positionManager();
        void setParams(positionManagerParams positionManagerParams_);
        void updatePosition(int pos);
        void updateSkewFactors();
        void updateSkewFactorsABS();
        void updateSkewFactorsBPS();
        void maxPositionHit();
        void startQuoters();
        void skewQuotes(double & fairBuyPrice , double & fairSellPrice);

    public : 
        Portfolio * _portfolio = nullptr;
        bool usePositionManager = true;
        bool useAbsoluteOffset = false;
        int positionShiftQty = 0;
        int maxPosition = 0;
        double bidDecreaseSkewFactor_BPS = 0;
        double bidIncreaseSkewFactor_BPS = 0;
        double askDecreaseSkewFactor_BPS = 0;
        double askIncreaseSkewFactor_BPS = 0;
        double bidDecreaseSkewFactor_ABS = 0;
        double bidIncreaseSkewFactor_ABS = 0;
        double askDecreaseSkewFactor_ABS = 0;
        double askIncreaseSkewFactor_ABS = 0;
        int bidDecreaseMaxShift = 0;
        int bidIncreaseMaxShift = 0;
        int askDecreaseMaxShift = 0;
        int askIncreaseMaxShift = 0;


        int position = 0;

        double buySkewFactor = 0;
        double sellSkewFactor = 0;

        bool buyingStopped = false;
        bool sellingStopped = false;

};

#endif
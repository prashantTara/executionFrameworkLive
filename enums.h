#ifndef __ENUMS_H__
#define __ENUMS_H__

enum LEADLAG{
    LEAD = 1,
    LAG = 2
};


enum fairPriceType
{
    MIDPRICE = 1,
    BID_ASK = 2,
    LTP = 3
};

enum QuoterType
{
    Q1 = 1,
    Q2 = 2,
    Q3 = 3,
    BAQ1 = 4,
    BAQ2 = 5,
    BAQ3 = 6
};

enum PORTFOLIO_RATIO_CHECK
{
    STOP = 1,
    START = 2,
    DO_NOTHING = 3,
};

#endif
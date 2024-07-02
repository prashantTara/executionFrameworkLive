#ifndef __FRAMEWORKSTRUCTURES_H__
#define __FRAMEWORKSTRUCTURES_H__

#include <string>
#include <vector>

struct ORDER{
    long long int orderId;
    std::string symbol;
    int price;
    int quantity;
    int fillQuantity =0;
    char status;
    char side;
	ORDER(){}
};

struct portfolioSymbols{

    std::string lagger;
    std::vector<std::string> leaderVector;
    bool useHedger = false;
    std::string hedger;
    bool useCurrencyHedger = false;
    std::string currencyHedger;
};

struct quoterParams{

    bool useAbsoluteOffset = false;
    double bidSpread_BPS = 0;
    double askSpread_BPS = 0;
    double bidSpread_ABS = 0;
    double askSpread_ABS = 0;
    bool useAbsoluteModifyThreshold = false;
    double quoteModifyPassiveThreshold_Bid_BPS = 0;
    double quoteModifyPassiveThreshold_Ask_BPS = 0;
    double quoteModifyAggresiveThreshold_Bid_BPS = 0;
    double quoteModifyAggresiveThreshold_Ask_BPS = 0;
    double quoteModifyPassiveThreshold_Bid_ABS = 0;
    double quoteModifyPassiveThreshold_Ask_ABS = 0;
    double quoteModifyAggresiveThreshold_Bid_ABS = 0;
    double quoteModifyAggresiveThreshold_Ask_ABS = 0;
    bool quoterSwitch = true;
    bool buy_switch = true;
    bool sell_switch = true;
    int buyQuoteQty = 1;
    int sellQuoteQty = 1;
    double quoteQtySkewFactorBuy = 1;
    double quoteQtySkewFactorSell = 1;
    bool useLongTrendSkew = false;
    bool useShortTrendSkew = false;
    bool useTrendOffsetABS = false;
    bool trendSkewInversion = false;
    double longTrendIncreaseSkewOffset_BPS = 0;
    double longTrendDecreaseSkewOffset_BPS = 0;
    double shortTrendIncreaseSkewOffset_BPS = 0;
    double shortTrendDecreaseSkewOffset_BPS = 0;
    double longTrendIncreaseSkewOffset_ABS = 0;
    double longTrendDecreaseSkewOffset_ABS = 0;
    double shortTrendIncreaseSkewOffset_ABS = 0;
    double shortTrendDecreaseSkewOffset_ABS = 0;
    double shortTrendQualifyPercent = 1;
    int maxShortTrendShifts = 1;
    double longTrendQualifyPercent = 1;
    int maxLongTrendShifts = 1;
    bool useLtpFilter = false;
    int minQuotePassiveGap = 0;
    int maxQuotePassiveGap = 0;
};

struct fairPriceParams{

    std::string fairPriceType;
    int rollingWindow = 0;
    bool useAbsoluteOffset = false;
    double fairPriceBidOffsetBPS = 0;
    double fairPriceAskOffsetBPS = 0;
    double fairPriceBidOffsetABS = 0;
    double fairPriceAskOffsetABS = 0;


};


struct supportParams{
    bool usePairLimitRatio = false;
    double maxPairRatio = 0;
    double minPairRatio = 0;
    bool useBidAskLimitRatio = false;
    double primaryMaxBidAskRatio = 0;
    double primaryMinBidAskRatio = 0;
    double secondaryMaxBidAskRatio = 0;
    double secondaryMinBidAskRatio = 0;
    bool useTrendSkew = false;
    int longTermTrendTicks = 100;
    int shortTermTrendTicks = 100;
    

    bool useBigMove = true;
    bool usePriceChangeBigMove = true;
    int priceChangeTimeWindow = 0;
    int priceChangeWindowTicks = 50;
    double priceChangeBigMove = 0;
    bool onBigMoveCancelOrder = false;
    bool useTradeBigMove = true;
    int tradeTimeWindow = 0;
    double tradeBigMoveQty = 0;
    bool changeOrderDelayONBigmove = false;
    bool turnOffQuotingBigMove = true;
    int turnOffQuotingBigMoveForTime = 0;
    bool widenOffSetOnBigMove = true;
    int widenOffSetOnBigMoveForTime = 0;
    bool useWidenOffSetOnBigMoveABS = false;
    double widenOffSetOnBigMoveByBPS = 0;
    double widenOffSetOnBigMoveByABS = 0;


};

struct stopLossParams{
    double slAcThr = 0;
    double trailStopLoss = 0;
    double maxPortfolioLoss = 0;
    double maxTradeLoss = 0;
    bool useTradeJumper = false;
    bool useAvgBook = false;
    bool useAvgBookThresholdABS = false;
    double avgOrderToBookThreshold_BPS = 0;
    double avgOrderToBookThreshold_ABS = 0;
    double bookCrossThreshold = 0;
    int bookLevel = 0;
    int orderDivisionFactor = 1;
};

struct positionManagerParams{
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
};


struct jumperParams{

    bool useJumper = false;
    int buyJoinQty = 0;
    int sellJoinQty = 0;
    int buyJumpQty = 0;
    int sellJumpQty = 0;
    bool useCummQty = false;
    int bidCummQty = 0;
    int askCummQty = 0;
    int bookLength = 5;
    int topToBottomPriority = 0;    
    int bottomToTopPriority = 0;
    bool useAbsOffset = false;
    double bidOffsetBPS = 0;    
    double askOffsetBPS = 0;    
    int bidOffsetABS = 0;    
    int askOffsetABS = 0;
    int buyQuoteQty = 0;
    int sellQuoteQty = 0;
    int buyMaxPosition = 0;
    int sellMaxPosition = 0;
    int modifyDelay = 0;
    bool jumperSwitch = false;
    bool jumperBuySwitch = false;
    bool jumperSellSwitch = false;
    bool useAvgBook = false;
    double avgOrderToBookThreshold_BPS = 0;
    double avgOrderToBookThreshold_ABS = 0;
    int avgBookLevels = 0;
    std::vector<double> levelWeightageVector;
};

struct sniperParams{

    bool useAbsoluteOffset = false;
    bool useSniper = false;
    double buyOpportunity_BPS = 0;
    double sellOpportunity_BPS = 0;
    double buyOpportunity_ABS = 0;
    double sellOpportunity_ABS = 0;
    double bidSpread_BPS = 0;
    double askSpread_BPS = 0;
    double bidSpread_ABS = 0;
    double askSpread_ABS = 0;
    double quoteModifyThreshold_Bid_BPS = 0;
    double quoteModifyThreshold_Ask_BPS = 0;
    double quoteModifyThreshold_Bid_ABS = 0;
    double quoteModifyThreshold_Ask_ABS = 0;
    bool sniperSwitch = true;
    bool buy_switch = true;
    bool sell_switch = true;
    int buyQuoteQty = 1;
    int sellQuoteQty = 1;
    int minBookBuyQtyToFire = 0;
    int minBookSellQtyToFire = 0;
    int buyShootCount = 0;
    int sellShootCount = 0;
    int totalShootCount = 0;
    int buyMaxPosition = 0;
    int sellMaxPosition = 0;
    int booklevel = 5;
};

struct multiQuoterParams
{
    bool useMultiQuoter = false;
    bool useAbsoluteOffset = false;
    int levels = 0;
    double bidSpread_BPS = 0;
    double askSpread_BPS = 0;
    double bidSpread_ABS = 0;
    double askSpread_ABS = 0;
    double bidSpreadIncrement_BPS = 0;
    double askSpreadIncrement_BPS = 0;
    double bidSpreadIncrement_ABS = 0;
    double askSpreadIncrement_ABS = 0;
    double quoteModifyPassiveThreshold_Bid_BPS = 0;
    double quoteModifyPassiveThreshold_Ask_BPS = 0;
    double quoteModifyAggresiveThreshold_Bid_BPS = 0;
    double quoteModifyAggresiveThreshold_Ask_BPS = 0;
    double quoteModifyPassiveThreshold_Bid_ABS = 0;
    double quoteModifyPassiveThreshold_Ask_ABS = 0;
    double quoteModifyAggresiveThreshold_Bid_ABS = 0;
    double quoteModifyAggresiveThreshold_Ask_ABS = 0;
    int buyQuoteQty = 1;
    int sellQuoteQty = 1;
    double quoteQtySkewFactorBuy = 1;
    double quoteQtySkewFactorSell = 1;
    int buyQuoteQtyIncrement = 1;
    int sellQuoteQtyIncrement = 1;
    double buyFillPercentage = 0.5;
    double sellFillPercentage = 0.5;
    int maxBuyPosition = 0;
    int maxSellPosition = 0;
    int orderLimit = 0;
    bool multiQuoterSwitch = true;
    bool buy_switch = true;
    bool sell_switch = true;
    bool onlyPassive = false;
    bool useLtpFilter = false;
    bool useLongTrendSkew = false;
    bool useShortTrendSkew = false;
    bool useTrendOffsetABS = false;
    bool trendSkewInversion = false;
    double longTrendIncreaseSkewOffset_BPS = 0;
    double longTrendDecreaseSkewOffset_BPS = 0;
    double shortTrendIncreaseSkewOffset_BPS = 0;
    double shortTrendDecreaseSkewOffset_BPS = 0;
    double longTrendIncreaseSkewOffset_ABS = 0;
    double longTrendDecreaseSkewOffset_ABS = 0;
    double shortTrendIncreaseSkewOffset_ABS = 0;
    double shortTrendDecreaseSkewOffset_ABS = 0;
    double shortTrendQualifyPercent = 1;
    int maxShortTrendShifts = 1;
    double longTrendQualifyPercent = 1;
    int maxLongTrendShifts = 1;
    int minQuotePassiveGap = 0;
    int maxQuotePassiveGap = 0;
    bool topLevelMinQtyFilter = false;
    int topLevelMinQtyBid = 0;
    int topLevelMinQtyAsk = 0;
    int topLevelBidQtySkewFactor = 0;
    int topLevelAskQtySkewFactor = 0;
    double topLevelBidQtySkewFactorPosNetQty = 1;
    double topLevelBidQtySkewFactorNegNetQty = 1;
    double topLevelAskQtySkewFactorPosNetQty = 1;
    double topLevelAskQtySkewFactorNegNetQty = 1;


};

#endif
#include "exeConfig.h"

exeConfig::exeConfig(std::string pathName_): _pathName(pathName_)
{
    readFile();
}

exeConfig::~exeConfig()
{
}

void exeConfig::readFile()
{
    parseMainFile();
    parseStopLossManagerFile();
    parsePositionManagerFile();
    parseSupportFile();
    parseJumperFile();
    parseMultiQuoterFile();
    parse();
    for(auto q : quoterMap){
        std::cout<<"ReadQuoter:"<<q.first<<std::endl;
        parseQuoterFile(q.first);
    }
    parseSniper();
    for(auto s : sniperMap){
        std::cout<<"ReadSniper:"<<s.first<<std::endl;
        parseSniperFile(s.first);
    }
}

void exeConfig::parseMainFile()
{
     std::ifstream configFileStream(_pathName);
    if (configFileStream.is_open())
    {
        std::string line;
        while (std::getline(configFileStream, line))
        {
            std::istringstream is_line(line);
            std::string key;
            if (std::getline(is_line, key, '='))
            {
                std::string value;
                if (key[0] == '#')
                    continue;
                if (std::getline(is_line, value))
                {
                    if(key == "File")
                    {
                        if(value == "Main.cfg")
                        {
                            std::cout<<"Inside_Main:"<<value<<std::endl;
                            while (std::getline(configFileStream, line))
                            {
                                std::istringstream is_main(line);
                                if (std::getline(is_main, key, '='))
                                {
                                    if (key[0] == '#')
                                        continue;
                                    if (std::getline(is_main, value))
                                    {
                                        if(key == "File")
                                        {
                                            std::cout<<"|MainFile ENDS"<<std::endl<<std::endl;
                                            return;
                                        }
                                        else if(key == "THEORTICAL_PRICE_INPUT_TYPE")
                                        {
                                            std::cout<<"MainFile:THEORTICAL_PRICE_INPUT_TYPE:"<<value<<std::endl;
                                            _fairPriceParams.fairPriceType = value;
                                        }  
                                        else if(key == "ROLLING_WINDOW_THEO_PRICE")
                                        {
                                            std::cout<<"MainFile:ROLLING_WINDOW_THEO_PRICE:"<<value<<std::endl;
                                            _fairPriceParams.rollingWindow = std::stoi(value);
                                        }
                                        else if(key == "USE_PRICE_OFFSET_ABSOLUTE")
                                        {
                                            std::cout<<"MainFile:USE_PRICE_OFFSET_ABSOLUTE:"<<value<<std::endl;
                                            if(value == "true"){
                                                _fairPriceParams.useAbsoluteOffset = true;
                                                _positionManagerParams.useAbsoluteOffset = true;
                                                _supportParams.useWidenOffSetOnBigMoveABS = true;
                                                _jumperParams.useAbsOffset = true;
                                                generalUseAbsoluteOffset = true;
                                            }
                                        }
                                        else if(key == "FAIR_PRICE_BID_OFFSET_BPS")
                                        {
                                            std::cout<<"MainFile:FAIR_PRICE_BID_OFFSET_BPS:"<<value<<std::endl;
                                            _fairPriceParams.fairPriceBidOffsetBPS = std::stod(value);
                                        }
                                        else if(key == "FAIR_PRICE_ASK_OFFSET_BPS")
                                        {
                                            std::cout<<"MainFile:FAIR_PRICE_ASK_OFFSET_BPS:"<<value<<std::endl;
                                            _fairPriceParams.fairPriceAskOffsetBPS = std::stod(value);
                                        }
                                        else if(key == "FAIR_PRICE_BID_OFFSET_ABS")
                                        {
                                            std::cout<<"MainFile:FAIR_PRICE_BID_OFFSET_ABS:"<<value<<std::endl;
                                            _fairPriceParams.fairPriceBidOffsetABS = std::stod(value);
                                        }
                                        else if(key == "FAIR_PRICE_ASK_OFFSET_ASB")
                                        {
                                            std::cout<<"MainFile:FAIR_PRICE_ASK_OFFSET_ASB:"<<value<<std::endl;
                                            _fairPriceParams.fairPriceAskOffsetABS = std::stod(value);
                                        }
                                        else if(key == "SWITCH")
                                        {
                                            std::cout<<"MainFile:SWITCH:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                mainSwitch = false;
                                            }
                                        }
                                        else if(key == "BUY_SWITCH" && mainSwitch == false)
                                        {
                                            std::cout<<"MainFile:BUY_SWITCH:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                mainBuySwitch = false;
                                            }
                                        }
                                        else if(key == "SELL_SWITCH" && mainSwitch == false)
                                        {
                                            std::cout<<"MainFile:SELL_SWITCH:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                mainSellSwitch = false;
                                            }
                                        }
                                        else if(key == "strategyStartTime")
                                        {
                                            std::cout<<"MainFile:strategyStartTime:"<<value<<std::endl;
                                            startTime = std::stoi(value);
                                        }
                                        else if(key == "strategyEndTime")
                                        {
                                            std::cout<<"MainFile:strategyEndTime:"<<value<<std::endl;
                                            squareOffTime = std::stoi(value);
                                        }
                                        else if(key == "lastEntryTime")
                                        {
                                            std::cout<<"MainFile:lastEntryTime:"<<value<<std::endl;
                                            endTime = std::stoi(value);
                                        }
                                        else if(key == "clearPositionTime")
                                        {
                                            std::cout<<"MainFile:clearPositionTime:"<<value<<std::endl;
                                            clearPositionTime = std::stoi(value);
                                        }
                                        else if(key == "USE_LTP_FILTER")
                                        {
                                            std::cout<<"MainFile|USE_LTP_FILTER:"<<value<<std::endl;
                                            if(value == "true")
                                            {
                                                useLtpFilter = true;
                                            }
                                        }
                                        else if(key == "LTP_FILLER_ROLLING_WINDOW")
                                        {
                                            std::cout<<"MainFile|LTP_FILLER_ROLLING_WINDOW:"<<value<<std::endl;
                                            ltpRollingWindow = std::stoi(value);
                                        }
                                        else if(key == "PassiveStopfromDprBPS")
                                        {
                                            std::cout<<"MainFile|PassiveStopfromDprBPS:"<<value<<std::endl;
                                            PassiveStopfromDprBPS = std::stod(value);
                                        }
                                        else if(key == "StopOnDprHit")
                                        {
                                            std::cout<<"MainFile|StopOnDprHit:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                stopOnDprHit = false;
                                            }
                                            else if(value == "true")
                                            {
                                                stopOnDprHit = true;
                                            }
                                        }
                                        else if(key == "IncreaseOnSidePositionsNearDPR")
                                        {
                                            std::cout<<"MainFile|IncreaseOnSidePositionsNearDPR:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                IncreaseOnSidePositionsNearDPR = false;
                                            }
                                            else if(value == "true")
                                            {
                                                IncreaseOnSidePositionsNearDPR = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }   
                    }               
                }
            }
        }
    }
}

void exeConfig::parseSupportFile()
{
     std::ifstream configFileStream(_pathName);
    if (configFileStream.is_open())
    {
        std::string line;
        while (std::getline(configFileStream, line))
        {
            std::istringstream is_line(line);
            std::string key;
            if (std::getline(is_line, key, '='))
            {
                std::string value;
                if (key[0] == '#')
                    continue;
                if (std::getline(is_line, value))
                {
                    if(key == "File")
                    {
                        if(value == "Support.cfg")
                        {
                            std::cout<<"Inside_Support:"<<value<<std::endl;
                            while (std::getline(configFileStream, line))
                            {
                                std::istringstream is_Support(line);
                                if (std::getline(is_Support, key, '='))
                                {
                                    if (key[0] == '#')
                                        continue;
                                    if (std::getline(is_Support, value))
                                    {
                                        if(key == "File")
                                        {
                                            std::cout<<"|Support ENDS"<<std::endl<<std::endl;
                                            return;
                                        }
                                        else if(key == "USE_PAIR_RATIO_LIMIT_RATIO")
                                        {
                                            std::cout<<"Support:USE_PAIR_RATIO_LIMIT_RATIO:"<<value<<std::endl;
                                            if(value == "true"){
                                                _supportParams.usePairLimitRatio = true;
                                            }
                                        }  
                                        else if(key == "MAX_LL_MID_SPREAD_RATIO")
                                        {
                                            std::cout<<"Support:MAX_LL_MID_SPREAD_RATIO:"<<value<<std::endl;
                                            _supportParams.maxPairRatio = std::stod(value);
                                        }
                                        else if(key == "MIN_LL_MID_SPREAD_RATIO")
                                        {
                                            std::cout<<"Support:MIN_LL_MID_SPREAD_RATIO:"<<value<<std::endl;
                                            _supportParams.minPairRatio = std::stod(value);
                                        }
                                        else if(key == "USE_BID_ASK_LIMIT_RATIO")
                                        {
                                            std::cout<<"Support:USE_BID_ASK_LIMIT_RATIO:"<<value<<std::endl;
                                            if(value == "true"){
                                                _supportParams.useBidAskLimitRatio = true;
                                            }
                                        }
                                        else if(key == "PRIMARY_MAXIMUM_BID_ASK_RATIO")
                                        {
                                            std::cout<<"Support:PRIMARY_MAXIMUM_BID_ASK_RATIO:"<<value<<std::endl;
                                            _supportParams.primaryMaxBidAskRatio = std::stod(value);
                                        }
                                        else if(key == "PRIMARY_MINIMUM_BID_ASK_RATIO")
                                        {
                                            std::cout<<"Support:PRIMARY_MINIMUM_BID_ASK_RATIO:"<<value<<std::endl;
                                            _supportParams.primaryMinBidAskRatio = std::stod(value);
                                        }
                                        else if(key == "SECONDARY_MAXIMUM_BID_ASK_RATIO")
                                        {
                                            std::cout<<"Support:SECONDARY_MAXIMUM_BID_ASK_RATIO:"<<value<<std::endl;
                                            _supportParams.secondaryMaxBidAskRatio = std::stod(value);
                                        }
                                        else if(key == "SECONDARY_MINIMUM_BID_ASK_RATIO")
                                        {
                                            std::cout<<"Support:SECONDARY_MINIMUM_BID_ASK_RATIO:"<<value<<std::endl;
                                            _supportParams.secondaryMinBidAskRatio = std::stod(value);
                                        }
                                        else if(key == "USE_TREND_SKEW")
                                        {
                                            std::cout<<"Support:USE_TREND_SKEW:"<<value<<std::endl;
                                            if(value == "true"){
                                                _supportParams.useTrendSkew = true;
                                            }
                                        }
                                        else if(key == "LONG_TERM_TREND_TICKS")
                                        {
                                            std::cout<<"Support:LONG_TERM_TREND_TICKS:"<<value<<std::endl;
                                            _supportParams.longTermTrendTicks = std::stod(value);
                                        }
                                        else if(key == "SHORT_TERM_TREND_TICKS")
                                        {
                                            std::cout<<"Support:SHORT_TERM_TREND_TICKS:"<<value<<std::endl;
                                            _supportParams.shortTermTrendTicks = std::stod(value);
                                        }
                                        else if(key == "USE_BIG_MOVE")
                                        {
                                            std::cout<<"Support:USE_BIG_MOVE:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                _supportParams.useBigMove = false;
                                            }
                                        }
                                        else if(key == "USE_PRICE_CHANGE_BIG_MOVE")
                                        {
                                            std::cout<<"Support:USE_PRICE_CHANGE_BIG_MOVE:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                _supportParams.usePriceChangeBigMove = false;
                                            }
                                        }
                                        else if(key == "TIME_WINDOW_FOR_PRICE_CHANGE_MICRO")
                                        {
                                            std::cout<<"Support:TIME_WINDOW_FOR_PRICE_CHANGE_MICRO:"<<value<<std::endl;
                                            _supportParams.priceChangeTimeWindow = std::stoi(value);
                                        }
                                        else if(key == "PRICE_CHANGE_TICKS_FOR_TIME_WINDOW")
                                        {
                                            std::cout<<"Support:PRICE_CHANGE_TICKS_FOR_TIME_WINDOW:"<<value<<std::endl;
                                            _supportParams.priceChangeWindowTicks = std::stoi(value);
                                        }
                                        else if(key == "TIME_WINDOW_PRICE_CHANGE_BPS")
                                        {
                                            std::cout<<"Support:TIME_WINDOW_PRICE_CHANGE_BPS:"<<value<<std::endl;
                                            _supportParams.priceChangeBigMove = std::stod(value);
                                        }
                                        else if(key == "ON_BIG_MOVE_CANCEL_ORDERS")
                                        {
                                            std::cout<<"Support:ON_BIG_MOVE_CANCEL_ORDERS:"<<value<<std::endl;
                                            if(value == "true")
                                            {
                                                _supportParams.onBigMoveCancelOrder = true;
                                            }
                                        }
                                        else if(key == "USE_TRADE_WINDOW_BIG_MOVE")
                                        {
                                            std::cout<<"Support:USE_TRADE_WINDOW_BIG_MOVE:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                _supportParams.useTradeBigMove = false;
                                            }
                                        }
                                        else if(key == "TIME_WINDOW_FOR_TRADE_QTY_MICRO")
                                        {
                                            std::cout<<"Support:TIME_WINDOW_FOR_TRADE_QTY_MICRO:"<<value<<std::endl;
                                            _supportParams.tradeTimeWindow = std::stod(value);
                                        }
                                        else if(key == "TIME_WINDOW_TRADE_LOT")
                                        {
                                            std::cout<<"Support:TIME_WINDOW_TRADE_LOT:"<<value<<std::endl;
                                            _supportParams.tradeBigMoveQty = std::stod(value);
                                        }
                                        else if(key == "CHANGE_ORDER_DELAY_ON_BIG_MOVE")
                                        {
                                            std::cout<<"Support:CHANGE_ORDER_DELAY_ON_BIG_MOVE:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                _supportParams.changeOrderDelayONBigmove = false;
                                            }
                                        }
                                        else if(key == "TURNOFF_QUOTING_ON_BIG_MOVE")
                                        {
                                            std::cout<<"Support:TURNOFF_QUOTING_ON_BIG_MOVE:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                _supportParams.turnOffQuotingBigMove = false;
                                            }
                                        }
                                        else if(key == "TURNOFF_QUOTING_ON_BIG_MOVE_FOR_TIME_MICROS")
                                        {
                                            std::cout<<"Support:TURNOFF_QUOTING_ON_BIG_MOVE_FOR_TIME_MICROS:"<<value<<std::endl;
                                            _supportParams.turnOffQuotingBigMoveForTime = std::stod(value);
                                        }
                                        else if(key == "WIDEN_OFFSETS_ON_BIG_MOVE")
                                        {
                                            std::cout<<"Support:WIDEN_OFFSETS_ON_BIG_MOVE:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                _supportParams.widenOffSetOnBigMove = false;
                                            }
                                        }
                                        else if(key == "WIDEN_OFFSETS_ON_BIG_MOVE_FOR_TIME_MICROS")
                                        {
                                            std::cout<<"Support:WIDEN_OFFSETS_ON_BIG_MOVE_FOR_TIME_MICROS:"<<value<<std::endl;
                                            _supportParams.widenOffSetOnBigMoveForTime = std::stod(value);
                                        }
                                        else if(key == "WIDEN_OFFSETS_ON_BIG_MOVE_BY_BPS")
                                        {
                                            std::cout<<"Support:WIDEN_OFFSETS_ON_BIG_MOVE_BY_BPS:"<<value<<std::endl;
                                            _supportParams.widenOffSetOnBigMoveByBPS = std::stod(value);
                                        }
                                        else if(key == "WIDEN_OFFSETS_ON_BIG_MOVE_BY_ABS")
                                        {
                                            std::cout<<"Support:WIDEN_OFFSETS_ON_BIG_MOVE_BY_ABS:"<<value<<std::endl;
                                            _supportParams.widenOffSetOnBigMoveByABS = std::stod(value);
                                        }
                                    }
                                }
                            }
                        }   
                    }               
                }
            }
        }
    }
}

void exeConfig::parseStopLossManagerFile()
{
     std::ifstream configFileStream(_pathName);
    if (configFileStream.is_open())
    {
        std::string line;
        while (std::getline(configFileStream, line))
        {
            std::istringstream is_line(line);
            std::string key;
            if (std::getline(is_line, key, '='))
            {
                std::string value;
                if (key[0] == '#')
                    continue;
                if (std::getline(is_line, value))
                {
                    if(key == "File")
                    {
                        if(value == "StopLossManager.cfg")
                        {
                            std::cout<<"Inside_StopLossManager:"<<value<<std::endl;
                            while (std::getline(configFileStream, line))
                            {
                                std::istringstream is_slManager(line);
                                if (std::getline(is_slManager, key, '='))
                                {
                                    if (key[0] == '#')
                                        continue;
                                    if (std::getline(is_slManager, value))
                                    {
                                        if(key == "File")
                                        {
                                            std::cout<<"|StopLossManager ENDS"<<std::endl<<std::endl;
                                            return;
                                        }
                                        else if(key == "STOPLOSS_ACTIVATION_THRESHOLD")
                                        {
                                            std::cout<<"StopLossManager:STOPLOSS_ACTIVATION_THRESHOLD:"<<value<<std::endl;
                                            _stopLossParams.slAcThr = std::stod(value);
                                        }  
                                        else if(key == "TRAIL_STOP_LOSS")
                                        {
                                            std::cout<<"StopLossManager:TRAIL_STOP_LOSS:"<<value<<std::endl;
                                            _stopLossParams.trailStopLoss = std::stod(value);
                                        }
                                        else if(key == "MAX_PORTFOLIO_LOSS")
                                        {
                                            std::cout<<"StopLossManager:MAX_PORTFOLIO_LOSS:"<<value<<std::endl;
                                            _stopLossParams.maxPortfolioLoss = std::stod(value);
                                        }
                                        else if(key == "MAX_TRADE_LOSS")
                                        {
                                            std::cout<<"StopLossManager:MAX_TRADE_LOSS:"<<value<<std::endl;
                                            _stopLossParams.maxTradeLoss = std::stod(value);
                                        }
                                        else if(key == "USE_TRADE_JUMPER")
                                        {
                                            std::cout<<"StopLossManager:USE_TRADE_JUMPER:"<<value<<std::endl;
                                            if(value == "true"){
                                                _stopLossParams.useTradeJumper = true;
                                            }
                                        }
                                        else if(key == "USE_PASSIVE_VIA_AVG_BOOK")
                                        {
                                            std::cout<<"StopLossManager:USE_PASSIVE_VIA_AVG_BOOK:"<<value<<std::endl;
                                            if(value == "true"){
                                                _stopLossParams.useAvgBook = true;
                                            }
                                        }
                                        else if(key == "USE_AVG_BOOK_THRESHOLD_IN_ABS")
                                        {
                                            std::cout<<"StopLossManager:USE_AVG_BOOK_THRESHOLD_IN_ABS:"<<value<<std::endl;
                                            if(value == "true"){
                                                _stopLossParams.useAvgBookThresholdABS = true;
                                            }
                                        }
                                        else if(key == "CROSS_BOOK_BPS")
                                        {
                                                std::cout<<"StopLossManager:CROSS_BOOK_BPS:"<<value<<std::endl;
                                                _stopLossParams.bookCrossThreshold = std::stod(value);
                                            
                                        } 
                                        else if(key == "AVG_ORDER_TO_BOOK_THRESHOLD_BPS")
                                        {
                                                std::cout<<"StopLossManager:AVG_ORDER_TO_BOOK_THRESHOLD_BPS:"<<value<<std::endl;
                                                _stopLossParams.avgOrderToBookThreshold_BPS = std::stod(value);
                                            
                                        }   
                                        else if(key == "AVG_ORDER_TO_BOOK_THRESHOLD_ABS")
                                        {
                                                std::cout<<"StopLossManager:AVG_ORDER_TO_BOOK_THRESHOLD_ABS:"<<value<<std::endl;
                                                _stopLossParams.avgOrderToBookThreshold_ABS = std::stod(value);
                                            
                                        }  
                                        else if(key == "BOOK_LEVEL")
                                        {
                                            std::cout<<"StopLossManager:BOOK_LEVEL:"<<value<<std::endl;
                                            _stopLossParams.bookLevel = std::stoi(value);
                                        }
                                        else if(key == "ORDER_DIVISION_FACTOR")
                                        {
                                            std::cout<<"StopLossManager:ORDER_DIVISION_FACTOR:"<<value<<std::endl;
                                            _stopLossParams.orderDivisionFactor = std::stoi(value);
                                        }
                                    }
                                }
                            }
                        }   
                    }               
                }
            }
        }
    }
}


void exeConfig::parsePositionManagerFile()
{
     std::ifstream configFileStream(_pathName);
    if (configFileStream.is_open())
    {
        std::string line;
        while (std::getline(configFileStream, line))
        {
            std::istringstream is_line(line);
            std::string key;
            if (std::getline(is_line, key, '='))
            {
                std::string value;
                if (key[0] == '#')
                    continue;
                if (std::getline(is_line, value))
                {
                    if(key == "File")
                    {
                        if(value == "PositionManager.cfg")
                        {
                            std::cout<<"Inside_PositionManager:"<<value<<std::endl;
                            while (std::getline(configFileStream, line))
                            {
                                std::istringstream is_positionManager(line);
                                if (std::getline(is_positionManager, key, '='))
                                {
                                    if (key[0] == '#')
                                        continue;
                                    if (std::getline(is_positionManager, value))
                                    {
                                        if(key == "File")
                                        {
                                            std::cout<<"|PositionManager ENDS"<<std::endl<<std::endl;
                                            return;
                                        }
                                        else if(key == "USEPOSITIONSHIFTMANAGER")
                                        {
                                            std::cout<<"PositionManager:USEPOSITIONSHIFTMANAGER:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                _positionManagerParams.usePositionManager = false;   
                                            }
                                        }  
                                        else if(key == "POSITION_SHIFT_QTY")
                                        {
                                            std::cout<<"PositionManager:POSITION_SHIFT_QTY:"<<value<<std::endl;
                                            _positionManagerParams.positionShiftQty = std::stoi(value);
                                        }
                                        else if(key == "MAX_POSITION")
                                        {
                                            std::cout<<"PositionManager:MAX_POSITION:"<<value<<std::endl;
                                            _positionManagerParams.maxPosition = std::stoi(value);
                                        }
                                        else if(key == "BID_DECREASE_POSITION_SKEW_FACTOR_BPS")
                                        {
                                            std::cout<<"PositionManager:BID_DECREASE_POSITION_SKEW_FACTOR_BPS:"<<value<<std::endl;
                                            _positionManagerParams.bidDecreaseSkewFactor_BPS = std::stod(value);
                                        }
                                        else if(key == "BID_INCREASE_POSITION_SKEW_FACTOR_BPS")
                                        {
                                            std::cout<<"PositionManager:BID_INCREASE_POSITION_SKEW_FACTOR_BPS:"<<value<<std::endl;
                                            _positionManagerParams.bidIncreaseSkewFactor_BPS = std::stod(value);
                                        }
                                        else if(key == "ASK_DECREASE_POSITION_SKEW_FACTOR_BPS")
                                        {
                                            std::cout<<"PositionManager:ASK_DECREASE_POSITION_SKEW_FACTOR_BPS:"<<value<<std::endl;
                                            _positionManagerParams.askDecreaseSkewFactor_BPS = std::stod(value);
                                        }
                                        else if(key == "ASK_INCREASE_POSITION_SKEW_FACTOR_BPS")
                                        {
                                            std::cout<<"PositionManager:ASK_INCREASE_POSITION_SKEW_FACTOR_BPS:"<<value<<std::endl;
                                            _positionManagerParams.askIncreaseSkewFactor_BPS = std::stod(value);
                                        }
                                        else if(key == "BID_DECREASE_POSITION_SKEW_FACTOR_ABS")
                                        {
                                            std::cout<<"PositionManager:BID_DECREASE_POSITION_SKEW_FACTOR_ABS:"<<value<<std::endl;
                                            _positionManagerParams.bidDecreaseSkewFactor_ABS = std::stod(value);
                                        }
                                        else if(key == "BID_INCREASE_POSITION_SKEW_FACTOR_ABS" && mainSwitch == true)
                                        {
                                            std::cout<<"PositionManager:BID_INCREASE_POSITION_SKEW_FACTOR_ABS:"<<value<<std::endl;
                                            _positionManagerParams.bidIncreaseSkewFactor_ABS = std::stod(value);
                                        }
                                        else if(key == "ASK_DECREASE_POSITION_SKEW_FACTOR_ABS")
                                        {
                                            std::cout<<"PositionManager:ASK_DECREASE_POSITION_SKEW_FACTOR_ABS:"<<value<<std::endl;
                                            _positionManagerParams.askDecreaseSkewFactor_ABS = std::stod(value);
                                        }
                                        else if(key == "ASK_INCREASE_POSITION_SKEW_FACTOR_ABS")
                                        {
                                            std::cout<<"PositionManager:ASK_INCREASE_POSITION_SKEW_FACTOR_ABS:"<<value<<std::endl;
                                            _positionManagerParams.askIncreaseSkewFactor_ABS = std::stod(value);
                                        }
                                        else if(key == "BID_DECREASE_MAX_SHIFTS")
                                        {
                                            std::cout<<"PositionManager:BID_DECREASE_MAX_SHIFTS:"<<value<<std::endl;
                                            _positionManagerParams.bidDecreaseMaxShift = std::stoi(value);
                                        }
                                        else if(key == "BID_INCREASE_MAX_SHIFTS")
                                        {
                                            std::cout<<"PositionManager:BID_INCREASE_MAX_SHIFTS:"<<value<<std::endl;
                                            _positionManagerParams.bidIncreaseMaxShift = std::stoi(value);
                                        }
                                        else if(key == "ASK_DECREASE_MAX_SHIFTS")
                                        {
                                            std::cout<<"PositionManager:ASK_DECREASE_MAX_SHIFTS:"<<value<<std::endl;
                                            _positionManagerParams.askDecreaseMaxShift = std::stoi(value);
                                        }
                                        else if(key == "ASK_INCREASE_MAX_SHIFTS")
                                        {
                                            std::cout<<"PositionManager:ASK_INCREASE_MAX_SHIFTS:"<<value<<std::endl;
                                            _positionManagerParams.askIncreaseMaxShift = std::stoi(value);
                                        }
                                    }
                                }
                            }
                        }   
                    }               
                }
            }
        }
    }
}



void exeConfig::parse()
{
    std::ifstream configFileStream(_pathName);
    if (configFileStream.is_open())
    {
        std::string line;
        while (std::getline(configFileStream, line))
        {
            std::istringstream is_line(line);
            std::string key;
            if (std::getline(is_line, key, '='))
            {
                std::string value;
                if (key[0] == '#')
                    continue;
                if (std::getline(is_line, value))
                {
                    size_t found = key.find("Quoter");
                    if(found != std::string::npos){
                        quoterVector.push_back(value);
                        quoterMap[key] = value;
                        // std::cout<<key<<" "<<value<<std::endl;
                    }
                }
            }
        }
    }
    else{
        std::cout<<"pathName_:"<<_pathName<<"|File Not Found"<<std::endl;
    }
}

void exeConfig::parseSniper()
{
    std::ifstream configFileStream(_pathName);
    if (configFileStream.is_open())
    {
        std::string line;
        while (std::getline(configFileStream, line))
        {
            std::istringstream is_line(line);
            std::string key;
            if (std::getline(is_line, key, '='))
            {
                std::string value;
                if (key[0] == '#')
                    continue;
                if (std::getline(is_line, value))
                {
                    size_t found = key.find("Sniper");
                    if(found != std::string::npos){
                        sniperVector.push_back(value);
                        sniperMap[key] = value;
                        // std::cout<<key<<" "<<value<<std::endl;
                    }
                }
            }
        }
    }
    else{
        std::cout<<"pathName_:"<<_pathName<<"|File Not Found"<<std::endl;
    }
}

void exeConfig::parseQuoterFile(std::string quoterFile){

    std::ifstream configFileStream(_pathName);
    if (configFileStream.is_open())
    {
        std::string line;
        while (std::getline(configFileStream, line))
        {
            std::istringstream is_line(line);
            std::string key;
            if (std::getline(is_line, key, '='))
            {
                std::string value;
                if (key[0] == '#')
                    continue;
                if (std::getline(is_line, value))
                {
                    if(key == "File")
                    {
                        if(value == quoterMap[quoterFile])
                        {
                            std::cout<<"Inside_Quoter:"<<value<<std::endl;
                            quoterParams qp;
                            qp.useAbsoluteOffset = generalUseAbsoluteOffset;
                            while (std::getline(configFileStream, line))
                            {
                                std::istringstream is_quoter(line);
                                if (std::getline(is_quoter, key, '='))
                                {
                                    if (key[0] == '#')
                                        continue;
                                    if (std::getline(is_quoter, value))
                                    {
                                        if(key == "File")
                                        {
                                            std::cout<<quoterFile<<"|END"<<std::endl<<std::endl;
                                            quoterParamsMap[quoterFile] = qp;
                                            return;
                                        } 
                                        else if(key == "BID_OFFSET_BPS")
                                        {
                                            std::cout<<quoterFile<<"|BID_OFFSET_BPS:"<<value<<std::endl;
                                            qp.bidSpread_BPS = std::stod(value);
                                        }
                                        else if(key == "ASK_OFFSET_BPS")
                                        {
                                            std::cout<<quoterFile<<"|ASK_OFFSET_BPS:"<<value<<std::endl;
                                            qp.askSpread_BPS = std::stod(value);
                                        }
                                        else if(key == "BID_OFFSET_ABS")
                                        {
                                            std::cout<<quoterFile<<"|BID_OFFSET_ABS:"<<value<<std::endl;
                                            qp.bidSpread_ABS = std::stod(value);
                                        }
                                        else if(key == "ASK_OFFSET_ABS")
                                        {
                                            std::cout<<quoterFile<<"|ASK_OFFSET_ABS:"<<value<<std::endl;
                                            qp.askSpread_ABS = std::stod(value);
                                        }
                                        else if(key == "USE_MODIFY_THRESHOLD_ABS")
                                        {
                                            std::cout<<quoterFile<<"|USE_MODIFY_THRESHOLD_ABS:"<<value<<std::endl;
                                            if(value == "true"){
                                                qp.useAbsoluteModifyThreshold = true;
                                            }
                                        }
                                        else if(key == "QUOTE_MODIFY_PASSIVE_THRESHOLD_BID_BPS")
                                        {
                                            std::cout<<quoterFile<<"|QUOTE_MODIFY_PASSIVE_THRESHOLD_BID_BPS:"<<value<<std::endl;
                                            qp.quoteModifyPassiveThreshold_Bid_BPS = std::stod(value);
                                        }
                                        else if(key == "QUOTE_MODIFY_PASSIVE_THRESHOLD_ASK_BPS")
                                        {
                                            std::cout<<quoterFile<<"|QUOTE_MODIFY_PASSIVE_THRESHOLD_ASK_BPS:"<<value<<std::endl;
                                            qp.quoteModifyPassiveThreshold_Ask_BPS = std::stod(value);
                                        }
                                        else if(key == "QUOTE_MODIFY_AGGRESIVE_THRESHOLD_BID_BPS")
                                        {
                                            std::cout<<quoterFile<<"|QUOTE_MODIFY_AGGRESIVE_THRESHOLD_BID_BPS:"<<value<<std::endl;
                                            qp.quoteModifyAggresiveThreshold_Bid_BPS = std::stod(value);
                                        }
                                        else if(key == "QUOTE_MODIFY_AGGRESIVE_THRESHOLD_ASK_BPS")
                                        {
                                            std::cout<<quoterFile<<"|QUOTE_MODIFY_AGGRESIVE_THRESHOLD_ASK_BPS:"<<value<<std::endl;
                                            qp.quoteModifyAggresiveThreshold_Ask_BPS = std::stod(value);
                                        }
                                        else if(key == "QUOTE_MODIFY_PASSIVE_THRESHOLD_BID_ABS")
                                        {
                                            std::cout<<quoterFile<<"|QUOTE_MODIFY_PASSIVE_THRESHOLD_BID_ABS:"<<value<<std::endl;
                                            qp.quoteModifyPassiveThreshold_Bid_ABS = std::stod(value);
                                        }
                                        else if(key == "QUOTE_MODIFY_PASSIVE_THRESHOLD_ASK_ABS")
                                        {
                                            std::cout<<quoterFile<<"|QUOTE_MODIFY_PASSIVE_THRESHOLD_ASK_ABS:"<<value<<std::endl;
                                            qp.quoteModifyPassiveThreshold_Ask_ABS = std::stod(value);
                                        }
                                        else if(key == "QUOTE_MODIFY_AGGRESIVE_THRESHOLD_BID_ABS")
                                        {
                                            std::cout<<quoterFile<<"|QUOTE_MODIFY_AGGRESIVE_THRESHOLD_BID_ABS:"<<value<<std::endl;
                                            qp.quoteModifyAggresiveThreshold_Bid_ABS = std::stod(value);
                                        }
                                        else if(key == "QUOTE_MODIFY_AGGRESIVE_THRESHOLD_ASK_ABS")
                                        {
                                            std::cout<<quoterFile<<"|QUOTE_MODIFY_AGGRESIVE_THRESHOLD_ASK_ABS:"<<value<<std::endl;
                                            qp.quoteModifyAggresiveThreshold_Ask_ABS = std::stod(value);
                                        }
                                        else if(key == "SWITCH")
                                        {
                                            std::cout<<quoterFile<<"|SWITCH:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                qp.quoterSwitch = false;
                                            }
                                        }
                                        else if(key == "BUY_SWITCH" && qp.quoterSwitch == false)
                                        {
                                            std::cout<<quoterFile<<"|BUY_SWITCH:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                qp.buy_switch = false;
                                            }
                                        }
                                        else if(key == "SELL_SWITCH" && qp.quoterSwitch == false)
                                        {
                                            std::cout<<quoterFile<<"|SELL_SWITCH:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                qp.sell_switch = false;
                                            }
                                        }
                                        else if(key == "BUY_QUOTE_QTY")
                                        {
                                            std::cout<<quoterFile<<"|BUY_QUOTE_QTY:"<<value<<std::endl;
                                            qp.buyQuoteQty = std::stoi(value);
                                        }
                                        else if(key == "SELL_QUOTE_QTY")
                                        {
                                            std::cout<<quoterFile<<"|SELL_QUOTE_QTY:"<<value<<std::endl;
                                            qp.sellQuoteQty = std::stoi(value);
                                        }
                                        else if(key == "QUOTE_SKEW_FACTOR_BUY")
                                        {
                                            std::cout<<quoterFile<<"|QUOTE_SKEW_FACTOR_BUY:"<<value<<std::endl;
                                            qp.quoteQtySkewFactorBuy = std::stod(value);
                                        }
                                        else if(key == "QUOTE_SKEW_FACTOR_SELL")
                                        {
                                            std::cout<<quoterFile<<"|QUOTE_SKEW_FACTOR_SELL:"<<value<<std::endl;
                                            qp.quoteQtySkewFactorSell = std::stod(value);
                                        }
                                        //Trend
                                        else if(key == "USE_LONGTERM_TREND")
                                        {
                                            std::cout<<quoterFile<<"|USE_LONGTERM_TREND:"<<value<<std::endl;
                                            if(value == "true" && _supportParams.useTrendSkew == true){
                                                qp.useLongTrendSkew = true;
                                                if(generalUseAbsoluteOffset == true){
                                                    qp.useTrendOffsetABS = true;
                                                }
                                            }
                                        }
                                        else if(key == "USE_SHORTTERM_TREND")
                                        {
                                            std::cout<<quoterFile<<"|USE_SHORTTERM_TREND:"<<value<<std::endl;
                                            if(value == "true" && _supportParams.useTrendSkew == true){
                                                qp.useShortTrendSkew = true;
                                                if(generalUseAbsoluteOffset == true){
                                                    qp.useTrendOffsetABS = true;
                                                }
                                            }
                                        }
                                        else if(key == "TREND_SKEW_REVERSION")
                                        {
                                            std::cout<<quoterFile<<"|TREND_SKEW_REVERSION:"<<value<<std::endl;
                                            if(value == "true"){
                                                qp.trendSkewInversion = true;
                                            }
                                        }
                                        else if(key == "LONG_TREND_INCREASE_SKEW_OFFSET_BPS")
                                        {
                                            std::cout<<quoterFile<<"|LONG_TREND_INCREASE_SKEW_OFFSET_BPS:"<<value<<std::endl;
                                            qp.longTrendIncreaseSkewOffset_BPS = std::stod(value);
                                        }
                                        else if(key == "LONG_TREND_DECREASE_SKEW_OFFSET_BPS")
                                        {
                                            std::cout<<quoterFile<<"|LONG_TREND_DECREASE_SKEW_OFFSET_BPS:"<<value<<std::endl;
                                            qp.longTrendDecreaseSkewOffset_BPS = std::stod(value);
                                        }
                                        else if(key == "SHORT_TREND_INCREASE_SKEW_OFFSET_BPS")
                                        {
                                            std::cout<<quoterFile<<"|SHORT_TREND_INCREASE_SKEW_OFFSET_BPS:"<<value<<std::endl;
                                            qp.shortTrendIncreaseSkewOffset_BPS = std::stod(value);
                                        }
                                        else if(key == "SHORT_TREND_DECREASE_SKEW_OFFSET_BPS")
                                        {
                                            std::cout<<quoterFile<<"|SHORT_TREND_DECREASE_SKEW_OFFSET_BPS:"<<value<<std::endl;
                                            qp.shortTrendDecreaseSkewOffset_BPS = std::stod(value);
                                        }
                                        else if(key == "LONG_TREND_INCREASE_SKEW_OFFSET_ABS")
                                        {
                                            std::cout<<quoterFile<<"|LONG_TREND_INCREASE_SKEW_OFFSET_ABS:"<<value<<std::endl;
                                            qp.longTrendIncreaseSkewOffset_ABS = std::stod(value);
                                        }
                                        else if(key == "LONG_TREND_DECREASE_SKEW_OFFSET_ABS")
                                        {
                                            std::cout<<quoterFile<<"|LONG_TREND_DECREASE_SKEW_OFFSET_ABS:"<<value<<std::endl;
                                            qp.longTrendDecreaseSkewOffset_ABS = std::stod(value);
                                        }
                                        else if(key == "SHORT_TREND_INCREASE_SKEW_OFFSET_ABS")
                                        {
                                            std::cout<<quoterFile<<"|SHORT_TREND_INCREASE_SKEW_OFFSET_ABS:"<<value<<std::endl;
                                            qp.shortTrendIncreaseSkewOffset_ABS = std::stod(value);
                                        }
                                        else if(key == "SHORT_TREND_DECREASE_SKEW_OFFSET_ABS")
                                        {
                                            std::cout<<quoterFile<<"|SHORT_TREND_DECREASE_SKEW_OFFSET_ABS:"<<value<<std::endl;
                                            qp.shortTrendDecreaseSkewOffset_ABS = std::stod(value);
                                        }
                                        else if(key == "USE_LTP_FILTER")
                                        {
                                            std::cout<<quoterFile<<"|USE_LTP_FILTER:"<<value<<std::endl;
                                            if(value == "true" && useLtpFilter == true)
                                            {
                                                qp.useLtpFilter = true;
                                            }
                                        } 
                                        else if(key == "SHORT_TREND_QUALIFY_PERCENT_BPS")
                                        {
                                            std::cout<<"Support:SHORT_TREND_QUALIFY_PERCENT_BPS:"<<value<<std::endl;
                                            qp.shortTrendQualifyPercent = std::stod(value);
                                        }
                                        else if(key == "LONG_TREND_QUALIFY_PERCENT_BPS")
                                        {
                                            std::cout<<"Support:LONG_TREND_QUALIFY_PERCENT_BPS:"<<value<<std::endl;
                                            qp.longTrendQualifyPercent = std::stod(value);
                                        }  
                                        else if(key == "MAX_LONG_TREND_SHIFTS")
                                        {
                                            std::cout<<"Support:MAX_LONG_TREND_SHIFTS:"<<value<<std::endl;
                                            qp.maxLongTrendShifts = std::stod(value);
                                        }  
                                        else if(key == "MAX_SHORT_TREND_SHIFTS")
                                        {
                                            std::cout<<"Support:MAX_SHORT_TREND_SHIFTS:"<<value<<std::endl;
                                            qp.maxShortTrendShifts = std::stod(value);
                                        }
                                        else if(key == "MIN_QUOTE_PASSIVE_GAP_ABS")
                                        {
                                            std::cout<<quoterFile<<"|MIN_QUOTE_PASSIVE_GAP_ABS:"<<value<<std::endl;
                                            qp.minQuotePassiveGap = std::stoi(value);
                                        }
                                        else if(key == "MAX_QUOTE_PASSIVE_GAP_ABS")
                                        {
                                            std::cout<<quoterFile<<"|MAX_QUOTE_PASSIVE_GAP_ABS:"<<value<<std::endl;
                                            qp.maxQuotePassiveGap = std::stoi(value);
                                        }                                           
                                        quoterParamsMap[quoterFile] = qp;
                                    }
                                }
                            }
                        }   
                    }               
                }
            }
        }
    }
}


void exeConfig::parseJumperFile()
{
     std::ifstream configFileStream(_pathName);
    if (configFileStream.is_open())
    {
        std::string line;
        while (std::getline(configFileStream, line))
        {
            std::istringstream is_line(line);
            std::string key;
            if (std::getline(is_line, key, '='))
            {
                std::string value;
                if (key[0] == '#')
                    continue;
                if (std::getline(is_line, value))
                {
                    if(key == "File")
                    {
                        if(value == "Jumper.cfg")
                        {
                            std::cout<<"Jumper:"<<value<<std::endl;
                            while (std::getline(configFileStream, line))
                            {
                                std::istringstream is_jumper(line);
                                if (std::getline(is_jumper, key, '='))
                                {
                                    if (key[0] == '#')
                                        continue;
                                    if (std::getline(is_jumper, value))
                                    {
                                        if(key == "File")
                                        {
                                            std::cout<<"|Jumper ENDS"<<std::endl<<std::endl;
                                            return;
                                        }
                                        else if(key == "USE_JUMPER")
                                        {
                                            std::cout<<"Jumper:USE_JUMPER:"<<value<<std::endl;
                                            if(value == "true"){
                                                _jumperParams.useJumper = true;
                                            }
                                        } 
                                        else if(key == "USE_CUMMULATIVE_QTY")
                                        {
                                            std::cout<<"Jumper:USE_CUMMULATIVE_QTY:"<<value<<std::endl;
                                            if(value == "true"){
                                                _jumperParams.useCummQty = true;
                                            }
                                        }  
                                        else if(key == "BUY_JOIN_QTY")
                                        {
                                            std::cout<<"Jumper:BUY_JOIN_QTY:"<<value<<std::endl;
                                            _jumperParams.buyJoinQty = std::stoi(value);
                                        }
                                        else if(key == "SELL_JOIN_QTY")
                                        {
                                            std::cout<<"Jumper:SELL_JOIN_QTY:"<<value<<std::endl;
                                            _jumperParams.sellJoinQty = std::stoi(value);
                                        }
                                        else if(key == "BUY_JUMP_QTY")
                                        {
                                            std::cout<<"Jumper:BUY_JUMP_QTY:"<<value<<std::endl;
                                            _jumperParams.buyJumpQty = std::stoi(value);
                                        }
                                        else if(key == "SELL_JUMP_QTY")
                                        {
                                            std::cout<<"Jumper:SELL_JUMP_QTY:"<<value<<std::endl;
                                            _jumperParams.sellJumpQty = std::stoi(value);
                                        }
                                        else if(key == "BID_CUMMULATIVE_QTY")
                                        {
                                            std::cout<<"Jumper:BID_CUMMULATIVE_QTY:"<<value<<std::endl;
                                            _jumperParams.bidCummQty = std::stoi(value);
                                        }
                                        else if(key == "ASK_CUMMULATIVE_QTY")
                                        {
                                            std::cout<<"Jumper:ASK_CUMMULATIVE_QTY:"<<value<<std::endl;
                                            _jumperParams.askCummQty = std::stoi(value);
                                        }
                                        else if(key == "BOOK_LEVEL")
                                        {
                                            std::cout<<"Jumper:BOOK_LEVEL:"<<value<<std::endl;
                                            _jumperParams.bookLength = std::stoi(value);
                                        }
                                        else if(key == "TOP_TO_BOTTOM")
                                        {
                                            std::cout<<"Jumper:TOP_TO_BOTTOM:"<<value<<std::endl;
                                            _jumperParams.topToBottomPriority = std::stoi(value);
                                        }
                                        else if(key == "BOTTOM_TO_TOP")
                                        {
                                            std::cout<<"Jumper:BOTTOM_TO_TOP:"<<value<<std::endl;
                                            _jumperParams.bottomToTopPriority = std::stoi(value);
                                        }
                                        else if(key == "BID_OFFSET_BPS")
                                        {
                                            std::cout<<"Jumper:BID_OFFSET_BPS:"<<value<<std::endl;
                                            _jumperParams.bidOffsetBPS = std::stod(value);
                                        }
                                        else if(key == "ASK_OFFSET_BPS")
                                        {
                                            std::cout<<"Jumper:ASK_OFFSET_BPS:"<<value<<std::endl;
                                            _jumperParams.askOffsetBPS = std::stod(value);
                                        }
                                        else if(key == "BID_OFFSET_ABS")
                                        {
                                            std::cout<<"Jumper:BID_OFFSET_ABS:"<<value<<std::endl;
                                            _jumperParams.bidOffsetABS = std::stoi(value);
                                        }
                                        else if(key == "ASK_OFFSET_ABS")
                                        {
                                            std::cout<<"Jumper:ASK_OFFSET_ABS:"<<value<<std::endl;
                                            _jumperParams.askOffsetABS = std::stoi(value);
                                        }
                                        else if(key == "BUY_QUOTE_QTY")
                                        {
                                            std::cout<<"Jumper:BUY_QUOTE_QTY:"<<value<<std::endl;
                                            _jumperParams.buyQuoteQty = std::stoi(value);
                                        }
                                        else if(key == "SELL_QUOTE_QTY")
                                        {
                                            std::cout<<"Jumper:SELL_QUOTE_QTY:"<<value<<std::endl;
                                            _jumperParams.sellQuoteQty = std::stoi(value);
                                        }
                                        else if(key == "BUY_MAX_POSITION")
                                        {
                                            std::cout<<"Jumper:BUY_MAX_POSITION:"<<value<<std::endl;
                                            _jumperParams.buyMaxPosition = std::stoi(value);
                                        }
                                        else if(key == "SELL_MAX_POSITION")
                                        {
                                            std::cout<<"Jumper:SELL_MAX_POSITION:"<<value<<std::endl;
                                            _jumperParams.sellMaxPosition = std::stoi(value);
                                        }
                                        else if(key == "MODIFY_DELAY_MICRO")
                                        {
                                            std::cout<<"Jumper:MODIFY_DELAY_MICRO:"<<value<<std::endl;
                                            _jumperParams.modifyDelay = std::stoi(value) * 1000;
                                        }
                                        else if(key == "SWITCH")
                                        {
                                            std::cout<<"Jumper:SWITCH:"<<value<<std::endl;
                                            if(value == "true"){
                                                _jumperParams.jumperSwitch = true;
                                            }
                                        }
                                        else if(key == "BUY_SWITCH")
                                        {
                                            std::cout<<"Jumper:BUY_SWITCH:"<<value<<std::endl;
                                            if(value == "true" && _jumperParams.jumperSwitch == true){
                                                _jumperParams.jumperBuySwitch = true;
                                            }
                                        }
                                        else if(key == "SELL_SWITCH")
                                        {
                                            std::cout<<"Jumper:SELL_SWITCH:"<<value<<std::endl;
                                            if(value == "true" && _jumperParams.jumperSwitch == true){
                                                _jumperParams.jumperSellSwitch = true;
                                            }
                                        }
                                        else if(key == "USE_PASSIVE_VIA_AVG_BOOK")
                                        {
                                            std::cout<<"Jumper:USE_PASSIVE_VIA_AVG_BOOK:"<<value<<std::endl;
                                            if(value == "true" ){
                                                _jumperParams.useAvgBook = true;
                                            }
                                        }
                                        else if(key == "AVG_ORDER_TO_BOOK_THRESHOLD_BPS")
                                        {
                                            std::cout<<"Jumper:AVG_ORDER_TO_BOOK_THRESHOLD_BPS:"<<value<<std::endl;
                                            _jumperParams.avgOrderToBookThreshold_BPS = std::stod(value);
                                        }
                                        else if(key == "AVG_ORDER_TO_BOOK_THRESHOLD_ABS")
                                        {
                                            std::cout<<"Jumper:AVG_ORDER_TO_BOOK_THRESHOLD_ABS:"<<value<<std::endl;
                                            _jumperParams.avgOrderToBookThreshold_ABS = std::stod(value);
                                        }
                                        else if(key == "AVG_BOOK_LEVEL")
                                        {
                                            std::cout<<"Jumper:AVG_BOOK_LEVEL:"<<value<<std::endl;
                                            _jumperParams.avgBookLevels = std::stoi(value);
                                        }
                                        else if(key == "BOOK_WEIGHTAGE_LEVELS")
                                        {
                                            std::cout<<"Jumper:BOOK_WEIGHTAGE_LEVELS:"<<value<<std::endl;
                                            std::istringstream is_weightage(value);
                                            std::string key;
                                            while(std::getline(is_weightage , key , ','))
                                            {
                                                _jumperParams.levelWeightageVector.push_back(std::stod(key));
                                            }
                                        }
                                    }
                                }
                            }
                        }   
                    }               
                }
            }
        }
    }
}

void exeConfig::parseSniperFile(std::string sniperFile){

    std::ifstream configFileStream(_pathName);
    if (configFileStream.is_open())
    {
        std::string line;
        while (std::getline(configFileStream, line))
        {
            std::istringstream is_line(line);
            std::string key;
            if (std::getline(is_line, key, '='))
            {
                std::string value;
                if (key[0] == '#')
                    continue;
                if (std::getline(is_line, value))
                {
                    if(key == "File")
                    {
                        if(value == sniperMap[sniperFile])
                        {
                            std::cout<<"Inside_Sniper:"<<value<<std::endl;
                            sniperParams sp;
                            sp.useAbsoluteOffset = generalUseAbsoluteOffset;
                            while (std::getline(configFileStream, line))
                            {
                                std::istringstream is_sniper(line);
                                if (std::getline(is_sniper, key, '='))
                                {
                                    if (key[0] == '#')
                                        continue;
                                    if (std::getline(is_sniper, value))
                                    {
                                        if(key == "File")
                                        {
                                            std::cout<<sniperFile<<"|END"<<std::endl<<std::endl;
                                            sniperParamsMap[sniperFile] = sp;
                                            return;
                                        }
                                        else if(key == "USE_SNIPER")
                                        {
                                            std::cout<<sniperFile<<"|USE_SNIPER:"<<value<<std::endl;
                                            if(value == "true"){
                                                sp.useSniper = true;
                                            }
                                        } 
                                        else if(key == "BUY_OPPORTUNITY_GAP_BPS")
                                        {
                                            std::cout<<sniperFile<<"|BUY_OPPORTUNITY_GAP_BPS:"<<value<<std::endl;
                                            sp.buyOpportunity_BPS = std::stod(value);
                                        }
                                        else if(key == "SELL_OPPORTUNITY_GAP_BPS")
                                        {
                                            std::cout<<sniperFile<<"|SELL_OPPORTUNITY_GAP_BPS:"<<value<<std::endl;
                                            sp.sellOpportunity_BPS = std::stod(value);
                                        }
                                        else if(key == "BUY_OPPORTUNITY_GAP_ABS")
                                        {
                                            std::cout<<sniperFile<<"|BUY_OPPORTUNITY_GAP_ABS:"<<value<<std::endl;
                                            sp.buyOpportunity_ABS = std::stod(value);
                                        }
                                        else if(key == "SELL_OPPORTUNITY_GAP_ABS")
                                        {
                                            std::cout<<sniperFile<<"|SELL_OPPORTUNITY_GAP_ABS:"<<value<<std::endl;
                                            sp.sellOpportunity_ABS = std::stod(value);
                                        }
                                        else if(key == "BID_OFFSET_BPS")
                                        {
                                            std::cout<<sniperFile<<"|BID_OFFSET_BPS:"<<value<<std::endl;
                                            sp.bidSpread_BPS = std::stod(value);
                                        }
                                        else if(key == "ASK_OFFSET_BPS")
                                        {
                                            std::cout<<sniperFile<<"|ASK_OFFSET_BPS:"<<value<<std::endl;
                                            sp.askSpread_BPS = std::stod(value);
                                        }
                                        else if(key == "BID_OFFSET_ABS")
                                        {
                                            std::cout<<sniperFile<<"|BID_OFFSET_ABS:"<<value<<std::endl;
                                            sp.bidSpread_ABS = std::stod(value);
                                        }
                                        else if(key == "ASK_OFFSET_ABS")
                                        {
                                            std::cout<<sniperFile<<"|ASK_OFFSET_ABS:"<<value<<std::endl;
                                            sp.askSpread_ABS = std::stod(value);
                                        }
                                        else if(key == "RECALCULATE_BUY_ORDER_PRICE_BPS")
                                        {
                                            std::cout<<sniperFile<<"|RECALCULATE_BUY_ORDER_PRICE_BPS:"<<value<<std::endl;
                                            sp.quoteModifyThreshold_Bid_BPS = std::stod(value);
                                        }
                                        else if(key == "RECALCULATE_SELL_ORDER_PRICE_BPS")
                                        {
                                            std::cout<<sniperFile<<"|RECALCULATE_SELL_ORDER_PRICE_BPS:"<<value<<std::endl;
                                            sp.quoteModifyThreshold_Ask_BPS = std::stod(value);
                                        }
                                        else if(key == "RECALCULATE_BUY_ORDER_PRICE_ABS")
                                        {
                                            std::cout<<sniperFile<<"|RECALCULATE_BUY_ORDER_PRICE_ABS:"<<value<<std::endl;
                                            sp.quoteModifyThreshold_Bid_ABS = std::stod(value);
                                        }
                                        else if(key == "RECALCULATE_SELL_ORDER_PRICE_ABS")
                                        {
                                            std::cout<<sniperFile<<"|RECALCULATE_SELL_ORDER_PRICE_ABS:"<<value<<std::endl;
                                            sp.quoteModifyThreshold_Ask_ABS = std::stod(value);
                                        }
                                        else if(key == "SWITCH")
                                        {
                                            std::cout<<sniperFile<<"|SWITCH:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                sp.sniperSwitch = false;
                                            }
                                        }
                                        else if(key == "BUY_SWITCH" && sp.sniperSwitch == false)
                                        {
                                            std::cout<<sniperFile<<"|BUY_SWITCH:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                sp.buy_switch = false;
                                            }
                                        }
                                        else if(key == "SELL_SWITCH" && sp.sniperSwitch == false)
                                        {
                                            std::cout<<sniperFile<<"|SELL_SWITCH:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                sp.sell_switch = false;
                                            }
                                        }
                                        else if(key == "BUY_QUOTE_QTY")
                                        {
                                            std::cout<<sniperFile<<"|BUY_QUOTE_QTY:"<<value<<std::endl;
                                            sp.buyQuoteQty = std::stoi(value);
                                        }
                                        else if(key == "SELL_QUOTE_QTY")
                                        {
                                            std::cout<<sniperFile<<"|SELL_QUOTE_QTY:"<<value<<std::endl;
                                            sp.sellQuoteQty = std::stoi(value);
                                        }
                                        else if(key == "MIN_BUY_BOOK_QTY")
                                        {
                                            std::cout<<sniperFile<<"|MIN_BUY_BOOK_QTY:"<<value<<std::endl;
                                            sp.minBookBuyQtyToFire = std::stoi(value);
                                        }
                                        else if(key == "MIN_SELL_BOOK_QTY")
                                        {
                                            std::cout<<sniperFile<<"|MIN_SELL_BOOK_QTY:"<<value<<std::endl;
                                            sp.minBookSellQtyToFire = std::stoi(value);
                                        }
                                        else if(key == "BUY_SHOOT_COUNT")
                                        {
                                            std::cout<<sniperFile<<"|BUY_SHOOT_COUNT:"<<value<<std::endl;
                                            sp.buyShootCount = std::stoi(value);
                                        }
                                        else if(key == "SELL_SHOOT_COUNT")
                                        {
                                            std::cout<<sniperFile<<"|SELL_SHOOT_COUNT:"<<value<<std::endl;
                                            sp.sellShootCount = std::stoi(value);
                                        }
                                        else if(key == "TOTAL_SHOOT_COUNT")
                                        {
                                            std::cout<<sniperFile<<"|TOTAL_SHOOT_COUNT:"<<value<<std::endl;
                                            sp.totalShootCount = std::stoi(value);
                                        }
                                        else if(key == "BUY_MAX_POSITION")
                                        {
                                            std::cout<<sniperFile<<"|SELL_SHOOT_COUNT:"<<value<<std::endl;
                                            sp.buyMaxPosition = std::stoi(value);
                                        }
                                        else if(key == "SELL_MAX_POSITION")
                                        {
                                            std::cout<<sniperFile<<"|TOTAL_SHOOT_COUNT:"<<value<<std::endl;
                                            sp.sellMaxPosition = std::stoi(value);
                                        }
                                        else if(key == "BOOK_LEVEL")
                                        {
                                            std::cout<<sniperFile<<"|BOOK_LEVEL:"<<value<<std::endl;
                                            sp.booklevel = std::stoi(value);
                                        }
                                        sniperParamsMap[sniperFile] = sp;
                                    }
                                }
                            }
                        }   
                    }               
                }
            }
        }
    }
}

void exeConfig::parseMultiQuoterFile()
{
     std::ifstream configFileStream(_pathName);
    if (configFileStream.is_open())
    {
        std::string line;
        while (std::getline(configFileStream, line))
        {
            std::istringstream is_line(line);
            std::string key;
            if (std::getline(is_line, key, '='))
            {
                std::string value;
                if (key[0] == '#')
                    continue;
                if (std::getline(is_line, value))
                {
                    if(key == "File")
                    {
                        if(value == "MultiQuoter.cfg")
                        {
                            std::cout<<"MultiQuoter:"<<value<<std::endl;
                            _multiQuoterParams.useAbsoluteOffset = generalUseAbsoluteOffset;
                            while (std::getline(configFileStream, line))
                            {
                                std::istringstream is_multiQuoter(line);
                                if (std::getline(is_multiQuoter, key, '='))
                                {
                                    if (key[0] == '#')
                                        continue;
                                    if (std::getline(is_multiQuoter, value))
                                    {
                                        if(key == "File")
                                        {
                                            std::cout<<"|MultiQuoter ENDS"<<std::endl<<std::endl;
                                            return;
                                        }
                                        else if(key == "USE_MULTI_QUOTER")
                                        {
                                            std::cout<<"MultiQuoter:USE_MULTI_QUOTER:"<<value<<std::endl;
                                            if(value == "true"){
                                                _multiQuoterParams.useMultiQuoter = true;
                                            }
                                        }
                                        else if(key == "ONLY_PASSIVE")
                                        {
                                            std::cout<<"MultiQuoter:ONLY_PASSIVE:"<<value<<std::endl;
                                            if(value == "true"){
                                                _multiQuoterParams.onlyPassive = true;
                                            }
                                        }
                                        else if(key == "LEVELS")
                                        {
                                            std::cout<<"MultiQuoter:LEVELS:"<<value<<std::endl;
                                            _multiQuoterParams.levels = std::stoi(value);
                                        }
                                        else if(key == "BID_OFFSET_BPS")
                                        {
                                            std::cout<<"MultiQuoter:BID_OFFSET_BPS:"<<value<<std::endl;
                                            _multiQuoterParams.bidSpread_BPS = std::stod(value);
                                        }
                                        else if(key == "ASK_OFFSET_BPS")
                                        {
                                            std::cout<<"MultiQuoter:ASK_OFFSET_BPS:"<<value<<std::endl;
                                            _multiQuoterParams.askSpread_BPS = std::stod(value);
                                        }
                                        else if(key == "BID_OFFSET_ABS")
                                        {
                                            std::cout<<"MultiQuoter:BID_OFFSET_ABS:"<<value<<std::endl;
                                            _multiQuoterParams.bidSpread_ABS = std::stod(value);
                                        }
                                        else if(key == "ASK_OFFSET_ABS")
                                        {
                                            std::cout<<"MultiQuoter:ASK_OFFSET_ABS:"<<value<<std::endl;
                                            _multiQuoterParams.askSpread_ABS = std::stod(value);
                                        }
                                        else if(key == "BID_OFFSET_BPS_INCREMENT")
                                        {
                                            std::cout<<"MultiQuoter:BID_OFFSET_BPS_INCREMENT:"<<value<<std::endl;
                                            _multiQuoterParams.bidSpreadIncrement_BPS = std::stod(value);
                                        }
                                        else if(key == "ASK_OFFSET_BPS_INCREMENT")
                                        {
                                            std::cout<<"MultiQuoter:ASK_OFFSET_BPS_INCREMENT:"<<value<<std::endl;
                                            _multiQuoterParams.askSpreadIncrement_BPS = std::stod(value);
                                        }
                                        else if(key == "BID_OFFSET_ABS_INCREMENT")
                                        {
                                            std::cout<<"MultiQuoter:BID_OFFSET_ABS_INCREMENT:"<<value<<std::endl;
                                            _multiQuoterParams.bidSpreadIncrement_ABS = std::stod(value);
                                        }
                                        else if(key == "ASK_OFFSET_ABS_INCREMENT")
                                        {
                                            std::cout<<"MultiQuoter:ASK_OFFSET_ABS_INCREMENT:"<<value<<std::endl;
                                            _multiQuoterParams.askSpreadIncrement_ABS = std::stod(value);
                                        }
                                        else if(key == "QUOTE_MODIFY_PASSIVE_THRESHOLD_BID_BPS")
                                        {
                                            std::cout<<"MultiQuoter:QUOTE_MODIFY_PASSIVE_THRESHOLD_BID_BPS:"<<value<<std::endl;
                                            _multiQuoterParams.quoteModifyPassiveThreshold_Bid_BPS = std::stod(value);
                                        }
                                        else if(key == "QUOTE_MODIFY_PASSIVE_THRESHOLD_ASK_BPS")
                                        {
                                            std::cout<<"MultiQuoter:QUOTE_MODIFY_PASSIVE_THRESHOLD_ASK_BPS:"<<value<<std::endl;
                                            _multiQuoterParams.quoteModifyPassiveThreshold_Ask_BPS = std::stod(value);
                                        }
                                        else if(key == "QUOTE_MODIFY_AGGRESIVE_THRESHOLD_BID_BPS")
                                        {
                                            std::cout<<"MultiQuoter:QUOTE_MODIFY_AGGRESIVE_THRESHOLD_BID_BPS:"<<value<<std::endl;
                                            _multiQuoterParams.quoteModifyAggresiveThreshold_Bid_BPS = std::stod(value);
                                        }
                                        else if(key == "QUOTE_MODIFY_AGGRESIVE_THRESHOLD_ASK_BPS")
                                        {
                                            std::cout<<"MultiQuoter:QUOTE_MODIFY_AGGRESIVE_THRESHOLD_ASK_BPS:"<<value<<std::endl;
                                            _multiQuoterParams.quoteModifyAggresiveThreshold_Ask_BPS = std::stod(value);
                                        }
                                        else if(key == "QUOTE_MODIFY_PASSIVE_THRESHOLD_BID_ABS")
                                        {
                                            std::cout<<"MultiQuoter:QUOTE_MODIFY_PASSIVE_THRESHOLD_BID_ABS:"<<value<<std::endl;
                                            _multiQuoterParams.quoteModifyPassiveThreshold_Bid_ABS = std::stod(value);
                                        }
                                        else if(key == "QUOTE_MODIFY_PASSIVE_THRESHOLD_ASK_ABS")
                                        {
                                            std::cout<<"MultiQuoter:QUOTE_MODIFY_PASSIVE_THRESHOLD_ASK_ABS:"<<value<<std::endl;
                                            _multiQuoterParams.quoteModifyPassiveThreshold_Ask_ABS = std::stod(value);
                                        }
                                        else if(key == "QUOTE_MODIFY_AGGRESIVE_THRESHOLD_BID_ABS")
                                        {
                                            std::cout<<"MultiQuoter:QUOTE_MODIFY_AGGRESIVE_THRESHOLD_BID_ABS:"<<value<<std::endl;
                                            _multiQuoterParams.quoteModifyAggresiveThreshold_Bid_ABS = std::stod(value);
                                        }
                                        else if(key == "QUOTE_MODIFY_AGGRESIVE_THRESHOLD_ASK_ABS")
                                        {
                                            std::cout<<"MultiQuoter:QUOTE_MODIFY_AGGRESIVE_THRESHOLD_ASK_ABS:"<<value<<std::endl;
                                            _multiQuoterParams.quoteModifyAggresiveThreshold_Ask_ABS = std::stod(value);
                                        }
                                        else if(key == "SWITCH")
                                        {
                                            std::cout<<"MultiQuoter:SWITCH:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                _multiQuoterParams.multiQuoterSwitch = false;
                                            }
                                        }
                                        else if(key == "BUY_SWITCH" && _multiQuoterParams.multiQuoterSwitch == false)
                                        {
                                            std::cout<<"MultiQuoter:BUY_SWITCH:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                _multiQuoterParams.buy_switch = false;
                                            }
                                        }
                                        else if(key == "SELL_SWITCH" && _multiQuoterParams.multiQuoterSwitch == false)
                                        {
                                            std::cout<<"MultiQuoter:SELL_SWITCH:"<<value<<std::endl;
                                            if(value == "false")
                                            {
                                                _multiQuoterParams.sell_switch = false;
                                            }
                                        }
                                        else if(key == "BUY_QUOTE_QTY")
                                        {
                                            std::cout<<"MultiQuoter:BUY_QUOTE_QTY:"<<value<<std::endl;
                                            _multiQuoterParams.buyQuoteQty = std::stoi(value);
                                        }
                                        else if(key == "SELL_QUOTE_QTY")
                                        {
                                            std::cout<<"MultiQuoter:SELL_QUOTE_QTY:"<<value<<std::endl;
                                            _multiQuoterParams.sellQuoteQty = std::stoi(value);
                                        }
                                        else if(key == "QUOTE_SKEW_FACTOR_BUY")
                                        {
                                            std::cout<<"MultiQuoter:|QUOTE_SKEW_FACTOR_BUY:"<<value<<std::endl;
                                            _multiQuoterParams.quoteQtySkewFactorBuy = std::stod(value);
                                        }
                                        else if(key == "QUOTE_SKEW_FACTOR_SELL")
                                        {
                                            std::cout<<"MultiQuoter:|QUOTE_SKEW_FACTOR_SELL:"<<value<<std::endl;
                                            _multiQuoterParams.quoteQtySkewFactorSell = std::stod(value);
                                        }
                                        else if(key == "BUY_QUOTE_QTY_INCREMENT")
                                        {
                                            std::cout<<"MultiQuoter:BUY_QUOTE_QTY_INCREMENT:"<<value<<std::endl;
                                            _multiQuoterParams.buyQuoteQtyIncrement = std::stoi(value);
                                        }
                                        else if(key == "SELL_QUOTE_QTY_INCREMENT")
                                        {
                                            std::cout<<"MultiQuoter:SELL_QUOTE_QTY_INCREMENT:"<<value<<std::endl;
                                            _multiQuoterParams.sellQuoteQtyIncrement = std::stoi(value);
                                        }
                                        else if(key == "BUY_REFILL_PERCENTAGE")
                                        {
                                            std::cout<<"MultiQuoter:BUY_REFILL_PERCENTAGE:"<<value<<std::endl;
                                            _multiQuoterParams.buyFillPercentage = std::stod(value);
                                        }
                                        else if(key == "SELL_REFILL_PERCENTAGE")
                                        {
                                            std::cout<<"MultiQuoter:SELL_REFILL_PERCENTAGE:"<<value<<std::endl;
                                            _multiQuoterParams.sellFillPercentage = std::stod(value);
                                        }
                                        else if(key == "MAX_BUY_POSITION")
                                        {
                                            std::cout<<"MultiQuoter:MAX_BUY_POSITION:"<<value<<std::endl;
                                            _multiQuoterParams.maxBuyPosition = std::stoi(value);
                                        }
                                        else if(key == "MAX_SELL_POSITION")
                                        {
                                            std::cout<<"MultiQuoter:MAX_SELL_POSITION:"<<value<<std::endl;
                                            _multiQuoterParams.maxSellPosition = std::stoi(value);
                                        }
                                        else if(key == "ORDER_LIMIT")
                                        {
                                            std::cout<<"MultiQuoter:ORDER_LIMIT:"<<value<<std::endl;
                                            _multiQuoterParams.orderLimit = std::stoi(value);
                                        }
                                        else if(key == "USE_LTP_FILTER")
                                        {
                                            std::cout<<"MultiQuoter:USE_LTP_FILTER:"<<value<<std::endl;
                                            if(value == "true" && useLtpFilter == true)
                                            {
                                                _multiQuoterParams.useLtpFilter = true;
                                            }
                                        }
                                        else if(key == "USE_LONGTERM_TREND")
                                        {
                                            std::cout<<"MultiQuoter:USE_LONGTERM_TREND:"<<value<<std::endl;
                                            if(value == "true" && _supportParams.useTrendSkew == true){
                                                _multiQuoterParams.useLongTrendSkew = true;
                                                if(generalUseAbsoluteOffset == true){
                                                    _multiQuoterParams.useTrendOffsetABS = true;
                                                }
                                            }
                                        }
                                        else if(key == "USE_SHORTTERM_TREND")
                                        {
                                            std::cout<<"MultiQuoter:USE_SHORTTERM_TREND:"<<value<<std::endl;
                                            if(value == "true" && _supportParams.useTrendSkew == true){
                                                _multiQuoterParams.useShortTrendSkew = true;
                                                if(generalUseAbsoluteOffset == true){
                                                    _multiQuoterParams.useTrendOffsetABS = true;
                                                }
                                            }
                                        }
                                        else if(key == "TREND_SKEW_REVERSION")
                                        {
                                            std::cout<<"MultiQuoter:TREND_SKEW_REVERSION:"<<value<<std::endl;
                                            if(value == "true"){
                                                _multiQuoterParams.trendSkewInversion = true;
                                            }
                                        }
                                        else if(key == "LONG_TREND_INCREASE_SKEW_OFFSET_BPS")
                                        {
                                            std::cout<<"MultiQuoter:LONG_TREND_INCREASE_SKEW_OFFSET_BPS:"<<value<<std::endl;
                                            _multiQuoterParams.longTrendIncreaseSkewOffset_BPS = std::stod(value);
                                        }
                                        else if(key == "LONG_TREND_DECREASE_SKEW_OFFSET_BPS")
                                        {
                                            std::cout<<"MultiQuoter:LONG_TREND_DECREASE_SKEW_OFFSET_BPS:"<<value<<std::endl;
                                            _multiQuoterParams.longTrendDecreaseSkewOffset_BPS = std::stod(value);
                                        }
                                        else if(key == "SHORT_TREND_INCREASE_SKEW_OFFSET_BPS")
                                        {
                                            std::cout<<"MultiQuoter:SHORT_TREND_INCREASE_SKEW_OFFSET_BPS:"<<value<<std::endl;
                                            _multiQuoterParams.shortTrendIncreaseSkewOffset_BPS = std::stod(value);
                                        }
                                        else if(key == "SHORT_TREND_DECREASE_SKEW_OFFSET_BPS")
                                        {
                                            std::cout<<"MultiQuoter:SHORT_TREND_DECREASE_SKEW_OFFSET_BPS:"<<value<<std::endl;
                                            _multiQuoterParams.shortTrendDecreaseSkewOffset_BPS = std::stod(value);
                                        }
                                        else if(key == "LONG_TREND_INCREASE_SKEW_OFFSET_ABS")
                                        {
                                            std::cout<<"MultiQuoter:LONG_TREND_INCREASE_SKEW_OFFSET_ABS:"<<value<<std::endl;
                                            _multiQuoterParams.longTrendIncreaseSkewOffset_ABS = std::stod(value);
                                        }
                                        else if(key == "LONG_TREND_DECREASE_SKEW_OFFSET_ABS")
                                        {
                                            std::cout<<"MultiQuoter:LONG_TREND_DECREASE_SKEW_OFFSET_ABS:"<<value<<std::endl;
                                            _multiQuoterParams.longTrendDecreaseSkewOffset_ABS = std::stod(value);
                                        }
                                        else if(key == "SHORT_TREND_INCREASE_SKEW_OFFSET_ABS")
                                        {
                                            std::cout<<"MultiQuoter:SHORT_TREND_INCREASE_SKEW_OFFSET_ABS:"<<value<<std::endl;
                                            _multiQuoterParams.shortTrendIncreaseSkewOffset_ABS = std::stod(value);
                                        }
                                        else if(key == "SHORT_TREND_DECREASE_SKEW_OFFSET_ABS")
                                        {
                                            std::cout<<"MultiQuoter:SHORT_TREND_DECREASE_SKEW_OFFSET_ABS:"<<value<<std::endl;
                                            _multiQuoterParams.shortTrendDecreaseSkewOffset_ABS = std::stod(value);
                                        }
                                        else if(key == "SHORT_TREND_QUALIFY_PERCENT_BPS")
                                        {
                                            std::cout<<"MultiQuoter:SHORT_TREND_QUALIFY_PERCENT_BPS:"<<value<<std::endl;
                                            _multiQuoterParams.shortTrendQualifyPercent = std::stod(value);
                                        }
                                        else if(key == "LONG_TREND_QUALIFY_PERCENT_BPS")
                                        {
                                            std::cout<<"MultiQuoter:LONG_TREND_QUALIFY_PERCENT_BPS:"<<value<<std::endl;
                                            _multiQuoterParams.longTrendQualifyPercent = std::stod(value);
                                        }  
                                        else if(key == "MAX_LONG_TREND_SHIFTS")
                                        {
                                            std::cout<<"MultiQuoter:MAX_LONG_TREND_SHIFTS:"<<value<<std::endl;
                                            _multiQuoterParams.maxLongTrendShifts = std::stod(value);
                                        }  
                                        else if(key == "MAX_SHORT_TREND_SHIFTS")
                                        {
                                            std::cout<<"MultiQuoter:MAX_SHORT_TREND_SHIFTS:"<<value<<std::endl;
                                            _multiQuoterParams.maxShortTrendShifts = std::stod(value);
                                        }
                                        else if(key == "MIN_QUOTE_PASSIVE_GAP_ABS")
                                        {
                                            std::cout<<"MultiQuoter:MIN_QUOTE_PASSIVE_GAP_ABS:"<<value<<std::endl;
                                            _multiQuoterParams.minQuotePassiveGap = std::stoi(value);
                                        }
                                        else if(key == "MAX_QUOTE_PASSIVE_GAP_ABS")
                                        {
                                            std::cout<<"MultiQuoter:MAX_QUOTE_PASSIVE_GAP_ABS:"<<value<<std::endl;
                                            _multiQuoterParams.maxQuotePassiveGap = std::stoi(value);
                                        }
                                        else if(key == "TOP_LEVEL_MIN_QTY_FILTER")
                                        {
                                            std::cout<<"MultiQuoter:TOP_LEVEL_MIN_QTY_FILTER:"<<value<<std::endl;
                                            if(value == "true"){
                                                _multiQuoterParams.topLevelMinQtyFilter = true;
                                            }
                                        }
                                        else if(key == "TOP_LEVEL_MIN_QTY_BID")
                                        {
                                            std::cout<<"MultiQuoter:TOP_LEVEL_MIN_QTY_BID:"<<value<<std::endl;
                                            _multiQuoterParams.topLevelMinQtyBid = std::stoi(value);
                                        }
                                        else if(key == "TOP_LEVEL_MIN_QTY_ASK")
                                        {
                                            std::cout<<"MultiQuoter:TOP_LEVEL_MIN_QTY_ASK:"<<value<<std::endl;
                                            _multiQuoterParams.topLevelMinQtyAsk = std::stoi(value);
                                        }
                                        else if(key == "TOP_LEVEL_BIDQTY_SKEW_FACTOR")
                                        {
                                            std::cout<<"MultiQuoter:TOP_LEVEL_BIDQTY_SKEW_FACTOR:"<<value<<std::endl;
                                            _multiQuoterParams.topLevelBidQtySkewFactor = std::stoi(value);
                                        }
                                        else if(key == "TOP_LEVEL_ASKQTY_SKEW_FACTOR")
                                        {
                                            std::cout<<"MultiQuoter:TOP_LEVEL_ASKQTY_SKEW_FACTOR:"<<value<<std::endl;
                                            _multiQuoterParams.topLevelAskQtySkewFactor = std::stoi(value);
                                        }
                                        else if(key == "TOP_LEVEL_MIN_QTY_BID_SKEWFACTOR_NETQTY_POS")
                                        {
                                            std::cout<<"MultiQuoter:TOP_LEVEL_MIN_QTY_BID_SKEWFACTOR_NETQTY_POS:"<<value<<std::endl;
                                            _multiQuoterParams.topLevelBidQtySkewFactorPosNetQty = std::stod(value);
                                        } 
                                        else if(key == "TOP_LEVEL_MIN_QTY_BID_SKEWFACTOR_NETQTY_NEG")
                                        {
                                            std::cout<<"MultiQuoter:TOP_LEVEL_MIN_QTY_BID_SKEWFACTOR_NETQTY_NEG:"<<value<<std::endl;
                                            _multiQuoterParams.topLevelBidQtySkewFactorNegNetQty = std::stod(value);
                                        } 
                                        else if(key == "TOP_LEVEL_MIN_QTY_ASK_SKEWFACTOR_NETQTY_POS")
                                        {
                                            std::cout<<"MultiQuoter:TOP_LEVEL_MIN_QTY_ASK_SKEWFACTOR_NETQTY_POS:"<<value<<std::endl;
                                            _multiQuoterParams.topLevelAskQtySkewFactorPosNetQty = std::stod(value);
                                        } 
                                        else if(key == "TOP_LEVEL_MIN_QTY_ASK_SKEWFACTOR_NETQTY_NEG")
                                        {
                                            std::cout<<"MultiQuoter:TOP_LEVEL_MIN_QTY_ASK_SKEWFACTOR_NETQTY_NEG:"<<value<<std::endl;
                                            _multiQuoterParams.topLevelAskQtySkewFactorNegNetQty = std::stod(value);
                                        }  
                                    }
                                }
                            }
                        }   
                    }               
                }
            }
        }
    }
}
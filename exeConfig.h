#ifndef __EXECONFIG_H__
#define __EXECONFIG_H__

#include <vector>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <set>
#include "frameworkStructures.h"


class exeConfig
{

    public:
        exeConfig(std::string pathName_);
        ~exeConfig();
        exeConfig &operator=(const exeConfig &) & = default; 
        void readFile();
        void parse();
        void parseSniper();
        void parseMainFile();
        void parseStopLossManagerFile();
        void parsePositionManagerFile();
        void parseSupportFile();
        void parseJumperFile();
        void parseMultiQuoterFile();
        void parseQuoterFile(std::string quoterFile);
        void parseSniperFile(std::string sniperFile);
        std::string getFairPriceMethod() const { return _fairPriceParams.fairPriceType; }
        std::string getSymbolsFile() const { return symbolsFile; } 
        std::unordered_map<std::string , std::string> getQuoterMap() {return quoterMap;}
        std::unordered_map<std::string , quoterParams> getQuoterParamsMap() { return quoterParamsMap;}
        std::unordered_map<std::string , sniperParams> getSniperParamsMap() { return sniperParamsMap;}
        fairPriceParams getFairPriceParams() const {return _fairPriceParams; }
        stopLossParams getStopLossParams() const {return _stopLossParams; }
        positionManagerParams getPositionManagerParams() const {return _positionManagerParams; }
        supportParams getSupportParams() const {return _supportParams;}
        long long int getStartTime() const {return startTime; }
        long long int getEndTime() const {return endTime; }
        long long int getSquareOffTime() const {return squareOffTime; }
        long long int getClearPositionTime() const {return clearPositionTime; }
        bool getMainSwitch() const {return mainSwitch; }
        bool getMainBuySwitch() const {return mainBuySwitch; }
        bool getMainSellSwitch() const {return mainSellSwitch; }
        bool getLtpFilter() const {return useLtpFilter; }
        int getLtpRollingWindow() const {return ltpRollingWindow; }
        jumperParams getJumperParams() const {return _jumperParams; }
        multiQuoterParams getMultiQuoterParams() const {return _multiQuoterParams; }
        bool useJumper() const {return _jumperParams.useJumper;}
        bool useMultiQuoter() const {return _multiQuoterParams.useMultiQuoter;}
        double getPassiveStopfromDpr(){return PassiveStopfromDprBPS;}
        bool getStopOnDprHit(){return stopOnDprHit;}
        bool getIncreaseOnSidePositionsNearDPR(){return IncreaseOnSidePositionsNearDPR;}
    public:
        std::string _pathName;
        std::string _symbol;
        std::string generalFairPriceMethod;
        std::string fairPriceMethod;
        std::string symbolsFile;
        std::vector<std::string> quoterVector; 
        std::unordered_map<std::string , std::string> quoterMap;
        std::unordered_map<std::string , quoterParams> quoterParamsMap;
        std::vector<std::string> sniperVector; 
        std::unordered_map<std::string , std::string> sniperMap;
        std::unordered_map<std::string , sniperParams> sniperParamsMap;
        long long int startTime = 91500;
		long long int endTime = 150000;
		long long int squareOffTime = 152000;
		long long int clearPositionTime = 0;
		bool useLtpFilter = false;
		int ltpRollingWindow = 5;
        fairPriceParams _fairPriceParams;
        stopLossParams _stopLossParams;
        positionManagerParams _positionManagerParams;
        supportParams _supportParams;
        jumperParams _jumperParams;
        multiQuoterParams _multiQuoterParams;
        bool mainSwitch = true;
        bool mainBuySwitch = true;
        bool mainSellSwitch = true;
        bool generalUseAbsoluteOffset = false;
        double PassiveStopfromDprBPS = 0;
        bool stopOnDprHit = false;
        bool IncreaseOnSidePositionsNearDPR = false;

};  

#endif
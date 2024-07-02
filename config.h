#ifndef __CONFIGG_H__
#define __CONFIGG_H__

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <string>
// #include <filesystem>
#include <dirent.h>
#include <unordered_map>
#include <vector>
#include <set>
#include <unistd.h>
#include "frameworkStructures.h"
using namespace std;
// namespace fs = std::filesystem; 

class Config
{

    public:
        Config(std::string pathName_);
        ~Config();
        Config &operator=(const Config &) & = default; 
        void getAllFiles(std::string pathName_);
        void readSymbolFile();
        std::string getSymbolFromTypeAndAsset(std::string type , std::string asset);
        std::string parseAbstractSymbolName(std::string value);
        void parse(std::string pathName_);
        std::unordered_map<std::string,std::vector<std::string>> getSymbolVectorPair() {return symbolVector;}
        std::unordered_map<std::string, portfolioSymbols> getSymbolVectorPortfolioParams() {return symbolVectorPortfolioParams;}
        std::string getContractFile(){ return contractFile;}
        bool getUseUncrossedBook(){return useUncrossedBook;}
    public:
        bool useUncrossedBook = false;
        std::string _pathName;
        std::string contractFile;
        std::vector<std::string> symbolFileVector;
        std::unordered_map<std::string,std::string> symbolTypeMap;
        std::string exeConfigPath;
        std::unordered_map<std::string,std::vector<std::string>> symbolVector;
        std::unordered_map<std::string, portfolioSymbols> symbolVectorPortfolioParams;
};

#endif
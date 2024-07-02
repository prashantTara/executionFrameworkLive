#include "config.h"

Config::Config(std::string pathName_): _pathName(pathName_)
{
    parse(_pathName);
    getAllFiles(exeConfigPath);
    readSymbolFile();
}

Config::~Config()
{
}

void Config::parse(std::string pathName_)
{
    std::ifstream configFileStream(pathName_);
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
                    if(key == "CONFIG_FILE_PATH")
                    {
                        std::cout<<"CONFIG_FILE_PATH:"<<value<<std::endl;
                            exeConfigPath = value;
                    }
                    else if(key == "CONTRACT_FILE")
                    {
                        std::cout<<"CONTRACT_FILE:"<<value<<std::endl;
                            contractFile = value;
                    }
                    else if(key == "USE_UNCROSSED_BOOK")
                    {
                        std::cout<<"USE_UNCROSSED_BOOK:"<<value<<std::endl;
                        if(value == "true"){
                            useUncrossedBook = true;
                        }
                    }
                    else if (key == "SYMBOL_TYPE")
                    {
                        std::istringstream ss(value);
                        std::string st;
                        std::vector<std::string> vec;
                        while(std::getline(ss, st , ','))
                        {
                            vec.push_back(st);
                        }
                        symbolTypeMap[vec[0]] = vec[1];
                        std::cout<<vec[0]<<" "<<vec[1]<<std::endl;
                    }
                }
            }
        }
    }
}

void Config::getAllFiles(std::string pathName_)
{   
        const char* directoryPath = pathName_.c_str();
        DIR* directory = opendir(directoryPath);
        if (directory) 
        {
            struct dirent* entry;
            while ((entry = readdir(directory)) != nullptr) 
            {
                if (entry->d_type == DT_REG || entry->d_type == DT_DIR) 
                {
                    std::string file = entry->d_name ;
                    if(file == "." || file == ".."){
                        continue;
                    }
                    file = pathName_ + file;
                    symbolFileVector.push_back(file);
                    std::cout<<file<<std::endl;
                }
            }
            closedir(directory);
        } 
        else 
        {
            std::cerr << "Error opening directory" << std::endl;
        }
}


std::string Config::getSymbolFromTypeAndAsset(std::string type , std::string asset)
{
    std::string Symbol = "";
    if(symbolTypeMap.find(type) != symbolTypeMap.end())
    { 
        size_t found = type.find("FUT");
        if(found != std::string::npos)
        {
            Symbol = asset + "_" + symbolTypeMap[type] + "_FUT" ;
        }
        else{
            Symbol = asset + "_" + symbolTypeMap[type];
        }
    }
    return Symbol;
}

std::string Config::parseAbstractSymbolName(std::string value)
{
    std::string Type = value.substr(0 , value.find_last_of("_") );
    std::string Asset = value.substr(value.find_last_of("_") + 1 , value.length());
    return getSymbolFromTypeAndAsset(Type , Asset);

}

void Config::readSymbolFile()
{
    for(auto file : symbolFileVector)
    {
        std::string filename = file;
        std::ifstream configFileStream(filename);
        if (configFileStream.is_open())
        {
            std::vector<std::string> symAsset;
            portfolioSymbols ps;
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
                        size_t found = key.find("LEADER");
                        if (found != std::string::npos)
                        {
                            std::string symbol = parseAbstractSymbolName(value); 
                            ps.leaderVector.push_back(symbol);
                            symAsset.push_back(symbol);
                        }
                        else if (key == "LAGGER")
                        {
                            std::string symbol = parseAbstractSymbolName(value); 
                            ps.lagger = symbol;
                            symAsset.push_back(symbol);
                        }
                        else if (key == "USE_HEDGER"){
                            if(value == "true"){
                                ps.useHedger = true;
                            }
                        }
                        else if (key == "USE_CURRENCY_HEDGER"){
                            if(value == "true"){
                                ps.useCurrencyHedger = true;
                            }
                        }
                        else if (key == "HEDGER" && ps.useHedger == true){
                            std::string symbol = parseAbstractSymbolName(value); 
                            ps.hedger = symbol;
                            symAsset.push_back(symbol);
                        }
                        else if (key == "CURRENCY_HEDGER" && ps.useCurrencyHedger == true){
                            std::string symbol = parseAbstractSymbolName(value);     
                            ps.currencyHedger = symbol;
                            symAsset.push_back(symbol);
                        }

                    }
                }
            }
            symbolVector[filename] = symAsset;
            symbolVectorPortfolioParams[filename] = ps;
        }
    }
}
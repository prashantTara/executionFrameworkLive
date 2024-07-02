import pandas as pd
from itertools import product
import os

#rmcmd = 'rm *_20230*.txt'
#os.system(rmcmd)

sourceLibcmd = 'source libPathForExecution.sh'
os.system(sourceLibcmd)

cmd = './simulatorSim.out config.txt strategyConfig.txt' 


rollingWindow = [300]
spread = [10]
threshold = [ 0.3]
maxPosition = [2 ]
positionSkew = [2]
postionSkewFactor = [0.2]
trendWindow = [10]
trendSkew = [  0.3]
zscore = [3]
meanRev = [1]
aggresive = [0]

# x = [[300,10,0.1,3,2,0.1,30,0.2,3,1,0]]

dates = ["20230608"]
assets = ["TATASTEEL","TATAMOTORS","BAJFINANCE","M&M","KOTAKBANK","ULTRACEMCO","HDFCBANK","MARUTI","HDFCLIFE","HEROMOTOCO","INFY","TITAN","ICICIBANK","AXISBANK","HDFC","ITC","INDUSINDBK","RELIANCE","HINDUNILVR","TCS","WIPRO","NTPC","DRREDDY","SBIN","EICHERMOT","BAJAJFINSV","LT","BHARTIARTL","TECHM","JSWSTEEL"]
x = list(product(assets,rollingWindow,spread,threshold ,maxPosition, positionSkew , postionSkewFactor , trendWindow , trendSkew , zscore , meanRev , aggresive ))

f1 = ["1372516200"]
f2 = ["1374935400"]

with open('config.txt', 'r', encoding='utf-8') as file:
    data = file.readlines()

with open('symbols.txt', 'r', encoding='utf-8') as file:
    symbolData = file.readlines()
with open('paramsPerAsset.txt', 'r', encoding='utf-8') as file:
    paramsData = file.readlines()

for i in range(0, len(dates)):
    for params in x:
        sym = params[0]
        print(sym , dates[i] )
        dateConf = "DATE="+dates[i]+"\n"
        sf1 = sym+"_"+f1[i]+"_FUT"
        sf2 = sym+"_"+f2[i]+"_FUT"
        symbolConf = "SYMBOL="+sf1+","+sf2
        data[1] = dateConf
        data[16]=symbolConf
        with open('config.txt', 'w', encoding='utf-8') as file:
            file.writelines(data)

        assetConf = "ASSET="+sym+"\n"
        f1conf = "f1="+f1[i]+"\n"
        f2conf = "f2="+f2[i]
        symbolData[0]=assetConf
        symbolData[1]=f1conf
        symbolData[2]=f2conf
        with open('symbols.txt', 'w', encoding='utf-8') as file:
            file.writelines(symbolData)
        
        #paramsConf = ""
        #for j in params:
         #   paramsConf = paramsConf + str(j) + ","
        #paramsData[0] = paramsConf
        #with open('paramsPerAsset.txt', 'w', encoding='utf-8') as file:
         #   file.writelines(paramsData) 

        os.system(cmd)
        with open("./summary/TBT_FO_"+dates[i]+".txt", 'r', encoding='utf-8') as file:
            summaryData = file.readlines()
        z = "summary_"+dates[i]+".txt"
        with open(z , 'a') as f:
            cd = dates[i] + "," + sym + "," + summaryData[0]
            f.write(cd)
            f.write('\n')


            

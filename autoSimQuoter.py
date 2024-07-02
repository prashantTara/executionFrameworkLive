import pandas as pd
from itertools import product
import os

#rmcmd = 'rm *_20230*.txt'
#os.system(rmcmd)

sourceLibcmd = 'source libPathForExecution.sh'
os.system(sourceLibcmd)

cmd = './simulator.out config.txt strategyConfig.txt' 


spread = ["0.0003","0.0005" , "0.001"]
# spread = ["0.0003"]
passiveModifyThreshold = ["0.0002" , "0.0003"]
aggresiveModifyThreshold = ["0.0001" , "0.0002"]
positionSkew = ["2" , "4"]
postionSkewFactor = ["0.0001" , "0.0002"]
maxShifts = ["3" , "4"]
ltpwindow = ["5" , "10", "20"]


# x = [[300,10,0.1,3,2,0.1,30,0.2,3,1,0]]

dates = ["20231121"]
assets = ["TATASTEEL","TATAMOTORS","BAJFINANCE","M&M","KOTAKBANK","ULTRACEMCO","HDFCBANK","MARUTI","HDFCLIFE","HEROMOTOCO","INFY","TITAN","ICICIBANK","AXISBANK","HDFC","ITC","INDUSINDBK","RELIANCE","HINDUNILVR","TCS","WIPRO","NTPC","DRREDDY","SBIN","EICHERMOT","BAJAJFINSV","LT","BHARTIARTL","TECHM","JSWSTEEL"]
# assets = ["TATASTEEL","TATAMOTORS","BAJFINANCE","KOTAKBANK" ,"ICICIBANK"]
# assets = ["BAJFINANCE"]
x = list(product(assets,spread,passiveModifyThreshold,aggresiveModifyThreshold ,positionSkew, postionSkewFactor , maxShifts))

f1 = ["1385821800"]
f2 = ["1388241000"]

with open('config.txt', 'r', encoding='utf-8') as file:
    data = file.readlines()

with open('strategyConfig.txt', 'r', encoding='utf-8') as file:
    symbolData = file.readlines()
with open('params/NSE_F2/MM_RATIO_EXEC_TATAMOTORS', 'r', encoding='utf-8') as file:
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
        data[7]=symbolConf
        with open('config.txt', 'w', encoding='utf-8') as file:
            file.writelines(data)

        leaderConf = "LEADER=NSE_FUT_F1_"+sym+"\n"
        laggerConf = "LAGGER=NSE_FUT_F2_"+sym+"\n"
        paramsData[0] = leaderConf
        paramsData[1] = laggerConf
        positionShift = "POSITION_SHIFT_QTY="+params[4]+"\n"
        positionfactor1 = "BID_DECREASE_POSITION_SKEW_FACTOR_BPS="+params[5]+"\n"
        positionfactor2 = "BID_INCREASE_POSITION_SKEW_FACTOR_BPS="+params[5]+"\n"
        positionfactor3 = "ASK_DECREASE_POSITION_SKEW_FACTOR_BPS="+params[5]+"\n"
        positionfactor4 = "ASK_INCREASE_POSITION_SKEW_FACTOR_BPS="+params[5]+"\n"
        paramsData[88] = positionShift
        paramsData[91] = positionfactor1
        paramsData[92] = positionfactor2
        paramsData[94] = positionfactor3
        paramsData[95] = positionfactor4
        positionShifts1 = "BID_DECREASE_MAX_SHIFTS="+params[6]+"\n"
        positionShifts2 = "BID_INCREASE_MAX_SHIFTS="+params[6]+"\n"
        positionShifts3 = "ASK_DECREASE_MAX_SHIFTS="+params[6]+"\n"
        positionShifts4 = "ASK_INCREASE_MAX_SHIFTS="+params[6]+"\n"
        paramsData[103] = positionShifts1
        paramsData[104] = positionShifts2
        paramsData[105] = positionShifts3
        paramsData[106] = positionShifts4
        offset1 = "BID_OFFSET_BPS="+params[1]+"\n"
        offset2 = "ASK_OFFSET_BPS="+params[1]+"\n"
        paramsData[244] = offset1
        paramsData[245] = offset2
        thres1 = "QUOTE_MODIFY_PASSIVE_THRESHOLD_BID_BPS="+params[2]+"\n"
        thres2 = "QUOTE_MODIFY_PASSIVE_THRESHOLD_ASK_BPS="+params[2]+"\n"
        thres3 = "QUOTE_MODIFY_AGGRESIVE_THRESHOLD_BID_BPS="+params[3]+"\n"
        thres4 = "QUOTE_MODIFY_AGGRESIVE_THRESHOLD_ASK_BPS="+params[3]+"\n"
        paramsData[249] = thres1
        paramsData[250] = thres2
        paramsData[251] = thres3
        paramsData[252] = thres4
        # ltpWin = "LTP_FILLER_ROLLING_WINDOW="+params[7]+"\n"
        # paramsData[29] = ltpWin
        
        stratConfSym1 = "SYMBOL_TYPE=NSE_FUT_F1,"+f1[i]+"\n"
        stratConfSym2 = "SYMBOL_TYPE=NSE_FUT_F2,"+f2[i]+"\n"
        symbolData[2]=stratConfSym1
        symbolData[3]=stratConfSym2
        with open('strategyConfig.txt', 'w', encoding='utf-8') as file:
            file.writelines(symbolData)
        
        #paramsConf = ""
        #for j in params:
         #   paramsConf = paramsConf + str(j) + ","
        #paramsData[0] = paramsConf
        with open('params/NSE_F2/MM_RATIO_EXEC_TATAMOTORS', 'w', encoding='utf-8') as file:
           file.writelines(paramsData) 

        os.system(cmd)
        with open("./summary/TBT_NSE_FO_"+dates[i]+".txt", 'r', encoding='utf-8') as file:
            summaryData = file.readlines()
        z = "summary_"+dates[i]+".txt"
        with open(z , 'a') as f:
            cd = dates[i] + "," + ','.join(params) + "," + summaryData[0]
            f.write(cd)
            f.write('\n')


            

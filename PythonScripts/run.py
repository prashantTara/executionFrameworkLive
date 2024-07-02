from os import listdir
import os
import sys,os

def read_summary(f,outFile):       
        with open(f) as f:
                lines = f.readlines()

                for line in lines:
                        l = line.split()
                        data_dict = {}
                        for word in l:
                            if ":" in word:
                                data_dict[word.split(":")[0]] = float(word.split(":")[1])
                        #print(data_dict)
                        ##print(l[0].split("/")[2])
                        strategyConfig = {}
                        with open("Strategy/"+l[0].split("/")[2]+"/strategyConfig.txt") as configFile:
                            for configline in configFile.readlines():
                                strategyConfig[configline.split("=")[0].strip()]=configline.split("=")[1].strip()
                        #print(strategyConfig)
                        data_dict["totalPNL"] = data_dict["avgPNL"]*(data_dict["NegetiveDays"]+data_dict["ZeroPNLDays"]+data_dict["PositiveDays"])
                        data_dict["grossPNL"] = data_dict["avgPNL"]*(data_dict["NegetiveDays"]+data_dict["ZeroPNLDays"]+data_dict["PositiveDays"]) + data_dict["TotalbuyTransactionCost"] +data_dict["TotalsellTransactionCost"]
                        print("Zentry > {}({})[{},{}]({}) sell: SL {} TP {} a[{}min,{}] {}-{} symbolSL{}; {}; {}; {}; {}; {}; {};{};{};".format(strategyConfig["zScoreThreshold"],strategyConfig["entryZScorePeriod"],strategyConfig["zScoreSL"],strategyConfig["zScoreExit"],
							strategyConfig["exitZScorePeriod"],
                            strategyConfig["stopLossPerc"],strategyConfig["takeProfitPerc"],int(strategyConfig["activationDuration"])/100,strategyConfig["zScoreActivation"],
							strategyConfig["strategyStartTime"],
							strategyConfig["strategyEndTime"],
							strategyConfig["symbolStopLoss"],
                            data_dict["avgPNL"]/data_dict["stDevPNL"],
                            data_dict["totalPNL"]/data_dict["DrawdownPNL"],
                            data_dict["grossPNL"]/(data_dict["TotalbuyTransactionCost"]+data_dict["TotalsellTransactionCost"]),
                            data_dict["PositiveDays"]/data_dict["NegetiveDays"],
                            data_dict["avgPNL"],
							data_dict["AnnualSharpRatio"],
							data_dict["AnnualReturn"],l[0].split("/")[2]))



def main():

        symbol = "Total"                                                # Symbol defaults to 'Total' for COMBINED_ALL_STRATS
        ParentSearchDir = sys.argv[1]                                   # Directory for Strategy Search Files
        if len(sys.argv)==3:                                            # if symbol is passed then used accordingly
                symbol = sys.argv[2]    

        if not os.path.exists(ParentSearchDir):
                return
        print("test1")
                                
        FileList = []
        
        outFile = open("temp.txt",'w')
        read_summary("COMBINED_ALL_STRATS.txt",outFile)
        outFile.close()

if __name__=="__main__":
        main()

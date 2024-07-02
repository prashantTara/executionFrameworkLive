import sys
import subprocess
from os import listdir

def changeConfig(StrategyNumber,Datafreq,Date,DataType):
    print ("\nRunning for Date: " + Date)

    configFilePath = "../Strategy/Strategy" + str(StrategyNumber) + "/config.txt"
    stratConfigFilePath = "../Strategy/Strategy" + str(StrategyNumber) + "/strategyConfig.txt"
    configLines=open("config.txt","r").readlines()
    configFile = open(configFilePath, "w")
    for line in configLines:
        if "DATE" not in line:
            configFile.write(line)
        else:
            configFile.write("DATE="+Date+"\n")
    
    
    configFile.close()
    subprocess.call(['./simulator.out','config.txt','strategyConfig.txt'],cwd="../Strategy/Strategy"+str(StrategyNumber))

def main():
    
    StrategyNumber = sys.argv[1]            # path to the directory

    # Datafreq = str(input())
    # DataType = str(input())
    # StartDate = str(input())
    # EndDate = str(input())

    Datafreq = sys.argv[2]
    DataType = sys.argv[3]
    StartDate = sys.argv[4]
    EndDate = sys.argv[5]

    StartYYYY = int(StartDate[:4])
    EndYYYY = int(EndDate[:4])
    StartMMDD = StartDate[4:8]
    EndMMDD = EndDate[4:8]

    for year in range(StartYYYY, EndYYYY+1):
        DataFileDirectory = "/home/data/nsedata/" + Datafreq + "/FUT/" + str(year)
        #####################3
        '''
            fileList = []
            for f in listdir(DataFileDirectory):
                            fileList.append(f[7:11])
            fileList.sort()
            for f in fileList:
                if f >= StartMMDD and f<=EndMMDD:
                    changeConfig(StrategyNumber,Datafreq ,str(year) + f , DataType,MATCHINGBAR,MAXINPUTDATALENGTHFORINDICATORS,EXECUTIONTIME,MAXINDICATORPASTVALUES,SLTRADE,SLSYMBOL,SLTOTAL,TRAILINGSL,BOOKPROFIT,ORDERVALUE,GAPBETWEENORDERS,MAXPYRAMIDSIZE,CHECKSIGNALWHENSLORDERISPENDING,INTERDAYSON,VOLUMEBASEDMACTHING,PERCENTAGEVOLUMEMATCHING,ORDERMATCHINGINMULTBARS)
                #####################
                '''
        fileList = []
        for f in listdir(DataFileDirectory):
                        fileList.append(f[7:11])
        fileList.sort()
        if year == StartYYYY and year == EndYYYY:
        #   for x in listdir(DataFileDirectory):
        #       f = x[7:11]
            for f in fileList:
                if f >= StartMMDD and f<=EndMMDD:
                    changeConfig(StrategyNumber,Datafreq ,str(year) + f , DataType)
        elif year == StartYYYY:
        #   for x in listdir(DataFileDirectory):
        #       f = x[7:11]
            for f in fileList:
                if f >= StartMMDD:
                    changeConfig(StrategyNumber,Datafreq ,str(year) + f , DataType)
        elif year == EndYYYY:
        #   for x in listdir(DataFileDirectory):
        #       f = x[7:11]
            for f in fileList:
                if f<= EndMMDD:
                    changeConfig(StrategyNumber,Datafreq ,str(year) + f , DataType)
        else:
        #   for x in listdir(DataFileDirectory):
        #       f = x[7:11]
            for f in fileList:
                changeConfig(StrategyNumber,Datafreq ,str(year) + f , DataType)


if __name__=="__main__":
    main()

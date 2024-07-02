import subprocess
import itertools
import os
from os.path import isfile, join
import multiprocessing as mp
import time
import shutil

#Variables to be defined********
NumberOfParallelProcesses = 20                   # number of strategies we want to run together
StrategyNumber = 1
StopLoss = 1
VolumeEnter = 2
GapPercLong = 3
GapPercShort = 4
Datafreq = "MINUTELY"
DataType = "EQ"
DataType = "FO"
#StartDate = "20200401"
#StartDate = "20220101"
StartDate = "20220801"
EndDate = "20221231"
#EndDate = "20220801"
#EndDate = "20211231"
OSStrategies=[5225]
#EndDate = "20191130"

def makeStrategyConfig(config):
    StrategyNumber=config['StrategyNumber']
    print("Making Strategy config: " + str(StrategyNumber)) #printing
    stratConfigFile = open("../Strategy/Strategy" + str(StrategyNumber) + "/strategyConfig.txt", "w")
    for key in config.keys():
        print(key,config[key])
        stratConfigFile.write(str(key)+"="+str(config[key])+"\n")
    stratConfigFile.close()
    subprocess.call(['python', 'runBacktester.py',str(StrategyNumber),Datafreq,DataType,StartDate,EndDate], cwd="./")
    subprocess.call(['python3', 'summary.py',str(StrategyNumber),Datafreq,DataType,StartDate,EndDate], cwd="./")


def main():
#   subprocess.call(['make'], cwd="../")            # compiling binary before executing
    
    start = time.time()

    global StrategyNumber,StopLoss,VolumeEnter,GapPercLong,GapPercShort,Datafreq,DataType,StartDate,EndDate

    print("Number of processors: ", mp.cpu_count())
    StrategyList = []
    if os.path.exists("../Strategy/"):
        shutil.rmtree("../Strategy")
    config=open("strategyConfig.txt","r").readlines()
    config=[x.strip() for x in config]
    config={x.split("=")[0]:x.split("=")[1].split(",") for x in config}
    configs=[dict(zip(config.keys(),x)) for x in list(itertools.product(*list(config.values())))]
    configs=[config for config in list(configs) if ((config["executionType"]=="HEDGE") or (config["executionType"] == "EXIT" and config["hedgeType"] == "FUTURES"))]
    print(len(configs))
    for config in configs:
        if not os.path.exists("../Strategy/Strategy" + str(StrategyNumber)):
            os.makedirs("../Strategy/Strategy" + str(StrategyNumber))
            os.makedirs("../Strategy/Strategy" + str(StrategyNumber) + "/summary")
        subprocess.call(['cp','-f','simulator.out',"Strategy/Strategy"+str(StrategyNumber)+"/simulator.out"], cwd="../")
        myConfig=config.copy()
        myConfig['StrategyNumber']=StrategyNumber
        if StrategyNumber in OSStrategies:
            StrategyList.append([myConfig])
        StrategyNumber += 1
    # Step 1: Init multiprocessing.Pool()
    #print("Creating pool")
    print(len(StrategyList))
    print(StrategyList)
    #pool = mp.Pool(NumberOfParallelProcesses)
    #print (NumberOfParallelProcesses)
    # Step 2: `pool starmap(Python3) where arguments is the number of times we want to call this function,and within these arguments
    # pass the arguments which are req by makeStrategyConfig function
    #print(StrategyList)
    #pool.starmap(makeStrategyConfig, StrategyList )
    while len(StrategyList)>0:
        f=open("num","r")
        lines=f.readlines()
        num=int(lines[0])
        strategyRuns=[]
        pool = mp.Pool(num)
        while len(StrategyList)>0 and len(strategyRuns)<num:
            strategyRuns.append(StrategyList.pop(0))
        pool.starmap(makeStrategyConfig,strategyRuns)
        pool.close()
        pool.join()

    #pool.starmap(makeStrategyConfig, StrategyList)
    # Step 3: close it
    pool.close()
    pool.join()

    #calling summary
#j  for element in StrategyList:

    print(time.time() - start)


if __name__ == "__main__":
    main()

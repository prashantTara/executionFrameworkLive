import subprocess
import os
from os.path import isfile, join
import multiprocessing as mp
import time
import sys

# Variables to be defined********
# number of strategies we want to run together
NumberOfParallelProcesses = 1
StrategyNumber = 1
StopLoss = 1
VolumeEnter = 2
GapPercLong = 3
GapPercShort = 4
Datafreq = "MINUTELY"
DataType = "FO"
StartDate = "20190101"
EndDate = "20191231"
MATCHINGBAR = "0"
CHECKSIGNALWHENSLORDERISPENDING = "1"
INTERDAYSON = "0"
VOLUMEBASEDMACTHING = "0"
PERCENTAGEVOLUMEMATCHING = "100"
ORDERMATCHINGINMULTBARS = "0"


# not used ****************************************
MAXINPUTDATALENGTHFORINDICATORS = "100"
EXECUTIONTIME = "0950"
MAXINDICATORPASTVALUES = "25"
SLTRADE = "200000000000000000"
SLSYMBOL = "400000000000000000"
SLTOTAL = "800000000000000000"
TRAILINGSL = "400000000000000000"
BOOKPROFIT = "400000000000000000"
ORDERVALUE = "100000000000000000"
GAPBETWEENORDERS = "5"
MAXPYRAMIDSIZE = "3"

# *****************************


def makeStrategyConfig(StrategyNumber):

    print("Making Strategy config: " + str(StrategyNumber))  # printing
    stratConfigFile = open("../Strategy/Strategy" +
                           str(StrategyNumber) + "/strategyConfig.txt", "w")

    stratConfigFile.write("StopLoss="+str(StopLoss)+"\nVolumeEnter="+str(VolumeEnter)+"\nGapPercLong="
                          + str(GapPercLong)+"\nGapPercShort="+str(GapPercShort))
    stratConfigFile.close()

    # copying indicatorConfig to StrategyNumber folder
    #subprocess.call(['cp','../../Mega/indicatorsConfig.txt',"../Strategy/Strategy" + str(StrategyNumber)]);
    print(str(StrategyNumber), Datafreq, DataType, StartDate, EndDate, MATCHINGBAR, MAXINPUTDATALENGTHFORINDICATORS, EXECUTIONTIME, MAXINDICATORPASTVALUES, SLTRADE, SLSYMBOL, SLTOTAL,
          TRAILINGSL, BOOKPROFIT, ORDERVALUE, GAPBETWEENORDERS, MAXPYRAMIDSIZE, CHECKSIGNALWHENSLORDERISPENDING, INTERDAYSON, VOLUMEBASEDMACTHING, PERCENTAGEVOLUMEMATCHING, ORDERMATCHINGINMULTBARS)
    subprocess.call(['python', 'runBacktester.py', str(StrategyNumber), Datafreq, DataType, StartDate, EndDate, MATCHINGBAR, MAXINPUTDATALENGTHFORINDICATORS, EXECUTIONTIME, MAXINDICATORPASTVALUES, SLTRADE, SLSYMBOL,
                     SLTOTAL, TRAILINGSL, BOOKPROFIT, ORDERVALUE, GAPBETWEENORDERS, MAXPYRAMIDSIZE, CHECKSIGNALWHENSLORDERISPENDING, INTERDAYSON, VOLUMEBASEDMACTHING, PERCENTAGEVOLUMEMATCHING, ORDERMATCHINGINMULTBARS], cwd="./")


def main():
    #	subprocess.call(['make'], cwd="../")            # compiling binary before executing
    totalStrat = sys.argv[1] 			# indicatorConfigList

    start = time.time()

    global StrategyNumber, StopLoss, VolumeEnter, GapPercLong, GapPercShort, Datafreq, DataType, StartDate, EndDate

    print("Number of processors: ", mp.cpu_count())
    StrategyList = []
    # for StopLoss in range(1000000, 1000001, 1000000):
    # 	for GapPercLong in range(25, 26, 25):
    # 		for GapPercShort in range(20, 21, 10):

    for indiElement in range(1, int(totalStrat)):
        # # creating a folder specific to StrategyNumber (if not created)
        if not os.path.exists("../Strategy/Strategy" + str(StrategyNumber)):
            os.makedirs("../Strategy/Strategy" + str(StrategyNumber))
            os.makedirs("../Strategy/Strategy" +
                        str(StrategyNumber) + "/summary")
        subprocess.call(
            ['cp', '-f', 'simulator.out', "../Strategy/Strategy"+str(StrategyNumber)])
        StrategyList.append([StrategyNumber])
        StrategyNumber += 1

    # Step 1: Init multiprocessing.Pool()
    pool = mp.Pool(NumberOfParallelProcesses)

    print(NumberOfParallelProcesses)
    # Step 2: `pool starmap(Python3) where arguments is the number of times we want to call this function,and within these arguments
    # pass the arguments which are req by makeStrategyConfig function
    pool.starmap(makeStrategyConfig, [(element)
                                      for element in StrategyList], chunksize=1)
    # Step 3: close it
    pool.close()

    # calling summary
    for element in StrategyList:
        subprocess.call(['python3', 'summary.py', str(
            element[0]), Datafreq, DataType, StartDate, EndDate], cwd="./")

    print(time.time() - start)


if __name__ == "__main__":
    main()

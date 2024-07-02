from os import listdir
import numpy as np
import sys
import math
import os

my_dict = {}

minPNL = []
maxPNL = []
drawDown = []
buyValue = []
sellValue = []
buyTransactionCost = []
sellTransactionCost = []
finalPNL = []
buyTransactionCost = 0
sellTransactionCost = 0


def getLastLine(file):
    with open(file) as f:
        for line in f:
            pass
        return line


def calcSummary(file):
    #	global minPNL,maxPNL,drawDown,buyValue,sellValue,buyTransactionCost,sellTransactionCost,finalPNL

    with open(file) as f:
        lines = f.readlines()

        for line in lines:
            listOfLines = line.split()

            if listOfLines[0] not in my_dict.keys():
                listTemp = []
                listTemp.extend(([], [], [], [], [], [], 0, 0,
                                 [], 0, 0, 0, 0, 0, 0, 0, 0, []))
                my_dict[listOfLines[0]] = listTemp

            makeList = []
            for element in listOfLines:
                l = element.split(":")
                try:
                    makeList.append(l[1])
                except IndexError:
                    pass

            # case to handle when total doesnt have no_of_buy_trade and no_of_sell_trades
            if listOfLines[0] == 'Total' and len(makeList) == 13:
                makeList.insert(9, "0")
                # adding dummy entries to preserve the order of indexing
                makeList.insert(10, "0")

            valueOfDict = my_dict[listOfLines[0]]
            valueOfDict[0].append(float(makeList[0]))
            valueOfDict[1].append(float(makeList[1]))
            valueOfDict[2].append(float(makeList[2]))
            valueOfDict[3].append(float(makeList[5]))
            valueOfDict[4].append(float(makeList[6]))
            valueOfDict[15] += float(makeList[9])  # noofbuytrade
            valueOfDict[16] += float(makeList[10])  # noofselltrade

            if listOfLines[0] == "Total":
                valueOfDict[5].append(float(makeList[4]))
                valueOfDict[6] += float(makeList[7])
                valueOfDict[7] += float(makeList[8])
                valueOfDict[8].append(float(makeList[11]))  # totalExposureMax
                valueOfDict[9] = max(valueOfDict[9], float(makeList[11]))
                valueOfDict[10] += float(makeList[12])  # totalExposure75
                valueOfDict[11] = max(valueOfDict[11], float(makeList[12]))
                valueOfDict[12] += float(makeList[13])  # TotalExposure50
                valueOfDict[13] = max(valueOfDict[13], float(makeList[13]))
                valueOfDict[14] += float(makeList[14])  # AvgExposureAvg
                # maxMarginReq
                valueOfDict[17].append(float(makeList[15]))
            else:
                valueOfDict[5].append(float(makeList[3]))
                valueOfDict[6] += float(makeList[11])
                valueOfDict[7] += float(makeList[12])

            my_dict[listOfLines[0]] = valueOfDict

# def calcMedian(array):
# 	l = len(array)
# 	if l == 0:
# 		return 0;
# 	if l%2 == 0:
# 		return (array[int(l/2) - 1] + array[int(l/2)]) / 2
# 	else:
# 		return array[int(l/2)]


def main():
    StrategyNumber = sys.argv[1]
    Datafreq = sys.argv[2]
    DataType = sys.argv[3]
    StartDate = sys.argv[4]
    EndDate = sys.argv[5]

    StartSummFile = Datafreq + "_" + DataType + "_" + StartDate + ".txt"
    EndSummFile = Datafreq + "_" + DataType + "_" + EndDate + ".txt"

    SummaryDirectory = "../Strategy/Strategy" + StrategyNumber + '/' + 'summary/'

    # if directory doesn't exist return.
    if not os.path.exists(SummaryDirectory):
        print("Directory does't exist, exiting!")
        return
    # if not os.path.exists(SummaryDirectory + "/Summary"):					# if Summary folder doesn't exist create one.
    #	os.makedirs(SummaryDirectory + "/Summary")

    outputdir = "../Strategy/Strategy" + StrategyNumber + '/'
    summaryWriteFile = open(outputdir + "Combined_ALL_DATES.txt", 'w')

    summaryFilesList = []
    for f in listdir(SummaryDirectory):
        if f >= StartSummFile and f <= EndSummFile:
            summaryFilesList.append(SummaryDirectory+f)
    print(summaryFilesList)

    summaryFilesList.sort()
    for element in summaryFilesList:
        calcSummary(element)

    for key, value in my_dict.items():
        value[0].sort()
        value[1].sort()
        value[2].sort()
        value[3].sort()
        value[4].sort()
        value[8].sort()


#		minPNL.sort()
#		maxPNL.sort()
#		drawDown.sort()
#		buyValue.sort()
#		sellValue.sort()
        # print minPNL
        # print maxPNL
        # print drawDown
        # print buyValue
        # print sellValue
        # print buyTransactionCost
        # print sellTransactionCost
        summaryWriteFile.write(key+" ")
        summaryWriteFile.write(" MedianminPNL:% s " %
                               (np.median(value[0])))		 # column[1]
        summaryWriteFile.write("MedianmaxPNL:% s " %
                               (np.median(value[1])))		 # column[2]
        summaryWriteFile.write("MediandrawDown:% s " %
                               (np.median(value[2])))		 # column[3]
        summaryWriteFile.write("MedianBuyValue:% s " %
                               (np.median(value[3])))		 # column[6]
        summaryWriteFile.write("MedianSellValue:% s " %
                               (np.median(value[4])))		 # column[7]
        summaryWriteFile.write("TotalBuyTrades:% s " %
                               (value[15])) 			 # TotalBuyTrades
        summaryWriteFile.write("TotalsellTrades:% s " % (
            value[16]))                     # TotalSellTrades
        summaryWriteFile.write("TotalbuyTransactionCost:% s " %
                               (value[6]))		 # column[8]
        summaryWriteFile.write(
            "TotalsellTransactionCost:% s " % (value[7]))		 # column[9]

        summaryWriteFile.write("MaxExposureMax:% s " % (value[9]))
        if len(value[8]) != 0:
            summaryWriteFile.write("TotalExposure75:% s " %
                                   (np.percentile(value[8], 75)))
            summaryWriteFile.write("TotalExposure50:% s " %
                                   (np.percentile(value[8], 50)))

        if len(summaryFilesList) != 0:
            summaryWriteFile.write("AvgExposureAvg:% s " % float(
                float(value[14])/float(len(summaryFilesList))))

        m=0
        for i in range(0,len(value[17])):
            m = max(m,value[17][i])
        summaryWriteFile.write("MaxMarginRequied:% s " % (m))
        
        if len(value[17]) !=0:
            summaryWriteFile.write("MaxMargin75:% s " %
                                       (np.percentile(value[17], 75)))
            summaryWriteFile.write("MaxMargin50:% s " %
                                       (np.percentile(value[17], 50)))

        summaryWriteFile.write("PNLarray "+str(value[5])+" ")	 # column[4]
        summaryWriteFile.write("minPNL:% s " % (min(value[5])))
        summaryWriteFile.write("maxPNL:% s " % (max(value[5])))
        summaryWriteFile.write("avgPNL:% s " % (np.average(value[5])))
        summaryWriteFile.write("medianPNL:% s " % (np.median(value[5])))
        summaryWriteFile.write("varianlPNL:% s " % (np.var(value[5])))
        stDevPNL = (np.std(value[5]))
        summaryWriteFile.write("stDevPNL:% s " % stDevPNL)

    #	print("Median of buyValue is : % s " % (np.median(buyValue)))
    #	print("Median of sellValue is : % s " % (np.median(sellValue)))
    #	print("Total of buyTransactionCost is : % s " % (buyTransactionCost))
    #	print("Total of sellTransactionCost is : % s " % (sellTransactionCost))
    #	print("Minimum PNL is : % s " % (min(finalPNL)))
    #	print("Maximum PNL is : % s " % (max(finalPNL)))
    #	print("Total PNL is : % s " % (sum(finalPNL)))
    #	print("Average PNL is : % s " % (np.average(finalPNL)))
    #	print("Median of PNL is : % s " % (np.median(finalPNL)))
    #	print("Variance of PNL is : % s " % (np.var(finalPNL)))
    #	print("Standard Deviation of PNL is : % s " % (np.std(finalPNL)))

        finalDrawdown = 0
        maxFinalPNL = 0
        cumFinalPNL = np.cumsum(value[5])

        lenCumFinalPNL = len(cumFinalPNL)
        if lenCumFinalPNL > 0:
            maxFinalPNL = cumFinalPNL[0]

        for i in range(0, lenCumFinalPNL):
            if cumFinalPNL[i] > maxFinalPNL:
                maxFinalPNL = cumFinalPNL[i]
            finalDrawdown = max(finalDrawdown, maxFinalPNL -
                                min(cumFinalPNL[i:lenCumFinalPNL]))

        summaryWriteFile.write("DrawdownPNL:% s " % ((finalDrawdown)))
    #	print("Drawdown PNL is : % s " % (finalDrawdown))
        listPositive = []
        listNegetive = []
        listZero = 0
        for element in value[5]:
            if element > 0:
                listPositive.append(element)
            elif element < 0:
                listNegetive.append(element)
            else:
                listZero += 1

        summaryWriteFile.write("PositiveDays:% s " % (len(listPositive)))
        if len(listPositive) > 0:
            summaryWriteFile.write("avgPositive:% s " %
                                   (np.average(listPositive)))
        summaryWriteFile.write("NegetiveDays:% s " % (len(listNegetive)))
        if len(listNegetive) > 0:
            summaryWriteFile.write("avgNegetive:% s " %
                                   (np.average(listNegetive)))

        if key == "Total":
            summaryWriteFile.write("ZeroPNLDays:% s " % (listZero))
        else:
            summaryWriteFile.write("ZeroPNLDays:% s\n" % (listZero))

    # Calculation of Annual Return

        if key == "Total":

            P_L_Array = value[5]
            Total_Exposure_Array = value[8]
            Annual_Return = 0

            maxMarginRequired = value[17]
            Annual_Return_calculated_from_margin = 0
            Annual_Return_Array = []

            for i in range(0, len(P_L_Array)):
                if Total_Exposure_Array[i] != 0:
                    Annual_Return += float(float(P_L_Array[i]) /
                                           float(Total_Exposure_Array[i]))

            for i in range(0, len(P_L_Array)):
                if maxMarginRequired[i] != 0:
                    t = float(float(P_L_Array[i])/float(maxMarginRequired[i]))
                    Annual_Return_calculated_from_margin += t
                    Annual_Return_Array.append(t)

            if len(P_L_Array) != 0:
                Annual_Return /= len(P_L_Array)
                Annual_Return_calculated_from_margin /= len(P_L_Array)


            k = (Annual_Return_calculated_from_margin*25200)
            summaryWriteFile.write("AnnualReturn:% s " % k)
            StDevMargin = (np.std(Annual_Return_Array))

            if StDevMargin!=0:
                k2 = (Annual_Return_calculated_from_margin/StDevMargin)*math.sqrt(252)
                summaryWriteFile.write("SharpRatio:% s\n" % k2)
            else: 
                summaryWriteFile.write("SharpRatio:%0\n")

if __name__ == "__main__":
    main()

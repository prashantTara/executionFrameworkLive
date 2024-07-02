from os import listdir
import os
import sys,os

def main():
	f = open("NIFTY100.txt","r")	
	line = f.readlines()[0].strip()
	f.close()
	symbols = line.split(",")
	symbol_map = {}
	for symbol in symbols:
		symbol_map[symbol[:-3]+"20JANFUT.NFO"] = symbol
	f=open("/home/data/nsedata/MINUTELY/FO/2020/FO_20200101_MINUTELY.txt","r")
	while len(symbols)>0:
		symbol, date, time, _, _, _, close, _, _ = f.readline().strip().split(",")
		if time > "09:15:59":
			break
		if symbol in symbol_map:
			#print(symbol,close)
			if float(close)<50:
				print(symbol_map[symbol],close)
				


if __name__=="__main__":
        main()

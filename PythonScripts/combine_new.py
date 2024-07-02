from os import listdir
import os
import sys,os

def Save_Total_Summary(f,symbol,outFile):	
	with open(f) as f:
		lines = f.readlines()

		for line in lines:
			l = line.split()
			
			if l[0]==symbol:
				outFile.write(f.name+ " " + ' '.join(l) + "\n")

def main():

	symbol = "Total"						# Symbol defaults to 'Total' for COMBINED_ALL_STRATS
	ParentSearchDir = sys.argv[1]					# Directory for Strategy Search Files
	if len(sys.argv)==3:						# if symbol is passed then used accordingly
		symbol = sys.argv[2]	

	if not os.path.exists(ParentSearchDir):
		return
	print("test1")
				
	FileList = []
	for d1 in listdir(ParentSearchDir):
		if d1.startswith("Strategy"):
			for d2 in listdir(ParentSearchDir+"/"+d1):
				if d2.startswith('Combined_ALL_DATES'):
					FileList.append(ParentSearchDir+"/"+d1+"/"+d2) 
	
	outFile = open("COMBINED_ALL_STRATS.txt",'w')
	FileList.sort()
	for f in FileList:
		Save_Total_Summary(f,symbol,outFile)		
	outFile.close()

if __name__=="__main__":
	main()

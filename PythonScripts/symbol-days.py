from os import listdir
import os
import sys,os

def main():
	snumber = sys.argv[1]
	with open(os.path.join("Strategy/Strategy"+snumber,"Combined_ALL_DATES.txt")) as f:
		lines = f.readlines()
		for line in lines:
			l = line.split()
			data_dict = {}
			for word in l:
				if ":" in word:
					data_dict[word.split(":")[0]] = float(word.split(":")[1])
			print(l[0], data_dict["PositiveDays"]+data_dict["NegetiveDays"])
			#print(l[0], data_dict)
if __name__=="__main__":
        main()

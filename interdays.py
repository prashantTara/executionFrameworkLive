import sys,os,shutil,urllib
from datetime import date, timedelta
from urllib.request import urlopen
from zipfile import ZipFile

months = {"01": "JAN", "02": "FEB", "03": "MAR", "04": "APR", "05": "MAY", "06": "JUN",
          "07": "JUL", "08": "AUG", "09": "SEP", "10": "OCT", "11": "NOV", "12": "DEC"}

# updates date in config file with passed date_ argument while keeping all the other values same.
def update_config_file_date(date_):
    print("Updating Config date:",date_)
    with open("config.txt") as f:
        lines = f.readlines()
        f.close()
        outfile = open("config.txt", 'w')
        for i in range(0, len(lines)):
            if lines[i].startswith('DATE'):                                                                                                   # Updating date in config
                outfile.write('DATE='+str(date_)+'\n')
            elif lines[i].startswith('INTERDAYSON'):                                                                                  
                outfile.write('INTERDAYSON=1')
            else:
                outfile.write(lines[i])
        outfile.close()

def main():
    args = sys.argv
    if len(args) < 4:
        print("Please enter DataFrequency,DataType,Start_date and End_date to run Simulator Interday mode. Exiting...")
        exit(1)

    data_frequency=args[1]
    data_type=args[2]
    start_date = date(int(args[3][0:4]), int(args[3][4:6]), int(args[3][6:8]))
    end_date = date(int(args[4][0:4]), int(args[4][4:6]), int(args[4][6:8]))
    
    bhav_parent_dir="/home/data/bhav_files/"+data_type+'/'                                                                                           # Bhav local storage directory
    delta = timedelta(days=1)                                                                                                                         # To increment or decrement date by 1 day
    sim_data_path="/home/data/nsedata/"+data_frequency+"/"+data_type+"/"

    if data_type=="FO": eq_or_der="DERIVATIVES"
    else: eq_or_der="EQUITIES"

#   Cleaning up before running sim in interday mode,for the case if previous interday run failed in between.
    if os.path.exists("temp"):                                                                                         # temp stores the bhav.csv file used by sim in interday mode.
        shutil.rmtree("./temp")

    if os.path.exists("openPositions.csv"):                                                                            # openPostitons.csv stores open positions.
        os.remove("openPositions.csv")

    if not os.path.exists("temp"):
        os.mkdir("temp")

    listDates=[]
	
    while start_date<=end_date:
        if os.path.exists(sim_data_path+str(start_date.strftime("%Y"))+"/"+data_type+"_"+start_date.strftime("%Y%m%d")+"_"+data_frequency+".txt"):
            listDates.append(start_date);
        start_date+=delta;

    for i,start_date in enumerate(listDates):
        print("\n\nRunning on date:",start_date)
        date_ = str(start_date.strftime("%d"))
        year = str(start_date.strftime("%Y"))
        month = str(start_date.strftime("%m")) 
        bhav_file_name=year+month+date_+".csv"

        if not os.path.exists(bhav_parent_dir+bhav_file_name):
            print("bhav not found:",bhav_parent_dir+bhav_file_name,"EXITING !!!!");
        
        update_config_file_date(start_date.strftime("%Y%m%d"))
        os.system("./simulator.out strategyConfig.txt config.txt")
        start_date+=delta

# Cleaning up after running sim in interday mode.
    if os.path.exists("temp"):                                                                                         # temp stores the bhav.csv file used by sim in interday mode.
        shutil.rmtree("./temp")
        print("cleared temp bhav zip files")

if __name__ == "__main__":
    main()

#!/bin/sh
#export LD_LIBRARY_PATH=/home/development/StrategyBox_combined/binaries:$LD_LIBRARY_PATH
# export LD_LIBRARY_PATH=/home/development/StrategyBox_Multi/binaries:$LD_LIBRARY_PATH

#CONFIG=__FOCONFIG__
#STRATEGYMODE=__LIVEMODE__
file_name=StrategyTest_log.txt
errorfile_name=StrategyTest_error.txt
current_time=$(date "+%Y.%m.%d-%H.%M.%S")

new_outputfileName=$file_name"_"$current_time
new_errorfileName=$errorfile_name"_"$current_time
#python3 script.py
echo "Parameters downloaded"

#make
echo "build done"
#taskset -c 10 ./strategyNew.out "config-FO-TBT-Paper.txt" "config.txt" "strategyConfig.txt" 1>$new_outputfileName 2>$new_errorfileName
taskset -c 7 ./simulator.out "config.txt" "strategyConfig.txt" 1>$new_outputfileName 2>$new_errorfileName
#taskset -c 17 ./main.out "config-FO-BAR-Paper.txt" "config.txt" "Mega/indicatorsConfig.txt"
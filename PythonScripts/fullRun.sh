export LD_LIBRARY_PATH=/home/infra/Simulator/Backtester:$LD_LIBRARY_PATH
isStartDate="20200501"
isEndDate="20211231"
osStartDate="20220101"
osEndDate="20220830"
cp config.txt PythonScripts/.
cp strategyConfig.txt PythonScripts/.
cp num PythonScripts/.
currDir=$(pwd)
cd PythonScripts/
sed -i "s/^StartDate.*/StartDate=\"${isStartDate}\"/" runBacktesterForMultStrategies.py
sed -i "s/^EndDate.*/EndDate=\"${isEndDate}\"/" runBacktesterForMultStrategies.py
python3 runBacktesterForMultStrategies.py > temp
cd ..
mv Strategy/ IS-run/
cd PythonScripts/
sed -i "s/^StartDate.*/StartDate=\"${osStartDate}\"/" runBacktesterForMultStrategies.py
sed -i "s/^EndDate.*/EndDate=\"${osEndDate}\"/" runBacktesterForMultStrategies.py
python3 runBacktesterForMultStrategies.py > temp
cd ..
mv Strategy/ OS-run/

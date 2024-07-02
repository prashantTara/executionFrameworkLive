minutely:
	g++ -O3 -L/home/infra/Simulator/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimMinutelyv0.8 -larrow -lparquet -DdebugStrat
tbt:
	g++ -O3 -L/home/infra/Simulator/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimTBTv0.8 -larrow -lparquet -DdebugStrat
snapshot:
	g++ -O3 -L/home/infra/Simulator/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimSnapshotv0.8 -larrow -lparquet -DdebugStrat -DSimSnapshot

minutelyNSEFO:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimMinutelyNSEFOv0.8 -larrow -lparquet -DdebugStrat -Dcsim
minutelyBSEFO:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimMinutelyBSEFOv0.8 -larrow -lparquet -DdebugStrat -Dcsim

minutelyNSECM:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimMinutelyNSECMv0.8 -larrow -lparquet -DdebugStrat -Dcsim
minutelyBSECM:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimMinutelyBSECMv0.8 -larrow -lparquet -DdebugStrat -Dcsim

minutelyNSECD:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimMinutelyNSECDv0.8 -larrow -lparquet -DdebugStrat -Dcsim
minutelyBSECD:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimMinutelyBSECDv0.8 -larrow -lparquet -DdebugStrat -Dcsim

tbtNSEFO:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp portfolio.cpp fairPriceLib/fairPriceMidPrice.cpp fairPriceLib/fairPriceBidAsk.cpp globalVar.cpp printingClass.cpp config.cpp exeConfig.cpp calSupport.cpp latency.cpp stopLoss.cpp positionManager.cpp Quoter.cpp Jumper.cpp multiQuoter.cpp Sniper.cpp strategy.cpp -lSimTBTNSEFOv0.8 -lstdc++fs -larrow -lparquet -Dcsim
tbtNSEFODebug:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp portfolio.cpp fairPriceLib/fairPriceMidPrice.cpp fairPriceLib/fairPriceBidAsk.cpp globalVar.cpp printingClass.cpp config.cpp exeConfig.cpp calSupport.cpp latency.cpp stopLoss.cpp positionManager.cpp Quoter.cpp Jumper.cpp multiQuoter.cpp Sniper.cpp strategy.cpp -lSimTBTNSEFOv0.8 -lstdc++fs -larrow -lparquet -DdebugStrat -Dcsim
tbtNSEFOLive:
	g++ -O3 -L/home/dev/StrategyBox -Wall -o Strategy_Algo_strategyFO.out strategyConnectorLive.cpp portfolio.cpp fairPriceLib/fairPriceMidPrice.cpp fairPriceLib/fairPriceBidAsk.cpp globalVar.cpp printingClass.cpp config.cpp exeConfig.cpp calSupport.cpp latency.cpp stopLoss.cpp positionManager.cpp Quoter.cpp Jumper.cpp multiQuoter.cpp Sniper.cpp strategy.cpp  -D__FOCONFIG__ -D__LIVEMODE__ -D__TBTSTRAT__ -l StrategyFOTBTProd -lstdc++fs -larrow

tbtBSEFO:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimTBTBSEFOv0.8 -larrow -lparquet -DdebugStrat -Dcsim

tbtNSECM:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp portfolio.cpp fairPriceLib/fairPriceMidPrice.cpp fairPriceLib/fairPriceBidAsk.cpp globalVar.cpp printingClass.cpp config.cpp exeConfig.cpp calSupport.cpp latency.cpp stopLoss.cpp positionManager.cpp Quoter.cpp Jumper.cpp multiQuoter.cpp Sniper.cpp strategy.cpp -lSimTBTNSECMv0.8 -lstdc++fs -larrow -lparquet -Dcsim
tbtNSECMDebug:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp portfolio.cpp fairPriceLib/fairPriceMidPrice.cpp fairPriceLib/fairPriceBidAsk.cpp globalVar.cpp printingClass.cpp config.cpp exeConfig.cpp calSupport.cpp latency.cpp stopLoss.cpp positionManager.cpp Quoter.cpp Jumper.cpp multiQuoter.cpp Sniper.cpp strategy.cpp -lSimTBTNSECMv0.8 -lstdc++fs -larrow -lparquet -DdebugStrat -Dcsim

tbtNSECMLive:
	g++ -L /home/dev/StrategyBox -Wall -o Strategy_Algo_strategyCM.out strategyConnectorLive.cpp portfolio.cpp fairPriceLib/fairPriceMidPrice.cpp fairPriceLib/fairPriceBidAsk.cpp globalVar.cpp printingClass.cpp config.cpp exeConfig.cpp calSupport.cpp latency.cpp stopLoss.cpp positionManager.cpp Jumper.cpp multiQuoter.cpp Sniper.cpp Quoter.cpp strategy.cpp -D__CMCONFIG__ -D__LIVEMODE__ -D__TBTSTRAT__ -l StrategyCMTBTProd -lstdc++fs -larrow

tbtBSECM:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimTBTBSECMv0.8 -larrow -lparquet -DdebugStrat -Dcsim


tbtNSECD:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimTBTNSECDv0.8 -larrow -lparquet -DdebugStrat -Dcsim
tbtBSECD:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimTBTBSECDv0.8 -larrow -lparquet -DdebugStrat -Dcsim


snapshotNSEFO:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimSnapshotNSEFOv0.8 -larrow -lparquet -DdebugStrat -DSimSnapshot -Dcsim
snapshotBSEFO:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimSnapshotBSEFOv0.8 -larrow -lparquet -DdebugStrat -DSimSnapshot -Dcsim

snapshotNSECM:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimSnapshotNSECMv0.8 -larrow -lparquet -DdebugStrat -DSimSnapshot -Dcsim
snapshotBSECM:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimSnapshotBSECMv0.8 -larrow -lparquet -DdebugStrat -DSimSnapshot -Dcsim

snapshotNSECD:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimSnapshotNSECDv0.8 -larrow -lparquet -DdebugStrat -DSimSnapshot -Dcsim
snapshotBSECD:
	g++ -O3 -L/home/infra/InternalSimulatorUsers/Backtester -Wall -o simulator.out strategyConnector.cpp strategy.cpp -lSimSnapshotBSECDv0.8 -larrow -lparquet -DdebugStrat -DSimSnapshot -Dcsim



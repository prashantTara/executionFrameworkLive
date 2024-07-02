#include "printingClass.h"

PrintingClass::PrintingClass(std::string logP)
{
	dumpOut.open(logP);
}

std::ofstream* PrintingClass::getDumpAddress()
{
	return &dumpOut;
}

bool PrintingClass::endsWith(const std::string& s, const std::string& suffix)
{
    return s.size() >= suffix.size() && s.rfind(suffix) == (s.size()-suffix.size());
}

int64_t PrintingClass::getCurrentTime()
{
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::chrono::system_clock::duration tp = now.time_since_epoch();
	int64_t nano = std::chrono::duration_cast<std::chrono::nanoseconds>(tp).count();
	return nano;
}

void PrintingClass::checkPrint()
{	int count = 0 ;
	while(1)
	{
	//      mutexLock;
		mtx.lock();
	//      popGlobalQueue;
		if(!sQueue.empty())
		{
			spr spr_=sQueue.front();
			sQueue.pop();
			mtx.unlock();
			std::string s = "|" + spr_.symbol + "|spread:"+std::to_string(spr_.sp)+"|mean&sd:"+std::to_string(spr_.mean) + "|" + std::to_string(spr_.sd)+"\n";
			dumpOut << getCurrentTime() << s;
			count++;
			if(count >=100){
				count = 0;
				if(endsWith(s,"\n") || s == "\n")
					dumpOut << std::flush;
			}
			
		}
	//      mutexUnLock;
	        else
			mtx.unlock();
	}
}

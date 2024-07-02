#ifndef __printingClass__
#define __printingClass__

#include "globalVar.h"
#include <iostream>
#include <fstream>

class PrintingClass{
	private:
	public:
		PrintingClass(std::string);
		std::ofstream* getDumpAddress();
		int64_t getCurrentTime();
		bool endsWith(const std::string& s, const std::string& suffix);
		void checkPrint();
		std::ofstream dumpOut;

};

#endif

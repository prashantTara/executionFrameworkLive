#ifndef __LATENCY_H__
#define __LATENCY_H__

#include <mutex>
#include<unordered_map>
#include<iostream>
#include<chrono>
// #include <fstream>

// #define MAX_DATE 24

struct Details{

int count = 0;
int printAfter = 100;
std::chrono::high_resolution_clock::time_point startTime ;
std::chrono::high_resolution_clock::time_point endTime ;
long long totalDuration = 0;

};

class Latency
{
        public:
                Latency();
                ~Latency();
        public:
                //friend Logger& operator << (Logger &logger_, const char* text_);
        std::unordered_map<std::string , Details > latencyMap;
        void create(std::string, int);
        void start(std::string );
        void end(std::string );

};




#endif
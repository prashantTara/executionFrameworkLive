#include "latency.h"


Latency::Latency()
{}

Latency::~Latency()
{

}

void Latency::create(std::string identifier, int threshold){
    if(latencyMap.find(identifier) != latencyMap.end()){
        std::cout<<"ERROR: PLEASE USE DIFFERENT IDENTIFIER"<<std::endl;
        return;
    }
    Details dt;
    dt.printAfter = threshold;
    latencyMap[identifier] = dt;
}

void Latency::start(std::string identifier)
{
    latencyMap[identifier].startTime = std::chrono::high_resolution_clock::now();

}

void Latency::end(std::string identifier)
{
    latencyMap[identifier].endTime = std::chrono::high_resolution_clock::now();
    latencyMap[identifier].totalDuration +=  std::chrono::duration_cast<std::chrono::nanoseconds>(latencyMap[identifier].endTime - latencyMap[identifier].startTime).count();
    latencyMap[identifier].count++;
    if(latencyMap[identifier].count >= latencyMap[identifier].printAfter){

        std::cout<< "TOTAL TIME FOR CASE : "<<identifier<<" per "<<latencyMap[identifier].printAfter<<" is : "<<latencyMap[identifier].totalDuration<<" nanoseconds"<<std::endl;
        latencyMap[identifier].count = 0;
        latencyMap[identifier].totalDuration = 0;


    }
}
#ifndef __SHAREDVAR_H__
#define __SHAREDVAR_H__

#include <queue>
#include <mutex>
#include <string>
struct spr{
    std::string symbol;
    int sp;
    int mean;
    int sd;
};
extern std::queue<spr> sQueue;
extern std::mutex mtx;
extern int64_t tcpShareBreachedCounter;
#endif

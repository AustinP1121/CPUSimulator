#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>

class ProcessManager
{
private:
    std::ifstream jobsFile; 

    int numOfJobs;

public:
    struct job
    {
        int jobID = 0;
        int jobArrivalTime = 0;
        int cpuCycle = 0;
        int cpuCycleRemainingTime = cpuCycle;

        int wt = 0;
        int tt = 0;
        int ct = 0;
    };

    struct sortByRemainingTime {
    bool operator()(const job& a, const job& b) const {
        return a.cpuCycleRemainingTime > b.cpuCycleRemainingTime;
    }
    };
    
    std::map<int, job> jobs; 


    float turnaroundTime = 0, waitingTime = 0,
    avgTurnaroundTime = 0, avgWaitingTime = 0, 
    completionTime = 0, contextSwitchTime = 0.1, 
    timeQuantum = 4;

    //std::queue<job> jobQueue;

    ProcessManager(std::string);
    ~ProcessManager();

    /***Utilities***/
    void ParseLineData(std::string, float[]); 
    void ProcessFile();
    static bool sortByVal( const std::pair<int, int> &a,
                    const std::pair<int, int> &b);
    static bool sortByCycleVal(const job &a, const job &b);
    static bool sortByArrivalTime(job a, job b);
    void resetVars();

    void ProcessJobFCFS();
    void ProcessJobSJN();
    void ProcessJobSRT();
    void ProcessJobRR();

    void DEBUGPrintJobs();
};
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

    struct job
    {
        int jobID = 0;
        int jobArrivalTime = 0;
        int cpuCycle = 0;
    };

    std::map<int, job> jobs; 

    std::ifstream jobsFile; 

    float numOfJobs;

public:
    float turnaroundTime = 0, waitingTime = 0,
    avgTurnaroundTime = 0, avgWaitingTime = 0, 
    completionTime = 0, contextSwitchTime = 0.1, 
    timeQuantum = 4;

    ProcessManager(std::string);
    ~ProcessManager();

    /***Utilities***/
    void ParseLineData(std::string, float[]); 
    void ProcessFile();
    static bool sortByVal( const std::pair<int, int> &a,
                    const std::pair<int, int> &b);
    void resetVars();

    void ProcessJobFCFS();
    void ProcessJobSJN();
    void ProcessJobSRT();
    void ProcessJobRR();

    void DEBUGPrintJobs();
};
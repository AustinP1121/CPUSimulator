#include "ProcessManager.h"

ProcessManager::ProcessManager(std::string fileName)
{
    jobsFile.open(fileName, std::ios::in);
}

ProcessManager::~ProcessManager()
{
}

void ProcessManager::ParseLineData(std::string lineDataIn, float data[])
{
    int i = 0; 

    std::istringstream s(lineDataIn);
    std::string substring;

    do
    {
        s >> substring;
        
        if (substring == "")
        {
            //do nothing
        }
        else
        {
            data[i] = std::stof(substring);
            i++;
        }

        substring = "";

    } while (s);
    
}

void ProcessManager::ProcessFile()
{
    float dataIn[3];
    
    if (jobsFile.is_open())
    {
        std::string lineData;
        std::getline(jobsFile, lineData);

        //convert string to int
        numOfJobs = std::stoi(lineData);

        for (int i = 0; i < numOfJobs; i++)
        {
            std::getline(jobsFile, lineData);

            ParseLineData(lineData, dataIn);
            
            job inJob;
            
            inJob.jobID = dataIn[0];
            inJob.jobArrivalTime = dataIn[1];
            inJob.cpuCycle = dataIn[2];

            jobs[i] = (dataIn[0], inJob);
        }
        
        jobsFile.close();
    }
    else
    {
        std::cout << "\nFile not open...\n";
    }
}

bool ProcessManager::sortByVal(const std::pair<int, int> &a, const std::pair<int, int> &b)
{
    return a.second < b.second;
}

void ProcessManager::resetVars()
{
    turnaroundTime = 0;
    waitingTime = 0;
    avgTurnaroundTime = 0;
    avgWaitingTime = 0;
}

void ProcessManager::ProcessJobFCFS()
{
    int currentTime = 0;

    for (int i = 0; i < numOfJobs; i++)
    {   
        turnaroundTime = (currentTime + jobs[i].cpuCycle - jobs[i].jobArrivalTime);

        currentTime += jobs[i].cpuCycle;
        
        waitingTime = turnaroundTime - jobs[i].cpuCycle;

        avgTurnaroundTime += turnaroundTime;
        avgWaitingTime += waitingTime;

        std::cout << "\n\nJob " << jobs[i].jobID << std::endl;
        std::cout << "\nTurnaround Time: " << turnaroundTime;
        std::cout << "\nWaiting Time: " << waitingTime;
        std::cout << "\nCurrent Time: " << currentTime;

        waitingTime = 0;
    }
    
    avgTurnaroundTime /= numOfJobs;
    avgWaitingTime /= numOfJobs;
    std::cout << "\n\nAverage Turnaround time: " << avgTurnaroundTime;
    std::cout << "\n\nAverage Waiting Time: " << avgWaitingTime;

    resetVars();
}

void ProcessManager::ProcessJobSJN()
{
    /***Sorting procedure to get the shortest jobs first***/

    //create container for jobs to be sorted
    std::vector<std::pair<int, int>> sortedJobs;

    //create iterator for the jobs map
    std::map<int, job> :: iterator it2;

    //copy and paste jobs from the jobs map to the container
    it2 = jobs.begin();
    
    for (it2; it2 != jobs.end(); it2++)
    {
        sortedJobs.push_back(std::make_pair(it2->first, it2->second.cpuCycle));
    }

    //sort jobs by cpu cycle time
    //from least to most amount of time needed to process
    std::stable_sort(sortedJobs.begin()+1, sortedJobs.end(), sortByVal);

    for (int i = 0; i < sortedJobs.size(); i++)
    {
        std::cout << sortedJobs[i].first + 1 << " " << sortedJobs[i].second << "\n";
    }

    //note: job id is reflected as index of the job rather than number
    
    /***Process each job****/
    
    //current time step
    int currentTime = 0;
    

    for (int i = 0; i < numOfJobs; i++)
    {   
        //current job being processed
        int currentJob;

        //get the job from the container of sorted jobs
        currentJob = sortedJobs[i].first;

        //calculate turnaround time
        turnaroundTime = (currentTime + jobs[currentJob].cpuCycle - jobs[currentJob].jobArrivalTime);

        //compute computation completion time
        currentTime += jobs[currentJob].cpuCycle;
        
        //compute the amount of time the current job waited in queue
        waitingTime = turnaroundTime - jobs[currentJob].cpuCycle;

        //increment averages
        avgTurnaroundTime += turnaroundTime;
        avgWaitingTime += waitingTime;

        //output job stats
        std::cout << "\n\nJob " << jobs[i].jobID << std::endl;
        std::cout << "\nTurnaround Time: " << turnaroundTime;
        std::cout << "\nWaiting Time: " << waitingTime;
        std::cout << "\nCurrent Time: " << currentTime;

        //reset waiting time
        waitingTime = 0;
    }
    
    //compute averages 
    avgTurnaroundTime /= numOfJobs;
    avgWaitingTime /= numOfJobs;
    
    //output runtime stats
    std::cout << "\n\nRuntime Statistics: ";
    std::cout << "\n\n\tAverage Turnaround time: " << avgTurnaroundTime;
    std::cout << "\n\n\tAverage Waiting Time: " << avgWaitingTime;

    //reset all variables
    resetVars();
}

void ProcessManager::DEBUGPrintJobs()
{
    for (int i = 0; i < numOfJobs; i++)
    {
        int a; 
        int b;
        int c;

        a = jobs[i].jobID;
        b = jobs[i].jobArrivalTime;
        c = jobs[i].cpuCycle;

        std::cout << a << " " << b << " " << c << std::endl; 
    }
}

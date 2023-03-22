#include "ProcessManager.h"

ProcessManager::ProcessManager(std::string fileName)
{
    jobsFile.open(fileName, std::ios::in);
    ProcessFile();
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

    for (int i = 0; i < numOfJobs; i++)
    {
        jobs[i].cpuCycleRemainingTime = jobs[i].cpuCycle;
    }
    
}

bool ProcessManager::sortByVal(const std::pair<int, int> &a, const std::pair<int, int> &b)
{
    return a.second < b.second;
}

bool ProcessManager::sortByCycleVal(const job &a, const job &b)
{
    return a.cpuCycleRemainingTime < b.cpuCycleRemainingTime;
}

bool ProcessManager::sortByArrivalTime(job a, job b)
{
    return a.jobArrivalTime < b.jobArrivalTime;
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
    std::map<int, job> :: iterator it;

    //copy and paste jobs from the jobs map to the container
    it = jobs.begin();
    
    for (it; it != jobs.end(); it++)
    {
        sortedJobs.push_back(std::make_pair(it->first, it->second.cpuCycle));
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

void ProcessManager::ProcessJobSRT()
{
    int completed = 0;
    int currentTime = 0;

    resetVars();

    std::vector<job> sortedJobs;

    for (int i = 0; i < numOfJobs; i++)
    {
        sortedJobs.push_back(jobs[i]);
    }

    std::stable_sort(sortedJobs.begin()+2, sortedJobs.end(), sortByCycleVal);

    while (!sortedJobs.empty())
    {
        job currentJob = sortedJobs.front(); 
        sortedJobs.erase(sortedJobs.begin());

        if (currentTime == 0)
        {
            currentTime++;
        }
        
        if (currentJob.cpuCycleRemainingTime > 0)
        {
            currentJob.cpuCycleRemainingTime--;
        }
        
        if (currentJob.cpuCycleRemainingTime == 0)
        {
            if (currentJob.jobID == 1)
            {
                currentTime--;
            }
            else
            {
                currentTime++;
            }

            currentJob.tt = currentTime - currentJob.jobArrivalTime;
            currentJob.wt = currentJob.tt - currentJob.cpuCycle;

            //compute the amount of time the current job waited in queue
            std::cout << "Job " << currentJob.jobID << " completed at time " << currentTime << std::endl;
            std::cout << "Turnaround time: " << currentJob.tt << std::endl;
            avgTurnaroundTime += currentJob.tt;
            std::cout << "Waiting time: " << currentJob.wt << std::endl;
            avgWaitingTime += currentJob.wt;
        }
        
        else
        {
            if (currentJob.cpuCycleRemainingTime > sortedJobs.front().cpuCycleRemainingTime)
            {
                sortedJobs.push_back(currentJob);
                currentTime++;
            }
            else if (currentJob.cpuCycleRemainingTime == sortedJobs.front().cpuCycleRemainingTime)
            {
                if (currentJob.jobArrivalTime < sortedJobs.front().jobArrivalTime)
                {
                    while (currentJob.cpuCycleRemainingTime <= sortedJobs.front().cpuCycleRemainingTime && currentJob.cpuCycleRemainingTime != 0)
                    {
                        currentJob.cpuCycleRemainingTime--;
                        ++currentTime;
                    }
                    if (currentJob.cpuCycleRemainingTime == 0)
                    {
                        if (currentJob.jobID == 1)
                        {
                            currentTime--;
                        }
                        else
                        {
                            currentTime++;
                        }
                        
                        currentJob.tt = currentTime - currentJob.jobArrivalTime;
                        currentJob.wt = currentJob.tt - currentJob.cpuCycle;
      
                        //compute the amount of time the current job waited in queue
                        std::cout << "Job " << currentJob.jobID << " completed at time " << currentTime << std::endl;
                        std::cout << "Turnaround time: " << currentJob.tt << std::endl;
                        avgTurnaroundTime += currentJob.tt;
                        std::cout << "Waiting time: " << currentJob.wt << std::endl;
                        avgWaitingTime += currentJob.wt;
                    }
                }
                
            }
            else if(currentJob.cpuCycleRemainingTime < sortedJobs.front().cpuCycleRemainingTime)
            {
                while (currentJob.cpuCycleRemainingTime < sortedJobs.front().cpuCycleRemainingTime && currentJob.cpuCycleRemainingTime != 0)
                {
                    currentJob.cpuCycleRemainingTime--;
                    ++currentTime;
                }
                if (currentJob.cpuCycleRemainingTime == 0)
                { 
                    if (currentJob.jobID == 1)
                    {
                        currentTime--;
                    }
                    
                    currentTime++;
                    currentJob.tt = currentTime - currentJob.jobArrivalTime;
                    currentJob.wt = currentJob.tt - currentJob.cpuCycle;
  
                    //compute the amount of time the current job waited in queue
                    std::cout << "Job " << currentJob.jobID << " completed at time " << currentTime << std::endl;
                    std::cout << "Turnaround time: " << currentJob.tt << std::endl;
                    avgTurnaroundTime += currentJob.tt;
                    std::cout << "Waiting time: " << currentJob.wt << std::endl;
                    avgWaitingTime += currentJob.wt;
                    
                }
                else
                {
                    sortedJobs.push_back(currentJob);
                    currentTime++;
                }
            }
            else
            {
                sortedJobs.push_back(currentJob);
                currentTime++;
            }
        }
    }

    std::cout << "\n\nAverage Turnaround time: " << avgTurnaroundTime / numOfJobs << std::endl;
    std::cout << "\nAverage Waiting Time: " << avgWaitingTime / numOfJobs << std::endl;

}



void ProcessManager::ProcessJobRR()
{
   std::queue<job> jobQueue;
   std::vector<job> jobList;

   int currentTime = 0;

   for (int i = 0; i < numOfJobs; i++)
   {
       jobQueue.push(jobs[i]);
   }

   while (!jobQueue.empty())
   {
         job currentJob = jobQueue.front();
         jobQueue.pop();
    
         if (currentJob.cpuCycleRemainingTime > 0)
         {
            if (currentJob.cpuCycleRemainingTime > timeQuantum)
            {
                currentJob.cpuCycleRemainingTime -= timeQuantum;                
            }
            else
            {
                currentJob.cpuCycleRemainingTime = 0;
            }
         }

         if (currentJob.cpuCycleRemainingTime == 0)
         {
              currentTime += timeQuantum;
              currentJob.tt = currentTime - currentJob.jobArrivalTime;
              currentJob.wt = currentJob.tt - currentJob.cpuCycle;
    
              //compute the amount of time the current job waited in queue
              std::cout << "Job " << currentJob.jobID << " completed at time " << currentTime << std::endl;
              std::cout << "Turnaround time: " << currentJob.tt << std::endl;
              avgTurnaroundTime += currentJob.tt;
              std::cout << "Waiting time: " << currentJob.wt << std::endl;
              avgWaitingTime += currentJob.wt;
         }
         else
         {
              jobQueue.push(currentJob);
              currentTime += timeQuantum;
         }
   }

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

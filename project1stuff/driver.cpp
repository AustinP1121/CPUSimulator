#include "ProcessManager.h"
#include <iomanip>

using namespace std;

int main()
{
    bool exit = false;
    int choice = 0;

    ProcessManager test("./cpuJobs.txt");

    while (!exit)
    {   
        cout << setw(40) << "CPU Scheduling Algorithms" << endl;
        cout << "\t1. FCFS" << endl;
        cout << "\t2. SJF" << endl;
        cout << "\t3. SRTF" << endl;
        cout << "\t4. RR" << endl;
        cout << "\t5. Exit" << endl;
        cout << "\n\tEnter a number: ";
        cin >> exit;

        switch (choice)
        {
        case 1:
            cout << "\tDo you want to change the context swit" << endl;
            test.ProcessJobFCFS();
            break;
        case 2:
            test.ProcessJobSJN();
            break;
        case 3:
            test.ProcessJobSRT();
            break;
        case 4:
            test.ProcessJobRR();
            break;
        case 5:
            cout << "Exiting..." << endl;
            exit = true;
            break;
        default:
            cout << "Invalid input" << endl;
            break;
        }
    }
    
}

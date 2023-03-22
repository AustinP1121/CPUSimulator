#include "ProcessManager.h"

using namespace std;

int main()
{
    ProcessManager test("./cpuJobs.txt");
    
    test.ProcessFile();

    test.ProcessJobRR();
    
}

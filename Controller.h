//Paul Tudan and Jefferson Kappers
//5-9-18

#include <Job.h>

#include <list>
#include <string>
using namespace std;

class Controler {
public:
	void readInputFile();
    int checkForRequest(int endQuantam);
    void processLineOfInput(string inputLine);
    void quantamStep();
    void completeJob();
    void checkQueues();
    void checkQueues();
    void processNewJob(Job *newJob);
    void insertReadyQueue(Job newJob);
    void insertWaitQueue(Job newJob);
    void insertFIFOQueue(Job newJob);
    void insertSJFQueue(Job newJob);
    Job findJobWithID(int id);

private:
	list<Job> SJFQueue;
	list<Job> FIFOQueue;
	list<Job> readyQueue;
	list<Job> waitQueue;
    int currentTime;
    int maxMemory;
    int freeMemory
    int maxDevices;
    int freeDevices;
    int quantumTime;
    list<Job>::iterator it;
}

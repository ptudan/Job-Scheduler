//Paul Tudan and Jefferson Kappers
//5-9-18

#include <Job.h>

#include <list>
#include <string>
using namespace std;

class Controler {
public:
	Controller controller(string filename);
	void start();

private:
	void initializeController(string inputLine);
	int getNextNum(string inputLine);
	void processNewJob(string inputLine);
	void processRelease(string inputLine);
	void processRequest(string inputLine);
	bool checkValidRequest();
	void processLineOfInput(string inputLine);
	void printStuff(string inputLine);
	void quantamStep();
	void completeJob();
	void checkQueues();
	void waitToReady();
	bool checkFIFOQueue();
	bool checkSJFQueue();
	bool checkWaitQueue();
	void insertNewJob(Job *newJob);
	void insertReadyQueue(Job *newJob);
	void insertWaitQueue(Job *newJob);
	void insertFIFOQueue(Job *newJob);
	void insertSJFQueue(Job *newJob);
	Job findJobWithID(int id);
	string nameOfFile;
    list<Job*> SJFQueue;
    list<Job*> FIFOQueue;
    list<Job*> readyQueue;
    list<Job*> waitQueue;
    list<Job*> completedQueue;
    list<Job>::iterator it;
    int currentTime;
    int maxMemory;
    int freeMemory;
    int maxDevices;
    int freeDevices;
    int quantumTime;
    int nextInput;
    int startTime;
}

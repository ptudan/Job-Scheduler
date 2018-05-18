//Paul Tudan and Jefferson Kappers
//5-9-18

#include <Job.h>

#include <list>
#include <string>
using namespace std;

class Controler {
public:
	void readInputFile();
private:
	list<Job> SJFHoldQueue;
	list<Job> FIFOHoldQueue;
	list<Job> readyQueue;
	list<Job> waitQueue;
    int currentTime;
    int maxMemory;
    int freeMemory
    int numberOfSerialDevices;
    int quantumTime;
}

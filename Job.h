//Paul Tudan and Jefferson Kappers
//5-9-18

#include <string>
using namespace std;

class Job{
public:
	void initiate(int arrivalTime, int ID, int memoryNeed, int maxDevices, int priority, int length);
	String currentStatus();
	bool isComplete();
	void step(int quantam);
	int getMemoryNeed();
	int getLength();
	int getPriority();
	int getMaxDevices();
	int getJobID();
	int getCurrentDevices();
	int getCurrentRequest();
	void releaseDevice(int num);
	void grantRequest();
	int requestDevice(int num);

private:
	int currentRequest;
	int arrivalTime;
	int completionTime;
	int ID;
	int memoryNeed;
	int maxDevices;
	int currentDevices;
	int priority;
	int length;
	int progress;

}
//Paul Tudan and Jefferson Kappers
//5-9-18

#include <string>
using namespace std;

class Job{
public:
	void initiate(int arrivalTime, int ID, int memoryNeed, int maxDevices, int priority, int duration);
	String currentStatus();
	void quantamStep(int quantam);
	void releaseDevice(int num);
	void addDevice(int num);
	int requestDevice(int num);

private:
	int arrivalTime;
	int completionTime;
	int ID;
	int memoryNeed;
	int maxDevices;
	int currentDevices;
	int priority;
	int duration;
	int progress;

}
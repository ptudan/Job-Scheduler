//Paul Tudan and Jefferson Kappers
//5-9-18
#pragma once
#include <string>
using namespace std;

class Job{
	public:
	Job(int arrivalTime, int ID, int memoryNeed, int maxDevices, int priority, int length);
	string currentStatus();
	bool isComplete();
	void complete(int ct);
	int step(int quantam);
	int getMemoryNeed();
	int getLength();
	int getPriority();
	int getMaxDevices();
	int getJobID();
	int getCurrentDevices();
	int getCurrentRequest();
	int releaseDevice(int num);
	void grantRequest();
	void requestDevice(int num);
	int getTurnaroundTime ();
	int arrivalTime;
	int completionTime;
	int ID;
	int memoryNeed;
	int maxDevices;
	int currentDevices;
	int currentRequest;
	int priority;
	int length;
	int progress;

};
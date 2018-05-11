//Paul Tudan and Jefferson Kappers
//5-11-18

#include <string>
#include <iostream>
using namespace std;

class Job{
	int arrivalTime;
	int completionTime;
	int ID;
	int memoryNeed;
	int maxDevices;
	int currentDevices;
	int currentRequest;
	int priority;
	int duration;
	int progress;

	void initiate(int arrivalTime, int ID, int memoryNeed, int maxDevices, int priority, int duration){
		this->arrivalTime = arrivalTime;
		this->ID  = ID;
		this->memoryNeed = memoryNeed;
		this->maxDevices = maxDevices;
		this->priority = priority;
		this->duration = duration;
	}

	String currentStatus(){

	}

	void quantamStep(int quantam){
		if(progress+quantam>duration) progress = duration;
		else progress+=quantam;
	}

	int releaseDevice(int num){
		if(currentDevices-num < 0) currentDevices = 0;
		else currentDevices -= num;
	}

	void addDevice(int num){
		if(currentDevices+num > maxDevices){
			int up = 
			currentDevices = maxDevices;
		else currentDevices += num;

	}

	int requestDevice(int num){
		
	}
}
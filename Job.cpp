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
	int length;
	int progress;

	void initiate(int arrivalTime, int ID, int memoryNeed, int maxDevices, int priority, int length){
		this->arrivalTime = arrivalTime;
		this->ID  = ID;
		this->memoryNeed = memoryNeed;
		this->maxDevices = maxDevices;
		this->priority = priority;
		this->length = length;
		currentRequest = 0;
		currentDevices = 0;
		progress = 0;
	}

	string currentStatus(){
		
	}

	bool isComplete(){
		return progress >= length;
	}

	int step(int quantam){
		if(progress+quantam>length){
			int ret = length - progress;
			progress = length;
			return ret;
		}
		else progress+=quantam;
		return quantam;
	}



	int getMemoryNeed(){
		return memoryNeed;
	}

	int getLength(){
		return length;
	}

	int getPriority(){
		return priority;
	}

	int getMaxDevices(){
		return maxDevices;
	}

	int getJobID(){
		return ID;
	}

	int getCurrentDevices(){
		return currentDevices;
	}

	int getCurrentRequest(){
		return currentRequest;
	}

	int releaseDevice(int num){
		int down;
		if(currentDevices-num < 0){
			down = currentDevices;
			currentDevices = 0;
		}
		else{
			currentDevices -= num;
			down = num;
		}
		return down;
	}

	void grantRequest(){
		currentDevices += currentRequest;
		currentRequest = 0;
	}

	void requestDevice(int num){
		int req;
		if(num + currentRequest > maxDevices - currentDevices){
			currentRequest = maxDevices -(currentDevices);
		}
		else{
			currentRequest +=num;
		}
	}

	int getTurnaroundTime () {
		return arrivalTime  - completionTime;
	}
}
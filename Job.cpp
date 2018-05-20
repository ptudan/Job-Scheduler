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

	int getMaxDevices(){
		return maxDevices;
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

	int addDevice(int num){
		int up;
		if(currentDevices+num > maxDevices){
			up = maxDevices - currentDevices;
			currentDevices = maxDevices;
		}
		else{
			currentDevices += num;
			up = num;
		}
		return up;

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
}
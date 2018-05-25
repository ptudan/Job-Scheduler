//Paul Tudan and Jefferson Kappers
//5-11-18
#pragma once
#include <string>
#include "job.h"
#include <iostream>
using namespace std;



Job::Job(int arrivalTime, int ID, int memoryNeed, int maxDevices, int priority, int length){
		this->arrivalTime = arrivalTime;
		this->ID  = ID;
		this->memoryNeed = memoryNeed;
		this->maxDevices = maxDevices;
		this->priority = priority;
		this->length = length;
		currentRequest = 0;
		currentDevices = 0;
		progress = 0;
		completionTime = -1;
	}

string Job::currentStatus(){
	
}

bool Job::isComplete(){
	return progress >= length;
}

void Job::complete(int ct){
	completionTime = ct;
}

int Job::step(int quantam){
	if(progress+quantam>length){
		int ret = length - progress;
		progress = length;
		return ret;
	}
	else progress+=quantam;
	return quantam;
}



int Job::getMemoryNeed(){
	return memoryNeed;
}

int Job::getLength(){
	return length;
}

int Job::getPriority(){
	return priority;
}

int Job::getMaxDevices(){
	return maxDevices;
}

int Job::getJobID(){
	return ID;
}

int Job::getCurrentDevices(){
	return currentDevices;
}

int Job::getCurrentRequest(){
	return currentRequest;
}

int Job::releaseDevice(int num){
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

void Job::grantRequest(){
	currentDevices += currentRequest;
	currentRequest = 0;
}

void Job::requestDevice(int num){
	int req;
	if(num + currentRequest > maxDevices - currentDevices){
		currentRequest = maxDevices -(currentDevices);
	}
	else{
		currentRequest +=num;
	}
}

int Job::getTurnaroundTime () {
	return arrivalTime  - completionTime;
}
//Paul Tudan and Jefferson Kappers
//5-11-18
#pragma once
#include "Job.h"
#include "Controller.h"
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;


Controller::Controller(string filename){
    nameOfFile = filename;
}

void Controller::start() {
    //this is the main function of the controller
    string oneLineString;
    ifstream inputFile;
    inputFile.open(nameOfFile);
    getline(inputFile, oneLineString);
    initializeController(oneLineString);//setup
    getline(inputFile, oneLineString);
    nextInput = getNextNum(oneLineString.substr(2));
    if (inputFile.is_open()) {
        while (inputFile.good() && currentTime < 9999) {

            if(readyQueue.empty()){//jump to next job if all queues empty
                currentTime = nextInput;
                processLineOfInput(oneLineString);
                getline(inputFile, oneLineString);
            }
            else if(currentTime + quantumTime < nextInput){//perform work without input
                quantumStep();
            }
            else{
                int endQuantum = currentTime + quantumTime;
                list<string> externalEvents;
                while((oneLineString[0] == 'A' || oneLineString[0] == 'D') && nextInput<=endQuantum){//collect external events
                    currentTime = nextInput;
                    externalEvents.push_back(oneLineString);
                    if(inputFile.good()){
                        getline(inputFile, oneLineString);
                        nextInput = getNextNum(oneLineString.substr(2));
                    }
                }
                if(oneLineString[0]=='Q' && nextInput <=endQuantum){//request devices
                    currentTime = nextInput;
                    processRequest(oneLineString);
                    if(inputFile.good()){
                        getline(inputFile, oneLineString);
                        nextInput = getNextNum(oneLineString.substr(2));
                    }
                }
                else if(oneLineString[0]=='L' && nextInput<=endQuantum){//release devices
                    currentTime = nextInput;
                    processRelease(oneLineString);
                    if(inputFile.good()){
                        getline(inputFile, oneLineString);
                        nextInput = getNextNum(oneLineString.substr(2));
                    }
                }

                while(!externalEvents.empty()){//activate external evetns that arrived during quantum
                    processLineOfInput(externalEvents.front());
                    externalEvents.pop_front();
                }
            }
        }
        while(!readyQueue.empty() && currentTime < 9999) quantumStep();
    }
    else {
        cout << "Unable to open file" << endl;
    }
}

void Controller::initializeController(string inputLine){
    if (inputLine[0] == 'C') {
        int ind = 2;
        startTime = getNextNum(inputLine);
        while(inputLine[ind]!='=') ind++;
        ind++;
        maxMemory = getNextNum(inputLine);
        while(inputLine[ind]!='=') ind++;
        ind++;
        maxDevices = getNextNum(inputLine);
        while(inputLine[ind]!='=') ind++;
        ind++;
        quantumTime = getNextNum(inputLine);
    }
    else cout <<"incorrectly attempted to initializeController\n";
}

int Controller::getNextNum(string inputLine){
    string temp = "";
    int counter = 0;
    while(inputLine[counter]!= ' '){
        temp = temp + inputLine[counter];
        counter++;
    }
    return stoi(temp); 
}


void Controller::processNewJob(string inputLine){
    int ind = 2;

    int arrTime = getNextNum(inputLine.substr(ind));
    while(inputLine[ind]!='=') ind++;
    ind++;

    int jobNum = getNextNum(inputLine.substr(ind));
    while(inputLine[ind]!='=') ind++;
    ind++;

    int memReq = getNextNum(inputLine.substr(ind));
    while(inputLine[ind]!='=') ind++;
    ind++;

    int maxDev = getNextNum(inputLine.substr(ind));
    while(inputLine[ind]!='=') ind++;
    ind++;

    int runTime = getNextNum(inputLine.substr(ind));
    while(inputLine[ind]!='=') ind++;
    ind++;

    int prior = getNextNum(inputLine.substr(ind));

        //cout<<"arrival time " <<arrTime << " jobNum "<<jobNum<<" memReq "<<memReq<<" maxDev "<<maxDev<<" runtime " <<runTime<< " priority "<<prior<<endl;
    insertNewJob(new Job(arrTime, jobNum, memReq, maxDev, runTime, prior));

}

void Controller::processRelease(string inputLine){
    int ind = 2;
    int arrTime = getNextNum(inputLine.substr(ind));
    while(inputLine[ind]!='=') ind++;
    ind++;
    int jobNum = getNextNum(inputLine.substr(ind));
    while(inputLine[ind]!='=') ind++;
    ind++;
    int relReq = getNextNum(inputLine.substr(ind));
    if(arrTime != currentTime){
        cout<< "attempted to release devices out of time, arrivalTime of request was "<< arrTime<< " and current time is "<<currentTime<<endl;
        exit(0);
    }
    else if(readyQueue.front()->getJobID() != jobNum){
        cout<<"Device requested for release while not running on the cpu (or the readyQueue is broken)"<<endl;
        exit(0);
    }
    else if(readyQueue.front()->getCurrentDevices() < relReq){
        cout<<"attempted to release too many devices"<<endl;
        exit(0);            
    }
    else{
        //release devices, move to back of ready queue
        Job* relJob = readyQueue.front();
        freeDevices += relJob->releaseDevice(relReq);
        readyQueue.pop_front();
        readyQueue.push_back(relJob);
        while(checkWaitQueue()){};//checks waitqueue for new devices to add to ready
    }
}

void Controller::processRequest(string inputLine){
    int ind = 2;
    int arrTime = getNextNum(inputLine.substr(ind));
    while(inputLine[ind]!='=') ind++;
    ind++;
    int jobNum = getNextNum(inputLine.substr(ind));
    while(inputLine[ind]!='=') ind++;
    ind++;
    int devReq = getNextNum(inputLine.substr(ind));
    if(arrTime != currentTime){
        cout<< "attempted to request devices out of time, arrivalTime of request was "<< arrTime<< " and current time is "<<currentTime<<endl;
        exit(0);
    }
    else if(readyQueue.front()->getJobID() != jobNum){
        cout<<"Device requested while not running on the cpu (or the readyQueue is broken)"<<endl;
        exit(0);
    }
    else{
        Job* reqJob = readyQueue.front();
        reqJob->requestDevice(devReq);
        readyQueue.pop_front();
        if(checkValidRequest()) readyQueue.push_back(reqJob);//move to ready or wait queue depending on whether request accepted
        else insertWaitQueue(reqJob);
    }

}

bool Controller::checkValidRequest(){
    //bankers algorithm
    Job* reqJob = readyQueue.front();
    if(readyQueue.size()== 1){
        if(reqJob->getCurrentRequest() <= freeDevices){
            freeDevices -= reqJob->getCurrentRequest();
            reqJob->grantRequest();
            return true;
        }
    }

    if(reqJob->getCurrentRequest() > freeDevices) return false;


    int needAfterReq = reqJob->getMaxDevices() - (reqJob->getCurrentDevices() + reqJob->getCurrentRequest());
        int minNeed = 0; //minimum required devices of any process in the readyQueue
        int maxNeed = 0; //maximum required devices of any process in the readyQueue
        int totalRetrivableDevices = freeDevices - reqJob->getCurrentRequest();
        int jobHit[readyQueue.size()];
        int counter2 = readyQueue.size()+2;
        while(counter2>=0){
            int counter = 0;
            for(it = readyQueue.begin(); it!= readyQueue.end(); it++){
                if(counter==0 || jobHit[counter]==1){ 
                    counter++;
                    continue;
                }
                
                int jobNeed = (*it)->getMaxDevices() - (*it)->getCurrentDevices();
                if(jobNeed <= totalRetrivableDevices){
                    totalRetrivableDevices += (*it)->getCurrentDevices();
                    jobHit[counter] = 1;
                }
                else if(jobNeed < minNeed) minNeed = jobNeed;
                else if(jobNeed > maxNeed) maxNeed = jobNeed;
                counter++;
            }

            if(jobHit[0]==0){
                if(needAfterReq <= totalRetrivableDevices){
                    totalRetrivableDevices += (reqJob->getCurrentRequest() + reqJob->getCurrentDevices());
                    jobHit[0] = 1;
                }
                else if(needAfterReq < minNeed) minNeed = needAfterReq;
                else if(needAfterReq > maxNeed) maxNeed = needAfterReq;
            }

            if(totalRetrivableDevices < minNeed){
                return false;
            }
            else if(totalRetrivableDevices >= maxNeed){
                freeDevices -= reqJob->getCurrentRequest();
                reqJob->grantRequest();
                return true;
            }
            counter2--;

        }
        cout<<"Error"<<endl;
        return false;

    }

    void Controller::processLineOfInput(string inputLine){
        //call the functions basically
        //update the nextInput variable so that you can do quantams

        if (inputLine[0] == 'Q') {
            cout<<"calling q from ploi, possible error"<<endl;
            processRequest(inputLine);
        }
        else if (inputLine[0] == 'L') {
            cout<<"calling l from ploi, possible error"<<endl;
            processRelease(inputLine);
        }
        else if (inputLine[0] == 'D') {
            printStuff(inputLine);
        }
        else if (inputLine[0] == 'A'){
            processNewJob(inputLine);
        }
        else{
            cout<<"attempted to do something other than CAQLD"<<endl;
        }
    }

    void Controller::printStuff(string inputLine){
        cout<<"jeff pls"<<endl;
    }   

    void Controller::quantumStep(){
        Job* currentJob = readyQueue.front();
        currentTime += currentJob->step(quantumTime);
        if(currentJob->isComplete()){
            completeJob();
            checkQueues();
        }
        else{
            readyQueue.pop_front();
            readyQueue.push_back(currentJob);
        }

    }

    void Controller::completeJob(){
        Job* completingJob = readyQueue.front();
        freeMemory += completingJob->getMemoryNeed();
        freeDevices += completingJob->releaseDevice(completingJob->getCurrentDevices());
        readyQueue.pop_front();
        completingJob->complete(currentTime);
        completedQueue.push_back(completingJob);
    }

    void Controller::checkQueues(){
        while(checkWaitQueue()){};
        while(checkSJFQueue()){};
        while(checkFIFOQueue()){};

    }

    void Controller::waitToReady(){
        //moves the object from the front of the wait queue to the back 
        //of the ready queue, updates devices
        Job* reqJob = waitQueue.front();
        freeDevices -= reqJob->getCurrentRequest();
        reqJob->grantRequest();
        cout << "moving job "<< reqJob->getJobID() << " from wait queue to ready queue \n";
        waitQueue.pop_front();
        readyQueue.push_back(reqJob);
    }

    bool Controller::checkFIFOQueue(){
        //returns true if an element is moved from FIFO queue to ready queue, false if not
        if(FIFOQueue.empty()) return false;
        Job* moveJob = FIFOQueue.front();
        if(moveJob->getMemoryNeed() <= freeMemory){
            freeMemory -= moveJob->getMemoryNeed();
            FIFOQueue.pop_front();
            readyQueue.push_back(moveJob);
            return true;
        }
        return false;
    }

    bool Controller::checkSJFQueue(){
        //returns true if an element is moved from SJF queue to ready queue, false if not
        if(SJFQueue.empty()) return false;
        Job* moveJob = SJFQueue.front();
        if(moveJob->getMemoryNeed() <= freeMemory){
            freeMemory -= moveJob->getMemoryNeed();
            SJFQueue.pop_front();
            readyQueue.push_back(moveJob);
            return true;
        }
        return false;
    }

    bool Controller::checkWaitQueue(){
        //-returns true if an element is moved from wait queue to ready queue, false if not
        //-should be called after a job completes
        //-will perform bankers algorithm based on the currentRequest of the job at the
        //front of the wait queue
        if(waitQueue.empty()) return false;
        Job* reqJob = waitQueue.front();

        if(reqJob->getCurrentRequest() > freeDevices) return false;
        else if(readyQueue.empty()){
            waitToReady();
            return true;
        }
        int needAfterReq = reqJob->getMaxDevices() - (reqJob->getCurrentDevices() + reqJob->getCurrentRequest());
        int minNeed = 0; //minimum required devices of any process in the readyQueue
        int maxNeed = 0; //maximum required devices of any process in the readyQueue
        int totalRetrivableDevices = freeDevices - reqJob->getCurrentRequest();
        int jobHit[readyQueue.size()+1];
        int counter2 = readyQueue.size()+2;
        while(counter2>=0){
            int counter = 0;
            for(it = readyQueue.begin(); it!= readyQueue.end(); it++){
                if(jobHit[counter]==1){ 
                    counter++;
                    continue;
                }
                
                int jobNeed = (*it)->getMaxDevices() - (*it)->getCurrentDevices();
                if(jobNeed <= totalRetrivableDevices){
                    totalRetrivableDevices += (*it)->getCurrentDevices();
                    jobHit[counter] = 1;
                }
                else if(jobNeed < minNeed) minNeed = jobNeed;
                else if(jobNeed > maxNeed) maxNeed = jobNeed;
                counter++;
            }

            if(jobHit[readyQueue.size()]==0){
                if(needAfterReq <= totalRetrivableDevices){
                    totalRetrivableDevices += (reqJob->getCurrentRequest() + reqJob->getCurrentDevices());
                    jobHit[readyQueue.size()] = 1;
                }
                else if(needAfterReq < minNeed) minNeed = needAfterReq;
                else if(needAfterReq > maxNeed) maxNeed = needAfterReq;
            }

            if(totalRetrivableDevices < minNeed){
                return false;
            }
            else if(totalRetrivableDevices >= maxNeed){
                waitToReady();
                return true;
            }
            counter2--;
        }
        cout<<"Error"<<endl;
        return false;

    }
    void Controller::insertNewJob(Job *newJob){
        if(newJob->getMemoryNeed() > maxMemory) cout << "Job exceeds maximum memory capacity\n";
        else if(newJob->getMaxDevices() > maxDevices) cout << "Job exceeds maximum device capacity\n";
        else if(newJob->getMemoryNeed() <= freeMemory) insertReadyQueue(newJob);
        else if(newJob->getPriority() == 1) insertSJFQueue(newJob);
        else insertFIFOQueue(newJob);

    }

    void Controller::insertReadyQueue(Job *newJob){
        if(newJob->getMemoryNeed() > freeMemory){
            cout << "not enough mem to insert job into RQ\n";
            exit(0);
        }
        else{
            freeMemory -= newJob->getMemoryNeed();
            readyQueue.push_back(newJob);
        }
    }

    void Controller::insertWaitQueue(Job *newJob){
        if(waitQueue.empty()){
            waitQueue.push_front(newJob);
            return;
        }
        it = waitQueue.begin();
        while(!((*it)->getCurrentRequest() > newJob->getCurrentRequest()) && it!=waitQueue.end()) it++;
        waitQueue.insert(it, newJob);
    }

    void Controller::insertFIFOQueue(Job *newJob){
        FIFOQueue.push_back(newJob);
    }

    void Controller::insertSJFQueue(Job *newJob){
        if(SJFQueue.empty()){
            SJFQueue.push_front(newJob);
            return;
        }
        it = SJFQueue.begin();
        while(!((*it)->getLength() > newJob->getLength()) && it!=SJFQueue.end()) it++;
        SJFQueue.insert(it, newJob);
    }

    void Controller::findJobWithID(int id) {
/*        //search queues
        cout << "Searching for job with id: \n" << id;

        //SJFQueue
        for (it = SJFQueue.begin(); it != SJFQueue.end(); ++it) {
            if (SJFQueue[it].getJobID() == id) {
                cout << "Job with id " << id << " is in the SFJQueue\n";
                return SJFQueue[it];
            }
            else {
                cout << "Job with id " << id << " not found in SFJQueue\n";
            }
        }

        //FIFOQueue
        for (it = FIFOQueue.begin(); it != FIFOQueue.end(); ++it) {
            if (FIFOQueue[it].getJobID() == id) {
                cout << "Job with id " << id << " is in the FIFOQueue\n";
                return FIFOQueue[it];
            }
            else {
                cout << "Job with id " << id << " not found in FIFOQueue\n";
            }
        }

        //readyQueue
        for (it = readyQueue.begin(); it != readyQueue.end(); ++it) {
            if (readyQueue[it].getJobID() == id) {
                cout << "Job with id " << id << " is in the readyQueue\n";
                return readyQueue[it];
            }
            else {
                cout << "Job with id " << id << " not found in readyQueue\n";
            }
        }

        //waitQueue
        for (it = waitQueue.begin(); it != waitQueue.end(); ++it) {
            if (waitQueue[it].getJobID() == id) {
                cout << "Job with id " << id << " is in the waitQueue\n";
                return waitQueue[it];
            }
            else {
                cout << "Job with id " << id << " not found in waitQueue\n";
            }
        }

        //completedQueue;
        for (it = completeQueue.begin(); it != completeQueue.end(); ++it) {
            if (completeQueue[it].getJobID() == id) {
                cout << "Job with id " << id << " is in the completeQueue\n";
                return completeQueue[it];
            }
            else {
                cout << "Job with id " << id << " not found in completeQueue\n";
            }
        }
        //maybe this should return a blank job
        cout << "Job with id: " << id << " not found\n";*/
    }
    
    void Controller::outputCurrentSystem () {

 /*       Json::Value event;   
        Json::Value vec(Json::arrayValue);
        vec.append(Json::Value(1));
        vec.append(Json::Value(2));
        vec.append(Json::Value(3));

        event["system"]["current time"]= currentTime;
        event["system"]["total_memory"]= maxMemory;
        event["system"]["free_memory"]= freeMemory;
        event["system"]["total_devices"]= maxDevices;
        event["system"]["free_devices"]= freeDevices;
        event["system"]["quantum"]= quantumTime;
        event["system"]["turnaround_time"]= currentTime - startTime;
        event["system"]["weighted_time"]= currentTime - startTime - length;
        event["system"]["running"]= currentTime;
        event["system"]["readyq"]["quques"]= readyQueue.size();
        event["system"]["submitq"]["quques"]= submitQueue.size();
        event["system"]["holdq_2"]["quques"]= SJFQueue.size();
        event["system"]["holdq_1"]["quques"]= FIFOQueue.size();
        event["system"]["completeq"["quques"]]= completedQueue.size();

        //the current contents of each of the queues
        for (it = SJFQueue.begin(); it != SJFQueue.end(); ++it) {
            event["system"]["jobs"]["arrival_time"]= (*it)->arrivalTime;
            event["system"]["jobs"]["devices_allocated"]= (*it)->currentDevices;
            event["system"]["jobs"]["id"]= (*it)->ID;
            event["system"]["jobs"]["remaining_time"]= (*it)->length + (*it)progress;
        }

        for (it = FIFOQueue.begin(); it != FIFOQueue.end(); ++it) {
            event["system"]["jobs"]["arrival_time"]= (*it)->arrivalTime;
            event["system"]["jobs"]["devices_allocated"]= (*it)->currentDevices;
            event["system"]["jobs"]["id"]= (*it)->ID;
            event["system"]["jobs"]["remaining_time"]= (*it)->length + (*it)->progress;
        }

        for (it = readyQueue.begin(); it != readyQueue.end(); ++it) {
            event["system"]["jobs"]["arrival_time"]= (*it)->arrivalTime;
            event["system"]["jobs"]["devices_allocated"]= (*it)->currentDevices;
            event["system"]["jobs"]["id"]= (*it)->ID;
            event["system"]["jobs"]["remaining_time"]= (*it)->length + (*it)->progress;
        }
        
        for (it = waitQueue.begin(); it != waitQueue.end(); ++it) {
            event["system"]["jobs"]["arrival_time"]= (*it)->arrivalTime;
            event["system"]["jobs"]["devices_allocated"]= (*it)->currentDevices;
            event["system"]["jobs"]["id"]= (*it)->ID;
            event["system"]["jobs"]["remaining_time"]= (*it)->length + (*it)->progress;
        }

        for (it = completedQueue.begin(); it != completedQueue.end(); ++it) {
            event["system"]["jobs"]["arrival_time"]= (*it)->arrivalTime;
            event["system"]["jobs"]["completion_time"]= (*it)->completionTime;
            event["system"]["jobs"]["id"]= (*it)->ID;
            event["system"]["jobs"]["remaining_time"]= (*it)->length + (*it)->progress;
        }

        std::cout << event << std::endl;
*/
    }

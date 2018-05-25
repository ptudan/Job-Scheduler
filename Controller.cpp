//Paul Tudan and Jefferson Kappers
//5-11-18

#include <Job.h>
#include <Controller.h>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;


class Controller {
private:
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

    void readInputFile() {
        string testOutputSting;
        ifstream inputFile;
        inputFile.open("input.txt");
        getline(inputFile, oneLineString);
        initalizeController(oneLineString);
        getline(inputFile, oneLineString);
        nextInput = getNextNum(oneLineString.substr(2));
        if (inputFile.is_open()) {
            while (inputFile.good() && currentTime < 9999) {

                if(readyQueue.empty()){
                    currentTime = nextInput;
                    processLineOfInput(string oneLineString);
                }
                else if(currentTime + quantum < nextInput){
                    quantumStep();
                }
                else{
                    endQuantum = currentTime + quantum;
                    list<string> externalEvents;
                    while((oneLineString[0] == 'A' || oneLineString[0] == 'D') && nextInput<=endQuantum){
                        currentTime = nextInput;
                        externalEvents.push_back(oneLineString);
                        if(inputFile.good()){
                            getline(inputFile, oneLineString);
                            nextInput = getNextNum(oneLineString.substr(2));
                        }
                    }
                    if(oneLineString[0]=='Q' && nextInput <=endQuantum){
                        currentTime = nextInput;
                        processRequest(oneLineString);
                        if(inputFile.good()){
                            getline(inputFile, oneLineString);
                            nextInput = getNextNum(oneLineString.substr(2));
                        }
                    }
                    else if(oneLineString[0]=='L' && nextInput<=endQuantum){
                        currentTime = nextInput;
                        processRelease(oneLineString);
                        if(inputFile.good()){
                            getline(inputFile, oneLineString);
                            nextInput = getNextNum(oneLineString.substr(2));
                        }
                    }

                    while(!externalEvents.empty()){
                        processLineOfInput(externalEvents.front());
                        externalEvents.pop_front();
                    }
                }

                getline(inputFile, oneLineString);
                nextInput = getNextNum(oneLineString.substr(2));
            }
            while(!readyQueue.empty() && currentTime < 9999) quantumStep();
        }
        else {
            cout << "Unable to open file" << endl;
        }
    }

    void initializeController(string inputLine){
        if (inputLine.first == 'C') {
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
            quantumTime = = getNextNum(inputLine);
        }
        else cout <<"incorrectly attempted to initializeController\n";
    }

    int getNextNum(string inputLine){
        string temp = "";
        int counter = 0;
        while(inputLine[counter]!= ' '){
            temp = temp + inputLine[counter];
            counter++;
        }
        return stoi(temp); 
    }


    void processNewJob(string inputLine){
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

    void processRelease(string inputLine){
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
            exit();
        }
        else if(readyQueue.front()->getJobID != jobNum){
            cout<<"Device requested for release while not running on the cpu (or the readyQueue is broken)"<<endl;
            exit();
        }
        else if(readyQueue.front()->getCurrentDevices() < relReq){
            cout<<"attempted to release too many devices"<<endl;
            exit();            
        }
        else{
            Job* relJob = readyQueue.front();
            freeDevices += relJob->releaseDevice(relReq);
            readyQueue.pop_front();
            readyQueue.push_back(relJob);
            while(checkWaitQueue()){};
        }
    }

    void processRequest(string inputLine){
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
            exit();
        }
        else if(readyQueue.front()->getJobID != jobNum){
            cout<<"Device requested while not running on the cpu (or the readyQueue is broken)"<<endl;
            exit();
        }
        else{
            Job* reqJob = readyQueue.front();
            reqJob->requestDevice(devReq);
            readyQueue.pop_front();
            if(checkValidRequest()) readyQueue.push_back(reqJob);
            else insertWaitQueue(reqJob);
        }

    }

    bool checkValidRequest(){
        Job* reqJob = readyQueue.front();
        if(readyQueue.size()== 1){
            if(reqJob->getCurrentRequest() <= freeDevices){
                freeDevices -= reqJob->getCurrentRequest();
                reqJob->grantRequest();
                return true;
            }
        }

        if(reqJob.getCurrentRequest() > freeDevices) return false;


        int needAfterReq = reqJob->getMaxDevices() - (reqJob->getCurrentDevices() + curReq);
        int minNeed = 0; //minimum required devices of any process in the readyQueue
        int maxNeed = 0; //maximum required devices of any process in the readyQueue
        int totalRetrivableDevices = freeDevices - reqJob->getCurrentRequest();
        int jobHit[readyQueue.size()];
        int counter2 = readyQueue.size()+2;
        while(counter2>=0){
            int counter = 1;
            for(it = readyQueue.begin()+1; it!= readyQueue.end(); it++){
                if(jobHit[counter]==1){ 
                    counter++;
                    continue;
                }
                
                int jobNeed = it->getMaxDevices() - it->getCurrentDevices();
                if(jobNeed <= totalRetrivableDevices){
                    totalRetrivableDevices += it->getCurrentDevices();
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
                reqJob.grantRequest();
                return true;
            }
            counter2--;

        }
        cout<<"Error"<<endl;
        return false;

    }

    void processLineOfInput(string inputLine){
        //call the functions basically
        //update the nextInput variable so that you can do quantams

        if (inputLine.first == 'Q') {
            cout<<"calling q from ploi, possible error"<<endl;
            processRequest(inputLine);
        }
        else if (intputLine.first == 'L') {
            cout<<"calling l from ploi, possible error"<<endl;
            processRelease(inputLine);
        }
        else if (inputLine.first == 'D') {
            printStuff(inputLine);
        }
        else (inputLine.first == 'A'){
            processNewJob(inputLine);
        }
    }

    void printStuff(string inputLine){
        cout<<"jeff pls"<<endl;
    }   

    void quantamStep(){
        Job* currentJob = readyQueue.front();
        currentTime += currentJob->step(quantam);
        if(currentJob->isComplete()){
            completeJob();
            checkQueues();
        }
        else{
            readyQueue.pop_front();
            readyQueue.push_back(currentJob);
        }

    }

    void completeJob(){
        Job* completingJob = readyQueue.front();
        freeMemory += completingJob->getMemoryNeed();
        freeDevices += completingJob->releaseDevice(completingJob->getCurrentDevices());
        readyQueue.pop_front();
        completingJob->complete(currentTime);
        completedQueue.push_back(completingJob);
    }

    void checkQueues(){
        while(checkWaitQueue()){};
        while(checkSJFQueue()){};
        while(checkFIFOQueue()){};

    }

    void waitToReady(){
        //moves the object from the front of the wait queue to the back 
        //of the ready queue, updates devices
        Job* reqJob = waitQueue.front();
        freeDevices -= reqJob->getCurrentRequest();
        reqJob->grantRequest();
        cout << "moving job "<< reqJob->getJobID() << " from wait queue to ready queue \n";
        waitQueue.pop_front();
        readyQueue.push_back(reqJob);
    }

    bool checkFIFOQueue(){
        //returns true if an element is moved from FIFO queue to ready queue, false if not
        if(FIFOQueue.empty()) return false;
        Job* moveJob = FIFOQueue.begin();
        if(moveJob->getMemoryNeed() <= freeMemory){
            freeMemory -= moveJob->getMemoryNeed();
            FIFOQueue.pop_front();
            readyQueue.push_back(moveJob);
            return true;
        }
        return false;
    }

    bool checkSJFQueue(){
        //returns true if an element is moved from SJF queue to ready queue, false if not
        if(SJFQueue.empty()) return false;
        Job* moveJob = SJFQueue.begin();
        if(moveJob->getMemoryNeed() <= freeMemory){
            freeMemory -= moveJob->getMemoryNeed();
            SJFQueue.pop_front();
            readyQueue.push_back(moveJob);
            return true;
        }
        return false;
    }

    bool checkWaitQueue(){
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
        int needAfterReq = reqJob->getMaxDevices() - (reqJob->getCurrentDevices() + curReq);
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
                
                int jobNeed = it->getMaxDevices() - it->getCurrentDevices();
                if(jobNeed <= totalRetrivableDevices){
                    totalRetrivableDevices += it->getCurrentDevices();
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
    void insertNewJob(Job *newJob){
        if(newJob->getMemoryNeed() > maxMemory) cout << "Job exceeds maximum memory capacity\n";
        else if(newJob->getMaxDevices() > maxDevices) cout << "Job exceeds maximum device capacity\n";
        else if(newJob->getMemoryNeed() <= freeMemory) insertReadyQueue(newJob);
        else if(newJob->getPriority() == 1) insertSJFQueue(newJob);
        else insertFIFOQueue(newJob);

    }

    void insertReadyQueue(Job *newJob){
        if(newJob->getMemoryNeed() > freeMemory){
            cout << "not enough mem to insert job into RQ\n";
            exit();
        }
        else{
            freeMemory -= newJob->getMemoryNeed();
            readyQueue.push_back(newJob);
        }
    }

    void insertWaitQueue(Job *newJob){
        if(waitQueue.empty()){
            waitQueue.push_front(newJob);
            return;
        }
        while(it = waitQueue.begin(); *it.getCurrentRequest() < newJob->getCurrentRequest() && it!=waitQueue.end()) it++;
        waitQueue.insert_after(it, newJob);
    }

    void insertFIFOQueue(Job *newJob){
        FIFOQueue.push_back(newJob);
    }

    void insertSJFQueue(Job *newJob){
        if(SJFQueue.empty()){
            SJFQueue.push_front(newJob);
            return;
        }
        while(it = SJFQueue.begin(); *it.getLength() < newJob->getLength() && it!=SJFQueue.end()) it++;
        SJFQueue.insert_after(it, newJob);
    }

    Job findJobWithID(int id) {
        //search queues
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
        cout << "Job with id: " << id << " not found\n";
    }
}
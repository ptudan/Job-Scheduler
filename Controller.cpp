//Paul Tudan and Jefferson Kappers
//5-11-18

#include <Job.h>
#include <list>
#include <string>
using namespace std;


class Controller {
private:
    list<Job> SJFQueue;
    list<Job> FIFOQueue;
    list<Job> readyQueue;
    list<Job> waitQueue;
    list<Job> completedQueue;
    list<Job>::iterator it;
    int currentTime;
    int maxMemory;
    int freeMemory;
    int maxDevices;
    int freeDevices;
    int quantumTime;
    int nextRequest;

	void readInputFile() {
        string testOutputSting
        ofstream inputFile;
        inputFile.open('input.txt');
        if (inputFile.is_open()) {
            while (!inputFile.eof) {
                getline(inputFile,oneLineString);
                cout<<oneLineString;
                for (i = 0; i <= oneLineString.size(); i++) {
                    if (oneLineString[i] == ) {

                    }
                }
            }
        }
        else {
            cout << "Unable to open file";
        }
    }

    int checkForRequest(endQuantam){
        //TODO
        //return an int for the number of requested devices
        //for release it could be a negative number
        //one function for update instead of release and requrest?
    }

    void processLineOfInput(){
        //call the functions basically
        //update the nextRequest variable so that you can do quantams
    }

    void quantamStep(){
        int endQuantam = currentTime + quantam;
        if(nextRequest>endQuantam){//no requests to process this quantam
            if(readyQueue.empty()){
                currentTime += quantam;
                return;
            }
            Job currentJob = readyQueue.front();
            currentTime += currentJob.step(quantam);
            if(currentJob.isComplete()){
                completeJob();
            }

        }



    }

    void completeJob(){
        //do we need completed queue for logs?
        Job completingJob = readyQueue.front();
        freeMemory += completingJob.getMemoryNeed();
        freeDevices += completingJob.getCurrentDevices();
        readyQueue.pop_front();
        completedQueue.push_back(completingJob);
        checkQueues();
    }

    void checkQueues(){
        while(checkWaitQueue()){};

    }

    void waitToReady(){

    }

    bool checkWaitQueue(){
        //10 total devices, reqJob has 1 currently. requesting 2, max 4
        //readyQueue looks below, number is devicesStillNeeded and devices owned
        //1 3 2 4
        //3 0 1 1
        if(waitQueue.empty()) return false;

        Job reqJob = waitQueue.front();

        if(reqJob.getCurrentRequest() > freeDevices) return false;
        else if(readyQueue.empty()){
            freeDevices -= reqJob.getCurrentRequest();
            reqJob.grantRequest();
            waitQueue.pop_front();
            readyQueue.push_back(reqJob);
            return true;
        }
        int needAfterReq = reqJob.getMaxDevices() - (reqJob.getCurrentDevices() + curReq);
        int minNeed = 0; //minimum required devices of any process in the readyQueue
        int maxNeed = 0; //maximum required devices of any process in the readyQueue
        int totalRetrivableDevices = freeDevices - reqJob.getCurrentRequest();
        int jobHit[readyQueue.size()+1];
        while(true){
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
                else if(jobNeed < minNeed){
                   minNeed = jobNeed;
                }
                else if(jobNeed > maxNeed){
                    maxNeed = jobNeed;
                }
                counter++;
            }
            if(jobHit[readyQueue.size()]==0){
                if(needAfterReq <= totalRetrivableDevices){
                    totalRetrivableDevices += (reqJob.getCurrentRequest() + reqJob.getCurrentDevices());
                }
                else if(needAfterReq < minNeed){
                    minNeed = needAfterReq;
                }
                else if(needAfterReq > maxNeed){
                    maxNeed = needAfterReq;
                }
                jobHit[readyQueue.size()] = 1;
            }
            if(totalRetrivableDevices < minNeed){
                //cout << "unable to move job "<< reqJob.getJobID() << " from wait queue to ready queue \n";
                return false;
            }
            else if(totalRetrivableDevices >= maxNeed){
                cout << "moving job "<< reqJob.getJobID() << " from wait queue to ready queue \n";
                freeDevices -= reqJob.getCurrentRequest();
                reqJob.grantRequest();
                waitQueue.pop_front();
                readyQueue.push_back(reqJob);
                return true;
            }

        }

    }

    void processNewJob(Job *newJob){
        if(*newJob.getMemoryNeed() > maxMemory) cout << "Job exceeds maximum memory capacity\n";
        else if(*newJob.getMaxDevices() > maxDevices) cout << "Job exceeds maximum device capacity\n";
        else if(*newJob.getMemoryNeed() < freeMemory) insertReadyQueue(*newJob);
        else if( )

    }

    void insertReadyQueue(Job newJob){
        if(newJob.getMemoryNeed() > freeMemory){
            cout << "not enough mem to insert job into RQ\n";
            exit();

        }
        else{
            freeMemory -= newJob.getMemoryNeed();
            readyQueue.push_back(newJob);
        }

    }

    void insertWaitQueue(Job newJob){
        if(waitQueue.empty()){
            waitQueue.push_front(newJob);
            return;
        }
        while(it = waitQueue.begin(); *it.getCurrentRequest() < newJob.getCurrentRequest() && it!=waitQueue.end()) it++;
        waitQueue.insert_after(it, newJob);
    }

    void insertFIFOQueue(Job newJob){
        FIFOQueue.push_back(newJob);
    }

    void insertSJFQueue(Job newJob){
        if(SJFQueue.empty()){
            SJFQueue.push_front(newJob);
            return;
        }
        while(it = SJFQueue.begin(); *it.getLength() < newJob.getLength() && it!=SJFQueue.end()) it++;
        SJFQueue.insert_after(it, newJob);
    }
}
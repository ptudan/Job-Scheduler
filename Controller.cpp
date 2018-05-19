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

    }

    void checkQueues(){
        //wait queue has a list of devices sorted in ascending order 
        //of currentDevicesRequested

    }

    void 

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

        it = waitQueue.begin();
        while(*it.getCurrentRequest() < newJob.getCurrentRequest() && it!=waitQueue.end()) it++;
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

        it = SJFQueue.begin();
        while(*it.getLength() < newJob.getLength() && it!=SJFQueue.end()) it++;
        SJFQueue.insert_after(it, newJob);
    }
}
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
                cout<<oneLineString<<endl;
                if (oneLineString.first == 'C') {
                    //initiate system with the following configuration from oneLineString:

                    //start time
                    currentTime = (int)oneLineString[2];

                    //main memory capacity and current free memory capacity
                    maxMemory = (int)oneLineString[7];
                    freeMemory = (int)oneLineString[7];

                    //number of serial devices
                    maxDevices = (int)oneLineString[11];
                    freeDevices = (int)oneLineString[11];

                    //quantum time
                    quantumTime = (int)oneLineString.last;

                }
                else if (oneLineString.first == 'A') {
                    //job intialization with these spesifications:
                    //int arrivalTime, int ID, int memoryNeed, int maxDevices, int priority, int length
                    Job newJob((int)oneLineString[3], (int)oneLineString[7], (int)oneLineString[11], (int)oneLineString[15], (int)oneLineString[19], (int)oneLineString[23]);
                    processNewJob(newJob);
                }
                else if (oneLineString.first == 'Q') {
                    //request for device(s) from job specified in oneLineString
                    currentTime = (int)oneLineString[3];
                    //find job with id == (int)oneLineString[7];
                    //change the job's number of currently used devices like:
                    //jobWithID.addDevice((int)oneLineString[11]);
                    freeDevices -= oneLineString[11];
                }
                else if (oneLineString.first == 'L') {
                    //release device(s) from job specified in oneLineString
                    currentTime = (int)oneLineString[3];
                    //find job with id == (int)oneLineString[7];
                    //change the job's number of currently used devices like:
                    //jobWithID.releaseDevice((int)oneLineString[11]);
                    freeDevices += oneLineString[11];
                }
                else if (oneLineString.first == 'D') {
                    int waitFor = (int)oneLineString[3];
                    if (waitFor == 9999) {
                        //display the system turnaround and weighted turnaround time
                        cout<<""<<endl;
                    }
                    else if (currentTime == waitFor) {
                        //display system status
                        cout<<"The system currently looks like this:"<<endl;
                        //list of each job
                        cout<<""<<endl;
                        //the remaining service time for unfinished jobs
                        cout<<""<<endl;
                        //the turnaround and weighted turnaround time for each finished job
                        cout<<""<<endl;
                        //the current contents of each of the queues
                        cout<<""<<endl;
                    }
                    else {
                        cout<<"How'd we get here?"<<endl;
                        cout<<"waitFor value: ", waitFor<<endl;
                    }
                }
                else {
                    cout<<"Job category not found for: ", oneLineString<<endl;
                }
            }
        }
        else {
            cout << "Unable to open file";
        }
    }

    int checkForRequest(int endQuantam){
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
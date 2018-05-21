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
                getline(inputFile, oneLineString);
                cout<<"Single line input: "oneLineString<<endl;
                processLineOfInput(string oneLineString);
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

    void processLineOfInput(string inputLine){
        //call the functions basically
        //update the nextRequest variable so that you can do quantams
        if (inputLine.first == 'C') {
            //initiate system with the following configuration from inputLine:

            //start time
            currentTime = (int)inputLine[2];

            //main memory capacity and current free memory capacity
            maxMemory = (int)inputLine[7];
            freeMemory = (int)inputLine[7];

            //number of serial devices
            maxDevices = (int)inputLine[11];
            freeDevices = (int)inputLine[11];

            //quantum time
            quantumTime = (int)inputLine.last;

        }
        else if (inputLine.first == 'A') {
            //job intialization with these spesifications:
            //int arrivalTime, int ID, int memoryNeed, int maxDevices, int priority, int length
            Job newJob((int)inputLine[3], (int)inputLine[7], (int)inputLine[11], (int)inputLine[15], (int)inputLine[19], (int)inputLine[23]);
            processNewJob(newJob);
        }
        else if (inputLine.first == 'Q') {
            //request for device(s) from job specified in inputLine
            currentTime = (int)inputLine[3];
            //find job with id == (int)inputLine[7];
            //change the job's number of currently used devices like:
            //jobWithID.addDevice((int)inputLine[11]);
            freeDevices -= inputLine[11];
        }
        else if (intputLine.first == 'L') {
            //release device(s) from job specified in inputLine
            currentTime = (int)inputLine[3];
            //find job with id == (int)inputLine[7];
            //change the job's number of currently used devices like:
            //jobWithID.releaseDevice((int)inputLine[11]);
            freeDevices += inputLine[11];
        }
        else if (inputLine.first == 'D') {
            int waitFor = (int)inputLine[3];
            if (waitFor == 9999) {
                //display the system turnaround and weighted turnaround time
                cout<<""<<endl;
            }
            else if (currentTime == waitFor) {
                //display system status
                cout<<"The system currently looks like this:";
                //list of each job
                cout<<"";
                //the remaining service time for unfinished jobs
                cout<<"";
                //the turnaround and weighted turnaround time for each finished job
                cout<<"";
                //the current contents of each of the queues
                cout<<"";
            }
            else {
                cout << "How'd we get here?";
                cout << "waitFor value: " << waitFor;
            }
        }
        else {
            cout << "Job category not found for: " << inputLine;
        }
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
                checkQueues();
                return;
            }
        }
    }

    void completeJob(){
        Job completingJob = readyQueue.front();
        freeMemory += completingJob.getMemoryNeed();
        freeDevices += completingJob.releaseDevice(completingJob.getCurrentDevices());
        readyQueue.pop_front();
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
        Job reqJob = waitQueue.front();
        freeDevices -= reqJob.getCurrentRequest();
        reqJob.grantRequest();
        cout << "moving job "<< reqJob.getJobID() << " from wait queue to ready queue \n";
        waitQueue.pop_front();
        readyQueue.push_back(reqJob);
    }

    bool checkFIFOQueue(){
        //returns true if an element is moved from FIFO queue to ready queue, false if not
        if(FIFOQueue.empty()) return false;
        Job moveJob = FIFOQueue.begin();
        if(moveJob.getMemoryNeed() <= freeMemory){
            freeMemory -= moveJob.getMemoryNeed();
            FIFOQueue.pop_front();
            readyQueue.push_back(moveJob);
            return true;
        }
        return false;
    }

    bool checkSJFQueue(){
        //returns true if an element is moved from SJF queue to ready queue, false if not
        if(SJFQueue.empty()) return false;
        Job moveJob = SJFQueue.begin();
        if(moveJob.getMemoryNeed() <= freeMemory){
            freeMemory -= moveJob.getMemoryNeed();
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
        Job reqJob = waitQueue.front();

        if(reqJob.getCurrentRequest() > freeDevices) return false;
        else if(readyQueue.empty()){
            waitToReady();
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
                else if(jobNeed < minNeed) minNeed = jobNeed;
                else if(jobNeed > maxNeed) maxNeed = jobNeed;
                counter++;
            }

            if(jobHit[readyQueue.size()]==0){
                if(needAfterReq <= totalRetrivableDevices){
                    totalRetrivableDevices += (reqJob.getCurrentRequest() + reqJob.getCurrentDevices());
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

        }

    }
    void processNewJob(Job *newJob){
        if(*newJob.getMemoryNeed() > maxMemory) cout << "Job exceeds maximum memory capacity\n";
        else if(*newJob.getMaxDevices() > maxDevices) cout << "Job exceeds maximum device capacity\n";
        else if(*newJob.getMemoryNeed() <= freeMemory) insertReadyQueue(*newJob);
        else if(*newJob.getPriority() == 1) insertSJFQueue(*newJob);
        else insertFIFOQueue(*newJob);

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
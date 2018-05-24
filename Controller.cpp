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
    int startTime;

    void readInputFile() {
        string testOutputSting;
        ofstream inputFile;
        inputFile.open('input.txt');
        getline(inputFile, oneLineString);
        initalizeController(oneLineString);
        if (inputFile.is_open()) {
            while (!inputFile.eof) {
                getline(inputFile, oneLineString);
                cout << "Single line input: " oneLineString << endl;
                processLineOfInput(string oneLineString);
            }
        }
        else {
            cout << "Unable to open file" << endl;
        }
    }

    int checkForRequest(int endQuantam){
        //TODO
        //return an int for the number of requested devices
        //for release it could be a negative number
        //one function for update instead of release and requrest?
    }

    void initializeController(string inputLine){
        if (inputLine.first == 'C') {
            //initiate system with the following configuration from inputLine:

            //start time to calculate the turnaround time and weighted turnaround time
            startTime = (int)inputLine[2];
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


    void newJobInput(string inputLine){
        void processLineOfInput(string inputLine){
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
            processNewJob(Job(arrTime, jobNum, memReq, maxDev, runTime, prior));

        }
    }

    bool processRequest(string inputLine){
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
        else if(readyQueue.front().getJobID != jobNum){
            cout<<"Device requested while not running on the cpu (or the readyQueue is broken)"<<endl;
            exit();
        }
        else{
            readyQueue.front().requestDevice(devReq);
            return checkValidRequest();
        }

    }

    bool checkValidRequest(){
        Job reqJob = readyQueue.front();
        if(readyQueue.size()== 1){
            if(reqJob.getCurrentRequest() <= freeDevices){
                freeDevices -= reqJob.getCurrentRequest();
                reqJob.grantRequest();
                return true;
            }
        }

        if(reqJob.getCurrentRequest() > freeDevices) return false;


        int needAfterReq = reqJob.getMaxDevices() - (reqJob.getCurrentDevices() + curReq);
        int minNeed = 0; //minimum required devices of any process in the readyQueue
        int maxNeed = 0; //maximum required devices of any process in the readyQueue
        int totalRetrivableDevices = freeDevices - reqJob.getCurrentRequest();
        int jobHit[readyQueue.size()];
        while(true){
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
                    totalRetrivableDevices += (reqJob.getCurrentRequest() + reqJob.getCurrentDevices());
                    jobHit[0] = 1;
                }
                else if(needAfterReq < minNeed) minNeed = needAfterReq;
                else if(needAfterReq > maxNeed) maxNeed = needAfterReq;
            }

            if(totalRetrivableDevices < minNeed){
                return false;
            }
            else if(totalRetrivableDevices >= maxNeed){
                freeDevices -= reqJob.getCurrentRequest();
                reqJob.grantRequest();
                return true;
            }

        }

    }

    void processLineOfInput(string inputLine){
        //call the functions basically
        //update the nextRequest variable so that you can do quantams

        if (inputLine.first == 'Q') {

            //request for device(s) from job specified in inputLine
            currentTime = (int)inputLine[3];

            //get job id
            int jobID = (int)inputLine[7];

            //find job with jobID
            selectedJob = findJobWithID(jobID);

            //change the job's number of currently used devices
            selectedjob.requestDevice((int)inputLine[11]);
            freeDevices -= inputLine[11];
        }
        else if (intputLine.first == 'L') {

            //release device(s) from job specified in inputLine
            currentTime = (int)inputLine[3];

            //get job id
            int jobID = (int)inputLine[7];

            //find job with jobId
            selectedJob = findJobWithID(jobID);

            //release the job's current devices
            selectedJob.releaseDevice((int)inputLine[11]);
            freeDevices += inputLine[11];
        }
        else if (inputLine.first == 'D') {
            int waitFor = (int)inputLine[3];
            if (waitFor == 9999) {
                
                //display the system turnaround and weighted turnaround time
                Json::Value event;   
                Json::Value vec(Json::arrayValue);
                vec.append(Json::Value(1));

                event["Jobs"]["turnaround_time"]= currentTime - startTime;
                
            }
            else if (currentTime == waitFor) {
                
                outputCurrentSystem();
                
            }
            else {
                cout << "Uhh ohh how'd we get here?\n";
                cout << "waitFor value: " << waitFor << endl;
            }
        }
        else {
            cout << "Job category not found for: " << inputLine << endl;
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
            }
            else{
                readyQueue.pop_front();
                readyQueue.push_back(currentJob);
            }
        }
        else{

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
    
    void outputCurrentSystem () {

        Json::Value event;   
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
        event["system"]["readyq"]["quques"]= readyQueue.length();
        event["system"]["submitq"]["quques"]= submitQueue.length();
        event["system"]["holdq_2"]["quques"]= SJFQueue.length();
        event["system"]["holdq_1"]["quques"]= FIFOQueue.length();
        event["system"]["completeq"["quques"]]= completeQueue.length();

        //the remaining service time for unfinished jobs
        for (it = waitQueue.begin(); it != waitQueue.end(); ++it) {
            event["system"]["jobs"]["arrival_time"]= waitQueue.arrivalTime;
            event["system"]["jobs"]["devices_allocated"]= waitQueue.currentDevices;
            event["system"]["jobs"]["id"]= waitQueue.id;
            event["system"]["jobs"]["remaining_time"]= waitQueue.length + waitQueue.arrivalTime - waitQueue.currentTime;
        }

        //the turnaround and weighted turnaround time for each finished job
        for (it = readydQueue.begin(); it != readyQueue.end(); ++it) {
            event["system"]["jobs"]["arrival_time"]= readyQueue.arrivalTime;
            event["system"]["jobs"]["devices_allocated"]= readyQueue.currentDevices;
            event["system"]["jobs"]["id"]= readyQueue.id;
            event["system"]["jobs"]["remaining_time"]= readyQueue.length + readyQueue.arrivalTime - readyQueue.currentTime;
        }

        //the current contents of each of the queues
        for (it = SJFQueue.begin(); it != SJFQueue.end(); ++it) {
            event["system"]["jobs"]["arrival_time"]= SJFQueue.arrivalTime;
            event["system"]["jobs"]["devices_allocated"]= SJFQueue.currentDevices;
            event["system"]["jobs"]["id"]= SJFQueue.id;
            event["system"]["jobs"]["remaining_time"]= SJFQueue.length + SJFQueue.arrivalTime - SJFQueue.currentTime;
        }

        for (it = FIFOQueue.begin(); it != FIFOQueue.end(); ++it) {
            event["system"]["jobs"]["arrival_time"]= FIFOQueue.arrivalTime;
            event["system"]["jobs"]["devices_allocated"]= FIFOQueue.currentDevices;
            event["system"]["jobs"]["id"]= FIFOQueue.id;
            event["system"]["jobs"]["remaining_time"]= FIFOQueue.length + FIFOQueue.arrivalTime - FIFOQueue.currentTime;
        }

        for (it = readyQueue.begin(); it != readyQueue.end(); ++it) {
            event["system"]["jobs"]["arrival_time"]= readyQueue.arrivalTime;
            event["system"]["jobs"]["devices_allocated"]= readyQueue.currentDevices;
            event["system"]["jobs"]["id"]= readyQueue.id;
            event["system"]["jobs"]["remaining_time"]= readyQueue.length + readyQueue.arrivalTime - readuQueue.currentTime;
        }
        
        for (it = waitQueue.begin(); it != waitQueue.end(); ++it) {
            event["system"]["jobs"]["arrival_time"]= waitQueue.arrivalTime;
            event["system"]["jobs"]["devices_allocated"]= waitQueue.currentDevices;
            event["system"]["jobs"]["id"]= waitQueue.id;
            event["system"]["jobs"]["remaining_time"]= waitQueue.length + waitQueue.arrivalTime - waitQueue.currentTime;
        }

        for (it = completedQueue.begin(); it != completedQueue.end(); ++it) {
            event["system"]["jobs"]["arrival_time"]= completedQueue.arrivalTime;
            event["system"]["jobs"]["completion_time"]= completedQueue.completionTime;
            event["system"]["jobs"]["id"]= completedQueue.id;
            event["system"]["jobs"]["remaining_time"]= completedQueue.length + completedQueue.arrivalTime - completedQueue.currentTime;
        }

        std::cout << event << std::endl;

    }
}

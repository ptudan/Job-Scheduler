//Paul Tudan and Jefferson Kappers
//5-11-18

#include <Job.h>
#include <list>
#include <string>
using namespace std;


class Controller {
private:
    list<Job> SJFHoldQueue;
    list<Job> FIFOHoldQueue;
    list<Job> readyQueue;
    list<Job> waitQueue;
    int currentTime;
    int maxMemory;
    int freeMemory;
    int numberOfSerialDevices;
    int quantumTime;

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

    void processNewJob(Job newJob){
        if(newJob.getMemoryNeed() < freeMemory) insertReadyQueue(newJob);

    }

    void insertReadyQueue(Job newJob){
        if(newJob.getMemoryNeed() > freeMemory) cout << "not enough mem to insert job into RQ";
        else freeMemory - newJob.getMemoryNeed();

        


    }




}
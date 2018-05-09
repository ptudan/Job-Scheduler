//Paul Tudan and Jefferson Kappers
//CISC361 Project
//5-9-18

#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char **argv) {
    string testOutputSting
    ofstream inputFile;
    inputFile.open('input.txt');
    if (inputFile.is_open()) {
        while (!inputFile.eof) {
            getline(inputFile,oneLineString);
            cout<<oneLineString;
            for (i = 0; i <= oneLineString.size(); i++) {

            }
        }
    }
    else {
        cout << "Unable to open file";
    }
    return 0
}

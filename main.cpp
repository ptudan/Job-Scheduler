#include "Job.h"
#include "Controller.h"
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

int main(){
	Controller c("input.txt");
	c.start();
	cout<<"did I really get here"<<endl;
}
/* 
 * File:   IOTest.cpp
 * Author: thomass
 * 
 * Created on November 5, 2010, 1:42 PM
 */

#include <iostream>
#include <fstream>
#include <ostream>
#include <stdlib.h>

using namespace std;

class IOGefaddel {

public:
	void readFromFile(string path);
	void printFileContents();
	void printFileReverse();
	void writeFile();

private:
	string buffer;
};


void IOGefaddel::readFromFile(string path) {
	ifstream inFile;
	inFile.open(path.c_str());
	if (inFile.fail()) {
		cerr << "unable to open file " << path << " for reading." << endl;
		exit(-1);
	}
	string line;
	while (getline(inFile, line)) {
		buffer += line + "\n";
	}
}

void IOGefaddel::printFileReverse() {

}

void IOGefaddel::printFileContents() {
	cout << buffer;
}

void IOGefaddel::writeFile() {
}

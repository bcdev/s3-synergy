/* 
 * File:   IOTest.h
 * Author: thomass
 *
 * Created on November 5, 2010, 1:42 PM
 */

#ifndef PLAYWITHIO_H
#define	PLAYWITHIO_H

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

#endif	/* PLAYWITHIO_H */


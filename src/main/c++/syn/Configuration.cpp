/*
 * Configuration.cpp
 *
 *  Created on: Nov 9, 2010
 *      Author: thomass
 */

#include "Configuration.h"
#include <iostream>

using namespace std;

/**
 * General namespace for ESAs software.
 */
namespace esa {
/**
 * Namespace for S3L2PP software.
 */
namespace S3L2PP {
/**
 * Namespace for the SYN processing branch.
 */
namespace syn {

string Configuration::getId() const {
	return id;
}

bool Configuration::getExportSyn() const {
	return exportSyn;
}

bool Configuration::getExportVgt() const {
	return exportVgt;
}

string Configuration::getInputFileLocation() const {
	return inputFileLocation;
}

string Configuration::getSynFileLocation() const {
	return synFileLocation;
}

string Configuration::getVgtFileLocation() const {
	return vgtFileLocation;
}

void Configuration::setId(string id) {
	this->id = id;
}

void Configuration::setExportSyn(bool exportSyn) {
	this->exportSyn = exportSyn;
}

void Configuration::setExportVgt(bool exportVgt) {
	this->exportVgt = exportVgt;
}

void Configuration::setInputFileLocation(string inputFileLocation) {
	this->inputFileLocation = inputFileLocation;
}

void Configuration::setSynFileLocation(string synFileLocation) {
	this->synFileLocation = synFileLocation;
}

void Configuration::setVgtFileLocation(string vgtFileLocation) {
	this->vgtFileLocation = vgtFileLocation;
}

void Configuration::print() {

	cout << "id = " << getId() << "\n";
	cout << "debugLevel = " << getDebugLevel() << "\n";
	cout << "exportSyn = " << boolToString(getExportSyn()) << "\n";
	cout << "exportVgt = " << boolToString(getExportVgt()) << "\n";
	cout << "inputFileLocation = " << getInputFileLocation() << "\n";
	cout << "synFileLocation = " << getSynFileLocation() << "\n";
	cout << "vgtFileLocation = " << getVgtFileLocation() << endl;
}

string Configuration::getDebugLevel() const
{
    return debugLevel;
}

void Configuration::setDebugLevel( string level )
{
    this->debugLevel = level;
}

string Configuration::boolToString( bool input ) {
	string output;
	if( input ) {
		return "true";
	} else {
		return "false";
	}
}

}
}
}

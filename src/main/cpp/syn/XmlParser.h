/*
 * XmlParser.h
 *
 *  Created on: Nov 8, 2010
 *      Author: thomass
 */

#ifndef XMLPARSER_H_
#define XMLPARSER_H_

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/util/XMLException.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include "xercesc/dom/DOMXPathResult.hpp"
#include <list>
#include "Configuration.h"

using namespace std;
using namespace xercesc_3_1;

class XmlParser {

private:
	string path;
	XercesDOMParser * parser;
	DOMDocument * doc;
	list<string> getNodeNames( DOMElement * root );
	void parseId( Configuration * config, DOMElement * node );
    void parseInputLocation(DOMElement * childNode, Configuration * config);
    void parseDebugLevel( Configuration * config, DOMElement * node );

protected:
	DOMElement * root;

public:
	XmlParser(string path);
	void readXml();
	void outputNodes();
	void cleanUp();
	string getTextContent( DOMNode * node );
	string getNodeName( DOMElement * node );
	string getNodeAttribute(DOMElement * node, string attributeName);
	void* getNodeForExpression( string xPath );
};

#endif /* XMLPARSER_H_ */

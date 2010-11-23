/*
 * XmlParser.h
 *
 *  Created on: Nov 8, 2010
 *      Author: thomass
 */

#ifndef XMLPARSER_H_
#define XMLPARSER_H_

#include <list>

#include "Configuration.h"
#include "XPathInitializer.h"

using std::string;
using std::list;

class XmlParser {
private:
    string path;
    list<string> getNodeNames(DOMElement * root);
    void parseId(Configuration * config, DOMElement * node);
    void parseInputLocation(DOMElement * childNode, Configuration * config);
    void parseDebugLevel(Configuration * config, DOMElement * node);
    void parse();

protected:
    XercesDOMParser* parser;
    DOMDocument* doc;
    DOMElement* root;

public:
    XmlParser(string path);
    ~XmlParser();
    void readXml();
    void outputNodes();
    void cleanUp();
    string getTextContent(DOMNode* node);
    string getNodeName(DOMElement* node);
    string getNodeAttribute(DOMElement* node, string attributeName);
    const string evaluateToString(string& expression);
    const vector<string> evaluateToStringList(string& expression);
};

#endif /* XMLPARSER_H_ */

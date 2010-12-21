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
    list<string> getNodeNames(DOMElement * root);
    void parse(string& path);

protected:
    XmlParser();
    XercesDOMParser* parser;
    DOMDocument* doc;
    DOMElement* root;

public:
    ~XmlParser();
    void readXml();
    void outputNodes();
    void cleanUp();
    string getTextContent(DOMNode* node);
    string getNodeName(DOMElement* node);
    string getNodeAttribute(DOMElement* node, string attributeName);
    const string evaluateToString(string& path, string& expression);
    const vector<string> evaluateToStringList(string& path, string& expression);
};

#endif /* XMLPARSER_H_ */

/*
 * XmlParser.h
 *
 *  Created on: Nov 8, 2010
 *      Author: thomass
 */

#ifndef XMLPARSER_H_
#define XMLPARSER_H_

#include <list>
#include <vector>

#include "XPathInitializer.h"

using std::list;
using std::string;
using std::vector;

class XmlParser {
public:
    XmlParser();
    ~XmlParser();

    void readXml();
    void outputNodes();
    void cleanUp();
    string getTextContent(DOMNode* node);
    string getNodeName(DOMElement* node);
    string getNodeAttribute(DOMElement* node, string attributeName);
    const string evaluateToString(const string& path, const char* expression) const;
    const string evaluateToString(const string& path, const string& expression) const;
    const vector<string> evaluateToStringList(const string& path, const string& expression) const;
    const vector<string> evaluateToStringList(const string& path, const char* expression) const;
    
protected:
    XercesDOMParser* parser;
    DOMDocument* doc;
    DOMElement* root;

private:
    list<string> getNodeNames(DOMElement * root);
    void parse(const string& path);
};

#endif /* XMLPARSER_H_ */

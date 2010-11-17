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
    XercesDOMParser* parser;
    DOMDocument* doc;
    list<string> getNodeNames(DOMElement * root);
    void parseId(Configuration * config, DOMElement * node);
    void parseInputLocation(DOMElement * childNode, Configuration * config);
    void parseDebugLevel(Configuration * config, DOMElement * node);

protected:
    DOMElement* root;

public:
    XmlParser(string path);
    void readXml();
    void readXml2(XPath& path);
    void outputNodes();
    void cleanUp();
    string getTextContent(DOMNode* node);
    string getNodeName(DOMElement* node);
    string getNodeAttribute(DOMElement* node, string attributeName);
    DOMXPathResult* evaluateXPathQuery(string expression);
};

#endif /* XMLPARSER_H_ */

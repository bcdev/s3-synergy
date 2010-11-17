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
#include <xalanc/XPath/XPathEvaluator.hpp>
#include <list>
#include "Configuration.h"

using std::string;

using xalanc::XPath;

using xercesc::DOMDocument;
using xercesc::DOMElement;
using xercesc::DOMNode;
using xercesc::DOMXPathResult;
using xercesc::XercesDOMParser;

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

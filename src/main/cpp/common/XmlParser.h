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

using namespace xercesc;

class XmlParser {
private:
    std::string path;
    XercesDOMParser * parser;
    DOMDocument * doc;
    list<std::string> getNodeNames(DOMElement * root);
    void parseId(Configuration * config, DOMElement * node);
    void parseInputLocation(DOMElement * childNode, Configuration * config);
    void parseDebugLevel(Configuration * config, DOMElement * node);

protected:
    DOMElement * root;

public:
    XmlParser(std::string path);
    void readXml();
    void readXml2( xalanc::XPath& path);
    void outputNodes();
    void cleanUp();
    std::string getTextContent(DOMNode * node);
    std::string getNodeName(DOMElement * node);
    std::string getNodeAttribute(DOMElement * node, std::string attributeName);
    DOMXPathResult* evaluateXPathQuery(std::string expression);
};

#endif /* XMLPARSER_H_ */

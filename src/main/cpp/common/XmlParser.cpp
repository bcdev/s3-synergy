/*
 * XmlParser.cpp
 *
 *  Created on: Nov 8, 2010
 *      Author: thomass
 */

#include <iostream>
#include <typeinfo>
#include "XmlParser.h"
#include <xercesc/dom/DOMXPathException.hpp>
#include <xercesc/dom/DOMXPathExpression.hpp>
#include <cstdlib>

using namespace xercesc_3_1;

XmlParser::XmlParser(std::string path) {
    this->path = path;
}

void XmlParser::readXml() {
    parser = new XercesDOMParser();
    parser->setValidationScheme(XercesDOMParser::Val_Always);
    parser->setCreateEntityReferenceNodes(false);
    parser->setIncludeIgnorableWhitespace(false);

    try {
        parser->parse(path.c_str());
        doc = parser->getDocument();
        if (doc == 0) {
            // todo replace with logging
            std::cerr << "\nPath " << path << " does not point to a valid xml file. System will exit.";
            std::exit(2);
        }
        root = doc->getDocumentElement();
    } catch (const XMLException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        // todo replace with logging
        cout << "Exception message is: \n" << message << "\n";
        XMLString::release(&message);
    }
}

DOMXPathResult* XmlParser::evaluateXPathQuery(std::string expression) {
    try {
        DOMXPathExpression* xPathExpression = doc->createExpression(XMLString::transcode(expression.c_str()), NULL);
        DOMXPathResult* result;
        xPathExpression->evaluate(root, DOMXPathResult::ANY_TYPE, result);
        return result;
    } catch (DOMXPathException toCatch) {
        // todo replace with logging
        cerr << "\n" << XMLString::transcode(toCatch.getMessage()) << "\n";
    }
    return 0;
}

std::string XmlParser::getNodeName(DOMElement * node) {
    const XMLCh * tagName = node->getTagName();
    return XMLString::transcode(tagName);
}

std::string XmlParser::getTextContent(DOMNode * node) {
    const XMLCh * content = node->getTextContent();
    return XMLString::transcode(content);
}

std::string XmlParser::getNodeAttribute(DOMElement * node, std::string attributeName) {
    const XMLCh * attributeXmlName = XMLString::transcode(attributeName.c_str());
    const XMLCh * attributeXmlValue = node->getAttribute(attributeXmlName);
    return XMLString::transcode(attributeXmlValue);
}

void XmlParser::outputNodes() {
    list<std::string> names = getNodeNames(root);
    for (list<std::string>::const_iterator it = names.begin(); it != names.end(); ++it) {
        cout << *it << "\n";
    }
}

list<std::string> XmlParser::getNodeNames(DOMElement * root) {
    list<std::string> names;
    names.push_front(getNodeName(root));
    DOMNodeList * childNodes = root->getChildNodes();
    if (childNodes != NULL) {
        for (XMLSize_t i = 0; i < childNodes->getLength(); i++) {
            DOMNode * item = childNodes->item(i);
            // important check! Without check, program runs but treats every node as element,
            // in spite of the cast
            if (item->getNodeType() && item->getNodeType() == DOMNode::ELEMENT_NODE) {
                list<std::string> names_rec = getNodeNames((DOMElement*) item);
                for (list<std::string>::const_iterator iter = names_rec.begin(); iter
                        != names_rec.end(); ++iter) {
                    names.push_front(*iter);
                }
            }
        }
    }
    return names;
}

/**
 * todo: put into destructor
 */
void XmlParser::cleanUp() {
    delete parser;
}

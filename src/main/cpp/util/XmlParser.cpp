/*
 * XmlParser.cpp
 *
 *  Created on: Nov 8, 2010
 *      Author: thomass
 */

#include <cstddef>
#include <exception>
#include <iostream>
#include <valarray>

#include "XmlParser.h"

using std::cout;
using std::exception;
using std::list;
using std::string;

XmlParser::XmlParser() {

}

XmlParser::~XmlParser() {
}

const string XmlParser::evaluateToString(string& path, string& expression) {
    return evaluateToString(path, expression.c_str());
}

const string XmlParser::evaluateToString(string& path, const char* expression) {
    parse(path);
    // Create a XalanDocument based on doc.
    XercesDOMSupport support;
    XercesParserLiaison liaison(support);
    XercesDOMWrapperParsedSource src(doc, liaison, support);
    XalanDocument* xalanDoc = src.getDocument();

    // Evaluate an XPath expression to obtain the text node and its content
    XPathEvaluator evaluator;
    XalanDOMChar* expr = XMLString::transcode(expression);
    XObjectPtr result = evaluator.evaluate(support, xalanDoc, expr);

    // cleaning up
    XMLString::release(&expr);

    const xalanc::XalanDOMString& resultString = result->str();
    return XMLString::transcode(resultString.data());
}

const vector<string> XmlParser::evaluateToStringList(string& path, string& expression) {
    return evaluateToStringList(path, expression.c_str());
}

const vector<string> XmlParser::evaluateToStringList(string& path, const char* expression) {
    parse(path);
    // Create a XalanDocument based on doc.
    XercesDOMSupport support;
    XercesParserLiaison liaison(support);
    XercesDOMWrapperParsedSource src(doc, liaison, support);
    XalanDocument* xalanDoc = src.getDocument();

    // Evaluate an XPath expression to obtain the text node and its content
    XPathEvaluator evaluator;
    XalanDOMChar* expr = XMLString::transcode(expression);
    XObjectPtr result = evaluator.evaluate(support, xalanDoc, expr);

    // cleaning up
    XMLString::release(&expr);

    const NodeRefListBase& nodeSet = result->nodeset();

    vector<string> output;
    for (size_t i = 0; i < nodeSet.getLength(); i++) {
        const XMLCh* name = nodeSet.item(i)->getNodeValue().data();
        char* value = XMLString::transcode(name);
        output.push_back(value);
    }
    return output;
}

void XmlParser::parse(string& path) {
    try {
        // construct a DOM parser
        parser = new XercesDOMParser();
        parser->setValidationScheme(XercesDOMParser::Val_Always);
        parser->setCreateEntityReferenceNodes(false);
        parser->setIncludeIgnorableWhitespace(false);

        // todo - register error handler
        // parser.setErrorHandler(&errorHandler);

        // Parse xml
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
    } catch (const DOMException& e) {
        cout << "xml error: " << XMLString::transcode(e.getMessage()) << "\n";
        std::exit(2);
    } catch (const std::exception& e) {
        cout << e.what() << "\n";
        std::exit(2);
    }

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
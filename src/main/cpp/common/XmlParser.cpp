/*
 * XmlParser.cpp
 *
 *  Created on: Nov 8, 2010
 *      Author: thomass
 */

#include <cstddef>
#include <exception>
#include <iostream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xalanc/DOMSupport/XalanDocumentPrefixResolver.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>
#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>
#include <xalanc/XPath/XObject.hpp>
#include <valarray>
#include "XPathInitializer.h"

#include "XmlParser.h"

using xercesc::DOMException;
using xercesc::DOMNodeList;
using xercesc::DOMXPathException;
using xercesc::DOMXPathExpression;
using xercesc::XMLException;
using xercesc::XMLString;

using xalanc::NodeRefListBase;
using xalanc::XalanDocument;
using xalanc::XalanDocumentPrefixResolver;
using xalanc::XercesDOMSupport;
using xalanc::XercesDOMWrapperParsedSource;
using xalanc::XercesParserLiaison;
using xalanc::XPathEvaluator;
using xalanc::XObjectPtr;

XmlParser::XmlParser(string path) {
    this->path = path;
}

void XmlParser::readXml2(XPath& expression) {
    try {
        // Initialize Xerces and XPath and construct a DOM parser.
        XPathInitializer init;

        parser->setValidationScheme(XercesDOMParser::Val_Always);
        parser->setCreateEntityReferenceNodes(false);
        parser->setIncludeIgnorableWhitespace(false);

        // todo - register error handler
        // parser.setErrorHandler(&errorHandler);

        // Parse animals.xml.
        parser->parse(path.c_str());
        DOMDocument* doc = parser->getDocument();
        root = doc->getDocumentElement();

        // Create a XalanDocument based on doc.
        XercesDOMSupport support;
        XercesParserLiaison liaison(support);
        XercesDOMWrapperParsedSource src(doc, liaison, support);
        XalanDocument* xalanDoc = src.getDocument();

        // Evaluate an XPath expression to obtain a list
        // of text nodes containing animals' names
        XPathEvaluator evaluator;
        XalanDocumentPrefixResolver resolver(xalanDoc);
        XObjectPtr result =
                evaluator.evaluate(
                support, // DOMSupport
                xalanDoc, // context node
                expression, // XPath expr
                resolver); // Namespace resolver
        const NodeRefListBase& nodeset = result->nodeset();


        // Iterate through the node list, printing the animals' names
        for (size_t i = 0,
                len = nodeset.getLength();
                i < len;
                ++i) {
            const XMLCh* name =
                    nodeset.item(i)->getNodeValue().c_str();
            // std::cout << toNative(name) << "\n";
        }
    } catch (const DOMException& e) {
        // cout << "xml error: " << toNative(e.getMessage()) << "\n";
        // return EXIT_FAILURE;
    } catch (const exception& e) {
        // cout << e.what() << "\n";
        // return EXIT_FAILURE;
    }
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
        const DOMXPathExpression* xPathExpression = doc->createExpression(XMLString::transcode(expression.c_str()), NULL);
        DOMXPathResult* result;
        xPathExpression->evaluate(root, DOMXPathResult::ANY_TYPE, result);
        return result;
    } catch (DOMXPathException toCatch) {
        // todo replace with logging
        //cerr << "\n" << XMLString::transcode(toCatch.getMessage()) << "\n";
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

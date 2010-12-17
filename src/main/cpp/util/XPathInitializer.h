/* 
 * File:   XPathInitializer.h
 * Author: ralf
 *
 * Created on November 16, 2010, 4:30 PM
 */

#ifndef XPATHINITIALIZER_H
#define	XPATHINITIALIZER_H

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMXPathException.hpp>
#include <xercesc/dom/DOMXPathExpression.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMXPathResult.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLException.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>

#include <xalanc/XPath/XPath.hpp>
#include <xalanc/XPath/XPathEvaluator.hpp>
#include <xalanc/DOMSupport/XalanDocumentPrefixResolver.hpp>
#include <xalanc/XalanTransformer/XercesDOMWrapperParsedSource.hpp>
#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>
#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XPath/XPathFactory.hpp>

using xercesc::DOMDocument;
using xercesc::DOMElement;
using xercesc::DOMException;
using xercesc::DOMNode;
using xercesc::DOMNodeList;
using xercesc::DOMXPathException;
using xercesc::DOMXPathExpression;
using xercesc::DOMXPathResult;
using xercesc::XercesDOMParser;
using xercesc::XMLException;
using xercesc::XMLPlatformUtils;
using xercesc::XMLString;

using xalanc::NodeRefListBase;
using xalanc::XalanDocument;
using xalanc::XalanDocumentPrefixResolver;
using xalanc::XercesDOMSupport;
using xalanc::XercesDOMWrapperParsedSource;
using xalanc::XercesParserLiaison;
using xalanc::XObjectPtr;
using xalanc::XObject;
using xalanc::XPath;
using xalanc::XPathEvaluator;
using xalanc::XPathFactory;
using xalanc::XalanDOMChar;

class XPathInitializer {
public:
    XPathInitializer();
    virtual ~XPathInitializer();
private:

};

#endif	/* XPATHINITIALIZER_H */


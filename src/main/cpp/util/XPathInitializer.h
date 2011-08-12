/* 
 * File:   XPathInitializer.h
 * Author: ralf
 *
 * Created on November 16, 2010, 4:30 PM
 */

#ifndef XPATHINITIALIZER_H
#define	XPATHINITIALIZER_H

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLException.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>

#include <xqilla/xqilla-dom3.hpp>

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

class XPathInitializer {
public:
	XPathInitializer() {
	    XQillaPlatformUtils::initialize();
	}

	~XPathInitializer() {
		XQillaPlatformUtils::terminate();
	}
};

#endif	/* XPATHINITIALIZER_H */


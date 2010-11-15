/*
 * XmlParser.cpp
 *
 *  Created on: Nov 8, 2010
 *      Author: thomass
 */

#include <iostream>
#include <typeinfo>
#include "XmlParser.h"
#include "xercesc/dom/DOMXPathException.hpp"

using namespace std;
using namespace xercesc_3_1;

XmlParser::XmlParser(string path) {
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
		root = doc->getDocumentElement();
	} catch (const XMLException& toCatch) {
		char* message = XMLString::transcode(toCatch.getMessage());
		cout << "Exception message is: \n" << message << "\n";
		XMLString::release(&message);
	}
}

string XmlParser::getNodeName(DOMElement * node) {
	const XMLCh * tagName = node->getTagName();
	return XMLString::transcode(tagName);
}

string XmlParser::getTextContent(DOMNode * node) {
	const XMLCh * content = node->getTextContent();
	return XMLString::transcode(content);
}

string XmlParser::getNodeAttribute(DOMElement * node, string attributeName) {
	const XMLCh * attributeXmlName = XMLString::transcode(attributeName.c_str());
	const XMLCh * attributeXmlValue = node->getAttribute( attributeXmlName );
	return XMLString::transcode( attributeXmlValue );
}

void* XmlParser::getNodeForExpression( string xPath ) {
//	XMLCh * xmlXPath = XMLString::transcode( xPath.c_str() );
//	try{
//		void* element = doc->evaluate( xmlXPath, root, NULL, DOMXPathResult::ANY_TYPE, NULL );
//		cout << typeid(element).name();
//	} catch (const DOMXPathException& toCatch) {
//		char* message = XMLString::transcode(toCatch.getMessage());
//		cerr << "failed because " << message << "\n";
//	}
	return NULL;
}

void XmlParser::outputNodes() {
	list<string> names = getNodeNames(root);
	for (list<string>::const_iterator it = names.begin(); it != names.end(); ++it) {
		cout << *it << "\n";
	}
}

list<string> XmlParser::getNodeNames(DOMElement * root) {
	list<string> names;
	names.push_front(getNodeName(root));
	DOMNodeList * childNodes = root->getChildNodes();
	if (childNodes != NULL) {
		for (XMLSize_t i = 0; i < childNodes->getLength(); i++) {
			DOMNode * item = childNodes->item(i);
			// important check! Without check, program runs but treats every node as element,
			// in spite of the cast
			if (item->getNodeType() && item->getNodeType() == DOMNode::ELEMENT_NODE) {
				list<string> names_rec = getNodeNames((DOMElement*) item);
				for (list<string>::const_iterator iter = names_rec.begin(); iter
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

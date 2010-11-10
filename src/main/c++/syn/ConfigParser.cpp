/*
 * ConfigParser.cpp
 *
 *  Created on: Nov 10, 2010
 *      Author: thomass
 */

#include "ConfigParser.h"

ConfigParser::ConfigParser( string path ) : XmlParser( path ) {}

ConfigParser::~ConfigParser() {
	// TODO Auto-generated destructor stub
}

Configuration ConfigParser::parseConfiguration() {
	Configuration * config = new Configuration();
	config->setId(getNodeAttribute( root, "id" ));
	DOMNodeList * childNodes = root->getChildNodes();
	for (XMLSize_t i = 0; i < childNodes->getLength(); i++) {
		DOMElement * node = (DOMElement*)childNodes->item(i);
		string nodeName = getNodeName(node);
		if (nodeName.compare("fileLocation") == 0) {
			config->setInputFileLocation( getTextContent( node ) );
		} else if (nodeName.compare("exportSyn") == 0) {
			parseSubConfigs(config, node, true);
		} else if (nodeName.compare("exportVgt") == 0) {
			parseSubConfigs(config, node, false);
		} else if (nodeName.compare("debugLevel") == 0) {
			config->setDebugLevel( getTextContent( node ) );
		}
	}
	return *config;
}

void ConfigParser::parseSubConfigs(Configuration * config, DOMElement * node, bool isSyn) {
	DOMNodeList * childNodes = node->getChildNodes();
	for (XMLSize_t j = 0; j < childNodes->getLength(); ++j) {
		DOMNode * item = childNodes->item(j);
		if (item->getNodeType() && item->getNodeType() == DOMNode::ELEMENT_NODE) {
			DOMNode * content = item->getFirstChild();
			if (getNodeName((DOMElement*)item).compare("performExport") == 0) {
				if (isSyn) {
					config->setExportSyn(getTextContent(content).compare( "true") == 0);
				} else {
					config->setExportVgt(getTextContent(content).compare( "true") == 0);
				}
			} else if (getNodeName((DOMElement*)item).compare( "outputFileLocation") == 0) {
				if (isSyn) {
					config->setSynFileLocation(getTextContent(content));
				} else {
					config->setVgtFileLocation(getTextContent(content));
				}
			}
		}
	}
}

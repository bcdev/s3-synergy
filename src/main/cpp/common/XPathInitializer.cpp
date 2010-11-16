/* 
 * File:   XPathInitializer.cpp
 * Author: ralf
 * 
 * Created on November 16, 2010, 4:30 PM
 */

#include "XPathInitializer.h"

XPathInitializer::XPathInitializer() {
    xercesc::XMLPlatformUtils::Initialize();
    xalanc::XPathEvaluator::initialize();
}

XPathInitializer::~XPathInitializer() {
    xercesc::XMLPlatformUtils::Terminate();
    xalanc::XPathEvaluator::terminate();
}


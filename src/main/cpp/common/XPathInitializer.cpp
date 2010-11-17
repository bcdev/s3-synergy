/* 
 * File:   XPathInitializer.cpp
 * Author: ralf
 * 
 * Created on November 16, 2010, 4:30 PM
 */

#include "XPathInitializer.h"

XPathInitializer::XPathInitializer() {
    XMLPlatformUtils::Initialize();
    XPathEvaluator::initialize();
}

XPathInitializer::~XPathInitializer() {
    XMLPlatformUtils::Terminate();
    XPathEvaluator::terminate();
}


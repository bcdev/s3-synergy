/* 
 * File:   XPathInitializer.h
 * Author: ralf
 *
 * Created on November 16, 2010, 4:30 PM
 */

#ifndef XPATHINITIALIZER_H
#define	XPATHINITIALIZER_H

#include <xqilla/xqilla-dom3.hpp>

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


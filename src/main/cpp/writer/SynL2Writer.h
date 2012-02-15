/*
 * File:   SynL2Writer.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:19 PM
 */

#ifndef SYNL2WRITER_H
#define	SYNL2WRITER_H

#include "../writer/AbstractWriter.h"

class SynL2Writer: public AbstractWriter {
public:
    SynL2Writer();
    ~SynL2Writer();

private:
	friend class SynL2WriterTest;
};

#endif	/* SYNL2WRITER_H */


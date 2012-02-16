/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */

#ifndef SYNL2WRITER_H
#define	SYNL2WRITER_H

#include "../writer/AbstractWriter.h"

/**
 * The SYN L2 writer module.
 */
class SynL2Writer: public AbstractWriter {
public:
    SynL2Writer();
    ~SynL2Writer();

private:
	friend class SynL2WriterTest;
};

#endif	/* SYNL2WRITER_H */


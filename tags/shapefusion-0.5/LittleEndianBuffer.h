/*
 * This file is part of ShapeFusion (Copyright 2000 Tito Dal Canton)
 *
 * ShapeFusion is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ShapeFusion is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ShapeFusion; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
 
//
// LittleEndianBuffer
// Utility class to access little-endian data buffers easily
//

#ifndef LITTLEENDIANBUFFER_H
#define LITTLEENDIANBUFFER_H

#include "GenericEndianBuffer.h"

class LittleEndianBuffer: public GenericEndianBuffer {
public:
	// self-allocate data buffer
	LittleEndianBuffer(unsigned int _size);
	// use a pre-allocated buffer
	LittleEndianBuffer(unsigned char *_data, unsigned int _size);
	~LittleEndianBuffer(void);
	// read little-endian values, advance position accordingly
	short ReadShort(void);
	unsigned short ReadUShort(void);
	long ReadLong(void);
	unsigned long ReadULong(void);
	// write little-endian values, advance position accordingly
	void WriteShort(short v);
	void WriteUShort(unsigned short v);
	void WriteLong(long v);
	void WriteULong(unsigned long v);
};

#endif

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

class LittleEndianBuffer {
private:
	unsigned char	*data,
					*position;
	unsigned int	size;
	bool			self_allocated;

public:
	// self-allocate data buffer
	LittleEndianBuffer(unsigned int _size);
	// use a pre-allocated buffer
	LittleEndianBuffer(unsigned char *_data, unsigned int _size);
	~LittleEndianBuffer(void);
	// set access position
	void Position(unsigned int pos);
	// return access position
	unsigned int Position(void) const;
	// read little-endian values, advance position accordingly
	char ReadChar(void);
	unsigned char ReadUChar(void);
	short ReadShort(void);
	unsigned short ReadUShort(void);
	long ReadLong(void);
	unsigned long ReadULong(void);
	void ReadBlock(unsigned long _size, unsigned char *dest);
	// write little-endian values, advance position accordingly
	void WriteChar(char v);
	void WriteUChar(unsigned char v);
	void WriteShort(short v);
	void WriteUShort(unsigned short v);
	void WriteLong(long v);
	void WriteULong(unsigned long v);
	void WriteBlock(unsigned long _size, const void *src);
	void WriteZeroes(unsigned int n);
	// stuff access
	unsigned char *Data(void) const;
	unsigned int Size(void) const;
};

#endif

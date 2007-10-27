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
#include <iostream>
#include "BigEndianBuffer.h"

BigEndianBuffer::BigEndianBuffer(unsigned int _size):
		size(0), self_allocated(true)
{
	data = new unsigned char[_size];
	if (data != NULL) {
		position = data;
		size = _size;
	}
}

BigEndianBuffer::BigEndianBuffer(unsigned char *_data, unsigned int _size):
		data(_data), position(_data), size(_size), self_allocated(false)
{
	
}

BigEndianBuffer::~BigEndianBuffer(void)
{
	if (self_allocated && data != NULL) {
		delete[] data;
		data = NULL;
	}
}

char BigEndianBuffer::ReadChar(void)
{
	if ((unsigned int)(position - data) < size) {
		unsigned char	v = *position;

		position++;
		return (char)v;
	} else {
		std::cerr << "BigEndianBuffer: attempted read beyond buffer limits\n";
		return 0;
	}
}

unsigned char BigEndianBuffer::ReadUChar(void)
{
	if ((unsigned int)(position - data) < size) {
		unsigned char	v = *position;

		position++;
		return v;
	} else {
		std::cerr << "BigEndianBuffer: attempted read beyond buffer limits\n";
		return 0;
	}
}

short BigEndianBuffer::ReadShort(void)
{
	if ((unsigned int)(position - data + 1) < size) {
		unsigned char	hi = *position++,
						lo = *position++;

		return (short)((hi << 8) | lo);
	} else {
		std::cerr << "BigEndianBuffer: attempted read beyond buffer limits\n";
		return 0;
	}
}

unsigned short BigEndianBuffer::ReadUShort(void)
{
	if ((unsigned int)(position - data + 1) < size) {
		unsigned char	hi = *position++,
						lo = *position++;
		
		return (unsigned short)((hi << 8) | lo);
	} else {
		std::cerr << "BigEndianBuffer: attempted read beyond buffer limits\n";
		return 0;
	}
}

long BigEndianBuffer::ReadLong(void)
{
	if ((unsigned int)(position - data + 3) < size) {
		unsigned char   a = *position++,
						b = *position++,
						c = *position++,
						d = *position++;
		
		return (long)((a << 24) | (b << 16) | (c << 8) | d);
	} else {
		std::cerr << "BigEndianBuffer: attempted read beyond buffer limits\n";
		return 0;
	}
}

unsigned long BigEndianBuffer::ReadULong(void)
{
	if ((unsigned int)(position - data + 3) < size) {
		unsigned char   a = *position++,
						b = *position++,
						c = *position++,
						d = *position++;
		
		return (unsigned long)((a << 24) | (b << 16) | (c << 8) | d);
	} else {
		std::cerr << "BigEndianBuffer: attempted read beyond buffer limits\n";
		return 0;
	}
}

void BigEndianBuffer::ReadBlock(unsigned long _size, unsigned char *dest)
{
	if ((unsigned int)(position - data + _size - 1) < size) {
		memcpy(dest, position, _size);
		position += _size;
	} else {
		std::cerr << "BigEndianBuffer: attempted read beyond buffer limits\n";
	}
}

void BigEndianBuffer::WriteChar(char v)
{
	if ((unsigned int)(position - data) < size)
		*position++ = v;
	else
		std::cerr << "BigEndianBuffer: attempted write beyond buffer limits\n";
}

void BigEndianBuffer::WriteUChar(unsigned char v)
{
	if ((unsigned int)(position - data) < size)
		*position++ = v;
	else
		std::cerr << "BigEndianBuffer: attempted write beyond buffer limits\n";
}

void BigEndianBuffer::WriteShort(short v)
{
	if ((unsigned int)(position - data + 1) < size) {
		*position++ = (v >> 8) & 0xff;
		*position++ = v & 0xff;
	} else {
		std::cerr << "BigEndianBuffer: attempted write beyond buffer limits\n";
	}
}

void BigEndianBuffer::WriteUShort(unsigned short v)
{
	if ((unsigned int)(position - data + 1) < size) {
		*position++ = (v >> 8) & 0xff;
		*position++ = v & 0xff;
	} else {
		std::cerr << "BigEndianBuffer: attempted write beyond buffer limits\n";
	}
}

void BigEndianBuffer::WriteLong(long v)
{
	if ((unsigned int)(position - data + 3) < size) {
		*position++ = (v >> 24) & 0xff;
		*position++ = (v >> 16) & 0xff;
		*position++ = (v >> 8) & 0xff;
		*position++ = v & 0xff;
	} else {
		std::cerr << "BigEndianBuffer: attempted write beyond buffer limits\n";
	}
}

void BigEndianBuffer::WriteULong(unsigned long v)
{
	if ((unsigned int)(position - data + 3) < size) {
		*position++ = (v >> 24) & 0xff;
		*position++ = (v >> 16) & 0xff;
		*position++ = (v >> 8) & 0xff;
		*position++ = v & 0xff;
	} else {
		std::cerr << "BigEndianBuffer: attempted write beyond buffer limits\n";
	}
}

void BigEndianBuffer::WriteBlock(unsigned long _size, const void *src)
{
	if ((unsigned int)(position - data + _size - 1) < size) {
		memcpy(position, src, _size);
		position += _size;
	} else {
		std::cerr << "BigEndianBuffer: attempted write beyond buffer limits\n";
	}
}

void BigEndianBuffer::WriteZeroes(unsigned int n)
{
	if ((unsigned int)(position - data + n - 1) < size) {
		memset(position, 0, n);
		position += n;
	} else {
		std::cerr << "BigEndianBuffer: attempted write beyond buffer limits\n";
	}
}

unsigned char *BigEndianBuffer::Data(void) const
{
	return data;
}

unsigned int BigEndianBuffer::Size(void) const
{
	return size;
}

void BigEndianBuffer::Position(unsigned int pos)
{
	if (pos < size)
		position = data + pos;
	else
		std::cerr << "BigEndianBuffer: attempted to position beyond buffer limits (" << pos << "/" << (size-1) << ")\n";
}

unsigned int BigEndianBuffer::Position(void) const
{
	return position - data;
}


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
#include <cmath>
#include <iostream>
#include "BigEndianBuffer.h"

BigEndianBuffer::BigEndianBuffer(unsigned int _size):
		GenericEndianBuffer(_size)
{

}

BigEndianBuffer::BigEndianBuffer(unsigned char *_data, unsigned int _size):
		GenericEndianBuffer(_data, _size)
{

}

BigEndianBuffer::~BigEndianBuffer(void)
{

}

short BigEndianBuffer::ReadShort(void)
{
	if ((unsigned int)(mPosition - mData + 1) < mSize) {
		unsigned char	hi = *mPosition++,
						lo = *mPosition++;

		return (short)((hi << 8) | lo);
	} else {
		std::cerr << "BigEndianBuffer: attempted read beyond buffer limits\n";
		return 0;
	}
}

unsigned short BigEndianBuffer::ReadUShort(void)
{
	if ((unsigned int)(mPosition - mData + 1) < mSize) {
		unsigned char	hi = *mPosition++,
						lo = *mPosition++;
		
		return (unsigned short)((hi << 8) | lo);
	} else {
		std::cerr << "BigEndianBuffer: attempted read beyond buffer limits\n";
		return 0;
	}
}

long BigEndianBuffer::ReadLong(void)
{
	if ((unsigned int)(mPosition - mData + 3) < mSize) {
		unsigned char   a = *mPosition++,
						b = *mPosition++,
						c = *mPosition++,
						d = *mPosition++;
		
		return (long)((a << 24) | (b << 16) | (c << 8) | d);
	} else {
		std::cerr << "BigEndianBuffer: attempted read beyond buffer limits\n";
		return 0;
	}
}

unsigned long BigEndianBuffer::ReadULong(void)
{
	if ((unsigned int)(mPosition - mData + 3) < mSize) {
		unsigned char   a = *mPosition++,
						b = *mPosition++,
						c = *mPosition++,
						d = *mPosition++;
		
		return (unsigned long)((a << 24) | (b << 16) | (c << 8) | d);
	} else {
		std::cerr << "BigEndianBuffer: attempted read beyond buffer limits\n";
		return 0;
	}
}

void BigEndianBuffer::WriteShort(short v)
{
	if ((unsigned int)(mPosition - mData + 1) < mSize) {
		*mPosition++ = (v >> 8) & 0xff;
		*mPosition++ = v & 0xff;
	} else {
		std::cerr << "BigEndianBuffer: attempted write beyond buffer limits\n";
	}
}

void BigEndianBuffer::WriteUShort(unsigned short v)
{
	if ((unsigned int)(mPosition - mData + 1) < mSize) {
		*mPosition++ = (v >> 8) & 0xff;
		*mPosition++ = v & 0xff;
	} else {
		std::cerr << "BigEndianBuffer: attempted write beyond buffer limits\n";
	}
}

void BigEndianBuffer::WriteLong(long v)
{
	if ((unsigned int)(mPosition - mData + 3) < mSize) {
		*mPosition++ = (v >> 24) & 0xff;
		*mPosition++ = (v >> 16) & 0xff;
		*mPosition++ = (v >> 8) & 0xff;
		*mPosition++ = v & 0xff;
	} else {
		std::cerr << "BigEndianBuffer: attempted write beyond buffer limits\n";
	}
}

void BigEndianBuffer::WriteULong(unsigned long v)
{
	if ((unsigned int)(mPosition - mData + 3) < mSize) {
		*mPosition++ = (v >> 24) & 0xff;
		*mPosition++ = (v >> 16) & 0xff;
		*mPosition++ = (v >> 8) & 0xff;
		*mPosition++ = v & 0xff;
	} else {
		std::cerr << "BigEndianBuffer: attempted write beyond buffer limits\n";
	}
}

void BigEndianBuffer::WriteFixed(double d)
{
	WriteLong(static_cast<long>(std::floor(d * 65536.0 + 0.5)));
}

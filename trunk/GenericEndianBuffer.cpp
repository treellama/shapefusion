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
#include <cstring>
#include <vector>
#include "GenericEndianBuffer.h"

GenericEndianBuffer::GenericEndianBuffer(unsigned int _size):
		mSize(0), mSelfAllocated(true)
{
	mData = new unsigned char[_size];
	if (mData != NULL) {
		mPosition = mData;
		mSize = _size;
	}
}

GenericEndianBuffer::GenericEndianBuffer(unsigned char *_data, unsigned int _size):
		mData(_data), mPosition(_data), mSize(_size), mSelfAllocated(false)
{
	
}

GenericEndianBuffer::~GenericEndianBuffer(void)
{
	if (mSelfAllocated && mData != NULL) {
		delete[] mData;
		mData = NULL;
	}
}

char GenericEndianBuffer::ReadChar(void)
{
	if ((unsigned int)(mPosition - mData) < mSize) {
		unsigned char	v = *mPosition;

		mPosition++;
		return (char)v;
	} else {
		std::cerr << "GenericEndianBuffer: attempted read beyond buffer limits\n";
		return 0;
	}
}

unsigned char GenericEndianBuffer::ReadUChar(void)
{
	if ((unsigned int)(mPosition - mData) < mSize) {
		unsigned char	v = *mPosition;

		mPosition++;
		return v;
	} else {
		std::cerr << "GenericEndianBuffer: attempted read beyond buffer limits\n";
		return 0;
	}
}

void GenericEndianBuffer::ReadBlock(unsigned long _size, unsigned char *dest)
{
	if ((unsigned int)(mPosition - mData + _size - 1) < mSize) {
		memcpy(dest, mPosition, _size);
		mPosition += _size;
	} else {
		std::cerr << "GenericEndianBuffer: attempted read beyond buffer limits\n";
	}
}

void GenericEndianBuffer::WriteChar(char v)
{
	if ((unsigned int)(mPosition - mData) < mSize)
		*mPosition++ = v;
	else
		std::cerr << "GenericEndianBuffer: attempted write beyond buffer limits\n";
}

void GenericEndianBuffer::WriteUChar(unsigned char v)
{
	if ((unsigned int)(mPosition - mData) < mSize)
		*mPosition++ = v;
	else
		std::cerr << "GenericEndianBuffer: attempted write beyond buffer limits\n";
}

void GenericEndianBuffer::Write4CharCode(char c1, char c2, char c3, char c4)
{
	if ((unsigned int)(mPosition - mData + 4 - 1) < mSize) {
		*mPosition++ = c1;
		*mPosition++ = c2;
		*mPosition++ = c3;
		*mPosition++ = c4;
	} else {
		std::cerr << "GenericEndianBuffer: attempted write beyond buffer limits\n";
	}
}

void GenericEndianBuffer::WriteBlock(unsigned long _size, const void *src)
{
	if ((unsigned int)(mPosition - mData + _size - 1) < mSize) {
		memcpy(mPosition, src, _size);
		mPosition += _size;
	} else {
		std::cerr << "GenericEndianBuffer: attempted write beyond buffer limits\n";
	}
}

void GenericEndianBuffer::WriteZeroes(unsigned int n)
{
	if ((unsigned int)(mPosition - mData + n - 1) < mSize) {
		memset(mPosition, 0, n);
		mPosition += n;
	} else {
		std::cerr << "GenericEndianBuffer: attempted write beyond buffer limits\n";
	}
}

unsigned char *GenericEndianBuffer::Data(void) const
{
	return mData;
}

unsigned int GenericEndianBuffer::Size(void) const
{
	return mSize;
}

void GenericEndianBuffer::Position(unsigned int pos)
{
	if (pos < mSize)
		mPosition = mData + pos;
	else
		std::cerr << "GenericEndianBuffer: attempted to position beyond buffer limits (" << pos << "/" << (mSize-1) << ")\n";
}

unsigned int GenericEndianBuffer::Position(void) const
{
	return mPosition - mData;
}

static const unsigned long CRC32_POLYNOMIAL = 0xEDB88320L;
static std::vector<unsigned long> crc_table;

static void BuildCRCTable()
{
	if (crc_table.empty()) {
		crc_table.resize(256);

		for (unsigned int i = 0; i < crc_table.size(); ++i) {
			unsigned long crc = i;
			for (int j = 0; j < 8; ++j) {
				if (crc & 1) {
					crc = (crc >> 1) ^ CRC32_POLYNOMIAL;
				} else {
					crc >>= 1;
				}
				crc_table[i] = crc;
			}
		}
	}
}

unsigned long GenericEndianBuffer::CalculateCRC() const 
{
	BuildCRCTable();

	unsigned long crc = 0xFFFFFFFFL;
	
	unsigned char* p = mData;
	for (unsigned int i = 0; i < mSize; ++i) {
		unsigned long a = (crc >> 8) & 0x00FFFFFFL;
		unsigned long b = crc_table[((int) crc ^ *p++) & 0xff];
		crc = a^b;
	}
	return crc ^ 0xFFFFFFFFL;
}

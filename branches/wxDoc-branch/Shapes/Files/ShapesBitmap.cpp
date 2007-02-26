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

#include "ShapesBitmap.h"

// bitmap flags
enum {
	COLUMN_ORDER			= 1 << 15,
	TRANSPARENCY_ENABLED	= 1 << 14
};

ShapesBitmap::ShapesBitmap(wxInt32 offset, bool verbose) :
	ShapesElement(verbose), mOffset(offset)
{}

ShapesBitmap::~ShapesBitmap(void)
{}

short ShapesBitmap::Width(void)
{
	return mWidth;
}

#if wxUSE_STD_IOSTREAM
wxSTD ostream& ShapesBitmap::SaveObject(wxSTD ostream& stream)
#else
wxOutputStream& ShapesBitmap::SaveObject(wxOutputStream& stream)
#endif
{

}

BigEndianBuffer& ShapesBitmap::LoadObject(BigEndianBuffer& buffer)
{
	
	buffer.Position(mOffset);
	
	mWidth = buffer.ReadShort();
	mHeight = buffer.ReadShort();
	mBytesPerRow = buffer.ReadShort();
	
	if (mWidth < 0) {
		wxLogError("[ShapesBitmap] Invalid bitmap width");
		return buffer;
	}
	if (mHeight < 0) {
		wxLogError("[ShapesBitmap] Invalid bitmap height");
		return buffer;
	}
	if (mBytesPerRow < -1) {
		wxLogError("[ShapesBitmap] Invalid bitmap bytes-per-row");
		return buffer;
	}
	
	short	flags;
	
	flags = buffer.ReadShort();
	
	mColumnOrder = flags & COLUMN_ORDER;
	mTransparent = flags & TRANSPARENCY_ENABLED;
	
	mBitDepth = buffer.ReadShort();
	if (mBitDepth != 8) {
		wxLogError("[ShapesBitmap] Invalid bitmap depth %d", mBitDepth);
		return buffer;
	}

	if (IsVerbose()) {
		wxLogDebug("[ShapesBitmap]         Width:		%d", mWidth);
		wxLogDebug("[ShapesBitmap]         Height:	%d", mHeight);
		wxLogDebug("[ShapesBitmap]         Bytes/Row:	%d", mBytesPerRow);
		wxLogDebug("[ShapesBitmap]         Flags:		%d", flags);
		wxLogDebug("[ShapesBitmap]         Bit Depth:	%d", mBitDepth);
	}

	
	// skip unused fields and placeholders
	unsigned int	numscanlines = mColumnOrder ? mWidth : mHeight;
	
	buffer.Position(buffer.Position() + 20 + numscanlines * 4);
	
	// load pixel data
	mPixels = new unsigned char[mWidth * mHeight];
	if (mBytesPerRow > -1) {
		// uncompressed bitmap
		if (mColumnOrder) {
			// column order
			unsigned char	*dstp;
			
			for (int x = 0; x < mWidth; x++) {
				dstp = mPixels + x;
				for (int y = 0; y < mHeight; y++) {
					*dstp = buffer.ReadUChar();
					dstp += mWidth;
				}
			}
		} else {
			// row order
			buffer.ReadBlock(mWidth * mHeight, mPixels);
		}
	} else {
		// compressed bitmap (always column order)
		memset(mPixels, 0, mWidth * mHeight);
		for (int x = 0; x < mWidth; x++) {
			short			p0, p1;
			unsigned char	*dstp;
			
			p0 = buffer.ReadShort();
			p1 = buffer.ReadShort();
			dstp = mPixels + x + p0 * mWidth;
			while (p0 != p1) {
				*dstp = buffer.ReadUChar();
				dstp += mWidth;
				p0++;
			}
		}
	}
	
	mGoodData = true;
	return buffer;
}

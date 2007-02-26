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

#include "ShapeFusionLists.h"
#include "BigEndianBuffer.h"
#define SIZEOF_bitmap_definition			30

// a bitmap
class ShapesBitmap : public ShapesElement
{
private:
	short			mWidth, mHeight,
					mBytesPerRow,	// width for uncompressed bitmaps, -1 for compressed ones
					mBitDepth;		// 8
	bool			mColumnOrder,	// store in column-order format
					mTransparent;
	unsigned char	*mPixels;
	// list of frames referencing this bitmap
	intList			mUsers;
	
	wxInt32 mOffset;
	
public:

	ShapesBitmap(wxInt32 offset, bool verbose = false);
	~ShapesBitmap(void);

	short Width(void);
	
    wxOutputStream& SaveObject(wxOutputStream& stream);
    BigEndianBuffer& LoadObject(BigEndianBuffer& stream);
};

WX_DECLARE_LIST(ShapesBitmap, ShapesBitmapList);
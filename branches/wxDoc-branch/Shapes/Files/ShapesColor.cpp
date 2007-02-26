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

#include "ShapesColor.h"

// color flags
enum {
	SELF_LUMINESCENT_COLOR	= 1 << 7
};

ShapesColor::ShapesColor(bool verbose) :
	ShapesElement(verbose)
{

}

ShapesColor::~ShapesColor(void)
{

}

#if wxUSE_STD_IOSTREAM
wxSTD ostream& ShapesColor::SaveObject(wxSTD ostream& stream)
#else
wxOutputStream& ShapesColor::SaveObject(wxOutputStream& stream)
#endif
{
#if !wxUSE_STD_IOSTREAM
	wxDataOutputStream data_stream(stream);
	data_stream.BigEndianOrdered(true);
#endif
	unsigned char	flags = (mLuminescent ? SELF_LUMINESCENT_COLOR : 0);
	
	data_stream << flags;
	data_stream << mValue;
	data_stream << mRed;
	data_stream << mGreen;
	data_stream << mBlue;
	
	return stream;
}

BigEndianBuffer& ShapesColor::LoadObject(BigEndianBuffer& buffer)
{
	unsigned char	flags;

	flags = buffer.ReadUChar();
	mValue = buffer.ReadUChar();
	mRed = buffer.ReadUShort();
	mGreen = buffer.ReadUShort();
	mBlue = buffer.ReadUShort();
	
	mLuminescent = flags & SELF_LUMINESCENT_COLOR;
	
	mGoodData = true;
	return buffer;
}

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

#include "ShapesColorTable.h"

ShapesColorTable::ShapesColorTable(wxInt32 colorcount, wxInt32 offset, bool verbose) :
	ShapesElement(verbose), mColorCount(colorcount), mOffset(offset)
{}

ShapesColorTable::~ShapesColorTable(void)
{}

#if wxUSE_STD_IOSTREAM
wxSTD ostream& ShapesColorTable::SaveObject(wxSTD ostream& stream)
#else
wxOutputStream& ShapesColorTable::SaveObject(wxOutputStream& stream)
#endif
{
#if !wxUSE_STD_IOSTREAM
	wxDataOutputStream data_stream(stream);
	data_stream.BigEndianOrdered(true);
#endif
	
	for (short i = 0; i < mColorCount; i++) {
		ShapesColor		color = mColors.Item(i);
		
		color.SaveObject(stream);
	}
	return stream;
}

BigEndianBuffer& ShapesColorTable::LoadObject(BigEndianBuffer& buffer)
{
	
	for (short i = 0; i < mColorCount; i++) {
		ShapesColor		*color = new ShapesColor(IsVerbose());
		
		color->LoadObject(buffer);
		
		mColors.Append(color);
	}
	
	mGoodData = true;
	return buffer;
}

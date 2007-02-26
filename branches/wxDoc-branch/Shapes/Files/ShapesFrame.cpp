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

#include "ShapesFrame.h"

// frame flags
enum {
	X_MIRROR			= 1 << 15,	// mirror along vertical axis
	Y_MIRROR			= 1 << 14,	// mirror along horizontal axis
	KEYPOINT_OBSCURED	= 1 << 13	// "host obscures parasite" (RenderPlaceObjs.cpp)
};

ShapesFrame::ShapesFrame(wxInt32 offset, bool verbose) :
	ShapesElement(verbose), mOffset(offset)
{}

ShapesFrame::~ShapesFrame(void)
{}

void ShapesFrame::CalculateScaleFactor(ShapesBitmapList *bitmaps, short chunkPixelToWorld)
{
	// calculate scale factor from world_* fields and associated bitmap dimensions.
	// If this fails, default to collection global scale factor
	if (mBitmapIndex >= 0 && mBitmapIndex < (int)bitmaps->GetCount()) {
		int	bw = bitmaps->Item(mBitmapIndex)->GetData()->Width();
		
		if (bw > 0)
			mScaleFactor = (mWorldRight - mWorldLeft) / bw;
		else
			mScaleFactor = chunkPixelToWorld;
	} else {
		mScaleFactor = chunkPixelToWorld;
	}
}

#if wxUSE_STD_IOSTREAM
wxSTD ostream& ShapesFrame::SaveObject(wxSTD ostream& stream)
#else
wxOutputStream& ShapesFrame::SaveObject(wxOutputStream& stream)
#endif
{

}

BigEndianBuffer& ShapesFrame::LoadObject(BigEndianBuffer& buffer)
{
	unsigned short	flags;
	wxInt32			mli_fixed;
	
	buffer.Position(mOffset);
	
	flags = buffer.ReadUShort();

	mXmirror = flags & X_MIRROR;
	mYmirror = flags & Y_MIRROR;
	mKeypointObscured = flags & KEYPOINT_OBSCURED;
	
	mli_fixed = buffer.ReadLong();
	
	mMinimumLightIntensity = ((mli_fixed >> 16) & 0xffff) + (float)(mli_fixed & 0xffff) / 65536.0;	// convert fixed point [0,1] to double
	
	mBitmapIndex = buffer.ReadShort();
	mOriginX = buffer.ReadShort();
	mOriginY = buffer.ReadShort();
	mKeyX = buffer.ReadShort();
	mKeyY = buffer.ReadShort();
	mWorldLeft = buffer.ReadShort();
	mWorldRight = buffer.ReadShort();
	mWorldTop = buffer.ReadShort();
	mWorldBottom = buffer.ReadShort();
	mWorldX0 = buffer.ReadShort();
	mWorldY0 = buffer.ReadShort();
	
	if (IsVerbose()) {
		wxLogDebug("[ShapesFrame]         Flags:			%d", flags);
		wxLogDebug("[ShapesFrame]         Min. Light Intensity:	%d", mMinimumLightIntensity);
		wxLogDebug("[ShapesFrame]         Bitmap Index:	%d", mBitmapIndex);
		wxLogDebug("[ShapesFrame]         Origin (X):		%d", mOriginX);
		wxLogDebug("[ShapesFrame]         Origin (Y):		%d", mOriginY);
		wxLogDebug("[ShapesFrame]         Key (X):		%d", mKeyX);
		wxLogDebug("[ShapesFrame]         Key (Y):		%d", mKeyY);
		wxLogDebug("[ShapesFrame]         World (Left):	%d", mWorldLeft);
		wxLogDebug("[ShapesFrame]         World (Right):	%d", mWorldRight);
		wxLogDebug("[ShapesFrame]         World (Top):	%d", mWorldTop);
		wxLogDebug("[ShapesFrame]         World (Bottom):	%d", mWorldBottom);
		wxLogDebug("[ShapesFrame]         World (X0):		%d", mWorldX0);
		wxLogDebug("[ShapesFrame]         World (Y0):		%d", mWorldY0);
	}

	mGoodData = true;
	return buffer;
}

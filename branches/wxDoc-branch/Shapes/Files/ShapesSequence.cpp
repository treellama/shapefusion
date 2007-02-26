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

#include "ShapesSequence.h"

// sequence types (ShapesSequence.number_of_views). Nobody
// seems to know what these values really mean, they just
// tell the actual number of views in a redundant way
enum {
	ANIMATED_1 = 1,		// simple isotropic animation
	ANIMATED_2TO8 = 2,	// 8 view animation
	ANIMATED_3TO4 = 3,	// 4 view animation
	ANIMATED_4 = 4,		// 4 view animation
	ANIMATED_5TO8 = 5,	// 8 view animation
	ANIMATED_8 = 8,		// 8 view animation
	ANIMATED_3TO5 = 9,	// 5 view animation
	UNANIMATED = 10,	// no animation, choose a random frame
	ANIMATED_5 = 11		// 5 view animation
};

ShapesSequence::ShapesSequence(wxInt32 offset, bool verbose) :
	ShapesElement(verbose), mOffset(offset)
{}

ShapesSequence::~ShapesSequence(void)
{}

// given a high_level_shape_definition.number_of_views value,
// return the real number of views
int ShapesSequence::ActualNumberOfViews(int t)
{
	switch (t) {
		case UNANIMATED:
		case ANIMATED_1:
			return 1;
		case ANIMATED_3TO4:
		case ANIMATED_4:
			return 4;
		case ANIMATED_3TO5:
		case ANIMATED_5:
			return 5;
		case ANIMATED_2TO8:
		case ANIMATED_5TO8:
		case ANIMATED_8:
			return 8;
		default:
			wxLogError("[ShapesSequence] Unknown sequence type %d, don't know the number of views", t);
			return t;
	}
	return -1;
}

#if wxUSE_STD_IOSTREAM
wxSTD ostream& ShapesSequence::SaveObject(wxSTD ostream& stream)
#else
wxOutputStream& ShapesSequence::SaveObject(wxOutputStream& stream)
#endif
{

}

BigEndianBuffer& ShapesSequence::LoadObject(BigEndianBuffer& buffer)
{
	short namelen;
	
	buffer.Position(mOffset);
	mType = buffer.ReadShort();
	mFlags = buffer.ReadUShort();
	
	// the name is a Mac Pascal string, not a C string (length,chars)
	namelen = buffer.ReadUChar();
	
	if (namelen > 33)
	{
		wxLogError("[ShapesSequence] error in loading sequence name : name too long (%d/32)", namelen);
		return buffer;
	}
	
	for (int i = 0; i < 33; i++)
	{
		char c = buffer.ReadUChar();
		if (i <= namelen)
			mName.Append(c);
	}
	
	mNumberOfViews = buffer.ReadShort();
	mFramesPerView = buffer.ReadShort();
	mTicksPerFrame = buffer.ReadShort();
	mKeyFrame = buffer.ReadShort();
	mTransferMode = buffer.ReadShort();
	mTransferModePeriod = buffer.ReadShort();
	mFirstFrameSound = buffer.ReadShort();
	mKeyFrameSound = buffer.ReadShort();
	mLastFrameSound = buffer.ReadShort();
	mPixelsToWorld = buffer.ReadShort();
	mLoopFrame = buffer.ReadShort();
	
	if (IsVerbose()) {
		wxLogDebug("[ShapesSequence]         Type:					%d", mType);
		wxLogDebug("[ShapesSequence]         Flags:					%d", mFlags);
		wxLogDebug("[ShapesSequence]         Name:					%s", mName.c_str());
		wxLogDebug("[ShapesSequence]         Number of Views:		%d", mNumberOfViews);
		wxLogDebug("[ShapesSequence]         Frames/Views:			%d", mFramesPerView);
		wxLogDebug("[ShapesSequence]         Ticks/Frame:			%d", mTicksPerFrame);
		wxLogDebug("[ShapesSequence]         Key Frame:				%d", mKeyFrame);
		wxLogDebug("[ShapesSequence]         Transfer Mode:			%d", mTransferMode);
		wxLogDebug("[ShapesSequence]         Transfer Mode Period:	%d", mTransferModePeriod);
		wxLogDebug("[ShapesSequence]         First Frame Sound:		%d", mFirstFrameSound);
		wxLogDebug("[ShapesSequence]         Key Frame Sound:		%d", mKeyFrameSound);
		wxLogDebug("[ShapesSequence]         Last Frame Sound:		%d", mLastFrameSound);
		wxLogDebug("[ShapesSequence]         Pixels to World:		%d", mPixelsToWorld);
		wxLogDebug("[ShapesSequence]         Loop Frame:			%d", mLoopFrame);
	}
	
	if (mNumberOfViews < 0 || mFramesPerView < 0 || mKeyFrame < 0 || mFirstFrameSound < -1 || mKeyFrameSound < -1 || mLastFrameSound < -1 || mLoopFrame < -1)
	{
		wxLogError("[ShapesSequence] error in loading sequence : incorrect value");
		return buffer;
	}
	
	wxInt32 oldpos = buffer.Position();
	
	buffer.Position(buffer.Position() + 28);
	
	oldpos = buffer.Position();
	// load frame indexes
	int	n = ActualNumberOfViews(mNumberOfViews) * mFramesPerView;
	
	if (n > 0) {
		for (int k = 0; k < n; k++)
		{
			short i = buffer.ReadShort();
			mFrameIndexes.Append(&i);
		}
	}
	
	buffer.ReadShort();	// terminating index (usually 0 but can be garbage)
	
	mGoodData = true;
	return buffer;
}

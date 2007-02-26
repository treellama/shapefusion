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

#define SIZEOF_high_level_shape_definition	88
// NOTE about SIZEOF_high_level_shape_definition. The original engine
// sets SIZEOF_high_level_shape_definition=90 because the first frame
// index is included in the high_level_shape_definition. I don't like
// this, so I do my way (but be careful!)

// a sequence, aka "high level shape definition"
class ShapesSequence : public ShapesElement
{
private:
	short			mType;
	unsigned short	mFlags;
	wxString		mName;
	short			mNumberOfViews,
					mFramesPerView,
					mTicksPerFrame,
					mKeyFrame,
					mTransferMode,
					mTransferModePeriod,
					mFirstFrameSound,
					mKeyFrameSound,
					mLastFrameSound,
					mPixelsToWorld,
					mLoopFrame;
	shortList		mFrameIndexes;
	
	wxInt32 mOffset;
	
	int ActualNumberOfViews(int t);
public:

	ShapesSequence(wxInt32 offset, bool verbose = false);
	~ShapesSequence(void);
	
    wxOutputStream& SaveObject(wxOutputStream& stream);
    BigEndianBuffer& LoadObject(BigEndianBuffer& stream);
};

WX_DECLARE_LIST(ShapesSequence, ShapesSequenceList);


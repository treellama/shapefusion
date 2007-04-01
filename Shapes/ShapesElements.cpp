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
#include "ShapesElements.h"
#include "utilities.h"

// on-file struct sizes
#define SIZEOF_collection_definition		544
#define SIZEOF_rgb_color_value				8
#define SIZEOF_bitmap_definition			30
#define SIZEOF_low_level_shape_definition	36
#define SIZEOF_high_level_shape_definition	88
// NOTE about SIZEOF_high_level_shape_definition. The original engine
// sets SIZEOF_high_level_shape_definition=90 because the first frame
// index is included in the high_level_shape_definition. I don't like
// this, so I do my way (but be careful!)

#define COLLECTION_VERSION					3

// color flags
enum {
	SELF_LUMINESCENT_COLOR	= 1 << 7
};

// bitmap flags
enum {
	COLUMN_ORDER			= 1 << 15,
	TRANSPARENCY_ENABLED	= 1 << 14
};

// frame flags
enum {
	X_MIRROR			= 1 << 15,	// mirror along vertical axis
	Y_MIRROR			= 1 << 14,	// mirror along horizontal axis
	KEYPOINT_OBSCURED	= 1 << 13	// "host obscures parasite" (RenderPlaceObjs.cpp)
};

ShapesColor::ShapesColor(bool verbose) : ShapesElement(verbose)
{

}

ShapesColor::~ShapesColor(void)
{

}

BigEndianBuffer& ShapesColor::SaveObject(BigEndianBuffer& buffer)
{
	unsigned char	flags = (mLuminescent ? SELF_LUMINESCENT_COLOR : 0);
	
	buffer.WriteUChar(flags);
	buffer.WriteUChar(mValue);
	buffer.WriteUShort(mRed);
	buffer.WriteUShort(mGreen);
	buffer.WriteUShort(mBlue);
	
	return buffer;
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

ShapesColorTable::ShapesColorTable(bool verbose) : ShapesElement(verbose)
{

}

ShapesColorTable::~ShapesColorTable(void)
{
	for (unsigned int i = 0; i < mColors.size(); i++)
		delete mColors[i];
}
	
BigEndianBuffer& ShapesColorTable::SaveObject(BigEndianBuffer& stream)
{
	for (unsigned int i = 0; i < mColors.size(); i++) {
		ShapesColor	*color = mColors[i];

		color->SaveObject(stream);
	}
	return stream;
}

BigEndianBuffer& ShapesColorTable::LoadObject(BigEndianBuffer& buffer, unsigned int offset, unsigned int color_count)
{
	buffer.Position(offset);

	for (unsigned int i = 0; i < color_count; i++) {
		ShapesColor	*color = new ShapesColor(IsVerbose());

		color->LoadObject(buffer);
		
		if (!color->IsGood())
		{
			wxLogError(wxT("[ShapesColorTable] Error loading color table"));
			mGoodData = false;
			return buffer;
		}
		mColors.push_back(color);
	}
	
	mGoodData = true;
	return buffer;
}

ShapesBitmap::ShapesBitmap(bool verbose) : ShapesElement(verbose), mPixels(NULL)
{
}

ShapesBitmap::ShapesBitmap(wxImage image, ShapesColorTable *colortable) :
	ShapesElement(false),
	mWidth(image.GetWidth()), mHeight(image.GetHeight()), mBytesPerRow(image.GetWidth()), mBitDepth(8), mColumnOrder(true), mTransparent(false), mPixels(NULL)
{
	unsigned char	*srcpixels = image.GetData(),
					*src = srcpixels,
					*dst;
	
	mPixels = new unsigned char[mWidth * mHeight];
	if (mPixels == NULL) {
		wxLogError(wxT("Could not allocate new %dx%d bitmap\n"), mWidth, mHeight);
		return;
	}
	dst = mPixels;
	// quantize from 8-bit RGB pixels to an indexed bitmap. We need to transform
	// RGB to HSV and perform the comparison in that space to get good results.
	// FIXME this is not yet perfect (not as good as PhotoShop or Gimp)
	for (int i = 0; i < mWidth * mHeight; i++) {
		unsigned char 	r = *src++, g = *src++, b = *src++;
		float			hue, sat, val,
						min_dist = 0;
		unsigned char   best_value = 0;
		
		RGB2HSV(r / 255.0, g / 255.0, b / 255.0, &hue, &sat, &val);
		for (unsigned int j = 0; j < colortable->ColorCount(); j++) {
			unsigned short	ct_r = colortable->GetColor(j)->Red(),
							ct_g = colortable->GetColor(j)->Green(),
							ct_b = colortable->GetColor(j)->Blue();
			float			hue2, sat2, val2,
				delta_h, delta_s, delta_v,
				dist;
			
			RGB2HSV(ct_r / 65535.0, ct_g / 65535.0, ct_b / 65535.0, &hue2, &sat2, &val2);
			delta_h = hue2 - hue;
			delta_s = sat2 - sat;
			delta_v = val2 - val;
			dist = delta_h*delta_h + delta_s*delta_s + delta_v*delta_v;
			if (dist < min_dist || j == 0) {
				min_dist = dist;
				best_value = colortable->GetColor(j)->Value();
			}
		}
		*dst++ = best_value;
		if (best_value == 0)
			mTransparent = true;	// guess the user will want transparency
	}
}

ShapesBitmap::~ShapesBitmap(void)
{
	if (mPixels)
		delete mPixels;
	mPixels = NULL;
}

unsigned int ShapesBitmap::SizeInFile(void) const
{
	unsigned int size = 0;

//	size = SIZEOF_bitmap_definition;

	// scanline pointer placeholder
	if (mColumnOrder)
		size += 4 * mWidth;
	else
		size += 4 * mHeight;
	if (mBytesPerRow == -1) {
		// compressed
		size += mWidth * 4;
		for (int x = 0; x < mWidth; x++) {
			unsigned char	*pp = mPixels + x;
			int				p0 = -1,
				p1;
			
			for (int y = 0; y < mHeight; y++) {
				if (*pp != 0) {
					p0 = y;
					break;
				}
				pp += mWidth;
			}
			if (p0 == -1)
				continue;	// no opaque mPixels in this column
			p1 = p0;
			pp = mPixels + x + mWidth * (mHeight - 1);
			for (int y = mHeight - 1; y >= 0; y--) {
				if (*pp != 0) {
					p1 = y;
					break;
				}
				pp -= mWidth;
			}
			size += p1 - p0 + 1;
		}
	} else {
		// plain
		size += mWidth * mHeight;
	}

	return size;
}

BigEndianBuffer& ShapesBitmap::SaveObject(BigEndianBuffer& buffer)
{
	short		flags = 0;
	
	if (mColumnOrder)
		flags |= COLUMN_ORDER;
	if (mTransparent)
		flags |= TRANSPARENCY_ENABLED;
	buffer.WriteShort(mWidth);
	buffer.WriteShort(mHeight);
	buffer.WriteShort(mBytesPerRow);
	buffer.WriteShort(flags);
	buffer.WriteShort(mBitDepth);
	buffer.WriteZeroes(20 + 4 * (mColumnOrder ? mWidth : mHeight));
	if (mBytesPerRow == -1) {
		// compress
		for (int x = 0; x < mWidth; x++) {
			unsigned char	*pp = mPixels + x;
			int				p0 = -1,
				p1;
			
			for (int y = 0; y < mHeight; y++) {
				if (*pp != 0) {
					p0 = y;
					break;
				}
				pp += mWidth;
			}
			if (p0 == -1) {
				// no opaque pixels in this column
				buffer.WriteShort(0);
				buffer.WriteShort(0);
			} else {
				// found opaque pixels, go on
				p1 = p0;
				pp = mPixels + x + mWidth * (mHeight - 1);
				for (int y = mHeight - 1; y >= 0; y--) {
					if (*pp != 0) {
						p1 = y;
						break;
					}
					pp -= mWidth;
				}
				buffer.WriteShort(p0);
				buffer.WriteShort(p1 + 1);
				pp = mPixels + x + p0 * mWidth;
				for (int y = p0; y <= p1; y++) {
					buffer.WriteChar(*pp);
					pp += mWidth;
				}
			}
		}
	} else {
		if (mColumnOrder) {
			for (int x = 0; x < mWidth; x++) {
				for (int y = 0; y < mHeight; y++)
					buffer.WriteChar(*(mPixels + x + y * mWidth));
			}
		} else {
			buffer.WriteBlock(mWidth * mHeight, mPixels);
		}
	}
	return buffer;
}

BigEndianBuffer& ShapesBitmap::LoadObject(BigEndianBuffer& buffer, unsigned int offset)
{
	buffer.Position(offset);

	mWidth = buffer.ReadShort();
	mHeight = buffer.ReadShort();
	mBytesPerRow = buffer.ReadShort();

	if (mWidth < 0) {
		wxLogError(wxT("[ShapesBitmap] Invalid bitmap width %d"), mWidth);
		return buffer;
	}
	if (mHeight < 0) {
		wxLogError(wxT("[ShapesBitmap] Invalid bitmap height %d"), mHeight);
		return buffer;
	}
	if (mBytesPerRow < -1) {
		wxLogError(wxT("[ShapesBitmap] Invalid bitmap bytes-per-row %d"), mBytesPerRow);
		return buffer;
	}

	short	flags = buffer.ReadShort();

	mColumnOrder = flags & COLUMN_ORDER;
	mTransparent = flags & TRANSPARENCY_ENABLED;

	mBitDepth = buffer.ReadShort();
	if (mBitDepth != 8) {
		wxLogError(wxT("[ShapesBitmap] Invalid bitmap depth %d"), mBitDepth);
		return buffer;
	}

	if (IsVerbose()) {
		wxLogDebug(wxT("[ShapesBitmap]         Width:		%d"), mWidth);
		wxLogDebug(wxT("[ShapesBitmap]         Height:		%d"), mHeight);
		wxLogDebug(wxT("[ShapesBitmap]         Bytes/Row:	%d"), mBytesPerRow);
		wxLogDebug(wxT("[ShapesBitmap]         Flags:		%d"), flags);
		wxLogDebug(wxT("[ShapesBitmap]         Bit Depth:	%d"), mBitDepth);
	}

	// skip unused fields and placeholders
	unsigned int	numscanlines = (mColumnOrder ? mWidth : mHeight);

	buffer.Position(buffer.Position() + 20 + numscanlines * 4);

	// load pixel data
	mPixels = new unsigned char[mWidth * mHeight];
	if (mPixels == NULL) {
		wxLogError(wxT("[ShapesBitmap] Could not allocate pixel buffer"));
		return buffer;
	}
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

ShapesFrame::ShapesFrame(bool verbose) : ShapesElement(verbose)
{
	// initialize values to something reasonable
	mBitmapIndex = -1;
	mXmirror = mYmirror = mKeypointObscured = false;
	mMinimumLightIntensity = 0;
	mOriginX = mOriginY = mKeyX = mKeyX = 0;
	mScaleFactor = 0;
	mWorldLeft = mWorldRight = mWorldTop = mWorldBottom = 0;
	mWorldX0 = mWorldY0 = 0;
}

ShapesFrame::~ShapesFrame(void)
{

}

BigEndianBuffer& ShapesFrame::SaveObject(BigEndianBuffer& buffer)
{
	unsigned short	flags = 0;
	float			mli_integer, mli_fractional;
	long			min_light_intensity = 0;
	
	if (mXmirror)
		flags |= X_MIRROR;
	if (mYmirror)
		flags |= Y_MIRROR;
	if (mKeypointObscured)
		flags |= KEYPOINT_OBSCURED;
		
	// float to fixed
	mli_fractional = modff(mMinimumLightIntensity, &mli_integer);
	min_light_intensity |= (((short)mli_integer) << 16) & 0xffff0000;
	min_light_intensity |= (short)roundf(mli_fractional * 0xffff) & 0x0000ffff;
	buffer.WriteUShort(flags);
	buffer.WriteLong(min_light_intensity);
	buffer.WriteShort(mBitmapIndex);
	buffer.WriteShort(mOriginX);
	buffer.WriteShort(mOriginY);
	buffer.WriteShort(mKeyX);
	buffer.WriteShort(mKeyY);
	buffer.WriteShort(mWorldLeft);
	buffer.WriteShort(mWorldRight);
	buffer.WriteShort(mWorldTop);
	buffer.WriteShort(mWorldBottom);
	buffer.WriteShort(mWorldX0);
	buffer.WriteShort(mWorldY0);
	buffer.WriteZeroes(8);
	
	return buffer;
}

BigEndianBuffer& ShapesFrame::LoadObject(BigEndianBuffer& buffer, unsigned int offset)
{
	unsigned short	flags;
	wxInt32			mli_fixed;

	buffer.Position(offset);

	flags = buffer.ReadUShort();

	mXmirror = flags & X_MIRROR;
	mYmirror = flags & Y_MIRROR;
	mKeypointObscured = flags & KEYPOINT_OBSCURED;
	
	mli_fixed = buffer.ReadLong();
	
	mMinimumLightIntensity = ((mli_fixed >> 16) & 0xffff) + (float)(mli_fixed & 0xffff) / 65536.0;	// convert fixed point [0,1] to float
	
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
		wxLogDebug(wxT("[ShapesFrame]         Flags:			%d"), flags);
		wxLogDebug(wxT("[ShapesFrame]         Min. Light Intensity:	%f"), mMinimumLightIntensity);
		wxLogDebug(wxT("[ShapesFrame]         Bitmap Index:	%d"), mBitmapIndex);
		wxLogDebug(wxT("[ShapesFrame]         Origin (X):		%d"), mOriginX);
		wxLogDebug(wxT("[ShapesFrame]         Origin (Y):		%d"), mOriginY);
		wxLogDebug(wxT("[ShapesFrame]         Key (X):		%d"), mKeyX);
		wxLogDebug(wxT("[ShapesFrame]         Key (Y):		%d"), mKeyY);
		wxLogDebug(wxT("[ShapesFrame]         World (Left):	%d"), mWorldLeft);
		wxLogDebug(wxT("[ShapesFrame]         World (Right):	%d"), mWorldRight);
		wxLogDebug(wxT("[ShapesFrame]         World (Top):	%d"), mWorldTop);
		wxLogDebug(wxT("[ShapesFrame]         World (Bottom):	%d"), mWorldBottom);
		wxLogDebug(wxT("[ShapesFrame]         World (X0):		%d"), mWorldX0);
		wxLogDebug(wxT("[ShapesFrame]         World (Y0):		%d"), mWorldY0);
	}

	mGoodData = true;
	return buffer;
}

ShapesSequence::ShapesSequence(bool verbose) : ShapesElement(verbose)
{
	// initialize values to something reasonable
	mType = 0;
	mFlags = 0;
	mName = _T("new sequence");
	mNumberOfViews = UNANIMATED;
	mFramesPerView = 0;
	mTicksPerFrame = 1;
	mKeyFrame = 0;
	mTransferMode = 0;
	mTransferModePeriod = 1;
	mFirstFrameSound = mKeyFrameSound = mLastFrameSound = -1;
	mPixelsToWorld = 0;
	mLoopFrame = 0;
}


ShapesSequence::~ShapesSequence(void)
{

}

unsigned int ShapesSequence::SizeInFile() const
{
	unsigned int size = 0;
	
	//size = SIZEOF_high_level_shape_definition;
	
	size += 2 * (FrameIndexCount() + 1);
		
	return size;
}

BigEndianBuffer& ShapesSequence::SaveObject(BigEndianBuffer& buffer)
{
	wxCSConv	seqnameconv(wxT("macintosh"));
	char		name[33] = "";

	buffer.WriteShort(mType);
	buffer.WriteUShort(mFlags);
	buffer.WriteChar(mName.Length());
	strncpy(name, mName.mb_str(seqnameconv), 33);
	buffer.WriteBlock(33, (unsigned char *)name);
	buffer.WriteShort(mNumberOfViews);
	buffer.WriteShort(mFramesPerView);
	buffer.WriteShort(mTicksPerFrame);
	buffer.WriteShort(mKeyFrame);
	buffer.WriteShort(mTransferMode);
	buffer.WriteShort(mTransferModePeriod);
	buffer.WriteShort(mFirstFrameSound);
	buffer.WriteShort(mKeyFrameSound);
	buffer.WriteShort(mLastFrameSound);
	buffer.WriteShort(mPixelsToWorld);
	buffer.WriteShort(mLoopFrame);
	buffer.WriteZeroes(28);
	for (unsigned int i = 0; i < mFrameIndexes.size(); i++)
		buffer.WriteShort(mFrameIndexes[i]);
	buffer.WriteShort(0);
	
	return buffer;
}

BigEndianBuffer& ShapesSequence::LoadObject(BigEndianBuffer& buffer, long offset)
{
	buffer.Position(offset);
	mType = buffer.ReadShort();
	mFlags = buffer.ReadUShort();
	
	// the mName is a Mac Pascal string, not a C string (length,chars)
	unsigned char	namelen = buffer.ReadUChar();

	if (namelen > 32) {
		wxLogError(wxT("[ShapesSequence] Sequence name too long (%d/32)"), namelen);
		return buffer;
	}

	char		name[33];
	wxCSConv	seqnameconv(wxT("macintosh"));

	buffer.ReadBlock(33, (unsigned char *)name);
	name[namelen] = 0;
	mName = wxString(name, seqnameconv, namelen);

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
	buffer.Position(buffer.Position() + 28);

	if (IsVerbose()) {
		wxLogDebug(wxT("[ShapesSequence]         Type:					%d"), mType);
		wxLogDebug(wxT("[ShapesSequence]         Flags:					%d"), mFlags);
		wxLogDebug(wxT("[ShapesSequence]         Name:					%s"), mName.c_str());
		wxLogDebug(wxT("[ShapesSequence]         Number of Views:		%d"), mNumberOfViews);
		wxLogDebug(wxT("[ShapesSequence]         Frames/Views:			%d"), mFramesPerView);
		wxLogDebug(wxT("[ShapesSequence]         Ticks/Frame:			%d"), mTicksPerFrame);
		wxLogDebug(wxT("[ShapesSequence]         Key Frame:				%d"), mKeyFrame);
		wxLogDebug(wxT("[ShapesSequence]         Transfer Mode:			%d"), mTransferMode);
		wxLogDebug(wxT("[ShapesSequence]         Transfer Mode Period:	%d"), mTransferModePeriod);
		wxLogDebug(wxT("[ShapesSequence]         First Frame Sound:		%d"), mFirstFrameSound);
		wxLogDebug(wxT("[ShapesSequence]         Key Frame Sound:		%d"), mKeyFrameSound);
		wxLogDebug(wxT("[ShapesSequence]         Last Frame Sound:		%d"), mLastFrameSound);
		wxLogDebug(wxT("[ShapesSequence]         Pixels to World:		%d"), mPixelsToWorld);
		wxLogDebug(wxT("[ShapesSequence]         Loop Frame:			%d"), mLoopFrame);
	}

	if (mNumberOfViews < 0 || mFramesPerView < 0) {
		wxLogError(wxT("[ShapesSequence] Invalid sequence type parameters: numberOfViews=%d, framesPerView=%d"),
						mNumberOfViews, mFramesPerView);
		return buffer;
	}
	// guess these shouldn't be < 0, but RED Shapes have a case with mKeyFrame=-1
	if (mKeyFrame < -1 || mLoopFrame < -1) {
		wxLogError(wxT("[ShapesSequence] Invalid key/loop frame values in sequence data: keyFrame=%d, loopFrame=%d"),
						mKeyFrame, mLoopFrame);
		return buffer;
	}
	if (mFirstFrameSound < -1 || mKeyFrameSound < -1 || mLastFrameSound < -1) {
		wxLogError(wxT("[ShapesSequence] Invalid sound values in sequence data: firstFrameSound=%d, keyFrameSound=%d, lastFrameSound=%d"),
						mFirstFrameSound, mKeyFrameSound, mLastFrameSound);
		return buffer;
	}

	// load frame indexes
	int	n = ActualNumberOfViews(mNumberOfViews) * mFramesPerView;

	if (n > 0) {
		for (int k = 0; k < n; k++)
			mFrameIndexes.push_back(buffer.ReadShort());
	}

	buffer.ReadShort();	// terminating index (usually 0 but can be garbage)

	mGoodData = true;
	return buffer;
}

// given a high_level_shape_definition.mNumberOfViews value,
// return the real number of views
int ActualNumberOfViews(int t)
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
			wxLogError(wxT("[ShapesSequence] Unknown sequence type %d, don't know the number of views"), t);
			return t;
	}
	return -1;
}

ShapesChunk::ShapesChunk(bool verbose) : ShapesElement(verbose)
{
	
}

ShapesChunk::~ShapesChunk(void)
{
	unsigned int i;

	for (i = 0; i < mColorTables.size(); i++)
		delete mColorTables[i];
	for (i = 0; i < mSequences.size(); i++)
		delete mSequences[i];
	for (i = 0; i < mFrames.size(); i++)
		delete mFrames[i];
	for (i = 0; i < mBitmaps.size(); i++)
		delete mBitmaps[i];
}

ShapesColorTable* ShapesChunk::GetColorTable(unsigned int index) const
{
	if (index < 0 || index > mColorTables.size())
		return NULL;
	return mColorTables[index];
}

ShapesBitmap* ShapesChunk::GetBitmap(unsigned int index) const
{
	if (index < 0 || index > mBitmaps.size())
		return NULL;
	return mBitmaps[index];
}

ShapesFrame* ShapesChunk::GetFrame(unsigned int index) const
{
	if (index < 0 || index > mFrames.size())
		return NULL;
	return mFrames[index];
}

ShapesSequence* ShapesChunk::GetSequence(unsigned int index) const
{
	if (index < 0 || index > mSequences.size())
		return NULL;
	return mSequences[index];
}

void ShapesChunk::InsertColorTable(ShapesColorTable *ct)
{
	mColorTables.push_back(ct);
}

void ShapesChunk::DeleteColorTable(unsigned int ct)
{
	mColorTables.erase(mColorTables.begin() + ct);
}

void ShapesChunk::InsertBitmap(ShapesBitmap *b)
{
	mBitmaps.push_back(b);
}

void ShapesChunk::DeleteBitmap(unsigned int b)
{
	if (b < mBitmaps.size()) {
		// preserve existing frame-bitmap associations and associate
		// a null bitmap to frames using the bitmap we're deleting
		for (unsigned int i = 0; i < mFrames.size(); i++) {
			short	bitmap_index = mFrames[i]->BitmapIndex();

			if (bitmap_index == (int)b)
				mFrames[i]->SetBitmapIndex(-1);
			else if (bitmap_index > (int)b)
				mFrames[i]->SetBitmapIndex(bitmap_index - 1);
		}
		// now actually delete the bitmap
		mBitmaps.erase(mBitmaps.begin() + b);
	}
}

void ShapesChunk::InsertFrame(ShapesFrame *f)
{
	mFrames.push_back(f);
}

void ShapesChunk::DeleteFrame(unsigned int f)
{
	if (f < mFrames.size()) {
		// preserve existing sequence-frame associations and
		// unreference this frame index from any sequence using it
		for (unsigned int i = 0; i < mSequences.size(); i++) {
			for (unsigned int j = 0; j < mSequences[i]->FrameIndexCount(); j++) {
				short frame_index = mSequences[i]->GetFrameIndex(j);

				if (frame_index == (int)f)
					mSequences[i]->SetFrameIndex(j, -1);
				else if (frame_index > (int)f)
					mSequences[i]->SetFrameIndex(j, frame_index - 1);
			}
		}
		// now actually delete the frame
		mFrames.erase(mFrames.begin() + f);
	}
}

void ShapesChunk::InsertSequence(ShapesSequence *s)
{
	mSequences.push_back(s);
}

void ShapesChunk::DeleteSequence(unsigned int s)
{
	if (s < mSequences.size())
		mSequences.erase(mSequences.begin() + s);
}

unsigned int ShapesChunk::SizeInFile(void) const
{
	unsigned int	bitmap_count = BitmapCount(),
					frame_count = FrameCount(),
					sequence_count = SequenceCount(),
					color_table_count = ColorTableCount(),
					size;
	
	// size of our definition
	size = SIZEOF_collection_definition;
	// add contribute of sequence offset table
	size += 4 * sequence_count;
	// add contribute of bitmap offset table
	size += 4 * bitmap_count;
	// add contribute of frame offset table
	size += 4 * frame_count;
	// add contribute of color tables
	if (color_table_count > 0)
		size += SIZEOF_rgb_color_value * color_table_count * GetColorTable(0)->ColorCount();
	// add contribute of bitmaps
	size += SIZEOF_bitmap_definition * bitmap_count;
	for (unsigned int i = 0; i < bitmap_count; i++) {
		ShapesBitmap	*bitmap = mBitmaps[i];

		size += bitmap->SizeInFile();
	}		
	// add contribute of frame definitions
	size += SIZEOF_low_level_shape_definition * frame_count;
	// add contribute of sequence definitions (and following frame indexes)
	size += SIZEOF_high_level_shape_definition * sequence_count;
	for (unsigned int i = 0 ; i < sequence_count ; i++) {
		ShapesSequence	*seq = mSequences[i];

		size += seq->SizeInFile();
	}

	return size;
}

BigEndianBuffer& ShapesChunk::SaveObject(BigEndianBuffer& buffer)
{
	unsigned int	bitmap_count = BitmapCount(),
					frame_count = FrameCount(),
					sequence_count = SequenceCount(),
					i;
	long			sequence_table_offset,
					sequence_offsets[sequence_count],
					frame_table_offset,
					frame_offsets[frame_count],
					bitmap_table_offset,
					bitmap_offsets[bitmap_count];
				
	// skip the collection definition, we'll fill it at the end
	buffer.Position(SIZEOF_collection_definition);
	// write color tables
	for (i = 0; i < ColorTableCount(); i++)
		mColorTables[i]->SaveObject(buffer);
	
	// write sequences
	sequence_table_offset = buffer.Position();
	if (sequence_count > 0) {
		buffer.Position(buffer.Position() + sequence_count * 4);
		
		for (i = 0; i < sequence_count; i++) {
			sequence_offsets[i] = buffer.Position();
			
			mSequences[i]->SaveObject(buffer);
		}
	}
	// write frames
	frame_table_offset = buffer.Position();
	buffer.Position(buffer.Position() + frame_count * 4);
	for (i = 0; i < frame_count; i++) {
		frame_offsets[i] = buffer.Position();
		
		mFrames[i]->SaveObject(buffer);
	}
	
	// write bitmaps
	bitmap_table_offset = buffer.Position();
	buffer.Position(buffer.Position() + bitmap_count * 4);
	for (i = 0; i < bitmap_count; i++) {
		bitmap_offsets[i] = buffer.Position();
		
		mBitmaps[i]->SaveObject(buffer);
	}
	
	// go back and write the collection definition (with correct offsets)
	buffer.Position(0);
	buffer.WriteShort(mVersion);
	buffer.WriteShort(mType);
	buffer.WriteUShort(mFlags);
	buffer.WriteShort(GetColorTable(0)->ColorCount());
	buffer.WriteShort(ColorTableCount());
	buffer.WriteLong(SIZEOF_collection_definition);
	buffer.WriteShort(sequence_count);
	buffer.WriteLong(sequence_table_offset);
	buffer.WriteShort(frame_count);
	buffer.WriteLong(frame_table_offset);
	buffer.WriteShort(bitmap_count);
	buffer.WriteLong(bitmap_table_offset);
	buffer.WriteShort(mPixelsToWorld);
	buffer.WriteLong(SizeInFile());
	buffer.WriteZeroes(506);
	
	// fill offset tables
	if (bitmap_count > 0) {
		buffer.Position(bitmap_table_offset);
		for (i = 0; i < bitmap_count; i++)
			buffer.WriteLong(bitmap_offsets[i]);
	}
	if (frame_count > 0) {
		buffer.Position(frame_table_offset);
		for (i = 0; i < frame_count; i++)
			buffer.WriteLong(frame_offsets[i]);
	}
	if (sequence_count > 0) {
		buffer.Position(sequence_table_offset);
		for (i = 0; i < sequence_count; i++)
			buffer.WriteLong(sequence_offsets[i]);
	}
	
	return buffer;
}

BigEndianBuffer& ShapesChunk::LoadObject(BigEndianBuffer& buffer)
{
	short	color_count,
			clut_count,
			bitmap_count,
			high_level_shape_count,
			low_level_shape_count,
			i;
	long	color_table_offset,
			high_level_shape_offset_table_offset,
			low_level_shape_offset_table_offset,
			bitmap_offset_table_offset,
			oldpos,
			offset,
			size;

	mVersion = buffer.ReadShort();
	mType = buffer.ReadShort();
	mFlags = buffer.ReadUShort();
	color_count = buffer.ReadShort();
	clut_count = buffer.ReadShort();
	color_table_offset = buffer.ReadLong();
	high_level_shape_count = buffer.ReadShort();
	high_level_shape_offset_table_offset = buffer.ReadLong();
	low_level_shape_count = buffer.ReadShort();
	low_level_shape_offset_table_offset = buffer.ReadLong();
	bitmap_count = buffer.ReadShort();
	bitmap_offset_table_offset = buffer.ReadLong();
	mPixelsToWorld = buffer.ReadShort();
	size = buffer.ReadLong();

	// validate values
	if (mVersion != COLLECTION_VERSION) {
		wxLogError(wxT("[ShapesChunk] Unknown collection version %d"), mVersion);
		return buffer;
	}

	if ((unsigned long)size != buffer.Size()) {
		wxLogError(wxT("[ShapesChunk] Chunk size mismatch (%ld/%d): this may not be a Marathon shapes file"), size, buffer.Size());
		return buffer;
	}
	if (color_table_offset < SIZEOF_collection_definition
		|| color_table_offset >= size
		|| high_level_shape_offset_table_offset < SIZEOF_collection_definition
		|| high_level_shape_offset_table_offset >= size
		|| low_level_shape_offset_table_offset < SIZEOF_collection_definition
		|| low_level_shape_offset_table_offset >= size
		|| bitmap_offset_table_offset < SIZEOF_collection_definition
		|| bitmap_offset_table_offset >= size) {
		wxLogError(wxT("[ShapesChunk] Invalid offsets in collection definition: this may not be a Marathon shapes file"));
		return buffer;
	}
	if (color_count < 0 || clut_count < 0 || high_level_shape_count < 0 || low_level_shape_count < 0 || bitmap_count < 0) {
		wxLogError(wxT("[ShapesChunk] Invalid object counts in collection definition: this may not be a Marathon shapes file"));
		return buffer;
	}

	if (IsVerbose()) {
		wxLogDebug(wxT("[ShapesChunk]         Version: %d"), mVersion);
		wxLogDebug(wxT("[ShapesChunk]         Type:    %d"), mType);
		wxLogDebug(wxT("[ShapesChunk]         Flags:   %d"), mFlags);
		wxLogDebug(wxT("[ShapesChunk]         %d color tables, %d colors per table"), clut_count, color_count);
		wxLogDebug(wxT("[ShapesChunk]         %d sequences"), high_level_shape_count);
		wxLogDebug(wxT("[ShapesChunk]         %d frames"), low_level_shape_count);
		wxLogDebug(wxT("[ShapesChunk]         %d bitmaps"), bitmap_count);
	}

	// load color tables
	for (i = 0; i < clut_count; i++) {
		ShapesColorTable	*color_table = new ShapesColorTable(IsVerbose());

		if (IsVerbose())
			wxLogDebug(wxT("[ShapesChunk] Loading colortable %d/%d"), i+1, clut_count);

		oldpos = buffer.Position();

		color_table->LoadObject(buffer, color_table_offset + i * color_count * SIZEOF_rgb_color_value, color_count);

		buffer.Position(oldpos);
		
		// we stop if an error occured
		if (!color_table->IsGood())
		{
			wxLogError(wxT("[ShapesChunk] Error loading color table %d... Dropped"), i);
			return buffer;
		}
		
		mColorTables.push_back(color_table);
	}
	
	// load bitmaps, decoding compressed ones
	buffer.Position(bitmap_offset_table_offset);
	for (i = 0; i < bitmap_count; i++) {
		offset = buffer.ReadLong();
		if (offset < SIZEOF_collection_definition || offset >= size) {
			wxLogError(wxT("[ShapesChunk] Invalid bitmap offset: this may not be a Marathon shapes file"));
			return buffer;
		}
		
		ShapesBitmap	*bitmap = new ShapesBitmap(IsVerbose());
		if (IsVerbose())
			wxLogDebug(wxT("[ShapesChunk] Loading bitmap %d/%d"), i+1, bitmap_count);
			
		oldpos = buffer.Position();
		
		bitmap->LoadObject(buffer, offset);
		
		buffer.Position(oldpos);
		
		// we stop if an error occured
		if (!bitmap->IsGood())
		{
			wxLogError(wxT("[ShapesDocument] Error loading bitmap %d... Dropped"), i);
			return buffer;
		}
		
		mBitmaps.push_back(bitmap);
	}

	// load sequences
	buffer.Position(high_level_shape_offset_table_offset);
	for (i = 0; i < high_level_shape_count; i++) {
		offset = buffer.ReadLong();
		if (offset < SIZEOF_collection_definition || offset >= size) {
			wxLogError(wxT("[ShapesChunk] Invalid sequence offset: this may not be a Marathon shapes file"));
			return buffer;
		}
		
		ShapesSequence	*sequence = new ShapesSequence(IsVerbose());
		if (IsVerbose())
			wxLogDebug(wxT("[ShapesChunk] Loading sequence %d/%d"), i+1, high_level_shape_count);
		
		oldpos = buffer.Position();
		
		sequence->LoadObject(buffer, offset);
		
		buffer.Position(oldpos);
		
		// we stop if an error occured
		if (!sequence->IsGood())
		{
			wxLogError(wxT("[ShapesDocument] Error loading sequence... Dropped"));
			return buffer;
		}
		
		mSequences.push_back(sequence);
	}
	
	// load frames
	buffer.Position(low_level_shape_offset_table_offset);
	for (i = 0; i < low_level_shape_count; i++) {
		offset = buffer.ReadLong();
		if (offset < SIZEOF_collection_definition || offset >= size) {
			wxLogError(wxT("[ShapesChunk] Invalid frame offset: this may not be a Marathon shapes file"));
			return buffer;
		}
		
		ShapesFrame	*frame = new ShapesFrame(IsVerbose());
		if (IsVerbose())
			wxLogDebug(wxT("[ShapesChunk] Loading frame %d/%d"), i+1, low_level_shape_count);

		oldpos = buffer.Position();
		
		frame->LoadObject(buffer, offset);
		
		buffer.Position(oldpos);
		// calculate scale factor from world_* fields and associated bitmap dimensions.
		// If this fails, default to collection global scale factor
		if (frame->BitmapIndex() >= 0 && frame->BitmapIndex() < (int)mBitmaps.size()) {
			int	bitmapWidth = mBitmaps[frame->BitmapIndex()]->Width();

			if (bitmapWidth > 0)
				frame->SetScaleFactor((frame->WorldRight() - frame->WorldLeft()) / bitmapWidth);
			else
				frame->SetScaleFactor(mPixelsToWorld);
		} else {
			frame->SetScaleFactor(mPixelsToWorld);
		}
		
		// store if correct
		if (!frame->IsGood())
		{
			wxLogError(wxT("[ShapesDocument] Error loading frame %d... Dropped"), i);
			return buffer;
		}
		
		mFrames.push_back(frame);
	}
	
	mGoodData = true;
	return buffer;
}

ShapesCollection::ShapesCollection(bool verbose): 
	ShapesElement(verbose)
{
	mChunks[0] = NULL;
	mChunks[1] = NULL;
}

ShapesCollection::~ShapesCollection(void)
{
	if (mChunks[0])
		delete mChunks[0];
	if (mChunks[1])
		delete mChunks[1];
}

bool ShapesCollection::Defined(unsigned int chunk) const
{
	if (chunk > COLL_VERSION_TRUECOLOR)
		return false;
	return mChunks[chunk] != NULL;
}

int ShapesCollection::Version(unsigned int chunk) const
{
	return (Defined(chunk) ? mChunks[chunk]->Version() : 0);
}

int ShapesCollection::Type(unsigned int chunk) const
{
	return (Defined(chunk) ? mChunks[chunk]->Type() : 0);
}

int ShapesCollection::Flags(unsigned int chunk) const
{
	return (Defined(chunk) ? mChunks[chunk]->Flags() : 0);
}

int ShapesCollection::ScaleFactor(unsigned int chunk) const
{
	return (Defined(chunk) ? mChunks[chunk]->ScaleFactor() : 0);
}

int ShapesCollection::ColorTableCount(unsigned int chunk) const
{
	return (Defined(chunk) ? mChunks[chunk]->ColorTableCount() : 0);
}

int ShapesCollection::BitmapCount(unsigned int chunk) const
{
	return (Defined(chunk) ? mChunks[chunk]->BitmapCount() : 0);
}

int ShapesCollection::FrameCount(unsigned int chunk) const
{
	return (Defined(chunk) ? mChunks[chunk]->FrameCount() : 0);
}

int ShapesCollection::SequenceCount(unsigned int chunk) const
{
	return (Defined(chunk) ? mChunks[chunk]->SequenceCount() : 0);
}

ShapesColorTable* ShapesCollection::GetColorTable(unsigned int chunk, unsigned int index) const
{
	return (Defined(chunk) ? mChunks[chunk]->GetColorTable(index) : NULL);
}

ShapesBitmap* ShapesCollection::GetBitmap(unsigned int chunk, unsigned int index) const
{
	return (Defined(chunk) ? mChunks[chunk]->GetBitmap(index) : NULL);
}

ShapesFrame* ShapesCollection::GetFrame(unsigned int chunk, unsigned int index) const
{
	return (Defined(chunk) ? mChunks[chunk]->GetFrame(index) : NULL);
}

ShapesSequence* ShapesCollection::GetSequence(unsigned int chunk, unsigned int index) const
{
	return (Defined(chunk) ? mChunks[chunk]->GetSequence(index) : NULL);
}

void ShapesCollection::InsertColorTable(ShapesColorTable *ct, unsigned int chunk)
{
	if (Defined(chunk))
		mChunks[chunk]->InsertColorTable(ct);
}

void ShapesCollection::DeleteColorTable(unsigned int chunk, unsigned int ct)
{
	if (Defined(chunk))
		mChunks[chunk]->DeleteColorTable(ct);
}

void ShapesCollection::InsertBitmap(ShapesBitmap *b, unsigned int chunk)
{
	if (Defined(chunk))
		mChunks[chunk]->InsertBitmap(b);
}

void ShapesCollection::DeleteBitmap(unsigned int chunk, unsigned int b)
{
	if (Defined(chunk))
		mChunks[chunk]->DeleteBitmap(b);
}

void ShapesCollection::InsertFrame(ShapesFrame *f, unsigned int chunk)
{
	if (Defined(chunk))
		mChunks[chunk]->InsertFrame(f);
}

void ShapesCollection::DeleteFrame(unsigned int chunk, unsigned int f)
{
	if (Defined(chunk))
		mChunks[chunk]->DeleteFrame(f);
}

void ShapesCollection::InsertSequence(ShapesSequence *s, unsigned int chunk)
{
	if (Defined(chunk))
		mChunks[chunk]->InsertSequence(s);
}

void ShapesCollection::DeleteSequence(unsigned int chunk, unsigned int s)
{
	if (Defined(chunk))
		mChunks[chunk]->DeleteSequence(s);
}

// calculate how much space a collection is going
// to take when encoded to its on-file format.
unsigned int ShapesCollection::SizeInFile(unsigned int chunk) const
{
	if (!Defined(chunk))
		return 0;

	unsigned int	size = 0;

	size += mChunks[chunk]->SizeInFile();
	return size;
}

#if wxUSE_STD_IOSTREAM
wxSTD ostream& ShapesCollection::SaveObject(wxSTD ostream& stream)
#else
wxOutputStream& ShapesCollection::SaveObject(wxOutputStream& stream)
#endif
{
	for (unsigned int i = 0; i < 2 ; i++) {
		if (Defined(i)) {
			BigEndianBuffer	chunkbuffer(mChunks[i]->SizeInFile());

			mChunks[i]->SaveObject(chunkbuffer);
#if wxUSE_STD_IOSTREAM
			stream.write((char *)chunkbuffer.Data(), chunkbuffer.Size());
#else
			stream.Write((char *)chunkbuffer.Data(), chunkbuffer.Size());
#endif
		}
	}
	return stream;
}

#if wxUSE_STD_IOSTREAM
wxSTD istream& ShapesCollection::LoadObject(wxSTD istream& stream)
#else
wxInputStream& ShapesCollection::LoadObject(wxInputStream& stream)
#endif
{
	BigEndianBuffer	coll_header(SIZEOF_collection_header);

#if wxUSE_STD_IOSTREAM
	stream.read((char *)coll_header.Data(), coll_header.Size());
#else
	stream.Read((char *)coll_header.Data(), coll_header.Size());
#endif

#if wxUSE_STD_IOSTREAM
	stream.seekg(0, std::ios::end);
	wxInt32 filesize = stream.tellg();
	stream.seekg(0, std::ios::beg);
#else
	wxInt32 filesize = stream.GetSize();
#endif
	
	long	offset8, length8,
			offset16, length16;
	
	mStatus = coll_header.ReadShort();
	mFlags = coll_header.ReadUShort();	
	offset8 = coll_header.ReadLong();
	length8 = coll_header.ReadLong();
	offset16 = coll_header.ReadLong();
	length16 = coll_header.ReadLong();
	
	if (offset8 < -1 || length8 < 0 || offset16 < -1 || length16 < 0) {
		wxLogError(wxT("[ShapesCollection] Invalid collection header: incorrect offsets"));
		wxLogError(wxT("[ShapesCollection] Invalid collection header: this may not be a Marathon shapes file"));
		return stream;
	}
	if ((offset8 + length8) > filesize) {
		wxLogError(wxT("[ShapesCollection] Invalid collection header: 8 bit offsets out-of-bounds"));
		wxLogError(wxT("[ShapesCollection] Invalid collection header: this may not be a Marathon shapes file"));
		return stream;
	}
	if ((offset16 + length16) > filesize) {
		wxLogError(wxT("[ShapesCollection] Invalid collection header: 16 bit offsets out-of-bounds"));
		wxLogError(wxT("[ShapesCollection] Invalid collection header: this may not be a Marathon shapes file"));
		return stream;
	}
	
	if (IsVerbose()) {
		wxLogDebug(wxT("[ShapesCollection]     Status: %d"), mStatus);
		wxLogDebug(wxT("[ShapesCollection]     Flags:  %d"), mFlags);
	}
	
	// is there the 8-bit version?
	if (offset8 != -1) {
		if (IsVerbose())
			wxLogDebug(wxT("[ShapesCollection]     8-bit chunk present"));
		
		BigEndianBuffer chunkbuffer(length8);
		
#if wxUSE_STD_IOSTREAM
		stream.seekg(offset8, std::ios::beg);
		stream.read((char *)chunkbuffer.Data(), chunkbuffer.Size());
#else
		stream.SeekI(offset8, wxFromStart);
		stream.Read((char *)chunkbuffer.Data(), chunkbuffer.Size());
#endif
		
		ShapesChunk	*pc = new ShapesChunk(IsVerbose());
		pc->LoadObject(chunkbuffer);
		
		if (!pc->IsGood())
		{
			wxLogError(wxT("[ShapesCollection] Error loading 8-bit chunk... Dropped"));
			return stream;
		}
		mChunks[0] = pc;
	}
	
	// is there the 16-bit version?
	if (offset16 != -1) {
		if (IsVerbose())
			wxLogDebug(wxT("[ShapesCollection]     16/32-bit chunk present"));
		
		BigEndianBuffer chunkbuffer(length16);
		
#if wxUSE_STD_IOSTREAM
		stream.seekg(offset16, std::ios::beg);
		stream.read((char *)chunkbuffer.Data(), chunkbuffer.Size());
#else
		stream.SeekI(offset16, wxFromStart);
		stream.Read((char *)chunkbuffer.Data(), chunkbuffer.Size());
#endif
		
		ShapesChunk	*pc = new ShapesChunk(IsVerbose());
		pc->LoadObject(chunkbuffer);
		
		if (!pc->IsGood())
		{
			wxLogError(wxT("[ShapesCollection] Error loading 16/32-bit chunk... Dropped"));
			return stream;
		}
		mChunks[1] = pc;
	}
	
	mGoodData = true;
	return stream;
}


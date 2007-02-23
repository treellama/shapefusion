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
#include "wx/datstrm.h"

ShapesElement::ShapesElement(bool verbose) : mVerboseLoading(verbose), mGoodData(false)
{}

ShapesElement::~ShapesElement(void)
{}

bool ShapesElement::IsGoodData()
{
	return mGoodData;
}

bool ShapesElement::IsVerbose()
{
	return mVerboseLoading;
}

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

}

#if wxUSE_STD_IOSTREAM
wxSTD istream& ShapesColor::LoadObject(wxSTD istream& stream)
#else
wxInputStream& ShapesColor::LoadObject(wxInputStream& stream)
#endif
{
#if !wxUSE_STD_IOSTREAM
	wxDataInputStream data_stream(stream);
	data_stream.BigEndianOrdered(true);
#endif
	unsigned char	flags;

	data_stream >> flags;
	data_stream >> mValue;
	data_stream >> mRed;
	data_stream >> mGreen;
	data_stream >> mBlue;
	
	mLuminescent = flags & SELF_LUMINESCENT_COLOR;
	
	mGoodData = true;
	return stream;
}

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

}

#if wxUSE_STD_IOSTREAM
wxSTD istream& ShapesColorTable::LoadObject(wxSTD istream& stream)
#else
wxInputStream& ShapesColorTable::LoadObject(wxInputStream& stream)
#endif
{
#if !wxUSE_STD_IOSTREAM
	wxDataInputStream data_stream(stream);
	data_stream.BigEndianOrdered(true);
#endif
	
	for (short i = 0; i < mColorCount; i++) {
		ShapesColor		*color = new ShapesColor(IsVerbose());
		
		color->LoadObject(stream);
		
		mColors.Append(color);
	}
	
	mGoodData = true;
	return stream;
}

ShapesSequence::ShapesSequence(wxInt32 offset, bool verbose) :
	ShapesElement(verbose), mOffset(offset)
{}

ShapesSequence::~ShapesSequence(void)
{}

#if wxUSE_STD_IOSTREAM
wxSTD ostream& ShapesSequence::SaveObject(wxSTD ostream& stream)
#else
wxOutputStream& ShapesSequence::SaveObject(wxOutputStream& stream)
#endif
{

}

#if wxUSE_STD_IOSTREAM
wxSTD istream& ShapesSequence::LoadObject(wxSTD istream& stream)
#else
wxInputStream& ShapesSequence::LoadObject(wxInputStream& stream)
#endif
{
#if !wxUSE_STD_IOSTREAM
	wxDataInputStream data_stream(stream);
	data_stream.BigEndianOrdered(true);
#endif
	
	stream.SeekI(mOffset, wxFromStart);
	data_stream >> mType;
	data_stream >> mFlags;
	
	// the name is a Mac Pascal string, not a C string (length,chars)
	/*data_stream >> namelen;
	
	for (int i = 0; i <= 32; i++)
		data_stream >> mName[i];
		
	if (namelen <= 32)
		mName[namelen] = '\0';*/
	data_stream >> mName;
	data_stream >> mNumberOfViews;
	data_stream >> mFramesPerView;
	data_stream >> mTicksPerFrame;
	data_stream >> mKeyFrame;
	data_stream >> mTransferMode;
	data_stream >> mTransferModePeriod;
	data_stream >> mFirstFrameSound;
	data_stream >> mKeyFrameSound;
	data_stream >> mLastFrameSound;
	data_stream >> mPixelsToWorld;
	data_stream >> mLoopFrame;
	
	stream.SeekI(28, wxFromCurrent);
	#warning tiennou : Unfinished !!!
/*	// load frame indexes
	int	n = CalcActualNumberOfViews(seq.number_of_views) * seq.frames_per_view;
	
	if (n > 0) {
		for (int k = 0; k < n; k++)
			seq.frame_indexes.push_back(chunkbuffer.ReadShort());
	}*/
	//chunkbuffer.ReadShort();	// terminating index (usually 0 but can be garbage)
	short j;
	data_stream >> j;
	
	mGoodData = true;
	return stream;
}

ShapesFrame::ShapesFrame(wxInt32 offset, bool verbose) :
	ShapesElement(verbose), mOffset(offset)
{}

ShapesFrame::~ShapesFrame(void)
{}

#if wxUSE_STD_IOSTREAM
wxSTD ostream& ShapesFrame::SaveObject(wxSTD ostream& stream)
#else
wxOutputStream& ShapesFrame::SaveObject(wxOutputStream& stream)
#endif
{

}

#if wxUSE_STD_IOSTREAM
wxSTD istream& ShapesFrame::LoadObject(wxSTD istream& stream)
#else
wxInputStream& ShapesFrame::LoadObject(wxInputStream& stream)
#endif
{
#if !wxUSE_STD_IOSTREAM
	wxDataInputStream data_stream(stream);
	data_stream.BigEndianOrdered(true);
#endif

	unsigned short	flags;
	wxInt32			mli_fixed;
	
	stream.SeekI(mOffset, wxFromStart);
	
	data_stream >> flags;

	mXmirror = flags & X_MIRROR;
	mYmirror = flags & Y_MIRROR;
	mKeypointObscured = flags & KEYPOINT_OBSCURED;
	
	data_stream >> mli_fixed;
	
	mMinimumLightIntensity = ((mli_fixed >> 16) & 0xffff) + (float)(mli_fixed & 0xffff) / 65536.0;	// convert fixed point [0,1] to double
	
	data_stream >> mBitmapIndex;
	data_stream >> mOriginX;
	data_stream >> mOriginY;
	data_stream >> mKeyX;
	data_stream >> mKeyY;
	data_stream >> mWorldLeft;
	data_stream >> mWorldRight;
	data_stream >> mWorldTop;
	data_stream >> mWorldBottom;
	data_stream >> mWorldX0;
	data_stream >> mWorldY0;
	// calculate scale factor from world_* fields and associated bitmap dimensions.
	// If this fails, default to collection global scale factor
	#warning tiennou : Unfinished !!!
	/*if (mBitmapIndex >= 0 && mBitmapIndex < (int)pc->bitmaps.size()) {
		int	bw = pc->bitmaps[frame.bitmap_index].width;
		
		if (bw > 0)
			mScaleFactor = (mWorldRight - mWorldLeft) / bw;
		else
			mScaleFactor = pc->pixels_to_world;
	} else {
		mScaleFactor = pc->pixels_to_world;
	}*/
}

ShapesBitmap::ShapesBitmap(wxInt32 offset, bool verbose) :
	ShapesElement(verbose), mOffset(offset)
{}

ShapesBitmap::~ShapesBitmap(void)
{}

#if wxUSE_STD_IOSTREAM
wxSTD ostream& ShapesBitmap::SaveObject(wxSTD ostream& stream)
#else
wxOutputStream& ShapesBitmap::SaveObject(wxOutputStream& stream)
#endif
{

}

#if wxUSE_STD_IOSTREAM
wxSTD istream& ShapesBitmap::LoadObject(wxSTD istream& stream)
#else
wxInputStream& ShapesBitmap::LoadObject(wxInputStream& stream)
#endif
{
#if !wxUSE_STD_IOSTREAM
	wxDataInputStream data_stream(stream);
	data_stream.BigEndianOrdered(true);
#endif
	
	stream.SeekI(mOffset, wxFromStart);
	
	data_stream >> mWidth;
	data_stream >> mHeight;
	data_stream >> mBytesPerRow;
	
	if (mWidth < 0) {
		wxLogError("[ShapesBitmap] Invalid bitmap width\n");
		return stream;
	}
	if (mHeight < 0) {
		wxLogError("[ShapesBitmap] Invalid bitmap height\n");
		return stream;
	}
	if (mBytesPerRow < -1) {
		wxLogError("[ShapesBitmap] Invalid bitmap bytes-per-row\n");
		return stream;
	}
	
	short	flags;
	
	data_stream >> flags;
	
	mColumnOrder = flags & COLUMN_ORDER;
	mTransparent = flags & TRANSPARENCY_ENABLED;
	
	data_stream >> mBitDepth;
	if (mBitDepth != 8) {
		wxLogError("[ShapesBitmap] Invalid bitmap depth %d\n", mBitDepth);
		return stream;
	}
	
	// skip unused fields and placeholders
	unsigned int	numscanlines = mColumnOrder ? mWidth : mHeight;
	
	stream.SeekI(stream.TellI() + 20 + numscanlines * 4);
	
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
					data_stream >> *dstp;
					dstp += mWidth;
				}
			}
		} else {
			// row order
			for (int i = 0; i < mWidth * mHeight; i++)
				data_stream >> mPixels[i];
			
			//			chunkbuffer.ReadBlock(btmp.width * btmp.height, btmp.pixels);
		}
	} else {
		// compressed bitmap (always column order)
		memset(mPixels, 0, mWidth * mHeight);
		for (int x = 0; x < mWidth; x++) {
			short			p0, p1;
			unsigned char	*dstp;
			
			data_stream >> p0;
			data_stream >> p1;
			//			p0 = chunkbuffer.ReadShort();
			//			p1 = chunkbuffer.ReadShort();
			dstp = mPixels + x + p0 * mWidth;
			while (p0 != p1) {
				data_stream >> *dstp;
				//				*dstp = chunkbuffer.ReadUChar();
				dstp += mWidth;
				p0++;
			}
		}
	}
	
	mGoodData = true;
	return stream;
}

ShapesChunk::ShapesChunk(wxInt32 offset, wxInt32 length, bool verbose): 
	ShapesElement(verbose), mOffset(offset), mLength(length)
{

}

ShapesChunk::~ShapesChunk(void)
{

}

#if wxUSE_STD_IOSTREAM
wxSTD ostream& ShapesChunk::SaveObject(wxSTD ostream& stream)
#else
wxOutputStream& ShapesChunk::SaveObject(wxOutputStream& stream)
#endif
{

}

#if wxUSE_STD_IOSTREAM
wxSTD istream& ShapesChunk::LoadObject(wxSTD istream& stream)
#else
wxInputStream& ShapesChunk::LoadObject(wxInputStream& stream)
#endif
{
#if !wxUSE_STD_IOSTREAM
	wxDataInputStream data_stream(stream);
	data_stream.BigEndianOrdered(true);
#endif

	stream.SeekI(mOffset, wxFromStart);
	short	color_count,
			clut_count,
			bitmap_count,
			high_level_shape_count,
			low_level_shape_count;
	wxInt32	color_table_offset,
			high_level_shape_offset_table_offset,
			low_level_shape_offset_table_offset,
			bitmap_offset_table_offset,
			size;
	
	data_stream >> mVersion;
	data_stream >> mType;
	data_stream >> mFlags;
	data_stream >> color_count;
	data_stream >> clut_count;
	data_stream >> color_table_offset;
	data_stream >> high_level_shape_count;
	data_stream >> high_level_shape_offset_table_offset;
	data_stream >> low_level_shape_count;
	data_stream >> low_level_shape_offset_table_offset;
	data_stream >> bitmap_count;
	data_stream >> bitmap_offset_table_offset;
	data_stream >> mPixels_to_world;
	data_stream >> size;
	
	// validate values
	if (mVersion != COLLECTION_VERSION) {
		wxLogError("[ShapesChunk] Unknown collection version %d\n", mVersion);
		return stream;
	}
	
	if (size != mLength) {
		wxLogError("[ShapesChunk] Chunk size mismatch (%d/%d): this may not be a Marathon shapes file\n", size, mLength);
		return stream;
	}
	if (color_table_offset < SIZEOF_collection_definition
		|| color_table_offset >= size
		|| high_level_shape_offset_table_offset < SIZEOF_collection_definition
		|| high_level_shape_offset_table_offset >= size
		|| low_level_shape_offset_table_offset < SIZEOF_collection_definition
		|| low_level_shape_offset_table_offset >= size
		|| bitmap_offset_table_offset < SIZEOF_collection_definition
		|| bitmap_offset_table_offset >= size) {
		wxLogError("[ShapesChunk] Invalid offsets in collection definition: this may not be a Marathon shapes file\n");
		return stream;
	}
	if (color_count < 0 || clut_count < 0 || high_level_shape_count < 0 || low_level_shape_count < 0 || bitmap_count < 0) {
		wxLogError("[ShapesChunk] Invalid object counts in collection definition: this may not be a Marathon shapes file\n");
		return stream;
	}

	if (IsVerbose()) {
		wxLogDebug("[ShapesChunk]         Version: %d\n", mVersion);
		wxLogDebug("[ShapesChunk]         Type:    %d\n", mType);
		wxLogDebug("[ShapesChunk]         Flags:   %d\n", mFlags);
		wxLogDebug("[ShapesChunk]         %d color tables, %d colors per table\n", clut_count, color_count);
		wxLogDebug("[ShapesChunk]         %d sequences\n", high_level_shape_count);
		wxLogDebug("[ShapesChunk]         %d frames\n", low_level_shape_count);
		wxLogDebug("[ShapesChunk]         %d bitmaps\n", bitmap_count);
	}
	
	
	// load color tables
	for (short i = 0; i < clut_count; i++) {
		ShapesColorTable	*color_table = new ShapesColorTable(color_count, color_table_offset, IsVerbose());
		
		color_table->LoadObject(stream);
	}
	
	// load bitmaps, decoding compressed ones
	stream.SeekI(bitmap_offset_table_offset, wxFromStart);
	
	for (short i = 0; i < bitmap_count; i++) {
		ShapesBitmap	*bitmap;
		
		wxInt32 offset = data_stream.Read32();
		
		if (offset < SIZEOF_collection_definition || offset >= size) {
			wxLogError("[ShapesChunk] Invalid bitmap offset: this may not be a Marathon shapes file\n");
			return stream;
		}
		
		bitmap = new ShapesBitmap(offset, IsVerbose());
		
		bitmap->LoadObject(stream);
		
		mBitmaps.Append(bitmap);
	}
	
	stream.SeekI(high_level_shape_offset_table_offset, wxFromStart);
	
	for (short i = 0; i < high_level_shape_count; i++) {
		ShapesSequence	*sequence;
		
		wxInt32 offset = data_stream.Read32();
		
		if (offset < SIZEOF_collection_definition || offset >= size) {
			wxLogError("[ShapesChunk] Invalid sequence offset: this may not be a Marathon shapes file\n");
			return stream;
		}
		
		sequence = new ShapesSequence(offset, IsVerbose());
		
		sequence->LoadObject(stream);
		
		mSequences.Append(sequence);
	}
	
	stream.SeekI(low_level_shape_offset_table_offset, wxFromStart);
	
	for (short i = 0; i < low_level_shape_count; i++) {
		ShapesFrame	*frame;
		
		wxInt32 offset = data_stream.Read32();
		
		if (offset < SIZEOF_collection_definition || offset >= size) {
			wxLogError("[ShapesChunk] Invalid frame offset: this may not be a Marathon shapes file\n");
			return stream;
		}
		
		frame = new ShapesFrame(offset, IsVerbose());
		
		frame->LoadObject(stream);
		
		mFrames.Append(frame);
	}
	
	mGoodData = true;
	return stream;
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

#if wxUSE_STD_IOSTREAM
wxSTD ostream& ShapesCollection::SaveObject(wxSTD ostream& stream)
#else
wxOutputStream& ShapesCollection::SaveObject(wxOutputStream& stream)
#endif
{

}

#if wxUSE_STD_IOSTREAM
wxSTD istream& ShapesCollection::LoadObject(wxSTD istream& stream)
#else
wxInputStream& ShapesCollection::LoadObject(wxInputStream& stream)
#endif
{
#if !wxUSE_STD_IOSTREAM
	wxDataInputStream data_stream(stream);
	data_stream.BigEndianOrdered(true);
#endif
	
	wxInt32 filesize = stream.GetSize();
	
	wxInt32		offset8, length8,
				offset16, length16;
		
	data_stream >> mStatus;
	data_stream >> mFlags;
	
	data_stream >> offset8;
	data_stream >> length8;
	
	data_stream >> offset16;
	data_stream >> length16;
	
	if (offset8 < -1 || length8 < 0 || offset16 < -1 || length16 < 0) {
		wxLogError("[ShapesCollection] Invalid collection header: incorrect offsets\n");
		wxLogError("[ShapesCollection] Invalid collection header: this may not be a Marathon shapes file\n");
		return stream;
	}
	
	if ((offset8 + length8) > filesize) {
		wxLogError("[ShapesCollection] Invalid collection header: 8 bit offsets out-of-bounds\n");
		wxLogError("[ShapesCollection] Invalid collection header: this may not be a Marathon shapes file\n");
		return stream;
	}
	
	if ((offset16 + length16) > filesize) {
		wxLogError("[ShapesCollection] Invalid collection header: 16 bit offsets out-of-bounds\n");
		wxLogError("[ShapesCollection] Invalid collection header: this may not be a Marathon shapes file\n");
		return stream;
	}
	
	if (IsVerbose()) {
		wxLogDebug("[ShapesCollection]     Status: %d\n", mStatus);
		wxLogDebug("[ShapesCollection]     Flags:  %d\n", mFlags);
	}
	
	// is there the 8-bit version?
	if (offset8 != -1)
	{
		if (IsVerbose())
			wxLogDebug("[ShapesCollection]     8-bit chunk present\n");
		
		ShapesChunk	*pc = new ShapesChunk(offset8, length8);
		pc->LoadObject(stream);
		
		mChunks[0] = pc;
	}
	
	// is there the 16-bit version?
	if (offset16 != -1) {
		if (IsVerbose())
			wxLogDebug("[ShapesCollection]     16/32-bit chunk present\n");
		
		ShapesChunk	*pc = new ShapesChunk(offset16, length16);
		pc->LoadObject(stream);
		
		mChunks[1] = pc;
	}
	
	mGoodData = true;
	return stream;
}
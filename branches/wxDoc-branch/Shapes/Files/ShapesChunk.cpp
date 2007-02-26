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

#include "ShapesChunk.h"

// chunk types. Bitmap encoding seems to depend on this setting
enum {
	_unused_collection = 0,	// plain
	_wall_collection,		// plain
	_object_collection,		// RLE
	_interface_collection,	// plain
	_scenery_collection		// RLE
};

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
	BigEndianBuffer	chunkbuffer(mLength);

	wxInt32 oldpos;
	wxInt32 offset;
	short i;
	
	stream.SeekI(mOffset, wxFromStart);
	stream.Read((char *)chunkbuffer.Data(), chunkbuffer.Size());
	
	short	color_count,
			clut_count,
			bitmap_count,
			high_level_shape_count,
			low_level_shape_count;
	long	color_table_offset,
			high_level_shape_offset_table_offset,
			low_level_shape_offset_table_offset,
			bitmap_offset_table_offset,
			size;
	
	mVersion = chunkbuffer.ReadShort();
	mType = chunkbuffer.ReadShort();
	mFlags = chunkbuffer.ReadUShort();
	color_count = chunkbuffer.ReadShort();
	clut_count = chunkbuffer.ReadShort();
	color_table_offset = chunkbuffer.ReadLong();
	high_level_shape_count = chunkbuffer.ReadShort();
	high_level_shape_offset_table_offset = chunkbuffer.ReadLong();
	low_level_shape_count = chunkbuffer.ReadShort();
	low_level_shape_offset_table_offset = chunkbuffer.ReadLong();
	bitmap_count = chunkbuffer.ReadShort();
	bitmap_offset_table_offset = chunkbuffer.ReadLong();
	mPixelsToWorld = chunkbuffer.ReadShort();
	size = chunkbuffer.ReadLong();
	
	// validate values
	if (mVersion != COLLECTION_VERSION) {
		wxLogError("[ShapesChunk] Unknown collection version %d", mVersion);
		return stream;
	}
	
	if (size != mLength) {
		wxLogError("[ShapesChunk] Chunk size mismatch (%d/%d): this may not be a Marathon shapes file", size, mLength);
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
		wxLogError("[ShapesChunk] Invalid offsets in collection definition: this may not be a Marathon shapes file");
		return stream;
	}
	if (color_count < 0 || clut_count < 0 || high_level_shape_count < 0 || low_level_shape_count < 0 || bitmap_count < 0) {
		wxLogError("[ShapesChunk] Invalid object counts in collection definition: this may not be a Marathon shapes file");
		return stream;
	}

	if (IsVerbose()) {
		wxLogDebug("[ShapesChunk]         Version: %d", mVersion);
		wxLogDebug("[ShapesChunk]         Type:    %d", mType);
		wxLogDebug("[ShapesChunk]         Flags:   %d", mFlags);
		wxLogDebug("[ShapesChunk]         %d color tables, %d colors per table", clut_count, color_count);
		wxLogDebug("[ShapesChunk]         %d sequences", high_level_shape_count);
		wxLogDebug("[ShapesChunk]         %d frames", low_level_shape_count);
		wxLogDebug("[ShapesChunk]         %d bitmaps", bitmap_count);
	}
	
	
	// load color tables
	for (i = 0; i < clut_count; i++) {
		ShapesColorTable	*color_table = new ShapesColorTable(color_count, color_table_offset, IsVerbose());
		if (IsVerbose())
			wxLogDebug("[ShapesChunk] Loading colortable %d/%d", i+1, clut_count);
			
		oldpos = chunkbuffer.Position();
		
		color_table->LoadObject(chunkbuffer);
		
		chunkbuffer.Position(oldpos);
		
		//store if correct
		if (color_table->IsGood())
			mColortables.Append(color_table);
		else
			wxLogError("[ShapesChunk] Error loading colortable... Dropped");
	}
	
	
/*	wxInt32List offsets;
	for (short i = 0; i < bitmap_count; i++) {
		
		wxInt32 *offset = new wxInt32(data_stream.Read32());
		
		if (*offset < SIZEOF_collection_definition || *offset >= size) {
			wxLogError("[ShapesChunk] Invalid bitmap offset: this may not be a Marathon shapes file");
			return stream;
		}
		
		offsets.Append(offset);
	}*/
	
	// load bitmaps, decoding compressed ones
	chunkbuffer.Position(bitmap_offset_table_offset);
	
	for (i = 0; i < bitmap_count; i++) {
		ShapesBitmap	*bitmap;
		
		offset = chunkbuffer.ReadLong();
		
		if (offset < SIZEOF_collection_definition || offset >= size) {
			wxLogError("[ShapesChunk] Invalid bitmap offset: this may not be a Marathon shapes file");
			return stream;
		}
		
		bitmap = new ShapesBitmap(offset, IsVerbose());
		if (IsVerbose())
			wxLogDebug("[ShapesChunk] Loading bitmap %d/%d", i+1, bitmap_count);
			
		oldpos = chunkbuffer.Position();
		
		bitmap->LoadObject(chunkbuffer);
		
		chunkbuffer.Position(oldpos);
		// store if correct
		if (bitmap->IsGood())
			mBitmaps.Append(bitmap);
		else
			wxLogError("[ShapesDocument] Error loading bitmap... Dropped");
		
	}
	
	chunkbuffer.Position(high_level_shape_offset_table_offset);
	
	for (i = 0; i < high_level_shape_count; i++) {
		ShapesSequence	*sequence;
		
		offset = chunkbuffer.ReadLong();
		
		if (offset < SIZEOF_collection_definition || offset >= size) {
			wxLogError("[ShapesChunk] Invalid sequence offset: this may not be a Marathon shapes file");
			return stream;
		}
		
		sequence = new ShapesSequence(offset, IsVerbose());
		if (IsVerbose())
			wxLogDebug("[ShapesChunk] Loading sequence %d/%d", i+1, high_level_shape_count);
		
		oldpos = chunkbuffer.Position();
		
		sequence->LoadObject(chunkbuffer);
		
		chunkbuffer.Position(oldpos);
		
		// store if correct
		if (sequence->IsGood())
			mSequences.Append(sequence);
		else
			wxLogError("[ShapesDocument] Error loading sequence... Dropped");
		
	}
	
	chunkbuffer.Position(low_level_shape_offset_table_offset);
	
	for (i = 0; i < low_level_shape_count; i++) {
		ShapesFrame	*frame;
		
		offset = chunkbuffer.ReadLong();
		
		if (offset < SIZEOF_collection_definition || offset >= size) {
			wxLogError("[ShapesChunk] Invalid frame offset: this may not be a Marathon shapes file");
			return stream;
		}
		
		frame = new ShapesFrame(offset, IsVerbose());
		if (IsVerbose())
			wxLogDebug("[ShapesChunk] Loading frame %d/%d", i+1, low_level_shape_count);
		
		oldpos = chunkbuffer.Position();
		
		frame->LoadObject(chunkbuffer);
		
		chunkbuffer.Position(oldpos);
		
		frame->CalculateScaleFactor(&mBitmaps, mPixelsToWorld);
		
		// store if correct
		if (frame->IsGood())
			mFrames.Append(frame);
		else
			wxLogError("[ShapesDocument] Error loading frame... Dropped");
		
	}
	
	mGoodData = true;
	return stream;
}


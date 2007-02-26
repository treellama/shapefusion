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
#include "BigEndianBuffer.h"
#include "wx/datstrm.h"
#include "wx/wfstream.h"

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


// calculate how much space a collection is going
// to take when encoded to its on-file format.
unsigned int ShapesCollection::CollectionSizeInFile(unsigned int version) const
{
	if (version > COLL_VERSION_TRUECOLOR)
		return 0;
	if (!CollectionDefined(version))
		return 0;

	ShapesChunk		*chunkp = collections[id].chunks[version];
	unsigned int	size = SIZEOF_collection_definition,
					bitmap_count = chunkp->bitmaps.size(),
					frame_count = chunkp->frames.size(),
					sequence_count = chunkp->sequences.size();

	// add contribute of color tables
	if (chunkp->ctabs.size() > 0)
		size += SIZEOF_rgb_color_value * chunkp->ctabs.size() * chunkp->ctabs[0].colors.size();
	// add contribute of bitmap offset table
	size += 4 * bitmap_count;
	// add contribute of bitmaps (we'll have to compress them if necessary)
	size += SIZEOF_bitmap_definition * bitmap_count;
	for (unsigned int i = 0; i < bitmap_count; i++) {
		ShpBitmap	*bitmapp = &chunkp->bitmaps[i];

		// scanline pointer placeholder
		if (bitmapp->column_order)
			size += 4 * bitmapp->width;
		else
			size += 4 * bitmapp->height;
		if (bitmapp->bytes_per_row == -1) {
			// compressed
			size += bitmapp->width * 4;
			for (int x = 0; x < bitmapp->width; x++) {
				unsigned char	*pp = bitmapp->pixels + x;
				int				p0 = -1,
								p1;

				for (int y = 0; y < bitmapp->height; y++) {
					if (*pp != 0) {
						p0 = y;
						break;
					}
					pp += bitmapp->width;
				}
				if (p0 == -1)
					continue;	// no opaque pixels in this column
				p1 = p0;
				pp = bitmapp->pixels + x + bitmapp->width * (bitmapp->height - 1);
				for (int y = bitmapp->height - 1; y >= 0; y--) {
					if (*pp != 0) {
						p1 = y;
						break;
					}
					pp -= bitmapp->width;
				}
				size += p1 - p0 + 1;
			}
		} else {
			// plain
			size += bitmapp->width * bitmapp->height;
		}
	}
	// add contribute of frame offset table
	size += 4 * frame_count;
	// add contribute of frame definitions
	size += SIZEOF_low_level_shape_definition * frame_count;
	// add contribute of sequence offset table
	size += 4 * sequence_count;
	// add contribute of sequence definitions (and following frame indexes)
	size += SIZEOF_high_level_shape_definition * sequence_count;
	for (unsigned int i = 0; i < sequence_count; i++)
		size += 2 * (chunkp->sequences[i].frame_indexes.size() + 1);

	return size;
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
	BigEndianBuffer	coll_header(SIZEOF_collection_header);

	stream.Read((char *)coll_header.Data(), coll_header.Size());
	
	wxInt32 filesize = stream.GetSize();
	
	long		offset8, length8,
				offset16, length16;
	
	mStatus = coll_header.ReadShort();
	mFlags = coll_header.ReadUShort();
	
	offset8 = coll_header.ReadLong();
	length8 = coll_header.ReadLong();
	
	offset16 = coll_header.ReadLong();
	length16 = coll_header.ReadLong();
	
	if (offset8 < -1 || length8 < 0 || offset16 < -1 || length16 < 0) {
		wxLogError("[ShapesCollection] Invalid collection header: incorrect offsets");
		wxLogError("[ShapesCollection] Invalid collection header: this may not be a Marathon shapes file");
		return stream;
	}
	
	if ((offset8 + length8) > filesize) {
		wxLogError("[ShapesCollection] Invalid collection header: 8 bit offsets out-of-bounds");
		wxLogError("[ShapesCollection] Invalid collection header: this may not be a Marathon shapes file");
		return stream;
	}
	
	if ((offset16 + length16) > filesize) {
		wxLogError("[ShapesCollection] Invalid collection header: 16 bit offsets out-of-bounds");
		wxLogError("[ShapesCollection] Invalid collection header: this may not be a Marathon shapes file");
		return stream;
	}
	
	if (IsVerbose()) {
		wxLogDebug("[ShapesCollection]     Status: %d", mStatus);
		wxLogDebug("[ShapesCollection]     Flags:  %d", mFlags);
	}
	
	// is there the 8-bit version?
	if (offset8 != -1)
	{
		if (IsVerbose())
			wxLogDebug("[ShapesCollection]     8-bit chunk present");
		
		ShapesChunk	*pc = new ShapesChunk(offset8, length8);
		pc->LoadObject(stream);
		
		if (pc->IsGood())
			mChunks[0] = pc;
		else
			wxLogError("[ShapesCollection] Error loading 8-bit chunk... Dropped");
	}
	
	// is there the 16-bit version?
	if (offset16 != -1) {
		if (IsVerbose())
			wxLogDebug("[ShapesCollection]     16/32-bit chunk present");
		
		ShapesChunk	*pc = new ShapesChunk(offset16, length16);
		pc->LoadObject(stream);
		
		if (pc->IsGood())
			mChunks[1] = pc;
		else
			wxLogError("[ShapesCollection] Error loading 16/32-bit chunk... Dropped");
	}
	
	mGoodData = true;
	return stream;
}
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

#ifndef SHAPESELEMENT_H
#define SHAPESELEMENT_H

#include "ShapeFusionLists.h"
#include "BigEndianBuffer.h"
#include "ShapesChunk.h"
#include "ShapesColorTable.h"
#include "ShapesBitmap.h"
#include "ShapesFrame.h"
#include "ShapesSequence.h"

#define COLLECTIONS_PER_FILE				32
#define COLLECTION_VERSION					3
// on-file struct sizes
#define SIZEOF_collection_header			32
#define SIZEOF_collection_definition		544

// internal-use utility constants
enum {
	COLL_VERSION_8BIT = 0,
	COLL_VERSION_TRUECOLOR
};

// a Shapes collection
class ShapesCollection : public ShapesElement
{
private:
	short			mStatus;
	unsigned short	mFlags;
	ShapesChunk		*mChunks[2];	// chunks for 8-bit and truecolor game
	
public:
	ShapesCollection(bool verbose = false);
	~ShapesCollection(void);
	
	int Status(void) const;
	int Flags(void) const;
	
	bool Defined(unsigned int chunk) const;
	int Version(unsigned int chunk) const;
	int Type(unsigned int chunk) const;
	int Flags(unsigned int chunk) const:
	int ScaleFactor(unsigned int chunk) const;
	int BitmapCount(unsigned int chunk) const;
	int FrameCount(unsigned int chunk) const;
	int SequenceCount(unsigned int chunk) const;
	ShapesColorTable* GetColorTable(unsigned int chunk, unsigned int index) const;
	ShapesBitmap* GetBitmap(unsigned int chunk, unsigned int index) const;
	ShapesFrame* GetFrame(unsigned int chunk, unsigned int index) const;
	ShapesSequence* GetSequence(unsigned int chunk, unsigned int index) const;
	
	unsigned int ShapesCollection::CollectionSizeInFile(unsigned int chunk) const;
	
#if wxUSE_STD_IOSTREAM
    wxSTD ostream& SaveObject(wxSTD ostream& stream);
    wxSTD istream& LoadObject(wxSTD istream& stream);
#else
    wxOutputStream& SaveObject(wxOutputStream& stream);
    wxInputStream& LoadObject(wxInputStream& stream);
#endif
};

WX_DECLARE_LIST(ShapesCollection, ShapesCollectionList);

#endif
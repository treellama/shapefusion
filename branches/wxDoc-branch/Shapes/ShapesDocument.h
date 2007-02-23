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

//
// Shapes
// Shapes class representing what's inside a Marathon shapes file.
// Note that, for keeping things simple and providing compatibility,
// the class doesn't store everything using the original on-disk format.
// For example, flags are expanded to bool fields and fixed point values
// are converted to doubles. Every bitmap is converted to a simple block
// of width*height pixels, regardless of its compression and pixel order.
// Conversions take place only when actually loading and writing files.
//

#ifndef SHAPESDOCUMENT_H
#define SHAPESDOCUMENT_H

#include "wx/docview.h"
#include "ShapesElements.h"

// class representing the contents of a Marathon shapes file
#if 0
class Shapes {
private:
	vector<ShpCollection>	collections;
	bool					good_data;
	bool					verbose_file_load;

	int LoadShapesChunk(unsigned int id, ShpChunk *pc, std::ifstream &is, long offset, long expected_size);

public:
	Shapes(void);
	~Shapes(void);

	// file I/O
	Shapes(std::string path);
	int WriteToFile(std::string path);
	bool GoodData(void) const;
	// collection data access
	int CollGlobalStatus(unsigned int id) const;
	unsigned int CollGlobalFlags(unsigned int id) const;
	bool CollDefined(unsigned int id, unsigned int version) const;
	int CollVersion(unsigned int id, unsigned int version) const;
	int CollType(unsigned int id, unsigned int version) const;
	unsigned int CollFlags(unsigned int id, unsigned int version) const;
	int CollScaleFactor(unsigned int id, unsigned int version) const;
	unsigned int CollBitmapCount(unsigned int id, unsigned int version) const;
	unsigned int CollCTCount(unsigned int id, unsigned int version) const;
	unsigned int CollFrameCount(unsigned int id, unsigned int version) const;
	unsigned int CollSeqCount(unsigned int id, unsigned int version) const;
	ShpColorTable *GetColorTable(unsigned int coll, unsigned int version, unsigned int ct) const;
	ShpBitmap *GetBitmap(unsigned int coll, unsigned int version, unsigned int bitmap) const;
	ShpFrame *GetFrame(unsigned int coll, unsigned int version, unsigned int frame) const;
	ShpSequence *GetSeq(unsigned int coll, unsigned int version, unsigned int seq) const;
	// collection alteration
	void InsertColorTable(ShpColorTable ct, unsigned int coll, unsigned int version);
	void DeleteColorTable(unsigned int coll, unsigned int version, unsigned int ct);
	void InsertBitmap(ShpBitmap b, unsigned int coll, unsigned int version);
	void DeleteBitmap(unsigned int coll, unsigned int version, unsigned int b);
	void InsertFrame(ShpFrame f, unsigned int coll, unsigned int version);
	void DeleteFrame(unsigned int coll, unsigned int version, unsigned int f);
	void InsertSequence(ShpSequence s, unsigned int coll, unsigned int version);
	void DeleteSequence(unsigned int coll, unsigned int version, unsigned int s);
	// utilities
	unsigned int CollectionSizeInFile(unsigned int id, unsigned int version) const;
};
#endif

class ShapesDocument: public wxDocument
{
    DECLARE_DYNAMIC_CLASS(ShapesDocument)
private:
	ShapesCollectionList	mCollections;
	bool					mGoodData;
	bool					mVerboseLoading;
public:

#if wxUSE_STD_IOSTREAM
    wxSTD ostream& SaveObject(wxSTD ostream& text_stream);
    wxSTD istream& LoadObject(wxSTD istream& text_stream);
#else
    wxOutputStream& SaveObject(wxOutputStream& stream);
    wxInputStream& LoadObject(wxInputStream& stream);
#endif

    ShapesDocument(void);
    ~ShapesDocument(void);
};

int CalcActualNumberOfViews(int t);

#endif

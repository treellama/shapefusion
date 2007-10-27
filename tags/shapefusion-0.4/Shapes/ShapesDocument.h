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
class ShapesDocument: public wxDocument, public ShapesElement
{
    DECLARE_DYNAMIC_CLASS(ShapesDocument)
private:
	vector<ShapesCollection*>	mCollections;
public:

	unsigned int CollectionCount(void) const;
	// collection data access
	int CollectionStatus(unsigned int id);
	unsigned int CollectionFlags(unsigned int id) const;
	bool CollectionDefined(unsigned int id, unsigned int chunk) const;
	int CollectionVersion(unsigned int id, unsigned int chunk) const;
	int CollectionType(unsigned int id, unsigned int chunk) const;
	unsigned int CollectionFlags(unsigned int id, unsigned int chunk) const;
	int CollectionScaleFactor(unsigned int id, unsigned int chunk) const;
	unsigned int CollectionBitmapCount(unsigned int id, unsigned int chunk) const;
	unsigned int CollectionColorTableCount(unsigned int id, unsigned int chunk) const;
	unsigned int CollectionFrameCount(unsigned int id, unsigned int chunk) const;
	unsigned int CollectionSequenceCount(unsigned int id, unsigned int chunk) const;
	ShapesColorTable *GetColorTable(unsigned int coll, unsigned int chunk, unsigned int ct) const;
	ShapesBitmap *GetBitmap(unsigned int coll, unsigned int chunk, unsigned int bitmap) const;
	ShapesFrame *GetFrame(unsigned int coll, unsigned int chunk, unsigned int frame) const;
	ShapesSequence *GetSequence(unsigned int coll, unsigned int chunk, unsigned int seq) const;
	// collection alteration
	void InsertColorTable(ShapesColorTable *ct, unsigned int coll, unsigned int chunk);
	void DeleteColorTable(unsigned int coll, unsigned int chunk, unsigned int ct);
	void InsertBitmap(ShapesBitmap *b, unsigned int coll, unsigned int chunk);
	void DeleteBitmap(unsigned int coll, unsigned int chunk, unsigned int b);
	void InsertFrame(ShapesFrame *f, unsigned int coll, unsigned int chunk);
	void DeleteFrame(unsigned int coll, unsigned int chunk, unsigned int f);
	void InsertSequence(ShapesSequence *s, unsigned int coll, unsigned int chunk);
	void DeleteSequence(unsigned int coll, unsigned int chunk, unsigned int s);
	
	bool DoOpenDocument(const wxString& file);
	
#if wxUSE_STD_IOSTREAM
    wxSTD ostream& SaveObject(wxSTD ostream& stream);
    wxSTD istream& LoadObject(wxSTD istream& stream);
#else
    wxOutputStream& SaveObject(wxOutputStream& stream);
    wxInputStream& LoadObject(wxInputStream& stream);
#endif

    ShapesDocument(void);
    ~ShapesDocument(void);
};

#endif

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

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "wx/datstrm.h"
#include "ShapesDocument.h"

IMPLEMENT_DYNAMIC_CLASS(ShapesDocument, wxDocument)

ShapesDocument::ShapesDocument():
	wxDocument(), mGoodData(false), mVerboseLoading(false)
{

}

ShapesDocument::~ShapesDocument()
{

}

// collection data access
int ShapesDocument::CollectionStatus(unsigned int id)
{
	return mCollections[id]->Status();
}

unsigned int ShapesDocument::CollectionFlags(unsigned int id) const
{
	return mCollections[id]->Flags();
}

bool ShapesDocument::CollectionDefined(unsigned int id, unsigned int chunk) const
{
	return mCollections[id]->Defined(chunk);
}

int ShapesDocument::CollectionVersion(unsigned int id, unsigned int chunk) const
{
	return mCollections[id]->Version(chunk);
}

int ShapesDocument::CollectionType(unsigned int id, unsigned int chunk) const
{
	return mCollections[id]->Type(chunk);
}

unsigned int ShapesDocument::CollectionFlags(unsigned int id, unsigned int chunk) const
{
	return mCollections[id]->Flags(chunk);
}

int ShapesDocument::CollectionScaleFactor(unsigned int id, unsigned int chunk) const
{
	return mCollections[id]->ScaleFactor(chunk);
}

unsigned int ShapesDocument::CollectionBitmapCount(unsigned int id, unsigned int chunk) const
{
	return mCollections[id]->BitmapCount(chunk);
}

unsigned int ShapesDocument::CollectionColorTableCount(unsigned int id, unsigned int chunk) const
{
	return mCollections[id]->ColorTableCount(chunk);
}

unsigned int ShapesDocument::CollectionFrameCount(unsigned int id, unsigned int chunk) const
{
	return mCollections[id]->FrameCount(chunk);
}

unsigned int ShapesDocument::CollectionSequenceCount(unsigned int id, unsigned int chunk) const
{
	return mCollections[id]->SequenceCount(chunk);
}

ShapesColorTable *ShapesDocument::GetColorTable(unsigned int coll, unsigned int chunk, unsigned int ct) const
{
	return mCollections[coll]->GetColorTable(chunk, ct);
}

ShapesBitmap *ShapesDocument::GetBitmap(unsigned int coll, unsigned int chunk, unsigned int bitmap) const
{
	return mCollections[coll]->GetBitmap(chunk, bitmap);
}

ShapesFrame *ShapesDocument::GetFrame(unsigned int coll, unsigned int chunk, unsigned int frame) const
{
	return mCollections[coll]->GetFrame(chunk, frame);
}

ShapesSequence *ShapesDocument::GetSequence(unsigned int coll, unsigned int chunk, unsigned int seq) const
{
	return mCollections[coll]->GetSequence(chunk, seq);
}

// collection alteration
void ShapesDocument::InsertColorTable(ShapesColorTable *ct, unsigned int coll, unsigned int chunk)
{
	mCollections[coll]->InsertColorTable(ct, chunk);
}

void ShapesDocument::DeleteColorTable(unsigned int coll, unsigned int chunk, unsigned int ct)
{
	mCollections[coll]->DeleteColorTable(chunk, ct);
}

void ShapesDocument::InsertBitmap(ShapesBitmap *b, unsigned int coll, unsigned int chunk)
{
	mCollections[coll]->InsertBitmap(b, chunk);
}

void ShapesDocument::DeleteBitmap(unsigned int coll, unsigned int chunk, unsigned int b)
{
	mCollections[coll]->DeleteBitmap(chunk, b);
}

void ShapesDocument::InsertFrame(ShapesFrame *f, unsigned int coll, unsigned int chunk)
{
	mCollections[coll]->InsertFrame(f, chunk);
}

void ShapesDocument::DeleteFrame(unsigned int coll, unsigned int chunk, unsigned int f)
{
	mCollections[coll]->DeleteFrame(chunk, f);
}

void ShapesDocument::InsertSequence(ShapesSequence *s, unsigned int coll, unsigned int chunk)
{
	mCollections[coll]->InsertSequence(s, chunk);
}

void ShapesDocument::DeleteSequence(unsigned int coll, unsigned int chunk, unsigned int s)
{
	mCollections[coll]->DeleteSequence(chunk, s);
}

#if wxUSE_STD_IOSTREAM
wxSTD ostream& ShapesDocument::SaveObject(wxSTD ostream& stream)
#else
wxOutputStream& ShapesDocument::SaveObject(wxOutputStream& stream)
#endif
{	// calculate collection sizes
	unsigned int	coll_size;
	
	// compose and write the collection header block
	{
		BigEndianBuffer	raw_headers(SIZEOF_collection_header * COLLECTIONS_PER_FILE);
		long			running_offset = SIZEOF_collection_header * COLLECTIONS_PER_FILE;
		
		for (unsigned int i = 0; i < COLLECTIONS_PER_FILE; i++) {
			ShapesCollection	coll = mCollections[i];
			raw_headers.WriteShort(coll.Status());
			raw_headers.WriteUShort(coll.Flags());
			// 8-bit version
			if (coll.Defined(COLL_VERSION_8BIT)) {
				coll_size = coll.SizeInFile(COLL_VERSION_8BIT);
				
				raw_headers.WriteLong(running_offset);
				raw_headers.WriteLong(coll_size);
				running_offset += coll_size;
			} else {
				raw_headers.WriteLong(-1);
				raw_headers.WriteLong(0);
			}
			// truecolor version
			if (coll.Defined(COLL_VERSION_TRUECOLOR)) {
				coll_size = coll.SizeInFile(COLL_VERSION_TRUECOLOR);
				
				raw_headers.WriteLong(running_offset);
				raw_headers.WriteLong(coll_size);
				running_offset += coll_size;
			} else {
				raw_headers.WriteLong(-1);
				raw_headers.WriteLong(0);
			}
			raw_headers.WriteZeroes(12);
		}
#if wxUSE_STD_IOSTREAM
		stream.write((char *)raw_headers.Data(), raw_headers.Size());
#else
		stream.Write((char *)raw_headers.Data(), raw_headers.Size());
#endif
	}
	
	for (unsigned int i = 0; i < COLLECTIONS_PER_FILE; i++) {
		mCollections[i]->SaveObject(stream);
	}
	
	return stream;
}

#if wxUSE_STD_IOSTREAM
wxSTD istream& ShapesDocument::LoadObject(wxSTD istream& stream)
#else
wxInputStream& ShapesDocument::LoadObject(wxInputStream& stream)
#endif
{
	// first check file size to immediately rule out invalid stuff
#if wxUSE_STD_IOSTREAM
	stream.seekg(0, std::ios::end);
	wxInt32 filesize = stream.tellg();
	stream.seekg(0, std::ios::beg);
#else
	wxInt32 filesize = stream.GetSize();
#endif
	if (filesize < COLLECTIONS_PER_FILE * SIZEOF_collection_header) {
		wxLogError(wxT("[ShapesDocument] File too small to be a Marathon shapes file"));
		mGoodData = false;
		return stream;
	}
	// load the collections
	for (unsigned int i = 0; i < COLLECTIONS_PER_FILE; i++) {
		ShapesCollection	*c = new ShapesCollection(mVerboseLoading);

		if (mVerboseLoading)
			wxLogDebug(wxT("[ShapesDocument] Loading collection %d"), i);

#if wxUSE_STD_IOSTREAM
		stream.seekg(i * SIZEOF_collection_header, std::ios::beg);
#else
		stream.SeekI(i * SIZEOF_collection_header);
#endif
		c->LoadObject(stream);

		// store if correct
		if (c->IsGood())
			mCollections.push_back(c);
		else
			wxLogError(wxT("[ShapesDocument] Error loading collection... Dropped"));
	}
	mGoodData = true;
	
	return stream;
}

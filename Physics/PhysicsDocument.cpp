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

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if wxUSE_STD_IOSTREAM
    #include "wx/ioswrap.h"
#else
    #include "wx/txtstrm.h"
#endif

#include "../BigEndianBuffer.h"
#include "PhysicsDocument.h"

#define FOUR_CHARS_TO_INT(a,b,c,d) (((unsigned int)(a) << 24) | ((unsigned int)(b) << 16) | ((unsigned int)(c) << 8) | (unsigned int)(d))

IMPLEMENT_DYNAMIC_CLASS(PhysicsDocument, wxDocument)

PhysicsDocument::PhysicsDocument() : wxDocument(), PhysicsElement(true)
{
	mConstants.resize(2);
}

PhysicsDocument::~PhysicsDocument()
{
}

bool PhysicsDocument::DoOpenDocument(const wxString& file)
{
	bool wxOpen = wxDocument::DoOpenDocument(file);

	if (!(wxOpen && mGoodData)) {
		wxLogError(wxT("[PhysicsDocument] There was an error while opening, see log"));
		return false;
	}
	return true;
}

#if wxUSE_STD_IOSTREAM
wxSTD istream& PhysicsDocument::LoadObject(wxSTD istream& stream)
#else
wxInputStream& PhysicsDocument::LoadObject(wxInputStream& stream)
#endif
{
	// validate the wad header, load the first wad
	BigEndianBuffer wad_header(128);
#if wxUSE_STD_IOSTREAM
	stream.read((char *)wad_header.Data(), wad_header.Size());
#else
	stream.Read((char *)wad_header.Data(), wad_header.Size());
#endif

	int version = wad_header.ReadShort();
	int data_version = wad_header.ReadShort();

	if (!(version == 1 || version == 2) || data_version != 0) {
		wxLogError(wxT("[PhysicsDocument] Error loading: Incorrect version/data version (%i/%i)"), version, data_version);
		return stream;
	}

	// skip to the directory offset and wad count
	wad_header.Position(72);
	
	long directory_offset = wad_header.ReadLong();
	int wad_count = wad_header.ReadShort();
	wad_header.ReadShort(); // application specific directory data size
	int entry_header_size = wad_header.ReadShort();
	
	if (wad_count != 1) {
		wxLogError(wxT("[PhysicsDocument] Error loading: wad count must be 1"));
		return stream;
	}

	// jump to the directory and read the offset of the first (and
	// hopefully only) wad
	BigEndianBuffer directory_entry(10);
#if wxUSE_STD_IOSTREAM
	stream.seekg(directory_offset, std::ios::beg);
	stream.read((char *) directory_entry.Data(), directory_entry.Size());
#else
	stream.SeekI(directory_offset, wxFromStart);
	stream.Read((char *) directory_entry.Data(), directory_entry.Size());
#endif
	long wad_offset = directory_entry.ReadLong();
	directory_entry.ReadLong(); // wad_size
	int wad_index = directory_entry.ReadShort();

	if (wad_index != 0) {
		wxLogError(wxT("[PhysicsDocument] Error loading: first wad index must be 0"));
		return stream;
	}

	long next_offset = 0;

	// read the tags
	do {
		BigEndianBuffer entry_header(entry_header_size);
#if wxUSE_STD_IOSTREAM
		stream.seekg(next_offset + wad_offset, std::ios::beg);
		stream.read((char *) entry_header.Data(), entry_header.Size());
#else
		stream.SeekI(next_offset + wad_offset, wxFromStart);
		stream.Read((char *) entry_header.Data(), entry_header.Size());
#endif
		unsigned long tag = entry_header.ReadULong();
		next_offset = entry_header.ReadLong();
		long entry_size = entry_header.ReadLong();

		BigEndianBuffer tag_data(entry_size);		
#if wxUSE_STD_IOSTREAM
		stream.read((char *) tag_data.Data(), tag_data.Size());
#else
		stream.Read((char *) tag_data.Data(), tag_data.Size());
#endif

		switch (tag)
		{
		case FOUR_CHARS_TO_INT('P','X','p','x'): {
			int count = tag_data.Size() / PhysicsConstants::kSize;
			mConstants.resize(2);
			for (int i = 0; i < count; ++i) {
				mConstants[i].LoadObject(tag_data);
				if (!mConstants[i].IsGood()) {
					return stream;
				}
			}
			break;
		}
		case FOUR_CHARS_TO_INT('M','N','p','x'): {
			int count = tag_data.Size() / MonsterDefinition::kSize;
			mMonsterDefinitions.resize(count);
			for (int i = 0; i < count; ++i) {
				mMonsterDefinitions[i].LoadObject(tag_data);
				if (!mMonsterDefinitions[i].IsGood()) {
					return stream;
				}
			}
			break;
		}
		case FOUR_CHARS_TO_INT('F','X','p','x'): {
			int count = tag_data.Size() / EffectDefinition::kSize;
			mEffectDefinitions.resize(count);
			for (int i = 0; i < count; ++i) {
				mEffectDefinitions[i].LoadObject(tag_data);
				if (!mEffectDefinitions[i].IsGood()) {
					return stream;
				}
			}
			break;
		}
		case FOUR_CHARS_TO_INT('P','R','p','x'): {
			int count = tag_data.Size() / ProjectileDefinition::kSize;
			mProjectileDefinitions.resize(count);
			for (int i = 0; i < count; ++i) {
				mProjectileDefinitions[i].LoadObject(tag_data);
				if (!mProjectileDefinitions[i].IsGood()) {
					return stream;
				}
			}
			break;
		}
		case FOUR_CHARS_TO_INT('W','P','p','x'): {
			int count = tag_data.Size() / WeaponDefinition::kSize;
			mWeaponDefinitions.resize(count);
			for (int i = 0; i < count; ++i) {
				mWeaponDefinitions[i].LoadObject(tag_data);
				if (!mWeaponDefinitions[i].IsGood()) {
					return stream;
				}
			}
			break;
		}
		}
	} while (next_offset);

	if (mConstants.size() && mMonsterDefinitions.size() && mEffectDefinitions.size() && mProjectileDefinitions.size() && mWeaponDefinitions.size()) {
		mGoodData = true;
	}

	return stream;
}

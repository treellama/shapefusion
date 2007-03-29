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

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if wxUSE_STD_IOSTREAM
    #include "wx/ioswrap.h"
#else
    #include "wx/txtstrm.h"
#endif

#include "SoundsDocument.h"
#include "SoundsView.h"

// Construct four-character-code
#define FOUR_CHARS_TO_INT(a,b,c,d) (((unsigned int)(a) << 24) | ((unsigned int)(b) << 16) | ((unsigned int)(c) << 8) | (unsigned int)(d))

enum {
	_sound_8bit = 0,
	_sound_16bit,
	NUMBER_OF_SOUND_SOURCES
};

IMPLEMENT_DYNAMIC_CLASS(SoundsDocument, wxDocument)

SoundsDocument::SoundsDocument():
	wxDocument(), SoundsElement(true)
{
	// We need storage for our sound sources...
	mSoundDefinitions.resize(NUMBER_OF_SOUND_SOURCES);
}

SoundsDocument::~SoundsDocument()
{
}

SoundsDefinition *SoundsDocument::GetSoundDefinition(unsigned short source_index, unsigned short sound_index)
{
	if (source_index > mSoundDefinitions.size())
		return NULL;
	
	if (sound_index > mSoundDefinitions[source_index].size())
		return NULL;

	return mSoundDefinitions[source_index][sound_index];
}

SoundsDefinition *SoundsDocument::Get8BitSoundDefinition(unsigned short sound_index)
{
	return GetSoundDefinition(_sound_8bit, sound_index);
}

SoundsDefinition *SoundsDocument::Get16BitSoundDefinition(unsigned short sound_index)
{
	return GetSoundDefinition(_sound_16bit, sound_index);
}

void SoundsDocument::AddSoundDefinition(void)
{
	// As there's always two versions (8-bit/16-bit) of the same sound, we add both there...
	SoundsDefinition	*snd8 = new SoundsDefinition(),
						*snd16 = new SoundsDefinition();

	// We add 8-bit...
	mSoundDefinitions[_sound_8bit].push_back(snd8);
	// ... and 16-bit
	mSoundDefinitions[_sound_16bit].push_back(snd16);
	
	// We mark ourselves as modified...
	Modify(true);
}

void SoundsDocument::DeleteSoundDefinition(unsigned int index)
{
	// We check we really have that much sounds...
	if (index > mSoundDefinitions[_sound_8bit].size())
		return;
	
	// We remove 8-bit version
	mSoundDefinitions[_sound_8bit].erase(mSoundDefinitions[_sound_8bit].begin() + index);
	// We remove 16-bit version
	mSoundDefinitions[_sound_16bit].erase(mSoundDefinitions[_sound_16bit].begin() + index);
	
	// We mark ourselves as modified...
	Modify(true);
}

bool SoundsDocument::DoOpenDocument(const wxString& file)
{
	bool wxOpen = wxDocument::DoOpenDocument(file);
	
	if (!(wxOpen && mGoodData))
	{
		wxLogError(wxT("[SoundsDocument] There was an error while loading, see log"));
		return false;
	}
	return true;
}

unsigned int SoundsDocument::GetSizeInFile(void)
{
	unsigned int size = SIZEOF_sound_file_header;
	
	for (unsigned int i = 0; i < mSoundDefinitions.size(); i++)
		for (unsigned int j = 0; j < mSoundDefinitions[i].size(); j++)
			size += mSoundDefinitions[i][j]->GetSizeInFile();
	
	return size;
}

#if wxUSE_STD_IOSTREAM
wxSTD ostream& SoundsDocument::SaveObject(wxSTD ostream& stream)
#else
wxOutputStream& SoundsDocument::SaveObject(wxOutputStream& stream)
#endif
{
	BigEndianBuffer filebuffer(GetSizeInFile());
	
	unsigned int source_count = mSoundDefinitions.size();
	unsigned int sound_count = mSoundDefinitions[0].size();
	
	filebuffer.WriteLong(mVersion);
	filebuffer.WriteLong(mTag);
	filebuffer.WriteShort(source_count);
	filebuffer.WriteShort(sound_count);
	
	filebuffer.Position(SIZEOF_sound_file_header);
	
	unsigned int current_sound_offset = SIZEOF_sound_file_header
			+ source_count * sound_count * SIZEOF_sound_definition;
	
	for (unsigned int i = 0; i < mSoundDefinitions.size(); i++) {
		for (unsigned int j = 0; j < mSoundDefinitions[i].size(); j++) {
			mSoundDefinitions[i][j]->SaveObject(filebuffer, current_sound_offset);
		}
	}
	
#if wxUSE_STD_IOSTREAM
	stream.write((char *)filebuffer.Data(), filebuffer.Size());
#else
	stream.Write((char *)filebuffer.Data(), filebuffer.Size());
#endif

	return stream;
}

#if wxUSE_STD_IOSTREAM
wxSTD istream& SoundsDocument::LoadObject(wxSTD istream& stream)
#else
wxInputStream& SoundsDocument::LoadObject(wxInputStream& stream)
#endif
{
#if wxUSE_STD_IOSTREAM
	stream.seekg(0, std::ios::end);
	wxInt32 filesize = stream.tellg();
	stream.seekg(0, std::ios::beg);
#else
	wxInt32 filesize = stream.GetSize();
#endif

	BigEndianBuffer filebuffer(filesize);
	
#if wxUSE_STD_IOSTREAM
	stream.read((char *)filebuffer.Data(), filebuffer.Size());
#else
	stream.Read((char *)filebuffer.Data(), filebuffer.Size());
#endif
	
	mVersion = filebuffer.ReadLong();
	mTag = filebuffer.ReadLong();
	mSourceCount = filebuffer.ReadShort();
	mSoundCount = filebuffer.ReadShort();
	
	if ((mVersion != 0 && mVersion != 1) || mTag != FOUR_CHARS_TO_INT('s','n','d','2')) {
		wxLogError(wxT("[SoundsDocument] Error loading : Incorrect version/tag"));
		return stream;
	}
	
	if (mSoundCount < 0 || mSourceCount < 0) {
		wxLogError(wxT("[SoundsDocument] Error loading : Incorrect Sound/Source count"));
		return stream;
	}
	
	if (mSoundCount == 0) {
		mSoundCount = mSourceCount;
		mSourceCount = 1;
	}
	
	if (IsVerbose()) {
		wxLogDebug(wxT("[SoundsDocument] Version:		%d"), mVersion);
		wxLogDebug(wxT("[SoundsDocument] Tag:			%d"), mTag);
		wxLogDebug(wxT("[SoundsDocument] Source Count:	%d"), mSourceCount);
		wxLogDebug(wxT("[SoundsDocument] Sound Count:	%d"), mSoundCount);
	}
	
	filebuffer.Position(SIZEOF_sound_file_header);

//	mSoundDefinitions.resize(mSourceCount);
	for (int i = 0; i < mSourceCount; i++) {
		for (int j = 0; j < mSoundCount; j++) {
			SoundsDefinition *snd = new SoundsDefinition(IsVerbose());
			unsigned int oldpos = filebuffer.Position();
			
			
			if (IsVerbose())
				wxLogDebug(wxT("[SoundsDocument] Loading source %d, sound %d"), i, j);
			
			snd->LoadObject(filebuffer);
			
			filebuffer.Position(oldpos + SIZEOF_sound_definition);
			
			if (!snd->IsGood()) {
				wxLogError(wxT("[SoundsDocument] Error loading sound definition. Skipping..."));
				return stream;
			}
			
			mSoundDefinitions[i].push_back(snd);
		}
	}
	
	mGoodData = true;
	return stream;
}


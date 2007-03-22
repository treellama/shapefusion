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

IMPLEMENT_DYNAMIC_CLASS(SoundsDocument, wxDocument)

SoundsDocument::SoundsDocument():
	wxDocument(), SoundsElement(true)
{

}

SoundsDocument::~SoundsDocument()
{
}

SoundsDefinition* SoundsDocument::GetSoundDefinition(unsigned short source_index, unsigned short sound_index)
{
	if (source_index > mSoundDefinitions.size())
		return NULL;
	
	if (sound_index > mSoundDefinitions[source_index].size())
		return NULL;

	return mSoundDefinitions[source_index][sound_index];
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

#if wxUSE_STD_IOSTREAM
wxSTD ostream& SoundsDocument::SaveObject(wxSTD ostream& stream)
#else
wxOutputStream& SoundsDocument::SaveObject(wxOutputStream& stream)
#endif
{
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
		wxLogError("[SoundsDocument] Error loading : Incorrect version/tag");
		return stream;
	}
	
	if (mSoundCount < 0 || mSourceCount < 0) {
		wxLogError("[SoundsDocument] Error loading : Incorrect Sound/Source count");
		return stream;
	}
	
	if (mSoundCount == 0) {
		mSoundCount = mSourceCount;
		mSourceCount = 1;
	}
	
	if (IsVerbose()) {
		wxLogDebug("[SoundsDocument] Version:		%d", mVersion);
		wxLogDebug("[SoundsDocument] Tag:			%d", mTag);
		wxLogDebug("[SoundsDocument] Source Count:	%d", mSourceCount);
		wxLogDebug("[SoundsDocument] Sound Count:	%d", mSoundCount);
	}
	
	filebuffer.Position(SIZEOF_sound_file_header);

	mSoundDefinitions.resize(mSourceCount);
	for (int i = 0; i < mSourceCount; i++) {
		for (int j = 0; j < mSoundCount; j++) {
			SoundsDefinition *snd = new SoundsDefinition(IsVerbose());
			unsigned int oldpos = filebuffer.Position();
			
			
			if (IsVerbose())
				wxLogDebug(wxT("[SoundsDocument] Loading source %d, sound %d"), i, j);
			
			snd->LoadObject(filebuffer);
			
			filebuffer.Position(oldpos + SIZEOF_sound_definition);
			
			if (!snd->IsGood()) {
				wxLogError("[SoundsDocument] Error loading sound definition. Skipping...");
				return stream;
			}
			
			mSoundDefinitions[i].push_back(snd);
		}
	}
	
	mGoodData = true;
	return stream;
}

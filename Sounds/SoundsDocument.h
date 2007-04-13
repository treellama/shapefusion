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

#ifndef __SOUNDDOCUMENT_H__
#define __SOUNDDOCUMENT_H__

#include "wx/docview.h"
#include "wx/cmdproc.h"
#include "SoundsElements.h"

class SoundsDocument: public wxDocument, public SoundsElement
{
    DECLARE_DYNAMIC_CLASS(SoundsDocument)
private:
	/* Marathon Sound Header */
	int mVersion;
	int mTag;
	
	short mSourceCount; // usually 2 (8-bit, 16-bit)
	short mSoundCount;
	
//	short unused[124];
	
	// immediately followed by source_count*sound_count sound_definition structures
	std::vector< std::vector<SoundsDefinition*> > mSoundDefinitions;
	
	/* some of our needed info */
	 bool mM2Demo;
public:

    SoundsDocument(void);
    ~SoundsDocument(void);
	
	int GetVersion(void) const {return mVersion;}
	int GetTag(void) const {return mTag;}
	
	unsigned int GetSourceCount(void) const {return mSourceCount;}
	unsigned int GetSoundCount(void) const {return mSoundCount;}
	
	SoundsDefinition *GetSoundDefinition(unsigned short source_index, unsigned short sound_index);
	SoundsDefinition *Get8BitSoundDefinition(unsigned short sound_index);
	SoundsDefinition *Get16BitSoundDefinition(unsigned short sound_index);
	
	void AddSoundDefinition(void);
	void DeleteSoundDefinition(unsigned int index);
	
	bool IsM2DemoFile(void) const {return mM2Demo;}
	void SetM2DemoFile(bool b) {mM2Demo = b;}
	
	bool DoOpenDocument(const wxString& file);
	
	// Utilities
	unsigned int GetSizeInFile(void);
#if wxUSE_STD_IOSTREAM
    wxSTD ostream& SaveObject(wxSTD ostream& stream);
    wxSTD istream& LoadObject(wxSTD istream& stream);
#else
    wxOutputStream& SaveObject(wxOutputStream& stream);
    wxInputStream& LoadObject(wxInputStream& stream);
#endif
};

#endif

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

#ifndef DEFAULTNAMES_H
#define DEFAULTNAMES_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <map>
#include <vector>

class DefaultNames {
public:
	static DefaultNames* Instance() { 
		if (!mInstance) {
			mInstance = new DefaultNames;
		}
		return mInstance;
	}

	wxString Get(const wxString& set, int index);
	std::vector<wxString> GetArray(const wxString& set);

private:
	DefaultNames();

	static DefaultNames* mInstance;

	std::map<wxString, std::map<int, wxString> > mMap;
};

static inline wxString GetName(const wxString& set, int index) { return DefaultNames::Instance()->Get(set, index); }

#endif

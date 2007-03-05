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
#ifndef MYTREEITEMDATA_H
#define MYTREEITEMDATA_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "wx/treectrl.h"
#include "ShapesDocument.h"

enum {
	TREESECTION_COLLECTION = 1,
	TREESECTION_VERSION,
	TREESECTION_BITMAPS,
	TREESECTION_COLORTABLES,
	TREESECTION_FRAMES,
	TREESECTION_SEQUENCES
};

class MyTreeItemData: public wxTreeItemData {
private:
	int	coll_id;
	int	version;
	int section;
	int	sequence;

public:
	MyTreeItemData(int id=-1, int vers=-1, int sect=-1, int seq=-1);
	~MyTreeItemData(void);
	
	int CollID(void) const;
	int Version(void) const;
	int Section(void) const;
	int Sequence(void) const;
};

#endif

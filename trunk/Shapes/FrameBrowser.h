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
// FrameBrowser
// A widget for displaying a list of ShapesFrame's as a scrollable list
// of selectable thumbnails. Each thumbnail displays the frame's
// associated bitmap, altered with the specified mirror transformations.
//

#ifndef FRAMEBROWSER_H
#define FRAMEBROWSER_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "ShapesDocument.h"

DECLARE_EVENT_TYPE(wxEVT_FRAMEBROWSER, -1)
DECLARE_EVENT_TYPE(wxEVT_FRAMEBROWSER_DELETE, -1)

class FrameBrowser: public wxScrolledWindow {
private:
	vector<ShapesFrame*>	frames;				// pointers to frames
	vector<ShapesBitmap*>	bitmaps;			// pointers to bitmaps
	vector<wxBitmap>		thumbnails;			// scaled/rendered frame thumbnails
	ShapesColorTable		*ctable;			// which palette to use for display
	vector<wxPoint>			tn_positions;		// thumbnail positions within window
	wxCoord					tn_size,			// thumbnail size
							margin;				// margin between thumbnails and window edges
	bool					auto_size,
							white_transparency;
	int						selection,			// selected thumbnail
							num_cols,
							num_rows;
	wxPen					tn_pen,
							selection_pen,
							invisible_pen;
	unsigned int			frozen_count;

	wxBitmap CreateThumbnail(ShapesFrame *fp);
	void UpdateVirtualSize(void);

protected:
	DECLARE_EVENT_TABLE();

public:
	FrameBrowser(wxWindow *parent, wxWindowID id);
	// event handlers
	void OnPaint(wxPaintEvent& e);
	void OnSize(wxSizeEvent& e);
	void OnMouseDown(wxMouseEvent& e);
	void OnKeyDown(wxKeyEvent& e);
	// wx things
	void Freeze(void);
	void Thaw(void);
	// access methods
	int GetSelection(void) const;
	void SetThumbnailSize(int size);
	void SetTranspPixelsDisplay(bool show);
	void AddFrame(ShapesFrame *fp);
	void AddBitmap(ShapesBitmap *bp);
	void SetColorTable(ShapesColorTable *ct);
	void Clear(void);
	// utilities
	void RebuildThumbnail(unsigned int i);
	void RebuildThumbnails(void);
};

#endif

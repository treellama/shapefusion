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
// BitmapView
// A widget for displaying a single, full size ShapesBitmap
//

#ifndef BITMAPVIEW_H
#define BITMAPVIEW_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "ShapesDocument.h"

class BitmapView: public wxScrolledWindow {
private:
	ShapesBitmap		*enc_bmp;			// encoded bitmap
	wxBitmap			dec_bmp;			// ready-to-draw bitmap
	ShapesColorTable	*ctable;			// which palette to use for display
	bool			white_transparency;	// hide transparent pixels
	wxPen			invisible_pen;

	bool			dragging;
	int				drag_start_x,
					drag_start_y;

protected:
	DECLARE_EVENT_TABLE();

public:
	BitmapView(wxWindow *parent);
	// event handlers
	void OnPaint(wxPaintEvent &e);
	void OnSize(wxSizeEvent &e);
	void OnDrag(wxMouseEvent &e);
	// access
	void SetTranspPixelsDisplay(bool show);
	void SetBitmap(ShapesBitmap *bp);
	ShapesBitmap *GetBitmap(void) const;
	void SetColorTable(ShapesColorTable *ct);
};

#endif

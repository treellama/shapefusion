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
// FrameView
// A widget for displaying a single, full size frame
//

#ifndef FRAMEVIEW_H
#define FRAMEVIEW_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "ShapesElements.h"

DECLARE_EVENT_TYPE(wxEVT_FRAMEVIEW_DRAG, -1)

class FrameView: public wxScrolledWindow {
private:
	ShapesFrame			*mFrame;			// frame data
	ShapesBitmap		*mEncBmp;			// encoded bitmap
	wxBitmap			mDecBmp;			// ready-to-draw associated frame bitmap
	ShapesColorTable	*mColorTable;		// which palette to use for display
	bool				mWhiteTransparency;	// hide transparent pixels
	wxPen				mOriginPen,
						mKeypointPen;
	wxCursor			mPanCursor,
						mPointCursor;

	bool				mPanning,
						mDraggingOrigin,
						mDraggingKey,
						mNearOrigin,
						mNearKey,
						mCenterOrigin;	// center frame origin rather than frame bitmap
	int					mDragStartX,
						mDragStartY;

protected:
	DECLARE_EVENT_TABLE();

public:
	FrameView(wxWindow *parent, wxWindowID id);
	// event handlers
	void OnPaint(wxPaintEvent &e);
	void OnSize(wxSizeEvent &e);
	void OnDrag(wxMouseEvent &e);
	// access
	void SetTranspPixelsDisplay(bool show);
	void SetCenterOrigin(bool center);
	void SetFrame(ShapesFrame *fp);
	ShapesFrame *GetFrame(void) const;
	void SetBitmap(ShapesBitmap *bp);
	void SetColorTable(ShapesColorTable *ct);
};

#endif

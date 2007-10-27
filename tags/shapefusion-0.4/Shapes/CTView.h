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
// CTView
// A widget for displaying a ShapesColorTable. We don't derive
// wxScrolledWindow because IMO the purpose of the widget doesn't
// require scrolling.
//

#ifndef CTVIEW_H
#define CTVIEW_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "ShapesElements.h"

DECLARE_EVENT_TYPE(wxEVT_CTVIEW_SELECTION, -1)
DECLARE_EVENT_TYPE(wxEVT_CTVIEW_COLOR, -1)

class CTView: public wxPanel {
private:
	ShapesColorTable	*mColorTable;		// link to the color table
	wxPen				mInvisiblePen,
						mSelectionPen;
	unsigned int		mSwatchSize,
						mMargin;
	wxBitmap			mLightBulbIcon;
	vector<bool>		mSelectionMask;

protected:
	DECLARE_EVENT_TABLE();
	void CalculateSwatchSize(void);

public:
	CTView(wxWindow *parent);
	// event handlers
	void OnPaint(wxPaintEvent &e);
	void OnSize(wxSizeEvent &e);
	void OnMouseDown(wxMouseEvent& e);
	void OnMouseDoubleClick(wxMouseEvent& e);
	// access
	void SetColorTable(ShapesColorTable *ct);
	vector<bool> GetSelection(void) const;
};

#endif

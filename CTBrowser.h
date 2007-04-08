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
// CTBrowser
// Widget for displaying a list of ShpColorTable's. Each color table
// is show as a matrix of color samples. The width of each sample will
// adjust to fit inside the available widget width, while the sample
// height is fixed.
//

#ifndef CTBROWSER_H
#define CTBROWSER_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <vector>
#include "Shapes.h"
using std::vector;

DECLARE_EVENT_TYPE(wxEVT_CTBROWSER, -1)

class CTBrowser: public wxScrolledWindow {
private:
	vector<ShpColorTable *>	cts;				// array of pointers to color tables
	unsigned int			colors_per_table;
	wxCoord					sample_w, sample_h,	// size of color samples
							margin;
	wxPen					invisible_pen,
							selection_pen;
	int						selection;

	void UpdateVirtualSize(void);

protected:
	DECLARE_EVENT_TABLE();

public:
	CTBrowser(wxWindow *parent);
	void OnPaint(wxPaintEvent& e);
	void OnSize(wxSizeEvent& e);
	void OnMouseDown(wxMouseEvent& e);
	void OnKeyDown(wxKeyEvent &e);

	void AddColorTable(ShpColorTable *ctp);
	void Clear(void);

	int GetSelection(void) const;
};

#endif

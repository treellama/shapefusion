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
#include "CTView.h"

BEGIN_EVENT_TABLE(CTView, wxPanel)
	EVT_PAINT(CTView::OnPaint)
	EVT_SIZE(CTView::OnSize)
END_EVENT_TABLE()

CTView::CTView(wxWindow *parent):
	wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER),
	mColorTable(NULL)
{
	SetBackgroundColour(wxColour(255, 255, 255));
	mInvisiblePen.SetColour(0, 0, 0);
	mInvisiblePen.SetStyle(wxTRANSPARENT);
}

void CTView::OnPaint(wxPaintEvent& e)
{
	wxPaintDC	tempdc(this);
	wxBrush		brush(wxColour(0, 0, 0), wxSOLID);
	int			sample_w = 10, x = 0, y = 0, width, height;

	tempdc.GetSize(&width, &height);
	tempdc.SetPen(mInvisiblePen);
	for (unsigned int j = 0; j < mColorTable->ColorCount(); j++) {
		brush.SetColour(mColorTable->GetColor(j)->Red() >> 8,
						mColorTable->GetColor(j)->Green() >> 8,
						mColorTable->GetColor(j)->Blue() >> 8);
		tempdc.SetBrush(brush);
		tempdc.DrawRectangle(x, y, sample_w, sample_w);
		if (mColorTable->GetColor(j)->Luminescent()) {
			// display the self-luminescent color flag
			tempdc.SetBrush(*wxWHITE_BRUSH);
			tempdc.DrawRectangle(x + 2, y + 2, 2, 2);
		}
		x += sample_w;
		if (x + sample_w >= width) {
			x = 0;
			y += sample_w;
		}
	}
}

void CTView::OnSize(wxSizeEvent &e)
{
	
}

void CTView::SetColorTable(ShapesColorTable *ct)
{
	mColorTable = ct;
	Refresh();
}


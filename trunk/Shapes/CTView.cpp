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
	wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER | wxFULL_REPAINT_ON_RESIZE),
	mColorTable(NULL), mSwatchSize(0), mMargin(2)
{
	SetBackgroundColour(wxColour(255, 255, 255));
	mInvisiblePen.SetColour(0, 0, 0);
	mInvisiblePen.SetStyle(wxTRANSPARENT);
}

void CTView::OnPaint(wxPaintEvent& e)
{
	wxPaintDC		tempdc(this);
	wxBrush			brush(wxColour(0, 0, 0), wxSOLID);
	unsigned int	x = mMargin, y = mMargin;
	int				width, height;

	tempdc.GetSize(&width, &height);
	tempdc.SetPen(mInvisiblePen);
	for (unsigned int j = 0; j < mColorTable->ColorCount(); j++) {
		brush.SetColour(mColorTable->GetColor(j)->Red() >> 8,
						mColorTable->GetColor(j)->Green() >> 8,
						mColorTable->GetColor(j)->Blue() >> 8);
		tempdc.SetBrush(brush);
		tempdc.DrawRectangle(x+2, y+2, mSwatchSize-4, mSwatchSize-4);
		if (mColorTable->GetColor(j)->Luminescent()) {
			// display the self-luminescent color flag
			tempdc.SetBrush(*wxWHITE_BRUSH);
			tempdc.DrawRectangle(x + 2, y + 2, 2, 2);
		}
		x += mSwatchSize;
		if (x + mSwatchSize >= width) {
			x = mMargin;
			y += mSwatchSize;
		}
	}
}

void CTView::OnSize(wxSizeEvent &e)
{
	CalculateSwatchSize();
}

void CTView::SetColorTable(ShapesColorTable *ct)
{
	bool	need_recalc = (ct != NULL) && (mColorTable != NULL)
				&& (ct->ColorCount() != mColorTable->ColorCount());

	mColorTable = ct;
	if (need_recalc)
		CalculateSwatchSize();
	Refresh();
}

// calculate best swatch size fitting into the widget area
void CTView::CalculateSwatchSize(void)
{
	if (mColorTable != NULL) {
		int				width, height;
		unsigned int	new_swatch_size = 2;

		GetVirtualSize(&width, &height);
		mSwatchSize = 0;
		while (mSwatchSize == 0) {
			// FIXME faster & more elegant math please
			unsigned int	x = mMargin, y = mMargin;

			for (unsigned int i = 0; i < mColorTable->ColorCount(); i++) {
				x += new_swatch_size;
				if (x + new_swatch_size >= width) {
					x = mMargin;
					y += new_swatch_size;
					if (y + new_swatch_size >= height) {
						mSwatchSize = new_swatch_size - 1;
						break;
					}
				}
			}
			new_swatch_size++;
		}
	} else {
		mSwatchSize = 0;
	}
}


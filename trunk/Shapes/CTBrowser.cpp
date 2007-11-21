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
#include "CTBrowser.h"

DEFINE_EVENT_TYPE(wxEVT_CTBROWSER)

BEGIN_EVENT_TABLE(CTBrowser, wxScrolledWindow)
	EVT_PAINT(CTBrowser::OnPaint)
	EVT_SIZE(CTBrowser::OnSize)
	EVT_LEFT_DOWN(CTBrowser::OnMouseDown)
	EVT_KEY_DOWN(CTBrowser::OnKeyDown)
END_EVENT_TABLE()

CTBrowser::CTBrowser(wxWindow *parent):
	wxScrolledWindow(parent, -1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER | wxFULL_REPAINT_ON_RESIZE),
	mSampleW(1), mSampleH(20), mSelection(-1)
{
	SetBackgroundColour(wxColour(255, 255, 255));
	mInvisiblePen.SetColour(0, 0, 0);
	mInvisiblePen.SetStyle(wxTRANSPARENT);
	mSelectionPen.SetColour(0, 0, 0);
	mSelectionPen.SetWidth(3);
	SetScrollRate(2, 2);
	mMargin = 10;
}

void CTBrowser::OnPaint(wxPaintEvent& e)
{
	wxPaintDC	tempdc(this);
	int			visible_w, visible_h,
				rx, ry,
				x = mMargin, y = mMargin;

	DoPrepareDC(tempdc);
	GetClientSize(&visible_w, &visible_h);
	// fill with white
	CalcUnscrolledPosition(0, 0, &rx, &ry);
	tempdc.SetPen(mInvisiblePen);
	// draw color samples
	for (unsigned int i = 0; i < mColorTables.size(); i++) {
		wxBrush	brush(wxColour(0, 0, 0), wxSOLID);

		// mark selected ct
		if ((int)i == mSelection) {
			tempdc.SetPen(mSelectionPen);
			tempdc.SetBrush(*wxWHITE_BRUSH);
			tempdc.DrawRectangle(x-3, y-3, mSampleW*mColorsPerTable+6, mSampleH+6);
			tempdc.SetPen(mInvisiblePen);
			tempdc.SetBrush(*wxTRANSPARENT_BRUSH);
		}
		// draw a square for each color
		for (unsigned int j = 0; j < mColorsPerTable; j++) {
			brush.SetColour(mColorTables[i]->GetColor(j)->Red() >> 8,
							mColorTables[i]->GetColor(j)->Green() >> 8,
							mColorTables[i]->GetColor(j)->Blue() >> 8);
			tempdc.SetBrush(brush);
			tempdc.DrawRectangle(x, y, mSampleW, mSampleH);
			if (mColorTables[i]->GetColor(j)->Luminescent()) {
				// display the self-luminescent color flag
				tempdc.SetBrush(*wxWHITE_BRUSH);
				tempdc.DrawRectangle(x + 2, y + 2, 2, 2);
			}
			x += mSampleW;
		}
		x = mMargin;
		y += mSampleH + mMargin;
	}
}

// widget resized, recalculate virtual size to correctly wrap thumbnails
void CTBrowser::OnSize(wxSizeEvent& e)
{
	UpdateVirtualSize();
}

void CTBrowser::OnMouseDown(wxMouseEvent& e)
{
	wxClientDC	dc(this);
	wxPoint		mouse;
	
	DoPrepareDC(dc);
	mouse = e.GetLogicalPosition(dc);
	switch (e.GetButton()) {
		case wxMOUSE_BTN_LEFT:	// ct selection
			{
				int	new_selection = -1;
				
				for (unsigned int i = 0; i < mColorTables.size(); i++) {
					wxRect	test(mMargin, mMargin + i * (mSampleH + mMargin), mSampleW*mColorsPerTable, mSampleH);

					if (test.Contains(mouse)) {
						new_selection = i;
						break;
					}
				}
				if (new_selection != mSelection) {
					mSelection = new_selection;
					Refresh();
					// send selection event
					wxCommandEvent	event(wxEVT_CTBROWSER, GetId());

					event.SetEventObject(this);
					event.SetInt(mSelection);
					GetEventHandler()->ProcessEvent(event);
				}
			}
			break;
	}
	e.Skip();
}

void CTBrowser::OnKeyDown(wxKeyEvent &e)
{
	int new_selection = mSelection;

	if (mSelection >= 0 && mSelection < (int)mColorTables.size()) {
		// arrow keys move the selection
		switch (e.GetKeyCode()) {
			case WXK_UP:
				if (mSelection > 0)
					new_selection--;
				break;
			case WXK_DOWN:
				if (mSelection < ((int)mColorTables.size()-1))
					new_selection++;
				break;
			default:
				e.Skip();
		}
	} else if (mColorTables.size() > 0 && (e.GetKeyCode() == WXK_UP || e.GetKeyCode() == WXK_DOWN)) {
		new_selection = 0;
	}
	if (new_selection != mSelection) {
		mSelection = new_selection;
		Refresh();
		// send selection event
		wxCommandEvent  event(wxEVT_CTBROWSER, GetId());
		
		event.SetEventObject(this);
		event.SetInt(mSelection);
		GetEventHandler()->ProcessEvent(event);
	}
}

// add a new ShapesColorTable to the list
void CTBrowser::AddColorTable(ShapesColorTable *ctp)
{
	if (ctp == NULL)
		return;
	mColorsPerTable = ctp->ColorCount();
	mColorTables.push_back(ctp);
	UpdateVirtualSize();
	Refresh();
}

// clear the color table list
void CTBrowser::Clear(void)
{
	mColorTables.clear();
	mSelection = -1;
	mColorsPerTable = 0;
	UpdateVirtualSize();
//	Refresh();	// not needed since the ShapesEditor only clears before refilling
}

int CTBrowser::GetSelection(void) const
{
	return mSelection;
}

// calculate and set the wxWindow virtual size,
// based on the number of color tables, colors per table,
// color sample dimensions and given visible size
void CTBrowser::UpdateVirtualSize(void)
{
	wxClientDC	dc(this);
	int			numcts = mColorTables.size(),
				width, height;

	GetClientSize(&width, &height);
	// calculate best sample_w
	for (int w = 1; w < mSampleH; w++) {
		if (2*mMargin + w*(int)mColorsPerTable > width) {
			mSampleW = w - 1;
			break;
		}
	}
	if (mSampleW < 1)
		mSampleW = 1;
	SetVirtualSize(2*mMargin + mSampleW*mColorsPerTable, mMargin + numcts * (mSampleH + mMargin));
}


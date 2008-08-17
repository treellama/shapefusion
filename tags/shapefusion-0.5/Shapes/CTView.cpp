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
#include <wx/colordlg.h>

DEFINE_EVENT_TYPE(wxEVT_CTVIEW_SELECTION)
DEFINE_EVENT_TYPE(wxEVT_CTVIEW_COLOR)

BEGIN_EVENT_TABLE(CTView, wxPanel)
	EVT_PAINT(CTView::OnPaint)
	EVT_SIZE(CTView::OnSize)
	EVT_LEFT_DOWN(CTView::OnMouseDown)
	EVT_LEFT_DCLICK(CTView::OnMouseDoubleClick)
END_EVENT_TABLE()

// icon for the self-luminescent flag
static char *self_luminescent_icon_xpm[] = {
"8 8 2 1",
"   c None",
".  c #000000",
"        ",
" .      ",
" ..     ",
" ...    ",
" ....   ",
" .....  ",
" ...... ",
"        "};

CTView::CTView(wxWindow *parent):
	wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER | wxFULL_REPAINT_ON_RESIZE),
	mColorTable(NULL), mSwatchSize(0), mMargin(7), mLightBulbIcon(self_luminescent_icon_xpm)
{
	SetBackgroundColour(wxColour(255, 255, 255));
	mInvisiblePen.SetColour(0, 0, 0);
	mInvisiblePen.SetStyle(wxTRANSPARENT);
	mSelectionPen.SetColour(0, 0, 0);
	mSelectionPen.SetWidth(3);
}

void CTView::OnPaint(wxPaintEvent& e)
{
	if (mColorTable == NULL)
		return;

	wxPaintDC		tempdc(this);
	wxBrush			brush(wxColour(0, 0, 0), wxSOLID);
	unsigned int	x = mMargin, y = mMargin;
	int				width, height;

	tempdc.GetSize(&width, &height);
	tempdc.SetPen(mInvisiblePen);
	for (unsigned int j = 0; j < mColorTable->ColorCount(); j++) {
		// draw selection marker
		if (mSelectionMask[j]) {
			tempdc.SetPen(mSelectionPen);
			tempdc.SetBrush(*wxWHITE_BRUSH);
			tempdc.DrawRectangle(x-3, y-3, mSwatchSize+6, mSwatchSize+6);
			tempdc.SetPen(mInvisiblePen);
		}
		// draw color swatch
		brush.SetColour(mColorTable->GetColor(j)->Red() >> 8,
						mColorTable->GetColor(j)->Green() >> 8,
						mColorTable->GetColor(j)->Blue() >> 8);
		tempdc.SetBrush(brush);
		tempdc.DrawRectangle(x, y, mSwatchSize, mSwatchSize);
		if (mColorTable->GetColor(j)->Luminescent()) {
			// display self-luminescent color flag
			tempdc.SetBrush(*wxWHITE_BRUSH);
			tempdc.DrawBitmap(mLightBulbIcon, x, y + mSwatchSize - mLightBulbIcon.GetHeight(), true);
		}
		x += mSwatchSize + mMargin;
		if ((int)(x + mSwatchSize + mMargin) >= width) {
			x = mMargin;
			y += mSwatchSize + mMargin;
		}
	}
}

void CTView::OnSize(wxSizeEvent &e)
{
	CalculateSwatchSize();
}

void CTView::OnMouseDown(wxMouseEvent& e)
{
	wxPoint			mouse = e.GetPosition();
	unsigned int	x = mMargin, y = mMargin;
	int				width, height;
	bool			found = false;

	GetClientSize(&width, &height);
	switch (e.GetButton()) {
		case wxMOUSE_BTN_LEFT:
			// first deselect all, if we're not doing multiple selection
			if (!wxGetMouseState().ShiftDown()) {
				for (unsigned int j = 0; j < mColorTable->ColorCount(); j++)
					mSelectionMask[j] = false;
			}
			// check for clicks inside a color swatch
			for (unsigned int j = 0; j < mColorTable->ColorCount(); j++) {
				wxRect	test(x, y, mSwatchSize, mSwatchSize);

				if (test.Contains(mouse)) {
					mSelectionMask[j] = true;
					found = true;
					break;
				}
				x += mSwatchSize + mMargin;
				if ((int)(x + mSwatchSize + mMargin) >= width) {
					x = mMargin;
					y += mSwatchSize + mMargin;
				}
			}
			Refresh();
			// send selection event (telling just how many items are selected)
			{
				wxCommandEvent	event(wxEVT_CTVIEW_SELECTION, GetId());
				int				selectedCount = 0;

				for (unsigned int j = 0; j < mColorTable->ColorCount(); j++)
					selectedCount += mSelectionMask[j] ? 1 : 0;
				event.SetEventObject(this);
				event.SetInt(selectedCount);
				GetEventHandler()->ProcessEvent(event);
			}
			break;
	}
	e.Skip();
}

// handle double-clicks on swatches, showing the
// color chooser to alter the swatch color
void CTView::OnMouseDoubleClick(wxMouseEvent& e)
{
	wxPoint			mouse = e.GetPosition();
	unsigned int	x = mMargin, y = mMargin;
	int				width, height;

	GetClientSize(&width, &height);
	for (unsigned int j = 0; j < mColorTable->ColorCount(); j++) {
		wxRect	test(x, y, mSwatchSize, mSwatchSize);

		if (test.Contains(mouse)) {
			// FIXME is it ok to set the CTView as the parent?
			wxColour	newColor = ::wxGetColourFromUser(this,
										wxColour(mColorTable->GetColor(j)->Red() >> 8,
										mColorTable->GetColor(j)->Green() >> 8,
										mColorTable->GetColor(j)->Blue() >> 8));

			if (newColor.Ok()) {
				mColorTable->GetColor(j)->SetRed(newColor.Red() << 8);
				mColorTable->GetColor(j)->SetGreen(newColor.Green() << 8);
				mColorTable->GetColor(j)->SetBlue(newColor.Blue() << 8);
			}
			Refresh();
			// send an event to let the world know
			wxCommandEvent	event(wxEVT_CTVIEW_COLOR, GetId());

			event.SetEventObject(this);
			event.SetInt(j);
			GetEventHandler()->ProcessEvent(event);
			break;
		}
		x += mSwatchSize + mMargin;
		if ((int)(x + mSwatchSize + mMargin) >= width) {
			x = mMargin;
			y += mSwatchSize + mMargin;
		}
	}
	e.Skip();
}

void CTView::SetColorTable(ShapesColorTable *ct)
{
	bool	need_recalc = (ct != NULL) && (mColorTable != NULL)
				&& (ct->ColorCount() != mColorTable->ColorCount());

	mColorTable = ct;
	// update selection mask
	mSelectionMask.clear();
	if (mColorTable != NULL) {
		for (unsigned int i = 0; i < mColorTable->ColorCount(); i++)
			mSelectionMask.push_back(false);
	}
	// visual updates
	if (need_recalc)
		CalculateSwatchSize();
	Refresh();
}

vector<bool> CTView::GetSelection(void) const
{
	return mSelectionMask;
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
				x += new_swatch_size + mMargin;
				if ((int)(x + new_swatch_size + mMargin) >= width) {
					x = mMargin;
					y += new_swatch_size + mMargin;
					if ((int)(y + new_swatch_size + mMargin) >= height) {
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


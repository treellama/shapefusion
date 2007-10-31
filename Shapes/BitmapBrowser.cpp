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
#include "wx/image.h"
#include "BitmapBrowser.h"
#include "utilities.h"

DEFINE_EVENT_TYPE(wxEVT_BITMAPBROWSER)
DEFINE_EVENT_TYPE(wxEVT_BITMAPBROWSER_DELETE)

BEGIN_EVENT_TABLE(BitmapBrowser, wxScrolledWindow)
	EVT_PAINT(BitmapBrowser::OnPaint)
	EVT_SIZE(BitmapBrowser::OnSize)
	EVT_LEFT_DOWN(BitmapBrowser::OnMouseDown)
	EVT_RIGHT_DOWN(BitmapBrowser::OnMouseDown)
	EVT_KEY_DOWN(BitmapBrowser::OnKeyDown)
END_EVENT_TABLE()

BitmapBrowser::BitmapBrowser(wxWindow *parent, wxWindowID id):
	wxScrolledWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER | wxFULL_REPAINT_ON_RESIZE),
	mColorTable(NULL), mSelection(-1), mNumCols(0), mNumRows(0), mFrozenCount(0)
{
	SetBackgroundColour(wxColour(255, 255, 255));
	mTNPen.SetColour(200, 200, 200);
	mSelectionPen.SetColour(0, 0, 0);
	mSelectionPen.SetWidth(3);
	SetScrollRate(0, 2);
	mTNSize = 64;
	mMargin = 7;
	mWhiteTransparency = true;
	mAutoSize = false;
}

void BitmapBrowser::OnPaint(wxPaintEvent& e)
{
	wxPaintDC   tempdc(this);
	int			cw, ch, rx, ry;

	DoPrepareDC(tempdc);
	GetClientSize(&cw, &ch);
	CalcUnscrolledPosition(0, 0, &rx, &ry);
	// draw thumbnails
	tempdc.SetPen(mTNPen);
	tempdc.SetBrush(*wxTRANSPARENT_BRUSH);
	for (int i = 0; i < (int)mBitmaps.size(); i++) {
		int	x = mTNPositions[i].x,
			y = mTNPositions[i].y;

		if (y + mTNSize < ry)
			continue;
		if (y > ry + ch)
			break;
			
		int	bw = mThumbnails[i].GetWidth(),
			bh = mThumbnails[i].GetHeight();

		if (i == mSelection) {
			tempdc.DrawBitmap(mThumbnails[i], x + mTNSize/2 - bw/2, y + mTNSize/2 - bh/2);
			tempdc.SetPen(mSelectionPen);
			tempdc.DrawRectangle(x-2, y-2, mTNSize+4, mTNSize+4);
			tempdc.SetPen(mTNPen);
		} else {
			tempdc.DrawRectangle(x-1, y-1, mTNSize+2, mTNSize+2);
			tempdc.DrawBitmap(mThumbnails[i], x + mTNSize/2 - bw/2, y + mTNSize/2 - bh/2);
		}
	}
}

// widget resized, recalculate virtual size to correctly wrap thumbnails
void BitmapBrowser::OnSize(wxSizeEvent& e)
{
	UpdateVirtualSize();
}

// handle clicks received by the widget
void BitmapBrowser::OnMouseDown(wxMouseEvent& e)
{
	wxClientDC	dc(this);
	wxPoint		mouse;

	DoPrepareDC(dc);
	mouse = e.GetLogicalPosition(dc);
	switch (e.GetButton()) {
		case wxMOUSE_BTN_LEFT:
			// handle bitmap selection
			{
				int	new_selection = -1;

				for (unsigned int i = 0; i < mTNPositions.size(); i++) {
					wxRect	test(mTNPositions[i].x, mTNPositions[i].y, mTNSize, mTNSize);

					if (test.Contains(mouse)) {
						new_selection = i;
						break;
					}
				}
				if (new_selection != mSelection) {
					mSelection = new_selection;
					Refresh();

					// send selection event
					wxCommandEvent	event(wxEVT_BITMAPBROWSER, GetId());

					event.SetEventObject(this);
					event.SetInt(mSelection);
					GetEventHandler()->ProcessEvent(event);
				}
			}
			break;
		case wxMOUSE_BTN_RIGHT:
			break;
	}
	e.Skip();
}

// handle keydown events
void BitmapBrowser::OnKeyDown(wxKeyEvent &e)
{
	switch (e.GetKeyCode()) {
		case WXK_LEFT:
		case WXK_RIGHT:
		case WXK_UP:
		case WXK_DOWN:
			{
				int	new_selection = mSelection;

				if (mSelection >= 0 && mSelection < (int)mBitmaps.size()) {
					switch (e.GetKeyCode()) {
						case WXK_LEFT:
							if (mSelection % mNumCols > 0)
								new_selection--;
							break;
						case WXK_RIGHT:
							if (mSelection % mNumCols < (mNumCols-1))
								new_selection++;
							break;
						case WXK_UP:
							if (mSelection / mNumCols > 0)
								new_selection -= mNumCols;
							break;
						case WXK_DOWN:
							if (mSelection / mNumCols < (mNumRows-1))
								new_selection += mNumCols;
							break;
					}
				} else if (mBitmaps.size() > 0) {
					new_selection = 0;
				}
				if (new_selection != mSelection && new_selection >= 0
						&& new_selection < (int)mBitmaps.size()) {
					// TODO scroll to show the new selection
					mSelection = new_selection;
					Refresh();

					// send bitmap selection event
					wxCommandEvent	event(wxEVT_BITMAPBROWSER, GetId());

					event.SetEventObject(this);
					event.SetInt(mSelection);
					GetEventHandler()->ProcessEvent(event);
				}
			}
			break;
		case WXK_DELETE:
			// send a bitmap delete event
			if (mSelection >= 0 && mSelection < (int)mBitmaps.size()) {
				wxCommandEvent	event(wxEVT_BITMAPBROWSER_DELETE, GetId());

				event.SetEventObject(this);
				event.SetInt(mSelection);
				GetEventHandler()->ProcessEvent(event);
			}
			break;
		default:
			e.Skip();
			break;
	}
}

// the Freeze()/Thaw() combo is necessary to get a reasonably
// responsive interface. Otherwise we would be doing bursts
// of UpdateVirtualSize() and potentially RebuildThumbnails()
// every time the user changes collection
void BitmapBrowser::Freeze(void)
{
	mFrozenCount++;
}

void BitmapBrowser::Thaw(void)
{
	if (mFrozenCount > 0) {
		mFrozenCount--;
		if (mFrozenCount == 0) {
			UpdateVirtualSize();
			Refresh();
		}
	}
}

int BitmapBrowser::GetSelection(void) const
{
	return mSelection;
}

// set the thumbnail size in pixels. Specify -1 to enable best-fit mode.
void BitmapBrowser::SetThumbnailSize(int size)
{
	if (size > 0) {
		mTNSize = size;
		mAutoSize = false;
	} else {
		mAutoSize = true;
	}
	if (mFrozenCount == 0) {
		UpdateVirtualSize();
		if (!mAutoSize)
			RebuildThumbnails();
		Refresh();
	}
}

void BitmapBrowser::SetTranspPixelsDisplay(bool show)
{
	mWhiteTransparency = show;
	if (mFrozenCount == 0) {
		RebuildThumbnails();
		Refresh();
	}
}

// add a new ShapesBitmap to the thumbnail list
void BitmapBrowser::AddBitmap(ShapesBitmap *bp)
{
	if (bp != NULL) {
		if (bp->Pixels() != NULL) {
			mBitmaps.push_back(bp);
			mThumbnails.push_back(CreateThumbnail(bp));
			if (mFrozenCount == 0) {
				UpdateVirtualSize();
				Refresh();
			}
		} else {
			wxLogError(wxT("[BitmapBrowser] Added a bitmap with NULL pixels"));
		}
	}
}

// clear the thumbnail list
void BitmapBrowser::Clear(void)
{
	mThumbnails.clear();
	mBitmaps.clear();
	mTNPositions.clear();
	mSelection = -1;
	UpdateVirtualSize();
	if (mFrozenCount == 0)
		Refresh();
}

// call before adding bitmaps!
void BitmapBrowser::SetColorTable(ShapesColorTable *ct)
{
	mColorTable = ct;
	if (mFrozenCount == 0) {
		RebuildThumbnails();
		Refresh();
	}
}

// calculate and set the wxWindow virtual size, based on the
// number of thumbnails, thumbnail dimensions and given visible
// size. Also pre-calculate thumbnail positions
void BitmapBrowser::UpdateVirtualSize(void)
{
	wxClientDC		dc(this);
	unsigned int	numbitmaps = mBitmaps.size();
	int				width, height;

	GetClientSize(&width, &height);
	if (numbitmaps < 1) {
		SetVirtualSize(0, 0);
		return;
	}

	if (mAutoSize) {
		// calculate the best mTNSize
		// (maximum value not requiring window scrolling)
		int	max_bitmap_dimension = 10,
			new_tn_size;

		SetScrollRate(0, 0);
		// find greatest dimension among all bitmaps
		for (unsigned int i = 0; i < numbitmaps; i++) {
			if (mBitmaps[i]->Width() > max_bitmap_dimension)
				max_bitmap_dimension = mBitmaps[i]->Width();
			if (mBitmaps[i]->Height() > max_bitmap_dimension)
				max_bitmap_dimension = mBitmaps[i]->Height();
		}
		// FIXME a better algorithm, without looping?
		for (new_tn_size = mMargin; ; new_tn_size++) {
			int	numcols = (width - mMargin) / (new_tn_size + mMargin),
				numrows = (numcols > 0) ? (numbitmaps / numcols) : numbitmaps;

			if (numrows * numcols < (int)numbitmaps)
				numrows++;
			int	total_height = numrows * (new_tn_size + mMargin) + mMargin;

			if (total_height > height || (new_tn_size + 2 * mMargin) > width) {
				new_tn_size--;
				break;
			}
			if (new_tn_size >= max_bitmap_dimension) {
				// no point in wasting window space with huge
				// thumbnails showing small bitmaps at their center
				new_tn_size = max_bitmap_dimension;
				break;
			}
		}
		if (new_tn_size != mTNSize) {
			mTNSize = new_tn_size;
			RebuildThumbnails();
		}
	} else {
		SetScrollRate(0, 2);
	}

	mNumCols = (width - mMargin) / (mTNSize + mMargin);
	mNumRows = (mNumCols > 0) ? (numbitmaps / mNumCols) : numbitmaps;

	if (mNumRows * mNumCols < (int)numbitmaps)
		mNumRows++;

	SetVirtualSize(width, mNumRows * (mTNSize + mMargin) + mMargin);

	// recalculate thumbnail positions
	int	x = mMargin,
		y = mMargin;

	mTNPositions.clear();
	for (unsigned int i = 0; i < numbitmaps; i++) {
		mTNPositions.push_back(wxPoint(x, y));

		x += mTNSize + mMargin;
		if (x + mTNSize + mMargin > width) {
			x = mMargin;
			y += mTNSize + mMargin;
		}
	}
}

// transform a ShapesBitmap to a wxBitmap thumbnail
wxBitmap BitmapBrowser::CreateThumbnail(ShapesBitmap *bp)
{
	wxImage newimg;

	if (mColorTable)
		newimg = ShapesBitmapToImage(bp, mColorTable, mWhiteTransparency);
	return ImageThumbnail(newimg, mTNSize, true);
}

void BitmapBrowser::RebuildThumbnail(unsigned int i)
{
	if (i < mBitmaps.size())
		mThumbnails[i] = CreateThumbnail(mBitmaps[i]);
}

// just re-decode the ShapesBitmaps to their thumbnail previews,
// without touching window sizes or thumbnail positions.
// Useful to update the display at the end of SetColorTable or after
// altering the ShapesBitmaps
void BitmapBrowser::RebuildThumbnails(void)
{
	for (unsigned int i = 0; i < mBitmaps.size(); i++)
		mThumbnails[i] = CreateThumbnail(mBitmaps[i]);
}


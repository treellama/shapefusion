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
// A widget for displaying a list of ShapesFrame's
// as a scrollable list of selectable thumbnails. Each thumbnail
// displays the frame's associated bitmap, altered with
// the specified mirror transformations.
// 
#include "wx/image.h"
#include "FrameBrowser.h"
#include "utilities.h"

DEFINE_EVENT_TYPE(wxEVT_FRAMEBROWSER)
DEFINE_EVENT_TYPE(wxEVT_FRAMEBROWSER_DELETE)

BEGIN_EVENT_TABLE(FrameBrowser, wxScrolledWindow)
	EVT_PAINT(FrameBrowser::OnPaint)
	EVT_SIZE(FrameBrowser::OnSize)
	EVT_LEFT_DOWN(FrameBrowser::OnMouseDown)
	EVT_RIGHT_DOWN(FrameBrowser::OnMouseDown)
	EVT_KEY_DOWN(FrameBrowser::OnKeyDown)
END_EVENT_TABLE()

FrameBrowser::FrameBrowser(wxWindow *parent, wxWindowID id):
	wxScrolledWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER | wxFULL_REPAINT_ON_RESIZE),
	mColorTable(NULL), mNumCols(0), mNumRows(0), mFrozenCount(0)
{
	SetBackgroundColour(wxColour(255, 255, 255));
	mThumbnailPen.SetColour(200, 200, 200);
	mSelectionPen.SetColour(0, 0, 0);
	mSelectionPen.SetWidth(3);
	SetScrollRate(0, 2);
	mThumbnailSize = 64;
	mAutoSize = false;
	mMargin = 7;
	mWhiteTransparency = true;
	mSelection = -1;
}

void FrameBrowser::OnPaint(wxPaintEvent& e)
{
	wxPaintDC   tempdc(this);
	int			cw, ch, rx, ry;

	DoPrepareDC(tempdc);
	GetClientSize(&cw, &ch);
	CalcUnscrolledPosition(0, 0, &rx, &ry);
	// draw thumbnails
	tempdc.SetPen(mThumbnailPen);
	tempdc.SetBrush(*wxTRANSPARENT_BRUSH);
	for (unsigned int i = 0; i < mThumbnails.size(); i++) {
		int	x = mThumbnailPositions[i].x,
			y = mThumbnailPositions[i].y;

		if (y + mThumbnailSize < ry)
			continue;
		if (y > ry + ch)
			break;

		int	bw = mThumbnails[i].GetWidth(),
			bh = mThumbnails[i].GetHeight();

		if ((int)i == mSelection) {
			tempdc.DrawBitmap(mThumbnails[i], x + mThumbnailSize/2 - bw/2, y + mThumbnailSize/2 - bh/2);
			tempdc.SetPen(mSelectionPen);
			tempdc.DrawRectangle(x-2, y-2, mThumbnailSize+4, mThumbnailSize+4);
			tempdc.SetPen(mThumbnailPen);
		} else {
			tempdc.DrawRectangle(x-1, y-1, mThumbnailSize+2, mThumbnailSize+2);
			tempdc.DrawBitmap(mThumbnails[i], x + mThumbnailSize/2 - bw/2, y + mThumbnailSize/2 - bh/2);
		}
	}
}

// widget resized, recalculate virtual size to correctly wrap thumbnails
void FrameBrowser::OnSize(wxSizeEvent& e)
{
	UpdateVirtualSize();
}

void FrameBrowser::OnMouseDown(wxMouseEvent& e)
{
	wxClientDC	dc(this);
	wxPoint		mouse;

	DoPrepareDC(dc);
	mouse = e.GetLogicalPosition(dc);
	switch (e.GetButton()) {
		case wxMOUSE_BTN_LEFT:
			// handle frame selection
			{
				int	new_selection = -1;

				for (unsigned int i = 0; i < mThumbnailPositions.size(); i++) {
					wxRect	test(mThumbnailPositions[i].x, mThumbnailPositions[i].y,
									mThumbnailSize, mThumbnailSize);

					if (test.Contains(mouse)) {
						new_selection = i;
						break;
					}
				}
				if (new_selection != mSelection) {
					mSelection = new_selection;
					Refresh();

					// send selection event
					wxCommandEvent	event(wxEVT_FRAMEBROWSER, GetId());

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

void FrameBrowser::OnKeyDown(wxKeyEvent &e)
{
	switch (e.GetKeyCode()) {
		case WXK_LEFT:
		case WXK_RIGHT:
		case WXK_UP:
		case WXK_DOWN:
			{
				int new_selection = mSelection;

				if (mSelection >= 0 && mSelection < (int)mFrames.size()) {
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
				} else if (mFrames.size() > 0) {
					new_selection = 0;
				}
				if (new_selection != mSelection && new_selection >= 0
						&& new_selection < (int)mFrames.size()) {
					// TODO scroll to show the new selection
					mSelection = new_selection;
					Refresh();
					
					// send frame selection event
					wxCommandEvent  event(wxEVT_FRAMEBROWSER, GetId());
					
					event.SetEventObject(this);
					event.SetInt(mSelection);
					GetEventHandler()->ProcessEvent(event);
				}
			}
			break;
		case WXK_DELETE:
			// send a frame delete event
			if (mSelection >= 0 && mSelection < (int)mFrames.size()) {
				wxCommandEvent	event(wxEVT_FRAMEBROWSER_DELETE, GetId());

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
void FrameBrowser::Freeze(void)
{
	mFrozenCount++;
}

void FrameBrowser::Thaw(void)
{
	if (mFrozenCount > 0) {
		mFrozenCount--;
		if (mFrozenCount == 0) {
			UpdateVirtualSize();
			Refresh();
		}
	}
}

int FrameBrowser::GetSelection(void) const
{
	return mSelection;
}

void FrameBrowser::SetThumbnailSize(int size)
{
	if (size > 0) {
		mThumbnailSize = size;
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

void FrameBrowser::SetTranspPixelsDisplay(bool show)
{
	mWhiteTransparency = show;
	if (mFrozenCount == 0) {
		RebuildThumbnails();
		Refresh();
	}
}

// add a new ShapesFrame to the thumbnail list
void FrameBrowser::AddFrame(ShapesFrame *fp)
{
	if (fp != NULL) {
		mFrames.push_back(fp);
		mThumbnails.push_back(CreateThumbnail(fp));
		if (mFrozenCount == 0) {
			UpdateVirtualSize();
			Refresh();
		}
	}
}

// add a ShapesBitmap to the bitmap pointer list.
// Call before adding frames!
void FrameBrowser::AddBitmap(ShapesBitmap *bp)
{
	if (bp != NULL) {
		if (bp->Pixels() != NULL)
			mBitmaps.push_back(bp);
		else
			wxLogError(wxT("FrameBrowser: someone tried to add a bitmap with NULL pixels"));
	}
}

// clear the thumbnail list
void FrameBrowser::Clear(void)
{
	mThumbnails.clear();
	mFrames.clear();
	mBitmaps.clear();
	mThumbnailPositions.clear();
	mSelection = -1;
	if (mFrozenCount == 0) {
		UpdateVirtualSize();
		Refresh();
	}
}

// clear just the bitmap pointer list
void FrameBrowser::ClearBitmaps(void)
{
	mBitmaps.clear();
}

// call before adding frames!
void FrameBrowser::SetColorTable(ShapesColorTable *ct)
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
void FrameBrowser::UpdateVirtualSize(void)
{
	wxClientDC	dc(this);
	int			numframes = mThumbnails.size(),
				width, height;

	GetClientSize(&width, &height);
	if (numframes < 1) {
		SetVirtualSize(0, 0);
		return;
	}

	if (mAutoSize && numframes > 0) {
		// calculate the best tn_size
		// (its maximum value not requiring window scrolling)
		int	max_bitmap_dimension = 10,
			new_tn_size;

		SetScrollRate(0, 0);
		// find greatest dimension among all referenced bitmaps
		for (unsigned int i = 0; i < mFrames.size(); i++) {
			int	bitmapindex = mFrames[i]->BitmapIndex();

			if (bitmapindex < 0 || bitmapindex >= (int)mBitmaps.size())
				continue;
			if (mBitmaps[bitmapindex]->Width() > max_bitmap_dimension)
				max_bitmap_dimension = mBitmaps[bitmapindex]->Width();
			if (mBitmaps[bitmapindex]->Height() > max_bitmap_dimension)
				max_bitmap_dimension = mBitmaps[bitmapindex]->Height();
		}
		// start with a small size (margin) and increase it until overflow
		for (new_tn_size = mMargin; ; new_tn_size++) {
			int	numcols = (width - mMargin) / (new_tn_size + mMargin),
				numrows = (numcols > 0) ? (numframes / numcols) : numframes;

			if (numrows * numcols < numframes)
				numrows++;
			int	total_height = numrows * (new_tn_size + mMargin) + mMargin;

			if (total_height > height || (new_tn_size + 2 * mMargin) > width) {
				// here we are
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
		if (new_tn_size != mThumbnailSize) {
			mThumbnailSize = new_tn_size;
			RebuildThumbnails();
		}
	} else {
		SetScrollRate(0, 2);
	}

	mNumCols = (width - mMargin) / (mThumbnailSize + mMargin);
	mNumRows = (mNumCols > 0) ? (numframes / mNumCols) : numframes;

	if (mNumRows * mNumCols < numframes)
		mNumRows++;

	SetVirtualSize(width, mNumRows * (mThumbnailSize + mMargin) + mMargin);

	// recalculate thumbnail positions
	int	x = mMargin, y = mMargin;

	mThumbnailPositions.clear();
	for (int i = 0; i < numframes; i++) {
		mThumbnailPositions.push_back(wxPoint(x, y));

		x += mThumbnailSize + mMargin;
		if (x + mThumbnailSize + mMargin > width) {
			x = mMargin;
			y += mThumbnailSize + mMargin;
		}
	}
}

// transform an ShapesFrame to a wxBitmap thumbnail
wxBitmap FrameBrowser::CreateThumbnail(ShapesFrame *fp)
{
	if (fp->BitmapIndex() < 0 || fp->BitmapIndex() >= (int)mBitmaps.size()) {
		// invalid or unset bitmap
		return BadThumbnail(mThumbnailSize);
	} else {
		// valid bitmap
		ShapesBitmap	*bp = mBitmaps[fp->BitmapIndex()];
		wxImage			newimg(bp->Width(), bp->Height());

		// decode the bitmap to a wxImage
		if (mColorTable)
			newimg = ShapesBitmapToImage(bp, mColorTable, mWhiteTransparency);

		// apply frame transformations
		if (fp->IsXmirrored())
			newimg = newimg.Mirror(true);
		if (fp->IsYmirrored())
			newimg = newimg.Mirror(false);

		return ImageThumbnail(newimg, mThumbnailSize, true);
	}
}

void FrameBrowser::RebuildThumbnail(unsigned int i)
{
	if (i < mThumbnails.size() && i < mFrames.size())
		mThumbnails[i] = CreateThumbnail(mFrames[i]);
}

// redecode frames to bitmaps (after color table change, frame parameter variations etc)
void FrameBrowser::RebuildThumbnails(void)
{
	for (unsigned int i = 0; i < mFrames.size(); i++)
		mThumbnails[i] = CreateThumbnail(mFrames[i]);
}


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
#include "SequenceView.h"
#include "utilities.h"

DEFINE_EVENT_TYPE(wxEVT_SEQUENCEVIEW)

BEGIN_EVENT_TABLE(SequenceView, wxScrolledWindow)
	EVT_PAINT(SequenceView::OnPaint)
	EVT_SIZE(SequenceView::OnSize)
	EVT_LEFT_DOWN(SequenceView::OnMouseDown)
	EVT_RIGHT_DOWN(SequenceView::OnMouseDown)
	EVT_LEFT_UP(SequenceView::OnMouseUp)
	EVT_KEY_DOWN(SequenceView::OnKeyDown)
	EVT_MOTION(SequenceView::OnMouseMove)
END_EVENT_TABLE()

const char	arrow_left_bits[] = { 0x08, 0x0c, 0x0e, 0x0f, 0x0e, 0x0c, 0x08 },
	  		arrow_right_bits[] = { 0x01, 0x03, 0x07, 0x0f, 0x07, 0x03, 0x01 };

SequenceView::SequenceView(wxWindow *parent, wxWindowID id):
	wxScrolledWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER | wxFULL_REPAINT_ON_RESIZE),
	mColorTable(NULL), mThumbnailSize(64), mMargin(7), mPrevBtnIcon(arrow_left_bits, 4, 7),
	mNextBtnIcon(arrow_right_bits, 4, 7), mAutoSize(false), mSelection(-1), mFrameIndexes(NULL)
{
	SetBackgroundColour(wxColour(255, 255, 255));
	mThumbnailPen.SetColour(200, 200, 200);
	mSelectionPen.SetColour(0, 0, 0);
	mSelectionPen.SetWidth(3);
	mAngleBrush.SetColour(200, 200, 200);
	SetScrollRate(2, 2);
	mAngleLabelSpace = 30;	// TODO get this from real font extent
	mWhiteTransparency = true;
	mNumberOfViews = 0;
	mFramesPerView = 0;
	mAnimationType = UNANIMATED;
}

void SequenceView::OnPaint(wxPaintEvent& e)
{
	wxPaintDC   tempdc(this);
	wxPoint		mouse;
	int			cw, ch, rx, ry;

	if (mFrameIndexes == NULL)
		return;

	DoPrepareDC(tempdc);
	GetClientSize(&cw, &ch);
	CalcUnscrolledPosition(0, 0, &rx, &ry);
	// get mouse position in scrolled coords
	mouse = ScreenToClient(wxGetMousePosition());
	mouse.x += rx;
	mouse.y += ry;
	// draw thumbnails
	tempdc.SetPen(mThumbnailPen);
	tempdc.SetBrush(*wxTRANSPARENT_BRUSH);
	tempdc.SetTextForeground(wxColour(0, 0, 0));
	tempdc.SetBackgroundMode(wxTRANSPARENT);
	for (unsigned int i = 0; i < mThumbnails.size(); i++) {
		int	x = mThumbnailPositions[i].x,
			y = mThumbnailPositions[i].y,
			bw = mThumbnails[i].GetWidth(),
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
		// draw arrow buttons if mouse is over this thumbnail
		wxRect	tnrect(x, y, mThumbnailSize, mThumbnailSize);

		if (tnrect.Contains(mouse)) {
			wxString	label = wxString::Format(wxT("%d"), (*mFrameIndexes)[i]);
			int			labelw, labelh;

			tempdc.SetFont(*wxSMALL_FONT);
			tempdc.DrawBitmap(mPrevBtnIcon, x + 2, y + mThumbnailSize - mPrevBtnIcon.GetHeight() - 2);
			tempdc.DrawBitmap(mNextBtnIcon, x + mThumbnailSize - mNextBtnIcon.GetWidth() - 2, y + mThumbnailSize - mNextBtnIcon.GetHeight() - 2);
			tempdc.GetTextExtent(label, &labelw, &labelh);
			tempdc.SetTextForeground(wxColour(255, 255, 255));
			tempdc.DrawText(label, x + (mThumbnailSize-labelw) / 2 - 1, y + mThumbnailSize - labelh - 2);
			tempdc.DrawText(label, x + (mThumbnailSize-labelw) / 2 + 1, y + mThumbnailSize - labelh - 2);
			tempdc.DrawText(label, x + (mThumbnailSize-labelw) / 2, y + mThumbnailSize - labelh - 2 - 1);
			tempdc.DrawText(label, x + (mThumbnailSize-labelw) / 2, y + mThumbnailSize - labelh - 2 + 1);
			tempdc.SetTextForeground(wxColour(0, 0, 0));
			tempdc.DrawText(label, x + (mThumbnailSize-labelw) / 2, y + mThumbnailSize - labelh - 2);
		}
	}
	if (mThumbnails.size() > 0 && mAnimationType != UNANIMATED && mAnimationType != ANIMATED_1) {
		// draw angle labels
		char		*deg = "°";
		wxString	deg2(deg, wxConvISO8859_1);	// FIXME does not work on OS X (draws infinity char)

		tempdc.SetFont(*wxSMALL_FONT);
		tempdc.SetBrush(mAngleBrush);
		for (int i = 0; i < mNumberOfViews; i++) {
			wxString	label = wxString::Format(wxT("%d"), i * 360 / mNumberOfViews) + deg2;
			int			tw, th;
	
			tempdc.GetTextExtent(label, &tw, &th);
			tempdc.DrawRectangle(mMargin-1, mMargin + i * (mThumbnailSize + mMargin) - 1, mAngleLabelSpace+2, mThumbnailSize+2);
			tempdc.DrawText(label, mMargin + mAngleLabelSpace - tw, mMargin + i * (mThumbnailSize + mMargin) + mThumbnailSize/2 - th/2);
		}
	}
}

// widget resized, recalculate virtual size to correctly wrap thumbnails
void SequenceView::OnSize(wxSizeEvent& e)
{
	UpdateVirtualSize();
}

void SequenceView::OnMouseDown(wxMouseEvent& e)
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
					wxCommandEvent	event(wxEVT_SEQUENCEVIEW, GetId());

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

void SequenceView::OnMouseUp(wxMouseEvent& e)
{
	wxClientDC  dc(this);
	wxPoint     mouse;
	
	DoPrepareDC(dc);
	mouse = e.GetLogicalPosition(dc);
	switch (e.GetButton()) {
		case wxMOUSE_BTN_LEFT:
			// clicks on the little arrows change the associated frame index
			// FIXME mouse tracking like real buttons (starting with a mouse down)
			{
				// find wether the mouse was released over a thumbnail
				int touched_thumbnail = -1;
				
				for (unsigned int i = 0; i < mThumbnailPositions.size(); i++) {
					wxRect  test(mThumbnailPositions[i].x, mThumbnailPositions[i].y,
									mThumbnailSize, mThumbnailSize);
					
					if (test.Contains(mouse)) {
						touched_thumbnail = i;
						break;
					}
				}
				if (touched_thumbnail > -1) {
					// find wether an arrow was touched
					int     x = mThumbnailPositions[touched_thumbnail].x,
							y = mThumbnailPositions[touched_thumbnail].y;
					wxRect  prev_rect(x + 2, y + mThumbnailSize - mPrevBtnIcon.GetHeight() - 2,
								mPrevBtnIcon.GetWidth(), mPrevBtnIcon.GetHeight()),
							next_rect(x + mThumbnailSize - mNextBtnIcon.GetWidth() - 2,
								y + mThumbnailSize - mNextBtnIcon.GetHeight() - 2,
								mPrevBtnIcon.GetWidth(), mPrevBtnIcon.GetHeight());
					
					if (prev_rect.Contains(mouse)) {
						if ((*mFrameIndexes)[touched_thumbnail] > -1) {
							(*mFrameIndexes)[touched_thumbnail]--;
							RebuildThumbnail(touched_thumbnail);
							Refresh();
						}
					} else if (next_rect.Contains(mouse)) {
						if ((*mFrameIndexes)[touched_thumbnail] < ((int)mFrames.size()-1)) {
							(*mFrameIndexes)[touched_thumbnail]++;
							RebuildThumbnail(touched_thumbnail);
							Refresh();
						}
					}
				}
			}
			break;
	}
	e.Skip();
}

void SequenceView::OnMouseMove(wxMouseEvent &e)
{
	Refresh();
}

void SequenceView::OnKeyDown(wxKeyEvent &e)
{
	if (e.ShiftDown()) {
		// edit frame index of selection
		switch (e.GetKeyCode()) {
			case WXK_LEFT:
				if (mSelection >= 0) {
					if ((*mFrameIndexes)[mSelection] > -1) {
						(*mFrameIndexes)[mSelection]--;
						RebuildThumbnail(mSelection);
						Refresh();
					}
				}
				break;
			case WXK_RIGHT:
				if (mSelection >= 0) {
					if ((*mFrameIndexes)[mSelection] < ((int)mFrames.size()-1)) {
						(*mFrameIndexes)[mSelection]++;
						RebuildThumbnail(mSelection);
						Refresh();
					}
				}
				break;
			default:
				e.Skip();
				break;
		}
	} else {
		// edit selection with arrow keys
		int	new_selection = mSelection;

		if (mSelection >= 0 && mSelection < (mNumberOfViews * mFramesPerView)) {
			switch (e.GetKeyCode()) {
				case WXK_LEFT:
					if (mSelection % mFramesPerView > 0)
						new_selection--;
					break;
				case WXK_RIGHT:
					if (mSelection % mFramesPerView < (mFramesPerView-1))
						new_selection++;
					break;
				case WXK_UP:
					if (mSelection / mFramesPerView > 0)
						new_selection -= mFramesPerView;
					break;
				case WXK_DOWN:
					if (mSelection / mFramesPerView < (mNumberOfViews-1))
						new_selection += mFramesPerView;
					break;
				default:
					e.Skip();
			}
		} else if (mNumberOfViews * mFramesPerView > 0) {
			new_selection = 0;
		}
		// TODO scroll to show the new position
		if (new_selection != mSelection) {
			mSelection = new_selection;
			Refresh();
			
			// send selection event
			wxCommandEvent	event(wxEVT_SEQUENCEVIEW, GetId());

			event.SetEventObject(this);
			event.SetInt(mSelection);
			GetEventHandler()->ProcessEvent(event);
		}
	}
}

int SequenceView::GetSelection(void) const
{
	return mSelection;
}

void SequenceView::SetThumbnailSize(int size)
{
	if (size > 0) {
		mThumbnailSize = size;
		mAutoSize = false;
	} else {
		mAutoSize = true;
	}
	UpdateVirtualSize();
	if (!mAutoSize)
		RebuildThumbnails();
	Refresh();
}

void SequenceView::SetTranspPixelsDisplay(bool show)
{
	mWhiteTransparency = show;
	RebuildThumbnails();
	Refresh();
}

// add a new ShapesFrame to the thumbnail list
void SequenceView::AddFrame(ShapesFrame *fp)
{
	if (fp != NULL)
		mFrames.push_back(fp);
}

// add a ShapesBitmap to the bitmap pointer list. Call before adding frames!
void SequenceView::AddBitmap(ShapesBitmap *bp)
{
	if (bp != NULL) {
		if (bp->Pixels() != NULL)
			mBitmaps.push_back(bp);
		else
			wxLogError(wxT("SequenceView: someone tried to add a bitmap with NULL pixels"));
	}
}

// set sequence parameters: frames per view, animation type and frame index array.
// Guess number of views from animation type. Also create thumbnails (add frames,
// bitmaps and set color table before!)
void SequenceView::SetSeqParameters(int animtype, int fpv, vector<short> *indexes)
{
	mAnimationType = animtype;
	mNumberOfViews = ActualNumberOfViews(animtype);
	mFramesPerView = fpv;
	mFrameIndexes = indexes;
	RebuildThumbnails();
	UpdateVirtualSize();
	Refresh();
}

// clear the thumbnail list
void SequenceView::Clear(void)
{
	mThumbnails.clear();
	mFrames.clear();
	mBitmaps.clear();
	mThumbnailPositions.clear();
	mSelection = -1;
	UpdateVirtualSize();
	Refresh();
}

// call before adding frames!
void SequenceView::SetColorTable(ShapesColorTable *ct)
{
	mColorTable = ct;
	RebuildThumbnails();
	Refresh();
}

// calculate and set the wxWindow virtual size, based on the
// number of thumbnails, thumbnail dimensions and given visible
// size. Also pre-calculate thumbnail positions
// FIXME behave like a FrameBrowser if mAnimationType == UNANIMATED,
// because frames are just to be chosen randomly and not really
// an animation. 
void SequenceView::UpdateVirtualSize(void)
{
	wxClientDC	dc(this);
	int			numframes = mThumbnails.size(),
				width, height,
				additional_pad = (mAnimationType == UNANIMATED || mAnimationType == ANIMATED_1)
									? 0 : (mAngleLabelSpace + mMargin);

	if (numframes < 1 || mFrameIndexes == NULL) {
		SetVirtualSize(0, 0);
		return;
	}
	GetClientSize(&width, &height);
	if (mAutoSize && numframes > 0) {
		// calculate the best tn_size
		// (its maximum value not requiring window scrolling)
		int	max_bitmap_dimension = 0,
			new_tn_size;

		SetScrollRate(0, 0);
		// find greatest dimension among all referenced bitmaps
		for (unsigned int i = 0; i < mFrameIndexes->size(); i++) {
			int	frame_index = (*mFrameIndexes)[i];

			if (frame_index < 0 || frame_index >= (int)mFrames.size())
				continue;

			int	bitmap_index = mFrames[frame_index]->BitmapIndex();

			if (bitmap_index < 0 || bitmap_index >= (int)mBitmaps.size())
				continue;
			if (mBitmaps[bitmap_index]->Width() > max_bitmap_dimension)
				max_bitmap_dimension = mBitmaps[bitmap_index]->Width();
			if (mBitmaps[bitmap_index]->Height() > max_bitmap_dimension)
				max_bitmap_dimension = mBitmaps[bitmap_index]->Height();
		}
		// start with a minimum size and increase it until overflow
		for (new_tn_size = 2*mMargin; ; new_tn_size++) {
			int	numcols = mFramesPerView,
				numrows = mNumberOfViews,
				total_width = numcols * (new_tn_size + mMargin) + mMargin + additional_pad,
				total_height = numrows * (new_tn_size + mMargin) + mMargin;

			if (total_width > width || total_height > height) {
				// here we are
				new_tn_size--;
				break;
			}
			if (max_bitmap_dimension > 0 && new_tn_size >= max_bitmap_dimension) {
				// no point in wasting window space with huge
				// thumbnails showing small bitmaps at their center
				break;
			}
		}
		if (new_tn_size != mThumbnailSize) {
			mThumbnailSize = new_tn_size;
			RebuildThumbnails();
		}
	} else {
		SetScrollRate(2, 2);
	}

	// now row & column count is simply mNumberOfViews and mFramesPerView,
	// not the mess needed in the FrameBrowser
	int	numcols = mFramesPerView,
		numrows = mNumberOfViews;

	SetVirtualSize(numcols * (mThumbnailSize + mMargin) + mMargin + additional_pad,
					numrows * (mThumbnailSize + mMargin) + mMargin);

	// recalculate thumbnail positions
	mThumbnailPositions.clear();
	for (int r = 0; r < numrows; r++) {
		for (int c = 0; c < numcols; c++)
			mThumbnailPositions.push_back(wxPoint(mMargin + (mThumbnailSize + mMargin) * c + additional_pad,
											mMargin + (mThumbnailSize + mMargin) * r));
	}
}

// transform an ShapesFrame to a wxBitmap thumbnail
wxBitmap SequenceView::CreateThumbnail(ShapesFrame *fp)
{
	if (fp->BitmapIndex() < 0 || fp->BitmapIndex() >= (int)mBitmaps.size()) {
		// invalid or unset bitmap
		// FIXME we are rebuilding this wxBitmap each time! Build it once
		// and store it somewhere (rebuild upon tn_size change)
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

		// TODO apply transfer mode

		return ImageThumbnail(newimg, mThumbnailSize, true);
	}
}

void SequenceView::RebuildThumbnail(unsigned int i)
{
	if (i < mThumbnails.size() && i < mFrameIndexes->size()) {
		if ((*mFrameIndexes)[i] >= 0 && (*mFrameIndexes)[i] < (int)mFrames.size())
			mThumbnails[i] = CreateThumbnail(mFrames[(*mFrameIndexes)[i]]);
		else
			mThumbnails[i] = BadThumbnail(mThumbnailSize);
	}
}

// redecode frames to bitmaps (after color table change, frame parameter variations etc)
void SequenceView::RebuildThumbnails(void)
{
	mThumbnails.clear();
	if (mFrameIndexes != NULL) {
		for (unsigned int i = 0; i < mFrameIndexes->size(); i++) {
			if ((*mFrameIndexes)[i] >= 0 && (*mFrameIndexes)[i] < (int)mFrames.size())
				mThumbnails.push_back(CreateThumbnail(mFrames[(*mFrameIndexes)[i]]));
			else
				mThumbnails.push_back(BadThumbnail(mThumbnailSize));
		}
	}
}


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
	ctable(NULL), selection(-1), num_cols(0), num_rows(0), frozen_count(0)
{
	SetBackgroundColour(wxColour(255, 255, 255));
	tn_pen.SetColour(200, 200, 200);
	selection_pen.SetColour(0, 0, 0);
	selection_pen.SetWidth(3);
	invisible_pen.SetColour(0, 0, 0);
	invisible_pen.SetStyle(wxTRANSPARENT);
	SetScrollRate(0, 2);
	tn_size = 64;
	margin = 7;
	white_transparency = true;
	auto_size = false;
}

void BitmapBrowser::OnPaint(wxPaintEvent& e)
{
	wxPaintDC   tempdc(this);
	int			cw, ch, rx, ry;

	DoPrepareDC(tempdc);
	GetClientSize(&cw, &ch);
	CalcUnscrolledPosition(0, 0, &rx, &ry);
	// draw thumbnails
	tempdc.SetPen(tn_pen);
	tempdc.SetBrush(*wxTRANSPARENT_BRUSH);
	for (int i = 0; i < (int)shp_bitmaps.size(); i++) {
		int	x = tn_positions[i].x,
			y = tn_positions[i].y;

		if (y + tn_size < ry)
			continue;
		if (y > ry + ch)
			break;
			
		int	bw = thumbnails[i].GetWidth(),
			bh = thumbnails[i].GetHeight();

		if (i == selection) {
			tempdc.DrawBitmap(thumbnails[i], x + tn_size/2 - bw/2, y + tn_size/2 - bh/2);
			tempdc.SetPen(selection_pen);
			tempdc.DrawRectangle(x-2, y-2, tn_size+4, tn_size+4);
			tempdc.SetPen(tn_pen);
		} else {
			tempdc.DrawRectangle(x-1, y-1, tn_size+2, tn_size+2);
			tempdc.DrawBitmap(thumbnails[i], x + tn_size/2 - bw/2, y + tn_size/2 - bh/2);
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

				for (unsigned int i = 0; i < tn_positions.size(); i++) {
					wxRect	test(tn_positions[i].x, tn_positions[i].y, tn_size, tn_size);

					if (test.Inside(mouse)) {
						new_selection = i;
						break;
					}
				}
				if (new_selection != selection) {
					selection = new_selection;
					Refresh();

					// send selection event
					wxCommandEvent	event(wxEVT_BITMAPBROWSER, GetId());

					event.SetEventObject(this);
					event.SetInt(selection);
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
				int	new_selection = selection;

				if (selection >= 0 && selection < (int)shp_bitmaps.size()) {
					switch (e.GetKeyCode()) {
						case WXK_LEFT:
							if (selection % num_cols > 0)
								new_selection--;
							break;
						case WXK_RIGHT:
							if (selection % num_cols < (num_cols-1))
								new_selection++;
							break;
						case WXK_UP:
							if (selection / num_cols > 0)
								new_selection -= num_cols;
							break;
						case WXK_DOWN:
							if (selection / num_cols < (num_rows-1))
								new_selection += num_cols;
							break;
					}
				} else if (shp_bitmaps.size() > 0) {
					new_selection = 0;
				}
				if (new_selection != selection && new_selection >= 0
						&& new_selection < (int)shp_bitmaps.size()) {
					// TODO scroll to show the new selection
					selection = new_selection;
					Refresh();

					// send bitmap selection event
					wxCommandEvent	event(wxEVT_BITMAPBROWSER, GetId());

					event.SetEventObject(this);
					event.SetInt(selection);
					GetEventHandler()->ProcessEvent(event);
				}
			}
			break;
		case WXK_DELETE:
			// send a bitmap delete event
			if (selection >= 0 && selection < (int)shp_bitmaps.size()) {
				wxCommandEvent	event(wxEVT_BITMAPBROWSER_DELETE, GetId());

				event.SetEventObject(this);
				event.SetInt(selection);
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
	frozen_count++;
}

void BitmapBrowser::Thaw(void)
{
	if (frozen_count > 0) {
		frozen_count--;
		if (frozen_count == 0) {
			UpdateVirtualSize();
			Refresh();
		}
	}
}

int BitmapBrowser::GetSelection(void) const
{
	return selection;
}

// set the thumbnail size in pixels. Specify -1 to enable best-fit mode.
void BitmapBrowser::SetThumbnailSize(int size)
{
	if (size > 0) {
		tn_size = size;
		auto_size = false;
	} else {
		auto_size = true;
	}
	if (frozen_count == 0) {
		UpdateVirtualSize();
		if (!auto_size)
			RebuildThumbnails();
		Refresh();
	}
}

void BitmapBrowser::SetTranspPixelsDisplay(bool show)
{
	white_transparency = show;
	if (frozen_count == 0) {
		RebuildThumbnails();
		Refresh();
	}
}

// add a new ShapesBitmap to the thumbnail list
void BitmapBrowser::AddBitmap(ShapesBitmap *bp)
{
	if (bp != NULL) {
		if (bp->Pixels() != NULL) {
			shp_bitmaps.push_back(bp);
			thumbnails.push_back(CreateThumbnail(bp));
			if (frozen_count == 0) {
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
	thumbnails.clear();
	shp_bitmaps.clear();
	tn_positions.clear();
	selection = -1;
	UpdateVirtualSize();
	if (frozen_count == 0)
		Refresh();
}

// call before adding bitmaps!
void BitmapBrowser::SetColorTable(ShapesColorTable *ct)
{
	ctable = ct;
	if (frozen_count == 0) {
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
	unsigned int	numbitmaps = shp_bitmaps.size();
	int				width, height;

	GetClientSize(&width, &height);
	if (numbitmaps < 1) {
		SetVirtualSize(0, 0);
		return;
	}

	if (auto_size) {
		// calculate the best tn_size
		// (its maximum value not requiring window scrolling)
		int	max_bitmap_dimension = 10,
			new_tn_size;

		SetScrollRate(0, 0);
		// find greatest dimension among all bitmaps
		for (unsigned int i = 0; i < numbitmaps; i++) {
			if (shp_bitmaps[i]->Width() > max_bitmap_dimension)
				max_bitmap_dimension = shp_bitmaps[i]->Width();
			if (shp_bitmaps[i]->Height() > max_bitmap_dimension)
				max_bitmap_dimension = shp_bitmaps[i]->Height();
		}
		// FIXME a better algorythm, without looping?
		for (new_tn_size = margin; ; new_tn_size++) {
			int	numcols = (width - margin) / (new_tn_size + margin),
				numrows = (numcols > 0) ? (numbitmaps / numcols) : numbitmaps;

			if (numrows * numcols < (int)numbitmaps)
				numrows++;
			int	total_height = numrows * (new_tn_size + margin) + margin;

			if (total_height > height || (new_tn_size + 2 * margin) > width) {
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
		if (new_tn_size != tn_size) {
			tn_size = new_tn_size;
			RebuildThumbnails();
		}
	} else {
		SetScrollRate(0, 2);
	}

	num_cols = (width - margin) / (tn_size + margin);
	num_rows = (num_cols > 0) ? (numbitmaps / num_cols) : numbitmaps;

	if (num_rows * num_cols < (int)numbitmaps)
		num_rows++;

	SetVirtualSize(width, num_rows * (tn_size + margin) + margin);

	// recalculate thumbnail positions
	int	x = margin,
		y = margin;

	tn_positions.clear();
	for (unsigned int i = 0; i < numbitmaps; i++) {
		tn_positions.push_back(wxPoint(x, y));

		x += tn_size + margin;
		if (x + tn_size + margin > width) {
			x = margin;
			y += tn_size + margin;
		}
	}
}

// transform an ShapesBitmap to a wxBitmap thumbnail
wxBitmap BitmapBrowser::CreateThumbnail(ShapesBitmap *bp)
{
	wxImage newimg;

	if (ctable)
		newimg = ShapesBitmapToImage(bp, ctable, white_transparency);
	return ImageThumbnail(newimg, tn_size, true);
}

void BitmapBrowser::RebuildThumbnail(unsigned int i)
{
	if (i < shp_bitmaps.size())
		thumbnails[i] = CreateThumbnail(shp_bitmaps[i]);
}

// just re-decode the ShapesBitmaps to their thumbnail previews,
// without touching window sizes or thumbnail positions.
// Useful to update the display at the end of SetColorTable or after
// altering the ShapesBitmaps
void BitmapBrowser::RebuildThumbnails(void)
{
	for (unsigned int i = 0; i < shp_bitmaps.size(); i++)
		thumbnails[i] = CreateThumbnail(shp_bitmaps[i]);
}


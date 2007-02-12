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
// A widget for displaying a list of ShpFrame's
// as a scrollable list of selectable thumbnails. Each thumbnail
// displays the frame's associated bitmap, altered with
// the specified mirror transformations.
// 
#include <iostream>
#include <math.h>
#include "wx/image.h"
#include "FrameBrowser.h"
#include "utilities.h"

DEFINE_EVENT_TYPE(wxEVT_FRAMEBROWSER)

BEGIN_EVENT_TABLE(FrameBrowser, wxScrolledWindow)
	EVT_PAINT(FrameBrowser::OnPaint)
	EVT_SIZE(FrameBrowser::OnSize)
	EVT_LEFT_DOWN(FrameBrowser::OnMouseDown)
	EVT_RIGHT_DOWN(FrameBrowser::OnMouseDown)
	EVT_KEY_DOWN(FrameBrowser::OnKeyDown)
END_EVENT_TABLE()

FrameBrowser::FrameBrowser(wxWindow *parent, wxWindowID id):
	wxScrolledWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER),
	ctable(NULL), num_cols(0), num_rows(0), frozen_count(0)
{
	SetBackgroundColour(wxColour(255, 255, 255));
	tn_pen.SetColour(200, 200, 200);
	selection_pen.SetColour(0, 0, 0);
	selection_pen.SetWidth(3);
	invisible_pen.SetColour(255, 255, 255);
	SetScrollRate(0, 2);
	tn_size = 64;
	auto_size = false;
	margin = 7;
	white_transparency = true;
	selection = -1;
}

void FrameBrowser::OnPaint(wxPaintEvent& e)
{
	wxPaintDC   tempdc(this);
	int			cw, ch, rx, ry;

	DoPrepareDC(tempdc);
	// fill with white
	GetClientSize(&cw, &ch);
	CalcUnscrolledPosition(0, 0, &rx, &ry);
	// draw thumbnails
	tempdc.SetPen(tn_pen);
	tempdc.SetBrush(*wxTRANSPARENT_BRUSH);
	for (unsigned int i = 0; i < thumbnails.size(); i++) {
		int	x = tn_positions[i].x,
			y = tn_positions[i].y;
			
		if (y + tn_size < ry)
			continue;
		if (y > ry + ch)
			break;

		int	bw = thumbnails[i].GetWidth(),
			bh = thumbnails[i].GetHeight();

		if ((int)i == selection) {
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
					wxCommandEvent	event(wxEVT_FRAMEBROWSER, GetId());

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

void FrameBrowser::OnKeyDown(wxKeyEvent &e)
{
	int	new_selection = selection;

	if (selection >= 0 && selection < (int)frames.size()) {
		switch (e.GetKeyCode()) {
			// arrow keys move the selection
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
			// clear deletes selected item
			case WXK_DELETE:
				// TODO send a "frame delete" event
				break;
			default:
				e.Skip();
		}
	} else if (frames.size() > 0) {
		new_selection = 0;
	}
	// TODO scroll to show the new selection
	if (new_selection != selection && new_selection >= 0 && new_selection < (int)frames.size()) {
		selection = new_selection;
		Refresh();

		// send selection event
		wxCommandEvent	event(wxEVT_FRAMEBROWSER, GetId());

		event.SetEventObject(this);
		event.SetInt(selection);
		GetEventHandler()->ProcessEvent(event);
	}
}

// the Freeze()/Thaw() combo is necessary to get a reasonably
// responsive interface. Otherwise we would be doing bursts
// of UpdateVirtualSize() and potentially RebuildThumbnails()
// every time the user changes collection
void FrameBrowser::Freeze(void)
{
	frozen_count++;
}

void FrameBrowser::Thaw(void)
{
	if (frozen_count > 0) {
		frozen_count--;
		if (frozen_count == 0) {
			UpdateVirtualSize();
			Refresh();
		}
	}
}

int FrameBrowser::GetSelection(void) const
{
	return selection;
}

void FrameBrowser::SetThumbnailSize(int size)
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

void FrameBrowser::SetTranspPixelsDisplay(bool show)
{
	white_transparency = show;
	if (frozen_count == 0) {
		RebuildThumbnails();
		Refresh();
	}
}

// add a new ShpFrame to the thumbnail list
void FrameBrowser::AddFrame(ShpFrame *fp)
{
	if (fp != NULL) {
		frames.push_back(fp);
		thumbnails.push_back(CreateThumbnail(fp));
		if (frozen_count == 0) {
			UpdateVirtualSize();
			Refresh();
		}
	}
}

// add a ShpBitmap to the bitmap pointer list.
// Call before adding frames!
void FrameBrowser::AddBitmap(ShpBitmap *bp)
{
	if (bp != NULL) {
		if (bp->pixels != NULL)
			bitmaps.push_back(bp);
		else
			std::cerr << "FrameBrowser: someone tried to add a bitmap with NULL pixels\n";
	}
}

// clear the thumbnail list
void FrameBrowser::Clear(void)
{
	thumbnails.clear();
	frames.clear();
	bitmaps.clear();
	tn_positions.clear();
	selection = -1;
	if (frozen_count == 0) {
		UpdateVirtualSize();
		Refresh();
	}
}

// call before adding frames!
void FrameBrowser::SetColorTable(ShpColorTable *ct)
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
void FrameBrowser::UpdateVirtualSize(void)
{
	wxClientDC	dc(this);
	int			numframes = thumbnails.size(),
				width, height;

	GetClientSize(&width, &height);
	if (numframes < 1) {
		SetVirtualSize(0, 0);
		return;
	}

	if (auto_size && numframes > 0) {
		// calculate the best tn_size
		// (its maximum value not requiring window scrolling)
		int	max_bitmap_dimension = 10,
			new_tn_size;

		SetScrollRate(0, 0);
		// find greatest dimension among all referenced bitmaps
		for (unsigned int i = 0; i < frames.size(); i++) {
			int	bitmapindex = frames[i]->bitmap_index;

			if (bitmapindex < 0 || bitmapindex >= (int)bitmaps.size())
				continue;
			if (bitmaps[bitmapindex]->width > max_bitmap_dimension)
				max_bitmap_dimension = bitmaps[bitmapindex]->width;
			if (bitmaps[bitmapindex]->height > max_bitmap_dimension)
				max_bitmap_dimension = bitmaps[bitmapindex]->height;
		}
		// start with a small size (margin) and increase it until overflow
		for (new_tn_size = margin; ; new_tn_size++) {
			int	numcols = (width - margin) / (new_tn_size + margin),
				numrows = (numcols > 0) ? (numframes / numcols) : numframes;

			if (numrows * numcols < numframes)
				numrows++;
			int	total_height = numrows * (new_tn_size + margin) + margin;

			if (total_height > height || (new_tn_size + 2 * margin) > width) {
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
		if (new_tn_size != tn_size) {
			tn_size = new_tn_size;
			RebuildThumbnails();
		}
	} else {
		SetScrollRate(0, 2);
	}

	num_cols = (width - margin) / (tn_size + margin);
	num_rows = (num_cols > 0) ? (numframes / num_cols) : numframes;

	if (num_rows * num_cols < numframes)
		num_rows++;

	SetVirtualSize(width, num_rows * (tn_size + margin) + margin);

	// recalculate thumbnail positions
	int	x = margin,
		y = margin;

	tn_positions.clear();
	for (int i = 0; i < numframes; i++) {
		tn_positions.push_back(wxPoint(x, y));

		x += tn_size + margin;
		if (x + tn_size + margin > width) {
			x = margin;
			y += tn_size + margin;
		}
	}
}

// transform an ShpFrame to a wxBitmap thumbnail
wxBitmap FrameBrowser::CreateThumbnail(ShpFrame *fp)
{
	if (fp->bitmap_index < 0 || fp->bitmap_index >= (int)bitmaps.size()) {
		// invalid or unset bitmap
		return BadThumbnail(tn_size);
	} else {
		// valid bitmap
		ShpBitmap	*bp = bitmaps[fp->bitmap_index];
		wxImage		newimg(bp->width, bp->height);

		// decode the bitmap to a wxImage
		if (ctable)
			newimg = ShpBitmapToImage(bp, ctable, white_transparency);

		// apply frame transformations
		if (fp->x_mirror)
			newimg = newimg.Mirror(true);
		if (fp->y_mirror)
			newimg = newimg.Mirror(false);

		return ImageThumbnail(newimg, tn_size, true);
	}
}

void FrameBrowser::RebuildThumbnail(unsigned int i)
{
	if (i < thumbnails.size() && i < frames.size())
		thumbnails[i] = CreateThumbnail(frames[i]);
}

// redecode frames to bitmaps (after color table change, frame parameter variations etc)
void FrameBrowser::RebuildThumbnails(void)
{
	for (unsigned int i = 0; i < frames.size(); i++)
		thumbnails[i] = CreateThumbnail(frames[i]);
}


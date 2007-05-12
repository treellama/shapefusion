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
	wxScrolledWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER),
	ctable(NULL), tn_size(64), margin(7), prev_btn(arrow_left_bits, 4, 7),
	next_btn(arrow_right_bits, 4, 7), auto_size(false), selection(-1), frame_indexes(NULL)
{
	SetBackgroundColour(wxColour(255, 255, 255));
	tn_pen.SetColour(200, 200, 200);
	selection_pen.SetColour(0, 0, 0);
	selection_pen.SetWidth(3);
	invisible_pen.SetColour(255, 255, 255);
	angle_brush.SetColour(200, 200, 200);
	SetScrollRate(2, 2);
	angle_label_space = 30;	// TODO get this from real font extent
	white_transparency = true;
	number_of_views = 0;
	frames_per_view = 0;
	animation_type = UNANIMATED;
}

void SequenceView::OnPaint(wxPaintEvent& e)
{
	wxPaintDC   tempdc(this);
	wxPoint		mouse;
	int			cw, ch, rx, ry;

	if (frame_indexes == NULL)
		return;

	DoPrepareDC(tempdc);
	// fill with white
	GetClientSize(&cw, &ch);
	CalcUnscrolledPosition(0, 0, &rx, &ry);
	tempdc.SetPen(invisible_pen);
	// get mouse position in scrolled coords
	mouse = ScreenToClient(wxGetMousePosition());
	mouse.x += rx;
	mouse.y += ry;
	// draw thumbnails
	tempdc.SetPen(tn_pen);
	tempdc.SetBrush(*wxTRANSPARENT_BRUSH);
	tempdc.SetTextForeground(wxColour(0, 0, 0));
	tempdc.SetBackgroundMode(wxTRANSPARENT);
	for (unsigned int i = 0; i < thumbnails.size(); i++) {
		int	x = tn_positions[i].x,
			y = tn_positions[i].y,
			bw = thumbnails[i].GetWidth(),
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
		// draw arrow buttons if mouse is over this thumbnail
		wxRect	tnrect(x, y, tn_size, tn_size);

		if (tnrect.Inside(mouse)) {
			wxString	label = wxString::Format(wxT("%d"), (*frame_indexes)[i]);
			int			labelw, labelh;

			tempdc.SetFont(*wxSMALL_FONT);
			tempdc.DrawBitmap(prev_btn, x + 2, y + tn_size - prev_btn.GetHeight() - 2);
			tempdc.DrawBitmap(next_btn, x + tn_size - next_btn.GetWidth() - 2, y + tn_size - next_btn.GetHeight() - 2);
			tempdc.GetTextExtent(label, &labelw, &labelh);
			tempdc.SetTextForeground(wxColour(255, 255, 255));
			tempdc.DrawText(label, x + (tn_size-labelw) / 2 - 1, y + tn_size - labelh - 2);
			tempdc.DrawText(label, x + (tn_size-labelw) / 2 + 1, y + tn_size - labelh - 2);
			tempdc.DrawText(label, x + (tn_size-labelw) / 2, y + tn_size - labelh - 2 - 1);
			tempdc.DrawText(label, x + (tn_size-labelw) / 2, y + tn_size - labelh - 2 + 1);
			tempdc.SetTextForeground(wxColour(0, 0, 0));
			tempdc.DrawText(label, x + (tn_size-labelw) / 2, y + tn_size - labelh - 2);
		}
	}
	if (thumbnails.size() > 0 && animation_type != UNANIMATED && animation_type != ANIMATED_1) {
		// draw angle labels
		char		*deg = "°";
		wxString	deg2(deg, wxConvISO8859_1);	// FIXME doesn't work on OS X (draws infinity char)

		tempdc.SetFont(*wxSMALL_FONT);
		tempdc.SetBrush(angle_brush);
		for (int i = 0; i < number_of_views; i++) {
			wxString	label = wxString::Format(wxT("%d"), i * 360 / number_of_views) + deg2;
			int			tw, th;
	
			tempdc.GetTextExtent(label, &tw, &th);
			tempdc.DrawRectangle(margin-1, margin + i * (tn_size + margin) - 1, angle_label_space+2, tn_size+2);
			tempdc.DrawText(label, margin + angle_label_space - tw, margin + i * (tn_size + margin) + tn_size/2 - th/2);
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
					wxCommandEvent	event(wxEVT_SEQUENCEVIEW, GetId());

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
				
				for (unsigned int i = 0; i < tn_positions.size(); i++) {
					wxRect  test(tn_positions[i].x, tn_positions[i].y, tn_size, tn_size);
					
					if (test.Inside(mouse)) {
						touched_thumbnail = i;
						break;
					}
				}
				if (touched_thumbnail > -1) {
					// find wether an arrow was touched
					int     x = tn_positions[touched_thumbnail].x,
							y = tn_positions[touched_thumbnail].y;
					wxRect  prev_rect(x + 2, y + tn_size - prev_btn.GetHeight() - 2,
								prev_btn.GetWidth(), prev_btn.GetHeight()),
							next_rect(x + tn_size - next_btn.GetWidth() - 2, y + tn_size - next_btn.GetHeight() - 2,
								prev_btn.GetWidth(), prev_btn.GetHeight());
					
					if (prev_rect.Inside(mouse)) {
						if ((*frame_indexes)[touched_thumbnail] > -1) {
							(*frame_indexes)[touched_thumbnail]--;
							RebuildThumbnail(touched_thumbnail);
							Refresh();
						}
					} else if (next_rect.Inside(mouse)) {
						if ((*frame_indexes)[touched_thumbnail] < ((int)frames.size()-1)) {
							(*frame_indexes)[touched_thumbnail]++;
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
				if (selection >= 0) {
					if ((*frame_indexes)[selection] > -1) {
						(*frame_indexes)[selection]--;
						RebuildThumbnail(selection);
						Refresh();
					}
				}
				break;
			case WXK_RIGHT:
				if (selection >= 0) {
					if ((*frame_indexes)[selection] < ((int)frames.size()-1)) {
						(*frame_indexes)[selection]++;
						RebuildThumbnail(selection);
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
		int	new_selection = selection;

		if (selection >= 0 && selection < (number_of_views * frames_per_view)) {
			switch (e.GetKeyCode()) {
				case WXK_LEFT:
					if (selection % frames_per_view > 0)
						new_selection--;
					break;
				case WXK_RIGHT:
					if (selection % frames_per_view < (frames_per_view-1))
						new_selection++;
					break;
				case WXK_UP:
					if (selection / frames_per_view > 0)
						new_selection -= frames_per_view;
					break;
				case WXK_DOWN:
					if (selection / frames_per_view < (number_of_views-1))
						new_selection += frames_per_view;
					break;
				default:
					e.Skip();
			}
		} else if (number_of_views * frames_per_view > 0) {
			new_selection = 0;
		}
		// TODO scroll to show the new position
		if (new_selection != selection) {
			selection = new_selection;
			Refresh();
			
			// send selection event
			wxCommandEvent	event(wxEVT_SEQUENCEVIEW, GetId());

			event.SetEventObject(this);
			event.SetInt(selection);
			GetEventHandler()->ProcessEvent(event);
		}
	}
}

int SequenceView::GetSelection(void) const
{
	return selection;
}

void SequenceView::SetThumbnailSize(int size)
{
	if (size > 0) {
		tn_size = size;
		auto_size = false;
	} else {
		auto_size = true;
	}
	UpdateVirtualSize();
	if (!auto_size)
		RebuildThumbnails();
	Refresh();
}

void SequenceView::SetTranspPixelsDisplay(bool show)
{
	white_transparency = show;
	RebuildThumbnails();
	Refresh();
}

// add a new ShapesFrame to the thumbnail list
void SequenceView::AddFrame(ShapesFrame *fp)
{
	if (fp != NULL)
		frames.push_back(fp);
}

// add a ShapesBitmap to the bitmap pointer list. Call before adding frames!
void SequenceView::AddBitmap(ShapesBitmap *bp)
{
	if (bp != NULL) {
		if (bp->Pixels() != NULL)
			bitmaps.push_back(bp);
		else
			std::cerr << "SequenceView: someone tried to add a bitmap with NULL pixels\n";
	}
}

// set sequence parameters: frames per view, animation type and frame index array.
// Guess number of views from animation type. Also create thumbnails (add frames,
// bitmaps and set color table before!)
void SequenceView::SetSeqParameters(int animtype, int fpv, vector<short> *indexes)
{
	animation_type = animtype;
	number_of_views = ActualNumberOfViews(animtype);
	frames_per_view = fpv;
	frame_indexes = indexes;
	RebuildThumbnails();
	UpdateVirtualSize();
	Refresh();
}

// clear the thumbnail list
void SequenceView::Clear(void)
{
	thumbnails.clear();
	frames.clear();
	bitmaps.clear();
	tn_positions.clear();
	selection = -1;
	UpdateVirtualSize();
	Refresh();
}

// call before adding frames!
void SequenceView::SetColorTable(ShapesColorTable *ct)
{
	ctable = ct;
	RebuildThumbnails();
	Refresh();
}

// calculate and set the wxWindow virtual size, based on the
// number of thumbnails, thumbnail dimensions and given visible
// size. Also pre-calculate thumbnail positions
// FIXME behave like a FrameBrowser if animation_type == UNANIMATED,
// because frames are just to be chosen randomly and not really
// an animation. 
void SequenceView::UpdateVirtualSize(void)
{
	wxClientDC	dc(this);
	int			numframes = thumbnails.size(),
				width, height,
				additional_pad = (animation_type == UNANIMATED || animation_type == ANIMATED_1) ? 0 : (angle_label_space + margin);

	if (numframes < 1 || frame_indexes == NULL) {
		SetVirtualSize(0, 0);
		return;
	}
	GetClientSize(&width, &height);
	if (auto_size && numframes > 0) {
		// calculate the best tn_size
		// (its maximum value not requiring window scrolling)
		int	max_bitmap_dimension = 0,
			new_tn_size;

		SetScrollRate(0, 0);
		// find greatest dimension among all referenced bitmaps
		for (unsigned int i = 0; i < frame_indexes->size(); i++) {
			int	frame_index = (*frame_indexes)[i];

			if (frame_index < 0 || frame_index >= (int)frames.size())
				continue;

			int	bitmap_index = frames[frame_index]->BitmapIndex();

			if (bitmap_index < 0 || bitmap_index >= (int)bitmaps.size())
				continue;
			if (bitmaps[bitmap_index]->Width() > max_bitmap_dimension)
				max_bitmap_dimension = bitmaps[bitmap_index]->Width();
			if (bitmaps[bitmap_index]->Height() > max_bitmap_dimension)
				max_bitmap_dimension = bitmaps[bitmap_index]->Height();
		}
		// start with a minimum size and increase it until overflow
		for (new_tn_size = 2*margin; ; new_tn_size++) {
			int	numcols = frames_per_view,
				numrows = number_of_views,
				total_width = numcols * (new_tn_size + margin) + margin + additional_pad,
				total_height = numrows * (new_tn_size + margin) + margin;

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
		if (new_tn_size != tn_size) {
			tn_size = new_tn_size;
			RebuildThumbnails();
		}
	} else {
		SetScrollRate(2, 2);
	}

	// now row & column count is simply number_of_views and frames_per_view,
	// not the mess needed in the FrameBrowser
	int	numcols = frames_per_view,
		numrows = number_of_views;

	SetVirtualSize(numcols * (tn_size + margin) + margin + additional_pad, numrows * (tn_size + margin) + margin);

	// recalculate thumbnail positions
	tn_positions.clear();
	for (int r = 0; r < numrows; r++) {
		for (int c = 0; c < numcols; c++)
			tn_positions.push_back(wxPoint(margin + (tn_size + margin) * c + additional_pad, margin + (tn_size + margin) * r));
	}
}

// transform an ShapesFrame to a wxBitmap thumbnail
wxBitmap SequenceView::CreateThumbnail(ShapesFrame *fp)
{
	if (fp->BitmapIndex() < 0 || fp->BitmapIndex() >= (int)bitmaps.size()) {
		// invalid or unset bitmap
		// FIXME we are rebuilding this wxBitmap each time! Build it once
		// and store it somewhere (rebuild upon tn_size change)
		return BadThumbnail(tn_size);
	} else {
		// valid bitmap
		ShapesBitmap	*bp = bitmaps[fp->BitmapIndex()];
		wxImage		newimg(bp->Width(), bp->Height());

		// decode the bitmap to a wxImage
		if (ctable)
			newimg = ShapesBitmapToImage(bp, ctable, white_transparency);

		// apply frame transformations
		if (fp->IsXmirrored())
			newimg = newimg.Mirror(true);
		if (fp->IsYmirrored())
			newimg = newimg.Mirror(false);

		// TODO apply transfer mode

		return ImageThumbnail(newimg, tn_size, true);
	}
}

void SequenceView::RebuildThumbnail(unsigned int i)
{
	if (i < thumbnails.size() && i < frame_indexes->size()) {
		if ((*frame_indexes)[i] >= 0 && (*frame_indexes)[i] < (int)frames.size())
			thumbnails[i] = CreateThumbnail(frames[(*frame_indexes)[i]]);
		else
			thumbnails[i] = BadThumbnail(tn_size);
	}
}

// redecode frames to bitmaps (after color table change, frame parameter variations etc)
void SequenceView::RebuildThumbnails(void)
{
	thumbnails.clear();
	if (frame_indexes != NULL) {
		for (unsigned int i = 0; i < frame_indexes->size(); i++) {
			if ((*frame_indexes)[i] >= 0 && (*frame_indexes)[i] < (int)frames.size())
				thumbnails.push_back(CreateThumbnail(frames[(*frame_indexes)[i]]));
			else
				thumbnails.push_back(BadThumbnail(tn_size));
		}
	}
}


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
#include "FrameView.h"
#include "utilities.h"

BEGIN_EVENT_TABLE(FrameView, wxScrolledWindow)
	EVT_PAINT(FrameView::OnPaint)
	EVT_LEFT_DOWN(FrameView::OnDrag)
	EVT_LEFT_UP(FrameView::OnDrag)
	EVT_MOTION(FrameView::OnDrag)
	EVT_SIZE(FrameView::OnSize)
END_EVENT_TABLE()

FrameView::FrameView(wxWindow *parent):
	wxScrolledWindow(parent, -1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER | wxFULL_REPAINT_ON_RESIZE),
	frame(NULL), enc_bmp(NULL), ctable(NULL), panning(false), dragging_origin(false), dragging_key(false), near_origin(false), near_key(false)
{
	SetBackgroundColour(wxColour(255, 255, 255));
	SetScrollRate(1, 1);
	EnableScrolling(false, false);
	// origin cursor pen
	origin_pen.SetColour(0, 128, 128);
	// key cursor pen
	key_pen.SetColour(128, 0, 128);
	// invisible pen
	invisible_pen.SetColour(255, 255, 255);
	// mouse cursors
	pan_cursor = wxCursor(wxCURSOR_ARROW);
	point_cursor = wxCursor(wxCURSOR_CROSS);
	white_transparency = true;
}

void FrameView::OnPaint(wxPaintEvent& e)
{
	wxPaintDC   tempdc(this);
	int			vw, vh, cw, ch, rx, ry;

	GetVirtualSize(&vw, &vh);
	DoPrepareDC(tempdc);
	// fill with white
	GetClientSize(&cw, &ch);
	CalcUnscrolledPosition(0, 0, &rx, &ry);
	tempdc.SetPen(invisible_pen);
	if (frame != NULL && ctable != NULL) {
		if (enc_bmp != NULL) {
			int		origin_x = vw/2 - dec_bmp.GetWidth()/2,
					origin_y = vh/2 - dec_bmp.GetHeight()/2;

			// draw bitmap
			tempdc.DrawBitmap(dec_bmp, origin_x, origin_y);
			// mark bitmap origin
			tempdc.SetPen(origin_pen);
			tempdc.CrossHair(frame->OriginX() + origin_x, frame->OriginY() + origin_y);
			tempdc.DrawCircle(frame->OriginX() + origin_x, frame->OriginY() + origin_y, 2);
			// mark bitmap keypoint
			tempdc.SetPen(key_pen);
			tempdc.CrossHair(frame->KeyX() + origin_x, frame->KeyY() + origin_y);
			tempdc.DrawCircle(frame->KeyX() + origin_x, frame->KeyY() + origin_y, 2);
			// origin & key labels
			wxString	origin_label = wxT("Origin"),
						key_label = wxT("Keypoint");
			int			text1w, text1h, text2w, text2h;

			tempdc.SetFont(*wxSMALL_FONT);
			tempdc.GetTextExtent(origin_label, &text1w, &text1h);
			tempdc.GetTextExtent(key_label, &text2w, &text2h);
			tempdc.SetTextForeground(wxColour(0, 128, 128));
			if (frame->OriginY() + origin_y >= text1h + 2)
				tempdc.DrawText(origin_label, vw - text1w - 2, frame->OriginY() + origin_y - text1h - 2);
			else
				tempdc.DrawText(origin_label, vw - text1w - 2, frame->OriginY() + origin_y + 2);
			tempdc.SetTextForeground(wxColour(128, 0, 128));
			if (frame->KeyY() + origin_y >= text2h + 2)
				tempdc.DrawText(key_label, vw - text2w - 2, frame->KeyY() + origin_y - text2h - 2);
			else
				tempdc.DrawText(key_label, vw - text2w - 2, frame->KeyY() + origin_y + 2);
		} else {
			wxString	no_bmp_label = wxT("No associated bitmap");
			int			text_w, text_h,
						origin_x = vw/2, origin_y = vh/2;

			tempdc.GetTextExtent(no_bmp_label, &text_w, &text_h);
			origin_x -= text_w / 2;
			origin_y -= text_h / 2;
			tempdc.SetTextForeground(wxColour(255, 0, 0));
			tempdc.DrawText(no_bmp_label, origin_x, origin_y);
		}
	}
}

// handle mouse drag (pan view, edit origin/keypoint)
void FrameView::OnDrag(wxMouseEvent &e)
{
	if (e.ButtonDown()) {
		int scroll_x, scroll_y;

		GetViewStart(&scroll_x, &scroll_y);
		if (near_origin) {
			// start dragging origin
			dragging_origin = true;
		} else if (near_key) {
			// start dragging keypoint
			dragging_key = true;
		} else {
			// start panning
			panning = true;
			drag_start_x = e.GetPosition().x + scroll_x;
			drag_start_y = e.GetPosition().y + scroll_y;
		}
	} else if (e.Dragging()) {
		if (panning) {
			// pan
			int	dx = drag_start_x - e.GetPosition().x,
				dy = drag_start_y - e.GetPosition().y;

			Scroll(dx, dy);
		} else if (dragging_origin) {
			// set origin
			int	vw, vh, scroll_x, scroll_y, origin_x, origin_y;

			GetVirtualSize(&vw, &vh);
			GetViewStart(&scroll_x, &scroll_y);
			origin_x = vw/2 - dec_bmp.GetWidth()/2;
			origin_y = vh/2 - dec_bmp.GetHeight()/2;
			frame->SetOriginX(e.GetPosition().x + scroll_x - origin_x);
			frame->SetOriginY(e.GetPosition().y + scroll_y - origin_y);
			// TODO generate event
			Refresh();
		} else if (dragging_key) {
			// set keypoint
			int vw, vh, scroll_x, scroll_y, origin_x, origin_y;

			GetVirtualSize(&vw, &vh);
			GetViewStart(&scroll_x, &scroll_y);
			origin_x = vw/2 - dec_bmp.GetWidth()/2;
			origin_y = vh/2 - dec_bmp.GetHeight()/2;
			frame->SetKeyX(e.GetPosition().x + scroll_x - origin_x);
			frame->SetKeyY(e.GetPosition().y + scroll_y - origin_y);
			// TODO generate event
			Refresh();
		}
	} else if (e.ButtonUp()) {
		panning = dragging_origin = dragging_key = false;
	} else if (e.Moving()) {
		// mouse moved without buttons
		int	vw, vh, scroll_x, scroll_y;

		GetVirtualSize(&vw, &vh);
		GetViewStart(&scroll_x, &scroll_y);

		int origin_x = vw/2 - dec_bmp.GetWidth()/2,
			origin_y = vh/2 - dec_bmp.GetHeight()/2,
			x = e.GetPosition().x + scroll_x,
			y = e.GetPosition().y + scroll_y,
			delta_x,
			delta_y,
			dist_origin,
			dist_keypoint;

		delta_x = x - (frame->OriginX() + origin_x);
		delta_y = y - (frame->OriginY() + origin_y);
		dist_origin = delta_x*delta_x + delta_y*delta_y;
		delta_x = x - (frame->KeyX() + origin_x);
		delta_y = y - (frame->KeyY() + origin_y);
		dist_keypoint = delta_x*delta_x + delta_y*delta_y;
		// are we near origin or keypoint?
		if (dist_origin < 25) {
			SetCursor(point_cursor);
			near_origin = true;
			near_key = false;
		} else if (dist_keypoint < 25) {
			SetCursor(point_cursor);
			near_origin = false;
			near_key = true;
		} else {
			SetCursor(pan_cursor);
			near_origin = near_key = false;
		}
	}
}

void FrameView::OnSize(wxSizeEvent &e)
{
	int cw, ch,
		vw = (frame == NULL || enc_bmp == NULL) ? 0 : enc_bmp->Width(),
		vh = (frame == NULL || enc_bmp == NULL) ? 0 : enc_bmp->Height();

	GetClientSize(&cw, &ch);
	if (enc_bmp != NULL && enc_bmp->Pixels() != NULL) {
		if (vw < cw)
			vw = cw;
		if (vh < ch)
			vh = ch;
	} else {
		vw = cw;
		vh = ch;
	}
	SetVirtualSize(vw, vh);
}

void FrameView::SetTranspPixelsDisplay(bool show)
{
	white_transparency = show;
	SetBitmap(enc_bmp);
	Refresh();
}

// associate a frame
void FrameView::SetFrame(ShapesFrame *fp)
{
	frame = fp;
	SetBitmap(enc_bmp);
	Refresh();
}

ShapesFrame *FrameView::GetFrame(void) const
{
	return frame;
}

// associate a bitmap to display (we could do this in SetFrame
// but we would need a pointer to the original Shapes object)
void FrameView::SetBitmap(ShapesBitmap *bp)
{
	int	cw, ch;

	GetClientSize(&cw, &ch);
	enc_bmp = bp;
	if (bp != NULL) {
		if (bp->Pixels() != NULL) {
			// adjust sizes
			int	vw = enc_bmp->Width(),
				vh = enc_bmp->Height();

			if (vw < cw)
				vw = cw;
			if (vh < ch)
				vh = ch;
			SetVirtualSize(vw, vh);
			// decode bitmap
			if (ctable != NULL) {
				wxImage	img = ShapesBitmapToImage(bp, ctable, white_transparency);

				// apply transformations
				if (frame) {
					if (frame->IsXmirrored())
						img = img.Mirror(true);
					if (frame->IsYmirrored())
						img = img.Mirror(false);
				}
				dec_bmp = wxBitmap(img);
			}
			Refresh();
		} else {
			wxLogError(wxT("[FrameView] Added a bitmap with NULL pixels"));
			SetVirtualSize(cw, ch);
		}
	} else {
		SetVirtualSize(cw, ch);
	}
}

// call this before Set'tingBitmap!
void FrameView::SetColorTable(ShapesColorTable *ct)
{
	ctable = ct;
	SetBitmap(enc_bmp);
	Refresh();
}


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

DEFINE_EVENT_TYPE(wxEVT_FRAMEVIEW_DRAG)

BEGIN_EVENT_TABLE(FrameView, wxScrolledWindow)
	EVT_PAINT(FrameView::OnPaint)
	EVT_LEFT_DOWN(FrameView::OnDrag)
	EVT_LEFT_UP(FrameView::OnDrag)
	EVT_MOTION(FrameView::OnDrag)
	EVT_SIZE(FrameView::OnSize)
END_EVENT_TABLE()

FrameView::FrameView(wxWindow *parent, wxWindowID id):
	wxScrolledWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER | wxFULL_REPAINT_ON_RESIZE),
	mFrame(NULL), mEncBmp(NULL), mColorTable(NULL), mPanning(false), mDraggingOrigin(false), mDraggingKey(false), mNearOrigin(false), mNearKey(false)
{
	SetBackgroundColour(wxColour(255, 255, 255));
	SetScrollRate(1, 1);
	EnableScrolling(false, false);
	// origin cursor pen
	mOriginPen.SetColour(0, 128, 128);
	// key cursor pen
	mKeypointPen.SetColour(128, 0, 128);
	// mouse cursors
	mPanCursor = wxCursor(wxCURSOR_ARROW);
	mPointCursor = wxCursor(wxCURSOR_CROSS);
	mWhiteTransparency = true;
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
	if (mFrame != NULL && mColorTable != NULL) {
		if (mEncBmp != NULL) {
			int		origin_x = vw/2 - mDecBmp.GetWidth()/2,
					origin_y = vh/2 - mDecBmp.GetHeight()/2;

			// draw bitmap
			tempdc.DrawBitmap(mDecBmp, origin_x, origin_y);
			// mark bitmap origin
			tempdc.SetPen(mOriginPen);
			tempdc.CrossHair(mFrame->OriginX() + origin_x, mFrame->OriginY() + origin_y);
			tempdc.DrawCircle(mFrame->OriginX() + origin_x, mFrame->OriginY() + origin_y, 2);
			// mark bitmap keypoint
			tempdc.SetPen(mKeypointPen);
			tempdc.CrossHair(mFrame->KeyX() + origin_x, mFrame->KeyY() + origin_y);
			tempdc.DrawCircle(mFrame->KeyX() + origin_x, mFrame->KeyY() + origin_y, 2);
			// origin & key labels
			wxString	origin_label = wxT("Origin"),
						key_label = wxT("Keypoint");
			int			text1w, text1h, text2w, text2h;

			tempdc.SetFont(*wxSMALL_FONT);
			tempdc.GetTextExtent(origin_label, &text1w, &text1h);
			tempdc.GetTextExtent(key_label, &text2w, &text2h);
			tempdc.SetTextForeground(wxColour(0, 128, 128));
			if (mFrame->OriginY() + origin_y >= text1h + 2)
				tempdc.DrawText(origin_label, vw - text1w - 2, mFrame->OriginY() + origin_y - text1h - 2);
			else
				tempdc.DrawText(origin_label, vw - text1w - 2, mFrame->OriginY() + origin_y + 2);
			tempdc.SetTextForeground(wxColour(128, 0, 128));
			if (mFrame->KeyY() + origin_y >= text2h + 2)
				tempdc.DrawText(key_label, vw - text2w - 2, mFrame->KeyY() + origin_y - text2h - 2);
			else
				tempdc.DrawText(key_label, vw - text2w - 2, mFrame->KeyY() + origin_y + 2);
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
	if (mFrame == NULL || mEncBmp == NULL)
		return;

	if (e.ButtonDown()) {
		int scroll_x, scroll_y;

		GetViewStart(&scroll_x, &scroll_y);
		if (mNearOrigin) {
			// start dragging origin
			mDraggingOrigin = true;
		} else if (mNearKey) {
			// start dragging keypoint
			mDraggingKey = true;
		} else {
			// start panning
			mPanning = true;
			mDragStartX = e.GetPosition().x + scroll_x;
			mDragStartY = e.GetPosition().y + scroll_y;
		}
	} else if (e.Dragging()) {
		if (mPanning) {
			// pan
			int	dx = mDragStartX - e.GetPosition().x,
				dy = mDragStartY - e.GetPosition().y;

			Scroll(dx, dy);
		} else if (mDraggingOrigin) {
			// set origin
			int	vw, vh, scroll_x, scroll_y, origin_x, origin_y;

			GetVirtualSize(&vw, &vh);
			GetViewStart(&scroll_x, &scroll_y);
			origin_x = vw/2 - mDecBmp.GetWidth()/2;
			origin_y = vh/2 - mDecBmp.GetHeight()/2;
			mFrame->SetOriginX(e.GetPosition().x + scroll_x - origin_x);
			mFrame->SetOriginY(e.GetPosition().y + scroll_y - origin_y);
			// send an event so that other gui elements can update
			wxCommandEvent  event(wxEVT_FRAMEVIEW_DRAG, GetId());

			event.SetEventObject(this);
			//event.SetInt(j);
			GetEventHandler()->ProcessEvent(event);
			Refresh();
		} else if (mDraggingKey) {
			// set keypoint
			int vw, vh, scroll_x, scroll_y, origin_x, origin_y;

			GetVirtualSize(&vw, &vh);
			GetViewStart(&scroll_x, &scroll_y);
			origin_x = vw/2 - mDecBmp.GetWidth()/2;
			origin_y = vh/2 - mDecBmp.GetHeight()/2;
			mFrame->SetKeyX(e.GetPosition().x + scroll_x - origin_x);
			mFrame->SetKeyY(e.GetPosition().y + scroll_y - origin_y);
			// send an event so that other gui elements can update
			wxCommandEvent  event(wxEVT_FRAMEVIEW_DRAG, GetId());

			event.SetEventObject(this);
			//event.SetInt(j);
			GetEventHandler()->ProcessEvent(event);
			Refresh();
		}
	} else if (e.ButtonUp()) {
		mPanning = mDraggingOrigin = mDraggingKey = false;
	} else if (e.Moving()) {
		// mouse moved without buttons
		int	vw, vh, scroll_x, scroll_y;

		GetVirtualSize(&vw, &vh);
		GetViewStart(&scroll_x, &scroll_y);

		int origin_x = vw/2 - mDecBmp.GetWidth()/2,
			origin_y = vh/2 - mDecBmp.GetHeight()/2,
			x = e.GetPosition().x + scroll_x,
			y = e.GetPosition().y + scroll_y,
			delta_x,
			delta_y,
			dist_origin,
			dist_keypoint;

		delta_x = x - (mFrame->OriginX() + origin_x);
		delta_y = y - (mFrame->OriginY() + origin_y);
		dist_origin = delta_x*delta_x + delta_y*delta_y;
		delta_x = x - (mFrame->KeyX() + origin_x);
		delta_y = y - (mFrame->KeyY() + origin_y);
		dist_keypoint = delta_x*delta_x + delta_y*delta_y;
		// are we near origin or keypoint?
		if (dist_origin < 25) {
			SetCursor(mPointCursor);
			mNearOrigin = true;
			mNearKey = false;
		} else if (dist_keypoint < 25) {
			SetCursor(mPointCursor);
			mNearOrigin = false;
			mNearKey = true;
		} else {
			SetCursor(mPanCursor);
			mNearOrigin = mNearKey = false;
		}
	}
}

void FrameView::OnSize(wxSizeEvent &e)
{
	int cw, ch,
		vw = (mFrame == NULL || mEncBmp == NULL) ? 0 : mEncBmp->Width(),
		vh = (mFrame == NULL || mEncBmp == NULL) ? 0 : mEncBmp->Height();

	GetClientSize(&cw, &ch);
	if (mEncBmp != NULL && mEncBmp->Pixels() != NULL) {
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
	mWhiteTransparency = show;
	SetBitmap(mEncBmp);
	Refresh();
}

// associate a frame
void FrameView::SetFrame(ShapesFrame *fp)
{
	mFrame = fp;
	SetBitmap(mEncBmp);
	Refresh();
}

ShapesFrame *FrameView::GetFrame(void) const
{
	return mFrame;
}

// associate a bitmap to display (we could do this in SetFrame
// but we would need a pointer to the original Shapes object)
void FrameView::SetBitmap(ShapesBitmap *bp)
{
	int	cw, ch;

	GetClientSize(&cw, &ch);
	mEncBmp = bp;
	if (bp != NULL) {
		if (bp->Pixels() != NULL) {
			// adjust sizes
			int	vw = mEncBmp->Width(),
				vh = mEncBmp->Height();

			if (vw < cw)
				vw = cw;
			if (vh < ch)
				vh = ch;
			SetVirtualSize(vw, vh);
			// decode bitmap
			if (mColorTable != NULL) {
				wxImage	img = ShapesBitmapToImage(bp, mColorTable, mWhiteTransparency);

				// apply transformations
				if (mFrame) {
					if (mFrame->IsXmirrored())
						img = img.Mirror(true);
					if (mFrame->IsYmirrored())
						img = img.Mirror(false);
				}
				mDecBmp = wxBitmap(img);
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
	mColorTable = ct;
	SetBitmap(mEncBmp);
	Refresh();
}


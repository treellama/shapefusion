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
#include <iostream>
#include <math.h>
#include "BitmapView.h"
#include "utilities.h"

BEGIN_EVENT_TABLE(BitmapView, wxScrolledWindow)
	EVT_PAINT(BitmapView::OnPaint)
	EVT_LEFT_DOWN(BitmapView::OnDrag)
	EVT_LEFT_UP(BitmapView::OnDrag)
	EVT_MOTION(BitmapView::OnDrag)
	EVT_SIZE(BitmapView::OnSize)
END_EVENT_TABLE()

BitmapView::BitmapView(wxWindow *parent): wxScrolledWindow(parent, -1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER),
	enc_bmp(NULL), ctable(NULL), dragging(false)
{
	SetBackgroundColour(wxColour(255, 255, 255));
	SetScrollRate(1, 1);
	white_transparency = true;
	invisible_pen.SetColour(255, 255, 255);
}

void BitmapView::OnPaint(wxPaintEvent& e)
{
	wxPaintDC   tempdc(this);
	int			vw, vh, cw, ch, rx, ry;

	GetVirtualSize(&vw, &vh);
	DoPrepareDC(tempdc);
	// fill with white
	GetClientSize(&cw, &ch);
	CalcUnscrolledPosition(0, 0, &rx, &ry);
	tempdc.SetPen(invisible_pen);
	if (enc_bmp != NULL && ctable != NULL)
		tempdc.DrawBitmap(dec_bmp, vw/2 - dec_bmp.GetWidth()/2, vh/2 - dec_bmp.GetHeight()/2);
}

// handle mouse drag (pan view)
void BitmapView::OnDrag(wxMouseEvent &e)
{
	if (e.ButtonDown()) {
		// start panning
		int scroll_x, scroll_y;

		GetViewStart(&scroll_x, &scroll_y);
		dragging = true;
		drag_start_x = e.GetPosition().x + scroll_x;
		drag_start_y = e.GetPosition().y + scroll_y;
	} else if (e.Dragging() && dragging) {
		// pan
		int	dx = drag_start_x - e.GetPosition().x,
			dy = drag_start_y - e.GetPosition().y;

		Scroll(dx, dy);
	} else if (e.ButtonUp()) {
		// end panning
		dragging = false;
	}
}

void BitmapView::OnSize(wxSizeEvent &e)
{
	int cw, ch,
		vw = (enc_bmp == NULL) ? 0 : enc_bmp->Width(),
		vh = (enc_bmp == NULL) ? 0 : enc_bmp->Height();
	
	GetClientSize(&cw, &ch);
	if (vw < cw)
		vw = cw;
	if (vh < ch)
		vh = ch;
	SetVirtualSize(vw, vh);
}

void BitmapView::SetTranspPixelsDisplay(bool show)
{
	white_transparency = show;
	if (enc_bmp != NULL && enc_bmp->Pixels() != NULL && ctable != NULL)
		dec_bmp = wxBitmap(ShapesBitmapToImage(enc_bmp, ctable, white_transparency));
	Refresh();
}

// add a new ShapesBitmap to the thumbnail list
void BitmapView::SetBitmap(ShapesBitmap *bp)
{
	enc_bmp = bp;
	if (bp != NULL) {
		if (bp->Pixels() != NULL) {
			// adjust sizes
			int	cw, ch,
				vw = enc_bmp->Width(),
				vh = enc_bmp->Height();

			GetClientSize(&cw, &ch);
			if (vw < cw)
				vw = cw;
			if (vh < ch)
				vh = ch;
			SetVirtualSize(vw, vh);
			// decode bitmap
			if (ctable != NULL)
				dec_bmp = wxBitmap(ShapesBitmapToImage(bp, ctable, white_transparency));
			Refresh();
		} else {
			wxLogError(wxT("[BitmapView] Addes a bitmap with NULL pixels"));
			SetVirtualSize(0, 0);
		}
	} else {
		SetVirtualSize(0, 0);
	}
}

ShapesBitmap *BitmapView::GetBitmap(void) const
{
	return enc_bmp;
}

// call this before SettingBitmap!
void BitmapView::SetColorTable(ShapesColorTable *ct)
{
	ctable = ct;
	if (enc_bmp != NULL && enc_bmp->Pixels() != NULL && ctable != NULL)
		dec_bmp = wxBitmap(ShapesBitmapToImage(enc_bmp, ctable, white_transparency));
	Refresh();
}


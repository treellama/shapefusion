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
#include "BitmapView.h"
#include "utilities.h"

BEGIN_EVENT_TABLE(BitmapView, wxScrolledWindow)
	EVT_PAINT(BitmapView::OnPaint)
	EVT_LEFT_DOWN(BitmapView::OnDrag)
	EVT_LEFT_UP(BitmapView::OnDrag)
	EVT_MOTION(BitmapView::OnDrag)
	EVT_SIZE(BitmapView::OnSize)
END_EVENT_TABLE()

BitmapView::BitmapView(wxWindow *parent):
	wxScrolledWindow(parent, -1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER | wxFULL_REPAINT_ON_RESIZE),
	mEncBmp(NULL), mColorTable(NULL), mDragging(false)
{
	SetBackgroundColour(wxColour(255, 255, 255));
	SetScrollRate(1, 1);
	mWhiteTransparency = true;
	mInvisiblePen.SetColour(255, 255, 255);
}

void BitmapView::OnPaint(wxPaintEvent& e)
{
	wxPaintDC   tempdc(this);
	int			vw, vh, cw, ch;

	GetVirtualSize(&vw, &vh);
	DoPrepareDC(tempdc);
	GetClientSize(&cw, &ch);
	tempdc.SetPen(mInvisiblePen);
	if (mEncBmp != NULL && mColorTable != NULL)
		tempdc.DrawBitmap(mDecBmp, vw/2 - mDecBmp.GetWidth()/2, vh/2 - mDecBmp.GetHeight()/2);
}

// handle mouse drag (pan view)
void BitmapView::OnDrag(wxMouseEvent &e)
{
	if (e.ButtonDown()) {
		// start panning
		int scroll_x, scroll_y;

		GetViewStart(&scroll_x, &scroll_y);
		mDragging = true;
		mDragStartX = e.GetPosition().x + scroll_x;
		mDragStartY = e.GetPosition().y + scroll_y;
	} else if (e.Dragging() && mDragging) {
		// pan
		int	dx = mDragStartX - e.GetPosition().x,
			dy = mDragStartY - e.GetPosition().y;

		Scroll(dx, dy);
	} else if (e.ButtonUp()) {
		// end panning
		mDragging = false;
	}
}

void BitmapView::OnSize(wxSizeEvent &e)
{
	int cw, ch,
		vw = (mEncBmp == NULL) ? 0 : mEncBmp->Width(),
		vh = (mEncBmp == NULL) ? 0 : mEncBmp->Height();
	
	GetClientSize(&cw, &ch);
	if (vw < cw)
		vw = cw;
	if (vh < ch)
		vh = ch;
	SetVirtualSize(vw, vh);
}

void BitmapView::SetTranspPixelsDisplay(bool show)
{
	mWhiteTransparency = show;
	if (mEncBmp != NULL && mEncBmp->Pixels() != NULL && mColorTable != NULL)
		mDecBmp = wxBitmap(ShapesBitmapToImage(mEncBmp, mColorTable, mWhiteTransparency));
	Refresh();
}

// add a new ShapesBitmap to the thumbnail list
void BitmapView::SetBitmap(ShapesBitmap *bp)
{
	mEncBmp = bp;
	if (mEncBmp != NULL) {
		if (mEncBmp->Pixels() != NULL) {
			// adjust sizes
			int	cw, ch,
				vw = mEncBmp->Width(),
				vh = mEncBmp->Height();

			GetClientSize(&cw, &ch);
			if (vw < cw)
				vw = cw;
			if (vh < ch)
				vh = ch;
			SetVirtualSize(vw, vh);
			// decode bitmap
			if (mColorTable != NULL)
				mDecBmp = wxBitmap(ShapesBitmapToImage(mEncBmp, mColorTable, mWhiteTransparency));
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
	return mEncBmp;
}

// call this before SettingBitmap!
void BitmapView::SetColorTable(ShapesColorTable *ct)
{
	mColorTable = ct;
	if (mEncBmp != NULL && mEncBmp->Pixels() != NULL && mColorTable != NULL)
		mDecBmp = wxBitmap(ShapesBitmapToImage(mEncBmp, mColorTable, mWhiteTransparency));
	Refresh();
}


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
// SequenceView
// An editor widget for sequence frames. It displays the frames of a
// sequence keeping them organized in views.
//

#ifndef SEQUENCEVIEW_H
#define SEQUENCEVIEW_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "ShapesElements.h"

DECLARE_EVENT_TYPE(wxEVT_SEQUENCEVIEW, -1)

class SequenceView: public wxScrolledWindow {
private:
	vector<ShapesFrame*>	mFrames;		// pointers to frames
	vector<ShapesBitmap*>	mBitmaps;		// pointers to bitmaps
	vector<wxBitmap>		mThumbnails;	// scaled/rendered frame thumbnails
	ShapesColorTable		*mColorTable;		// which palette to use for display
	vector<wxPoint>			mThumbnailPositions;	// thumbnail positions within window
	wxCoord					mThumbnailSize,
							mMargin,			// margin between thumbnails and window edges
							mAngleLabelSpace;
	wxBitmap				mPrevBtnIcon,
							mNextBtnIcon;
	bool					mAutoSize,
							mWhiteTransparency;
	int						mSelection;			// selected thumbnail
	wxPen					mThumbnailPen,
							mSelectionPen;
	wxBrush					mAngleBrush;

	int						mNumberOfViews,
							mFramesPerView,
							mAnimationType;
	vector<short>			*mFrameIndexes;

	wxBitmap CreateThumbnail(ShapesFrame *fp);
	void UpdateVirtualSize(void);

	void PopupFrameIndexDialog(int index);

protected:
	DECLARE_EVENT_TABLE();

public:
	SequenceView(wxWindow *parent, wxWindowID id);
	// event handlers
	void OnPaint(wxPaintEvent& e);
	void OnSize(wxSizeEvent& e);
	void OnMouseDown(wxMouseEvent& e);
	void OnMouseMove(wxMouseEvent &e);
	void OnMouseUp(wxMouseEvent& e);
	void OnKeyDown(wxKeyEvent& e);
	void OnMouseDoubleClick(wxMouseEvent& e);
	// access methods
	int GetSelection(void) const;
	void SetThumbnailSize(int size);
	void SetTranspPixelsDisplay(bool show);
	void AddFrame(ShapesFrame *fp);
	void AddBitmap(ShapesBitmap *bp);
	void SetColorTable(ShapesColorTable *ct);
	void SetSeqParameters(int animtype, int fpv, vector<short> *indexes);
	void Clear(void);
	// utilities
	void RebuildThumbnail(unsigned int i);
	void RebuildThumbnails(void);
};

#endif

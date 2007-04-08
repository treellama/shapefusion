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
#include "CTBrowser.h"

DEFINE_EVENT_TYPE(wxEVT_CTBROWSER)

BEGIN_EVENT_TABLE(CTBrowser, wxScrolledWindow)
	EVT_PAINT(CTBrowser::OnPaint)
	EVT_SIZE(CTBrowser::OnSize)
	EVT_LEFT_DOWN(CTBrowser::OnMouseDown)
	EVT_KEY_DOWN(CTBrowser::OnKeyDown)
END_EVENT_TABLE()

CTBrowser::CTBrowser(wxWindow *parent): wxScrolledWindow(parent, -1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER),
	sample_w(1), sample_h(20), selection(-1)
{
	SetBackgroundColour(wxColour(255, 255, 255));
	invisible_pen.SetColour(0, 0, 0);
	invisible_pen.SetStyle(wxTRANSPARENT);
	selection_pen.SetColour(0, 0, 0);
	selection_pen.SetWidth(3);
	SetScrollRate(2, 2);
	margin = 10;
}

void CTBrowser::OnPaint(wxPaintEvent& e)
{
	wxPaintDC	tempdc(this);
	int			visible_w, visible_h,
				rx, ry,
				x = margin, y = margin;

	DoPrepareDC(tempdc);
	GetClientSize(&visible_w, &visible_h);
	// fill with white
	CalcUnscrolledPosition(0, 0, &rx, &ry);
	tempdc.SetPen(invisible_pen);
	// draw color samples
	for (unsigned int i = 0; i < cts.size(); i++) {
		wxBrush	brush(wxColour(0, 0, 0), wxSOLID);

		// mark selected ct
		if ((int)i == selection) {
			tempdc.SetPen(selection_pen);
			tempdc.SetBrush(*wxWHITE_BRUSH);
			tempdc.DrawRectangle(x-3, y-3, sample_w*colors_per_table+6, sample_h+6);
			tempdc.SetPen(invisible_pen);
			tempdc.SetBrush(*wxTRANSPARENT_BRUSH);
		}
		// draw a square for each color
		for (unsigned int j = 0; j < colors_per_table; j++) {
			brush.SetColour(cts[i]->colors[j].red >> 8,
							cts[i]->colors[j].green >> 8,
							cts[i]->colors[j].blue >> 8);
			tempdc.SetBrush(brush);
			tempdc.DrawRectangle(x, y, sample_w, sample_h);
			if (cts[i]->colors[j].self_luminescent) {
				// display the self-luminescent color flag
				tempdc.SetBrush(*wxWHITE_BRUSH);
				tempdc.DrawRectangle(x + 2, y + 2, 2, 2);
			}
			x += sample_w;
		}
		x = margin;
		y += sample_h + margin;
	}
}

// widget resized, recalculate virtual size to correctly wrap thumbnails
void CTBrowser::OnSize(wxSizeEvent& e)
{
	UpdateVirtualSize();
}

void CTBrowser::OnMouseDown(wxMouseEvent& e)
{
	wxClientDC	dc(this);
	wxPoint		mouse;
	
	DoPrepareDC(dc);
	mouse = e.GetLogicalPosition(dc);
	switch (e.GetButton()) {
		case wxMOUSE_BTN_LEFT:	// ct selection
			{
				int	new_selection = -1;
				
				for (unsigned int i = 0; i < cts.size(); i++) {
					wxRect	test(margin, margin + i * (sample_h + margin), sample_w*colors_per_table, sample_h);

					if (test.Inside(mouse)) {
						new_selection = i;
						break;
					}
				}
				if (new_selection != selection) {
					selection = new_selection;
					Refresh();
					// send selection event
					wxCommandEvent	event(wxEVT_CTBROWSER, GetId());

					event.SetEventObject(this);
					event.SetInt(selection);
					GetEventHandler()->ProcessEvent(event);
				}
			}
			break;
	}
	e.Skip();
}

void CTBrowser::OnKeyDown(wxKeyEvent &e)
{
	int new_selection = selection;

	if (selection >= 0 && selection < (int)cts.size()) {
		// arrow keys move the selection
		switch (e.GetKeyCode()) {
			case WXK_UP:
				if (selection > 0)
					new_selection--;
				break;
			case WXK_DOWN:
				if (selection < ((int)cts.size()-1))
					new_selection++;
				break;
			default:
				e.Skip();
		}
	} else if (cts.size() > 0 && (e.GetKeyCode() == WXK_UP || e.GetKeyCode() == WXK_DOWN)) {
		new_selection = 0;
	}
	if (new_selection != selection) {
		selection = new_selection;
		Refresh();
		// send selection event
		wxCommandEvent  event(wxEVT_CTBROWSER, GetId());
		
		event.SetEventObject(this);
		event.SetInt(selection);
		GetEventHandler()->ProcessEvent(event);
	}
}

// add a new ShpColorTable to the list
void CTBrowser::AddColorTable(ShpColorTable *ctp)
{
	if (ctp == NULL)
		return;
	colors_per_table = ctp->colors.size();
	cts.push_back(ctp);
	UpdateVirtualSize();
	Refresh();
}

// clear the color table list
void CTBrowser::Clear(void)
{
	cts.clear();
	selection = -1;
	UpdateVirtualSize();
//	Refresh();	// not needed since the ShapesEditor only clears before refilling
}

int CTBrowser::GetSelection(void) const
{
	return selection;
}

// calculate and set the wxWindow virtual size,
// based on the number of color tables, colors per table,
// color sample dimensions and given visible size
void CTBrowser::UpdateVirtualSize(void)
{
	wxClientDC	dc(this);
	int			numcts = cts.size(),
				width, height;

	GetClientSize(&width, &height);
	// calculate best sample_w
	for (int w = 1; w < sample_h; w++) {
		if (2*margin + w*(int)colors_per_table > width) {
			sample_w = w - 1;
			break;
		}
	}
	if (sample_w < 1)
		sample_w = 1;
	// set virtual size
	SetVirtualSize(2*margin + sample_w*colors_per_table, margin + numcts * (sample_h + margin));
}


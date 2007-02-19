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

#include "ShapesView.h"
#include "ShapeFusionApp.h"

IMPLEMENT_DYNAMIC_CLASS(ShapesView, wxView)

bool ShapesView::OnCreate(wxDocument *doc, long WXUNUSED(flags) )
{
    frame = wxGetApp().CreateChildFrame(doc, this, false);
    
    int width, height;
    frame->GetClientSize(&width, &height);
//    textsw = new MyTextWindow(this, frame, wxPoint(0, 0), wxSize(width, height), wxTE_MULTILINE);
    frame->SetTitle(_T("TextEditView"));
    
#ifdef __X__
    // X seems to require a forced resize
    int x, y;
    frame->GetSize(&x, &y);
    frame->SetSize(wxDefaultCoord, wxDefaultCoord, x, y);
#endif
    
    frame->Show(true);
    Activate(true);
    
    return true;
}

// Handled by wxTextWindow
void ShapesView::OnDraw(wxDC *WXUNUSED(dc) )
{
}

void ShapesView::OnUpdate(wxView *WXUNUSED(sender), wxObject *WXUNUSED(hint) )
{
}

bool ShapesView::OnClose(bool deleteWindow)
{
    if (!GetDocument()->Close())
        return false;
    
    Activate(false);
    
    if (deleteWindow)
    {
        delete frame;
        return true;
    }
    return true;
}

// Define a constructor for my text subwindow
/*MyTextWindow::MyTextWindow(wxView *v, wxFrame *frame, const wxPoint& pos, const wxSize& size, const long style):
    wxTextCtrl(frame, wxID_ANY, _T(""), pos, size, style)
{
    view = v;
}*/


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

#include "ShapeFusionMain.h"
#include "ShapeFusionMenus.h"

/*
* This is the top-level window of the application.
*/

IMPLEMENT_CLASS(ShapeFusionMain, wxDocParentFrame)
BEGIN_EVENT_TABLE(ShapeFusionMain, wxDocParentFrame)
    EVT_MENU(ABOUT_MENU, ShapeFusionMain::OnAbout)
    EVT_MENU_RANGE(FILE_HISTORY_FILE1, FILE_HISTORY_FILE9, ShapeFusionMain::OnMRUFile)
END_EVENT_TABLE()

ShapeFusionMain::ShapeFusionMain(wxDocManager *manager, wxFrame *frame, wxWindowID id, const wxString& title,
                 const wxPoint& pos, const wxSize& size, const long type):
wxDocParentFrame(manager, frame, id, title, pos, size, type)
{
}

void ShapeFusionMain::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	 wxMessageDialog *dlg = new wxMessageDialog(this,
								wxT("ShapeFusion 0.4\nhttp://shapefusion.sourceforge.net\n\nCopyright 2000-2007, Tito Dal Canton\n\n"
									"An editor for Marathon II, Marathon Infinity and AlephOne shapes files. "
									"Released under the terms of the GNU General Public License: for more information "
									"see the COPYING file that comes with ShapeFusion."),
								wxT("About ShapeFusion"), wxOK | wxICON_INFORMATION);
	 dlg->ShowModal();
	 dlg->Destroy();
}

void ShapeFusionMain::OnMenuHistory(wxCommandEvent& event)
{
	wxString f(m_docManager->GetHistoryFile(event.GetId() - FILE_HISTORY_FILE1));
	if (!f.empty())
		(void)m_docManager->CreateDocument(f, wxDOC_SILENT);
}


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

#include "ShapeFusionApp.h"
#include "ShapeFusionMenus.h"
#include "ShapesDocument.h"
#include "ShapesView.h"
#include "SoundsDocument.h"
#include "SoundsView.h"

ShapeFusionMain *frame = (ShapeFusionMain *)NULL;

IMPLEMENT_APP(ShapeFusionApp)

ShapeFusionApp::ShapeFusionApp(void)
{
    m_docManager = (wxDocManager *)NULL;
}

bool ShapeFusionApp::OnInit(void)
{
	if (!wxApp::OnInit())
		return false;
	
	// so that we can import every sort of bitmap format
	wxInitAllImageHandlers();
		
	// Create a document manager
	m_docManager = new wxDocManager;

	// Create a template relating drawing documents to their views
	(void) new wxDocTemplate(m_docManager, _T("Shapes"), _T("*"), _T(""), _T(""), _T("Shapes"), _T("Shapes"),
		CLASSINFO(ShapesDocument), CLASSINFO(ShapesView));
	(void) new wxDocTemplate(m_docManager, _T("Sounds"), _T("*"), _T(""), _T(""), _T("Sounds"), _T("Sounds"),
		CLASSINFO(SoundsDocument), CLASSINFO(SoundsView));

#ifdef __WXMAC__
	//TODO: Put correct file extension values here
//    wxFileName::MacRegisterDefaultTypeAndCreator( wxT("*") , 'WXMB' , 'WXMA' );
//    wxFileName::MacRegisterDefaultTypeAndCreator( wxT("*") , 'WXMB' , 'WXMA' );
#endif
    
    // Create the main frame window
#ifdef __WXMAC__
	// a hack to make the frame invisible on MacOS, which is more Mac-like
	// http://www.wxwidgets.org/wiki/index.php/WxMac_Issues#The_Mac_OS_menu_bar
	frame = new ShapeFusionMain(m_docManager, (wxFrame *)NULL, wxID_ANY, _T("ShapeFusion Workspace"), wxPoint(5,5), wxSize(0,0), 0);
#else
    frame = new ShapeFusionMain(m_docManager, (wxFrame *)NULL, wxID_ANY, _T("ShapeFusion Workspace"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE);
#endif

    // Give it an icon (this is ignored in MDI mode: uses resources)
#ifdef __WXMSW__
    frame->SetIcon(wxIcon(_T("doc_icn")));
#endif

    wxMenuBar *menu_bar = new wxMenuBar;

	CreateFileMenu(menu_bar);
	CreateEditMenu(menu_bar);
	CreateHelpMenu(menu_bar);
    
	// Associate the menu bar with the frame
	frame->SetMenuBar(menu_bar);

	//FIXME: This doesn't work
	//wxMenuItem *menuItem = menu_bar->FindItem(FILE_MENU_HISTORY);
	//m_docManager->FileHistoryUseMenu(menuItem->GetMenu());
    
    frame->Centre(wxBOTH);
    frame->Show(true);
    
    SetTopWindow(frame);
    return true;
}

int ShapeFusionApp::OnExit(void)
{
    delete m_docManager;
    return 0;
}

/*
* Centralised code for creating a document frame.
* Called when a new view is created (after a New/Open event)
*/
wxFrame *ShapeFusionApp::CreateChildFrame(wxDocument *doc, wxView *view, const wxString title, wxPoint point, wxSize size, long style)
{
    // Make a child frame
    wxDocChildFrame *subframe = new wxDocChildFrame(doc, view, GetMainFrame(), wxID_ANY, title,
        point, size, style);
    
    wxMenuBar *menu_bar = new wxMenuBar;
    
	CreateFileMenu(menu_bar);
    CreateEditMenu(menu_bar);
	CreateHelpMenu(menu_bar);
	
    // Associate the menu bar with the frame
    subframe->SetMenuBar(menu_bar);
	
	//FIXME: This doesn't work
	//wxMenuItem *menuItem = menu_bar->FindItem(FILE_MENU_HISTORY);
	//m_docManager->FileHistoryUseMenu(menuItem->GetMenu());
	
    subframe->Centre(wxBOTH);
    
    return subframe;
}

ShapeFusionMain *GetMainFrame(void)
{
    return frame;
}

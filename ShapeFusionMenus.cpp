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

#include "ShapeFusionMenus.h"

void CreateFileMenu(wxMenuBar *menu_bar)
{
	wxMenu	*file_menu,
			*recent_file_menu;
/*			*file_new_menu,
			*file_open_menu;*/
	
	// We create our two submenus
/*	file_new_menu = new wxMenu();
	file_new_menu->Append(FILE_MENU_NEW_SHAPES, wxT("Shapes file"));
	file_new_menu->Append(FILE_MENU_NEW_PHYSICS, wxT("Physics file"));
	file_new_menu->Append(FILE_MENU_NEW_MAP, wxT("Map file"));
	file_new_menu->Append(FILE_MENU_NEW_SOUNDS, wxT("Sounds file"));
	file_open_menu = new wxMenu();
	file_open_menu->Append(FILE_MENU_OPEN_SHAPES, wxT("Shapes file"));
	file_open_menu->Append(FILE_MENU_OPEN_PHYSICS, wxT("Physics file"));
	file_open_menu->Append(FILE_MENU_OPEN_MAP, wxT("Map file"));
	file_open_menu->Append(FILE_MENU_OPEN_SOUNDS, wxT("Sounds file"));*/
	// We create the recent menu here...
	recent_file_menu = new wxMenu();
	
	// We create the 'main' File menu
	file_menu = new wxMenu();
//	file_menu->Append(FILE_MENU_NEW, wxT("New..."), file_new_menu);
//	file_menu->Append(FILE_MENU_OPEN, wxT("Open..."), file_open_menu);
	file_menu->Append(FILE_MENU_NEW, wxT("New...\tCtrl+N"));
	file_menu->Append(FILE_MENU_OPEN, wxT("Open...\tCtrl+O"));
	file_menu->Append(FILE_MENU_HISTORY, wxT("Open Recent..."), recent_file_menu);
	file_menu->Append(FILE_MENU_CLOSE, wxT("&Close\tCtrl+W"));
	file_menu->AppendSeparator();
	file_menu->Append(FILE_MENU_SAVE, wxT("&Save\tCtrl+S"));
	file_menu->Append(FILE_MENU_SAVEAS, wxT("Save as...\tCtrl+Shift+S"));
	file_menu->Append(FILE_MENU_REVERT, wxT("&Revert"));
	file_menu->AppendSeparator();
	file_menu->Append(FILE_MENU_PRINT, _T("&Print..."));
	file_menu->Append(FILE_MENU_PRINT_SETUP, _T("Print &Setup..."));
	file_menu->Append(FILE_MENU_PREVIEW, _T("Print Pre&view"));
	file_menu->AppendSeparator();
	file_menu->Append(FILE_MENU_QUIT, wxT("&Quit\tAlt+F4"));
	
	file_menu->Enable(FILE_MENU_SAVE, false);
	file_menu->Enable(FILE_MENU_SAVEAS, false);
	file_menu->Enable(FILE_MENU_CLOSE, false);
	
	// We add the File menu to the passed menubar
	menu_bar->Append(file_menu, wxT("&File"));
}

void CreateEditMenu(wxMenuBar *menu_bar)
{
	wxMenu	*edit_menu;
	
	edit_menu = new wxMenu();
	edit_menu->Append(EDIT_MENU_UNDO, wxT("&Undo\tCtrl+Z"));
	edit_menu->Append(EDIT_MENU_REDO, wxT("&Redo\tCtrl+Shift+Z"));
    edit_menu->AppendSeparator();
	edit_menu->Append(EDIT_MENU_CUT, wxT("&Cut\tCtrl+X"));
	edit_menu->Append(EDIT_MENU_COPY, wxT("&Copy\tCtrl+C"));
	edit_menu->Append(EDIT_MENU_PASTE, wxT("&Paste\tCtrl+V"));
    edit_menu->AppendSeparator();
	edit_menu->Append(EDIT_MENU_DELETE, wxT("&Delete"));
	
	edit_menu->Enable(EDIT_MENU_CUT, false);
	edit_menu->Enable(EDIT_MENU_COPY, false);
	edit_menu->Enable(EDIT_MENU_PASTE, false);
	edit_menu->Enable(EDIT_MENU_DELETE, false);
	
	// We add the Edit menu to the passed menubar
	menu_bar->Append(edit_menu, wxT("&Edit"));
}

void CreateHelpMenu(wxMenuBar *menu_bar)
{
	wxMenu *help_menu;
	
	help_menu = new wxMenu();
	help_menu->Append(ABOUT_MENU, wxT("About ShapeFusion"));
	help_menu->Append(HELP_MENU, wxT("ShapeFusion Help"));

	menu_bar->Append(help_menu, wxT("&Help"));
}

void CreateViewMenu(wxMenuBar *menu_bar)
{
	// view menu
	wxMenu	*view_menu,
			*view_colortable_submenu,
			*view_tnsize_submenu;
	
	// We create our two submenus
	view_colortable_submenu = new wxMenu();
	for (unsigned int i = 0; i < 8; i++) {
		view_colortable_submenu->AppendRadioItem(VIEW_MENU_COLORTABLE_0 + i, wxString::Format(wxT("%d"), i));
		view_colortable_submenu->Enable(VIEW_MENU_COLORTABLE_0 + i, false);
	}
	
	view_tnsize_submenu = new wxMenu();
	view_tnsize_submenu->AppendRadioItem(VIEW_MENU_TNSIZE_SMALL, wxT("Small"));
	view_tnsize_submenu->AppendRadioItem(VIEW_MENU_TNSIZE_MEDIUM, wxT("Medium"));
	view_tnsize_submenu->AppendRadioItem(VIEW_MENU_TNSIZE_LARGE, wxT("Large"));
	view_tnsize_submenu->AppendRadioItem(VIEW_MENU_TNSIZE_AUTO, wxT("Best fit"));
	for (unsigned int i = VIEW_MENU_TNSIZE_SMALL; i <= VIEW_MENU_TNSIZE_AUTO; i++)
		view_tnsize_submenu->Check(i, i == VIEW_MENU_TNSIZE_MEDIUM);
	
	// Now the main View menu
	view_menu = new wxMenu();
	view_menu->Append(VIEW_MENU_COLOR_TABLE, wxT("Use color table"), view_colortable_submenu);
	view_menu->Append(VIEW_MENU_THUMBNAIL_SIZE, wxT("Thumbnail size"), view_tnsize_submenu);
	view_menu->AppendCheckItem(VIEW_MENU_TRANSPARENCY, wxT("Show transparent pixels"));

	// We add that to the passed menubar
	menu_bar->Append(view_menu, wxT("&View"));
}

void CreateShapesMenu(wxMenuBar *menu_bar)
{
	wxMenu *shapes_menu;
	
	// We create and build the Shapes menu
	shapes_menu = new wxMenu();
	shapes_menu->Append(SHAPES_MENU_ADDCOLORTABLE, wxT("New color table..."));
	shapes_menu->Append(SHAPES_MENU_SAVECOLORTABLE, wxT("Export color table to GIMP..."));
	shapes_menu->Append(SHAPES_MENU_SAVECOLORTABLETOPS, wxT("Export color table to PhotoShop..."));
	shapes_menu->AppendSeparator();
	shapes_menu->Append(SHAPES_MENU_ADDBITMAP, wxT("New bitmap..."));
	shapes_menu->Append(SHAPES_MENU_EXPORTBITMAP, wxT("Export bitmap..."));
	shapes_menu->Append(SHAPES_MENU_EXPORTMASK, wxT("Export bitmap mask..."));
	shapes_menu->Append(SHAPES_MENU_EXPORTBITMAPS, wxT("Export all bitmaps..."));
	shapes_menu->Append(SHAPES_MENU_EXPORTMASKS, wxT("Export all bitmap masks..."));
	shapes_menu->AppendSeparator();
	shapes_menu->Append(SHAPES_MENU_ADDFRAME, wxT("New frame"));
	shapes_menu->AppendSeparator();
	shapes_menu->Append(SHAPES_MENU_ADDSEQUENCE, wxT("New sequence"));
	
	// Let's disable all these items...
	shapes_menu->Enable(SHAPES_MENU_ADDCOLORTABLE, false);
	shapes_menu->Enable(SHAPES_MENU_SAVECOLORTABLE, false);
	shapes_menu->Enable(SHAPES_MENU_SAVECOLORTABLETOPS, false);
	shapes_menu->Enable(SHAPES_MENU_ADDBITMAP, false);
	shapes_menu->Enable(SHAPES_MENU_EXPORTBITMAP, false);
	shapes_menu->Enable(SHAPES_MENU_EXPORTMASK, false);
	shapes_menu->Enable(SHAPES_MENU_EXPORTBITMAPS, false);
	shapes_menu->Enable(SHAPES_MENU_EXPORTMASKS, false);
	shapes_menu->Enable(SHAPES_MENU_ADDFRAME, false);
	shapes_menu->Enable(SHAPES_MENU_ADDSEQUENCE, false);
	
	// .. and add the whole to the menuBar
	menu_bar->Append(shapes_menu, wxT("&Shapes"));
}

void CreateSoundsMenu(wxMenuBar *menu_bar)
{
	wxMenu	*sounds_menu;
	
	// We create and build the Sounds menu
	sounds_menu = new wxMenu();
	sounds_menu->Append(SOUNDS_MENU_ADDCLASS, wxT("Add new sound class"));
	sounds_menu->AppendSeparator();
	sounds_menu->Append(SOUNDS_MENU_EXPORT, wxT("Export selected sound"));
	sounds_menu->Append(SOUNDS_MENU_IMPORT, wxT("Import sound"));
	
	// Let's disable them ...
	sounds_menu->Enable(SOUNDS_MENU_ADDCLASS, false);
	sounds_menu->Enable(SOUNDS_MENU_EXPORT, false);
	sounds_menu->Enable(SOUNDS_MENU_IMPORT, false);
	
	// ... and add the whole to the menu_bar
	menu_bar->Append(sounds_menu, wxT("S&ounds"));
}


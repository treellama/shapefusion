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

#ifndef SHAPEFUSIONMENUS_H
#define SHAPEFUSIONMENUS_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

enum {
	// Application Menus (we just use wx Indexes here)
	FILE_MENU_NEW = wxID_NEW,
	FILE_MENU_OPEN = wxID_OPEN,
	FILE_MENU_CLOSE = wxID_CLOSE,
	FILE_MENU_QUIT = wxID_EXIT,
	FILE_MENU_SAVE = wxID_SAVE,
	FILE_MENU_SAVEAS = wxID_SAVEAS,
	FILE_MENU_REVERT = wxID_REVERT,
	FILE_MENU_PRINT = wxID_PRINT,
	FILE_MENU_PRINT_SETUP = wxID_PRINT_SETUP,
	FILE_MENU_PREVIEW = wxID_PREVIEW,
	ABOUT_MENU = wxID_ABOUT,
	HELP_MENU = wxID_HELP,
	FILE_HISTORY_FILE1 = wxID_FILE1,
	FILE_HISTORY_FILE9 = wxID_FILE9,
	// Edit Menu
	EDIT_MENU_UNDO = wxID_UNDO,
	EDIT_MENU_REDO = wxID_REDO,
	EDIT_MENU_CUT = wxID_CUT,
	EDIT_MENU_COPY = wxID_COPY,
	EDIT_MENU_PASTE = wxID_PASTE,
	EDIT_MENU_DELETE = wxID_DELETE,
	// We put our menus that don't come from wx here,
	// so they'll have menu ID starting from 1
	FILE_MENU_NEW_SHAPES = 1,
	FILE_MENU_NEW_PHYSICS,
	FILE_MENU_NEW_MAP,
	FILE_MENU_NEW_SOUNDS,
	FILE_MENU_OPEN_SHAPES,
	FILE_MENU_OPEN_PHYSICS,
	FILE_MENU_OPEN_MAP,
	FILE_MENU_OPEN_SOUNDS,
	FILE_MENU_HISTORY,
	// Shapes editing menus
	// View Menu
	VIEW_MENU_TRANSPARENCY,
	// Thumbnails submenu
	VIEW_MENU_THUMBNAIL_SIZE,
	VIEW_MENU_TNSIZE_SMALL,
	VIEW_MENU_TNSIZE_MEDIUM,
	VIEW_MENU_TNSIZE_LARGE,
	VIEW_MENU_TNSIZE_AUTO,
	// Color Tables submenu
	VIEW_MENU_COLOR_TABLE,
	VIEW_MENU_COLORTABLE_0,
	VIEW_MENU_COLORTABLE_7 = VIEW_MENU_COLORTABLE_0 + 7,
	// Shapes menu
	SHAPES_MENU_ADDCOLORTABLE,
	SHAPES_MENU_SAVECOLORTABLE,
	SHAPES_MENU_SAVECOLORTABLETOPS,
	SHAPES_MENU_ADDBITMAP,
	SHAPES_MENU_EXPORTBITMAP,
	SHAPES_MENU_EXPORTMASK,
	SHAPES_MENU_EXPORTBITMAPS,
	SHAPES_MENU_EXPORTMASKS,
	SHAPES_MENU_ADDFRAME,
	SHAPES_MENU_ADDSEQUENCE,
	// Sounds menu
	SOUNDS_MENU_ADDCLASS,
	SOUNDS_MENU_EXPORTSOUND,
	SOUNDS_MENU_IMPORTSOUND
};

void CreateFileMenu(wxMenuBar *menu_bar);
void CreateEditMenu(wxMenuBar *menu_bar);
void CreateHelpMenu(wxMenuBar *menu_bar);
void CreateViewMenu(wxMenuBar *menu_bar);
void CreateShapesMenu(wxMenuBar *menu_bar);
void CreateSoundsMenu(wxMenuBar *menu_bar);
#endif


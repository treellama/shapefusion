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

// FIXME
// - There seems to be a strange bug in the wx sizer algorithm. This bug
//   causes SOME of the wxStaticText widgets to wrap badly, where they
//   should actually stay on a single line (they should expand and forge
//   the sizer at their need). They look really like they was given a
//   fixed size instead. This effect seems to happen only on Linux/GTK,
//   on MacOS X everything is perfect. This could be due to the more
//   up-to-date version of wx I'm using under OS X. As a workaround, I'm
//   currently giving fixed sizes to those static labels, specifying an
//   excess width so they certainly won't wrap. This is bad.
// - For text controls EVT_TEXT is great (it's actually the only way to
//   get a "normal" behaviour), but it's fired also when calling SetValue
//   with a different string than what is currently present. So when we
//   fill the fields for a panel, a burst of EVT_TEXT will come in,
//   overwriting the associated structure with the data already stored
//   in it. This should not cause trouble (as long as controls are filled
//   in the correct order), but it's bad.
// - The minimum light intensity field in the frame editor should better
//   be a slider or a spin field, as it's a percentage value.
#include <iostream>
#include <string>
#include "wx/image.h"
#include "wx/progdlg.h"
#include "ShapesEditor.h"
#include "MyTreeItemData.h"
#include "utilities.h"

#define INT_TO_WXSTRING(a)	wxString::Format(wxT("%d"), a)

BEGIN_EVENT_TABLE(ShapesEditor, wxFrame)
	EVT_MENU(wxID_OPEN, ShapesEditor::MenuFileOpen)
	EVT_MENU(wxID_SAVE, ShapesEditor::MenuFileSave)
	EVT_MENU(wxID_SAVEAS, ShapesEditor::MenuFileSave)
	EVT_MENU(wxID_EXIT, ShapesEditor::MenuFileQuit)
	EVT_MENU(wxID_DELETE, ShapesEditor::MenuEditDelete)
	EVT_MENU_RANGE(MVIEW_COLORTABLE_0, MVIEW_COLORTABLE_7, ShapesEditor::MenuViewCT)
	EVT_MENU_RANGE(MVIEW_TNSIZE_SMALL, MVIEW_TNSIZE_AUTO, ShapesEditor::MenuViewTNSize)
	EVT_MENU(MVIEW_TRANSPARENCY, ShapesEditor::MenuViewTransparency)
	EVT_MENU(MSHAPES_ADDCOLORTABLE, ShapesEditor::MenuShapesAddColorTable)
	EVT_MENU(MSHAPES_SAVECOLORTABLE, ShapesEditor::MenuShapesSaveColorTable)
	EVT_MENU(MSHAPES_SAVECOLORTABLETOPS, ShapesEditor::MenuShapesSaveColorTable)
	EVT_MENU(MSHAPES_ADDBITMAP, ShapesEditor::MenuShapesAddBitmap)
	EVT_MENU(MSHAPES_EXPORTBITMAPS, ShapesEditor::MenuShapesExportBitmaps)
	EVT_MENU(MSHAPES_ADDFRAME, ShapesEditor::MenuShapesNewFrame)
	EVT_MENU(MSHAPES_ADDSEQUENCE, ShapesEditor::MenuShapesNewSequence)
	EVT_MENU(wxID_ABOUT, ShapesEditor::MenuHelpAbout)
	EVT_TREE_SEL_CHANGED(-1, ShapesEditor::TreeSelect)
	// bitmaps
	EVT_COMMAND(BITMAP_BROWSER, wxEVT_BITMAPBROWSER, ShapesEditor::BitmapSelect)
	EVT_COMMAND(BITMAP_BROWSER, wxEVT_BITMAPBROWSER_DELETE, ShapesEditor::BitmapDelete)
	EVT_COMMAND_RANGE(CB_COLUMN_ORDER, CB_ENABLE_TRANSPARENCY, wxEVT_COMMAND_CHECKBOX_CLICKED, ShapesEditor::ToggleBitmapCheckboxes)
	EVT_BUTTON(BTN_SAVE_BITMAP, ShapesEditor::AskSaveBitmap)
	// color tables
	EVT_COMMAND(wxID_ANY, wxEVT_CTBROWSER, ShapesEditor::CTSelect)
	// frames
	EVT_COMMAND(FRAME_BROWSER, wxEVT_FRAMEBROWSER, ShapesEditor::FrameSelect)
	EVT_SPINCTRL(FIELD_BITMAP_INDEX, ShapesEditor::BitmapIndexSpin)
	EVT_COMMAND_RANGE(CB_XMIRROR, CB_KEYPOINT, wxEVT_COMMAND_CHECKBOX_CLICKED, ShapesEditor::ToggleFrameCheckboxes)
	EVT_TEXT(FIELD_ORIGIN_X, ShapesEditor::EditFrameFields)
	EVT_TEXT(FIELD_ORIGIN_Y, ShapesEditor::EditFrameFields)
	EVT_TEXT(FIELD_KEY_X, ShapesEditor::EditFrameFields)
	EVT_TEXT(FIELD_KEY_Y, ShapesEditor::EditFrameFields)
	EVT_TEXT(FIELD_FRAME_SCALEFACTOR, ShapesEditor::EditFrameFields)
	EVT_TEXT(FIELD_MIN_LIGHT_INT, ShapesEditor::EditFrameFields)
	// sequences
	EVT_TEXT(FIELD_SEQ_NAME, ShapesEditor::EditSequenceFields)
	EVT_BUTTON(BTN_DELETE_SEQ, ShapesEditor::DeleteSequence)
	EVT_CHOICE(MENU_SEQ_TYPE, ShapesEditor::EditSequenceType)
	EVT_TEXT(FIELD_SEQ_FRAMES_PER_VIEW, ShapesEditor::EditSequenceFields)
	EVT_TEXT(FIELD_SEQ_TICKS_PER_FRAME, ShapesEditor::EditSequenceFields)
	EVT_TEXT(FIELD_SEQ_LOOP_FRAME, ShapesEditor::EditSequenceFields)
	EVT_TEXT(FIELD_SEQ_KEY_FRAME, ShapesEditor::EditSequenceFields)
	EVT_CHOICE(MENU_SEQ_XFER_MODE, ShapesEditor::EditSequenceXferMode)
	EVT_TEXT(FIELD_SEQ_XFER_MODE_PERIOD, ShapesEditor::EditSequenceFields)
	EVT_TEXT(FIELD_SEQ_FIRST_FRAME_SND, ShapesEditor::EditSequenceFields)
	EVT_TEXT(FIELD_SEQ_KEY_FRAME_SND, ShapesEditor::EditSequenceFields)
	EVT_TEXT(FIELD_SEQ_LAST_FRAME_SND, ShapesEditor::EditSequenceFields)
	EVT_TEXT(FIELD_SEQ_SCALE_FACTOR, ShapesEditor::EditSequenceFields)
END_EVENT_TABLE()

char	*collnames[] = {	"Interface graphics",
							"Weapons in hand",
							"Juggernaut",
							"Tick",
							"Projectiles & explosions",
							"Hunter",
							"Player",
							"Items",
							"Trooper",
							"Pfhor fighter",
							"Defender",
							"Yeti",
							"Bob",
							"Vacuum Bob",
							"Enforcer",
							"Hummer",
							"Compiler",
							"Walls 1 (water)",
							"Walls 2 (lava)",
							"Walls 3 (sewage)",
							"Walls 4 (jiaro)",
							"Walls 5 (pfhor)",
							"Scenery 1 (water)",
							"Scenery 2 (lava)",
							"Scenery 3 (sewage)",
							"Scenery 4 (jiaro)",
							"Scenery 5 (pfhor)",
							"Landscape 1",
							"Landscape 2",
							"Landscape 3",
							"Landscape 4",
							"Cyborg" };

ShapesEditor::ShapesEditor(const wxChar *t, int x, int y, int w, int h):
		wxFrame(0, wxID_ANY, t, wxPoint(x, y), wxSize(w, h), wxDEFAULT_FRAME_STYLE, wxT("shapefusion")),
		seqnameconv(wxT("macintosh")),
		payload(NULL), selected_coll(-1), selected_vers(-1), selected_sequence(-1), view_ct(-1), show_transparent_pixels(false)
{
	SetSizeHints(200, 200);

	mainbox = new wxBoxSizer(wxHORIZONTAL);
	// create the collection tree
	colltree = new wxTreeCtrl(this, -1, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT);
	wxTreeItemId	treeroot = colltree->AddRoot(wxT("No file loaded"));
	mainbox->Add(colltree, 2, wxEXPAND);
	// empty space (e.g. what is displayed when selecting the Sequences node)
	dummy_sizer = new wxBoxSizer(wxVERTICAL);
	mainbox->Add(dummy_sizer, 5, wxEXPAND);
	// collection section
	coll_sizer = new wxBoxSizer(wxVERTICAL);
	coll_text = new wxStaticText(this, -1, wxT("Collection info"));
	coll_static_box = new wxStaticBox(this, -1, wxT("Collection info"));
	coll_inner_box = new wxStaticBoxSizer(coll_static_box, wxVERTICAL);
	coll_inner_box->Add(coll_text, 0, wxALL, 5);
	coll_sizer->AddStretchSpacer();
	coll_sizer->Add(coll_inner_box, 0, wxALIGN_CENTER);
	coll_sizer->AddStretchSpacer();
	mainbox->Add(coll_sizer, 5, wxEXPAND);
	mainbox->Show(coll_sizer, false);
	// chunk section
	chunk_sizer = new wxBoxSizer(wxVERTICAL);
	chunk_static_box = new wxStaticBox(this, -1, wxT("Version info"));
	chunk_inner_box = new wxStaticBoxSizer(chunk_static_box, wxVERTICAL);
	chunk_undef_label = new wxStaticText(this, -1, wxT("Not defined"));
	chunk_inner_box->Add(chunk_undef_label, 0, wxCENTER | wxALL, 5);
	chunk_grid = new wxFlexGridSizer(2, 4);
	chunk_inner_box->Add(chunk_grid, 0, wxCENTER | wxALL, 5);
	chunk_version_label = new wxStaticText(this, -1, wxT("Version:"));
	chunk_type_label = new wxStaticText(this, -1, wxT("Collection type:"));
	chunk_flags_label = new wxStaticText(this, -1, wxT("Flags:"));
	chunk_sf_label = new wxStaticText(this, -1, wxT("Collection scale factor:"));
	chunk_version_field = new wxTextCtrl(this, -1, wxT("0"));
	wxString	coll_type_labels[] = {	wxT("Unused"),
										wxT("Wall textures"),
										wxT("Objects"),
										wxT("Interface graphics"),
										wxT("Scenery objects") };
	chunk_type_menu = new wxChoice(this, -1, wxDefaultPosition, wxDefaultSize, 5, coll_type_labels, 0);
	chunk_flags_field = new wxTextCtrl(this, -1, wxT("0"));
	chunk_sf_field = new wxTextCtrl(this, -1, wxT("0"));
	chunk_grid->Add(chunk_version_label, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
	chunk_grid->Add(chunk_version_field, 0, wxALIGN_LEFT);
	chunk_grid->Add(chunk_type_label, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
	chunk_grid->Add(chunk_type_menu, 0, wxALIGN_LEFT);
	chunk_grid->Add(chunk_flags_label, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
	chunk_grid->Add(chunk_flags_field, 0, wxALIGN_LEFT);
	chunk_grid->Add(chunk_sf_label, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
	chunk_grid->Add(chunk_sf_field, 0, wxALIGN_LEFT);
	chunk_version_field->Enable(false);
	chunk_type_menu->Enable(false);
	chunk_flags_field->Enable(false);
	chunk_sf_field->Enable(false);
	chunk_sizer->AddStretchSpacer();
	chunk_sizer->Add(chunk_inner_box, 0, wxALIGN_CENTER);
	chunk_sizer->AddStretchSpacer();
	mainbox->Add(chunk_sizer, 5, wxEXPAND);
	mainbox->Show(chunk_sizer, false);
	// create the color tables section
	ct_outer_sizer = new wxBoxSizer(wxVERTICAL);
	ctb = new CTBrowser(this);
	ct_count_label = new wxStaticText(this, -1, wxT("N color tables"));
	ct_outer_sizer->Add(ctb, 1, wxGROW);
	ct_outer_sizer->Add(ct_count_label, 0, wxALIGN_LEFT | wxLEFT | wxTOP | wxBOTTOM, 10);
	mainbox->Add(ct_outer_sizer, 5, wxEXPAND);
	mainbox->Show(ct_outer_sizer, false);
	// create the bitmaps section
	b_outer_sizer = new wxBoxSizer(wxVERTICAL);
	bb = new BitmapBrowser(this, BITMAP_BROWSER);
	bb->SetThumbnailSize(64);
	b_count_label = new wxStaticText(this, -1, wxT("N bitmaps"));
	b_edit_static_box = new wxStaticBox(this, -1, wxT("Bitmap N of M"));
	b_edit_box = new wxStaticBoxSizer(b_edit_static_box, wxHORIZONTAL);
	b_edit_inner_box = new wxBoxSizer(wxVERTICAL);
	b_info_label = new wxStaticText(this, -1, wxT("AxB pixels"));
	b_order_checkbox = new wxCheckBox(this, CB_COLUMN_ORDER, wxT("Store pixels in column order"));
	b_transparency_checkbox = new wxCheckBox(this, CB_ENABLE_TRANSPARENCY, wxT("Enable transparency"));
	b_view = new BitmapView(this);
	b_save = new wxButton(this, BTN_SAVE_BITMAP, wxT("Save bitmap as..."));
	b_replace = new wxButton(this, BTN_REPLACE_BITMAP, wxT("Replace bitmap..."));
	b_edit_inner_box->Add(b_info_label, 0, wxALIGN_LEFT | wxBOTTOM, 10);
	b_edit_inner_box->Add(b_order_checkbox, 0, wxALIGN_LEFT);
	b_edit_inner_box->Add(b_transparency_checkbox, 0, wxALIGN_LEFT);
	b_edit_inner_box->AddStretchSpacer();
	b_edit_inner_box->Add(b_save, 0, wxEXPAND | wxTOP, 10);
	b_edit_inner_box->Add(b_replace, 0, wxEXPAND | wxTOP, 10);
	b_edit_box->Add(b_edit_inner_box, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT | wxBOTTOM, 5);
	b_edit_box->Add(b_view, 1, wxEXPAND | wxLEFT | wxTOP | wxRIGHT | wxBOTTOM, 5);
	b_outer_sizer->Add(bb, 1, wxGROW);
	b_outer_sizer->Add(b_count_label, 0, wxALIGN_LEFT | wxLEFT | wxTOP | wxBOTTOM, 10);
	b_outer_sizer->Add(b_edit_box, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);
	b_outer_sizer->Show(b_edit_box, false);
	mainbox->Add(b_outer_sizer, 5, wxEXPAND);
	mainbox->Show(b_outer_sizer, false);
	// create the frames section
	f_outer_sizer = new wxBoxSizer(wxVERTICAL);
	fb = new FrameBrowser(this, FRAME_BROWSER);
	fb->SetThumbnailSize(64);
	f_count_label = new wxStaticText(this, -1, wxT("N frames"));
	f_edit_static_box = new wxStaticBox(this, -1, wxT("Frame N"));
	f_edit_box = new wxStaticBoxSizer(f_edit_static_box, wxHORIZONTAL);
	f_bitmap_label = new wxStaticText(this, -1, wxT("Associated bitmap:"));
	f_bitmap_id = new wxSpinCtrl(this, FIELD_BITMAP_INDEX, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_xmirror_checkbox = new wxCheckBox(this, CB_XMIRROR, wxT("X mirror"));
//	f_xmirror_checkbox->SetToolTip(wxT("Flip the bitmap horizontally"));
	f_ymirror_checkbox = new wxCheckBox(this, CB_YMIRROR, wxT("Y mirror"));
	f_keypoint_checkbox = new wxCheckBox(this, CB_KEYPOINT, wxT("Keypoint obscured"));
	f_origin_x_label = new wxStaticText(this, -1, wxT("Origin X:"), wxDefaultPosition, wxSize(150, -1));
	f_origin_x_field = new wxTextCtrl(this, FIELD_ORIGIN_X, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_origin_y_label = new wxStaticText(this, -1, wxT("Origin Y:"), wxDefaultPosition, wxSize(150, -1));
	f_origin_y_field = new wxTextCtrl(this, FIELD_ORIGIN_Y, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_key_x_label = new wxStaticText(this, -1, wxT("Keypoint X:"), wxDefaultPosition, wxSize(150, -1));
	f_key_x_field = new wxTextCtrl(this, FIELD_KEY_X, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_key_y_label = new wxStaticText(this, -1, wxT("Keypoint Y:"), wxDefaultPosition, wxSize(150, -1));
	f_key_y_field = new wxTextCtrl(this, FIELD_KEY_Y, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_scalefactor_label = new wxStaticText(this, -1, wxT("Scale factor:"), wxDefaultPosition, wxSize(150, -1));
	f_scalefactor_field = new wxTextCtrl(this, FIELD_FRAME_SCALEFACTOR, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_mli_label = new wxStaticText(this, -1, wxT("Minimum lightness (%):"), wxDefaultPosition, wxSize(150, -1));
	f_mli_field = new wxTextCtrl(this, FIELD_MIN_LIGHT_INT, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_origin_box = new wxFlexGridSizer(10, 2, 5, 0);
	f_origin_box->Add(f_bitmap_label, 1, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	f_origin_box->Add(f_bitmap_id, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	f_origin_box->Add(f_xmirror_checkbox, 1, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	f_origin_box->AddStretchSpacer();
	f_origin_box->Add(f_ymirror_checkbox, 1, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	f_origin_box->AddStretchSpacer();
	f_origin_box->Add(f_scalefactor_label, 1, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	f_origin_box->Add(f_scalefactor_field, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	f_origin_box->Add(f_origin_x_label, 1, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	f_origin_box->Add(f_origin_x_field, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	f_origin_box->Add(f_origin_y_label, 1, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	f_origin_box->Add(f_origin_y_field, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	f_origin_box->Add(f_key_x_label, 1, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	f_origin_box->Add(f_key_x_field, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	f_origin_box->Add(f_key_y_label, 1, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	f_origin_box->Add(f_key_y_field, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	f_origin_box->Add(f_keypoint_checkbox, 1, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	f_origin_box->AddStretchSpacer();
	f_origin_box->Add(f_mli_label, 1, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	f_origin_box->Add(f_mli_field, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	f_view = new FrameView(this);
	f_edit_inner_box = new wxBoxSizer(wxVERTICAL);
	f_edit_inner_box->Add(f_origin_box, 0, wxALIGN_LEFT);
	f_edit_box->Add(f_edit_inner_box, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 5);
	f_edit_box->Add(f_view, 1, wxEXPAND | wxLEFT | wxTOP | wxRIGHT | wxBOTTOM, 5);
	f_outer_sizer->Add(fb, 1, wxGROW);
	f_outer_sizer->Add(f_count_label, 0, wxALIGN_LEFT | wxLEFT | wxTOP | wxBOTTOM, 10);
	f_outer_sizer->Add(f_edit_box, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);
	f_outer_sizer->Show(f_edit_box, false);
	mainbox->Add(f_outer_sizer, 5, wxEXPAND);
	mainbox->Show(f_outer_sizer, false);
	// create the sequences section
	s_outer_static_box = new wxStaticBox(this, -1, wxT("Sequence"));
	s_outer_sizer = new wxStaticBoxSizer(s_outer_static_box, wxVERTICAL);
	mainbox->Add(s_outer_sizer, 5, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);

	s_box1 = new wxBoxSizer(wxHORIZONTAL);
	s_outer_sizer->Add(s_box1, 0, wxEXPAND | wxALIGN_TOP | wxALL, 5);
	s_name_label = new wxStaticText(this, -1, wxT("Name:"));
	s_name_field = new wxTextCtrl(this, FIELD_SEQ_NAME, wxT("foobar"));
	s_delete_button = new wxButton(this, BTN_DELETE_SEQ, wxT("Delete sequence"));
	s_box1->Add(s_name_label, 0, wxALIGN_CENTER | wxRIGHT, 5);
	s_box1->Add(s_name_field, 1, wxALIGN_CENTER);
	s_box1->Add(s_delete_button, 0, wxALIGN_CENTER | wxLEFT, 20);

	s_box2 = new wxBoxSizer(wxHORIZONTAL);
	s_outer_sizer->Add(s_box2, 0, wxEXPAND | wxALIGN_TOP | wxALL, 5);
	s_grid_box = new wxFlexGridSizer(5, 2, 4, 0);
	s_box2->Add(s_grid_box, 0, wxEXPAND | wxALIGN_TOP);
	s_type_label = new wxStaticText(this, -1, wxT("Sequence type:"), wxDefaultPosition, wxSize(120, -1));
	wxString	anim_type_labels[] = { wxT("Display a random frame"),
										wxT("Animation with 1 view"),
										wxT("Animation with 4 views"),
										wxT("Animation with 5 views"),
										wxT("Animation with 8 views") };
	s_type_menu = new wxChoice(this, MENU_SEQ_TYPE, wxDefaultPosition, wxDefaultSize, 5, anim_type_labels, 0);
	s_fpv_label = new wxStaticText(this, -1, wxT("Frames per view:"), wxDefaultPosition, wxSize(120, -1));
	s_fpv_field = new wxTextCtrl(this, FIELD_SEQ_FRAMES_PER_VIEW, wxT("1"));
	s_tpf_label = new wxStaticText(this, -1, wxT("Ticks per frame:"), wxDefaultPosition, wxSize(120, -1));
	s_tpf_field = new wxTextCtrl(this, FIELD_SEQ_TICKS_PER_FRAME, wxT("1"));
	s_lf_label = new wxStaticText(this, -1, wxT("Loop at frame:"), wxDefaultPosition, wxSize(120, -1));
	s_lf_field = new wxTextCtrl(this, FIELD_SEQ_LOOP_FRAME, wxT("0"));
	s_kf_label = new wxStaticText(this, -1, wxT("Key frame:"), wxDefaultPosition, wxSize(120, -1));
	s_kf_field = new wxTextCtrl(this, FIELD_SEQ_KEY_FRAME, wxT("0"));
	s_grid_box->Add(s_type_label, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	s_grid_box->Add(s_type_menu, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	s_grid_box->Add(s_fpv_label, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	s_grid_box->Add(s_fpv_field, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	s_grid_box->Add(s_tpf_label, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	s_grid_box->Add(s_tpf_field, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	s_grid_box->Add(s_lf_label, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	s_grid_box->Add(s_lf_field, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	s_grid_box->Add(s_kf_label, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	s_grid_box->Add(s_kf_field, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	s_separator = new wxStaticLine(this, -1, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	s_box2->Add(s_separator, 1, wxEXPAND);

	s_grid_box2 = new wxFlexGridSizer(6, 2, 4, 0);
	s_box2->Add(s_grid_box2, 0, wxEXPAND | wxALIGN_TOP);
	s_xfermode_label = new wxStaticText(this, -1, wxT("Transfer mode:"), wxDefaultPosition, wxSize(140, -1));
	wxString	xfermode_labels[] = {	wxT("Normal"),
										wxT("Fade out to black"),
										wxT("50% invisibility"),
										wxT("75% invisibility"),
										wxT("Pulsate"),
										wxT("Wobble"),
										wxT("Fast wobble"),
										wxT("100% static"),
										wxT("50% static"),
										wxT("Landscape"),
										wxT("Smear"),
										wxT("Fade out static"),
										wxT("Pulsating static"),
										wxT("Fold in"),
										wxT("Fold out"),
										wxT("Horizontal slide"),
										wxT("Fast horizontal slide"),
										wxT("Vertical slide"),
										wxT("Fast vertical slide"),
										wxT("Wander"),
										wxT("Fast wander"),
										wxT("Big landscape")
									};
	s_xfermode_menu = new wxChoice(this, MENU_SEQ_XFER_MODE, wxDefaultPosition, wxDefaultSize, 22, xfermode_labels, 0);
	s_xferperiod_label = new wxStaticText(this, -1, wxT("Transfer mode period:"), wxDefaultPosition, wxSize(150, -1));
	s_xferperiod_field = new wxTextCtrl(this, FIELD_SEQ_XFER_MODE_PERIOD, wxT("1"));
	s_ffs_label = new wxStaticText(this, -1, wxT("First frame sound:"), wxDefaultPosition, wxSize(150, -1));
	s_ffs_field = new wxTextCtrl(this, FIELD_SEQ_FIRST_FRAME_SND, wxT("0"));
	s_kfs_label = new wxStaticText(this, -1, wxT("Key frame sound:"), wxDefaultPosition, wxSize(150, -1));
	s_kfs_field = new wxTextCtrl(this, FIELD_SEQ_KEY_FRAME_SND, wxT("0"));
	s_lfs_label = new wxStaticText(this, -1, wxT("Last frame sound:"), wxDefaultPosition, wxSize(150, -1));
	s_lfs_field = new wxTextCtrl(this, FIELD_SEQ_LAST_FRAME_SND, wxT("0"));
	s_sf_label = new wxStaticText(this, -1, wxT("Shape scale factor:"), wxDefaultPosition, wxSize(150, -1));
	s_sf_field = new wxTextCtrl(this, FIELD_SEQ_SCALE_FACTOR, wxT("1"));
	s_grid_box2->Add(s_xfermode_label, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxRIGHT, 5);
	s_grid_box2->Add(s_xfermode_menu, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	s_grid_box2->Add(s_xferperiod_label, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxRIGHT, 5);
	s_grid_box2->Add(s_xferperiod_field, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	s_grid_box2->Add(s_ffs_label, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	s_grid_box2->Add(s_ffs_field, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	s_grid_box2->Add(s_kfs_label, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	s_grid_box2->Add(s_kfs_field, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	s_grid_box2->Add(s_lfs_label, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	s_grid_box2->Add(s_lfs_field, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	s_grid_box2->Add(s_sf_label, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
	s_grid_box2->Add(s_sf_field, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);

	s_fb = new SequenceView(this, wxID_ANY);
	s_outer_sizer->Add(s_fb, 1, wxEXPAND | wxALL, 5);
	mainbox->Show(s_outer_sizer, false);
	
	mainbox->Layout();

	// create the menu bar
	menubar = new wxMenuBar();
	// file menu
	file_menu = new wxMenu();
	file_menu->Append(wxID_OPEN, wxT("Open..."));
	file_menu->AppendSeparator();
	file_menu->Append(wxID_SAVE, wxT("Save"));
	file_menu->Append(wxID_SAVEAS, wxT("Save as..."));
	file_menu->AppendSeparator();
	file_menu->Append(wxID_EXIT, wxT("Quit"));
	file_menu->Enable(wxID_SAVE, false);
	file_menu->Enable(wxID_SAVEAS, false);
	// edit menu
	edit_menu = new wxMenu();
	edit_menu->Append(wxID_CUT, wxT("Cut"));
	edit_menu->Append(wxID_COPY, wxT("Copy"));
	edit_menu->Append(wxID_PASTE, wxT("Paste"));
	edit_menu->AppendSeparator();
	edit_menu->Append(wxID_DELETE, wxT("Delete"));
	edit_menu->Enable(wxID_CUT, false);
	edit_menu->Enable(wxID_COPY, false);
	edit_menu->Enable(wxID_PASTE, false);
	edit_menu->Enable(wxID_DELETE, false);
	// view menu
	view_menu = new wxMenu();
	view_colortable_submenu = new wxMenu();
	for (unsigned int i = 0; i < 8; i++) {
		view_colortable_submenu->AppendRadioItem(MVIEW_COLORTABLE_0 + i, INT_TO_WXSTRING(i));
		view_colortable_submenu->Enable(MVIEW_COLORTABLE_0 + i, false);
	}
	view_menu->Append(MVIEW_COLOR_TABLE, wxT("Use color table"), view_colortable_submenu);
	view_tnsize_submenu = new wxMenu();
	view_tnsize_submenu->AppendRadioItem(MVIEW_TNSIZE_SMALL, wxT("Small"));
	view_tnsize_submenu->AppendRadioItem(MVIEW_TNSIZE_MEDIUM, wxT("Medium"));
	view_tnsize_submenu->AppendRadioItem(MVIEW_TNSIZE_LARGE, wxT("Large"));
	view_tnsize_submenu->AppendRadioItem(MVIEW_TNSIZE_AUTO, wxT("Best fit"));
	for (unsigned int i = MVIEW_TNSIZE_SMALL; i <= MVIEW_TNSIZE_AUTO; i++)
		view_tnsize_submenu->Check(i, i == MVIEW_TNSIZE_MEDIUM);
	view_menu->Append(MVIEW_COLOR_TABLE, wxT("Thumbnail size"), view_tnsize_submenu);
	view_menu->AppendCheckItem(MVIEW_TRANSPARENCY, wxT("Show transparent pixels"));
	// shapes menu
	shapes_menu = new wxMenu();
	shapes_menu->Append(MSHAPES_ADDCOLORTABLE, wxT("New color table..."));
	shapes_menu->Append(MSHAPES_SAVECOLORTABLE, wxT("Save color table to GIMP..."));
	shapes_menu->Append(MSHAPES_SAVECOLORTABLETOPS, wxT("Save color table to PhotoShop..."));
	shapes_menu->AppendSeparator();
	shapes_menu->Append(MSHAPES_ADDBITMAP, wxT("New bitmap..."));
	shapes_menu->Append(MSHAPES_EXPORTBITMAPS, wxT("Save all bitmaps..."));
	shapes_menu->AppendSeparator();
	shapes_menu->Append(MSHAPES_ADDFRAME, wxT("New frame"));
	shapes_menu->AppendSeparator();
	shapes_menu->Append(MSHAPES_ADDSEQUENCE, wxT("New sequence"));
	shapes_menu->Enable(MSHAPES_ADDCOLORTABLE, false);
	shapes_menu->Enable(MSHAPES_SAVECOLORTABLE, false);
	shapes_menu->Enable(MSHAPES_SAVECOLORTABLETOPS, false);
	shapes_menu->Enable(MSHAPES_ADDBITMAP, false);
	shapes_menu->Enable(MSHAPES_EXPORTBITMAPS, false);
	shapes_menu->Enable(MSHAPES_ADDFRAME, false);
	shapes_menu->Enable(MSHAPES_ADDSEQUENCE, false);
	// help menu
	help_menu = new wxMenu();
	help_menu->Append(wxID_ABOUT, wxT("About..."));
	menubar->Append(file_menu, wxT("&File"));
	menubar->Append(edit_menu, wxT("&Edit"));
	menubar->Append(view_menu, wxT("&View"));
	menubar->Append(shapes_menu, wxT("&Shapes"));
	menubar->Append(help_menu, wxT("&Help"));

	SetSizer(mainbox);
	SetMenuBar(menubar);
	
	menubar->EnableTop(menubar->FindMenu(wxT("Shapes")), false);
}

ShapesEditor::~ShapesEditor(void)
{

}

// return the id of the "Sequences" tree item relative to
// the specified collection and color version
wxTreeItemId ShapesEditor::GetSequencesTreeItem(unsigned int collection, unsigned int version) const
{
	wxTreeItemIdValue	thecookie;
	wxTreeItemId		rootid = colltree->GetRootItem(),
						collnameid = colltree->GetFirstChild(rootid, thecookie);

	// descend into the tree towards the right item
	while (collnameid.IsOk()) {
		MyTreeItemData	*collname_data = dynamic_cast<MyTreeItemData *>(colltree->GetItemData(collnameid));

		if (collname_data->CollID() == (int)collection) {
			// found right collection branch
			wxTreeItemIdValue	thecookieII;
			wxTreeItemId		collversid = colltree->GetFirstChild(collnameid, thecookieII);

			while (collversid.IsOk()) {
				MyTreeItemData	*collvers_data = dynamic_cast<MyTreeItemData *>(colltree->GetItemData(collversid));

				if (collvers_data->Version() == (int)version) {
					// found right version node
					wxTreeItemIdValue	thecookieIII;
					wxTreeItemId		sectid = colltree->GetFirstChild(collversid, thecookieIII);

					while (sectid.IsOk()) {
						MyTreeItemData	*sect_data = dynamic_cast<MyTreeItemData *>(colltree->GetItemData(sectid));

						if (sect_data->Section() == TREESECTION_SEQUENCES) {
							// here we are
							return sectid;
						}
						sectid = colltree->GetNextChild(collversid, thecookieIII);
					}
				}
				collversid = colltree->GetNextChild(collnameid, thecookieII);
			}
		}
		collnameid = colltree->GetNextChild(rootid, thecookie);
	}
	return wxTreeItemId();	// ctor makes it invalid, so ok
}

void ShapesEditor::MenuFileOpen(wxCommandEvent &e)
{
	// let user choose the file
	wxFileDialog	*dlg = new wxFileDialog(this, wxT("Select a Marathon shapes file:"),
								wxT(""), wxT(""), wxT("*"), wxOPEN);

	if (dlg->ShowModal() == wxID_OK) {
		wxString	s = dlg->GetPath();
		wxBeginBusyCursor();
		Shapes		*newshapes = new Shapes((std::string)s.fn_str());
		wxEndBusyCursor();

		if (newshapes->GoodData()) {
			// delete the old shapes block and insert the new one
			if (payload != NULL)
				delete payload;
			payload = newshapes;
			filepath = s;
			// update all levels of the tree control
			colltree->DeleteAllItems();
			wxTreeItemId	treeroot = colltree->AddRoot(filepath.AfterLast('/'));

			for (unsigned int i = 0; i < COLLECTIONS_PER_FILE; i++) {
				// collection name nodes
				MyTreeItemData	*itemdata = new MyTreeItemData(i, -1, TREESECTION_COLLECTION);
				wxTreeItemId	coll = colltree->AppendItem(treeroot, wxString(collnames[i], wxConvUTF8), -1, -1, itemdata);

				for (unsigned int j = 0; j < 2; j++) {
					// color version nodes
					MyTreeItemData	*id = new MyTreeItemData(i, j, TREESECTION_VERSION);
					wxString		label;

					if (j == COLL_VERSION_8BIT)
						label = wxT("8-bit color version");
					else if (j == COLL_VERSION_TRUECOLOR)
						label = wxT("True color version");
					wxTreeItemId	coll2 = colltree->AppendItem(coll, label, -1, -1, id);
	
					if (payload->CollDefined(i, j)) {
						// section nodes
						MyTreeItemData	*id_b = new MyTreeItemData(i, j, TREESECTION_BITMAPS),
										*id_ct = new MyTreeItemData(i, j, TREESECTION_COLORTABLES),
										*id_f = new MyTreeItemData(i, j, TREESECTION_FRAMES),
										*id_s = new MyTreeItemData(i, j, TREESECTION_SEQUENCES);
						wxTreeItemId	coll_b = colltree->AppendItem(coll2, wxT("Bitmaps"), -1, -1, id_b),
										coll_ct = colltree->AppendItem(coll2, wxT("Color tables"), -1, -1, id_ct),
										coll_f = colltree->AppendItem(coll2, wxT("Frames"), -1, -1, id_f),
										coll_s = colltree->AppendItem(coll2, wxT("Sequences"), -1, -1, id_s);
	
						for (unsigned int k = 0; k < payload->CollSeqCount(i, j); k++) {
							// sequence nodes
							ShpSequence		*seq = payload->GetSeq(i, j, k);
							wxString    	blabel;
							MyTreeItemData	*id_seq = new MyTreeItemData(i, j, TREESECTION_SEQUENCES, k);
	
							blabel << k;
							if (strlen(seq->name) > 0)
								blabel << wxT(" - ") << wxString(seq->name, seqnameconv);
							colltree->AppendItem(coll_s, blabel, -1, -1, id_seq);
						}
					}
				}
			}
			colltree->Expand(colltree->GetRootItem());
			SetTitle(s.AfterLast('/'));
			file_menu->Enable(wxID_SAVE, true);
			file_menu->Enable(wxID_SAVEAS, true);
			menubar->EnableTop(menubar->FindMenu(wxT("Shapes")), true);
		} else {
			// the Shapes object failed to load data from file
			wxString	errormsg;

			errormsg << wxT("Sorry, could not open ");
			errormsg << s;
			errormsg << wxT(". Probably it is not a Marathon Shapes file, or it may be damaged.");
			wxMessageBox(errormsg, wxT("Error opening file"), wxOK | wxICON_ERROR, this);
		}
	}
	dlg->Destroy();
}

void ShapesEditor::MenuFileSave(wxCommandEvent &e)
{
	if (e.GetId() == wxID_SAVEAS) {
		// "save as" command: ask a new file name and save to it
		wxString	newpath = wxFileSelector(wxT("Save Shapes as"),
						wxT(""), wxT("Shapes"), wxT(""), wxT("*.*"), wxSAVE | wxOVERWRITE_PROMPT);

		if (!newpath.empty()) {
			wxBeginBusyCursor();
			payload->WriteToFile((std::string)newpath.fn_str());
			wxEndBusyCursor();
			filepath = newpath;
		}
	} else {
		// save to the current file path
		wxBeginBusyCursor();
		payload->WriteToFile((std::string)filepath.fn_str());
		wxEndBusyCursor();
	}
}

void ShapesEditor::MenuFileQuit(wxCommandEvent &e)
{
	Close(false);
}

// TODO
void ShapesEditor::MenuEditDelete(wxCommandEvent &e)
{
	// delete selected color table, bitmap, frame or sequence
	int	selection = bb->GetSelection();

	if (selection > -1) {
		// the user interface update should be nearly identical to the TreeSelect case
		bb->Freeze();
		bb->Clear();	// FIXME just remove THAT bitmap!
		b_outer_sizer->Show(b_count_label, true);
		b_outer_sizer->Show(b_edit_box, false);
		b_view->SetBitmap(NULL);
		payload->DeleteBitmap(selected_coll, selected_vers, selection);

		unsigned int	bitmap_count = payload->CollBitmapCount(selected_coll, selected_vers);

		for (unsigned int i = 0; i < bitmap_count; i++)
			bb->AddBitmap(payload->GetBitmap(selected_coll, selected_vers, i));
		bb->Thaw();

		wxString	count_string;
		
		count_string << bitmap_count << wxT(" bitmap");
		if (bitmap_count != 1)
			count_string << wxT("s");
		b_count_label->SetLabel(count_string);
		Layout();
	}
}

void ShapesEditor::MenuViewTransparency(wxCommandEvent &e)
{
	show_transparent_pixels = e.IsChecked();
	wxBeginBusyCursor();
	bb->SetTranspPixelsDisplay(!show_transparent_pixels);
	b_view->SetTranspPixelsDisplay(!show_transparent_pixels);
	fb->SetTranspPixelsDisplay(!show_transparent_pixels);
	f_view->SetTranspPixelsDisplay(!show_transparent_pixels);
	s_fb->SetTranspPixelsDisplay(!show_transparent_pixels);
	wxEndBusyCursor();
}

// view->color table menu handler
void ShapesEditor::MenuViewCT(wxCommandEvent &e)
{
	view_ct = e.GetId() - MVIEW_COLORTABLE_0;

	ShpColorTable	*ctp = payload->GetColorTable(selected_coll, selected_vers, view_ct);

	wxBeginBusyCursor();
	bb->SetColorTable(ctp);
	b_view->SetColorTable(ctp);
	fb->SetColorTable(ctp);
	f_view->SetColorTable(ctp);
	s_fb->SetColorTable(ctp);
	wxEndBusyCursor();
}

// view->thumbnail size menu handler
void ShapesEditor::MenuViewTNSize(wxCommandEvent &e)
{
	int	size = -1;

	switch (e.GetId()) {
		case MVIEW_TNSIZE_SMALL:
			size = 32;
			break;
		case MVIEW_TNSIZE_MEDIUM:
			size = 64;
			break;
		case MVIEW_TNSIZE_LARGE:
			size = 128;
			break;
		case MVIEW_TNSIZE_AUTO:
			size = -1;
			break;
	}
	wxBeginBusyCursor();
	bb->SetThumbnailSize(size);
	fb->SetThumbnailSize(size);
	s_fb->SetThumbnailSize(size);
	wxEndBusyCursor();
}

// TODO
void ShapesEditor::MenuShapesAddColorTable(wxCommandEvent &e)
{
	wxFileDialog	*dlg = new wxFileDialog(this, wxT("Choose a GIMP color table"), 
					                                wxT(""), wxT(""), wxT("*"), wxOPEN);

	if (dlg->ShowModal() == wxID_OK) {
		wxString		filename = dlg->GetPath();
		std::ifstream	fstream(filename.mb_str(), std::ios::in);

		if (fstream.good()) {
			// TODO handle both GIMP and Photoshop palettes
			// if this is going to be the first color table,
			// then add every color. Otherwise, keep only the
			// first <color per table> colors (warn the user)
			fstream.close();
		} else {
			std::cerr << "Error loading palette " << filename.mb_str() << "\n";
		}
	}
	dlg->Destroy();
}

// prompt the user and save the selected color table, either in
// Gimp or PhotoShop format (depending on event id)
void ShapesEditor::MenuShapesSaveColorTable(wxCommandEvent &e)
{
	if (payload != NULL && selected_coll != -1 && selected_vers != -1) {
		int	selection = ctb->GetSelection();

		if (selection >= 0) {
			bool		ps = (e.GetId() == MSHAPES_SAVECOLORTABLETOPS);
			wxString	prompt = wxString::Format(wxT("Save color table %d"), selection),
						name, ctpath;

			if (ps) {
				name = wxString::Format(wxT("ColorTable%d.act"), selection);
				ctpath = wxFileSelector(prompt, wxT(""), name, wxT(""),
										wxT("PhotoShop color table|*.act"),
										wxSAVE | wxOVERWRITE_PROMPT);
			} else {
				name = wxString::Format(wxT("ColorTable%d.gpl"), selection);
				ctpath = wxFileSelector(prompt, wxT(""), name, wxT(""),
										wxT("Gimp color table|*.gpl"),
										wxSAVE | wxOVERWRITE_PROMPT);
			}

			if (!ctpath.empty()) {
				ShpColorTable	*ct = payload->GetColorTable(selected_coll, selected_vers, selection);
				std::ofstream	cts(ctpath.fn_str());

				if (cts.good()) {
					// file opened successfully
					if (ps) {
						// PhotoShop binary color table format
						// (MacOS file type is '8BCT', extension '.act')
						unsigned char	*rgb = new unsigned char[ct->colors.size() * 3];

						if (rgb != NULL) {
							unsigned char	*prgb = rgb;

							// first a block of RGB byte triplets
							for (unsigned int i = 0; i < ct->colors.size(); i++) {
								*prgb++ = ct->colors[i].red >> 8;
								*prgb++ = ct->colors[i].green >> 8;
								*prgb++ = ct->colors[i].blue >> 8;
							}
							cts.write((char *)rgb, ct->colors.size() * 3);
							delete[] rgb;
							// if we didn't reach 256 colors, pad with zeroes to fill and end with
							//    00 n ff ff
							// where n is the number of colors
							if (ct->colors.size() < 256) {
								unsigned int	padsize = 3 * (256 - ct->colors.size());
								unsigned char	*pad = new unsigned char[padsize],
												tail[4] = { 0, ct->colors.size(), 0xff, 0xff};
								
								memset(pad, 0, padsize);
								cts.write((char *)pad, padsize);
								cts.write((char *)tail, 4);
								delete[] pad;
							}
						}
					} else {
						// Gimp ASCII format
						cts << "GIMP Palette\n";
						cts << "Name: ShapeFusion exported palette\n";
						cts << "#\n";
						for (unsigned int i = 0; i < ct->colors.size(); i++) {
							cts << (ct->colors[i].red >> 8) << ' ';
							cts << (ct->colors[i].green >> 8) << ' ';
							cts << (ct->colors[i].blue >> 8) << '\n';
						}
					}
					cts.close();
				} else {
					// failed to open file for writing
					wxString    errormsg;

					errormsg << wxT("Sorry, could not save color table to ") << ctpath << wxT(".");
					wxMessageBox(errormsg, wxT("Error saving color table"), wxOK | wxICON_ERROR, this);
				}
			}
		}
	}
}

void ShapesEditor::MenuShapesAddBitmap(wxCommandEvent &e)
{
	if (payload != NULL && selected_coll != -1 && selected_vers != -1) {
		wxFileDialog	*dlg = new wxFileDialog(this, wxT("Choose a bitmap to add"),
													wxT(""), wxT(""), wxT("BMP files|*.bmp"), wxOPEN);

		if (dlg->ShowModal() == wxID_OK) {
			wxString	filename = dlg->GetPath();
			wxImage		img;

			if (img.LoadFile(filename)) {
				// we have the wxImage now. Encode it to a new bitmap
				ShpBitmap		newbmp;
				ShpColorTable	*ct = payload->GetColorTable(selected_coll, selected_vers, view_ct);
				unsigned char	*srcpixels = img.GetData(),
								*src = srcpixels,
								*dst;

				wxBeginBusyCursor();
				newbmp.column_order = newbmp.enable_transparency = false;
				newbmp.width = img.GetWidth();
				newbmp.height = img.GetHeight();
				newbmp.bit_depth = 8;
				newbmp.bytes_per_row = img.GetWidth();
				newbmp.pixels = new unsigned char[newbmp.width * newbmp.height];
				if (newbmp.pixels == NULL) {
					std::cerr << "Could not allocate new " << newbmp.width << "x" << newbmp.height << " bitmap\n";
					dlg->Destroy();
					return;
				}
				dst = newbmp.pixels;
				// automagically initialize bitmap flags
				if (payload->CollType(selected_coll, selected_vers) == _object_collection ||
						payload->CollType(selected_coll, selected_vers) == _scenery_collection) {
					newbmp.bytes_per_row = -1;
					newbmp.column_order = true;
				}
				// quantize from 8-bit RGB pixels to an indexed bitmap. We need to transform
				// RGB to HSV and perform the comparison in that space to get good results.
				// FIXME 
				// - this is not yet perfect (not as good as PhotoShop or Gimp)
				// - move all this code to another place (utilities.cpp?)
				for (int i = 0; i < newbmp.width * newbmp.height; i++) {
					unsigned char 	r = *src++, g = *src++, b = *src++;
					float			hue, sat, val,
									min_dist = 0;
					unsigned char   best_value = 0;

					RGB2HSV(r / 255.0, g / 255.0, b / 255.0, &hue, &sat, &val);
					for (unsigned int j = 0; j < ct->colors.size(); j++) {
						unsigned short	ct_r = ct->colors[j].red,
										ct_g = ct->colors[j].green,
										ct_b = ct->colors[j].blue;
						float			hue2, sat2, val2,
										delta_h, delta_s, delta_v,
										dist;

						RGB2HSV(ct_r / 65535.0, ct_g / 65535.0, ct_b / 65535.0, &hue2, &sat2, &val2);
						delta_h = hue2 - hue;
						delta_s = sat2 - sat;
						delta_v = val2 - val;
						dist = delta_h*delta_h + delta_s*delta_s + delta_v*delta_v;
						if (dist < min_dist || j == 0) {
							min_dist = dist;
							best_value = ct->colors[j].value;
						}
					}
					*dst++ = best_value;
					if (best_value == 0)
						newbmp.enable_transparency = true;	// guess the user will want transparency
				}
				payload->InsertBitmap(newbmp, selected_coll, selected_vers);

				// update the GUI
				unsigned int	bitmap_count = payload->CollBitmapCount(selected_coll, selected_vers);
				ShpBitmap		*pnewbitmap = payload->GetBitmap(selected_coll, selected_vers, bitmap_count-1);
				wxString		count_string = wxString::Format(wxT("%u bitmap"), bitmap_count);

				bb->AddBitmap(pnewbitmap);
				fb->AddBitmap(pnewbitmap);
				if (bitmap_count != 1)
					count_string << wxT("s");
				b_count_label->SetLabel(count_string);
				
				wxEndBusyCursor();
			} else {
				wxString	errormsg;

				errormsg << wxT("Sorry, could not load bitmap from ") << filename << wxT(".");
				wxMessageBox(errormsg, wxT("Error adding bitmap"), wxOK | wxICON_ERROR, this);
			}
		}
		dlg->Destroy();
	}
}

// save all bitmaps of the selected collection as separate files in a folder
void ShapesEditor::MenuShapesExportBitmaps(wxCommandEvent &e)
{
	wxString	dir = wxDirSelector(wxT("Select destination folder"));

	if (!dir.empty()) {
		for (unsigned int i = 0; i < payload->CollBitmapCount(selected_coll, selected_vers); i++) {
			wxString	file_name = wxString::Format(wxT("bitmap%.3d.bmp"), i);
			wxImage		img = ShpBitmapToImage(payload->GetBitmap(selected_coll, selected_vers, i),
							payload->GetColorTable(selected_coll, selected_vers, view_ct),
							!show_transparent_pixels);

			img.SaveFile(dir + wxT("/") + file_name, wxBITMAP_TYPE_BMP);
		}
	}
}

void ShapesEditor::MenuShapesNewFrame(wxCommandEvent &e)
{
	if (payload != NULL && selected_coll != -1 && selected_vers != -1) {
		// append an empty frame
		ShpFrame	newframe;

		// initialize values to something reasonable
		newframe.bitmap_index = -1;
		newframe.x_mirror = newframe.y_mirror = newframe.keypoint_obscured = false;
		newframe.minimum_light_intensity = 0;
		newframe.origin_x = newframe.origin_y = newframe.key_x = newframe.key_y = 0;
		newframe.scale_factor = payload->CollScaleFactor(selected_coll, selected_vers);
		newframe.world_left = newframe.world_right = newframe.world_top = newframe.world_bottom = 0;
		newframe.world_x0 = newframe.world_y0 = 0;
		payload->InsertFrame(newframe, selected_coll, selected_vers);
		fb->AddFrame(payload->GetFrame(selected_coll, selected_vers, payload->CollFrameCount(selected_coll, selected_vers)-1));
		// update frame count label
		unsigned int	frame_count = payload->CollFrameCount(selected_coll, selected_vers);
		wxString		count_string = wxString::Format(wxT("%u frame"), frame_count);
		
		if (frame_count != 1)
			count_string << wxT("s");
		f_count_label->SetLabel(count_string);
	}
}

void ShapesEditor::MenuShapesNewSequence(wxCommandEvent &e)
{
	if (payload != NULL && selected_coll != -1 && selected_vers != -1) {
		// append an empty sequence
		ShpSequence		newseq;

		// initialize values to something reasonable
		newseq.type = 0;
		newseq.flags = 0;
		strcpy(newseq.name, "new sequence");
		newseq.number_of_views = UNANIMATED;
		newseq.frames_per_view = 0;
		newseq.ticks_per_frame = 1;
		newseq.key_frame = 0;
		newseq.transfer_mode = 0;
		newseq.transfer_mode_period = 1;
		newseq.first_frame_sound = newseq.key_frame_sound = newseq.last_frame_sound = -1;
		newseq.pixels_to_world = 0;
		newseq.loop_frame = 0;
		payload->InsertSequence(newseq, selected_coll, selected_vers);
		
		// insert the new entry in the main tree
		wxTreeItemId	thenode = GetSequencesTreeItem(selected_coll, selected_vers);
		int				seq_id = payload->CollSeqCount(selected_coll, selected_vers) - 1;
		ShpSequence     *seq = payload->GetSeq(selected_coll, selected_vers, seq_id);
		MyTreeItemData  *itemdata = new MyTreeItemData(selected_coll, selected_vers, TREESECTION_SEQUENCES, seq_id);
		wxString		label;

		label << seq_id;
		if (strlen(seq->name) > 0)
			label << wxT(" - ") << wxString(seq->name, seqnameconv);
		colltree->AppendItem(thenode, label, -1, -1, itemdata);
	}
}

void ShapesEditor::MenuHelpAbout(wxCommandEvent &e)
{
	 wxMessageDialog *dlg = new wxMessageDialog(this,
								wxT("ShapeFusion 0.2\nhttp://shapefusion.sourceforge.net\n\nCopyright 2000-2007, Tito Dal Canton\n\n"
									"An editor for Marathon II, Marathon Infinity and AlephOne shapes files. "
									"Released under the terms of the GNU General Public License: for more information "
									"see the COPYING file that comes with ShapeFusion."),
								wxT("About ShapeFusion"), wxOK | wxICON_INFORMATION);
	 dlg->ShowModal();
	 dlg->Destroy();
}

// user selected a tree entry
void ShapesEditor::TreeSelect(wxTreeEvent &e)
{
	MyTreeItemData	*data = dynamic_cast<MyTreeItemData *>(colltree->GetItemData(e.GetItem()));

	if (data) {
		int	new_coll = data->CollID(),
			new_vers = data->Version(),
			new_section = data->Section();

		if (new_coll != selected_coll || new_vers != selected_vers) {
			// user has moved to another collection/version

			// first clear the user interface
			int	ctmenucount = view_colortable_submenu->GetMenuItemCount();

			view_ct = -1;
			ctb->Clear();
			bb->Freeze();
			bb->Clear();
			b_view->SetBitmap(NULL);
			fb->Freeze();
			fb->Clear();
			f_view->SetFrame(NULL);
			f_view->SetBitmap(NULL);
			edit_menu->SetLabel(wxID_DELETE, wxT("Delete"));
			edit_menu->Enable(wxID_DELETE, false);
			for (int i = 0; i < ctmenucount; i++)
				view_colortable_submenu->Enable(MVIEW_COLORTABLE_0 + i, false);
			shapes_menu->Enable(MSHAPES_SAVECOLORTABLE, false);
			shapes_menu->Enable(MSHAPES_SAVECOLORTABLETOPS, false);

			// set collection info panel
			coll_static_box->SetLabel(wxString::Format(wxT("Global info for collection %d"), new_coll));

			wxString	collinfo_s;
			collinfo_s << wxT("Status: ") << payload->CollGlobalStatus(new_coll) << wxT("\n");
			collinfo_s << wxT("Flags: ") << payload->CollGlobalFlags(new_coll) << wxT("\n\n");
			if (payload->CollDefined(new_coll, COLL_VERSION_8BIT))
				collinfo_s << wxT("8-bit color version present\n");
			else
				collinfo_s << wxT("No 8-bit color version\n");
			if (payload->CollDefined(new_coll, COLL_VERSION_TRUECOLOR))
				collinfo_s << wxT("True color version present");
			else
				collinfo_s << wxT("No true color version");
			coll_text->SetLabel(collinfo_s);

			if (new_coll != -1 && new_vers != -1) {
				if (new_vers == COLL_VERSION_TRUECOLOR)
					chunk_static_box->SetLabel(wxT("Shapes for true color and OpenGL display"));
				else
					chunk_static_box->SetLabel(wxT("Shapes for 8-bit color display"));

				if (payload->CollDefined(new_coll, new_vers)) {
					// a defined collection has been selected
					unsigned int	ct_count = payload->CollCTCount(new_coll, new_vers),
									bitmap_count = payload->CollBitmapCount(new_coll, new_vers),
									frame_count = payload->CollFrameCount(new_coll, new_vers);
					wxString		count_string;

					wxBeginBusyCursor();
					shapes_menu->Enable(MSHAPES_ADDBITMAP, true);
					shapes_menu->Enable(MSHAPES_EXPORTBITMAPS, true);
					shapes_menu->Enable(MSHAPES_ADDFRAME, true);
					shapes_menu->Enable(MSHAPES_ADDSEQUENCE, true);

					// chunk info panel
					chunk_version_field->SetValue(INT_TO_WXSTRING(payload->CollVersion(new_coll, new_vers)));
					chunk_type_menu->SetSelection(payload->CollType(new_coll, new_vers));
					chunk_flags_field->SetValue(INT_TO_WXSTRING(payload->CollFlags(new_coll, new_vers)));
					chunk_sf_field->SetValue(INT_TO_WXSTRING(payload->CollScaleFactor(new_coll, new_vers)));

					// color tables
					view_ct = 0;
					for (unsigned int i = 0; i < ct_count; i++)
						ctb->AddColorTable(payload->GetColorTable(new_coll, new_vers, i));
					for (int i = 0; i < ctmenucount; i++)
						view_colortable_submenu->Enable(MVIEW_COLORTABLE_0 + i, i < (int)ct_count);
					if (ct_count > 0)
						view_colortable_submenu->Check(MVIEW_COLORTABLE_0, true);
					count_string << ct_count << wxT(" color table");
					if (ct_count != 1)
						count_string << wxT("s");
					if (ct_count > 0)
						count_string << wxT(", ") << payload->GetColorTable(new_coll, new_vers, 0)->colors.size() << wxT(" colors per table");
					ct_count_label->SetLabel(count_string);

					// bitmaps
					bb->SetColorTable(payload->GetColorTable(new_coll, new_vers, 0));
					b_view->SetColorTable(payload->GetColorTable(new_coll, new_vers, 0));
					for (unsigned int i = 0; i < bitmap_count; i++)
						bb->AddBitmap(payload->GetBitmap(new_coll, new_vers, i));
					count_string.Clear();
					count_string << bitmap_count << wxT(" bitmap");
					if (bitmap_count != 1)
						count_string << wxT("s");
					b_count_label->SetLabel(count_string);

					// frames
					fb->SetColorTable(payload->GetColorTable(new_coll, new_vers, 0));
					f_view->SetColorTable(payload->GetColorTable(new_coll, new_vers, 0));
					for (unsigned int i = 0; i < bitmap_count; i++)
						fb->AddBitmap(payload->GetBitmap(new_coll, new_vers, i));
					for (unsigned int i = 0; i < frame_count; i++)
						fb->AddFrame(payload->GetFrame(new_coll, new_vers, i));
					count_string.Clear();
					count_string << frame_count << wxT(" frame");
					if (frame_count != 1)
						count_string << wxT("s");
					f_count_label->SetLabel(count_string);

					wxEndBusyCursor();
				} else {
					shapes_menu->Enable(MSHAPES_ADDBITMAP, false);
					shapes_menu->Enable(MSHAPES_EXPORTBITMAPS, false);
					shapes_menu->Enable(MSHAPES_ADDFRAME, false);
					shapes_menu->Enable(MSHAPES_ADDSEQUENCE, false);
				}
			} else {
				shapes_menu->Enable(MSHAPES_ADDBITMAP, false);
				shapes_menu->Enable(MSHAPES_EXPORTBITMAPS, false);
				shapes_menu->Enable(MSHAPES_ADDFRAME, false);
				shapes_menu->Enable(MSHAPES_ADDSEQUENCE, false);
			}
			selected_coll = new_coll;
			selected_vers = new_vers;
			bb->Thaw();
			fb->Thaw();
		}
	
		// handle sequence selection
		selected_sequence = data->Sequence();
		if (selected_sequence > -1) {
			ShpSequence *seq = payload->GetSeq(new_coll, new_vers, selected_sequence);

			// setup sequence panel controls
			s_outer_static_box->SetLabel(wxString::Format(wxT("Sequence %d of %u"),
									selected_sequence, payload->CollSeqCount(new_coll, new_vers)));
			s_name_field->SetValue(wxString(seq->name, seqnameconv));
			switch (seq->number_of_views) {
				case UNANIMATED:	s_type_menu->SetSelection(0);	break;
				case ANIMATED_1:	s_type_menu->SetSelection(1);	break;
				case ANIMATED_3TO4:	s_type_menu->SetSelection(2);	break;
				case ANIMATED_4:	s_type_menu->SetSelection(2);	break;
				case ANIMATED_3TO5:	s_type_menu->SetSelection(3);	break;
				case ANIMATED_5:	s_type_menu->SetSelection(3);	break;
				case ANIMATED_2TO8:	s_type_menu->SetSelection(4);	break;
				case ANIMATED_5TO8:	s_type_menu->SetSelection(4);	break;
				case ANIMATED_8:	s_type_menu->SetSelection(4);	break;
				default:
					s_type_menu->SetSelection(0);
					wxMessageBox(wxString::Format(wxT("This sequence has an unknown type %d, and ShapeFusion can not handle it. Something strange may happen now!"), seq->number_of_views),
									wxT("Warning"), wxOK | wxICON_ERROR, this);
					break;
			}
			s_fpv_field->SetValue(INT_TO_WXSTRING(seq->frames_per_view));
			s_tpf_field->SetValue(INT_TO_WXSTRING(seq->ticks_per_frame));
			s_lf_field->SetValue(INT_TO_WXSTRING(seq->loop_frame));
			s_kf_field->SetValue(INT_TO_WXSTRING(seq->key_frame));
			s_xfermode_menu->SetSelection(seq->transfer_mode);
			s_xferperiod_field->SetValue(INT_TO_WXSTRING(seq->transfer_mode_period));
			s_ffs_field->SetValue(INT_TO_WXSTRING(seq->first_frame_sound));
			s_kfs_field->SetValue(INT_TO_WXSTRING(seq->key_frame_sound));
			s_lfs_field->SetValue(INT_TO_WXSTRING(seq->last_frame_sound));
			s_sf_field->SetValue(INT_TO_WXSTRING(seq->pixels_to_world));
			// setup the sequence view
			wxBeginBusyCursor();
			s_fb->Freeze();
			s_fb->Clear();
			s_fb->SetColorTable(payload->GetColorTable(new_coll, new_vers, view_ct));
			for (unsigned int i = 0; i < payload->CollBitmapCount(selected_coll, selected_vers); i++)
				s_fb->AddBitmap(payload->GetBitmap(selected_coll, selected_vers, i));
			for (unsigned int i = 0; i < payload->CollFrameCount(selected_coll, selected_vers); i++)
				s_fb->AddFrame(payload->GetFrame(selected_coll, selected_vers, i));
			s_fb->SetSeqParameters(seq->number_of_views, seq->frames_per_view, &seq->frame_indexes);
			s_fb->Thaw();
			edit_menu->SetLabel(wxID_DELETE, wxT("Delete sequence"));
			edit_menu->Enable(wxID_DELETE, true);
			wxEndBusyCursor();
		}

		// handle section selection: show/hide editing panels
		mainbox->Show(dummy_sizer, false);
		mainbox->Show(coll_sizer, false);
		mainbox->Show(chunk_sizer, false);
		mainbox->Show(b_outer_sizer, false);
		mainbox->Show(ct_outer_sizer, false);
		mainbox->Show(f_outer_sizer, false);
		mainbox->Show(s_outer_sizer, false);
		switch (new_section) {
			case TREESECTION_COLLECTION:
				mainbox->Show(coll_sizer, true);
				break;
			case TREESECTION_VERSION:
				mainbox->Show(chunk_sizer, true);
				chunk_inner_box->Show(chunk_undef_label, !payload->CollDefined(selected_coll, selected_vers));
				chunk_inner_box->Show(chunk_grid, payload->CollDefined(selected_coll, selected_vers));
				break;
			case TREESECTION_BITMAPS:
				mainbox->Show(b_outer_sizer, true);
				b_outer_sizer->Show(b_edit_box, bb->GetSelection() != -1);
				b_outer_sizer->Show(b_count_label, bb->GetSelection() == -1);
				break;
			case TREESECTION_COLORTABLES:
				mainbox->Show(ct_outer_sizer, true);
				break;
			case TREESECTION_FRAMES:
				mainbox->Show(f_outer_sizer, true);
				f_outer_sizer->Show(f_edit_box, fb->GetSelection() != -1);
				f_outer_sizer->Show(f_count_label, fb->GetSelection() == -1);
				break;
			case TREESECTION_SEQUENCES:
				if (selected_sequence > -1)
					mainbox->Show(s_outer_sizer, true);
				else
					mainbox->Show(dummy_sizer, true);
				break;
		}
		mainbox->Layout();
	}
}

// selection event in the bitmap browser
void ShapesEditor::BitmapSelect(wxCommandEvent &e)
{
	int	selection = e.GetInt();

	if (selection < 0) {
		// deselection
		b_view->SetBitmap(NULL);
		b_outer_sizer->Show(b_count_label, true);
		b_outer_sizer->Show(b_edit_box, false);
		edit_menu->SetLabel(wxID_DELETE, wxT("Delete"));
		edit_menu->Enable(wxID_DELETE, false);
	} else {
		ShpBitmap	*sel_bitmap = payload->GetBitmap(selected_coll, selected_vers, selection);
		
		// set labels
		wxString	info_label = wxString::Format(wxT("%dx%d pixels, "), sel_bitmap->width, sel_bitmap->height);

		if (sel_bitmap->bytes_per_row > 0)
			info_label << wxT("plain encoding");
		else
			info_label << wxT("RLE");
		b_info_label->SetLabel(info_label);
		b_edit_static_box->SetLabel(wxString::Format(wxT("Bitmap %d of %d"),
								selection, payload->CollBitmapCount(selected_coll, selected_vers)));
		// set flag check boxes
		b_order_checkbox->SetValue(sel_bitmap->column_order);
		b_transparency_checkbox->SetValue(sel_bitmap->enable_transparency);
		// set bitmap view
		b_view->SetBitmap(sel_bitmap);
		b_outer_sizer->Show(b_count_label, false);
		b_outer_sizer->Show(b_edit_box, true);
		edit_menu->SetLabel(wxID_DELETE, wxT("Delete bitmap"));
		edit_menu->Enable(wxID_DELETE, true);
	}
	b_outer_sizer->Layout();
}

void ShapesEditor::BitmapDelete(wxCommandEvent &e)
{
	if (e.GetSelection() < 0)
		return;

	bb->Freeze();
	bb->Clear();    // FIXME just remove THAT bitmap!
	b_outer_sizer->Show(b_count_label, true);
	b_outer_sizer->Show(b_edit_box, false);
	b_view->SetBitmap(NULL);
	payload->DeleteBitmap(selected_coll, selected_vers, e.GetSelection());
	
	unsigned int    bitmap_count = payload->CollBitmapCount(selected_coll, selected_vers);
	
	for (unsigned int i = 0; i < bitmap_count; i++)
		bb->AddBitmap(payload->GetBitmap(selected_coll, selected_vers, i));
	bb->Thaw();
	
	wxString    count_string;
	
	count_string << bitmap_count << wxT(" bitmap");
	if (bitmap_count != 1)
		count_string << wxT("s");
	b_count_label->SetLabel(count_string);
	Layout();
}

void ShapesEditor::ToggleBitmapCheckboxes(wxCommandEvent &e)
{
	ShpBitmap	*sel_bitmap = b_view->GetBitmap();

	if (sel_bitmap != NULL) {
		switch (e.GetId()) {
			case CB_COLUMN_ORDER:
				sel_bitmap->column_order = e.IsChecked();
				break;
			case CB_ENABLE_TRANSPARENCY:
				sel_bitmap->enable_transparency = e.IsChecked();
				bb->RebuildThumbnail(bb->GetSelection());
				bb->Refresh();
				b_view->SetBitmap(sel_bitmap);
				// FIXME also update the FrameBrowser and all that
				break;
		}
	}
}

void ShapesEditor::CTSelect(wxCommandEvent &e)
{
	int	selection = e.GetInt();

	if (selection < 0) {
		// deselection
		shapes_menu->Enable(MSHAPES_SAVECOLORTABLE, false);
		shapes_menu->Enable(MSHAPES_SAVECOLORTABLETOPS, false);
		edit_menu->SetLabel(wxID_DELETE, wxT("Delete"));
		edit_menu->Enable(wxID_DELETE, false);
	} else {
		// selection
		shapes_menu->Enable(MSHAPES_SAVECOLORTABLE, true);
		shapes_menu->Enable(MSHAPES_SAVECOLORTABLETOPS, true);
		edit_menu->SetLabel(wxID_DELETE, wxT("Delete color table"));
		edit_menu->Enable(wxID_DELETE, true);
	}
}

// selection event in the frame browser
void ShapesEditor::FrameSelect(wxCommandEvent &e)
{
	int selection = e.GetInt();

	if (selection < 0) {
		f_view->SetFrame(NULL);
		f_view->SetBitmap(NULL);
		f_outer_sizer->Show(f_count_label, true);
		f_outer_sizer->Show(f_edit_box, false);
		edit_menu->SetLabel(wxID_DELETE, wxT("Delete"));
		edit_menu->Enable(wxID_DELETE, false);
	} else {
		ShpFrame	*sel_frame = payload->GetFrame(selected_coll, selected_vers, selection);
		ShpBitmap	*assoc_bitmap = NULL;

		// set labels
		f_edit_static_box->SetLabel(wxString::Format(wxT("Frame %d of %u"),
								selection, payload->CollFrameCount(selected_coll, selected_vers)));
		// set frame view
		f_view->SetFrame(sel_frame);
		if (sel_frame->bitmap_index >= 0)
			assoc_bitmap = payload->GetBitmap(selected_coll, selected_vers, sel_frame->bitmap_index);
		f_view->SetBitmap(assoc_bitmap);
		// set controls
		f_bitmap_id->SetRange(-1, payload->CollBitmapCount(selected_coll, selected_vers) - 1);
		f_bitmap_id->SetValue(sel_frame->bitmap_index);
		f_xmirror_checkbox->SetValue(sel_frame->x_mirror);
		f_ymirror_checkbox->SetValue(sel_frame->y_mirror);
		f_keypoint_checkbox->SetValue(sel_frame->keypoint_obscured);
		f_origin_x_field->SetValue(INT_TO_WXSTRING(sel_frame->origin_x));
		f_origin_y_field->SetValue(INT_TO_WXSTRING(sel_frame->origin_y));
		f_key_x_field->SetValue(INT_TO_WXSTRING(sel_frame->key_x));
		f_key_y_field->SetValue(INT_TO_WXSTRING(sel_frame->key_y));
		f_scalefactor_field->SetValue(INT_TO_WXSTRING(sel_frame->scale_factor));
		f_mli_field->SetValue(INT_TO_WXSTRING((int)(sel_frame->minimum_light_intensity * 100.0)));
		f_outer_sizer->Show(f_count_label, false);
		f_outer_sizer->Show(f_edit_box, true);
		edit_menu->SetLabel(wxID_DELETE, wxT("Delete frame"));
		edit_menu->Enable(wxID_DELETE, true);
	}
	f_outer_sizer->Layout();
}

void ShapesEditor::AskSaveBitmap(wxCommandEvent &e)
{
	int	selected_bmp = bb->GetSelection();
	
	if (selected_bmp < 0)
		return;

	wxString	prompt = wxString::Format(wxT("Save bitmap %d as:"), selected_bmp),
				filename = wxString::Format(wxT("bitmap%d.bmp"), selected_bmp);

	wxFileDialog	*dlg = new wxFileDialog(this, prompt, wxT(""), filename, wxT("*.bmp"), wxSAVE | wxOVERWRITE_PROMPT);

	if (dlg->ShowModal() == wxID_OK) {
		// FIXME create an 8-bit bitmap with the exact color table,
		// instead of wasting space with an RGB one
		wxImage	img = ShpBitmapToImage(payload->GetBitmap(selected_coll, selected_vers, selected_bmp),
										payload->GetColorTable(selected_coll, selected_vers, view_ct),
										!show_transparent_pixels);

		img.SaveFile(dlg->GetPath(), wxBITMAP_TYPE_BMP);
	}
	dlg->Destroy();
}

// bitmap index change in the frame panel
void ShapesEditor::BitmapIndexSpin(wxSpinEvent &e)
{
	int			newid = e.GetPosition();
	ShpFrame	*sel_frame = f_view->GetFrame();

	if (sel_frame != NULL) {
		sel_frame->bitmap_index = newid;
		f_view->SetBitmap(payload->GetBitmap(selected_coll, selected_vers, newid));
		f_view->SetFrame(sel_frame);
		fb->RebuildThumbnail(fb->GetSelection());
		fb->Refresh();
	}
}

// checkbox toggle in the frame panel
void ShapesEditor::ToggleFrameCheckboxes(wxCommandEvent &e)
{
	ShpFrame	*sel_frame = f_view->GetFrame();

	if (sel_frame != NULL) {
		switch (e.GetId()) {
			case CB_XMIRROR:	sel_frame->x_mirror = e.IsChecked();			break;
			case CB_YMIRROR:	sel_frame->y_mirror = e.IsChecked();			break;
			case CB_KEYPOINT:	sel_frame->keypoint_obscured = e.IsChecked();	break;
		}
		// update display
		if (e.GetId() != CB_KEYPOINT) {
			fb->RebuildThumbnail(fb->GetSelection());
			fb->Refresh();
			f_view->SetFrame(sel_frame);
		}
	}
}

// user messed with fields in the frame panel
void ShapesEditor::EditFrameFields(wxCommandEvent &e)
{
	ShpFrame	*sel_frame = f_view->GetFrame();
	wxString	s = e.GetString();

	if (sel_frame != NULL) {
		long	v = 0;
		
		if (s.ToLong(&v)) {
			bool	recalculate_world_fields = false;

			switch (e.GetId()) {
				case FIELD_ORIGIN_X:
					sel_frame->origin_x = v;
					recalculate_world_fields = true;
					break;
				case FIELD_ORIGIN_Y:
					sel_frame->origin_y = v;
					recalculate_world_fields = true;
					break;
				case FIELD_KEY_X:
					sel_frame->key_x = v;
					recalculate_world_fields = true;
					break;
				case FIELD_KEY_Y:
					sel_frame->key_y = v;
					recalculate_world_fields = true;
					break;
				case FIELD_FRAME_SCALEFACTOR:
					sel_frame->scale_factor = v;
					recalculate_world_fields = true;
					break;
				case FIELD_MIN_LIGHT_INT:
					if (v > 100) {
						wxBell();
						f_mli_field->SetValue(wxT("100"));
						v = 100;
					} else if (v < 0) {
						wxBell();
						f_mli_field->SetValue(wxT("0"));
						v = 0;
					}
					sel_frame->minimum_light_intensity = v / 100.0;
					break;
			}
			// recalculate world_* fields if needed and possible
			if (recalculate_world_fields && sel_frame->bitmap_index >= 0
					&& sel_frame->bitmap_index < (int)payload->CollBitmapCount(selected_coll, selected_vers)) {
				ShpBitmap	*assoc_bitmap = payload->GetBitmap(selected_coll, selected_vers, sel_frame->bitmap_index);
				int			w = assoc_bitmap->width,
							h = assoc_bitmap->height,
							scale_factor = sel_frame->scale_factor;

				sel_frame->world_left = -scale_factor * sel_frame->origin_x;
				sel_frame->world_top = scale_factor * sel_frame->origin_y;
				sel_frame->world_right = scale_factor * (w - sel_frame->origin_x);
				sel_frame->world_bottom = -scale_factor * (h - sel_frame->origin_y);
				sel_frame->world_x0 = scale_factor * (sel_frame->key_x - sel_frame->origin_x);
				sel_frame->world_y0 = -scale_factor * (sel_frame->key_y - sel_frame->origin_y);
			}
			f_view->Refresh();
		}
	}
}

// "delete sequence" button click
void ShapesEditor::DeleteSequence(wxCommandEvent &e)
{
	// first delete the sequence for real
	payload->DeleteSequence(selected_coll, selected_vers, selected_sequence);

	// for updating the tree control we could just delete the selected
	// sequence item, but then MyTreeItemData structures associated
	// to the following items would be broken and everything would crash.
	// We could correct them, but reinserting all items is simpler.
	wxTreeItemId	seqnode = GetSequencesTreeItem(selected_coll, selected_vers);

	colltree->SelectItem(seqnode);
	colltree->DeleteChildren(seqnode);
	for (unsigned int k = 0; k < payload->CollSeqCount(selected_coll, selected_vers); k++) {
		ShpSequence		*seq = payload->GetSeq(selected_coll, selected_vers, k);
		wxString		label;
		MyTreeItemData	*seqdata = new MyTreeItemData(selected_coll, selected_vers, TREESECTION_SEQUENCES, k);

		label << k;
		if (strlen(seq->name) > 0)
			label << wxT(" - ") << wxString(seq->name, seqnameconv);
		colltree->AppendItem(seqnode, label, -1, -1, seqdata);
	}

	selected_sequence = -1;
}

// sequence type menu in the sequence editor
void ShapesEditor::EditSequenceType(wxCommandEvent &e)
{
	if (selected_sequence >= 0) {
		ShpSequence	*sel_seq = payload->GetSeq(selected_coll, selected_vers, selected_sequence);
		int			real_nov;

		// always use ANIMATED_4, ANIMATED_5, ANIMATED_8
		// and never other values like ANIMATED_3TO4. Apparently
		// nobody knows the real meaning of these values
		switch (s_type_menu->GetSelection()) {
			case 0:	sel_seq->number_of_views = UNANIMATED;	break;
			case 1:	sel_seq->number_of_views = ANIMATED_1;	break;
			case 2: sel_seq->number_of_views = ANIMATED_4;	break;
			case 3:	sel_seq->number_of_views = ANIMATED_5;	break;
			case 4:	sel_seq->number_of_views = ANIMATED_8;	break;
		}
		real_nov = CalcActualNumberOfViews(sel_seq->number_of_views);
//		s_nov_field->SetValue(INT_TO_WXSTRING(real_nov));
		// init frame indexes to all invalid frames
		// FIXME very annoying for the user, do better
		sel_seq->frame_indexes.clear();
		for (int i = 0; i < real_nov * sel_seq->frames_per_view; i++)
			sel_seq->frame_indexes.push_back(-1);
		s_fb->SetSeqParameters(sel_seq->number_of_views, sel_seq->frames_per_view, &sel_seq->frame_indexes);
	}
}

// transfer mode menu in the sequence editor
void ShapesEditor::EditSequenceXferMode(wxCommandEvent &e)
{
	if (selected_sequence >= 0) {
		ShpSequence	*sel_seq = payload->GetSeq(selected_coll, selected_vers, selected_sequence);

		sel_seq->transfer_mode = s_xfermode_menu->GetSelection();
	}
}

// user messed with fields in the sequence editor
void ShapesEditor::EditSequenceFields(wxCommandEvent &e)
{
	if (selected_sequence >= 0) {
		ShpSequence	*sel_seq = payload->GetSeq(selected_coll, selected_vers, selected_sequence);
		wxString	s = e.GetString();

		if (sel_seq != NULL) {
			if (e.GetId() == FIELD_SEQ_NAME) {
				strncpy(sel_seq->name, s.mb_str(seqnameconv), 32);
				// update the tree item label
				wxTreeItemId		seqnode = GetSequencesTreeItem(selected_coll, selected_vers);
				wxTreeItemIdValue	cookie;
				wxTreeItemId		id = colltree->GetFirstChild(seqnode, cookie);
			
				while (id.IsOk()) {
					MyTreeItemData	*itemdata = dynamic_cast<MyTreeItemData *>(colltree->GetItemData(id));

					if (itemdata->Sequence() == selected_sequence) {
						// here we are
						wxString	blabel;

						blabel << selected_sequence;
						if (strlen(sel_seq->name) > 0)
							blabel << wxT(" - ") << wxString(sel_seq->name, seqnameconv);
						colltree->SetItemText(id, blabel);
						break;
					}
					id = colltree->GetNextChild(seqnode, cookie);
				}
			} else {
				// numeric fields
				long	v;

				if (s.ToLong(&v)) {
					switch (e.GetId()) {
						case FIELD_SEQ_FRAMES_PER_VIEW:
							// must update the SequenceView too
							if (v != sel_seq->frames_per_view) {
								int		real_nov = CalcActualNumberOfViews(sel_seq->number_of_views),
										old_fpv = sel_seq->frames_per_view;
								short	old_indexes[real_nov * old_fpv];

								for (int i = 0; i < real_nov * old_fpv; i++)
									old_indexes[i] = sel_seq->frame_indexes[i];
								sel_seq->frames_per_view = v;
								// try to preserve existing frame
								// references as much as possible
								sel_seq->frame_indexes.clear();
								for (int i = 0; i < real_nov; i++) {
									for (int j = 0; j < v; j++) {
										if (j < old_fpv)
											sel_seq->frame_indexes.push_back(old_indexes[i*old_fpv + j]);
										else
											sel_seq->frame_indexes.push_back(-1);
									}
								}
								s_fb->SetSeqParameters(sel_seq->number_of_views, v, &sel_seq->frame_indexes);
							}
							break;
						case FIELD_SEQ_TICKS_PER_FRAME:
							sel_seq->ticks_per_frame = v;
							break;
						case FIELD_SEQ_LOOP_FRAME:
							sel_seq->loop_frame = v;
							break;
						case FIELD_SEQ_KEY_FRAME:
							sel_seq->key_frame = v;
							break;
						case FIELD_SEQ_XFER_MODE_PERIOD:
							sel_seq->transfer_mode_period = v;
							break;
						case FIELD_SEQ_FIRST_FRAME_SND:
							sel_seq->first_frame_sound = v;
							break;
						case FIELD_SEQ_KEY_FRAME_SND:
							sel_seq->key_frame_sound = v;
							break;
						case FIELD_SEQ_LAST_FRAME_SND:
							sel_seq->last_frame_sound = v;
							break;
						case FIELD_SEQ_SCALE_FACTOR:
							sel_seq->pixels_to_world = v;
							break;
					}
				}
			}
		}
	}
}


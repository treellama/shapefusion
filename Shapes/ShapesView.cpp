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

#include <fstream>
#include "ShapesView.h"
#include "ShapesDocument.h"
#include "../DefaultNames.h"
#include "utilities.h"
#include <wx/wfstream.h>

#define INT_TO_WXSTRING(a)	wxString::Format(wxT("%d"), a)

BEGIN_EVENT_TABLE(ShapesView, wxView)
EVT_MENU(EDIT_MENU_COPY, ShapesView::MenuEditCopy)
EVT_MENU(EDIT_MENU_DELETE, ShapesView::MenuEditDelete)
EVT_MENU(EDIT_MENU_PASTE, ShapesView::MenuEditPaste)
EVT_MENU_RANGE(VIEW_MENU_COLORTABLE_0, VIEW_MENU_COLORTABLE_7, ShapesView::MenuViewCT)
EVT_MENU_RANGE(VIEW_MENU_TNSIZE_SMALL, VIEW_MENU_TNSIZE_AUTO, ShapesView::MenuViewTNSize)
EVT_MENU(VIEW_MENU_TRANSPARENCY, ShapesView::MenuViewTransparency)
EVT_MENU(VIEW_MENU_CENTERORIGIN, ShapesView::MenuViewCenterOrigin)
EVT_MENU(SHAPES_MENU_ADDCOLORTABLE, ShapesView::MenuShapesAddColorTable)
EVT_MENU(SHAPES_MENU_SAVECOLORTABLE, ShapesView::MenuShapesSaveColorTable)
EVT_MENU(SHAPES_MENU_SAVECOLORTABLETOPS, ShapesView::MenuShapesSaveColorTable)
EVT_MENU(SHAPES_MENU_ADDBITMAP, ShapesView::MenuShapesAddBitmap)
EVT_MENU(SHAPES_MENU_EXPORTBITMAP, ShapesView::MenuShapesExportBitmap)
EVT_MENU(SHAPES_MENU_EXPORTMASK, ShapesView::MenuShapesExportBitmapMask)
EVT_MENU(SHAPES_MENU_EXPORTBITMAPS, ShapesView::MenuShapesExportBitmaps)
EVT_MENU(SHAPES_MENU_EXPORTMASKS, ShapesView::MenuShapesExportBitmapMasks)
EVT_MENU(SHAPES_MENU_ADDFRAME, ShapesView::MenuShapesNewFrame)
EVT_MENU(SHAPES_MENU_ADDSEQUENCE, ShapesView::MenuShapesNewSequence)
EVT_MENU(SHAPES_MENU_GENERATEPATCH, ShapesView::MenuShapesGeneratePatch)
EVT_MENU(SHAPES_MENU_IMPORTPATCH, ShapesView::MenuShapesImportPatch)
EVT_TREE_SEL_CHANGED(-1, ShapesView::OnTreeSelect)
// bitmaps
EVT_COMMAND(BITMAP_BROWSER, wxEVT_BITMAPBROWSER, ShapesView::OnBitmapSelect)
EVT_COMMAND(BITMAP_BROWSER, wxEVT_BITMAPBROWSER_DELETE, ShapesView::BitmapDelete)
EVT_COMMAND_RANGE(CB_COLUMN_ORDER, CB_ENABLE_TRANSPARENCY, wxEVT_COMMAND_CHECKBOX_CLICKED, ShapesView::ToggleBitmapCheckboxes)
// color tables
EVT_COMMAND(wxID_ANY, wxEVT_CTBROWSER, ShapesView::OnCTSelect)
EVT_COMMAND(wxID_ANY, wxEVT_CTVIEW_SELECTION, ShapesView::CTColorSelect)
EVT_COMMAND(wxID_ANY, wxEVT_CTVIEW_COLOR, ShapesView::CTColorChanged)
EVT_COMMAND(CB_SELF_LUMINESCENT, wxEVT_COMMAND_CHECKBOX_CLICKED, ShapesView::ToggleSelfLuminCheckbox)
EVT_BUTTON(BTN_GRADIENT, ShapesView::MakeCTGradient)
// frames
EVT_COMMAND(FRAME_BROWSER, wxEVT_FRAMEBROWSER, ShapesView::OnFrameSelect)
EVT_COMMAND(FRAME_BROWSER, wxEVT_FRAMEBROWSER_DELETE, ShapesView::FrameDelete)
EVT_COMMAND(FRAME_VIEW, wxEVT_FRAMEVIEW_DRAG, ShapesView::OnFramePointDrag)
EVT_SPINCTRL(FIELD_BITMAP_INDEX, ShapesView::BitmapIndexSpin)
EVT_COMMAND_RANGE(CB_XMIRROR, CB_KEYPOINT, wxEVT_COMMAND_CHECKBOX_CLICKED, ShapesView::ToggleFrameCheckboxes)
EVT_TEXT(FIELD_ORIGIN_X, ShapesView::EditFrameFields)
EVT_TEXT(FIELD_ORIGIN_Y, ShapesView::EditFrameFields)
EVT_TEXT(FIELD_KEY_X, ShapesView::EditFrameFields)
EVT_TEXT(FIELD_KEY_Y, ShapesView::EditFrameFields)
EVT_TEXT(FIELD_FRAME_SCALEFACTOR, ShapesView::EditFrameFields)
EVT_TEXT(FIELD_MIN_LIGHT_INT, ShapesView::EditFrameFields)
// sequences
EVT_TEXT(FIELD_SEQ_NAME, ShapesView::EditSequenceFields)
EVT_BUTTON(BTN_DELETE_SEQ, ShapesView::DeleteSequence)
EVT_CHOICE(MENU_SEQ_TYPE, ShapesView::EditSequenceType)
EVT_TEXT(FIELD_SEQ_FRAMES_PER_VIEW, ShapesView::EditSequenceFields)
EVT_TEXT(FIELD_SEQ_TICKS_PER_FRAME, ShapesView::EditSequenceFields)
EVT_TEXT(FIELD_SEQ_LOOP_FRAME, ShapesView::EditSequenceFields)
EVT_TEXT(FIELD_SEQ_KEY_FRAME, ShapesView::EditSequenceFields)
EVT_CHOICE(MENU_SEQ_XFER_MODE, ShapesView::EditSequenceXferMode)
EVT_TEXT(FIELD_SEQ_XFER_MODE_PERIOD, ShapesView::EditSequenceFields)
EVT_TEXT(FIELD_SEQ_FIRST_FRAME_SND, ShapesView::EditSequenceFields)
EVT_TEXT(FIELD_SEQ_KEY_FRAME_SND, ShapesView::EditSequenceFields)
EVT_TEXT(FIELD_SEQ_LAST_FRAME_SND, ShapesView::EditSequenceFields)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(ShapesView, wxView)

ShapesView::ShapesView(void):
wxView(), mSelectedColl(-1), mSelectedVers(-1), mSelectedSequence(-1), mViewColorTable(-1)
{
	mFrame = (wxFrame *)NULL;
}

ShapesView::~ShapesView(void)
{
}

bool ShapesView::OnCreate(wxDocument *doc, long WXUNUSED(flags))
{
	wxString	frameTitle = _T("ShapeFusion : Shapes : ");
	
	frameTitle.Append(doc->GetFilename());
    mFrame = wxGetApp().CreateChildFrame(doc, this, frameTitle, wxPoint(0, 0), wxSize(900, 600));
	mFrame->SetSizeHints(200, 200);
	CreateViewMenu(mFrame->GetMenuBar());
	CreateShapesMenu(mFrame->GetMenuBar());
	menubar = mFrame->GetMenuBar();
	
	// Add everything to panel, so background is same as a dialog
	main_panel = new wxPanel(mFrame);
	main_panel->Show();
	
	mainbox = new wxBoxSizer(wxHORIZONTAL);
	// create the collection tree
	colltree = new wxTreeCtrl(main_panel, -1, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT);
	colltree->DeleteAllItems();
	wxTreeItemId	treeroot = colltree->AddRoot(doc->GetFilename());
	mainbox->Add(colltree, 2, wxEXPAND);
	// empty space (e.g. what is displayed when selecting the Sequences node)
	// adding at least a panel is apparently needed to make sizers work
	dummy_sizer = new wxBoxSizer(wxVERTICAL);
	wxPanel	*dummy_panel = new wxPanel(main_panel);
	dummy_sizer->Add(dummy_panel, 1, wxEXPAND);
	mainbox->Add(dummy_sizer, 5, wxEXPAND);
	// collection section
	coll_sizer = new wxBoxSizer(wxVERTICAL);
	coll_text = new wxStaticText(main_panel, -1, wxT("Collection info"));
	coll_static_box = new wxStaticBox(main_panel, -1, wxT("Collection info"));
	coll_inner_box = new wxStaticBoxSizer(coll_static_box, wxVERTICAL);
	coll_inner_box->Add(coll_text, 0, wxALL, 5);
	coll_sizer->AddStretchSpacer();
	coll_sizer->Add(coll_inner_box, 0, wxALIGN_CENTER);
	coll_sizer->AddStretchSpacer();
	mainbox->Add(coll_sizer, 5, wxEXPAND);
	mainbox->Show(coll_sizer, false);
	// chunk section
	chunk_sizer = new wxBoxSizer(wxVERTICAL);
	chunk_static_box = new wxStaticBox(main_panel, -1, wxT("Version info"));
	chunk_static_box->SetThemeEnabled(true);
	chunk_inner_box = new wxStaticBoxSizer(chunk_static_box, wxVERTICAL);
	chunk_undef_label = new wxStaticText(main_panel, -1, wxT("Not defined"));
	chunk_inner_box->Add(chunk_undef_label, 0, wxCENTER | wxALL, 5);
	chunk_grid = new wxFlexGridSizer(2, 4);
	chunk_inner_box->Add(chunk_grid, 0, wxCENTER | wxALL, 5);
	chunk_version_label = new wxStaticText(main_panel, -1, wxT("Version:"));
	chunk_type_label = new wxStaticText(main_panel, -1, wxT("Collection type:"));
	chunk_flags_label = new wxStaticText(main_panel, -1, wxT("Flags:"));
	chunk_sf_label = new wxStaticText(main_panel, -1, wxT("Collection scale factor:"));
	chunk_version_field = new wxTextCtrl(main_panel, -1, wxT("0"));
	wxString	coll_type_labels[] = {	wxT("Unused"),
		wxT("Wall textures"),
		wxT("Objects"),
		wxT("Interface graphics"),
		wxT("Scenery objects") };
	chunk_type_menu = new wxChoice(main_panel, -1, wxDefaultPosition, wxDefaultSize, 5, coll_type_labels, 0);
	chunk_flags_field = new wxTextCtrl(main_panel, -1, wxT("0"));
	chunk_sf_field = new wxTextCtrl(main_panel, -1, wxT("0"));
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
	ctb = new CTBrowser(main_panel);
	ct_count_label = new wxStaticText(main_panel, -1, wxT("N color tables"));
	ct_edit_static_box = new wxStaticBox(main_panel, -1, wxT("Color table N of M"));
	ct_edit_box = new wxStaticBoxSizer(ct_edit_static_box, wxVERTICAL);
	ct_view = new CTView(main_panel);
	ct_inner_edit_box = new wxBoxSizer(wxHORIZONTAL);
	ct_self_lumin_checkbox = new wxCheckBox(main_panel, CB_SELF_LUMINESCENT, wxT("Self-luminescent color"), wxDefaultPosition, wxDefaultSize, wxCHK_3STATE);
	ct_gradient_button = new wxButton(main_panel, BTN_GRADIENT, wxT("Make gradient"));
	ct_edit_box->Add(ct_view, 1, wxEXPAND | wxLEFT | wxTOP | wxRIGHT | wxBOTTOM, 5);
	ct_edit_box->Add(ct_inner_edit_box, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT | wxBOTTOM, 5);
	ct_inner_edit_box->Add(ct_self_lumin_checkbox, 0, wxALIGN_CENTER);
	ct_inner_edit_box->AddStretchSpacer();
	ct_inner_edit_box->Add(ct_gradient_button, 0, wxALIGN_CENTER);
	ct_outer_sizer->Add(ctb, 1, wxGROW);
	ct_outer_sizer->Add(ct_count_label, 1, wxALIGN_LEFT | wxLEFT | wxTOP | wxBOTTOM, 10);
	ct_outer_sizer->Add(ct_edit_box, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);
	ct_outer_sizer->Show(ct_edit_box, false);
	mainbox->Add(ct_outer_sizer, 5, wxEXPAND);
	mainbox->Show(ct_outer_sizer, false);
	// create the bitmaps section
	b_outer_sizer = new wxBoxSizer(wxVERTICAL);
	bb = new BitmapBrowser(main_panel, BITMAP_BROWSER);
	bb->SetThumbnailSize(64);
	b_count_label = new wxStaticText(main_panel, -1, wxT("N bitmaps"));
	b_edit_static_box = new wxStaticBox(main_panel, -1, wxT("Bitmap N of M"));
	b_edit_box = new wxStaticBoxSizer(b_edit_static_box, wxHORIZONTAL);
	b_edit_inner_box = new wxBoxSizer(wxVERTICAL);
	b_info_label = new wxStaticText(main_panel, -1, wxT("AxB pixels"));
	b_order_checkbox = new wxCheckBox(main_panel, CB_COLUMN_ORDER, wxT("Store pixels in column order"));
	b_transparency_checkbox = new wxCheckBox(main_panel, CB_ENABLE_TRANSPARENCY, wxT("Enable transparency"));
	b_view = new BitmapView(main_panel);
	b_edit_inner_box->Add(b_info_label, 0, wxALIGN_LEFT | wxBOTTOM, 10);
	b_edit_inner_box->Add(b_order_checkbox, 0, wxALIGN_LEFT);
	b_edit_inner_box->Add(b_transparency_checkbox, 0, wxALIGN_LEFT);
	b_edit_inner_box->AddStretchSpacer();
	b_edit_box->Add(b_edit_inner_box, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT | wxBOTTOM, 5);
	b_edit_box->Add(b_view, 1, wxEXPAND | wxLEFT | wxTOP | wxRIGHT | wxBOTTOM, 5);
	b_outer_sizer->Add(bb, 1, wxGROW);
	b_outer_sizer->Add(b_count_label, 1, wxALIGN_LEFT | wxLEFT | wxTOP | wxBOTTOM, 10);
	b_outer_sizer->Add(b_edit_box, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);
	b_outer_sizer->Show(b_edit_box, false);
	mainbox->Add(b_outer_sizer, 5, wxEXPAND);
	mainbox->Show(b_outer_sizer, false);
	// create the frames section
	f_outer_sizer = new wxBoxSizer(wxVERTICAL);
	// FIXME the FrameView should be initialized later, but spurious event handlers
	// dereferencing it are triggered during initialization
	f_view = new FrameView(main_panel, FRAME_VIEW);
	fb = new FrameBrowser(main_panel, FRAME_BROWSER);
	fb->SetThumbnailSize(64);
	f_count_label = new wxStaticText(main_panel, -1, wxT("N frames"));
	f_edit_static_box = new wxStaticBox(main_panel, -1, wxT("Frame N"));
	f_edit_box = new wxStaticBoxSizer(f_edit_static_box, wxHORIZONTAL);
	f_bitmap_label = new wxStaticText(main_panel, -1, wxT("Associated bitmap:"));
	f_bitmap_id = new wxSpinCtrl(main_panel, FIELD_BITMAP_INDEX, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_xmirror_checkbox = new wxCheckBox(main_panel, CB_XMIRROR, wxT("X mirror"));
	f_ymirror_checkbox = new wxCheckBox(main_panel, CB_YMIRROR, wxT("Y mirror"));
	f_keypoint_checkbox = new wxCheckBox(main_panel, CB_KEYPOINT, wxT("Keypoint obscured"));
	f_origin_x_label = new wxStaticText(main_panel, -1, wxT("Origin X:"), wxDefaultPosition, wxSize(150, -1));
	f_origin_x_field = new wxTextCtrl(main_panel, FIELD_ORIGIN_X, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_origin_y_label = new wxStaticText(main_panel, -1, wxT("Origin Y:"), wxDefaultPosition, wxSize(150, -1));
	f_origin_y_field = new wxTextCtrl(main_panel, FIELD_ORIGIN_Y, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_key_x_label = new wxStaticText(main_panel, -1, wxT("Keypoint X:"), wxDefaultPosition, wxSize(150, -1));
	f_key_x_field = new wxTextCtrl(main_panel, FIELD_KEY_X, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_key_y_label = new wxStaticText(main_panel, -1, wxT("Keypoint Y:"), wxDefaultPosition, wxSize(150, -1));
	f_key_y_field = new wxTextCtrl(main_panel, FIELD_KEY_Y, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_scalefactor_label = new wxStaticText(main_panel, -1, wxT("Scale factor:"), wxDefaultPosition, wxSize(150, -1));
	f_scalefactor_field = new wxTextCtrl(main_panel, FIELD_FRAME_SCALEFACTOR, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_mli_label = new wxStaticText(main_panel, -1, wxT("Minimum lightness (%):"), wxDefaultPosition, wxSize(150, -1));
	f_mli_field = new wxTextCtrl(main_panel, FIELD_MIN_LIGHT_INT, wxT("0"), wxDefaultPosition, wxSize(60, -1));
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
	f_edit_inner_box = new wxBoxSizer(wxVERTICAL);
	f_edit_inner_box->Add(f_origin_box, 0, wxALIGN_LEFT);
	f_edit_box->Add(f_edit_inner_box, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 5);
	f_edit_box->Add(f_view, 1, wxEXPAND | wxLEFT | wxTOP | wxRIGHT | wxBOTTOM, 5);
	f_outer_sizer->Add(fb, 1, wxGROW);
	f_outer_sizer->Add(f_count_label, 1, wxALIGN_LEFT | wxLEFT | wxTOP | wxBOTTOM, 10);
	f_outer_sizer->Add(f_edit_box, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);
	f_outer_sizer->Show(f_edit_box, false);
	mainbox->Add(f_outer_sizer, 5, wxEXPAND);
	mainbox->Show(f_outer_sizer, false);
	// create the sequences section
	s_outer_static_box = new wxStaticBox(main_panel, -1, wxT("Sequence"));
	s_outer_sizer = new wxStaticBoxSizer(s_outer_static_box, wxVERTICAL);
	
	s_box1 = new wxBoxSizer(wxHORIZONTAL);
	s_outer_sizer->Add(s_box1, 0, wxEXPAND | wxALIGN_TOP | wxALL, 5);
	s_name_label = new wxStaticText(main_panel, -1, wxT("Name:"));
	s_name_field = new wxTextCtrl(main_panel, FIELD_SEQ_NAME, wxT("foobar"));
	s_delete_button = new wxButton(main_panel, BTN_DELETE_SEQ, wxT("Delete sequence"));
	s_box1->Add(s_name_label, 0, wxALIGN_CENTER | wxRIGHT, 5);
	s_box1->Add(s_name_field, 1, wxALIGN_CENTER);
	s_box1->Add(s_delete_button, 0, wxALIGN_CENTER | wxLEFT, 20);
	
	s_box2 = new wxBoxSizer(wxHORIZONTAL);
	s_outer_sizer->Add(s_box2, 0, wxEXPAND | wxALIGN_TOP | wxALL, 5);
	s_grid_box = new wxFlexGridSizer(5, 2, 4, 0);
	s_box2->Add(s_grid_box, 0, wxEXPAND | wxALIGN_TOP);
	s_type_label = new wxStaticText(main_panel, -1, wxT("Sequence type:"), wxDefaultPosition, wxSize(120, -1));
	wxString	anim_type_labels[] = { wxT("Display a random frame"),
		wxT("Animation with 1 view"),
		wxT("Animation with 4 views"),
		wxT("Animation with 5 views"),
		wxT("Animation with 8 views") };
	s_type_menu = new wxChoice(main_panel, MENU_SEQ_TYPE, wxDefaultPosition, wxDefaultSize, 5, anim_type_labels, 0);
	s_fpv_label = new wxStaticText(main_panel, -1, wxT("Frames per view:"), wxDefaultPosition, wxSize(120, -1));
	s_fpv_field = new wxTextCtrl(main_panel, FIELD_SEQ_FRAMES_PER_VIEW, wxT("1"));
	s_tpf_label = new wxStaticText(main_panel, -1, wxT("Ticks per frame:"), wxDefaultPosition, wxSize(120, -1));
	s_tpf_field = new wxTextCtrl(main_panel, FIELD_SEQ_TICKS_PER_FRAME, wxT("1"));
	s_lf_label = new wxStaticText(main_panel, -1, wxT("Loop at frame:"), wxDefaultPosition, wxSize(120, -1));
	s_lf_field = new wxTextCtrl(main_panel, FIELD_SEQ_LOOP_FRAME, wxT("0"));
	s_kf_label = new wxStaticText(main_panel, -1, wxT("Key frame:"), wxDefaultPosition, wxSize(120, -1));
	s_kf_field = new wxTextCtrl(main_panel, FIELD_SEQ_KEY_FRAME, wxT("0"));
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
	
	s_separator = new wxPanel(main_panel);
	s_box2->Add(s_separator, 1, wxEXPAND);
	
	s_grid_box2 = new wxFlexGridSizer(5, 2, 4, 0);
	s_box2->Add(s_grid_box2, 0, wxEXPAND | wxALIGN_TOP);
	s_xfermode_label = new wxStaticText(main_panel, -1, wxT("Transfer mode:"), wxDefaultPosition, wxSize(140, -1));
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
	s_xfermode_menu = new wxChoice(main_panel, MENU_SEQ_XFER_MODE, wxDefaultPosition, wxDefaultSize, 22, xfermode_labels, 0);
	s_xferperiod_label = new wxStaticText(main_panel, -1, wxT("Transfer mode period:"), wxDefaultPosition, wxSize(150, -1));
	s_xferperiod_field = new wxTextCtrl(main_panel, FIELD_SEQ_XFER_MODE_PERIOD, wxT("1"));
	s_ffs_label = new wxStaticText(main_panel, -1, wxT("First frame sound:"), wxDefaultPosition, wxSize(150, -1));
	s_ffs_field = new wxTextCtrl(main_panel, FIELD_SEQ_FIRST_FRAME_SND, wxT("0"));
	s_kfs_label = new wxStaticText(main_panel, -1, wxT("Key frame sound:"), wxDefaultPosition, wxSize(150, -1));
	s_kfs_field = new wxTextCtrl(main_panel, FIELD_SEQ_KEY_FRAME_SND, wxT("0"));
	s_lfs_label = new wxStaticText(main_panel, -1, wxT("Last frame sound:"), wxDefaultPosition, wxSize(150, -1));
	s_lfs_field = new wxTextCtrl(main_panel, FIELD_SEQ_LAST_FRAME_SND, wxT("0"));
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
	
	s_fb = new SequenceView(main_panel, wxID_ANY);
	s_outer_sizer->Add(s_fb, 1, wxEXPAND | wxALL, 5);
	mainbox->Add(s_outer_sizer, 5, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);
	mainbox->Show(s_outer_sizer, false);
	
	mainbox->Layout();
	main_panel->SetSizer(mainbox);
#ifdef __X__
    // X seems to require a forced resize
    int x, y;
	
    mFrame->GetSize(&x, &y);
    mFrame->SetSize(wxDefaultCoord, wxDefaultCoord, x, y);
#endif	
	mFrame->Show(true);
    Activate(true);
    
    return true;
}

void ShapesView::OnDraw(wxDC *WXUNUSED(dc))
{
}

void ShapesView::OnUpdate(wxView *WXUNUSED(sender), wxObject *WXUNUSED(hint))
{
	unsigned int	collCount = ((ShapesDocument*)GetDocument())->CollectionCount();
	
	if (collCount == 0)
		return;
	
	// update all levels of the tree control
	for (unsigned int i = 0; i < collCount; i++) {
		// collection name nodes
		ShapesTreeItemData	*itemdata = new ShapesTreeItemData(i, -1, TREESECTION_COLLECTION);
		wxTreeItemId		coll = colltree->AppendItem(colltree->GetRootItem(), GetName(wxT("collection"), i),
														-1, -1, itemdata);
		
		for (unsigned int j = 0; j < 2; j++) {
			// color version nodes
			ShapesTreeItemData	*id = new ShapesTreeItemData(i, j, TREESECTION_VERSION);
			wxString			label;
			
			if (j == COLL_VERSION_8BIT)
				label = wxT("8-bit color version");
			else if (j == COLL_VERSION_TRUECOLOR)
				label = wxT("True color version");
			wxTreeItemId	coll2 = colltree->AppendItem(coll, label, -1, -1, id);
			
			if (((ShapesDocument*)GetDocument())->CollectionDefined(i, j)) {
				// section nodes
				ShapesTreeItemData	*id_b = new ShapesTreeItemData(i, j, TREESECTION_BITMAPS),
				*id_ct = new ShapesTreeItemData(i, j, TREESECTION_COLORTABLES),
				*id_f = new ShapesTreeItemData(i, j, TREESECTION_FRAMES),
				*id_s = new ShapesTreeItemData(i, j, TREESECTION_SEQUENCES);
				wxTreeItemId		coll_b = colltree->AppendItem(coll2, wxT("Bitmaps"), -1, -1, id_b),
				coll_ct = colltree->AppendItem(coll2, wxT("Color tables"), -1, -1, id_ct),
				coll_f = colltree->AppendItem(coll2, wxT("Frames"), -1, -1, id_f),
				coll_s = colltree->AppendItem(coll2, wxT("Sequences"), -1, -1, id_s);
				
				for (unsigned int k = 0; k < ((ShapesDocument*)GetDocument())->CollectionSequenceCount(i, j); k++) {
					// sequence nodes
					ShapesSequence		*seq = ((ShapesDocument*)GetDocument())->GetSequence(i, j, k);
					wxString    		blabel;
					ShapesTreeItemData	*id_seq = new ShapesTreeItemData(i, j, TREESECTION_SEQUENCES, k);
					
					blabel << k;
					if (seq->Name().Length() > 0)
						blabel << wxT(" - ") << seq->Name();
					colltree->AppendItem(coll_s, blabel, -1, -1, id_seq);
				}
			}
		}
	}
}

bool ShapesView::OnClose(bool deleteWindow)
{
	if (!GetDocument()->Close())
		return false;
	Activate(false);
	if (deleteWindow) {
		delete mFrame;
		return true;
	}
	return true;
}

// return the id of the "Sequences" tree item relative to
// the specified collection and color version
wxTreeItemId ShapesView::GetSequencesTreeItem(unsigned int collection, unsigned int version) const
{
	wxTreeItemIdValue	thecookie;
	wxTreeItemId		rootid = colltree->GetRootItem(),
	collnameid = colltree->GetFirstChild(rootid, thecookie);
	
	// descend into the tree towards the right item
	while (collnameid.IsOk()) {
		ShapesTreeItemData	*collname_data = dynamic_cast<ShapesTreeItemData *>(colltree->GetItemData(collnameid));
		
		if (collname_data->CollID() == (int)collection) {
			// found right collection branch
			wxTreeItemIdValue	thecookieII;
			wxTreeItemId		collversid = colltree->GetFirstChild(collnameid, thecookieII);
			
			while (collversid.IsOk()) {
				ShapesTreeItemData	*collvers_data = dynamic_cast<ShapesTreeItemData *>(colltree->GetItemData(collversid));
				
				if (collvers_data->Version() == (int)version) {
					// found right version node
					wxTreeItemIdValue	thecookieIII;
					wxTreeItemId		sectid = colltree->GetFirstChild(collversid, thecookieIII);
					
					while (sectid.IsOk()) {
						ShapesTreeItemData	*sect_data = dynamic_cast<ShapesTreeItemData *>(colltree->GetItemData(sectid));
						
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

void ShapesView::MenuEditCopy(wxCommandEvent& e)
{
	ShapesTreeItemData* selected_item_data = dynamic_cast<ShapesTreeItemData*>(colltree->GetItemData(colltree->GetSelection()));
	
	if (selected_item_data != NULL) {
		switch (selected_item_data->Section()) {
			case TREESECTION_BITMAPS:
				DoCopyBitmap(bb->GetSelection());
				break;
			case TREESECTION_VERSION:
				DoCopyChunk(selected_item_data->CollID(), selected_item_data->Version());
				break;
			default:
				break;
		}
	}
}

// handle the Edit->Delete menu command, which is context-sensitive
void ShapesView::MenuEditDelete(wxCommandEvent &e)
{
	ShapesTreeItemData	*selected_item_data = dynamic_cast<ShapesTreeItemData *>(colltree->GetItemData(colltree->GetSelection()));
	
	if (selected_item_data != NULL) {
		// what should we delete?
		switch (selected_item_data->Section()) {
			case TREESECTION_COLORTABLES:
				DoDeleteColorTable(ctb->GetSelection());
				break;
			case TREESECTION_BITMAPS:
				DoDeleteBitmap(bb->GetSelection());
				break;
			case TREESECTION_FRAMES:
				DoDeleteFrame(fb->GetSelection());
				break;
			case TREESECTION_SEQUENCES:
				break;
		}
	}
}

void ShapesView::MenuEditPaste(wxCommandEvent& e)
{
	ShapesTreeItemData* selected_item_data = dynamic_cast<ShapesTreeItemData*>(colltree->GetItemData(colltree->GetSelection()));
	
	if (selected_item_data != NULL) {
		switch (selected_item_data->Section()) {
			case TREESECTION_BITMAPS:
				DoPasteBitmap(bb->GetSelection());
				break;
			case TREESECTION_VERSION:
				DoPasteChunk(selected_item_data->CollID(), selected_item_data->Version());
				break;
			default:
				break;
		}
	}
}

void ShapesView::MenuViewTransparency(wxCommandEvent &e)
{
	bool	showTranspPixels = e.IsChecked();
	
	wxBeginBusyCursor();
	bb->SetTranspPixelsDisplay(!showTranspPixels);
	b_view->SetTranspPixelsDisplay(!showTranspPixels);
	fb->SetTranspPixelsDisplay(!showTranspPixels);
	f_view->SetTranspPixelsDisplay(!showTranspPixels);
	s_fb->SetTranspPixelsDisplay(!showTranspPixels);
	wxEndBusyCursor();
}

void ShapesView::MenuViewCenterOrigin(wxCommandEvent &e)
{
	f_view->SetCenterOrigin(e.IsChecked());
}

// color table menu handler
void ShapesView::MenuViewCT(wxCommandEvent &e)
{
	mViewColorTable = e.GetId() - VIEW_MENU_COLORTABLE_0;
	
	ShapesColorTable	*ctp = ((ShapesDocument*)GetDocument())->GetColorTable(
																			   mSelectedColl, mSelectedVers, mViewColorTable);
	
	wxBeginBusyCursor();
	bb->SetColorTable(ctp);
	b_view->SetColorTable(ctp);
	fb->SetColorTable(ctp);
	f_view->SetColorTable(ctp);
	s_fb->SetColorTable(ctp);
	wxEndBusyCursor();
}

// thumbnail size menu handler
void ShapesView::MenuViewTNSize(wxCommandEvent &e)
{
	int	size = -1;
	
	switch (e.GetId()) {
		case VIEW_MENU_TNSIZE_SMALL:
			size = 32;
			break;
		case VIEW_MENU_TNSIZE_MEDIUM:
			size = 64;
			break;
		case VIEW_MENU_TNSIZE_LARGE:
			size = 128;
			break;
		case VIEW_MENU_TNSIZE_AUTO:
			size = -1;
			break;
	}
	wxBeginBusyCursor();
	bb->SetThumbnailSize(size);
	fb->SetThumbnailSize(size);
	s_fb->SetThumbnailSize(size);
	wxEndBusyCursor();
}

void ShapesView::MenuShapesAddColorTable(wxCommandEvent &e)
{
	wxFileDialog	*dlg = new wxFileDialog(mFrame, wxT("Import a color table"), wxT(""), wxT(""),
											wxT("Photoshop color table|*.act|Gimp palette|*.gpl"), wxOPEN);
	
	if (dlg->ShowModal() == wxID_OK) {
		wxString		filename = dlg->GetPath();
		std::ifstream	ifs(filename.mb_str(), std::ios::binary);
		
		if (ifs.good()) {
			ShapesColorTable	*newct = new ShapesColorTable(ifs, filename.AfterLast('.')),
			*firstct = ((ShapesDocument*)GetDocument())->GetColorTable(mSelectedColl, mSelectedVers, 0);
			
			ifs.close();
			if (newct->ColorCount() > 0) {
				if (newct->GetColor(0)->Red() != 0 || newct->GetColor(0)->Green() != 0 || newct->GetColor(0)->Blue() != 255<<8)
					wxMessageBox(wxT("The first color of the table being imported is not the usual Marathon chroma key color"
									 " (no red, no green, maximum blue). It should be corrected manually to avoid problems."),
								 wxT("Invalid chroma key color"), wxOK | wxICON_WARNING, mFrame);
				if (firstct != NULL) {
					// handle cases in which the new color table has more or less colors than existing ones
					if (newct->ColorCount() > firstct->ColorCount()) {
						// more colors, append dummy colors to existing tables
						unsigned int	numcolors = newct->ColorCount() - firstct->ColorCount(),
						numcts = ((ShapesDocument*)GetDocument())->CollectionColorTableCount(mSelectedColl, mSelectedVers);
						
						for (unsigned int i = 0; i < numcts; i++) {
							ShapesColorTable	*ct = ((ShapesDocument*)GetDocument())->GetColorTable(mSelectedColl, mSelectedVers, i);
							
							for (unsigned int j = 0; j < numcolors; j++)
								ct->InsertColor(new ShapesColor(0, 0, 0, numcolors + j));
						}
					} else if (newct->ColorCount() < firstct->ColorCount()) {
						// less colors, append dummy colors to the new table
						unsigned int	numcolors = firstct->ColorCount() - newct->ColorCount();
						
						for (unsigned int i = 0; i < numcolors; i++)
							newct->InsertColor(new ShapesColor(0, 0, 0, newct->ColorCount() + i));
					}
				}
				((ShapesDocument*)GetDocument())->InsertColorTable(newct, mSelectedColl, mSelectedVers);
				((ShapesDocument*)GetDocument())->Modify(true);
				// update the GUI
				ctb->AddColorTable(newct);
				unsigned int	colorTableCount = ((ShapesDocument*)GetDocument())->CollectionColorTableCount(
																											  mSelectedColl, mSelectedVers);
				wxMenu 			*colortables_submenu;
				menubar->FindItem(VIEW_MENU_COLORTABLE_0, &colortables_submenu);
				for (unsigned int i = 0; i < colortables_submenu->GetMenuItemCount(); i++) {
					menubar->Enable(VIEW_MENU_COLORTABLE_0 + i, i < colorTableCount);
					menubar->Check(VIEW_MENU_COLORTABLE_0 + mViewColorTable, i == (unsigned int)mViewColorTable);
				}
			} else {
				wxString	errormsg;
				
				errormsg << wxT("Sorry, could not load a color table from ") << filename << wxT(" because the file format is unknown or the file contains no colors.");
				wxMessageBox(errormsg, wxT("Error loading color table"), wxOK | wxICON_ERROR, mFrame);
				delete newct;
			}
		} else {
			wxString	errormsg;
			
			errormsg << wxT("Sorry, could not load a color table from ") << filename << wxT(" because the file is not readable.");
			wxMessageBox(errormsg, wxT("Error loading color table"), wxOK | wxICON_ERROR, mFrame);
		}
	}
	dlg->Destroy();
}

// prompt the user and save the selected color table, either in
// Gimp or PhotoShop format (depending on event id)
void ShapesView::MenuShapesSaveColorTable(wxCommandEvent &e)
{
	if (((ShapesDocument*)GetDocument()) != NULL && mSelectedColl != -1 && mSelectedVers != -1) {
		int	selection = ctb->GetSelection();
		
		if (selection >= 0) {
			bool		ps = (e.GetId() == SHAPES_MENU_SAVECOLORTABLETOPS);
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
				ShapesColorTable	*ct = ((ShapesDocument*)GetDocument())->GetColorTable(mSelectedColl, mSelectedVers, selection);
				int					err = 0;
				
				if (ps)
					err = ct->SaveToPhotoshop(ctpath);
				else
					err = ct->SaveToGimp(ctpath);
				
				if (err != 0) {
					wxString	errormsg;
					
					errormsg << wxT("Sorry, could not save color table to ") << ctpath << wxT(".");
					wxMessageBox(errormsg, wxT("Error saving color table"), wxOK | wxICON_ERROR, mFrame);
				}
			}
		}
	}
}

void ShapesView::MenuShapesAddBitmap(wxCommandEvent &e)
{
	if (((ShapesDocument*)GetDocument()) != NULL && mSelectedColl != -1 && mSelectedVers != -1) {
		wxFileDialog	*dlg = new wxFileDialog(mFrame, wxT("Choose a bitmap to add"), wxT(""), wxT(""),
												wxT("Common bitmap files (BMP, JPEG, PNG, GIF, TIFF)|*.bmp;*.jpg;*.jpeg;*.tif;*.tiff;*.png;*.gif"),
												wxOPEN);
		
		if (dlg->ShowModal() == wxID_OK) {
			wxString	filename = dlg->GetPath();
			wxImage		img;
			
			if (img.LoadFile(filename)) {
				wxBeginBusyCursor();
				// we have the wxImage now. Encode it to a new bitmap
				ShapesColorTable	*ct = ((ShapesDocument*)GetDocument())->GetColorTable(mSelectedColl, mSelectedVers, mViewColorTable);
				ShapesBitmap		*newbmp = new ShapesBitmap(img, ct);
				
				// automagically initialize bitmap flags
				if (((ShapesDocument*)GetDocument())->CollectionType(mSelectedColl, mSelectedVers) == _object_collection ||
					((ShapesDocument*)GetDocument())->CollectionType(mSelectedColl, mSelectedVers) == _scenery_collection) {
					// compress weapons, monsters and scenery
					newbmp->SetBytesPerRow(-1);
				} else if (((ShapesDocument*)GetDocument())->CollectionType(mSelectedColl, mSelectedVers) == _interface_collection) {
					// interface elements are row-ordered (not so important with A1 actually)
					newbmp->SetColumnOrdered(false);
				}
				((ShapesDocument*)GetDocument())->InsertBitmap(newbmp, mSelectedColl, mSelectedVers);
				
				// update the GUI
				unsigned int	bitmap_count = ((ShapesDocument*)GetDocument())->CollectionBitmapCount(mSelectedColl, mSelectedVers);
				ShapesBitmap	*pnewbitmap = ((ShapesDocument*)GetDocument())->GetBitmap(mSelectedColl, mSelectedVers, bitmap_count-1);
				wxString		count_string = wxString::Format(wxT("%u bitmap"), bitmap_count);
				
				bb->AddBitmap(pnewbitmap);
				fb->AddBitmap(pnewbitmap);
				if (bitmap_count != 1)
					count_string << wxT("s");
				b_count_label->SetLabel(count_string);
				
				wxEndBusyCursor();
				((ShapesDocument*)GetDocument())->Modify(true);
			} else {
				wxString	errormsg;
				
				errormsg << wxT("Sorry, could not load bitmap from ") << filename << wxT(".");
				wxMessageBox(errormsg, wxT("Error adding bitmap"), wxOK | wxICON_ERROR, mFrame);
			}
		}
		dlg->Destroy();
	}
}

// export selected bitmap to a BMP file
void ShapesView::MenuShapesExportBitmap(wxCommandEvent &e)
{
	if (((ShapesDocument*)GetDocument()) != NULL && mSelectedColl != -1 && mSelectedVers != -1) {
		int selection = bb->GetSelection();
		
		if (selection >= 0) {
			wxString	prompt = wxString::Format(wxT("Export bitmap %d"), selection),
			name = wxString::Format(wxT("bitmap%.3d.bmp"), selection),
			path = wxFileSelector(prompt, wxT(""), name, wxT(""), wxT("BMP image|*.bmp"), wxSAVE | wxOVERWRITE_PROMPT);
			
			if (!path.empty()) {
				ShapesBitmap		*bitmap = ((ShapesDocument*)GetDocument())->GetBitmap(mSelectedColl, mSelectedVers, selection);
				ShapesColorTable	*colorTable = ((ShapesDocument*)GetDocument())->GetColorTable(mSelectedColl, mSelectedVers, mViewColorTable);
				
				bitmap->SaveToBMP(path, colorTable);
			}
		}
	}
}

void ShapesView::MenuShapesExportBitmapMask(wxCommandEvent &e)
{
	if (((ShapesDocument*)GetDocument()) != NULL && mSelectedColl != -1 && mSelectedVers != -1) {
		int selection = bb->GetSelection();
		
		if (selection >= 0) {
			wxString	prompt = wxString::Format(wxT("Export bitmap %d mask"), selection),
			name = wxString::Format(wxT("bitmap%.3dmask.bmp"), selection),
			path = wxFileSelector(prompt, wxT(""), name, wxT(""), wxT("BMP image|*.bmp"), wxSAVE | wxOVERWRITE_PROMPT);
			
			if (!path.empty()) {
				ShapesBitmap	*bitmap = ((ShapesDocument*)GetDocument())->GetBitmap(mSelectedColl, mSelectedVers, selection);
				
				bitmap->SaveMaskToBMP(path);
			}
		}
	}
}

// export all bitmaps of the selected collection to separate BMP files in a folder
void ShapesView::MenuShapesExportBitmaps(wxCommandEvent &e)
{
	if (((ShapesDocument*)GetDocument()) != NULL && mSelectedColl != -1 && mSelectedVers != -1) {
		wxString	dirPath = wxDirSelector(wxT("Select destination folder"));
		
		if (!dirPath.empty()) {
			ShapesColorTable	*colorTable = ((ShapesDocument*)GetDocument())->GetColorTable(mSelectedColl, mSelectedVers, mViewColorTable);
			
			wxBeginBusyCursor();
			for (unsigned int i = 0; i < ((ShapesDocument*)GetDocument())->CollectionBitmapCount(mSelectedColl, mSelectedVers); i++) {
				wxString		name = wxString::Format(wxT("bitmap%.3d.bmp"), i);
				ShapesBitmap	*bitmap = ((ShapesDocument*)GetDocument())->GetBitmap(mSelectedColl, mSelectedVers, i);
				
				bitmap->SaveToBMP(dirPath + wxT("/") + name, colorTable);
			}
			wxEndBusyCursor();
		}
	}
}

void ShapesView::MenuShapesExportBitmapMasks(wxCommandEvent &e)
{
	if (((ShapesDocument*)GetDocument()) != NULL && mSelectedColl != -1 && mSelectedVers != -1) {
		wxString	dirPath = wxDirSelector(wxT("Select destination folder"));
		
		if (!dirPath.empty()) {
			wxBeginBusyCursor();
			for (unsigned int i = 0; i < ((ShapesDocument*)GetDocument())->CollectionBitmapCount(mSelectedColl, mSelectedVers); i++) {
				wxString		name = wxString::Format(wxT("bitmap%.3dmask.bmp"), i);
				ShapesBitmap	*bitmap = ((ShapesDocument*)GetDocument())->GetBitmap(mSelectedColl, mSelectedVers, i);
				
				bitmap->SaveMaskToBMP(dirPath + wxT("/") + name);
			}
			wxEndBusyCursor();
		}
	}
}

void ShapesView::MenuShapesNewFrame(wxCommandEvent &e)
{
	if (((ShapesDocument*)GetDocument()) != NULL && mSelectedColl != -1 && mSelectedVers != -1) {
		// append an empty frame
		ShapesFrame	*newframe = new ShapesFrame();
		
		newframe->SetScaleFactor(((ShapesDocument*)GetDocument())->CollectionScaleFactor(mSelectedColl, mSelectedVers));
		if (((ShapesDocument*)GetDocument())->CollectionBitmapCount(mSelectedColl, mSelectedVers) > ((ShapesDocument*)GetDocument())->CollectionFrameCount(mSelectedColl, mSelectedVers)) {
			newframe->SetBitmapIndex(((ShapesDocument*)GetDocument())->CollectionFrameCount(mSelectedColl, mSelectedVers));
		}
		((ShapesDocument*)GetDocument())->InsertFrame(newframe, mSelectedColl, mSelectedVers);
		fb->AddFrame(((ShapesDocument*)GetDocument())->GetFrame(mSelectedColl, mSelectedVers, ((ShapesDocument*)GetDocument())->CollectionFrameCount(mSelectedColl, mSelectedVers)-1));
		// update frame count label
		unsigned int	frame_count = ((ShapesDocument*)GetDocument())->CollectionFrameCount(mSelectedColl, mSelectedVers);
		wxString		count_string = wxString::Format(wxT("%u frame"), frame_count);
		
		if (frame_count != 1)
			count_string << wxT("s");
		f_count_label->SetLabel(count_string);
		((ShapesDocument*)GetDocument())->Modify(true);
	}
}

void ShapesView::MenuShapesNewSequence(wxCommandEvent &e)
{
	if (((ShapesDocument*)GetDocument()) != NULL && mSelectedColl != -1 && mSelectedVers != -1) {
		// append an empty sequence
		ShapesSequence		*newseq = new ShapesSequence();
		
		((ShapesDocument*)GetDocument())->InsertSequence(newseq, mSelectedColl, mSelectedVers);
		
		// insert the new entry in the main tree
		wxTreeItemId	thenode = GetSequencesTreeItem(mSelectedColl, mSelectedVers);
		int				seq_id = ((ShapesDocument*)GetDocument())->CollectionSequenceCount(mSelectedColl, mSelectedVers) - 1;
		ShapesSequence     *seq = ((ShapesDocument*)GetDocument())->GetSequence(mSelectedColl, mSelectedVers, seq_id);
		ShapesTreeItemData  *itemdata = new ShapesTreeItemData(mSelectedColl, mSelectedVers, TREESECTION_SEQUENCES, seq_id);
		wxString		label;
		
		label << seq_id;
		if (seq->Name().Length() > 0)
			label << wxT(" - ") << seq->Name();
		colltree->AppendItem(thenode, label, -1, -1, itemdata);
		((ShapesDocument*)GetDocument())->Modify(true);
	}
}

void ShapesView::MenuShapesGeneratePatch(wxCommandEvent&)
{
	ShapesDocument* document = (ShapesDocument*) GetDocument();
	
	if (document == NULL) {
		return;
	}
	
	// prompt the user for a base for the patch
	wxFileDialog dlg(mFrame, wxT("Choose a base file (e.g. standard Infinity shapes)"), wxT(""), wxT(""), wxT("Shapes files (*.shpA)|*.shpA|All files (*.*)|*.*"), wxOPEN);
	if (dlg.ShowModal() != wxID_OK) {
		return;
	}
	
	ShapesDocument base;
	if (!base.DoOpenDocument(dlg.GetPath())) {
		return;
	}
	
	// prompt the user for a patch location
	wxString path = wxFileSelector(wxT("Export patch file"), wxT(""), wxT("Shapes Patch.ShPa"), wxT(""), wxT("Shapes patch|*.ShPa"), wxSAVE | wxOVERWRITE_PROMPT);
	
	if (path.empty()) {
		return;
	}
	
#if wxUSE_STD_IOSTREAM
	wxSTD ofstream stream(path.mb_str(), wxSTD ios_base::out | wxSTD ios_base::binary | wxSTD ios_base::trunc);
#else
	wxFileOutputStream stream(path);
	if (!stream.IsOk()) {
		return;
	}
#endif
	document->SavePatch(stream, base);
}

void ShapesView::MenuShapesImportPatch(wxCommandEvent&)
{
	ShapesDocument* document = (ShapesDocument*) GetDocument();

	if (document == NULL) {
		return;
	}

	// prompt the user for a patch
	wxString  path = wxFileSelector(wxT("Choose a patch file"), wxT(""), wxT(""), wxT(""), wxT("Patch files (*.ShPa)|*.ShPa|All files (*.*)|*.*"), wxOPEN);

	if (path.empty()) {
		return;
	}

#if wxUSE_STD_IOSTREAM
	wxSTD ifstream stream(path.mb_str(), wxSTD ios_base::in | wxSTD ios_base::binary);
#else
	wxFileInputStream stream(path);
	if (!stream.IsOk()) {
		return;
	}
#endif

	if (!document->LoadPatch(stream)) {
		wxMessageBox(wxT("Error loading shapes patch; the patch may be partially applied!"), wxT("Error loading shapes patch"), wxOK | wxICON_ERROR, mFrame);
	}

	colltree->Unselect();
	colltree->CollapseAll();
	mSelectedColl = -1;
	mSelectedVers = -1;
	mSelectedSequence = -1;
	mViewColorTable = -1;
	mainbox->Show(dummy_sizer, true);
	mainbox->Show(coll_sizer, false);
	mainbox->Show(chunk_sizer, false);
	mainbox->Show(b_outer_sizer, false);
	mainbox->Show(ct_outer_sizer, false);
	mainbox->Show(f_outer_sizer, false);
	mainbox->Show(s_outer_sizer, false);
}

// user selected a tree entry
void ShapesView::OnTreeSelect(wxTreeEvent &e)
{
	ShapesTreeItemData	*data = dynamic_cast<ShapesTreeItemData *>(colltree->GetItemData(e.GetItem()));
	
	if (data) {
		int	new_coll = data->CollID(),
		new_vers = data->Version(),
		new_section = data->Section();
		
		if (new_coll != mSelectedColl || new_vers != mSelectedVers) {
			// user has moved to another collection/version
			
			// first clear the user interface
			wxMenu			*colortables_submenu;
			menubar->FindItem(VIEW_MENU_COLORTABLE_0, &colortables_submenu);
			unsigned int	ctmenucount = colortables_submenu->GetMenuItemCount();
			
			mViewColorTable = -1;
			ctb->Clear();
			ct_view->SetColorTable(NULL);
			bb->Freeze();
			bb->Clear();
			b_view->SetBitmap(NULL);
			fb->Freeze();
			fb->Clear();
			f_view->SetFrame(NULL);
			f_view->SetBitmap(NULL);
			s_fb->Clear();
			for (unsigned int i = 0; i < ctmenucount; i++)
				menubar->Enable(VIEW_MENU_COLORTABLE_0 + i, false);
			menubar->Enable(SHAPES_MENU_SAVECOLORTABLE, false);
			menubar->Enable(SHAPES_MENU_SAVECOLORTABLETOPS, false);
			menubar->Enable(SHAPES_MENU_EXPORTBITMAP, false);
			menubar->Enable(SHAPES_MENU_EXPORTMASK, false);
			
			// set collection info panel
			coll_static_box->SetLabel(wxString::Format(wxT("Global info for collection %d"), new_coll));
			
			wxString	collinfo_s;
			collinfo_s << wxT("Status: ") << ((ShapesDocument*)GetDocument())->CollectionStatus(new_coll) << wxT("\n");
			collinfo_s << wxT("Flags: ") << ((ShapesDocument*)GetDocument())->CollectionFlags(new_coll) << wxT("\n\n");
			if (((ShapesDocument*)GetDocument())->CollectionDefined(new_coll, COLL_VERSION_8BIT))
				collinfo_s << wxT("8-bit color version present\n");
			else
				collinfo_s << wxT("No 8-bit color version\n");
			if (((ShapesDocument*)GetDocument())->CollectionDefined(new_coll, COLL_VERSION_TRUECOLOR))
				collinfo_s << wxT("True color version present");
			else
				collinfo_s << wxT("No true color version");
			coll_text->SetLabel(collinfo_s);
			
			if (new_coll != -1 && new_vers != -1) {
				if (new_vers == COLL_VERSION_TRUECOLOR)
					chunk_static_box->SetLabel(wxT("Shapes for true color and OpenGL display"));
				else
					chunk_static_box->SetLabel(wxT("Shapes for 8-bit color display"));
				
				if (((ShapesDocument*)GetDocument())->CollectionDefined(new_coll, new_vers)) {
					// a defined collection has been selected
					unsigned int	ct_count = ((ShapesDocument*)GetDocument())->CollectionColorTableCount(new_coll, new_vers),
					bitmap_count = ((ShapesDocument*)GetDocument())->CollectionBitmapCount(new_coll, new_vers),
					frame_count = ((ShapesDocument*)GetDocument())->CollectionFrameCount(new_coll, new_vers);
					wxString		count_string;
					
					wxBeginBusyCursor();
					menubar->Enable(SHAPES_MENU_ADDCOLORTABLE, true);
					menubar->Enable(SHAPES_MENU_ADDBITMAP, true);
					menubar->Enable(SHAPES_MENU_EXPORTBITMAPS, true);
					menubar->Enable(SHAPES_MENU_EXPORTMASKS, true);
					menubar->Enable(SHAPES_MENU_ADDFRAME, true);
					menubar->Enable(SHAPES_MENU_ADDSEQUENCE, true);
					
					// chunk info panel
					chunk_version_field->ChangeValue(INT_TO_WXSTRING(((ShapesDocument*)GetDocument())->CollectionVersion(new_coll, new_vers)));
					chunk_type_menu->SetSelection(((ShapesDocument*)GetDocument())->CollectionType(new_coll, new_vers));
					chunk_flags_field->ChangeValue(INT_TO_WXSTRING(((ShapesDocument*)GetDocument())->CollectionFlags(new_coll, new_vers)));
					chunk_sf_field->ChangeValue(INT_TO_WXSTRING(((ShapesDocument*)GetDocument())->CollectionScaleFactor(new_coll, new_vers)));
					
					// color tables
					mViewColorTable = 0;
					for (unsigned int i = 0; i < ct_count; i++)
						ctb->AddColorTable(((ShapesDocument*)GetDocument())->GetColorTable(new_coll, new_vers, i));
					for (unsigned int i = 0; i < ctmenucount; i++)
						menubar->Enable(VIEW_MENU_COLORTABLE_0 + i, i < ct_count);
					if (ct_count > 0)
						menubar->Check(VIEW_MENU_COLORTABLE_0, true);
					count_string << ct_count << wxT(" color table");
					if (ct_count != 1)
						count_string << wxT("s");
					if (ct_count > 0)
						count_string << wxT(", ") << ((ShapesDocument*)GetDocument())->GetColorTable(new_coll, new_vers, 0)->ColorCount() << wxT(" colors per table");
					ct_count_label->SetLabel(count_string);
					
					// bitmaps
					bb->SetColorTable(((ShapesDocument*)GetDocument())->GetColorTable(new_coll, new_vers, 0));
					b_view->SetColorTable(((ShapesDocument*)GetDocument())->GetColorTable(new_coll, new_vers, 0));
					for (unsigned int i = 0; i < bitmap_count; i++)
						bb->AddBitmap(((ShapesDocument*)GetDocument())->GetBitmap(new_coll, new_vers, i));
					count_string.Clear();
					count_string << bitmap_count << wxT(" bitmap");
					if (bitmap_count != 1)
						count_string << wxT("s");
					b_count_label->SetLabel(count_string);
					
					// frames
					fb->SetColorTable(((ShapesDocument*)GetDocument())->GetColorTable(new_coll, new_vers, 0));
					f_view->SetColorTable(((ShapesDocument*)GetDocument())->GetColorTable(new_coll, new_vers, 0));
					for (unsigned int i = 0; i < bitmap_count; i++)
						fb->AddBitmap(((ShapesDocument*)GetDocument())->GetBitmap(new_coll, new_vers, i));
					for (unsigned int i = 0; i < frame_count; i++)
						fb->AddFrame(((ShapesDocument*)GetDocument())->GetFrame(new_coll, new_vers, i));
					count_string.Clear();
					count_string << frame_count << wxT(" frame");
					if (frame_count != 1)
						count_string << wxT("s");
					f_count_label->SetLabel(count_string);
					
					wxEndBusyCursor();
				} else {
					menubar->Enable(SHAPES_MENU_ADDCOLORTABLE, false);
					menubar->Enable(SHAPES_MENU_ADDBITMAP, false);
					menubar->Enable(SHAPES_MENU_EXPORTBITMAPS, false);
					menubar->Enable(SHAPES_MENU_EXPORTMASKS, false);
					menubar->Enable(SHAPES_MENU_ADDFRAME, false);
					menubar->Enable(SHAPES_MENU_ADDSEQUENCE, false);
				}
			} else {
				menubar->Enable(SHAPES_MENU_ADDCOLORTABLE, false);
				menubar->Enable(SHAPES_MENU_ADDBITMAP, false);
				menubar->Enable(SHAPES_MENU_EXPORTBITMAPS, false);
				menubar->Enable(SHAPES_MENU_EXPORTMASKS, false);
				menubar->Enable(SHAPES_MENU_ADDFRAME, false);
				menubar->Enable(SHAPES_MENU_ADDSEQUENCE, false);
			}
			mSelectedColl = new_coll;
			mSelectedVers = new_vers;
			bb->Thaw();
			fb->Thaw();
		}
		
		// handle sequence selection
		mSelectedSequence = data->Sequence();
		if (mSelectedSequence > -1) {
			ShapesSequence *seq = ((ShapesDocument*)GetDocument())->GetSequence(new_coll, new_vers, mSelectedSequence);
			
			// setup sequence panel controls
			s_outer_static_box->SetLabel(wxString::Format(wxT("Sequence %d of %u"),
														  mSelectedSequence, ((ShapesDocument*)GetDocument())->CollectionSequenceCount(new_coll, new_vers)));
			s_name_field->ChangeValue(seq->Name());
			switch (seq->NumberOfViews()) {
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
					wxMessageBox(wxString::Format(wxT("This sequence has an unknown type %d, and ShapeFusion can not handle it. Something strange may happen now!"), seq->NumberOfViews()),
								 wxT("Warning"), wxOK | wxICON_ERROR, mFrame);
					break;
			}
			s_fpv_field->ChangeValue(INT_TO_WXSTRING(seq->FramesPerView()));
			s_tpf_field->ChangeValue(INT_TO_WXSTRING(seq->TicksPerFrame()));
			s_lf_field->ChangeValue(INT_TO_WXSTRING(seq->LoopFrame()));
			s_kf_field->ChangeValue(INT_TO_WXSTRING(seq->KeyFrame()));
			s_xfermode_menu->SetSelection(seq->TransferMode());
			s_xferperiod_field->ChangeValue(INT_TO_WXSTRING(seq->TransferModePeriod()));
			s_ffs_field->ChangeValue(INT_TO_WXSTRING(seq->FirstFrameSound()));
			s_kfs_field->ChangeValue(INT_TO_WXSTRING(seq->KeyFrameSound()));
			s_lfs_field->ChangeValue(INT_TO_WXSTRING(seq->LastFrameSound()));
			// setup the sequence view
			wxBeginBusyCursor();
			s_fb->Freeze();
			s_fb->Clear();
			s_fb->SetColorTable(((ShapesDocument*)GetDocument())->GetColorTable(new_coll, new_vers, mViewColorTable));
			for (unsigned int i = 0; i < ((ShapesDocument*)GetDocument())->CollectionBitmapCount(mSelectedColl, mSelectedVers); i++)
				s_fb->AddBitmap(((ShapesDocument*)GetDocument())->GetBitmap(mSelectedColl, mSelectedVers, i));
			for (unsigned int i = 0; i < ((ShapesDocument*)GetDocument())->CollectionFrameCount(mSelectedColl, mSelectedVers); i++)
				s_fb->AddFrame(((ShapesDocument*)GetDocument())->GetFrame(mSelectedColl, mSelectedVers, i));
			s_fb->SetSeqParameters(seq->NumberOfViews(), seq->FramesPerView(), &seq->mFrameIndexes, this);
			s_fb->Thaw();
			wxEndBusyCursor();
		}

		menubar->SetLabel(EDIT_MENU_DELETE, _("Delete"));
		menubar->Enable(EDIT_MENU_COPY, false);
		menubar->Enable(EDIT_MENU_DELETE, false);
		menubar->Enable(EDIT_MENU_PASTE, false);
		
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
				chunk_inner_box->Show(chunk_undef_label, !((ShapesDocument*)GetDocument())->CollectionDefined(mSelectedColl, mSelectedVers));
				chunk_inner_box->Show(chunk_grid, ((ShapesDocument*)GetDocument())->CollectionDefined(mSelectedColl, mSelectedVers));

				menubar->Enable(EDIT_MENU_COPY, true);
				menubar->Enable(EDIT_MENU_PASTE, true);
				break;
			case TREESECTION_BITMAPS:
				mainbox->Show(b_outer_sizer, true);
				b_outer_sizer->Show(b_edit_box, bb->GetSelection() != -1);
				b_outer_sizer->Show(b_count_label, bb->GetSelection() == -1);
				break;
			case TREESECTION_COLORTABLES:
				mainbox->Show(ct_outer_sizer, true);
				ct_outer_sizer->Show(ct_edit_box, ctb->GetSelection() != -1);
				ct_outer_sizer->Show(ct_count_label, ctb->GetSelection() == -1);
				break;
			case TREESECTION_FRAMES:
				mainbox->Show(f_outer_sizer, true);
				f_outer_sizer->Show(f_edit_box, fb->GetSelection() != -1);
				f_outer_sizer->Show(f_count_label, fb->GetSelection() == -1);
				break;
			case TREESECTION_SEQUENCES:
				menubar->SetLabel(EDIT_MENU_DELETE, _("Delete sequence"));
				if (mSelectedSequence > -1) {
					mainbox->Show(s_outer_sizer, true);
					menubar->Enable(EDIT_MENU_DELETE, true);
				} else {
					mainbox->Show(dummy_sizer, true);
				}
				break;
		}
		mainbox->Layout();
	}
}

// selection event in the bitmap browser
void ShapesView::OnBitmapSelect(wxCommandEvent &e)
{
	int	selection = e.GetInt();
	
	if (selection < 0) {
		// deselection
		b_view->SetBitmap(NULL);
		b_outer_sizer->Show(b_count_label, true);
		b_outer_sizer->Show(b_edit_box, false);
		menubar->SetLabel(EDIT_MENU_DELETE, wxT("Delete"));
		menubar->Enable(EDIT_MENU_COPY, false);
		menubar->Enable(EDIT_MENU_DELETE, false);
		menubar->Enable(EDIT_MENU_PASTE, false);
		menubar->Enable(SHAPES_MENU_EXPORTBITMAP, false);
		menubar->Enable(SHAPES_MENU_EXPORTMASK, false);
	} else {
		ShapesBitmap	*sel_bitmap = ((ShapesDocument*)GetDocument())->GetBitmap(mSelectedColl, mSelectedVers, selection);
		
		// set labels
		wxString	info_label = wxString::Format(wxT("%dx%d pixels, "), sel_bitmap->Width(), sel_bitmap->Height());
		
		if (sel_bitmap->BytesPerRow() > 0)
			info_label << wxT("plain encoding");
		else
			info_label << wxT("RLE");
		b_info_label->SetLabel(info_label);
		b_edit_static_box->SetLabel(wxString::Format(wxT("Bitmap %d of %d"),
													 selection, ((ShapesDocument*)GetDocument())->CollectionBitmapCount(mSelectedColl, mSelectedVers)));
		// set flag check boxes
		b_order_checkbox->SetValue(sel_bitmap->IsColumnOrdered());
		b_transparency_checkbox->SetValue(sel_bitmap->IsTransparent());
		// set bitmap view
		b_view->SetBitmap(sel_bitmap);
		b_outer_sizer->Show(b_count_label, false);
		b_outer_sizer->Show(b_edit_box, true);
		menubar->SetLabel(EDIT_MENU_DELETE, wxT("Delete bitmap"));
		menubar->Enable(EDIT_MENU_COPY, true);
		menubar->Enable(EDIT_MENU_DELETE, true);
		menubar->Enable(EDIT_MENU_PASTE, true);
		menubar->Enable(SHAPES_MENU_EXPORTBITMAP, true);
		menubar->Enable(SHAPES_MENU_EXPORTMASK, true);
	}
	b_outer_sizer->Layout();
}

// handle a delete event from the bitmap browser
void ShapesView::BitmapDelete(wxCommandEvent &e)
{
	DoDeleteBitmap(e.GetSelection());
}

void ShapesView::DoCopyBitmap(int which)
{
	ShapesBitmap* bitmap = ((ShapesDocument*)GetDocument())->GetBitmap(mSelectedColl, mSelectedVers, which);
	ShapesColorTable* colorTable = ((ShapesDocument*)GetDocument())->GetColorTable(mSelectedColl, mSelectedVers, mViewColorTable);
	
	bitmap->ClipboardCopy(colorTable);
}

void ShapesView::DoDeleteColorTable(int which)
{
	if (which >= 0) {
		// first do like a color table deselection
		ct_view->SetColorTable(NULL);
		ct_outer_sizer->Show(ct_count_label, true);
		ct_outer_sizer->Show(ct_edit_box, false);
		menubar->Enable(SHAPES_MENU_SAVECOLORTABLE, false);
		menubar->Enable(SHAPES_MENU_SAVECOLORTABLETOPS, false);
		menubar->SetLabel(EDIT_MENU_DELETE, wxT("Delete"));
		menubar->Enable(EDIT_MENU_COPY, false);
		menubar->Enable(EDIT_MENU_DELETE, false);
		menubar->Enable(EDIT_MENU_PASTE, false);
		ctb->Freeze();
		ctb->Clear();
		// delete
		((ShapesDocument*)GetDocument())->DeleteColorTable(mSelectedColl, mSelectedVers, which);
		// update the view color table
		if (mViewColorTable == which) {
			mViewColorTable = 0;
			ShapesColorTable	*ctp = ((ShapesDocument*)GetDocument())->GetColorTable(
																					   mSelectedColl, mSelectedVers, mViewColorTable);
			
			wxBeginBusyCursor();
			bb->SetColorTable(ctp);
			b_view->SetColorTable(ctp);
			fb->SetColorTable(ctp);
			f_view->SetColorTable(ctp);
			s_fb->SetColorTable(ctp);
			wxEndBusyCursor();
		} else if (mViewColorTable > which) {
			mViewColorTable = mViewColorTable - 1;
			ShapesColorTable    *ctp = ((ShapesDocument*)GetDocument())->GetColorTable(
																					   mSelectedColl, mSelectedVers, mViewColorTable);
			
			wxBeginBusyCursor();
			bb->SetColorTable(ctp);
			b_view->SetColorTable(ctp);
			fb->SetColorTable(ctp);
			f_view->SetColorTable(ctp);
			s_fb->SetColorTable(ctp);
			wxEndBusyCursor();
		}
		// reset other gui elements
		unsigned int	colorTableCount = ((ShapesDocument*)GetDocument())->CollectionColorTableCount(
																									  mSelectedColl, mSelectedVers);
		
		for (unsigned int i = 0; i < colorTableCount; i++)
			ctb->AddColorTable(((ShapesDocument*)GetDocument())->GetColorTable(mSelectedColl, mSelectedVers, i));
		ctb->Thaw();
		wxMenu	*colortables_submenu;
		menubar->FindItem(VIEW_MENU_COLORTABLE_0, &colortables_submenu);
		for (unsigned int i = 0; i < colortables_submenu->GetMenuItemCount(); i++) {
			menubar->Enable(VIEW_MENU_COLORTABLE_0 + i, i < colorTableCount);
			menubar->Check(VIEW_MENU_COLORTABLE_0 + mViewColorTable, i == (unsigned int)mViewColorTable);
		}
		((ShapesDocument*)GetDocument())->Modify(true);
	}
}

void ShapesView::DoDeleteBitmap(int which)
{
	if (which >= 0) {
		// fist make sure no GUI element references that bitmap anymore
		bb->Freeze();
		bb->Clear();				// FIXME just remove that bitmap
		fb->Freeze();
		fb->ClearBitmaps();			// FIXME just remove that bitmap
		b_outer_sizer->Show(b_count_label, true);
		b_outer_sizer->Show(b_edit_box, false);
		b_view->SetBitmap(NULL);
		// delete
		((ShapesDocument*)GetDocument())->DeleteBitmap(mSelectedColl, mSelectedVers, which);
		// update the GUI
		unsigned int	bitmap_count = ((ShapesDocument*)GetDocument())->CollectionBitmapCount(mSelectedColl, mSelectedVers);
		
		for (unsigned int i = 0; i < bitmap_count; i++) {
			ShapesBitmap	*bmp = ((ShapesDocument*)GetDocument())->GetBitmap(mSelectedColl, mSelectedVers, i);
			
			bb->AddBitmap(bmp);
			fb->AddBitmap(bmp);
		}
		bb->Thaw();
		fb->RebuildThumbnails();	// FIXME just rebuild dirty frames
		fb->Thaw();
		
		wxString	count_string;
		
		count_string << bitmap_count << wxT(" bitmap");
		if (bitmap_count != 1)
			count_string << wxT("s");
		b_count_label->SetLabel(count_string);
		mFrame->Layout();
		((ShapesDocument*)GetDocument())->Modify(true);
	}
}

void ShapesView::DoDeleteFrame(int which)
{
	if (which >= 0) {
		// first make sure no GUI element references that frame anymore
		fb->Freeze();
		fb->Clear();	// FIXME just remove THAT frame
		f_outer_sizer->Show(f_count_label, true);
		f_outer_sizer->Show(f_edit_box, false);
		f_view->SetFrame(NULL);
		// delete
		((ShapesDocument*)GetDocument())->DeleteFrame(mSelectedColl, mSelectedVers, which);
		
		unsigned int	frame_count = ((ShapesDocument*)GetDocument())->CollectionFrameCount(mSelectedColl, mSelectedVers),
		bitmap_count = ((ShapesDocument*)GetDocument())->CollectionBitmapCount(mSelectedColl, mSelectedVers);
		
		for (unsigned int i = 0; i < bitmap_count; i++)
			fb->AddBitmap(((ShapesDocument*)GetDocument())->GetBitmap(mSelectedColl, mSelectedVers, i));
		for (unsigned int i = 0; i < frame_count; i++)
			fb->AddFrame(((ShapesDocument*)GetDocument())->GetFrame(mSelectedColl, mSelectedVers, i));
		fb->Thaw();
		
		wxString	count_string;
		
		count_string << frame_count << wxT(" frame");
		if (frame_count != 1)
			count_string << wxT("s");
		f_count_label->SetLabel(count_string);
		mFrame->Layout();
		((ShapesDocument*)GetDocument())->Modify(true);
	}
}

void ShapesView::DoPasteBitmap(int which)
{
	ShapesBitmap* bitmap = ((ShapesDocument*)GetDocument())->GetBitmap(mSelectedColl, mSelectedVers, which);
	ShapesColorTable* colorTable = ((ShapesDocument*)GetDocument())->GetColorTable(mSelectedColl, mSelectedVers, mViewColorTable);
	
	wxBeginBusyCursor();
	bitmap->ClipboardPaste(colorTable);
	
	// automagically initialize bitmap flags
	if (((ShapesDocument*)GetDocument())->CollectionType(mSelectedColl, mSelectedVers) == _object_collection ||
	    ((ShapesDocument*)GetDocument())->CollectionType(mSelectedColl, mSelectedVers) == _scenery_collection) {
		// compress weapons, monsters and scenery
		bitmap->SetBytesPerRow(-1);
	} else if (((ShapesDocument*)GetDocument())->CollectionType(mSelectedColl, mSelectedVers) == _interface_collection) {
		// interface elements are row-ordered (not so important with A1 actually)
		bitmap->SetColumnOrdered(false);
	}
	
	((ShapesDocument*)GetDocument())->Modify(true);
	bb->RebuildThumbnail(bb->GetSelection());
	bb->Refresh();
	b_view->SetBitmap(bitmap);
	fb->Freeze();
	fb->RebuildThumbnails();
	fb->Thaw();
	wxEndBusyCursor();
}

void ShapesView::DoCopyChunk(unsigned int coll, unsigned int chunk)
{
	ShapesChunk* c = ((ShapesDocument*)GetDocument())->GetChunk(coll, chunk);
	if (c) {
		c->ClipboardCopy();
	}
}

void ShapesView::DoPasteChunk(unsigned int coll, unsigned int chunk)
{
	ShapesChunk* c = ((ShapesDocument*)GetDocument())->GetChunk(coll, chunk);
	if (c) {
		c->ClipboardPaste();
		((ShapesDocument*)GetDocument())->Modify(true);
		mSelectedColl = -1;
		mSelectedVers = -1;
		mSelectedSequence = -1;

		colltree->DeleteAllItems();
		OnUpdate(0, 0);
	}
}
	
void ShapesView::ToggleBitmapCheckboxes(wxCommandEvent &e)
{
	ShapesBitmap	*sel_bitmap = b_view->GetBitmap();
	
	if (sel_bitmap != NULL) {
		switch (e.GetId()) {
			case CB_COLUMN_ORDER:
				sel_bitmap->SetColumnOrdered(e.IsChecked());
				((ShapesDocument*)GetDocument())->Modify(true);
				break;
			case CB_ENABLE_TRANSPARENCY:
				sel_bitmap->SetTransparent(e.IsChecked());
				bb->RebuildThumbnail(bb->GetSelection());
				bb->Refresh();
				b_view->SetBitmap(sel_bitmap);
				// FIXME also update the FrameBrowser and all that
				((ShapesDocument*)GetDocument())->Modify(true);
				break;
		}
	}
}

// callback for selections in the color table browser
void ShapesView::OnCTSelect(wxCommandEvent &e)
{
	int	selection = e.GetInt();
	
	if (selection < 0) {
		// deselection
		ct_view->SetColorTable(NULL);
		ct_outer_sizer->Show(ct_count_label, true);
		ct_outer_sizer->Show(ct_edit_box, false);
		menubar->Enable(SHAPES_MENU_SAVECOLORTABLE, false);
		menubar->Enable(SHAPES_MENU_SAVECOLORTABLETOPS, false);
		menubar->SetLabel(EDIT_MENU_DELETE, wxT("Delete"));
		menubar->Enable(EDIT_MENU_COPY, false);
		menubar->Enable(EDIT_MENU_DELETE, false);
		menubar->Enable(EDIT_MENU_PASTE, false);
	} else {
		// selection
		ShapesColorTable	*ct = ((ShapesDocument*)GetDocument())->GetColorTable(mSelectedColl, mSelectedVers, selection);
		
		ct_view->SetColorTable(ct);
		ct_edit_static_box->SetLabel(wxString::Format(wxT("Color table %d of %d, %d colors per table"), selection,
													  ((ShapesDocument*)GetDocument())->CollectionColorTableCount(mSelectedColl, mSelectedVers),
													  ct->ColorCount()));
		ct_outer_sizer->Show(ct_count_label, false);
		ct_outer_sizer->Show(ct_edit_box, true);
		menubar->Enable(SHAPES_MENU_SAVECOLORTABLE, true);
		menubar->Enable(SHAPES_MENU_SAVECOLORTABLETOPS, true);
		menubar->SetLabel(EDIT_MENU_DELETE, wxT("Delete color table"));
		// FIXME make sure there is at least one color table for now
		if (((ShapesDocument*)GetDocument())->CollectionColorTableCount(mSelectedColl, mSelectedVers) > 1)
			menubar->Enable(EDIT_MENU_DELETE, true);
		else
			menubar->Enable(EDIT_MENU_DELETE, false);
		menubar->Enable(EDIT_MENU_COPY, false);
		menubar->Enable(EDIT_MENU_PASTE, false);
	}
	ct_self_lumin_checkbox->Disable();
	ct_gradient_button->Disable();
	ct_outer_sizer->Layout();
}

// callback for color selections in the color table editor
void ShapesView::CTColorSelect(wxCommandEvent &e)
{
	ct_self_lumin_checkbox->Disable();
	ct_gradient_button->Disable();
	switch (e.GetInt()) {
		default:	// more colors selected
			ct_gradient_button->Enable();
		case 1:		// just one color selected
			ct_self_lumin_checkbox->Enable();
			// set the checkbox value
		{
			ShapesColorTable	*ct = ((ShapesDocument*)GetDocument())->GetColorTable(mSelectedColl,
																					  mSelectedVers, ctb->GetSelection());
			vector<bool>		selection = ct_view->GetSelection();
			bool				someAreOn = false,
			someAreOff = false;
			
			for (unsigned int i = 0; i < selection.size(); i++) {
				if (selection[i]) {
					if (ct->GetColor(i)->Luminescent())
						someAreOn = true;
					else
						someAreOff = true;
				}
			}
			if (someAreOn && someAreOff)
				ct_self_lumin_checkbox->Set3StateValue(wxCHK_UNDETERMINED);
			else if (someAreOn && !someAreOff)
				ct_self_lumin_checkbox->Set3StateValue(wxCHK_CHECKED);
			else
				ct_self_lumin_checkbox->Set3StateValue(wxCHK_UNCHECKED);
		}
		case 0:		// no colors selected
			break;
	}
}

// callback for color alteration in the color table editor
void ShapesView::CTColorChanged(wxCommandEvent &e)
{
	((ShapesDocument*)GetDocument())->Modify(true);
	ctb->Refresh();
	// refresh thumbnails if needed
	if (ctb->GetSelection() == mViewColorTable) {
		bb->Freeze();
		bb->RebuildThumbnails();
		bb->Thaw();
		fb->Freeze();
		fb->RebuildThumbnails();
		fb->Thaw();
	}
}

// callback for the "self luminescent color" checkbox
void ShapesView::ToggleSelfLuminCheckbox(wxCommandEvent &e)
{
	ShapesColorTable	*ct = ((ShapesDocument*)GetDocument())->GetColorTable(mSelectedColl,
																			  mSelectedVers, ctb->GetSelection());
	vector<bool>		selection = ct_view->GetSelection();
	
	for (unsigned int i = 0; i < selection.size(); i++) {
		if (selection[i])
			ct->GetColor(i)->SetLuminescent(e.IsChecked());
	}
	((ShapesDocument*)GetDocument())->Modify(true);
	ctb->Refresh();
	ct_view->Refresh();
	// TODO invalidate thumbnails if needed
}

// callback for the "make gradient" button in the color table editor
void ShapesView::MakeCTGradient(wxCommandEvent &e)
{
	vector<bool>	selection = ct_view->GetSelection();
	int				firstIndex = -1, lastIndex = -1;
	
	// find first and last selected items
	for (unsigned int i = 0; i < selection.size(); i++) {
		if (selection[i]) {
			if (firstIndex == -1)
				firstIndex = i;
			lastIndex = i;
		}
	}
	if (firstIndex > -1 && firstIndex < lastIndex) {
		// linearly interpolate colors in between
		ShapesColorTable	*ct = ((ShapesDocument*)GetDocument())->GetColorTable(mSelectedColl,
																				  mSelectedVers, ctb->GetSelection());
		int					r1 = ct->GetColor(firstIndex)->Red(),
		g1 = ct->GetColor(firstIndex)->Green(),
		b1 = ct->GetColor(firstIndex)->Blue(),
		r2 = ct->GetColor(lastIndex)->Red(),
		g2 = ct->GetColor(lastIndex)->Green(),
		b2 = ct->GetColor(lastIndex)->Blue(),
		delta = lastIndex - firstIndex;
		
		for (int k = firstIndex+1; k < lastIndex; k++) {
			ShapesColor	*color = ct->GetColor(k);
			
			color->SetRed(r1 + (r2-r1)*(k-firstIndex)/delta);
			color->SetGreen(g1 + (g2-g1)*(k-firstIndex)/delta);
			color->SetBlue(b1 + (b2-b1)*(k-firstIndex)/delta);
		}
	}
	((ShapesDocument*)GetDocument())->Modify(true);
	ctb->Refresh();
	ct_view->Refresh();
	// refresh thumbnails if needed
	if (ctb->GetSelection() == mViewColorTable) {
		bb->Freeze();
		bb->RebuildThumbnails();
		bb->Thaw();
		fb->Freeze();
		fb->RebuildThumbnails();
		fb->Thaw();
	}
}

// callback for selections in the frame browser
void ShapesView::OnFrameSelect(wxCommandEvent &e)
{
	int selection = e.GetInt();
	
	if (selection < 0) {
		f_view->SetFrame(NULL);
		f_view->SetBitmap(NULL);
		f_outer_sizer->Show(f_count_label, true);
		f_outer_sizer->Show(f_edit_box, false);
		menubar->SetLabel(EDIT_MENU_DELETE, wxT("Delete"));
		menubar->Enable(EDIT_MENU_COPY, false);
		menubar->Enable(EDIT_MENU_DELETE, false);
		menubar->Enable(EDIT_MENU_PASTE, false);
	} else {
		ShapesFrame	*sel_frame = ((ShapesDocument*)GetDocument())->GetFrame(mSelectedColl, mSelectedVers, selection);
		ShapesBitmap	*assoc_bitmap = NULL;
		
		// set labels
		f_edit_static_box->SetLabel(wxString::Format(wxT("Frame %d of %u"),
													 selection, ((ShapesDocument*)GetDocument())->CollectionFrameCount(mSelectedColl, mSelectedVers)));
		// set frame view
		f_view->SetFrame(sel_frame);
		if (sel_frame->BitmapIndex() >= 0)
			assoc_bitmap = ((ShapesDocument*)GetDocument())->GetBitmap(mSelectedColl, mSelectedVers, sel_frame->BitmapIndex());
		f_view->SetBitmap(assoc_bitmap);
		// set controls
		f_bitmap_id->SetRange(-1, ((ShapesDocument*)GetDocument())->CollectionBitmapCount(mSelectedColl, mSelectedVers) - 1);
		f_bitmap_id->SetValue(sel_frame->BitmapIndex());
		f_xmirror_checkbox->SetValue(sel_frame->IsXmirrored());
		f_ymirror_checkbox->SetValue(sel_frame->IsYmirrored());
		f_keypoint_checkbox->SetValue(sel_frame->IsKeypointObscured());
		f_origin_x_field->ChangeValue(INT_TO_WXSTRING(sel_frame->OriginX()));
		f_origin_y_field->ChangeValue(INT_TO_WXSTRING(sel_frame->OriginY()));
		f_key_x_field->ChangeValue(INT_TO_WXSTRING(sel_frame->KeyX()));
		f_key_y_field->ChangeValue(INT_TO_WXSTRING(sel_frame->KeyY()));
		f_scalefactor_field->ChangeValue(INT_TO_WXSTRING(sel_frame->ScaleFactor()));
		f_mli_field->ChangeValue(INT_TO_WXSTRING((int)roundf(sel_frame->MinimumLightIntensity() * 100.0)));
		f_outer_sizer->Show(f_count_label, false);
		f_outer_sizer->Show(f_edit_box, true);
		menubar->SetLabel(EDIT_MENU_DELETE, wxT("Delete frame"));
		menubar->Enable(EDIT_MENU_DELETE, true);
		menubar->Enable(EDIT_MENU_COPY, false);
		menubar->Enable(EDIT_MENU_PASTE, false);
	}
	f_outer_sizer->Layout();
}

// handle a delete event from the frame browser
void ShapesView::FrameDelete(wxCommandEvent &e)
{
	DoDeleteFrame(e.GetSelection());
}

// handle an event from the FrameView: update the values
// in the fields and mark the document as modified
void ShapesView::OnFramePointDrag(wxCommandEvent &e)
{
	ShapesFrame	*frame = f_view->GetFrame();
	
	if (frame != NULL) {
		f_origin_x_field->ChangeValue(INT_TO_WXSTRING(frame->OriginX()));
		f_origin_y_field->ChangeValue(INT_TO_WXSTRING(frame->OriginY()));
		f_key_x_field->ChangeValue(INT_TO_WXSTRING(frame->KeyX()));
		f_key_y_field->ChangeValue(INT_TO_WXSTRING(frame->KeyY()));
		((ShapesDocument*)GetDocument())->Modify(true);
	}
}

// bitmap index change in the frame panel
void ShapesView::BitmapIndexSpin(wxSpinEvent &e)
{
	int			newid = e.GetPosition();
	ShapesFrame	*sel_frame = f_view->GetFrame();
	
	if (sel_frame != NULL) {
		sel_frame->SetBitmapIndex(newid);
		f_view->SetBitmap(((ShapesDocument*)GetDocument())->GetBitmap(mSelectedColl, mSelectedVers, newid));
		f_view->SetFrame(sel_frame);
		fb->RebuildThumbnail(fb->GetSelection());
		fb->Refresh();
		((ShapesDocument*)GetDocument())->Modify(true);
	}
}

// checkbox toggle in the frame panel
void ShapesView::ToggleFrameCheckboxes(wxCommandEvent &e)
{
	ShapesFrame	*sel_frame = f_view->GetFrame();
	
	if (sel_frame != NULL) {
		switch (e.GetId()) {
			case CB_XMIRROR:	sel_frame->SetXmirrored(e.IsChecked());			break;
			case CB_YMIRROR:	sel_frame->SetYmirrored(e.IsChecked());			break;
			case CB_KEYPOINT:	sel_frame->SetKeypointObscured(e.IsChecked());	break;
		}
		// update display
		if (e.GetId() != CB_KEYPOINT) {
			fb->RebuildThumbnail(fb->GetSelection());
			fb->Refresh();
			f_view->SetFrame(sel_frame);
		}
		((ShapesDocument*)GetDocument())->Modify(true);
	}
}

// user messed with fields in the frame panel
void ShapesView::EditFrameFields(wxCommandEvent &e)
{
	ShapesFrame	*sel_frame = f_view->GetFrame();
	wxString	s = e.GetString();
	
	if (sel_frame != NULL) {
		long	v = 0;
		
		if (s.ToLong(&v)) {
			bool	recalculate_world_fields = false;
			
			switch (e.GetId()) {
				case FIELD_ORIGIN_X:
					sel_frame->SetOriginX(v);
					recalculate_world_fields = true;
					break;
				case FIELD_ORIGIN_Y:
					sel_frame->SetOriginY(v);
					recalculate_world_fields = true;
					break;
				case FIELD_KEY_X:
					sel_frame->SetKeyX(v);
					recalculate_world_fields = true;
					break;
				case FIELD_KEY_Y:
					sel_frame->SetKeyY(v);
					recalculate_world_fields = true;
					break;
				case FIELD_FRAME_SCALEFACTOR:
					sel_frame->SetScaleFactor(v);
					recalculate_world_fields = true;
					break;
				case FIELD_MIN_LIGHT_INT:
					if (v > 100) {
						wxBell();
						f_mli_field->ChangeValue(wxT("100"));
						v = 100;
					} else if (v < 0) {
						wxBell();
						f_mli_field->ChangeValue(wxT("0"));
						v = 0;
					}
					sel_frame->SetMinimumLightIntensity(v / 100.0);
					break;
			}
			// recalculate world_* fields if needed and possible
			if (recalculate_world_fields && sel_frame->BitmapIndex() >= 0
				&& sel_frame->BitmapIndex() < (int)((ShapesDocument*)GetDocument())->CollectionBitmapCount(mSelectedColl, mSelectedVers)) {
				ShapesBitmap	*assoc_bitmap = ((ShapesDocument*)GetDocument())->GetBitmap(mSelectedColl, mSelectedVers, sel_frame->BitmapIndex());
				int			w = assoc_bitmap->Width(),
				h = assoc_bitmap->Height(),
				scale_factor = sel_frame->ScaleFactor();
				
				sel_frame->SetWorldLeft(-scale_factor * sel_frame->OriginX());
				sel_frame->SetWorldTop(scale_factor * sel_frame->OriginY());
				sel_frame->SetWorldRight(scale_factor * (w - sel_frame->OriginX()));
				sel_frame->SetWorldBottom(-scale_factor * (h - sel_frame->OriginY()));
				sel_frame->SetWorldX0(scale_factor * (sel_frame->KeyX() - sel_frame->OriginX()));
				sel_frame->SetWorldY0(-scale_factor * (sel_frame->KeyY() - sel_frame->OriginY()));
			}
			f_view->Refresh();
			((ShapesDocument*)GetDocument())->Modify(true);
		}
	}
}

// callback for "delete sequence" button clicks
void ShapesView::DeleteSequence(wxCommandEvent &e)
{
	// first delete the sequence for real
	((ShapesDocument*)GetDocument())->DeleteSequence(mSelectedColl, mSelectedVers, mSelectedSequence);
	
	// for updating the tree control we could just delete the selected
	// sequence item, but then ShapesTreeItemData structures associated
	// to the following items would be broken and everything would crash.
	// We could correct them, but reinserting all items is simpler.
	wxTreeItemId	seqnode = GetSequencesTreeItem(mSelectedColl, mSelectedVers);
	
	colltree->SelectItem(seqnode);
	colltree->DeleteChildren(seqnode);
	for (unsigned int k = 0; k < ((ShapesDocument*)GetDocument())->CollectionSequenceCount(mSelectedColl, mSelectedVers); k++) {
		ShapesSequence		*seq = ((ShapesDocument*)GetDocument())->GetSequence(mSelectedColl, mSelectedVers, k);
		wxString		label;
		ShapesTreeItemData	*seqdata = new ShapesTreeItemData(mSelectedColl, mSelectedVers, TREESECTION_SEQUENCES, k);
		
		label << k;
		if (seq->Name().Len() > 0)
			label << wxT(" - ") << seq->Name();
		colltree->AppendItem(seqnode, label, -1, -1, seqdata);
	}
	
	mSelectedSequence = -1;
	((ShapesDocument*)GetDocument())->Modify(true);
}

// sequence type menu in the sequence editor
void ShapesView::EditSequenceType(wxCommandEvent &e)
{
	if (mSelectedSequence >= 0) {
		ShapesSequence	*sel_seq = ((ShapesDocument*)GetDocument())->GetSequence(mSelectedColl, mSelectedVers, mSelectedSequence);
		int			real_nov, old_nov;
		
		old_nov = ActualNumberOfViews(sel_seq->NumberOfViews());
		// always use ANIMATED_4, ANIMATED_5, ANIMATED_8
		// and never other values like ANIMATED_3TO4. Apparently
		// nobody knows the real meaning of these values
		switch (s_type_menu->GetSelection()) {
			case 0:	sel_seq->SetNumberOfViews(UNANIMATED);	break;
			case 1:	sel_seq->SetNumberOfViews(ANIMATED_1);	break;
			case 2: sel_seq->SetNumberOfViews(ANIMATED_4);	break;
			case 3:	sel_seq->SetNumberOfViews(ANIMATED_5);	break;
			case 4:	sel_seq->SetNumberOfViews(ANIMATED_8);	break;
		}
		real_nov = ActualNumberOfViews(sel_seq->NumberOfViews());
		
		// Let's handle sequence frames changes...
		if (real_nov > old_nov) {
			// We are adding one (or more) view
			// We need to add FramesPerView() * (real_nov - old_nov)
			// to the END of the frame_index array
			for (int i = 0; i < sel_seq->FramesPerView() * (real_nov - old_nov); i++)
				sel_seq->mFrameIndexes.push_back(-1);
		} else if (real_nov < old_nov) {
			// We are removing one (or more) view
			// We need to remove FramesPerView() * (old_nov - real_nov)
			// from the END of the frame_index array
			for (int i = 0; i < sel_seq->FramesPerView() * (old_nov - real_nov); i++)
				sel_seq->mFrameIndexes.pop_back();
		} else {
			// Hmm, number of views unchanged, don't bother...
		}
		s_fb->SetSeqParameters(sel_seq->NumberOfViews(), sel_seq->FramesPerView(), &sel_seq->mFrameIndexes, this);
		((ShapesDocument*)GetDocument())->Modify(true);
	}
}

// transfer mode menu in the sequence editor
void ShapesView::EditSequenceXferMode(wxCommandEvent &e)
{
	if (mSelectedSequence >= 0) {
		ShapesSequence	*sel_seq = ((ShapesDocument*)GetDocument())->GetSequence(mSelectedColl, mSelectedVers, mSelectedSequence);
		
		sel_seq->SetTransferMode(s_xfermode_menu->GetSelection());
		((ShapesDocument*)GetDocument())->Modify(true);
	}
}

// user messed with fields in the sequence editor
void ShapesView::EditSequenceFields(wxCommandEvent &e)
{
	if (mSelectedSequence >= 0) {
		ShapesSequence	*sel_seq = ((ShapesDocument*)GetDocument())->GetSequence(mSelectedColl, mSelectedVers, mSelectedSequence);
		wxString	s = e.GetString();
		
		if (sel_seq != NULL) {
			if (e.GetId() == FIELD_SEQ_NAME) {
				sel_seq->SetName(s.Left(32));
				// update the tree item label
				wxTreeItemId		seqnode = GetSequencesTreeItem(mSelectedColl, mSelectedVers);
				wxTreeItemIdValue	cookie;
				wxTreeItemId		id = colltree->GetFirstChild(seqnode, cookie);
				
				while (id.IsOk()) {
					ShapesTreeItemData	*itemdata = dynamic_cast<ShapesTreeItemData *>(colltree->GetItemData(id));
					
					if (itemdata->Sequence() == mSelectedSequence) {
						// here we are
						wxString	blabel;
						
						blabel << mSelectedSequence;
						if (sel_seq->Name().Length() > 0)
							blabel << wxT(" - ") << sel_seq->Name();
						colltree->SetItemText(id, blabel);
						break;
					}
					id = colltree->GetNextChild(seqnode, cookie);
				}
				((ShapesDocument*)GetDocument())->Modify(true);
			} else {
				// numeric fields
				long	v;
				
				if (s.ToLong(&v)) {
					switch (e.GetId()) {
						case FIELD_SEQ_FRAMES_PER_VIEW:
							// must update the SequenceView too
							if (v != sel_seq->FramesPerView()) {
								int		real_nov = ActualNumberOfViews(sel_seq->NumberOfViews()),
								old_fpv = sel_seq->FramesPerView();
								short	old_indexes[real_nov * old_fpv];
								
								for (int i = 0; i < real_nov * old_fpv; i++)
									old_indexes[i] = sel_seq->mFrameIndexes[i];
								sel_seq->SetFramesPerView(v);
								// try to preserve existing frame
								// references as much as possible
								sel_seq->mFrameIndexes.clear();
								for (int i = 0; i < real_nov; i++) {
									for (int j = 0; j < v; j++) {
										if (j < old_fpv)
											sel_seq->mFrameIndexes.push_back(old_indexes[i*old_fpv + j]);
										else
											sel_seq->mFrameIndexes.push_back(-1);
									}
								}
								s_fb->SetSeqParameters(sel_seq->NumberOfViews(), v, &sel_seq->mFrameIndexes, this);
							}
							break;
						case FIELD_SEQ_TICKS_PER_FRAME:
							sel_seq->SetTicksPerFrame(v);
							break;
						case FIELD_SEQ_LOOP_FRAME:
							sel_seq->SetLoopFrame(v);
							break;
						case FIELD_SEQ_KEY_FRAME:
							sel_seq->SetKeyFrame(v);
							break;
						case FIELD_SEQ_XFER_MODE_PERIOD:
							sel_seq->SetTransferModePeriod(v);
							break;
						case FIELD_SEQ_FIRST_FRAME_SND:
							sel_seq->SetFirstFrameSound(v);
							break;
						case FIELD_SEQ_KEY_FRAME_SND:
							sel_seq->SetKeyFrameSound(v);
							break;
						case FIELD_SEQ_LAST_FRAME_SND:
							sel_seq->SetLastFrameSound(v);
							break;
					}
					((ShapesDocument*)GetDocument())->Modify(true);
				}
			}
		}
	}
}


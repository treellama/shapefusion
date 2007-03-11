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
#include "utilities.h"

#define INT_TO_WXSTRING(a)	wxString::Format(wxT("%d"), a)

BEGIN_EVENT_TABLE(ShapesView, wxView)
	EVT_MENU(EDIT_MENU_DELETE, ShapesView::MenuEditDelete)
	EVT_MENU_RANGE(VIEW_MENU_COLORTABLE_0, VIEW_MENU_COLORTABLE_7, ShapesView::MenuViewCT)
	EVT_MENU_RANGE(VIEW_MENU_TNSIZE_SMALL, VIEW_MENU_TNSIZE_AUTO, ShapesView::MenuViewTNSize)
	EVT_MENU(VIEW_MENU_TRANSPARENCY, ShapesView::MenuViewTransparency)
	EVT_MENU(SHAPES_MENU_ADDCOLORTABLE, ShapesView::MenuShapesAddColorTable)
	EVT_MENU(SHAPES_MENU_SAVECOLORTABLE, ShapesView::MenuShapesSaveColorTable)
	EVT_MENU(SHAPES_MENU_SAVECOLORTABLETOPS, ShapesView::MenuShapesSaveColorTable)
	EVT_MENU(SHAPES_MENU_ADDBITMAP, ShapesView::MenuShapesAddBitmap)
	EVT_MENU(SHAPES_MENU_EXPORTBITMAPS, ShapesView::MenuShapesExportBitmaps)
	EVT_MENU(SHAPES_MENU_ADDFRAME, ShapesView::MenuShapesNewFrame)
	EVT_MENU(SHAPES_MENU_ADDSEQUENCE, ShapesView::MenuShapesNewSequence)
	EVT_TREE_SEL_CHANGED(-1, ShapesView::TreeSelect)
	// bitmaps
	EVT_COMMAND(BITMAP_BROWSER, wxEVT_BITMAPBROWSER, ShapesView::BitmapSelect)
	EVT_COMMAND(BITMAP_BROWSER, wxEVT_BITMAPBROWSER_DELETE, ShapesView::BitmapDelete)
	EVT_COMMAND_RANGE(CB_COLUMN_ORDER, CB_ENABLE_TRANSPARENCY, wxEVT_COMMAND_CHECKBOX_CLICKED, ShapesView::ToggleBitmapCheckboxes)
	EVT_BUTTON(BTN_SAVE_BITMAP, ShapesView::AskSaveBitmap)
	// color tables
	EVT_COMMAND(wxID_ANY, wxEVT_CTBROWSER, ShapesView::CTSelect)
	// frames
	EVT_COMMAND(FRAME_BROWSER, wxEVT_FRAMEBROWSER, ShapesView::FrameSelect)
	EVT_COMMAND(FRAME_BROWSER, wxEVT_FRAMEBROWSER_DELETE, ShapesView::FrameDelete)
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
	EVT_TEXT(FIELD_SEQ_SCALE_FACTOR, ShapesView::EditSequenceFields)
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
							
IMPLEMENT_DYNAMIC_CLASS(ShapesView, wxView)

ShapesView::ShapesView(void): 
	wxView(),
	selected_coll(-1), selected_vers(-1), selected_sequence(-1), view_ct(-1), show_transparent_pixels(false)
{
	frame = (wxFrame *) NULL;
}

ShapesView::~ShapesView(void)
{
}

bool ShapesView::OnCreate(wxDocument *doc, long WXUNUSED(flags) )
{
	wxString frameTitle = _T("ShapeFusion : Shapes : ");
	frameTitle.Append(doc->GetFilename());
	
    frame = wxGetApp().CreateChildFrame(doc, this, frameTitle, wxPoint(0, 0), wxSize(900, 600));

	frame->SetSizeHints(200, 200);
		
	CreateViewMenu(frame->GetMenuBar());
	CreateShapesMenu(frame->GetMenuBar());

	menubar = frame->GetMenuBar();

	mainbox = new wxBoxSizer(wxHORIZONTAL);
	// create the collection tree
	colltree = new wxTreeCtrl(frame, -1, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT);
	colltree->DeleteAllItems();
	wxTreeItemId	treeroot = colltree->AddRoot(doc->GetFilename());
	mainbox->Add(colltree, 2, wxEXPAND);
	// empty space (e.g. what is displayed when selecting the Sequences node)
	dummy_sizer = new wxBoxSizer(wxVERTICAL);
	mainbox->Add(dummy_sizer, 5, wxEXPAND);
	// collection section
	coll_sizer = new wxBoxSizer(wxVERTICAL);
	coll_text = new wxStaticText(frame, -1, wxT("Collection info"));
	coll_static_box = new wxStaticBox(frame, -1, wxT("Collection info"));
	coll_inner_box = new wxStaticBoxSizer(coll_static_box, wxVERTICAL);
	coll_inner_box->Add(coll_text, 0, wxALL, 5);
	coll_sizer->AddStretchSpacer();
	coll_sizer->Add(coll_inner_box, 0, wxALIGN_CENTER);
	coll_sizer->AddStretchSpacer();
	mainbox->Add(coll_sizer, 5, wxEXPAND);
	mainbox->Show(coll_sizer, false);
	// chunk section
	chunk_sizer = new wxBoxSizer(wxVERTICAL);
	chunk_static_box = new wxStaticBox(frame, -1, wxT("Version info"));
	chunk_inner_box = new wxStaticBoxSizer(chunk_static_box, wxVERTICAL);
	chunk_undef_label = new wxStaticText(frame, -1, wxT("Not defined"));
	chunk_inner_box->Add(chunk_undef_label, 0, wxCENTER | wxALL, 5);
	chunk_grid = new wxFlexGridSizer(2, 4);
	chunk_inner_box->Add(chunk_grid, 0, wxCENTER | wxALL, 5);
	chunk_version_label = new wxStaticText(frame, -1, wxT("Version:"));
	chunk_type_label = new wxStaticText(frame, -1, wxT("Collection type:"));
	chunk_flags_label = new wxStaticText(frame, -1, wxT("Flags:"));
	chunk_sf_label = new wxStaticText(frame, -1, wxT("Collection scale factor:"));
	chunk_version_field = new wxTextCtrl(frame, -1, wxT("0"));
	wxString	coll_type_labels[] = {	wxT("Unused"),
										wxT("Wall textures"),
										wxT("Objects"),
										wxT("Interface graphics"),
										wxT("Scenery objects") };
	chunk_type_menu = new wxChoice(frame, -1, wxDefaultPosition, wxDefaultSize, 5, coll_type_labels, 0);
	chunk_flags_field = new wxTextCtrl(frame, -1, wxT("0"));
	chunk_sf_field = new wxTextCtrl(frame, -1, wxT("0"));
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
	ctb = new CTBrowser(frame);
	ct_count_label = new wxStaticText(frame, -1, wxT("N color tables"));
	ct_outer_sizer->Add(ctb, 1, wxGROW);
	ct_outer_sizer->Add(ct_count_label, 0, wxALIGN_LEFT | wxLEFT | wxTOP | wxBOTTOM, 10);
	mainbox->Add(ct_outer_sizer, 5, wxEXPAND);
	mainbox->Show(ct_outer_sizer, false);
	// create the bitmaps section
	b_outer_sizer = new wxBoxSizer(wxVERTICAL);
	bb = new BitmapBrowser(frame, BITMAP_BROWSER);
	bb->SetThumbnailSize(64);
	b_count_label = new wxStaticText(frame, -1, wxT("N bitmaps"));
	b_edit_static_box = new wxStaticBox(frame, -1, wxT("Bitmap N of M"));
	b_edit_box = new wxStaticBoxSizer(b_edit_static_box, wxHORIZONTAL);
	b_edit_inner_box = new wxBoxSizer(wxVERTICAL);
	b_info_label = new wxStaticText(frame, -1, wxT("AxB pixels"));
	b_order_checkbox = new wxCheckBox(frame, CB_COLUMN_ORDER, wxT("Store pixels in column order"));
	b_transparency_checkbox = new wxCheckBox(frame, CB_ENABLE_TRANSPARENCY, wxT("Enable transparency"));
	b_view = new BitmapView(frame);
	b_save = new wxButton(frame, BTN_SAVE_BITMAP, wxT("Save bitmap as..."));
	b_replace = new wxButton(frame, BTN_REPLACE_BITMAP, wxT("Replace bitmap..."));
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
	fb = new FrameBrowser(frame, FRAME_BROWSER);
	fb->SetThumbnailSize(64);
	f_count_label = new wxStaticText(frame, -1, wxT("N frames"));
	f_edit_static_box = new wxStaticBox(frame, -1, wxT("Frame N"));
	f_edit_box = new wxStaticBoxSizer(f_edit_static_box, wxHORIZONTAL);
	f_bitmap_label = new wxStaticText(frame, -1, wxT("Associated bitmap:"));
	f_bitmap_id = new wxSpinCtrl(frame, FIELD_BITMAP_INDEX, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_xmirror_checkbox = new wxCheckBox(frame, CB_XMIRROR, wxT("X mirror"));
//	f_xmirror_checkbox->SetToolTip(wxT("Flip the bitmap horizontally"));
	f_ymirror_checkbox = new wxCheckBox(frame, CB_YMIRROR, wxT("Y mirror"));
	f_keypoint_checkbox = new wxCheckBox(frame, CB_KEYPOINT, wxT("Keypoint obscured"));
	f_origin_x_label = new wxStaticText(frame, -1, wxT("Origin X:"), wxDefaultPosition, wxSize(150, -1));
	f_origin_x_field = new wxTextCtrl(frame, FIELD_ORIGIN_X, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_origin_y_label = new wxStaticText(frame, -1, wxT("Origin Y:"), wxDefaultPosition, wxSize(150, -1));
	f_origin_y_field = new wxTextCtrl(frame, FIELD_ORIGIN_Y, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_key_x_label = new wxStaticText(frame, -1, wxT("Keypoint X:"), wxDefaultPosition, wxSize(150, -1));
	f_key_x_field = new wxTextCtrl(frame, FIELD_KEY_X, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_key_y_label = new wxStaticText(frame, -1, wxT("Keypoint Y:"), wxDefaultPosition, wxSize(150, -1));
	f_key_y_field = new wxTextCtrl(frame, FIELD_KEY_Y, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_scalefactor_label = new wxStaticText(frame, -1, wxT("Scale factor:"), wxDefaultPosition, wxSize(150, -1));
	f_scalefactor_field = new wxTextCtrl(frame, FIELD_FRAME_SCALEFACTOR, wxT("0"), wxDefaultPosition, wxSize(60, -1));
	f_mli_label = new wxStaticText(frame, -1, wxT("Minimum lightness (%):"), wxDefaultPosition, wxSize(150, -1));
	f_mli_field = new wxTextCtrl(frame, FIELD_MIN_LIGHT_INT, wxT("0"), wxDefaultPosition, wxSize(60, -1));
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
	f_view = new FrameView(frame);
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
	s_outer_static_box = new wxStaticBox(frame, -1, wxT("Sequence"));
	s_outer_sizer = new wxStaticBoxSizer(s_outer_static_box, wxVERTICAL);
	mainbox->Add(s_outer_sizer, 5, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);

	s_box1 = new wxBoxSizer(wxHORIZONTAL);
	s_outer_sizer->Add(s_box1, 0, wxEXPAND | wxALIGN_TOP | wxALL, 5);
	s_name_label = new wxStaticText(frame, -1, wxT("Name:"));
	s_name_field = new wxTextCtrl(frame, FIELD_SEQ_NAME, wxT("foobar"));
	s_delete_button = new wxButton(frame, BTN_DELETE_SEQ, wxT("Delete sequence"));
	s_box1->Add(s_name_label, 0, wxALIGN_CENTER | wxRIGHT, 5);
	s_box1->Add(s_name_field, 1, wxALIGN_CENTER);
	s_box1->Add(s_delete_button, 0, wxALIGN_CENTER | wxLEFT, 20);

	s_box2 = new wxBoxSizer(wxHORIZONTAL);
	s_outer_sizer->Add(s_box2, 0, wxEXPAND | wxALIGN_TOP | wxALL, 5);
	s_grid_box = new wxFlexGridSizer(5, 2, 4, 0);
	s_box2->Add(s_grid_box, 0, wxEXPAND | wxALIGN_TOP);
	s_type_label = new wxStaticText(frame, -1, wxT("Sequence type:"), wxDefaultPosition, wxSize(120, -1));
	wxString	anim_type_labels[] = { wxT("Display a random frame"),
										wxT("Animation with 1 view"),
										wxT("Animation with 4 views"),
										wxT("Animation with 5 views"),
										wxT("Animation with 8 views") };
	s_type_menu = new wxChoice(frame, MENU_SEQ_TYPE, wxDefaultPosition, wxDefaultSize, 5, anim_type_labels, 0);
	s_fpv_label = new wxStaticText(frame, -1, wxT("Frames per view:"), wxDefaultPosition, wxSize(120, -1));
	s_fpv_field = new wxTextCtrl(frame, FIELD_SEQ_FRAMES_PER_VIEW, wxT("1"));
	s_tpf_label = new wxStaticText(frame, -1, wxT("Ticks per frame:"), wxDefaultPosition, wxSize(120, -1));
	s_tpf_field = new wxTextCtrl(frame, FIELD_SEQ_TICKS_PER_FRAME, wxT("1"));
	s_lf_label = new wxStaticText(frame, -1, wxT("Loop at frame:"), wxDefaultPosition, wxSize(120, -1));
	s_lf_field = new wxTextCtrl(frame, FIELD_SEQ_LOOP_FRAME, wxT("0"));
	s_kf_label = new wxStaticText(frame, -1, wxT("Key frame:"), wxDefaultPosition, wxSize(120, -1));
	s_kf_field = new wxTextCtrl(frame, FIELD_SEQ_KEY_FRAME, wxT("0"));
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

	s_separator = new wxStaticLine(frame, -1, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	s_box2->Add(s_separator, 1, wxEXPAND);

	s_grid_box2 = new wxFlexGridSizer(6, 2, 4, 0);
	s_box2->Add(s_grid_box2, 0, wxEXPAND | wxALIGN_TOP);
	s_xfermode_label = new wxStaticText(frame, -1, wxT("Transfer mode:"), wxDefaultPosition, wxSize(140, -1));
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
	s_xfermode_menu = new wxChoice(frame, MENU_SEQ_XFER_MODE, wxDefaultPosition, wxDefaultSize, 22, xfermode_labels, 0);
	s_xferperiod_label = new wxStaticText(frame, -1, wxT("Transfer mode period:"), wxDefaultPosition, wxSize(150, -1));
	s_xferperiod_field = new wxTextCtrl(frame, FIELD_SEQ_XFER_MODE_PERIOD, wxT("1"));
	s_ffs_label = new wxStaticText(frame, -1, wxT("First frame sound:"), wxDefaultPosition, wxSize(150, -1));
	s_ffs_field = new wxTextCtrl(frame, FIELD_SEQ_FIRST_FRAME_SND, wxT("0"));
	s_kfs_label = new wxStaticText(frame, -1, wxT("Key frame sound:"), wxDefaultPosition, wxSize(150, -1));
	s_kfs_field = new wxTextCtrl(frame, FIELD_SEQ_KEY_FRAME_SND, wxT("0"));
	s_lfs_label = new wxStaticText(frame, -1, wxT("Last frame sound:"), wxDefaultPosition, wxSize(150, -1));
	s_lfs_field = new wxTextCtrl(frame, FIELD_SEQ_LAST_FRAME_SND, wxT("0"));
	s_sf_label = new wxStaticText(frame, -1, wxT("Shape scale factor:"), wxDefaultPosition, wxSize(150, -1));
	s_sf_field = new wxTextCtrl(frame, FIELD_SEQ_SCALE_FACTOR, wxT("1"));
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

	s_fb = new SequenceView(frame, wxID_ANY);
	s_outer_sizer->Add(s_fb, 1, wxEXPAND | wxALL, 5);
	mainbox->Show(s_outer_sizer, false);
	
	mainbox->Layout();
	frame->SetSizer(mainbox);

//    editor = new ShapesView(this, frame, wxPoint(0, 0), wxSize(900, 600));
	
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

void ShapesView::OnDraw(wxDC *WXUNUSED(dc) )
{
	wxLogDebug(wxT("Got OnDraw();"));
}

void ShapesView::OnUpdate(wxView *WXUNUSED(sender), wxObject *WXUNUSED(hint) )
{
	wxLogDebug(wxT("Got OnUpdate();"));
	// update all levels of the tree control
	
	for (unsigned int i = 0; i < COLLECTIONS_PER_FILE; i++) {
		// collection name nodes
		ShapesTreeItemData	*itemdata = new ShapesTreeItemData(i, -1, TREESECTION_COLLECTION);
		wxTreeItemId	coll = colltree->AppendItem(colltree->GetRootItem(), wxString(collnames[i], wxConvUTF8), -1, -1, itemdata);
		
		for (unsigned int j = 0; j < 2; j++) {
			// color version nodes
			ShapesTreeItemData	*id = new ShapesTreeItemData(i, j, TREESECTION_VERSION);
			wxString		label;
			
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
				wxTreeItemId	coll_b = colltree->AppendItem(coll2, wxT("Bitmaps"), -1, -1, id_b),
								coll_ct = colltree->AppendItem(coll2, wxT("Color tables"), -1, -1, id_ct),
								coll_f = colltree->AppendItem(coll2, wxT("Frames"), -1, -1, id_f),
								coll_s = colltree->AppendItem(coll2, wxT("Sequences"), -1, -1, id_s);
				
				for (unsigned int k = 0; k < ((ShapesDocument*)GetDocument())->CollectionSequenceCount(i, j); k++) {
					// sequence nodes
					ShapesSequence		*seq = ((ShapesDocument*)GetDocument())->GetSequence(i, j, k);
					wxString    	blabel;
					ShapesTreeItemData	*id_seq = new ShapesTreeItemData(i, j, TREESECTION_SEQUENCES, k);
					
					blabel << k;
					if (seq->Name().Length() > 0)
						blabel << wxT(" - ") << seq->Name();
					colltree->AppendItem(coll_s, blabel, -1, -1, id_seq);
				}
			}
		}
	}
	colltree->Expand(colltree->GetRootItem());
}

bool ShapesView::OnClose(bool deleteWindow)
{
	if (!GetDocument()->Close())
		return false;
	Activate(false);
	if (deleteWindow) {
		delete frame;
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

// handle the Edit->Delete command (which is context-sensitive)
void ShapesView::MenuEditDelete(wxCommandEvent &e)
{
	wxTreeItemId	selected_item = colltree->GetSelection();
	ShapesTreeItemData	*item_data = dynamic_cast<ShapesTreeItemData *>(colltree->GetItemData(selected_item));

	if (item_data != NULL) {
		int	selection;

		// what should we delete?
		switch (item_data->Section()) {
			case TREESECTION_BITMAPS:
				// delete selected bitmap if any
				selection = bb->GetSelection();
				if (selection > -1) {
					// the user interface update should be nearly identical to the TreeSelect case
					bb->Freeze();
					bb->Clear();	// FIXME just remove THAT bitmap!
					b_outer_sizer->Show(b_count_label, true);
					b_outer_sizer->Show(b_edit_box, false);
					b_view->SetBitmap(NULL);
					((ShapesDocument*)GetDocument())->DeleteBitmap(selected_coll, selected_vers, selection);

					unsigned int	bitmap_count = ((ShapesDocument*)GetDocument())->CollectionBitmapCount(selected_coll, selected_vers);

					for (unsigned int i = 0; i < bitmap_count; i++)
						bb->AddBitmap(((ShapesDocument*)GetDocument())->GetBitmap(selected_coll, selected_vers, i));
					bb->Thaw();

					wxString	count_string;
		
					count_string << bitmap_count << wxT(" bitmap");
					if (bitmap_count != 1)
						count_string << wxT("s");
					b_count_label->SetLabel(count_string);
					frame->Layout();
				}
				break;
			case TREESECTION_FRAMES:
				// delete selected frame if any
				selection = fb->GetSelection();
				if (selection > -1) {
					fb->Freeze();
					fb->Clear();    // FIXME just remove THAT frame
					f_outer_sizer->Show(f_count_label, true);
					f_outer_sizer->Show(f_edit_box, false);
					f_view->SetFrame(NULL);
					((ShapesDocument*)GetDocument())->DeleteFrame(selected_coll, selected_vers, selection);

					unsigned int	frame_count = ((ShapesDocument*)GetDocument())->CollectionFrameCount(selected_coll, selected_vers),
									bitmap_count = ((ShapesDocument*)GetDocument())->CollectionBitmapCount(selected_coll, selected_vers);

					for (unsigned int i = 0; i < bitmap_count; i++)
						fb->AddBitmap(((ShapesDocument*)GetDocument())->GetBitmap(selected_coll, selected_vers, i));
					for (unsigned int i = 0; i < frame_count; i++)
						fb->AddFrame(((ShapesDocument*)GetDocument())->GetFrame(selected_coll, selected_vers, i));
					fb->Thaw();

					wxString    count_string;

					count_string << frame_count << wxT(" frame");
					if (frame_count != 1)
						count_string << wxT("s");
					f_count_label->SetLabel(count_string);
					frame->Layout();
				}
				break;
			case TREESECTION_SEQUENCES:
				break;
		}
	}
}

void ShapesView::MenuViewTransparency(wxCommandEvent &e)
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

// color table menu handler
void ShapesView::MenuViewCT(wxCommandEvent &e)
{
	view_ct = e.GetId() - VIEW_MENU_COLORTABLE_0;

	ShapesColorTable	*ctp = ((ShapesDocument*)GetDocument())->GetColorTable(selected_coll, selected_vers, view_ct);

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

// TODO
void ShapesView::MenuShapesAddColorTable(wxCommandEvent &e)
{
	wxFileDialog	*dlg = new wxFileDialog(frame, wxT("Choose a GIMP color table"), 
					                                wxT(""), wxT(""), wxT("*"), wxOPEN);

	if (dlg->ShowModal() == wxID_OK) {
		wxString		filename = dlg->GetPath();
/*		std::ifstream	fstream(filename.mb_str(), std::ios::in);

		if (fstream.good()) {
			// TODO handle both GIMP and Photoshop palettes
			// if this is going to be the first color table,
			// then add every color. Otherwise, keep only the
			// first <color per table> colors (warn the user)
			fstream.close();
		} else {
			std::cerr << "Error loading palette " << filename.mb_str() << "\n";
		}*/
	}
	dlg->Destroy();
}

// prompt the user and save the selected color table, either in
// Gimp or PhotoShop format (depending on event id)
void ShapesView::MenuShapesSaveColorTable(wxCommandEvent &e)
{
	if (((ShapesDocument*)GetDocument()) != NULL && selected_coll != -1 && selected_vers != -1) {
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
/*				ShapesColorTable	*ct = ((ShapesDocument*)GetDocument())->GetColorTable(selected_coll, selected_vers, selection);
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
				}*/
			}
		}
	}
}

void ShapesView::MenuShapesAddBitmap(wxCommandEvent &e)
{
	if (((ShapesDocument*)GetDocument()) != NULL && selected_coll != -1 && selected_vers != -1) {
		wxFileDialog	*dlg = new wxFileDialog(frame, wxT("Choose a bitmap to add"),
													wxT(""), wxT(""), wxT("BMP files|*.bmp"), wxOPEN);

		if (dlg->ShowModal() == wxID_OK) {
			wxString	filename = dlg->GetPath();
			wxImage		img;

			if (img.LoadFile(filename)) {
				// we have the wxImage now. Encode it to a new bitmap
				/*ShapesBitmap		newbmp;
				ShapesColorTable	*ct = ((ShapesDocument*)GetDocument())->GetColorTable(selected_coll, selected_vers, view_ct);
				unsigned char	*srcpixels = img.GetData(),
								*src = srcpixels,
								*dst;

				wxBeginBusyCursor();
				newbmp.IsColumnOrdered() = newbmp.IsTransparent() = false;
				newbmp.width = img.GetWidth();
				newbmp.height = img.GetHeight();
				newbmp.bit_depth = 8;
				newbmp.BytesPerRow() = img.GetWidth();
				newbmp.pixels = new unsigned char[newbmp.width * newbmp.height];
				if (newbmp.pixels == NULL) {
					std::cerr << "Could not allocate new " << newbmp.width << "x" << newbmp.height << " bitmap\n";
					dlg->Destroy();
					return;
				}
				dst = newbmp.pixels;
				// automagically initialize bitmap flags
				if (((ShapesDocument*)GetDocument())->CollectionType(selected_coll, selected_vers) == _object_collection ||
						((ShapesDocument*)GetDocument())->CollectionType(selected_coll, selected_vers) == _scenery_collection) {
					newbmp.BytesPerRow() = -1;
					newbmp.IsColumnOrdered() = true;
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
						newbmp.IsTransparent() = true;	// guess the user will want transparency
				}
				((ShapesDocument*)GetDocument())->InsertBitmap(newbmp, selected_coll, selected_vers);

				// update the GUI
				unsigned int	bitmap_count = ((ShapesDocument*)GetDocument())->CollectionBitmapCount(selected_coll, selected_vers);
				ShapesBitmap		*pnewbitmap = ((ShapesDocument*)GetDocument())->GetBitmap(selected_coll, selected_vers, bitmap_count-1);
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
				wxMessageBox(errormsg, wxT("Error adding bitmap"), wxOK | wxICON_ERROR, this);*/
			}
		}
		dlg->Destroy();
	}
}

// save all bitmaps of the selected collection as separate files in a folder
void ShapesView::MenuShapesExportBitmaps(wxCommandEvent &e)
{
	wxString	dir = wxDirSelector(wxT("Select destination folder"));

	if (!dir.empty()) {
		for (unsigned int i = 0; i < ((ShapesDocument*)GetDocument())->CollectionBitmapCount(selected_coll, selected_vers); i++) {
			wxString	file_name = wxString::Format(wxT("bitmap%.3d.bmp"), i);
			wxImage		img = ShapesBitmapToImage(((ShapesDocument*)GetDocument())->GetBitmap(selected_coll, selected_vers, i),
										((ShapesDocument*)GetDocument())->GetColorTable(selected_coll, selected_vers, view_ct),
							!show_transparent_pixels);

			img.SaveFile(dir + wxT("/") + file_name, wxBITMAP_TYPE_BMP);
		}
	}
}

void ShapesView::MenuShapesNewFrame(wxCommandEvent &e)
{
	if (((ShapesDocument*)GetDocument()) != NULL && selected_coll != -1 && selected_vers != -1) {
		// append an empty frame
		ShapesFrame	*newframe = new ShapesFrame();
		newframe->SetScaleFactor(((ShapesDocument*)GetDocument())->CollectionScaleFactor(selected_coll, selected_vers));
		((ShapesDocument*)GetDocument())->InsertFrame(newframe, selected_coll, selected_vers);
		fb->AddFrame(((ShapesDocument*)GetDocument())->GetFrame(selected_coll, selected_vers, ((ShapesDocument*)GetDocument())->CollectionFrameCount(selected_coll, selected_vers)-1));
		// update frame count label
		unsigned int	frame_count = ((ShapesDocument*)GetDocument())->CollectionFrameCount(selected_coll, selected_vers);
		wxString		count_string = wxString::Format(wxT("%u frame"), frame_count);
		
		if (frame_count != 1)
			count_string << wxT("s");
		f_count_label->SetLabel(count_string);
	}
}

void ShapesView::MenuShapesNewSequence(wxCommandEvent &e)
{
	if (((ShapesDocument*)GetDocument()) != NULL && selected_coll != -1 && selected_vers != -1) {
		// append an empty sequence
		ShapesSequence		*newseq = new ShapesSequence();
		
		((ShapesDocument*)GetDocument())->InsertSequence(newseq, selected_coll, selected_vers);
		
		// insert the new entry in the main tree
		wxTreeItemId	thenode = GetSequencesTreeItem(selected_coll, selected_vers);
		int				seq_id = ((ShapesDocument*)GetDocument())->CollectionSequenceCount(selected_coll, selected_vers) - 1;
		ShapesSequence     *seq = ((ShapesDocument*)GetDocument())->GetSequence(selected_coll, selected_vers, seq_id);
		ShapesTreeItemData  *itemdata = new ShapesTreeItemData(selected_coll, selected_vers, TREESECTION_SEQUENCES, seq_id);
		wxString		label;

		label << seq_id;
		if (seq->Name().Length() > 0)
			label << wxT(" - ") << seq->Name();
		colltree->AppendItem(thenode, label, -1, -1, itemdata);
	}
}

// user selected a tree entry
void ShapesView::TreeSelect(wxTreeEvent &e)
{
	ShapesTreeItemData	*data = dynamic_cast<ShapesTreeItemData *>(colltree->GetItemData(e.GetItem()));

	if (data) {
		int	new_coll = data->CollID(),
			new_vers = data->Version(),
			new_section = data->Section();

		if (new_coll != selected_coll || new_vers != selected_vers) {
			// user has moved to another collection/version

			// first clear the user interface
			wxMenu			*colortables_submenu;
			menubar->FindItem(VIEW_MENU_COLORTABLE_0, &colortables_submenu);
			unsigned int	ctmenucount = colortables_submenu->GetMenuItemCount();

			view_ct = -1;
			ctb->Clear();
			bb->Freeze();
			bb->Clear();
			b_view->SetBitmap(NULL);
			fb->Freeze();
			fb->Clear();
			f_view->SetFrame(NULL);
			f_view->SetBitmap(NULL);
			s_fb->Clear();
			menubar->SetLabel(EDIT_MENU_DELETE, wxT("Delete"));
			menubar->Enable(EDIT_MENU_DELETE, false);
			for (unsigned int i = 0; i < ctmenucount; i++)
				menubar->Enable(VIEW_MENU_COLORTABLE_0 + i, false);
			menubar->Enable(SHAPES_MENU_SAVECOLORTABLE, false);
			menubar->Enable(SHAPES_MENU_SAVECOLORTABLETOPS, false);

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
					menubar->Enable(SHAPES_MENU_ADDBITMAP, true);
					menubar->Enable(SHAPES_MENU_EXPORTBITMAPS, true);
					menubar->Enable(SHAPES_MENU_ADDFRAME, true);
					menubar->Enable(SHAPES_MENU_ADDSEQUENCE, true);

					// chunk info panel
					chunk_version_field->SetValue(INT_TO_WXSTRING(((ShapesDocument*)GetDocument())->CollectionVersion(new_coll, new_vers)));
					chunk_type_menu->SetSelection(((ShapesDocument*)GetDocument())->CollectionType(new_coll, new_vers));
					chunk_flags_field->SetValue(INT_TO_WXSTRING(((ShapesDocument*)GetDocument())->CollectionFlags(new_coll, new_vers)));
					chunk_sf_field->SetValue(INT_TO_WXSTRING(((ShapesDocument*)GetDocument())->CollectionScaleFactor(new_coll, new_vers)));

					// color tables
					view_ct = 0;
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
					menubar->Enable(SHAPES_MENU_ADDBITMAP, false);
					menubar->Enable(SHAPES_MENU_EXPORTBITMAPS, false);
					menubar->Enable(SHAPES_MENU_ADDFRAME, false);
					menubar->Enable(SHAPES_MENU_ADDSEQUENCE, false);
				}
			} else {
				menubar->Enable(SHAPES_MENU_ADDBITMAP, false);
				menubar->Enable(SHAPES_MENU_EXPORTBITMAPS, false);
				menubar->Enable(SHAPES_MENU_ADDFRAME, false);
				menubar->Enable(SHAPES_MENU_ADDSEQUENCE, false);
			}
			selected_coll = new_coll;
			selected_vers = new_vers;
			bb->Thaw();
			fb->Thaw();
		}
	
		// handle sequence selection
		selected_sequence = data->Sequence();
		if (selected_sequence > -1) {
			ShapesSequence *seq = ((ShapesDocument*)GetDocument())->GetSequence(new_coll, new_vers, selected_sequence);

			// setup sequence panel controls
			s_outer_static_box->SetLabel(wxString::Format(wxT("Sequence %d of %u"),
									selected_sequence, ((ShapesDocument*)GetDocument())->CollectionSequenceCount(new_coll, new_vers)));
			s_name_field->SetValue(seq->Name());
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
									wxT("Warning"), wxOK | wxICON_ERROR, frame);
					break;
			}
			s_fpv_field->SetValue(INT_TO_WXSTRING(seq->FramesPerView()));
			s_tpf_field->SetValue(INT_TO_WXSTRING(seq->TicksPerFrame()));
			s_lf_field->SetValue(INT_TO_WXSTRING(seq->LoopFrame()));
			s_kf_field->SetValue(INT_TO_WXSTRING(seq->KeyFrame()));
			s_xfermode_menu->SetSelection(seq->TransferMode());
			s_xferperiod_field->SetValue(INT_TO_WXSTRING(seq->TransferModePeriod()));
			s_ffs_field->SetValue(INT_TO_WXSTRING(seq->FirstFrameSound()));
			s_kfs_field->SetValue(INT_TO_WXSTRING(seq->KeyFrameSound()));
			s_lfs_field->SetValue(INT_TO_WXSTRING(seq->LastFrameSound()));
			s_sf_field->SetValue(INT_TO_WXSTRING(seq->PixelsToWorld()));
			// setup the sequence view
			wxBeginBusyCursor();
			s_fb->Freeze();
			s_fb->Clear();
			s_fb->SetColorTable(((ShapesDocument*)GetDocument())->GetColorTable(new_coll, new_vers, view_ct));
			for (unsigned int i = 0; i < ((ShapesDocument*)GetDocument())->CollectionBitmapCount(selected_coll, selected_vers); i++)
				s_fb->AddBitmap(((ShapesDocument*)GetDocument())->GetBitmap(selected_coll, selected_vers, i));
			for (unsigned int i = 0; i < ((ShapesDocument*)GetDocument())->CollectionFrameCount(selected_coll, selected_vers); i++)
				s_fb->AddFrame(((ShapesDocument*)GetDocument())->GetFrame(selected_coll, selected_vers, i));
			s_fb->SetSeqParameters(seq->NumberOfViews(), seq->FramesPerView(), &seq->mFrameIndexes);
			s_fb->Thaw();
			menubar->SetLabel(EDIT_MENU_DELETE, wxT("Delete sequence"));
			menubar->Enable(EDIT_MENU_DELETE, true);
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
				chunk_inner_box->Show(chunk_undef_label, !((ShapesDocument*)GetDocument())->CollectionDefined(selected_coll, selected_vers));
				chunk_inner_box->Show(chunk_grid, ((ShapesDocument*)GetDocument())->CollectionDefined(selected_coll, selected_vers));
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
void ShapesView::BitmapSelect(wxCommandEvent &e)
{
	int	selection = e.GetInt();

	if (selection < 0) {
		// deselection
		b_view->SetBitmap(NULL);
		b_outer_sizer->Show(b_count_label, true);
		b_outer_sizer->Show(b_edit_box, false);
		menubar->SetLabel(EDIT_MENU_DELETE, wxT("Delete"));
		menubar->Enable(EDIT_MENU_DELETE, false);
	} else {
		ShapesBitmap	*sel_bitmap = ((ShapesDocument*)GetDocument())->GetBitmap(selected_coll, selected_vers, selection);
		
		// set labels
		wxString	info_label = wxString::Format(wxT("%dx%d pixels, "), sel_bitmap->Width(), sel_bitmap->Height());

		if (sel_bitmap->BytesPerRow() > 0)
			info_label << wxT("plain encoding");
		else
			info_label << wxT("RLE");
		b_info_label->SetLabel(info_label);
		b_edit_static_box->SetLabel(wxString::Format(wxT("Bitmap %d of %d"),
								selection, ((ShapesDocument*)GetDocument())->CollectionBitmapCount(selected_coll, selected_vers)));
		// set flag check boxes
		b_order_checkbox->SetValue(sel_bitmap->IsColumnOrdered());
		b_transparency_checkbox->SetValue(sel_bitmap->IsTransparent());
		// set bitmap view
		b_view->SetBitmap(sel_bitmap);
		b_outer_sizer->Show(b_count_label, false);
		b_outer_sizer->Show(b_edit_box, true);
		menubar->SetLabel(EDIT_MENU_DELETE, wxT("Delete bitmap"));
		menubar->Enable(EDIT_MENU_DELETE, true);
	}
	b_outer_sizer->Layout();
}

void ShapesView::BitmapDelete(wxCommandEvent &e)
{
	if (e.GetSelection() < 0)
		return;

	bb->Freeze();
	bb->Clear();    // FIXME just remove THAT bitmap!
	b_outer_sizer->Show(b_count_label, true);
	b_outer_sizer->Show(b_edit_box, false);
	b_view->SetBitmap(NULL);
	((ShapesDocument*)GetDocument())->DeleteBitmap(selected_coll, selected_vers, e.GetSelection());
	
	unsigned int    bitmap_count = ((ShapesDocument*)GetDocument())->CollectionBitmapCount(selected_coll, selected_vers);
	
	for (unsigned int i = 0; i < bitmap_count; i++)
		bb->AddBitmap(((ShapesDocument*)GetDocument())->GetBitmap(selected_coll, selected_vers, i));
	bb->Thaw();
	
	wxString    count_string;
	
	count_string << bitmap_count << wxT(" bitmap");
	if (bitmap_count != 1)
		count_string << wxT("s");
	b_count_label->SetLabel(count_string);
	frame->Layout();
}

void ShapesView::ToggleBitmapCheckboxes(wxCommandEvent &e)
{
	ShapesBitmap	*sel_bitmap = b_view->GetBitmap();

	if (sel_bitmap != NULL) {
		switch (e.GetId()) {
			case CB_COLUMN_ORDER:
				sel_bitmap->SetColumnOrdered(e.IsChecked());
				break;
			case CB_ENABLE_TRANSPARENCY:
				sel_bitmap->SetTransparent(e.IsChecked());
				bb->RebuildThumbnail(bb->GetSelection());
				bb->Refresh();
				b_view->SetBitmap(sel_bitmap);
				// FIXME also update the FrameBrowser and all that
				break;
		}
	}
}

void ShapesView::CTSelect(wxCommandEvent &e)
{
	int	selection = e.GetInt();

	if (selection < 0) {
		// deselection
		menubar->Enable(SHAPES_MENU_SAVECOLORTABLE, false);
		menubar->Enable(SHAPES_MENU_SAVECOLORTABLETOPS, false);
		menubar->SetLabel(EDIT_MENU_DELETE, wxT("Delete"));
		menubar->Enable(EDIT_MENU_DELETE, false);
	} else {
		// selection
		menubar->Enable(SHAPES_MENU_SAVECOLORTABLE, true);
		menubar->Enable(SHAPES_MENU_SAVECOLORTABLETOPS, true);
		menubar->SetLabel(EDIT_MENU_DELETE, wxT("Delete color table"));
		menubar->Enable(EDIT_MENU_DELETE, true);
	}
}

// selection event in the frame browser
void ShapesView::FrameSelect(wxCommandEvent &e)
{
	int selection = e.GetInt();

	if (selection < 0) {
		f_view->SetFrame(NULL);
		f_view->SetBitmap(NULL);
		f_outer_sizer->Show(f_count_label, true);
		f_outer_sizer->Show(f_edit_box, false);
		menubar->SetLabel(EDIT_MENU_DELETE, wxT("Delete"));
		menubar->Enable(EDIT_MENU_DELETE, false);
	} else {
		ShapesFrame	*sel_frame = ((ShapesDocument*)GetDocument())->GetFrame(selected_coll, selected_vers, selection);
		ShapesBitmap	*assoc_bitmap = NULL;

		// set labels
		f_edit_static_box->SetLabel(wxString::Format(wxT("Frame %d of %u"),
								selection, ((ShapesDocument*)GetDocument())->CollectionFrameCount(selected_coll, selected_vers)));
		// set frame view
		f_view->SetFrame(sel_frame);
		if (sel_frame->BitmapIndex() >= 0)
			assoc_bitmap = ((ShapesDocument*)GetDocument())->GetBitmap(selected_coll, selected_vers, sel_frame->BitmapIndex());
		f_view->SetBitmap(assoc_bitmap);
		// set controls
		f_bitmap_id->SetRange(-1, ((ShapesDocument*)GetDocument())->CollectionBitmapCount(selected_coll, selected_vers) - 1);
		f_bitmap_id->SetValue(sel_frame->BitmapIndex());
		f_xmirror_checkbox->SetValue(sel_frame->IsXmirrored());
		f_ymirror_checkbox->SetValue(sel_frame->IsYmirrored());
		f_keypoint_checkbox->SetValue(sel_frame->IsKeypointObscured());
		f_origin_x_field->SetValue(INT_TO_WXSTRING(sel_frame->OriginX()));
		f_origin_y_field->SetValue(INT_TO_WXSTRING(sel_frame->OriginY()));
		f_key_x_field->SetValue(INT_TO_WXSTRING(sel_frame->KeyX()));
		f_key_y_field->SetValue(INT_TO_WXSTRING(sel_frame->KeyY()));
		f_scalefactor_field->SetValue(INT_TO_WXSTRING(sel_frame->ScaleFactor()));
		f_mli_field->SetValue(INT_TO_WXSTRING((int)(sel_frame->MinimumLightIntensity() * 100.0)));
		f_outer_sizer->Show(f_count_label, false);
		f_outer_sizer->Show(f_edit_box, true);
		menubar->SetLabel(EDIT_MENU_DELETE, wxT("Delete frame"));
		menubar->Enable(EDIT_MENU_DELETE, true);
	}
	f_outer_sizer->Layout();
}

void ShapesView::FrameDelete(wxCommandEvent &e)
{
	if (e.GetSelection() < 0)
		return;
	
	fb->Freeze();
	fb->Clear();    // FIXME just remove THAT frame
	f_outer_sizer->Show(f_count_label, true);
	f_outer_sizer->Show(f_edit_box, false);
	f_view->SetFrame(NULL);
	((ShapesDocument*)GetDocument())->DeleteFrame(selected_coll, selected_vers, e.GetSelection());
	
	unsigned int	frame_count = ((ShapesDocument*)GetDocument())->CollectionFrameCount(selected_coll, selected_vers),
					bitmap_count = ((ShapesDocument*)GetDocument())->CollectionBitmapCount(selected_coll, selected_vers);
	
	for (unsigned int i = 0; i < bitmap_count; i++)
		fb->AddBitmap(((ShapesDocument*)GetDocument())->GetBitmap(selected_coll, selected_vers, i));
	for (unsigned int i = 0; i < frame_count; i++)
		fb->AddFrame(((ShapesDocument*)GetDocument())->GetFrame(selected_coll, selected_vers, i));
	fb->Thaw();

	wxString    count_string;

	count_string << frame_count << wxT(" frame");
	if (frame_count != 1)
		count_string << wxT("s");
	f_count_label->SetLabel(count_string);
	frame->Layout();

}

void ShapesView::AskSaveBitmap(wxCommandEvent &e)
{
	int	selected_bmp = bb->GetSelection();
	
	if (selected_bmp < 0)
		return;

	wxString	prompt = wxString::Format(wxT("Save bitmap %d as:"), selected_bmp),
				filename = wxString::Format(wxT("bitmap%d.bmp"), selected_bmp);

	wxFileDialog	*dlg = new wxFileDialog(frame, prompt, wxT(""), filename, wxT("*.bmp"), wxSAVE | wxOVERWRITE_PROMPT);

	if (dlg->ShowModal() == wxID_OK) {
		// FIXME create an 8-bit bitmap with the exact color table,
		// instead of wasting space with an RGB one
		wxImage	img = ShapesBitmapToImage(((ShapesDocument*)GetDocument())->GetBitmap(selected_coll, selected_vers, selected_bmp),
										((ShapesDocument*)GetDocument())->GetColorTable(selected_coll, selected_vers, view_ct),
										!show_transparent_pixels);

		img.SaveFile(dlg->GetPath(), wxBITMAP_TYPE_BMP);
	}
	dlg->Destroy();
}

// bitmap index change in the frame panel
void ShapesView::BitmapIndexSpin(wxSpinEvent &e)
{
	int			newid = e.GetPosition();
	ShapesFrame	*sel_frame = f_view->GetFrame();

	if (sel_frame != NULL) {
		sel_frame->SetBitmapIndex(newid);
		f_view->SetBitmap(((ShapesDocument*)GetDocument())->GetBitmap(selected_coll, selected_vers, newid));
		f_view->SetFrame(sel_frame);
		fb->RebuildThumbnail(fb->GetSelection());
		fb->Refresh();
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
						f_mli_field->SetValue(wxT("100"));
						v = 100;
					} else if (v < 0) {
						wxBell();
						f_mli_field->SetValue(wxT("0"));
						v = 0;
					}
					sel_frame->SetMinimumLightIntensity(v / 100.0);
					break;
			}
			// recalculate world_* fields if needed and possible
			if (recalculate_world_fields && sel_frame->BitmapIndex() >= 0
					&& sel_frame->BitmapIndex() < (int)((ShapesDocument*)GetDocument())->CollectionBitmapCount(selected_coll, selected_vers)) {
				ShapesBitmap	*assoc_bitmap = ((ShapesDocument*)GetDocument())->GetBitmap(selected_coll, selected_vers, sel_frame->BitmapIndex());
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
		}
	}
}

// "delete sequence" button click
void ShapesView::DeleteSequence(wxCommandEvent &e)
{
	// first delete the sequence for real
	((ShapesDocument*)GetDocument())->DeleteSequence(selected_coll, selected_vers, selected_sequence);

	// for updating the tree control we could just delete the selected
	// sequence item, but then ShapesTreeItemData structures associated
	// to the following items would be broken and everything would crash.
	// We could correct them, but reinserting all items is simpler.
	wxTreeItemId	seqnode = GetSequencesTreeItem(selected_coll, selected_vers);

	colltree->SelectItem(seqnode);
	colltree->DeleteChildren(seqnode);
	for (unsigned int k = 0; k < ((ShapesDocument*)GetDocument())->CollectionSequenceCount(selected_coll, selected_vers); k++) {
		ShapesSequence		*seq = ((ShapesDocument*)GetDocument())->GetSequence(selected_coll, selected_vers, k);
		wxString		label;
		ShapesTreeItemData	*seqdata = new ShapesTreeItemData(selected_coll, selected_vers, TREESECTION_SEQUENCES, k);

		label << k;
		if (seq->Name().Len() > 0)
			label << wxT(" - ") << seq->Name();
		colltree->AppendItem(seqnode, label, -1, -1, seqdata);
	}

	selected_sequence = -1;
}

// sequence type menu in the sequence editor
void ShapesView::EditSequenceType(wxCommandEvent &e)
{
	if (selected_sequence >= 0) {
		ShapesSequence	*sel_seq = ((ShapesDocument*)GetDocument())->GetSequence(selected_coll, selected_vers, selected_sequence);
		int			real_nov;

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
		// init frame indexes to all invalid frames
		// FIXME very annoying for the user, do better
		sel_seq->mFrameIndexes.clear();
		for (int i = 0; i < real_nov * sel_seq->FramesPerView(); i++)
			sel_seq->mFrameIndexes.push_back(-1);
		s_fb->SetSeqParameters(sel_seq->NumberOfViews(), sel_seq->FramesPerView(), &sel_seq->mFrameIndexes);
	}
}

// transfer mode menu in the sequence editor
void ShapesView::EditSequenceXferMode(wxCommandEvent &e)
{
	if (selected_sequence >= 0) {
		ShapesSequence	*sel_seq = ((ShapesDocument*)GetDocument())->GetSequence(selected_coll, selected_vers, selected_sequence);

		sel_seq->SetTransferMode(s_xfermode_menu->GetSelection());
	}
}

// user messed with fields in the sequence editor
void ShapesView::EditSequenceFields(wxCommandEvent &e)
{
	if (selected_sequence >= 0) {
		ShapesSequence	*sel_seq = ((ShapesDocument*)GetDocument())->GetSequence(selected_coll, selected_vers, selected_sequence);
		wxString	s = e.GetString();

		if (sel_seq != NULL) {
			if (e.GetId() == FIELD_SEQ_NAME) {
				sel_seq->SetName(s.Left(32));
				// update the tree item label
				wxTreeItemId		seqnode = GetSequencesTreeItem(selected_coll, selected_vers);
				wxTreeItemIdValue	cookie;
				wxTreeItemId		id = colltree->GetFirstChild(seqnode, cookie);
			
				while (id.IsOk()) {
					ShapesTreeItemData	*itemdata = dynamic_cast<ShapesTreeItemData *>(colltree->GetItemData(id));

					if (itemdata->Sequence() == selected_sequence) {
						// here we are
						wxString	blabel;

						blabel << selected_sequence;
						if (sel_seq->Name().Length() > 0)
							blabel << wxT(" - ") << sel_seq->Name();
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
								s_fb->SetSeqParameters(sel_seq->NumberOfViews(), v, &sel_seq->mFrameIndexes);
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
						case FIELD_SEQ_SCALE_FACTOR:
							sel_seq->SetPixelsToWorld(v);
							break;
					}
				}
			}
		}
	}
}


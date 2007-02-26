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

ShapesView::ShapesView(void): wxView()
{
	frame = (wxFrame *) NULL;
	/* textsw = (MyTextWindow *) NULL; */
}

ShapesView::~ShapesView(void)
{

}

bool ShapesView::OnCreate(wxDocument *doc, long WXUNUSED(flags) )
{
	wxString frameTitle = _T("ShapeFusion : Shapes");
    frame = wxGetApp().CreateChildFrame(doc, this, frameTitle, wxPoint(0, 0), wxSize(900, 600));
    
    int width, height;
	wxPoint point;
    frame->GetClientSize(&width, &height);
    
/*	mainbox = new wxBoxSizer(wxHORIZONTAL);
	// create the collection tree
	colltree = new wxTreeCtrl(frame, -1, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT);
	wxTreeItemId	treeroot = colltree->AddRoot(wxT("No file loaded"));
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

	mainbox->Layout();*/
	
	CreateViewMenu(frame->GetMenuBar());
	CreateShapesMenu(frame->GetMenuBar());
	
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

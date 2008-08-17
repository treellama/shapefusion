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

#ifndef SHAPESVIEW_H
#define SHAPESVIEW_H

#include "wx/docview.h"
#include "wx/treectrl.h"
#include "wx/spinctrl.h"
#include "wx/statline.h"
#include "../ShapeFusionApp.h"
#include "ShapesTreeItemData.h"
#include "CTBrowser.h"
#include "CTView.h"
#include "BitmapBrowser.h"
#include "BitmapView.h"
#include "FrameBrowser.h"
#include "FrameView.h"
#include "SequenceView.h"
#include "../ShapeFusionMenus.h"

class ShapesView: public wxView
{
    DECLARE_DYNAMIC_CLASS(ShapesView)
private:
	// control ids
	enum {
		// color tables
		CB_SELF_LUMINESCENT,
		BTN_GRADIENT,
		// bitmaps
		BITMAP_BROWSER,
		CB_COLUMN_ORDER,
		CB_ENABLE_TRANSPARENCY,
		// frames
		FRAME_BROWSER,
		FRAME_VIEW,
		FIELD_BITMAP_INDEX,
		CB_XMIRROR,
		CB_YMIRROR,
		CB_KEYPOINT,
		FIELD_ORIGIN_X,
		FIELD_ORIGIN_Y,
		FIELD_KEY_X,
		FIELD_KEY_Y,
		FIELD_FRAME_SCALEFACTOR,
		FIELD_MIN_LIGHT_INT,
		// sequences
		FIELD_SEQ_NAME,
		BTN_DELETE_SEQ,
		MENU_SEQ_TYPE,
		FIELD_SEQ_NUMBER_OF_VIEWS,
		FIELD_SEQ_FRAMES_PER_VIEW,
		FIELD_SEQ_TICKS_PER_FRAME,
		FIELD_SEQ_LOOP_FRAME,
		FIELD_SEQ_KEY_FRAME,
		MENU_SEQ_XFER_MODE,
		FIELD_SEQ_XFER_MODE_PERIOD,
		FIELD_SEQ_FIRST_FRAME_SND,
		FIELD_SEQ_KEY_FRAME_SND,
		FIELD_SEQ_LAST_FRAME_SND,
	};

	wxBoxSizer			*mainbox;
	wxMenuBar			*menubar;
	wxTreeCtrl			*colltree;
	wxBoxSizer			*dummy_sizer;
	// widgets for collection info
	wxBoxSizer			*coll_sizer;
	wxStaticBox			*coll_static_box;
	wxStaticBoxSizer	*coll_inner_box;
	wxStaticText		*coll_text;
	// widgets for chunk info
	wxBoxSizer			*chunk_sizer;
	wxStaticBox			*chunk_static_box;
	wxStaticBoxSizer	*chunk_inner_box;
	wxStaticText			*chunk_undef_label;
	wxFlexGridSizer			*chunk_grid;
	wxStaticText				*chunk_version_label,
								*chunk_type_label,
								*chunk_flags_label,
								*chunk_sf_label;
	wxTextCtrl					*chunk_version_field,
								*chunk_flags_field,
								*chunk_sf_field;
	wxChoice					*chunk_type_menu;
	// widgets for color tables section
	wxBoxSizer			*ct_outer_sizer;
	CTBrowser			*ctb;
	wxStaticText		*ct_count_label;
	wxStaticBox			*ct_edit_static_box;
	wxStaticBoxSizer	*ct_edit_box;
	CTView					*ct_view;
	wxBoxSizer				*ct_inner_edit_box;
	wxCheckBox					*ct_self_lumin_checkbox;
	wxButton					*ct_gradient_button;
	// widgets for bitmaps section
	wxBoxSizer			*b_outer_sizer,
						*b_edit_inner_box;
	BitmapBrowser		*bb;
	wxStaticText		*b_count_label;
	wxStaticBox			*b_edit_static_box;
	wxStaticBoxSizer	*b_edit_box;
	wxCheckBox			*b_order_checkbox,
						*b_transparency_checkbox;
	wxStaticText		*b_info_label;
	BitmapView			*b_view;
	// widgets for frames section
	wxBoxSizer			*f_outer_sizer,
						*f_edit_inner_box;
	FrameBrowser		*fb;
	wxStaticText		*f_count_label;
	wxStaticBox			*f_edit_static_box;
	wxStaticBoxSizer	*f_edit_box;
	wxStaticText		*f_bitmap_label;
	wxSpinCtrl			*f_bitmap_id;
	wxTextCtrl			*f_origin_x_field,
						*f_origin_y_field,
						*f_key_x_field,
						*f_key_y_field,
						*f_scalefactor_field;
	wxCheckBox			*f_xmirror_checkbox,
						*f_ymirror_checkbox,
						*f_keypoint_checkbox;
	FrameView			*f_view;
	wxFlexGridSizer		*f_origin_box;
	wxStaticText		*f_origin_x_label,
						*f_origin_y_label,
						*f_key_x_label,
						*f_key_y_label,
						*f_scalefactor_label,
						*f_mli_label,
						*f_w_left_label,
						*f_w_right_label,
						*f_w_top_label,
						*f_w_bottom_label,
						*f_w_x0_label,
						*f_w_y0_label;
	wxTextCtrl			*f_mli_field,
						*f_w_left_field,
						*f_w_right_field,
						*f_w_top_field,
						*f_w_bottom_field,
						*f_w_x0_field,
						*f_w_y0_field;
	// widgets for sequences section
	wxStaticBoxSizer	*s_outer_sizer;
	wxStaticBox			*s_outer_static_box;
	wxBoxSizer			*s_box1;
	wxStaticText			*s_name_label;
	wxTextCtrl				*s_name_field;
	wxButton				*s_delete_button;
	wxBoxSizer			*s_box2;
	wxFlexGridSizer			*s_grid_box;
	wxStaticText				*s_type_label;
	wxChoice					*s_type_menu;
	wxStaticText				*s_fpv_label;
	wxTextCtrl					*s_fpv_field;
	wxStaticText        		*s_tpf_label;
	wxTextCtrl          		*s_tpf_field;
	wxStaticText				*s_lf_label;
	wxTextCtrl					*s_lf_field;
	wxStaticText				*s_kf_label;
	wxTextCtrl					*s_kf_field;
	wxStaticLine			*s_separator;
	wxFlexGridSizer			*s_grid_box2;
	wxStaticText				*s_xfermode_label;
	wxChoice					*s_xfermode_menu;
	wxStaticText				*s_xferperiod_label;
	wxTextCtrl					*s_xferperiod_field;
	wxStaticText				*s_ffs_label,
								*s_kfs_label,
								*s_lfs_label;
	wxTextCtrl					*s_ffs_field,
								*s_kfs_field,
								*s_lfs_field;
	SequenceView		*s_fb;

	int		mSelectedColl,
			mSelectedVers,
			mSelectedSequence,
			mViewColorTable;
    wxFrame	*mFrame;
	
protected:
	DECLARE_EVENT_TABLE();

public:
	ShapesView();
	~ShapesView(void);
	
	bool OnCreate(wxDocument *doc, long flags);
	void OnDraw(wxDC *dc);
	void OnUpdate(wxView *sender, wxObject *hint = (wxObject *) NULL);
	bool OnClose(bool deleteWindow = true);
	wxTreeItemId GetSequencesTreeItem(unsigned int collection, unsigned int version) const;
	// menu event callbacks
	void MenuFileOpen(wxCommandEvent &e);
	void MenuFileSave(wxCommandEvent&);
	void MenuFileQuit(wxCommandEvent &e);
	void MenuEditDelete(wxCommandEvent &e);
	void MenuViewCT(wxCommandEvent &e);
	void MenuViewTNSize(wxCommandEvent &e);
	void MenuViewTransparency(wxCommandEvent &e);
	void MenuViewCenterOrigin(wxCommandEvent &e);
	void MenuShapesAddColorTable(wxCommandEvent &e);
	void MenuShapesSaveColorTable(wxCommandEvent &e);
	void MenuShapesSaveColorTableToPS(wxCommandEvent &e);
	void MenuShapesAddBitmap(wxCommandEvent &e);
	void MenuShapesExportBitmap(wxCommandEvent &e);
	void MenuShapesExportBitmapMask(wxCommandEvent &e);
	void MenuShapesExportBitmaps(wxCommandEvent &e);
	void MenuShapesExportBitmapMasks(wxCommandEvent &e);
	void MenuShapesNewFrame(wxCommandEvent &e);
	void MenuShapesNewSequence(wxCommandEvent &e);
	// control callbacks
	void OnTreeSelect(wxTreeEvent &e);
	void OnBitmapSelect(wxCommandEvent &e);
	void BitmapDelete(wxCommandEvent &e);
	void OnCTSelect(wxCommandEvent &e);
	void CTColorSelect(wxCommandEvent &e);
	void CTColorChanged(wxCommandEvent &e);
	void ToggleSelfLuminCheckbox(wxCommandEvent &e);
	void MakeCTGradient(wxCommandEvent &e);
	void ToggleBitmapCheckboxes(wxCommandEvent &e);
	void OnFrameSelect(wxCommandEvent &e);
	void FrameDelete(wxCommandEvent &e);
	void OnFramePointDrag(wxCommandEvent &e);
	void BitmapIndexSpin(wxSpinEvent &e);
	void ToggleFrameCheckboxes(wxCommandEvent &e);
	void EditFrameFields(wxCommandEvent &e);
	void DeleteSequence(wxCommandEvent &e);
	void EditSequenceType(wxCommandEvent &e);
	void EditSequenceXferMode(wxCommandEvent &e);
	void EditSequenceFields(wxCommandEvent &e);

	void DoDeleteColorTable(int which);
	void DoDeleteBitmap(int which);
	void DoDeleteFrame(int which);
};

#endif

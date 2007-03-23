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

#ifndef __DRAWVIEW_H__
#define __DRAWVIEW_H__

#include "wx/docview.h"
#include "SoundsDocument.h"

class SoundsView: public wxView
{
    DECLARE_DYNAMIC_CLASS(SoundsView)
private:
	// control ids
	enum {
		SOUND_CLASS_MENU,
//		SOUND_LIST_ADD_BUTTON,
//		SOUND_LIST_REMOVE_BUTTON,
		SOURCE_RADIO_BOX,
		SOUND_BEHAVIOR_MENU,
		SOUND_CHANCE_MENU,
		SOUND_FLAGS_CHECKBOX,
		SOUND_FLAGS_RESTART,
		SOUND_FLAGS_ABORT,
		SOUND_FLAGS_RESIST,
		SOUND_FLAGS_CHANGE,
		SOUND_FLAGS_OBSTRUCTED,
		SOUND_FLAGS_MOBSTRUCTED,
		SOUND_FLAGS_AMBIENT,
		SOUND_LOW_PITCH_SLIDER,
		SOUND_HIGH_PITCH_SLIDER,
		PERMUTATION_LIST_BOX,
//		PERMUTATION_IMPORT_BUTTON,
//		PERMUTATION_EXPORT_BUTTON
	};
	
	wxBoxSizer		*frame_sizer;
	wxFlexGridSizer		*sound_class_sizer;
	wxStaticText			*sound_class_text;
	wxChoice				*sound_class_menu;
	wxRadioBox				*source_radio_box;
	wxStaticBox			*editor_static_box;
	wxStaticBoxSizer	*editor_static_sizer;
	wxBoxSizer				*top_editor_sizer;
	wxBoxSizer					*left_editor_sizer;
	wxListBox						*permutation_list_box;
	wxGridSizer						*left_editor_menus_sizer;
	wxStaticText						*behavior_text;
	wxChoice								*behavior_menu;
	wxStaticText						*chance_text;
	wxChoice							*chance_menu;
	wxBoxSizer					*right_editor_sizer;
//	wxCheckListBox					*flags_check_boxes;
	wxCheckBox						*flag_restart_checkbox;
	wxCheckBox						*flag_abort_checkbox;
	wxCheckBox						*flag_resist_checkbox;
	wxCheckBox						*flag_change_checkbox;
	wxCheckBox						*flag_obstructed_checkbox;
	wxCheckBox						*flag_mobstructed_checkbox;
	wxCheckBox						*flag_ambient_checkbox;
	wxGridSizer				*bottom_editor_sizer;
	wxStaticText				*low_pitch_text;
	wxSlider					*low_pitch_slider;
	wxStaticText				*high_pitch_text;
	wxSlider					*high_pitch_slider;

    wxFrame			*frame;
	wxMenuBar		*menubar;
	SoundsDocument	*payload;
public:
    
    SoundsView(void) { frame = (wxFrame *) NULL; };
    ~SoundsView(void) {};
    
    bool OnCreate(wxDocument *doc, long flags);
    void OnDraw(wxDC *dc);
    void OnUpdate(wxView *sender, wxObject *hint = (wxObject *) NULL);
    bool OnClose(bool deleteWindow = true);
	
	bool Update(void);
	
	void SoundClassMenuChanged(wxCommandEvent &e);
    
	void AddSound(wxCommandEvent &e);
	void RemoveSound(wxCommandEvent &e);
	void SourceSelected(wxCommandEvent &e);
	void BehaviorChanged(wxCommandEvent &e);
	void ChanceChanged(wxCommandEvent &e);
	void FlagsChanged(wxCommandEvent &e);
	void LowPitchChanged(wxScrollEvent &e);
	void HighPitchChanged(wxScrollEvent &e);
	void PermutationSelected(wxCommandEvent &e);
	void ImportSound(wxCommandEvent &e);
	void ExportSound(wxCommandEvent &e);
    DECLARE_EVENT_TABLE()
};
#endif

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
		SOUND_CLASS_ID_FIELD,
		SOUND_CLASS_NUMBER_FIELD,
		SOUND_CLASS_LIST,
		SOUND_FLAGS_RESTART,
		SOUND_FLAGS_ABORT,
		SOUND_FLAGS_RESIST,
		SOUND_FLAGS_CHANGE,
		SOUND_FLAGS_OBSTRUCTED,
		SOUND_FLAGS_MOBSTRUCTED,
		SOUND_FLAGS_AMBIENT,
		SOUND_VOLUME_RADIO_BUTTON,
		SOUND_CHANCE_MENU,
		SOUND_LOW_PITCH_FIELD,
		SOUND_HIGH_PITCH_FIELD,
		SOUND_EIGHT_BIT_PERMUTATIONS_LIST,
		SOUND_SIXTEEN_BIT_PERMUTATIONS_LIST,
		SOUND_REMAP_CHECK_BOX
		
	};

	wxBoxSizer		*frame_sizer;
	wxSizer				*sound_class_sizer;
	wxSizer					*sound_class_header_sizer;
	wxStaticText				*sound_class_text;
	wxStaticText				*sound_class_id_text;
	wxTextCtrl					*sound_class_id_field;
	wxStaticText				*sound_class_number_text;
	wxStaticText				*sound_class_number_field;
	wxListBox				*sound_class_list;
	wxSizer				*sound_editor_sizer;
	wxSizer					*sound_flags_sizer;
	wxCheckBox					*sound_flag_restart_checkbox;
	wxCheckBox					*sound_flag_abort_checkbox;
	wxCheckBox					*sound_flag_resist_checkbox;
	wxCheckBox					*sound_flag_change_checkbox;
	wxCheckBox					*sound_flag_obstructed_checkbox;
	wxCheckBox					*sound_flag_mobstructed_checkbox;
	wxCheckBox					*sound_flag_ambient_checkbox;
	wxRadioBox				*sound_volume_radio_button;
	wxSizer					*sound_menus_sizer;
	wxStaticText				*sound_chance_text;
	wxChoice					*sound_chance_menu;
	wxStaticText				*sound_low_pitch_text;
	wxTextCtrl					*sound_low_pitch_field;
	wxStaticText				*sound_high_pitch_text;
	wxTextCtrl					*sound_high_pitch_field;
	wxSizer					*sound_permutation_sizer;
	wxSizer						*sound_eight_bit_sizer;
	wxStaticText					*sound_eight_bit_text;
	wxListBox						*sound_eight_bit_list;
	wxSizer						*sound_sixteen_bit_sizer;
	wxStaticText					*sound_sixteen_bit_text;
	wxListBox						*sound_sixteen_bit_list;
	wxCheckBox						*sound_remap_check_box;

    wxFrame			*frame;
	wxMenuBar		*menubar;
	SoundsDocument	*payload;
	int				mSoundClass,
					mSoundSource,
					mSoundPermutation;
	
public:
    
    SoundsView(void);
    ~SoundsView(void) {};
    
    bool OnCreate(wxDocument *doc, long flags);
    void OnDraw(wxDC *dc);
    void OnUpdate(wxView *sender, wxObject *hint = (wxObject *) NULL);
    bool OnClose(bool deleteWindow = true);
	
	void Update(void);
	
	
	void SoundClassChanged(wxCommandEvent &e);
	void AddSound(wxCommandEvent &e);
	void RemoveSound(wxCommandEvent &e);
	void SourceRadioButtonChanged(wxCommandEvent &e);
	void VolumeButtonChanged(wxCommandEvent &e);
	void ChanceMenuChanged(wxCommandEvent &e);
	void FlagsChanged(wxCommandEvent &e);
	void LowPitchValueChanged(wxScrollEvent &e);
	void HighPitchValueChanged(wxScrollEvent &e);
	void SoundPermutationSelected(wxCommandEvent &e);
	
	// Menu events
	void MenuDelete(wxCommandEvent &e);
	void MenuAddSoundClass(wxCommandEvent &e);
	void MenuImportSound(wxCommandEvent &e);
	void MenuExportSound(wxCommandEvent &e);
	
	void ExportSound(wxString filepath);
	
protected:
    DECLARE_EVENT_TABLE()
};
#endif

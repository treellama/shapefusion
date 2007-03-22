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
		SOUND_LIST_BOX,
		SOUND_LIST_ADD_BUTTON,
		SOUND_LIST_REMOVE_BUTTON,
		SOURCE_RADIO_BOX,
		SOUND_BEHAVIOR_MENU,
		SOUND_CHANCE_MENU,
		SOUND_FLAGS_BOX,
		SOUND_LOW_PITCH_SLIDER,
		SOUND_HIGH_PITCH_SLIDER,
		PERMUTATION_LIST_BOX
	};
	
	wxBoxSizer			*frame_sizer;
	wxBoxSizer				*sound_list_sizer;
	wxListBox					*sound_list;
	wxBoxSizer					*sound_list_button_sizer;
	wxButton						*sound_list_add_button;
	wxButton						*sound_list_remove_button;
	wxBoxSizer				*editor_sizer;
	wxStaticText				*source_static_text;
	wxRadioBox					*source_radio_box;
	wxBoxSizer					*sound_menu_sizer;
	wxStaticText					*sound_behavior_label;
	wxChoice						*sound_behavior_menu;
	wxStaticText					*sound_chance_label;
	wxChoice						*sound_chance_menu;
	wxRadioBox					*sound_flags_box;
	wxSlider					*sound_low_pitch_slider;
	wxSlider					*sound_high_pitch_slider;
	wxStaticText				*permutation_static_text;
	wxListBox					*permutation_list_box;

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
	
	void SoundSelected(wxCommandEvent &e);
    
	void AddSound(wxCommandEvent &e);
	void RemoveSound(wxCommandEvent &e);
	void SourceSelected(wxCommandEvent &e);
	void BehaviorChanged(wxCommandEvent &e);
	void ChanceChanged(wxCommandEvent &e);
	void FlagsChanged(wxCommandEvent &e);
	void LowPitchChanged(wxScrollEvent &e);
	void HighPitchChanged(wxScrollEvent &e);
	void PermutationSelected(wxCommandEvent &e);
    DECLARE_EVENT_TABLE()
};
#endif

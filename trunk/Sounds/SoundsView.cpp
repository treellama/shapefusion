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

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/wfstream.h"

#include "../ShapeFusionApp.h"
#include "../ShapeFusionMenus.h"
#include "SoundsView.h"

BEGIN_EVENT_TABLE(SoundsView, wxView)
	EVT_LISTBOX(SOUND_CLASS_LIST, SoundsView::SoundClassChanged)
	EVT_RADIOBOX(SOUND_VOLUME_RADIO_BUTTON, SoundsView::VolumeButtonChanged)
	EVT_CHOICE(SOUND_CHANCE_MENU, SoundsView::ChanceMenuChanged)
	EVT_CHECKBOX(SOUND_FLAGS_RESTART, SoundsView::FlagsChanged)
	EVT_CHECKBOX(SOUND_FLAGS_ABORT, SoundsView::FlagsChanged)
	EVT_CHECKBOX(SOUND_FLAGS_RESIST, SoundsView::FlagsChanged)
	EVT_CHECKBOX(SOUND_FLAGS_CHANGE, SoundsView::FlagsChanged)
	EVT_CHECKBOX(SOUND_FLAGS_OBSTRUCTED, SoundsView::FlagsChanged)
	EVT_CHECKBOX(SOUND_FLAGS_MOBSTRUCTED, SoundsView::FlagsChanged)
	EVT_CHECKBOX(SOUND_FLAGS_AMBIENT, SoundsView::FlagsChanged)
	EVT_LISTBOX(SOUND_EIGHT_BIT_PERMUTATIONS_LIST, SoundsView::SoundPermutationSelected)
	EVT_LISTBOX(SOUND_SIXTEEN_BIT_PERMUTATIONS_LIST, SoundsView::SoundPermutationSelected)
	EVT_LISTBOX_DCLICK(SOUND_EIGHT_BIT_PERMUTATIONS_LIST, SoundsView::SoundPermutationDoubleClicked)
	EVT_LISTBOX_DCLICK(SOUND_SIXTEEN_BIT_PERMUTATIONS_LIST, SoundsView::SoundPermutationDoubleClicked)
	EVT_MENU(EDIT_MENU_DELETE, SoundsView::MenuDelete)
	EVT_MENU(SOUNDS_MENU_ADDCLASS, SoundsView::MenuAddSoundClass)
	EVT_MENU(SOUNDS_MENU_EXPORT, SoundsView::MenuExportSound)
	EVT_MENU(SOUNDS_MENU_IMPORT, SoundsView::MenuImportSound)
END_EVENT_TABLE()

/*char *randomsndnames[] = {	"Water",
							"Sewage",
							"Lava",
							"Goo",
							"Under Media",
							"Wind",
							"Waterfall",
							"Siren",
							"Fan",
							"S'pht Door",
							"S'pht Platform",
							"Heavy S'pht Door",
							"Heavy S'pht Platform",
							"Light Machinery",
							"Heavy Machinery",
							"Transformer",
							"Sparking Transformer",
							"Machine Binder",
							"Machine Bookpress",
							"Machine Puncher",
							"Electric",
							"Alarm",
							"Night Wind",
							"Pfhor Door",
							"Pfhor Platform",
							"Alien Noise 1",
							"Alien Noise 2",
							"Alien Harmonics" };
							
char *ambientsndnames[] = {	"Water Drip",
							"Surface Explosion",
							"Underground Explosion",
							"Owl",
							"Creak" };*/

IMPLEMENT_DYNAMIC_CLASS(SoundsView, wxView)

SoundsView::SoundsView() : mSoundClass(wxNOT_FOUND), mSoundSource(wxNOT_FOUND), mSoundPermutation(wxNOT_FOUND)
{
	frame = NULL;
	menubar = NULL;
	payload = NULL;
}

// What to do when a view is created. Creates actual
// windows for displaying the view.
bool SoundsView::OnCreate(wxDocument *doc, long WXUNUSED(flags) )
{
	wxString frameTitle = _T("ShapeFusion : Sounds : ");
	frameTitle.Append(doc->GetFilename());
	
    frame = wxGetApp().CreateChildFrame(doc, this, frameTitle, wxPoint(0, 0), wxSize(600, 400), wxDEFAULT_FRAME_STYLE);// & ~ (wxRESIZE_BORDER | wxRESIZE_BOX | wxMAXIMIZE_BOX));
	
	payload = (SoundsDocument*)doc;
	
	menubar = frame->GetMenuBar();
	
	CreateSoundsMenu(menubar);
	
	// Because we can always add sound classes
	menubar->Enable(SOUNDS_MENU_ADDCLASS, true);
		
	wxString volume_labels[] = { wxT("Soft"), wxT("Medium"), wxT("Loud") };
	wxString chances_labels[] = { wxT("100%"), wxT("90%"), wxT("80%"), wxT("70%"), wxT("60%"), wxT("50%"), wxT("40%"), wxT("30%"), wxT("20%"), wxT("10%") };
	
	sound_class_text = new wxStaticText(frame, wxID_ANY, wxT("Sound Classes: "));
	sound_class_id_text = new wxStaticText(frame, wxID_ANY, wxT("Class ID: "));
	sound_class_id_field = new wxTextCtrl(frame, SOUND_CLASS_ID_FIELD, wxT(""));
	
	sound_class_number_text = new wxStaticText(frame, wxID_ANY, wxT("Class Number: "));
	sound_class_number_field = new wxStaticText(frame, SOUND_CLASS_NUMBER_FIELD, wxT(""));
	
	sound_class_list = new wxListBox(frame, SOUND_CLASS_LIST, wxDefaultPosition, wxDefaultSize, 0, NULL);
	
	sound_flag_restart_checkbox = new wxCheckBox(frame, SOUND_FLAGS_RESTART, wxT("Cannot be restarted"));
	sound_flag_abort_checkbox = new wxCheckBox(frame, SOUND_FLAGS_ABORT, wxT("Does not self-abort"));
	sound_flag_resist_checkbox = new wxCheckBox(frame, SOUND_FLAGS_RESIST, wxT("Resists pitch changes"));
	sound_flag_change_checkbox = new wxCheckBox(frame, SOUND_FLAGS_CHANGE, wxT("Can't change pitch"));
	sound_flag_obstructed_checkbox = new wxCheckBox(frame, SOUND_FLAGS_OBSTRUCTED, wxT("Can't be obstructed"));
	sound_flag_mobstructed_checkbox = new wxCheckBox(frame, SOUND_FLAGS_MOBSTRUCTED, wxT("Can't be media obstructed"));
	sound_flag_ambient_checkbox = new wxCheckBox(frame, SOUND_FLAGS_AMBIENT, wxT("Is ambient"));
	
	sound_volume_radio_button = new wxRadioBox(frame, SOUND_VOLUME_RADIO_BUTTON, wxT("Volume"), wxDefaultPosition, wxDefaultSize, 3, volume_labels, 3, wxRA_SPECIFY_COLS);
	
	sound_chance_text = new wxStaticText(frame, wxID_ANY, wxT("Chance: "));
	sound_chance_menu = new wxChoice(frame, SOUND_CHANCE_MENU, wxDefaultPosition, wxDefaultSize, 10, chances_labels);
	
	sound_low_pitch_text = new wxStaticText(frame, wxID_ANY, wxT("Low Pitch: "));
	sound_low_pitch_field = new wxTextCtrl(frame, SOUND_LOW_PITCH_FIELD);
	sound_high_pitch_text = new wxStaticText(frame, wxID_ANY, wxT("High Pitch: "));
	sound_high_pitch_field = new wxTextCtrl(frame, SOUND_HIGH_PITCH_FIELD);
	
	sound_eight_bit_text = new wxStaticText(frame, wxID_ANY, wxT("8-bit Sounds:"));
	sound_eight_bit_list = new wxListBox(frame, SOUND_EIGHT_BIT_PERMUTATIONS_LIST, wxDefaultPosition, wxDefaultSize, 0, NULL);
	
	sound_sixteen_bit_text = new wxStaticText(frame, wxID_ANY, wxT("16-bit Sounds: "));
	sound_sixteen_bit_list = new wxListBox(frame, SOUND_SIXTEEN_BIT_PERMUTATIONS_LIST, wxDefaultPosition, wxDefaultSize, 0, NULL);
	
	sound_remap_check_box = new wxCheckBox(frame, SOUND_REMAP_CHECK_BOX, wxT("Remap 8-bit"));
	
	frame_sizer = new wxBoxSizer(wxHORIZONTAL);
	sound_class_sizer = new wxBoxSizer(wxVERTICAL);
	sound_class_header_sizer = new wxFlexGridSizer(2, 2);
	sound_editor_sizer = new wxBoxSizer(wxVERTICAL);
	sound_flags_sizer = new wxStaticBoxSizer(wxVERTICAL, frame, wxT("Flags"));
	sound_menus_sizer = new wxFlexGridSizer(2, 3);
	sound_permutation_sizer = new wxBoxSizer(wxHORIZONTAL);
	sound_eight_bit_sizer = new wxBoxSizer(wxVERTICAL);
	sound_sixteen_bit_sizer = new wxBoxSizer(wxVERTICAL);
	
	sound_class_header_sizer->Add(sound_class_id_text, 0, wxALIGN_CENTER_VERTICAL, 0);
	sound_class_header_sizer->Add(sound_class_id_field, 0, 0, 0);
	sound_class_header_sizer->Add(sound_class_number_text, 0, wxALIGN_CENTER_VERTICAL, 0);
	sound_class_header_sizer->Add(sound_class_number_field, 0, 0, 0);
	
	sound_class_sizer->Add(sound_class_text, 0, 0, 0);
	sound_class_sizer->Add(sound_class_header_sizer, 0, 0, 0);
	sound_class_sizer->Add(sound_class_list, 1, wxEXPAND, 0);
	
	sound_flags_sizer->Add(sound_flag_restart_checkbox, 0, 0, 0);
	sound_flags_sizer->Add(sound_flag_abort_checkbox, 0, 0, 0);
	sound_flags_sizer->Add(sound_flag_resist_checkbox, 0, 0, 0);
	sound_flags_sizer->Add(sound_flag_change_checkbox, 0, 0, 0);
	sound_flags_sizer->Add(sound_flag_obstructed_checkbox, 0, 0, 0);
	sound_flags_sizer->Add(sound_flag_mobstructed_checkbox, 0, 0, 0);
	sound_flags_sizer->Add(sound_flag_ambient_checkbox, 0, 0, 0);
	
	sound_menus_sizer->Add(sound_chance_text, 0, wxALIGN_CENTER_VERTICAL, 0);
	sound_menus_sizer->Add(sound_chance_menu, 0, 0, 0);
	sound_menus_sizer->Add(sound_low_pitch_text, 0, wxALIGN_CENTER_VERTICAL, 0);
	sound_menus_sizer->Add(sound_low_pitch_field, 0, 0, 0);
	sound_menus_sizer->Add(sound_high_pitch_text, 0, wxALIGN_CENTER_VERTICAL, 0);
	sound_menus_sizer->Add(sound_high_pitch_field, 0, 0, 0);
	
	sound_eight_bit_sizer->Add(sound_eight_bit_text, 0, 0, 0);
	sound_eight_bit_sizer->Add(sound_eight_bit_list, 1, wxEXPAND | wxRIGHT, 5);
	sound_sixteen_bit_sizer->Add(sound_sixteen_bit_text, 0, 0, 0);
	sound_sixteen_bit_sizer->Add(sound_sixteen_bit_list, 1, wxEXPAND, 0);
	sound_sixteen_bit_sizer->Add(sound_remap_check_box, 0, 0, 0);
	
	sound_permutation_sizer->Add(sound_eight_bit_sizer, 1, wxEXPAND, 0);
	sound_permutation_sizer->Add(sound_sixteen_bit_sizer, 1, wxEXPAND, 0);
	
	sound_editor_sizer->Add(sound_flags_sizer, 0, 0, 0);
	sound_editor_sizer->AddSpacer(1);
	sound_editor_sizer->Add(sound_volume_radio_button, 0, 0, 0);
	sound_editor_sizer->AddSpacer(1);
	sound_editor_sizer->Add(sound_menus_sizer, 0, 0, 0);
	sound_editor_sizer->AddSpacer(5);
	sound_editor_sizer->Add(sound_permutation_sizer, 1, wxEXPAND, 0);
	
	frame_sizer->Add(sound_class_sizer, 0, wxEXPAND | wxALL, 5);
	frame_sizer->AddSpacer(5);
	frame_sizer->Add(sound_editor_sizer, 0, wxEXPAND | wxALL, 5);
	
	frame->SetSizer(frame_sizer);
	frame_sizer->Layout();
	frame_sizer->SetSizeHints(frame);
	
	frame->Show(true);
#ifdef __X__
	// X seems to require a forced resize
	int x, y;
	frame->GetSize(&x, &y);
	frame->SetSize(wxDefaultCoord, wxDefaultCoord, x, y);
#endif
	
	return true;
}

// Sneakily gets used for default print/preview
// as well as drawing on the screen.
void SoundsView::OnDraw(wxDC *dc)
{

}

void SoundsView::OnUpdate(wxView *WXUNUSED(sender), wxObject *WXUNUSED(hint))
{
	sound_class_list->Clear();
	
	bool gequals = true;
	
	for (unsigned int i = 0; i < payload->GetSoundCount() ; i++) {
		sound_class_list->Append(wxString::Format(wxT("Sound %d"), i));
		// We check if there is a difference between 8-bit and 16-bit
		// SoundsDefinitions
		bool equals = payload->Get8BitSoundDefinition(i)->HaveSameAttributesAs(*payload->Get16BitSoundDefinition(i));
		if (!equals)
			wxLogDebug(wxT("Sound source different at %d"), i);
		gequals = gequals && equals;
	}
	
	if (!gequals) {
		// FIXME : Update this when we have a "complete" editor...
		wxMessageDialog msg(frame,
						wxT("It seems 8-bit and 16-bit versions of some of this Sound file "
						"sounds have differences. This editor will replace 16-bit sounds "
						"flags with those from 8-bit sounds, to ensure consistency. "
						"If you really need to be able to change 16-bit flags independently, "
						"please file a feature request."),
						wxT("Warning !"), wxOK | wxICON_WARNING);
						
		msg.ShowModal();
	}
	
	Update();
}

void SoundsView::Update(void)
{
	// We disable our menuitems, in case selection is invalid 
	menubar->Enable(SOUNDS_MENU_IMPORT, false);
	menubar->Enable(SOUNDS_MENU_EXPORT, false);
	menubar->Enable(EDIT_MENU_DELETE, false);
	
	if (sound_class_list->GetCount() == 0) {
		// There is no sound class
		// We cannot have a selection
		mSoundPermutation = wxNOT_FOUND;
		mSoundSource = wxNOT_FOUND;
	} else {
		// We have a sound class
		
		// Make sure we always have something selected
		mSoundClass = sound_class_list->GetSelection();
		if (mSoundClass == wxNOT_FOUND) {
			wxLogDebug(wxT("[SoundsView] There is no sound selected. Selecting first item..."));
			sound_class_list->SetSelection(0);
			mSoundClass = sound_class_list->GetSelection();
		}
		
		// We build the permutations listbox
		sound_eight_bit_list->Clear();
		sound_sixteen_bit_list->Clear();
		
		SoundsDefinition *def = payload->Get8BitSoundDefinition(mSoundClass);
		if (def) {
			for (unsigned int i = 0; i < def->GetPermutationCount(); i++) {
				sound_eight_bit_list->Append(wxString::Format(wxT("%d"), def->GetPermutation(i)->Size()));
			}
		}
		
		def = payload->Get16BitSoundDefinition(mSoundClass);
		if (def) {
			for (unsigned int i = 0; i < def->GetPermutationCount(); i++) {
				sound_sixteen_bit_list->Append(wxString::Format(wxT("%d"), def->GetPermutation(i)->Size()));
			}
		}
		
		// As soon as we have a sound class selected, we can
		// - import a sound into it
		// - delete it
		menubar->Enable(SOUNDS_MENU_IMPORT, true);
		menubar->Enable(EDIT_MENU_DELETE, true);
		
		if (payload->Get8BitSoundDefinition(mSoundClass)->GetPermutationCount() != 0) {
			// There is 8-bit sounds, we select first
			mSoundSource = 0;
			mSoundPermutation = 0;
			sound_eight_bit_list->SetSelection(0);
			// We deselect 16-bit list
			sound_sixteen_bit_list->SetSelection(wxNOT_FOUND);
		} else {
			// There is no 8-bit sounds
			if (payload->Get16BitSoundDefinition(mSoundClass)->GetPermutationCount() != 0) {
				// We have 16-bit sounds, we select this one...
				mSoundSource = 1;
				mSoundPermutation = 0;
				sound_sixteen_bit_list->SetSelection(0);
				// We deselect 8-bit list
				sound_eight_bit_list->SetSelection(wxNOT_FOUND);
			} else {
				// There is neither 8-bit nor 16-bit sounds, we bail...
				return;
			}
		}
		
		// We enable this, our selection is valid...
		menubar->Enable(SOUNDS_MENU_EXPORT, true);
		
		def = payload->Get8BitSoundDefinition(mSoundClass);
				
		sound_class_number_field->SetLabel(wxString::Format(wxT("%d"), mSoundClass));
		sound_class_id_field->SetValue(wxString::Format(wxT("%d"), def->GetSoundCode()));
		
		sound_volume_radio_button->SetSelection(def->GetBehaviorIndex());
		sound_chance_menu->SetSelection(def->GetChance());
		
		sound_flag_restart_checkbox->SetValue(def->IsNotRestartable());
		sound_flag_abort_checkbox->SetValue(def->IsNotSelfAbortable());
		sound_flag_resist_checkbox->SetValue(def->IsPitchChangeResistant());
		sound_flag_change_checkbox->SetValue(def->IsNotPitchChangeable());
		sound_flag_obstructed_checkbox->SetValue(def->IsNotObstructed());
		sound_flag_mobstructed_checkbox->SetValue(def->IsNotMediaObstructed());
		sound_flag_ambient_checkbox->SetValue(def->IsAmbient());

#if ((wxMAJOR_VERSION < 2) || (wxMAJOR_VERSION == 2 && wxMINOR_VERSION < 7))
	#warning You better use a newer version of wxWidgets
		sound_low_pitch_field->SetValue(wxString::Format(wxT("%g"), def->GetLowPitch()));
		sound_high_pitch_field->SetValue(wxString::Format(wxT("%g"), def->GetHighPitch()));
#else
		sound_low_pitch_field->ChangeValue(wxString::Format(wxT("%g"), def->GetLowPitch()));
		sound_high_pitch_field->ChangeValue(wxString::Format(wxT("%g"), def->GetHighPitch()));
#endif
	}
}

// Clean up windows used for displaying the view.
bool SoundsView::OnClose(bool deleteWindow)
{
    if (!GetDocument()->Close())
        return false;
    
    SetFrame((wxFrame *) NULL);
    
    Activate(false);
    
    if (deleteWindow) {
        delete frame;
        return true;
    }
    return true;
}

void SoundsView::SoundClassChanged(wxCommandEvent &e)
{
	Update();
}

void SoundsView::VolumeButtonChanged(wxCommandEvent &e)
{
	SoundsDefinition *def = payload->Get8BitSoundDefinition(mSoundClass);
	def->SetBehaviorIndex(sound_volume_radio_button->GetSelection());
	
	def = payload->Get16BitSoundDefinition(mSoundClass);
	def->SetBehaviorIndex(sound_volume_radio_button->GetSelection());
}

void SoundsView::ChanceMenuChanged(wxCommandEvent &e)
{
	SoundsDefinition *def = payload->Get8BitSoundDefinition(mSoundClass);
	def->SetChance(sound_chance_menu->GetSelection());
	
	def = payload->Get16BitSoundDefinition(mSoundClass);
	def->SetChance(sound_chance_menu->GetSelection());
}

void SoundsView::FlagsChanged(wxCommandEvent &e)
{
	SoundsDefinition *def8 = payload->Get8BitSoundDefinition(mSoundClass);
	SoundsDefinition *def16 = payload->Get16BitSoundDefinition(mSoundClass);
	switch (e.GetId()) {
		case SOUND_FLAGS_RESTART:
			def8->SetNotRestartable(e.IsChecked());
			def16->SetNotRestartable(e.IsChecked());
			break;
		
		case SOUND_FLAGS_ABORT:
			def8->SetNotSelfAbortable(e.IsChecked());
			def16->SetNotSelfAbortable(e.IsChecked());
			break;
		
		case SOUND_FLAGS_RESIST:
			def8->SetPitchChangeResistant(e.IsChecked());
			def16->SetPitchChangeResistant(e.IsChecked());
			break;
		
		case SOUND_FLAGS_CHANGE:
			def8->SetNotPitchChangeable(e.IsChecked());
			def16->SetNotPitchChangeable(e.IsChecked());
			break;
		
		case SOUND_FLAGS_OBSTRUCTED:
			def8->SetNotObstructed(e.IsChecked());
			def16->SetNotObstructed(e.IsChecked());
			break;
		
		case SOUND_FLAGS_MOBSTRUCTED:
			def8->SetNotMediaObstructed(e.IsChecked());
			def16->SetNotMediaObstructed(e.IsChecked());
			break;
		
		case SOUND_FLAGS_AMBIENT:
			def8->SetAmbient(e.IsChecked());
			def16->SetAmbient(e.IsChecked());
			break;
		
		default:
			wxLogDebug(wxT("Invalid control id in FlagsChanged"));
			break;
	}
}

void SoundsView::LowPitchValueChanged(wxScrollEvent &e)
{

	long int l;
	sound_low_pitch_field->GetValue().ToLong(&l);
	
	SoundsDefinition *def = payload->Get8BitSoundDefinition(mSoundClass);
	def->SetLowPitch(l);
	
	def = payload->Get16BitSoundDefinition(mSoundClass);
	def->SetLowPitch(l);
}

void SoundsView::HighPitchValueChanged(wxScrollEvent &e)
{
	long int l;
	sound_high_pitch_field->GetValue().ToLong(&l);
	
	SoundsDefinition *def = payload->Get8BitSoundDefinition(mSoundClass);
	def->SetHighPitch(l);
	
	def = payload->Get16BitSoundDefinition(mSoundClass);
	def->SetHighPitch(l);
}

void SoundsView::MenuDelete(wxCommandEvent &e)
{
	wxWindow *win = sound_class_list->FindFocus();
	
	switch (win->GetId()) {
		case SOUND_CLASS_LIST:
			wxLogDebug(wxT("Delete Sound Class"));
			break;
		
		case SOUND_EIGHT_BIT_PERMUTATIONS_LIST:
			wxLogDebug(wxT("Delete 8-bit sound"));
			break;
			
		case SOUND_SIXTEEN_BIT_PERMUTATIONS_LIST:
			wxLogDebug(wxT("Delete 16-bit sound"));
			break;
		
		default:
			break;
	}
}

void SoundsView::MenuAddSoundClass(wxCommandEvent &e)
{
	wxLogDebug(wxT("Adding an item"));
	payload->AddSoundDefinition();
	
	// We add the new Sound class item by hand
	sound_class_list->Append(wxString::Format(wxT("Sound %d"), sound_class_list->GetCount()));
	
	Update();
}

void SoundsView::MenuImportSound(wxCommandEvent &e)
{
	
}

void SoundsView::MenuExportSound(wxCommandEvent &e)
{
	if (mSoundClass == wxNOT_FOUND || mSoundSource == wxNOT_FOUND || mSoundPermutation == wxNOT_FOUND) {
		wxMessageDialog msg(frame, wxT("Sorry, you need to select a sound class and a permutation to export a sound"), wxT("Error : No selection"), wxOK | wxICON_EXCLAMATION);
		msg.ShowModal();
		return;
	}
	
	wxFileDialog dlg(frame, wxT("Choose a file name :"), wxT(""), wxString::Format(wxT("Sound %d-%d.wav"), mSoundClass, mSoundPermutation), wxT("WAV files (*.wav)|*.wav|AIFF files (*.aif)|*.aif"), wxSAVE | wxOVERWRITE_PROMPT);

	if (dlg.ShowModal() == wxID_OK) {
		SoundsDefinition	*def = payload->GetSoundDefinition(mSoundSource, mSoundClass);
		AppleSoundHeader	*sound = def->GetPermutation(mSoundPermutation);
		bool				result = false;

		switch (dlg.GetFilterIndex()) {
			case 0: // Selected *.wav
				result = sound->SaveToWave(dlg.GetPath());
				break;
			case 1: // Selected *.aif
				result = sound->SaveToAiff(dlg.GetPath());
				break;
			default:
				break;
		}

		if (!result)
			wxLogDebug(wxT("[SoundsView] Error exporting sound"));
	}
}

void SoundsView::SoundPermutationSelected(wxCommandEvent &e)
{
	// We unselect the other permutation field
	if (e.GetId() == SOUND_EIGHT_BIT_PERMUTATIONS_LIST) {
		wxLogDebug(wxT("Selected 8-bit"));
		sound_sixteen_bit_list->SetSelection(wxNOT_FOUND);
		mSoundSource = 0;
		mSoundPermutation = sound_eight_bit_list->GetSelection();
	} else if (e.GetId() == SOUND_SIXTEEN_BIT_PERMUTATIONS_LIST) {
		wxLogDebug(wxT("Selected 16-bit"));
		sound_eight_bit_list->SetSelection(wxNOT_FOUND);
		mSoundSource = 1;
		mSoundPermutation = sound_sixteen_bit_list->GetSelection();
	}
}

void SoundsView::SoundPermutationDoubleClicked(wxCommandEvent &e)
{
	SoundsDefinition *def = payload->GetSoundDefinition(mSoundSource, mSoundClass);
	def->GetPermutation(mSoundPermutation)->PlaySound();
}

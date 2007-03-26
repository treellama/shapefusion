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

#include "ShapeFusionApp.h"
#include "SoundsView.h"
#include "SoundsDocument.h"

#define MAX_SLIDER_VALUE 300

BEGIN_EVENT_TABLE(SoundsView, wxView)
	EVT_CHOICE(SOUND_CLASS_MENU, SoundsView::SoundClassMenuChanged)
//	EVT_BUTTON(SOUND_LIST_ADD_BUTTON, SoundsView::AddSound)
//	EVT_BUTTON(SOUND_LIST_REMOVE_BUTTON, SoundsView::RemoveSound)
	EVT_RADIOBOX(SOURCE_RADIO_BOX, SoundsView::SourceRadioButtonChanged)
	EVT_CHOICE(SOUND_BEHAVIOR_MENU, SoundsView::BehaviorMenuChanged)
	EVT_CHOICE(SOUND_CHANCE_MENU, SoundsView::ChanceMenuChanged)
//	EVT_RADIOBOX(SOUND_FLAGS_BOX, SoundsView::FlagsChanged)
	EVT_CHECKBOX(SOUND_FLAGS_RESTART, SoundsView::FlagsChanged)
	EVT_CHECKBOX(SOUND_FLAGS_ABORT, SoundsView::FlagsChanged)
	EVT_CHECKBOX(SOUND_FLAGS_RESIST, SoundsView::FlagsChanged)
	EVT_CHECKBOX(SOUND_FLAGS_CHANGE, SoundsView::FlagsChanged)
	EVT_CHECKBOX(SOUND_FLAGS_OBSTRUCTED, SoundsView::FlagsChanged)
	EVT_CHECKBOX(SOUND_FLAGS_MOBSTRUCTED, SoundsView::FlagsChanged)
	EVT_CHECKBOX(SOUND_FLAGS_AMBIENT, SoundsView::FlagsChanged)
	EVT_COMMAND_SCROLL_THUMBRELEASE(SOUND_LOW_PITCH_SLIDER, SoundsView::LowPitchSliderChanged)
	EVT_COMMAND_SCROLL_THUMBRELEASE(SOUND_HIGH_PITCH_SLIDER, SoundsView::HighPitchSliderChanged)
	EVT_LISTBOX(PERMUTATION_LIST_BOX, SoundsView::PermutationSelected)
//	EVT_BUTTON(PERMUTATION_IMPORT_BUTTON, SoundsView::ImportSound)
//	EVT_BUTTON(PERMUTATION_EXPORT_BUTTON, SoundsView::ExportSound)
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

// What to do when a view is created. Creates actual
// windows for displaying the view.
bool SoundsView::OnCreate(wxDocument *doc, long WXUNUSED(flags) )
{
	wxString frameTitle = _T("ShapeFusion : Sounds : ");
	frameTitle.Append(doc->GetFilename());
	
    frame = wxGetApp().CreateChildFrame(doc, this, frameTitle, wxPoint(0, 0), wxSize(600, 400), wxDEFAULT_FRAME_STYLE & ~ (wxRESIZE_BORDER | wxRESIZE_BOX | wxMAXIMIZE_BOX));
	
	menubar = frame->GetMenuBar();

	payload = (SoundsDocument*)doc;

	
	// We create our gui
	
	wxString sources_types[] = { wxT("8-bit"), wxT("16-bit") };
	wxString behavior_labels[] = { wxT("Quiet"), wxT("Normal"), wxT("Loud") };
	wxString chances_labels[] = { wxT("100%"), wxT("90%"), wxT("80%"), wxT("70%"), wxT("60%"), wxT("50%"), wxT("40%"), wxT("30%"), wxT("20%"), wxT("10%") };
//	wxString flags_labels[] = { wxT("Cannot be restarted"), wxT("Does not self-abort"), wxT("Resists pitch changes"), wxT("Can't change pitch"), wxT("Can't be obstructed"), wxT("Can't be media obstructed"), wxT("Is ambient") };
	
	sound_class_text = new wxStaticText(frame, -1, wxT("Sound class : "));
	sound_class_menu = new wxChoice(frame, SOUND_CLASS_MENU, wxDefaultPosition, wxDefaultSize, 0, NULL);
	
	source_radio_box = new wxRadioBox(frame, SOURCE_RADIO_BOX, wxT("Sound source : "), wxDefaultPosition, wxDefaultSize, 2, sources_types, 2, wxRA_SPECIFY_COLS);
	
	editor_static_box = new wxStaticBox(frame, -1, wxEmptyString);
	
	permutation_list_box = new wxListBox(frame, PERMUTATION_LIST_BOX, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE | wxLB_ALWAYS_SB);
	behavior_text = new wxStaticText(frame, -1, wxT("Behavior : "));
	behavior_menu = new wxChoice(frame, SOUND_BEHAVIOR_MENU, wxDefaultPosition, wxDefaultSize, 3, behavior_labels);
	chance_text = new wxStaticText(frame, -1, wxT("Chance : "));
	chance_menu = new wxChoice(frame, SOUND_CHANCE_MENU, wxDefaultPosition, wxDefaultSize, 10, chances_labels);
	
//	flags_check_boxes = new wxCheckListBox(frame, SOUND_FLAGS_CHECKBOX, wxDefaultPosition, wxDefaultSize, 7, flags_labels);

	flag_restart_checkbox = new wxCheckBox(frame, SOUND_FLAGS_RESTART, wxT("Cannot be restarted"));
	flag_abort_checkbox = new wxCheckBox(frame, SOUND_FLAGS_ABORT, wxT("Does not self-abort"));
	flag_resist_checkbox = new wxCheckBox(frame, SOUND_FLAGS_RESIST, wxT("Resists pitch changes"));
	flag_change_checkbox = new wxCheckBox(frame, SOUND_FLAGS_CHANGE, wxT("Can't change pitch"));
	flag_obstructed_checkbox = new wxCheckBox(frame, SOUND_FLAGS_OBSTRUCTED, wxT("Can't be obstructed"));
	flag_mobstructed_checkbox = new wxCheckBox(frame, SOUND_FLAGS_MOBSTRUCTED, wxT("Can't be media obstructed"));
	flag_ambient_checkbox = new wxCheckBox(frame, SOUND_FLAGS_AMBIENT, wxT("Is ambient"));
	
	low_pitch_text = new wxStaticText(frame, -1, wxT("Low Pitch : "));
	low_pitch_slider = new wxSlider(frame, SOUND_LOW_PITCH_SLIDER, 0, 0, 100);
	high_pitch_text = new wxStaticText(frame, -1, wxT("High Pitch : "));
	high_pitch_slider = new wxSlider(frame, SOUND_HIGH_PITCH_SLIDER, 0, 0, 100);
	
	
	frame_sizer = new wxBoxSizer(wxVERTICAL);
	sound_class_sizer = new wxFlexGridSizer(2, 2);
	editor_static_sizer = new wxStaticBoxSizer(editor_static_box, wxVERTICAL);
	top_editor_sizer = new wxBoxSizer(wxHORIZONTAL);
	left_editor_sizer = new wxBoxSizer(wxVERTICAL);
	left_editor_menus_sizer = new wxGridSizer(2, 2);
	right_editor_sizer = new wxBoxSizer(wxVERTICAL);
	bottom_editor_sizer = new wxFlexGridSizer(2, 2);
	
	sound_class_sizer->Add(sound_class_text, 1, wxEXPAND, 0);
	sound_class_sizer->Add(sound_class_menu, 1, wxEXPAND, 0);
	sound_class_sizer->Add(source_radio_box, 1, 0, 0);
	
	left_editor_menus_sizer->Add(behavior_text, 0, 0, 0);
	left_editor_menus_sizer->Add(behavior_menu, 0, wxEXPAND, 0);
	left_editor_menus_sizer->Add(chance_text, 0, 0, 0);
	left_editor_menus_sizer->Add(chance_menu, 0, wxEXPAND, 0);
	
	left_editor_sizer->Add(permutation_list_box, 0, wxEXPAND, 0);
	left_editor_sizer->Add(left_editor_menus_sizer, 0, wxEXPAND, 0);
	
	right_editor_sizer->Add(flag_restart_checkbox, 0, 0, 0);
	right_editor_sizer->Add(flag_abort_checkbox, 0, 0, 0);
	right_editor_sizer->Add(flag_resist_checkbox, 0, 0, 0);
	right_editor_sizer->Add(flag_change_checkbox, 0, 0, 0);
	right_editor_sizer->Add(flag_obstructed_checkbox, 0, 0, 0);
	right_editor_sizer->Add(flag_mobstructed_checkbox, 0, 0, 0);
	right_editor_sizer->Add(flag_ambient_checkbox, 0, 0, 0);
	
	top_editor_sizer->Add(left_editor_sizer, 1, wxEXPAND, 0);
	top_editor_sizer->AddSpacer(10);
	top_editor_sizer->Add(right_editor_sizer, 1, wxEXPAND, 0);
	
	bottom_editor_sizer->Add(low_pitch_text, 0, 0, 0);
	bottom_editor_sizer->Add(low_pitch_slider, 1, wxEXPAND, 0);
	bottom_editor_sizer->Add(high_pitch_text, 0, 0, 0);
	bottom_editor_sizer->Add(high_pitch_slider, 1, wxEXPAND, 0);
	
	editor_static_sizer->Add(top_editor_sizer, 0, wxEXPAND, 0);
	editor_static_sizer->AddSpacer(10);
	editor_static_sizer->Add(bottom_editor_sizer, 1, wxEXPAND, 0);
	
	frame_sizer->Add(sound_class_sizer, 1, wxEXPAND | wxALL, 5);
	frame_sizer->Add(editor_static_sizer, 0, wxEXPAND | wxALL, 5);
	
	frame->SetSizer(frame_sizer);
	frame_sizer->Layout();
	frame_sizer->SetSizeHints(frame);
	
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
	sound_class_menu->Clear();
	
	for (unsigned int i = 0; i < payload->GetSoundCount() ; i++) {
		sound_class_menu->Append(wxString::Format(wxT("Sound %d"), i));
	}
	
	//FIXME
	int sound_index = sound_class_menu->GetSelection(); 
	if (sound_index == wxNOT_FOUND) {
		wxLogDebug(wxT("[SoundsView] There is no sound selected. Selecting first item..."));
		sound_class_menu->SetSelection(0);
	}
	
	int source_index = source_radio_box->GetSelection();
	if (source_index == wxNOT_FOUND) {
		wxLogDebug(wxT("[SoundsView] There is no source selected. Selecting first item..."));
		source_radio_box->SetSelection(0);
	}
	
	Update();
	
	frame->Show(true);
}

bool SoundsView::Update(void)
{
	mSoundIndex = sound_class_menu->GetSelection();
	mSourceIndex = source_radio_box->GetSelection();
	
	if (mSoundIndex == wxNOT_FOUND || mSourceIndex == wxNOT_FOUND)
		return false;
	
	SoundsDefinition *def = payload->GetSoundDefinition(mSourceIndex, mSoundIndex);
	
	behavior_menu->SetSelection(def->GetBehaviorIndex());
	chance_menu->SetSelection(def->GetChance());
	
	flag_restart_checkbox->SetValue(def->IsNotRestartable());
	flag_abort_checkbox->SetValue(def->IsNotSelfAbortable());
	flag_resist_checkbox->SetValue(def->IsPitchChangeResistant());
	flag_change_checkbox->SetValue(def->IsNotPitchChangeable());
	flag_obstructed_checkbox->SetValue(def->IsNotObstructed());
	flag_mobstructed_checkbox->SetValue(def->IsNotMediaObstructed());
	flag_ambient_checkbox->SetValue(def->IsAmbient());
	
	low_pitch_slider->SetValue((int)(def->GetLowPitch() * MAX_SLIDER_VALUE));
	high_pitch_slider->SetValue((int)(def->GetHighPitch() * MAX_SLIDER_VALUE));
	
	permutation_list_box->Clear();
	for (unsigned int i = 0; i < def->GetPermutationCount(); i++) {
		permutation_list_box->Append(wxString::Format(wxT("Permutation %d"), i));
	}
	
	return true;
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

void SoundsView::AddSound(wxCommandEvent &e)
{

}

void SoundsView::RemoveSound(wxCommandEvent &e)
{

}

void SoundsView::SoundClassMenuChanged(wxCommandEvent &e)
{
	Update();
}


void SoundsView::SourceRadioButtonChanged(wxCommandEvent &e)
{
	Update();
}

void SoundsView::BehaviorMenuChanged(wxCommandEvent &e)
{
	SoundsDefinition *def = payload->GetSoundDefinition(mSourceIndex, mSoundIndex);
	
	def->SetBehaviorIndex(behavior_menu->GetSelection());
}

void SoundsView::ChanceMenuChanged(wxCommandEvent &e)
{
	SoundsDefinition *def = payload->GetSoundDefinition(mSourceIndex, mSoundIndex);
	
	def->SetChance(chance_menu->GetSelection());
}

void SoundsView::FlagsChanged(wxCommandEvent &e)
{
	SoundsDefinition *def = payload->GetSoundDefinition(mSourceIndex, mSoundIndex);
	switch (e.GetId()) {
		case SOUND_FLAGS_RESTART:
			def->SetNotRestartable(e.IsChecked());
			break;
		
		case SOUND_FLAGS_ABORT:
			def->SetNotSelfAbortable(e.IsChecked());
			break;
		
		case SOUND_FLAGS_RESIST:
			def->SetPitchChangeResistant(e.IsChecked());
			break;
		
		case SOUND_FLAGS_CHANGE:
			def->SetNotPitchChangeable(e.IsChecked());
			break;
		
		case SOUND_FLAGS_OBSTRUCTED:
			def->SetNotObstructed(e.IsChecked());
			break;
		
		case SOUND_FLAGS_MOBSTRUCTED:
			def->SetNotMediaObstructed(e.IsChecked());
			break;
		
		case SOUND_FLAGS_AMBIENT:
			def->SetAmbient(e.IsChecked());
			break;
		
		default:
			wxLogDebug(wxT("Invalid control id in FlagsChanged"));
			break;
	}
}

void SoundsView::LowPitchSliderChanged(wxScrollEvent &e)
{
	SoundsDefinition *def = payload->GetSoundDefinition(mSourceIndex, mSoundIndex);
	
	def->SetLowPitch(low_pitch_slider->GetValue());
}

void SoundsView::HighPitchSliderChanged(wxScrollEvent &e)
{
	SoundsDefinition *def = payload->GetSoundDefinition(mSourceIndex, mSoundIndex);
	
	def->SetHighPitch(high_pitch_slider->GetValue());
}

void SoundsView::PermutationSelected(wxCommandEvent &e)
{}

void SoundsView::ImportSound(wxCommandEvent &e)
{
	
}

void SoundsView::ExportSound(wxCommandEvent &e)
{
	
}

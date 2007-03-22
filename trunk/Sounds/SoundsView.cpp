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
	EVT_LISTBOX(SOUND_LIST_BOX, SoundsView::SoundSelected)
	EVT_BUTTON(SOUND_LIST_ADD_BUTTON, SoundsView::AddSound)
	EVT_BUTTON(SOUND_LIST_REMOVE_BUTTON, SoundsView::RemoveSound)
	EVT_RADIOBOX(SOURCE_RADIO_BOX, SoundsView::SourceSelected)
	EVT_CHOICE(SOUND_BEHAVIOR_MENU, SoundsView::BehaviorChanged)
	EVT_CHOICE(SOUND_CHANCE_MENU, SoundsView::ChanceChanged)
	EVT_RADIOBOX(SOUND_FLAGS_BOX, SoundsView::FlagsChanged)
	EVT_COMMAND_SCROLL_THUMBRELEASE(SOUND_LOW_PITCH_SLIDER, SoundsView::LowPitchChanged)
	EVT_COMMAND_SCROLL_THUMBRELEASE(SOUND_HIGH_PITCH_SLIDER, SoundsView::HighPitchChanged)
	EVT_LISTBOX(PERMUTATION_LIST_BOX, SoundsView::PermutationSelected)
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
	
    frame = wxGetApp().CreateChildFrame(doc, this, frameTitle, wxPoint(0, 0), wxSize(600, 400));
	
	menubar = frame->GetMenuBar();

	payload = (SoundsDocument*)doc;

	frame_sizer = new wxBoxSizer(wxHORIZONTAL);
	
	// sound list section
	sound_list_sizer = new wxBoxSizer(wxVERTICAL);
	
	sound_list = new wxListBox(frame, SOUND_LIST_BOX, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE | wxLB_ALWAYS_SB);
	
	sound_list_button_sizer = new wxBoxSizer(wxHORIZONTAL);
	sound_list_add_button = new wxButton(frame, SOUND_LIST_ADD_BUTTON, wxT("Add"), wxDefaultPosition, wxDefaultSize);
	sound_list_remove_button = new wxButton(frame, SOUND_LIST_REMOVE_BUTTON, wxT("Remove"), wxDefaultPosition, wxDefaultSize);
	
	sound_list_button_sizer->Add(sound_list_add_button, wxEXPAND);
	sound_list_button_sizer->Add(sound_list_remove_button, wxEXPAND);
	
	sound_list_sizer->Add(sound_list, 2);
	sound_list_sizer->Add(sound_list_button_sizer);
	
	frame_sizer->Add(sound_list_sizer, 1, wxEXPAND);
	
	frame_sizer->AddSpacer(5);
	
	// editor section
	editor_sizer = new wxBoxSizer(wxVERTICAL);
	wxString sources_types[] = { wxT("8-bit"), wxT("16-bit") };
	source_radio_box = new wxRadioBox(frame, SOURCE_RADIO_BOX, wxString("Editing : "), wxDefaultPosition, wxDefaultSize, 2, sources_types, 2, wxRA_SPECIFY_COLS);
	
	sound_menu_sizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxString behavior_labels[] = { wxT("Quiet"), wxT("Normal"), wxT("Loud") };
							
	wxString chances_labels[] = { wxT("10%"), wxT("20%"), wxT("30%"), wxT("40%"), wxT("50%"), wxT("60%"), wxT("70%"), wxT("80%"), wxT("90%"), wxT("100%") };
	sound_behavior_label = new wxStaticText(frame, -1, wxT("Behavior : "));
	sound_behavior_menu = new wxChoice(frame, SOUND_BEHAVIOR_MENU, wxDefaultPosition, wxDefaultSize, 3, behavior_labels);
	sound_chance_label = new wxStaticText(frame, -1, wxT("Chance : "));
	sound_chance_menu = new wxChoice(frame, SOUND_CHANCE_MENU, wxDefaultPosition, wxDefaultSize, 9, chances_labels);
	
	sound_menu_sizer->Add(sound_behavior_label, wxEXPAND);
	sound_menu_sizer->Add(sound_behavior_menu, wxEXPAND);
	sound_menu_sizer->AddSpacer(20);
	sound_menu_sizer->Add(sound_chance_label, wxEXPAND);
	sound_menu_sizer->Add(sound_chance_menu, wxEXPAND);
	
	wxString flags_labels[] = {
							wxT("Cannot be restarted"),
							wxT("Does not self-abort"),
							wxT("Resist pitch changes"),
							wxT("Cannot change pitch"),
							wxT("Cannot be obstructed"),
							wxT("Cannot be media obstructed"),
							wxT("Is ambient") };
	
	sound_flags_box = new wxRadioBox(frame, SOUND_FLAGS_BOX, wxString("Flags : "), wxDefaultPosition, wxDefaultSize, 7, flags_labels, 2, wxRA_USE_CHECKBOX | wxRA_SPECIFY_COLS);
	
	sound_low_pitch_slider = new wxSlider(frame, SOUND_LOW_PITCH_SLIDER, 0, 0, MAX_SLIDER_VALUE);
	sound_high_pitch_slider = new wxSlider(frame, SOUND_HIGH_PITCH_SLIDER, 0, 0, MAX_SLIDER_VALUE);
	
	permutation_static_text = new wxStaticText(frame, -1, wxT("Permutations :"));
	permutation_list_box = new wxListBox(frame, PERMUTATION_LIST_BOX, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE | wxLB_ALWAYS_SB);
	
	editor_sizer->Add(source_radio_box);
	editor_sizer->Add(sound_menu_sizer, 1, wxEXPAND);
	editor_sizer->Add(sound_flags_box);
	editor_sizer->Add(sound_low_pitch_slider);
	editor_sizer->Add(sound_high_pitch_slider);
	editor_sizer->Add(permutation_static_text);
	editor_sizer->Add(permutation_list_box, 1, wxEXPAND);
	
	frame_sizer->Add(editor_sizer, 5, wxEXPAND);
	
	frame_sizer->Layout();
	frame->SetSizer(frame_sizer);
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
	sound_list->Clear();
	
	for (unsigned int i = 0; i < payload->GetSoundCount() ; i++) {
		sound_list->Append(wxString::Format("Sound %d", i));
	}
	
	//FIXME
	int sound_index = sound_list->GetSelection(); 
	if (sound_index == wxNOT_FOUND) {
		wxLogDebug(wxT("[SoundsView] There is no sound selected. Selecting first item..."));
		sound_list->SetFirstItem(0);
		sound_list->SetSelection(1);
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
		
	int sound_index = sound_list->GetSelection();
	int source_index = source_radio_box->GetSelection();
	
	if (sound_index == wxNOT_FOUND || source_index == wxNOT_FOUND)
		return false;
	
	wxLogDebug("[SoundsView] Selected sound %d", sound_index);
	wxLogDebug("[SoundsView] Selected source %d", source_index);
	
	SoundsDefinition *def = payload->GetSoundDefinition(source_index, sound_index);
	
	sound_behavior_menu->SetSelection(def->GetBehaviorIndex());
	//TODO
//	sound_chance_menu->SetSelection(def->GetChance());
	
//	sound_flags_box
	
	sound_low_pitch_slider->SetValue((int)(def->GetLowPitch() * MAX_SLIDER_VALUE));
	sound_high_pitch_slider->SetValue((int)(def->GetHighPitch() * MAX_SLIDER_VALUE));
	
	permutation_list_box->Clear();
	for (unsigned int i = 0; i < def->GetPermutationCount(); i++) {
		permutation_list_box->Append(wxString::Format("Permutation %d", i));
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

void SoundsView::SoundSelected(wxCommandEvent &e)
{
	wxLogDebug(wxT("Sound selection Changed to item %d"), e.GetInt());
	Update();
}


void SoundsView::SourceSelected(wxCommandEvent &e)
{
	wxLogDebug(wxT("Source selection Changed to item %d"), e.GetInt());
	Update();
}

void SoundsView::BehaviorChanged(wxCommandEvent &e)
{

}

void SoundsView::ChanceChanged(wxCommandEvent &e)
{

}

void SoundsView::FlagsChanged(wxCommandEvent &e)
{

}

void SoundsView::LowPitchChanged(wxScrollEvent &e)
{

}

void SoundsView::HighPitchChanged(wxScrollEvent &e)
{

}

void SoundsView::PermutationSelected(wxCommandEvent &e)
{

}


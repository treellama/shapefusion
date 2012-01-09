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

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <cmath>

#include "PhysicsView.h"
#include "PhysicsTreeItemData.h"

#include "../DefaultNames.h"

BEGIN_EVENT_TABLE(PhysicsView, wxView)
        EVT_TREE_SEL_CHANGED(-1, PhysicsView::OnTreeSelect)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(PhysicsView, wxView)

const wxSize choiceSize(200, -1);

static inline wxString Format(double f) { 
	return wxString::Format(wxT("%0.4f"), f);
}

static inline wxString Format(unsigned short u) {
	return wxString::Format(wxT("%hu"), u);
}

static inline wxString Format(short i) {
	return wxString::Format(wxT("%hi"), i);
}

void PhysicsView::CreateAliens()
{
	aliens_appearance_sizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticBoxSizer* appearance_sizer = new wxStaticBoxSizer(wxHORIZONTAL, mFrame, _("Appearance"));

	wxFlexGridSizer* appearance_grid_sizer = new wxFlexGridSizer(2);
	appearance_grid_sizer->AddGrowableCol(0);
	appearance_grid_sizer->SetHGap(10);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Graphic Collection:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_collection_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(alien_collection_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Color Table:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_color_table_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(alien_color_table_field, 0, wxALIGN_CENTER_VERTICAL);

	wxStaticText* sequence_label = new wxStaticText(mFrame, wxID_ANY, _("Sequence IDs:"));
//	wxFont font = sequence_label->GetFont();
//	font.SetWeight(wxFONTWEIGHT_BOLD);
//	sequence_label->SetFont(font);
	appearance_grid_sizer->Add(sequence_label, 0, wxALIGN_CENTER_VERTICAL);
	appearance_grid_sizer->AddSpacer(30);
	
	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Hit:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_hit_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(alien_hit_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Hard Dying:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_hard_dying_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(alien_hard_dying_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Soft Dying:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_soft_dying_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(alien_soft_dying_field, 0, wxALIGN_CENTER_VERTICAL);
	
	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Hard Dead:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_hard_dead_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(alien_hard_dead_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Soft Dead:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_soft_dead_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(alien_soft_dead_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Stationary:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_stationary_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(alien_stationary_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Moving:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_moving_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(alien_moving_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Teleport In:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_teleport_in_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(alien_teleport_in_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Teleport Out:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_teleport_out_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(alien_teleport_out_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Melee Attack:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_melee_attack_shape_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(alien_melee_attack_shape_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Ranged Attack:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_ranged_attack_shape_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(alien_ranged_attack_shape_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_sizer->Add(appearance_grid_sizer);

	aliens_appearance_sizer->Add(appearance_sizer);
	aliens_appearance_sizer->AddSpacer(10);

	wxStaticBoxSizer* sounds_sizer = new wxStaticBoxSizer(wxHORIZONTAL, mFrame, _("Sounds"));

	wxFlexGridSizer* sounds_grid_sizer = new wxFlexGridSizer(2);
	sounds_grid_sizer->AddGrowableCol(0);
	sounds_grid_sizer->SetHGap(10);

	std::vector<wxString> sound_strings = DefaultNames::Instance()->GetArray(wxT("sound"));
	sound_strings.insert(sound_strings.begin(), GetName(wxT("sound"), -1));

	sounds_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Activation:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_activation_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	sounds_grid_sizer->Add(alien_activation_choice, 0, wxALIGN_CENTER_VERTICAL);

	sounds_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Friendly Activation:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_friendly_activation_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	sounds_grid_sizer->Add(alien_friendly_activation_choice, 0, wxALIGN_CENTER_VERTICAL);

	sounds_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Clear:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_clear_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	sounds_grid_sizer->Add(alien_clear_choice, 0, wxALIGN_CENTER_VERTICAL);

	sounds_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Kill:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_kill_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	sounds_grid_sizer->Add(alien_kill_choice, 0, wxALIGN_CENTER_VERTICAL);

	sounds_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Apology:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_apology_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	sounds_grid_sizer->Add(alien_apology_choice, 0, wxALIGN_CENTER_VERTICAL);

	sounds_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Friendly Fire:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_friendly_fire_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	sounds_grid_sizer->Add(alien_friendly_fire_choice, 0, wxALIGN_CENTER_VERTICAL);

	sounds_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Flaming:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_flaming_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	sounds_grid_sizer->Add(alien_flaming_choice, 0, wxALIGN_CENTER_VERTICAL);

	sounds_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Random:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_random_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	sounds_grid_sizer->Add(alien_random_choice, 0, wxALIGN_CENTER_VERTICAL);

	sounds_grid_sizer->AddSpacer(10);
	sounds_grid_sizer->AddSpacer(10);

	sounds_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Random Sound Mask:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_random_sound_mask_field = new wxTextCtrl(mFrame, wxID_ANY);
	sounds_grid_sizer->Add(alien_random_sound_mask_field, 0, wxALIGN_CENTER_VERTICAL);

	sounds_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Sound Pitch:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_sound_pitch_field = new wxTextCtrl(mFrame, wxID_ANY);
	sounds_grid_sizer->Add(alien_sound_pitch_field, 0, wxALIGN_CENTER_VERTICAL);

	sounds_sizer->Add(sounds_grid_sizer);

	aliens_appearance_sizer->Add(sounds_sizer);
	
	mainbox->Add(aliens_appearance_sizer, 5, wxALL, 10);
	mainbox->Show(aliens_appearance_sizer, false);

	aliens_combat_sizer = new wxBoxSizer(wxHORIZONTAL);

	wxFlexGridSizer* combat_grid_sizer = new wxFlexGridSizer(2);
	combat_grid_sizer->SetHGap(10);
	combat_grid_sizer->SetVGap(10);

	wxBoxSizer* attack_frequency_sizer = new wxBoxSizer(wxHORIZONTAL);

	attack_frequency_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Attack Frequency:")), 1, wxALIGN_CENTER_VERTICAL);
	attack_frequency_sizer->AddSpacer(10);
	alien_attack_frequency_field = new wxTextCtrl(mFrame, wxID_ANY);
	attack_frequency_sizer->Add(alien_attack_frequency_field, 0, wxALIGN_CENTER_VERTICAL);

	combat_grid_sizer->Add(attack_frequency_sizer);
	combat_grid_sizer->AddSpacer(0);

	wxStaticBoxSizer* melee_attack_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Melee Attack"));
	wxFlexGridSizer* melee_attack_grid_sizer = new wxFlexGridSizer(2);
	melee_attack_grid_sizer->AddGrowableCol(0);
	melee_attack_grid_sizer->SetHGap(10);

	std::vector<wxString> shot_strings = DefaultNames::Instance()->GetArray(wxT("shot"));
	shot_strings.insert(shot_strings.begin(), GetName(wxT("shot"), -1));

	melee_attack_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Type:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_melee_attack_type_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, shot_strings.size(), &shot_strings[0]);
	melee_attack_grid_sizer->Add(alien_melee_attack_type_choice, 0, wxALIGN_CENTER_VERTICAL);

	melee_attack_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Repetitions:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_melee_attack_repetitions_field = new wxTextCtrl(mFrame, wxID_ANY);
	melee_attack_grid_sizer->Add(alien_melee_attack_repetitions_field, 0, wxALIGN_CENTER_VERTICAL);

	melee_attack_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Error:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_melee_attack_error_field = new wxTextCtrl(mFrame, wxID_ANY);
	melee_attack_grid_sizer->Add(alien_melee_attack_error_field, 0, wxALIGN_CENTER_VERTICAL);

	melee_attack_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Range:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_melee_attack_range_field = new wxTextCtrl(mFrame, wxID_ANY);
	melee_attack_grid_sizer->Add(alien_melee_attack_range_field, 0, wxALIGN_CENTER_VERTICAL);

	melee_attack_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Sequence ID:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_melee_attack_sequence_field = new wxTextCtrl(mFrame, wxID_ANY);
	melee_attack_grid_sizer->Add(alien_melee_attack_sequence_field, 0, wxALIGN_CENTER_VERTICAL);

	melee_attack_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("dx:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_melee_attack_dx_field = new wxTextCtrl(mFrame, wxID_ANY);
	melee_attack_grid_sizer->Add(alien_melee_attack_dx_field, 0, wxALIGN_CENTER_VERTICAL);

	melee_attack_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("dy:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_melee_attack_dy_field = new wxTextCtrl(mFrame, wxID_ANY);
	melee_attack_grid_sizer->Add(alien_melee_attack_dy_field, 0, wxALIGN_CENTER_VERTICAL);

	melee_attack_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("dz:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_melee_attack_dz_field = new wxTextCtrl(mFrame, wxID_ANY);
	melee_attack_grid_sizer->Add(alien_melee_attack_dz_field, 0, wxALIGN_CENTER_VERTICAL);

	melee_attack_sizer->Add(melee_attack_grid_sizer, 0, wxEXPAND);

	combat_grid_sizer->Add(melee_attack_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* ranged_attack_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Ranged Attack"));
	wxFlexGridSizer* ranged_attack_grid_sizer = new wxFlexGridSizer(2);
	ranged_attack_grid_sizer->AddGrowableCol(0);
	ranged_attack_grid_sizer->SetHGap(10);

	ranged_attack_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Type:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_ranged_attack_type_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, shot_strings.size(), &shot_strings[0]);
	ranged_attack_grid_sizer->Add(alien_ranged_attack_type_choice, 0, wxALIGN_CENTER_VERTICAL);

	ranged_attack_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Repetitions:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_ranged_attack_repetitions_field = new wxTextCtrl(mFrame, wxID_ANY);
	ranged_attack_grid_sizer->Add(alien_ranged_attack_repetitions_field, 0, wxALIGN_CENTER_VERTICAL);

	ranged_attack_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Error:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_ranged_attack_error_field = new wxTextCtrl(mFrame, wxID_ANY);
	ranged_attack_grid_sizer->Add(alien_ranged_attack_error_field, 0, wxALIGN_CENTER_VERTICAL);

	ranged_attack_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Range:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_ranged_attack_range_field = new wxTextCtrl(mFrame, wxID_ANY);
	ranged_attack_grid_sizer->Add(alien_ranged_attack_range_field, 0, wxALIGN_CENTER_VERTICAL);

	ranged_attack_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Sequence ID:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_ranged_attack_sequence_field = new wxTextCtrl(mFrame, wxID_ANY);
	ranged_attack_grid_sizer->Add(alien_ranged_attack_sequence_field, 0, wxALIGN_CENTER_VERTICAL);

	ranged_attack_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("dx:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_ranged_attack_dx_field = new wxTextCtrl(mFrame, wxID_ANY);
	ranged_attack_grid_sizer->Add(alien_ranged_attack_dx_field, 0, wxALIGN_CENTER_VERTICAL);

	ranged_attack_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("dy:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_ranged_attack_dy_field = new wxTextCtrl(mFrame, wxID_ANY);
	ranged_attack_grid_sizer->Add(alien_ranged_attack_dy_field, 0, wxALIGN_CENTER_VERTICAL);

	ranged_attack_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("dz:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_ranged_attack_dz_field = new wxTextCtrl(mFrame, wxID_ANY);
	ranged_attack_grid_sizer->Add(alien_ranged_attack_dz_field, 0, wxALIGN_CENTER_VERTICAL);

	ranged_attack_sizer->Add(ranged_attack_grid_sizer);

	combat_grid_sizer->Add(ranged_attack_sizer);

	wxStaticBoxSizer* shrapnel_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Shrapnel"));
	wxFlexGridSizer* shrapnel_grid_sizer = new wxFlexGridSizer(2);
	shrapnel_grid_sizer->AddGrowableCol(0);
	shrapnel_grid_sizer->SetHGap(10);

	shrapnel_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Shrapnel Radius:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_shrapnel_radius_field = new wxTextCtrl(mFrame, wxID_ANY);
	shrapnel_grid_sizer->Add(alien_shrapnel_radius_field, 0, wxALIGN_CENTER_VERTICAL);

	std::vector<wxString> damage_strings = DefaultNames::Instance()->GetArray(wxT("damage"));
	damage_strings.insert(damage_strings.begin(), _("None"));

	shrapnel_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Damage Type:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_shrapnel_damage_type_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, damage_strings.size(), &damage_strings[0]);
	shrapnel_grid_sizer->Add(alien_shrapnel_damage_type_choice, 0, wxALIGN_CENTER_VERTICAL);

	shrapnel_sizer->Add(shrapnel_grid_sizer, 0, wxEXPAND);
	
	alien_shrapnel_alien_damage_checkbox = new wxCheckBox(mFrame, wxID_ANY, _("Alien Damage (varies with level)"));
	shrapnel_sizer->Add(alien_shrapnel_alien_damage_checkbox, 0, wxEXPAND);

	wxFlexGridSizer* shrapnel_grid_sizer2 = new wxFlexGridSizer(2);
	shrapnel_grid_sizer2->AddGrowableCol(0);
	shrapnel_grid_sizer2->SetHGap(10);

	// make the fields line up with the grid above
	shrapnel_grid_sizer2->AddSpacer(0);
	shrapnel_grid_sizer2->Add(choiceSize.GetWidth(), 0);

	shrapnel_grid_sizer2->Add(new wxStaticText(mFrame, wxID_ANY, _("Base Damage:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_shrapnel_base_damage_field = new wxTextCtrl(mFrame, wxID_ANY);
	shrapnel_grid_sizer2->Add(alien_shrapnel_base_damage_field, 0, wxALIGN_CENTER_VERTICAL);

	shrapnel_grid_sizer2->Add(new wxStaticText(mFrame, wxID_ANY, _("Random Damage:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_shrapnel_random_damage_field = new wxTextCtrl(mFrame, wxID_ANY);
	shrapnel_grid_sizer2->Add(alien_shrapnel_random_damage_field, 0, wxALIGN_CENTER_VERTICAL);

	shrapnel_grid_sizer2->Add(new wxStaticText(mFrame, wxID_ANY, _("Scale:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_shrapnel_damage_scale_field = new wxTextCtrl(mFrame, wxID_ANY);
	shrapnel_grid_sizer2->Add(alien_shrapnel_damage_scale_field, 0, wxALIGN_CENTER_VERTICAL);

	shrapnel_sizer->Add(shrapnel_grid_sizer2, 0, wxEXPAND);

	combat_grid_sizer->Add(shrapnel_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* impact_effects_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Impact Effects (hits on monster)"));
	wxFlexGridSizer* impact_effects_grid_sizer = new wxFlexGridSizer(2);
	impact_effects_grid_sizer->AddGrowableCol(0);
	impact_effects_grid_sizer->SetHGap(10);

	std::vector<wxString> effect_strings = DefaultNames::Instance()->GetArray(wxT("effect"));
	effect_strings.insert(effect_strings.begin(), GetName(wxT("sound"), -1));

	impact_effects_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Ranged:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_ranged_impact_effect_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, effect_strings.size(), &effect_strings[0]);
	impact_effects_grid_sizer->Add(alien_ranged_impact_effect_choice, 0, wxALIGN_CENTER_VERTICAL);

	impact_effects_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Melee:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_melee_impact_effect_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, effect_strings.size(), &effect_strings[0]);
	impact_effects_grid_sizer->Add(alien_melee_impact_effect_choice, 0, wxALIGN_CENTER_VERTICAL);

	impact_effects_sizer->Add(impact_effects_grid_sizer, 0, wxEXPAND);
	
	combat_grid_sizer->Add(impact_effects_sizer, 0, wxEXPAND);

	aliens_combat_sizer->Add(combat_grid_sizer);

	mainbox->Add(aliens_combat_sizer, 5, wxALL, 10);
	mainbox->Show(aliens_combat_sizer, false);

	aliens_constants_sizer = new wxBoxSizer(wxVERTICAL);

	wxFlexGridSizer* constants_grid_sizer = new wxFlexGridSizer(2);
	constants_grid_sizer->SetHGap(10);
	constants_grid_sizer->SetVGap(10);

	wxFlexGridSizer* vitality_grid_sizer = new wxFlexGridSizer(2);
	vitality_grid_sizer->AddGrowableCol(0);
	vitality_grid_sizer->SetHGap(10);

	vitality_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Vitality:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_vitality_field = new wxTextCtrl(mFrame, wxID_ANY);
	vitality_grid_sizer->Add(alien_vitality_field, 0, wxALIGN_CENTER_VERTICAL);

	constants_grid_sizer->Add(vitality_grid_sizer, 0, wxEXPAND);

	wxFlexGridSizer* radius_height_sizer = new wxFlexGridSizer(2);
	radius_height_sizer->AddGrowableCol(0);
	radius_height_sizer->SetHGap(10);

	radius_height_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Radius:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_radius_field = new wxTextCtrl(mFrame, wxID_ANY);
	radius_height_sizer->Add(alien_radius_field, 0, wxALIGN_CENTER_VERTICAL);

	radius_height_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Height:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_height_field = new wxTextCtrl(mFrame, wxID_ANY);
	radius_height_sizer->Add(alien_height_field, 0, wxALIGN_CENTER_VERTICAL);
	
	constants_grid_sizer->Add(radius_height_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* movement_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Movement"));
	wxFlexGridSizer* movement_grid_sizer = new wxFlexGridSizer(2);
	movement_grid_sizer->AddGrowableCol(0);
	movement_grid_sizer->SetHGap(10);

	movement_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Speed:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_speed_field = new wxTextCtrl(mFrame, wxID_ANY);
	movement_grid_sizer->Add(alien_speed_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Terminal Velocity:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_terminal_velocity_field = new wxTextCtrl(mFrame, wxID_ANY);
	movement_grid_sizer->Add(alien_terminal_velocity_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Gravity:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_gravity_field = new wxTextCtrl(mFrame, wxID_ANY);
	movement_grid_sizer->Add(alien_gravity_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->AddSpacer(10);
	movement_grid_sizer->AddSpacer(10);

	movement_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Min. Ledge Jump:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_min_ledge_jump_field = new wxTextCtrl(mFrame, wxID_ANY);
	movement_grid_sizer->Add(alien_min_ledge_jump_field, 0, wxALIGN_CENTER_VERTICAL);	

	movement_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Max. Ledge Jump:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_max_ledge_jump_field = new wxTextCtrl(mFrame, wxID_ANY);
	movement_grid_sizer->Add(alien_max_ledge_jump_field, 0, wxALIGN_CENTER_VERTICAL);	

	movement_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("External Velocity Scale:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_ext_velocity_scale_field = new wxTextCtrl(mFrame, wxID_ANY);
	movement_grid_sizer->Add(alien_ext_velocity_scale_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Hover Height:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_hover_height_field = new wxTextCtrl(mFrame, wxID_ANY);
	movement_grid_sizer->Add(alien_hover_height_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->AddSpacer(10);
	movement_grid_sizer->AddSpacer(10);

	movement_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Door Retry Mask:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_door_retry_mask_field = new wxTextCtrl(mFrame, wxID_ANY);
	movement_grid_sizer->Add(alien_door_retry_mask_field, 0, wxALIGN_CENTER_VERTICAL);	

	movement_sizer->Add(movement_grid_sizer, 0, wxEXPAND);

	constants_grid_sizer->Add(movement_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* perception_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Perception"));
	wxFlexGridSizer* perception_grid_sizer = new wxFlexGridSizer(2);
	perception_grid_sizer->AddGrowableCol(0);
	perception_grid_sizer->SetHGap(10);

	perception_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Visual Range:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_visual_range_field = new wxTextCtrl(mFrame, wxID_ANY);
	perception_grid_sizer->Add(alien_visual_range_field, 0, wxALIGN_CENTER_VERTICAL);	

	perception_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Dark Visual Range:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_dark_visual_range_field = new wxTextCtrl(mFrame, wxID_ANY);
	perception_grid_sizer->Add(alien_dark_visual_range_field, 0, wxALIGN_CENTER_VERTICAL);

	perception_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Intelligence:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_intelligence_field = new wxTextCtrl(mFrame, wxID_ANY);
	perception_grid_sizer->Add(alien_intelligence_field, 0, wxALIGN_CENTER_VERTICAL);

	perception_sizer->Add(perception_grid_sizer, 0, wxEXPAND);

	constants_grid_sizer->Add(perception_sizer, 0, wxEXPAND);

	aliens_constants_sizer->Add(constants_grid_sizer);
	aliens_constants_sizer->AddSpacer(10);

	wxFlexGridSizer* carrying_grid_sizer = new wxFlexGridSizer(2);
	carrying_grid_sizer->AddGrowableCol(0);
	carrying_grid_sizer->SetHGap(10);

	std::vector<wxString> item_strings = DefaultNames::Instance()->GetArray(wxT("item"));
	item_strings.insert(item_strings.begin(), _("None"));

	carrying_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Carrying Item Type:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_carrying_item_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, item_strings.size(), &item_strings[0]);
	carrying_grid_sizer->Add(alien_carrying_item_choice, 0, wxALIGN_CENTER_VERTICAL);

	carrying_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Contrail Effect:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_contrail_effect_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, effect_strings.size(), &effect_strings[0]);
	carrying_grid_sizer->Add(alien_contrail_effect_choice, 0, wxALIGN_CENTER_VERTICAL);

	aliens_constants_sizer->Add(carrying_grid_sizer);
	
	mainbox->Add(aliens_constants_sizer, 5, wxALL, 10);
	mainbox->Show(aliens_constants_sizer, false);

	aliens_behavior_sizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* class_sizer = new wxBoxSizer(wxHORIZONTAL);

	std::vector<wxString> class_strings = DefaultNames::Instance()->GetArray(wxT("class"));

	class_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Class:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_class_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, class_strings.size(), &class_strings[0]);
	class_sizer->AddSpacer(10);
	class_sizer->Add(alien_class_choice, 0, wxALIGN_CENTER_VERTICAL);

	aliens_behavior_sizer->Add(class_sizer);

	wxStaticBoxSizer* friends_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Friends"));
	wxGridSizer* friends_grid_sizer = new wxGridSizer(4);

	for (int i = 0; i < 16; ++i) {
		alien_friends_checkboxes[i] = new wxCheckBox(mFrame, wxID_ANY, GetName(wxT("class"), i));

	}

	for (int col = 0; col < 4; ++col) {
		for (int row = 0; row < 4; ++row) {
			friends_grid_sizer->Add(alien_friends_checkboxes[row * 4 + col], 0, wxALIGN_CENTER_VERTICAL);
		}
	}
	
	friends_sizer->Add(friends_grid_sizer, 0, wxEXPAND);

	aliens_behavior_sizer->AddSpacer(10);
	aliens_behavior_sizer->Add(friends_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* enemies_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Enemies"));
	wxGridSizer* enemies_grid_sizer = new wxGridSizer(4);

	for (int i = 0; i < 16; ++i) {
		alien_enemies_checkboxes[i] = new wxCheckBox(mFrame, wxID_ANY, GetName(wxT("class"), i));
	}

	for (int col = 0; col < 4; ++col) {
		for (int row = 0; row < 4; ++row) {
			enemies_grid_sizer->Add(alien_enemies_checkboxes[row * 4 + col], 0, wxALIGN_CENTER_VERTICAL);
		}
	}

	enemies_sizer->Add(enemies_grid_sizer, 0, wxEXPAND);
	
	aliens_behavior_sizer->AddSpacer(10);
	aliens_behavior_sizer->Add(enemies_sizer, 0, wxEXPAND);

	const wxString flags_strings[] = {
		_("Flies"),
		_("Is Alien"),
		_("Major"),
		_("Minor"),
		_("Cannot Skip"),
		_("Floats"),
		_("Cannot Attack"),
		_("Uses Sniper Ledges"),
		_("Invisible"),
		_("Subtly Invisible"),
		_("Kamikaze"),
		_("Berserker"),
		_("Enlarged"),
		_("Delayed Hard Death"),
		_("Fires Symmetrically"),
		_("Nuclear Hard Death"),
		_("Can't Fire Backwards"),
		_("Die in Flames"),
		_("Stay with Clear Shot"),
		_("Tiny"),
		_("Attacks Immediately"),
		_("Not Afraid of Water"),
		_("Not Afraid of Sewage"),
		_("Not Afraid of Lava"),
		_("Not Afraid of Goo"),
		_("Can Teleport Under Liquid"),
		_("Chooses Weapon Randomly"),
	};

	const int flags_layout[] = {
		0, 8, 17,
		1, 9, 18,
		2, 10, 19,
		3, 11, 20,
		4, 12, 21,
		5, 13, 22,
		6, 14, 23,
		7, 15, 24,
		26, 16, 25
	};

	wxStaticBoxSizer* flags_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Flags"));
	wxGridSizer* flags_grid_sizer = new wxGridSizer(3);
	
	for (int i = 0; i < 27; ++i) {
		alien_flags_checkboxes[i] = new wxCheckBox(mFrame, wxID_ANY, flags_strings[i]);
	}

	for (int i = 0; i < 27; ++i) {
		flags_grid_sizer->Add(alien_flags_checkboxes[flags_layout[i]], 0, wxALIGN_CENTER_VERTICAL);
	}

	flags_sizer->Add(flags_grid_sizer, 0, wxEXPAND);
	
	aliens_behavior_sizer->AddSpacer(10);
	aliens_behavior_sizer->Add(flags_sizer, 0, wxEXPAND);
	
	mainbox->Add(aliens_behavior_sizer, 5, wxALL, 10);
	mainbox->Show(aliens_behavior_sizer, false);

	aliens_immunities_sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* immunities_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Immunities"));
	wxGridSizer* immunities_grid_sizer = new wxGridSizer(4);
	
	for (int i = 0; i < 24; ++i) {
		alien_immunities_checkboxes[i] = new wxCheckBox(mFrame, wxID_ANY, GetName(wxT("damage"), i));
	}

	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 4; ++j) {
			immunities_grid_sizer->Add(alien_immunities_checkboxes[j * 6 + i], 0, wxALIGN_CENTER_VERTICAL);
		}
	}

	immunities_sizer->Add(immunities_grid_sizer, 0, wxEXPAND);

	aliens_immunities_sizer->Add(immunities_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* weaknesses_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Weaknesses"));
	wxGridSizer* weaknesses_grid_sizer = new wxGridSizer(4);

	for (int i = 0; i < 24; ++i) {
		alien_weaknesses_checkboxes[i] = new wxCheckBox(mFrame, wxID_ANY, GetName(wxT("damage"), i));
	}

	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 4; ++j) {
			weaknesses_grid_sizer->Add(alien_weaknesses_checkboxes[j * 6 + i], 0, wxALIGN_CENTER_VERTICAL);
		}
	}

	weaknesses_sizer->Add(weaknesses_grid_sizer, 0, wxEXPAND);
	
	aliens_immunities_sizer->AddSpacer(10);
	aliens_immunities_sizer->Add(weaknesses_sizer, 0, wxEXPAND);

	mainbox->Add(aliens_immunities_sizer, 5, wxALL, 10);
	mainbox->Show(aliens_immunities_sizer, false);
}

void PhysicsView::CreateEffects()
{
	effects_sizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticBoxSizer* effects_static_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Effect"));
	
	wxFlexGridSizer* effects_grid_sizer = new wxFlexGridSizer(2);
	effects_grid_sizer->AddGrowableCol(0);
	effects_grid_sizer->SetHGap(10);
	
	effects_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Graphic collection:")), 0, wxALIGN_CENTER_VERTICAL);
	eff_collection_field = new wxTextCtrl(mFrame, wxID_ANY);
	effects_grid_sizer->Add(eff_collection_field, 0, wxALIGN_CENTER_VERTICAL);

	effects_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Color table:")), 0, wxALIGN_CENTER_VERTICAL);
	eff_color_table_field = new wxTextCtrl(mFrame, wxID_ANY);
	effects_grid_sizer->Add(eff_color_table_field, 0, wxALIGN_CENTER_VERTICAL);

	effects_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Sequence:")), 0, wxALIGN_CENTER_VERTICAL);
	eff_sequence_field = new wxTextCtrl(mFrame, wxID_ANY);
	effects_grid_sizer->Add(eff_sequence_field, 0, wxALIGN_CENTER_VERTICAL);

	effects_grid_sizer->AddSpacer(10);
	effects_grid_sizer->AddSpacer(10);

	effects_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Sound pitch:")), 0, wxALIGN_CENTER_VERTICAL);
	eff_pitch_field = new wxTextCtrl(mFrame, wxID_ANY);
	effects_grid_sizer->Add(eff_pitch_field, 0, wxALIGN_CENTER_VERTICAL);

	effects_grid_sizer->AddSpacer(10);
	effects_grid_sizer->AddSpacer(10);

	effects_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Delay:")), 0, wxALIGN_CENTER_VERTICAL);
	eff_delay_field = new wxTextCtrl(mFrame, wxID_ANY);
	eff_delay_field->Disable();
	effects_grid_sizer->Add(eff_delay_field, 0, wxALIGN_CENTER_VERTICAL);

	std::vector<wxString> delay_sound_choices = DefaultNames::Instance()->GetArray(wxT("sound"));
	delay_sound_choices.insert(delay_sound_choices.begin(), GetName(wxT("sound"), -1));

	effects_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Delay sound:")), 0, wxALIGN_CENTER_VERTICAL);
	eff_delay_sound_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, delay_sound_choices.size(), &delay_sound_choices[0]);
	effects_grid_sizer->Add(eff_delay_sound_choice, 0, wxALIGN_CENTER_VERTICAL);

	effects_static_sizer->Add(effects_grid_sizer);
	
	effects_static_sizer->AddSpacer(10);

	eff_end_when_animation_loops_checkbox = new wxCheckBox(mFrame, wxID_ANY, _("End when animation loops"));
	effects_static_sizer->Add(eff_end_when_animation_loops_checkbox);

	eff_end_when_transfer_animation_loops_checkbox = new wxCheckBox(mFrame, wxID_ANY, _("End when transfer animation loops"));
	effects_static_sizer->Add(eff_end_when_transfer_animation_loops_checkbox);

	eff_sound_only_checkbox = new wxCheckBox(mFrame, wxID_ANY, _("Sound only"));
	effects_static_sizer->Add(eff_sound_only_checkbox);

	eff_media_effect_checkbox = new wxCheckBox(mFrame, wxID_ANY, _("Media effect"));
	effects_static_sizer->Add(eff_media_effect_checkbox);
	
	effects_sizer->Add(effects_static_sizer);
	
	mainbox->Add(effects_sizer, 5, wxALL, 10);
	mainbox->Show(effects_sizer, false);
}

void PhysicsView::CreatePhysicsConstants()
{
	// physics constants
	physics_sizer = new wxFlexGridSizer(2);
	physics_sizer->SetHGap(10);
	physics_sizer->SetVGap(20);

	wxStaticBoxSizer* movement_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Movement"));
	wxFlexGridSizer* movement_grid_sizer = new wxFlexGridSizer(2);

	movement_grid_sizer->AddGrowableCol(0);
	movement_grid_sizer->SetHGap(10);

	movement_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Max. forward velocity:")), 0, wxALIGN_CENTER_VERTICAL);
	max_forward_velocity_field = new wxTextCtrl(mFrame, wxID_ANY);
	movement_grid_sizer->Add(max_forward_velocity_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Max. backward velocity:")), 0, wxALIGN_CENTER_VERTICAL);
	max_backward_velocity_field = new wxTextCtrl(mFrame, wxID_ANY);
	movement_grid_sizer->Add(max_backward_velocity_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Max. perpendicular velocity:")), 0, wxALIGN_CENTER_VERTICAL);
	max_perpendicular_velocity_field = new wxTextCtrl(mFrame, wxID_ANY);
	movement_grid_sizer->Add(max_perpendicular_velocity_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->AddSpacer(0);
	movement_grid_sizer->AddSpacer(0);

	movement_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Acceleration:")), 0, wxALIGN_CENTER_VERTICAL);
	acceleration_field = new wxTextCtrl(mFrame, wxID_ANY);
	movement_grid_sizer->Add(acceleration_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Deceleration:")), 0, wxALIGN_CENTER_VERTICAL);
	deceleration_field = new wxTextCtrl(mFrame, wxID_ANY);
	movement_grid_sizer->Add(deceleration_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Airborne deceleration:")), 0, wxALIGN_CENTER_VERTICAL);
	airborne_deceleration_field = new wxTextCtrl(mFrame, wxID_ANY);
	movement_grid_sizer->Add(airborne_deceleration_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Gravitational acceleration:")), 0, wxALIGN_CENTER_VERTICAL);
	gravitational_acceleration_field = new wxTextCtrl(mFrame, wxID_ANY);
	movement_grid_sizer->Add(gravitational_acceleration_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Climbing acceleration:")), 0, wxALIGN_CENTER_VERTICAL);
	climbing_acceleration_field = new wxTextCtrl(mFrame, wxID_ANY);
	movement_grid_sizer->Add(climbing_acceleration_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Terminal velocity:")), 0, wxALIGN_CENTER_VERTICAL);
	terminal_velocity_field = new wxTextCtrl(mFrame, wxID_ANY);
	movement_grid_sizer->Add(terminal_velocity_field, 0, wxALIGN_CENTER_VERTICAL);	

	movement_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("External deceleration:")), 0, wxALIGN_CENTER_VERTICAL);
	external_deceleration_field = new wxTextCtrl(mFrame, wxID_ANY);
	movement_grid_sizer->Add(external_deceleration_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_sizer->Add(movement_grid_sizer, 0, wxEXPAND);
	physics_sizer->Add(movement_sizer, 0, wxEXPAND);

	wxBoxSizer* steps_player_size_sizer = new wxBoxSizer(wxVERTICAL);
	wxStaticBoxSizer* steps_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Steps"));
	wxFlexGridSizer* steps_grid_sizer = new wxFlexGridSizer(2);
	steps_grid_sizer->AddGrowableCol(0);
	steps_grid_sizer->SetHGap(10);

	steps_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Step delta:")), 0, wxALIGN_CENTER_VERTICAL);
	step_delta_field = new wxTextCtrl(mFrame, wxID_ANY);
	steps_grid_sizer->Add(step_delta_field, 0, wxALIGN_CENTER_VERTICAL);

	steps_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Step amplitude:")), 0, wxALIGN_CENTER_VERTICAL);
	step_amplitude_field = new wxTextCtrl(mFrame, wxID_ANY);
	steps_grid_sizer->Add(step_amplitude_field, 0, wxALIGN_CENTER_VERTICAL);
	
	steps_sizer->Add(steps_grid_sizer, 0, wxEXPAND);

	steps_player_size_sizer->Add(steps_sizer, 0, wxEXPAND);
	steps_player_size_sizer->AddSpacer(10);

	wxStaticBoxSizer* size_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Player Size"));
	wxFlexGridSizer* size_grid_sizer = new wxFlexGridSizer(2);
	size_grid_sizer->AddGrowableCol(0);
	size_grid_sizer->SetHGap(10);

	size_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Radius:")), 0, wxALIGN_CENTER_VERTICAL);
	radius_field = new wxTextCtrl(mFrame, wxID_ANY);
	size_grid_sizer->Add(radius_field, 0, wxALIGN_CENTER_VERTICAL);

	size_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Height:")), 0, wxALIGN_CENTER_VERTICAL);
	height_field = new wxTextCtrl(mFrame, wxID_ANY);
	size_grid_sizer->Add(height_field, 0, wxALIGN_CENTER_VERTICAL);

	size_sizer->Add(size_grid_sizer, 0, wxEXPAND);
	
	steps_player_size_sizer->Add(size_sizer, 0, wxEXPAND);
	steps_player_size_sizer->AddStretchSpacer();

	physics_sizer->Add(steps_player_size_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* turning_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Turning/Head Movement"));
	wxFlexGridSizer* turning_grid_sizer = new wxFlexGridSizer(2);
	turning_grid_sizer->AddGrowableCol(0);
	turning_grid_sizer->SetHGap(10);
	
	turning_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Angular acceleration:")), 0, wxALIGN_CENTER_VERTICAL);
	angular_acceleration_field = new wxTextCtrl(mFrame, wxID_ANY);
	turning_grid_sizer->Add(angular_acceleration_field, 0, wxALIGN_CENTER_VERTICAL);

	turning_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Angular deceleration:")), 0, wxALIGN_CENTER_VERTICAL);
	angular_deceleration_field = new wxTextCtrl(mFrame, wxID_ANY);
	turning_grid_sizer->Add(angular_deceleration_field, 0, wxALIGN_CENTER_VERTICAL);

	turning_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Maximum angular velocity:")), 0, wxALIGN_CENTER_VERTICAL);
	maximum_angular_velocity_field = new wxTextCtrl(mFrame, wxID_ANY);
	turning_grid_sizer->Add(maximum_angular_velocity_field, 0, wxALIGN_CENTER_VERTICAL);

	turning_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Angular recentering velocity:")), 0, wxALIGN_CENTER_VERTICAL);
	angular_recentering_velocity_field = new wxTextCtrl(mFrame, wxID_ANY);
	turning_grid_sizer->Add(angular_recentering_velocity_field, 0, wxALIGN_CENTER_VERTICAL);

	turning_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Head angular velocity:")), 0, wxALIGN_CENTER_VERTICAL);
	head_angular_velocity_field = new wxTextCtrl(mFrame, wxID_ANY);
	turning_grid_sizer->Add(head_angular_velocity_field, 0, wxALIGN_CENTER_VERTICAL);

	turning_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Head angular maximum:")), 0, wxALIGN_CENTER_VERTICAL);
	head_angular_maximum_field = new wxTextCtrl(mFrame, wxID_ANY);
	turning_grid_sizer->Add(head_angular_maximum_field, 0, wxALIGN_CENTER_VERTICAL);

	turning_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Maximum elevation:")), 0, wxALIGN_CENTER_VERTICAL);
	maximum_elevation_field = new wxTextCtrl(mFrame, wxID_ANY);
	turning_grid_sizer->Add(maximum_elevation_field, 0, wxALIGN_CENTER_VERTICAL);

	turning_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("External angular deceleration:")), 0, wxALIGN_CENTER_VERTICAL);
	external_angular_deceleration_field = new wxTextCtrl(mFrame, wxID_ANY);
	turning_grid_sizer->Add(external_angular_deceleration_field, 0, wxALIGN_CENTER_VERTICAL);

	turning_sizer->Add(turning_grid_sizer, 0, wxEXPAND);
	physics_sizer->Add(turning_sizer, 0);

	wxStaticBoxSizer* camera_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Heights (for camera)"));
	wxFlexGridSizer* camera_grid_sizer = new wxFlexGridSizer(2);
	camera_grid_sizer->AddGrowableCol(0);
	camera_grid_sizer->SetHGap(10);

	camera_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Dead height:")), 0, wxALIGN_CENTER_VERTICAL);
	dead_height_field = new wxTextCtrl(mFrame, wxID_ANY);
	camera_grid_sizer->Add(dead_height_field, 0, wxALIGN_CENTER_VERTICAL);

	camera_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Camera height:")), 0, wxALIGN_CENTER_VERTICAL);
	camera_height_field = new wxTextCtrl(mFrame, wxID_ANY);
	camera_grid_sizer->Add(camera_height_field, 0, wxALIGN_CENTER_VERTICAL);

	camera_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Splash height:")), 0, wxALIGN_CENTER_VERTICAL);
	splash_height_field = new wxTextCtrl(mFrame, wxID_ANY);
	camera_grid_sizer->Add(splash_height_field, 0, wxALIGN_CENTER_VERTICAL);

	camera_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Half camera separation:")), 0, wxALIGN_CENTER_VERTICAL);
	half_camera_separation_field = new wxTextCtrl(mFrame, wxID_ANY);
	camera_grid_sizer->Add(half_camera_separation_field, 0, wxALIGN_CENTER_VERTICAL);

	camera_sizer->Add(camera_grid_sizer, 0, wxEXPAND);
	camera_sizer->AddStretchSpacer();
	physics_sizer->Add(camera_sizer, 0);	

	mainbox->Add(physics_sizer, 5, wxALL, 10);
	mainbox->Show(physics_sizer, false);
}

void PhysicsView::CreateShots()
{
	shots_sizer = new wxBoxSizer(wxHORIZONTAL);
	
	wxStaticBoxSizer* shots_static_sizer = new wxStaticBoxSizer(wxHORIZONTAL, mFrame, _("Shot"));
	
	wxBoxSizer* column_1_sizer = new wxBoxSizer(wxVERTICAL);
	
	wxFlexGridSizer* shape_sizer = new wxFlexGridSizer(2);
	shape_sizer->AddGrowableCol(1);
	shape_sizer->SetHGap(10);

	shape_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Graphic Collection:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_collection_field = new wxTextCtrl(mFrame, wxID_ANY);
	shape_sizer->Add(shots_collection_field, 0, wxALIGN_CENTER_VERTICAL);

	shape_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Color Table:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_color_table_field = new wxTextCtrl(mFrame, wxID_ANY);
	shape_sizer->Add(shots_color_table_field, 0, wxALIGN_CENTER_VERTICAL);

	shape_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Sequence:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_sequence_field = new wxTextCtrl(mFrame, wxID_ANY);
	shape_sizer->Add(shots_sequence_field, 0, wxALIGN_CENTER_VERTICAL);

	column_1_sizer->Add(shape_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* damage_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Damage"));

	wxFlexGridSizer* damage_grid_sizer = new wxFlexGridSizer(2);
	damage_grid_sizer->AddGrowableCol(0);
	damage_grid_sizer->SetHGap(10);

	damage_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Damage Type:")), 0, wxALIGN_CENTER_VERTICAL);
	
	std::vector<wxString> damage_strings = DefaultNames::Instance()->GetArray(wxT("damage"));
	damage_strings.insert(damage_strings.begin(), _("None"));

	shots_damage_type_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, damage_strings.size(), &damage_strings[0]);
	damage_grid_sizer->Add(shots_damage_type_choice, 0, wxALIGN_CENTER_VERTICAL);

	damage_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Damage Base:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_damage_base_field = new wxTextCtrl(mFrame, wxID_ANY);
	damage_grid_sizer->Add(shots_damage_base_field, 0, wxALIGN_CENTER_VERTICAL);

	damage_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Damage Random:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_damage_random_field = new wxTextCtrl(mFrame, wxID_ANY);
	damage_grid_sizer->Add(shots_damage_random_field, 0, wxALIGN_CENTER_VERTICAL);

	damage_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Damage Scale:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_damage_scale_field = new wxTextCtrl(mFrame, wxID_ANY);
	damage_grid_sizer->Add(shots_damage_scale_field, 0, wxALIGN_CENTER_VERTICAL);
	
	damage_sizer->Add(damage_grid_sizer, 0, wxEXPAND);
	
	shots_alien_damage_checkbox = new wxCheckBox(mFrame, wxID_ANY, _("Alien Damage (varies with level)"));
	damage_sizer->Add(shots_alien_damage_checkbox);

	column_1_sizer->Add(damage_sizer, 0, wxEXPAND | wxALL, 10);

	wxFlexGridSizer* shots_grid_sizer = new wxFlexGridSizer(2);
	shots_grid_sizer->AddGrowableCol(0);
	shots_grid_sizer->SetHGap(10);
	
	std::vector<wxString> sound_strings = DefaultNames::Instance()->GetArray(wxT("sound"));
	sound_strings.insert(sound_strings.begin(), GetName(wxT("sound"), -1));

	shots_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Flyby Sound:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_flyby_sound_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	shots_grid_sizer->Add(shots_flyby_sound_choice, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Rebound Sound:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_rebound_sound_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	shots_grid_sizer->Add(shots_rebound_sound_choice, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Sound Pitch:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_sound_pitch_field = new wxTextCtrl(mFrame, wxID_ANY);
	shots_grid_sizer->Add(shots_sound_pitch_field, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->AddSpacer(10);
	shots_grid_sizer->AddSpacer(10);

	shots_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Radius:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_radius_field = new wxTextCtrl(mFrame, wxID_ANY);
	shots_grid_sizer->Add(shots_radius_field, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Area of Effect:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_area_of_effect_field = new wxTextCtrl(mFrame, wxID_ANY);
	shots_grid_sizer->Add(shots_area_of_effect_field, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Speed:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_speed_field = new wxTextCtrl(mFrame, wxID_ANY);
	shots_grid_sizer->Add(shots_speed_field, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Maximum Range:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_range_field = new wxTextCtrl(mFrame, wxID_ANY);
	shots_grid_sizer->Add(shots_range_field, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->AddSpacer(10);
	shots_grid_sizer->AddSpacer(10);

	std::vector<wxString> effect_strings = DefaultNames::Instance()->GetArray(wxT("effect"));
	effect_strings.insert(effect_strings.begin(), GetName(wxT("sound"), -1));
	
	shots_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Detonation Effect:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_detonation_effect_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, effect_strings.size(), &effect_strings[0]);
	shots_grid_sizer->Add(shots_detonation_effect_choice, 0, wxALIGN_CENTER_VERTICAL);

	static const wxString media_effect_strings[] = {
		_("None"),
		_("Small Splash"),
		_("Medium Splash"),
	};

	shots_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Media Detonation Effect:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_media_detonation_effect_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, 3, media_effect_strings);
	shots_grid_sizer->Add(shots_media_detonation_effect_choice, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->AddSpacer(10);
	shots_grid_sizer->AddSpacer(10);

	shots_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Contrail:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_contrail_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, effect_strings.size(), &effect_strings[0]);
	shots_grid_sizer->Add(shots_contrail_choice, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Ticks between contrails:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_contrail_ticks_field = new wxTextCtrl(mFrame, wxID_ANY);
	shots_grid_sizer->Add(shots_contrail_ticks_field, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Maximum contrails:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_maximum_contrails_field = new wxTextCtrl(mFrame, wxID_ANY);
	shots_grid_sizer->Add(shots_maximum_contrails_field, 0, wxALIGN_CENTER_VERTICAL);

	column_1_sizer->Add(shots_grid_sizer);

	shots_static_sizer->Add(column_1_sizer, 0, wxEXPAND);
	shots_static_sizer->AddSpacer(10);

	wxBoxSizer* column_2_sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* flags_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Flags"));

	static const wxString flags_strings[] = {
		_("Guided"),
		_("Stop when animation loops"),
		_("Persistent"),
		_("Alien"),
		_("Affected by gravity"),
		_("No horizontal error"),
		_("No vertical error"),
		_("Can toggle control panels"),
		_("Positive vertical error"),
		_("Melee projectile"),
		_("Persistent and virulent"),
		_("Usually pass transparent side"),
		_("Sometimes pass transparent side"),
		_("Doubly affected by gravity"),
		_("Rebounds from floor"),
		_("Penetrates liquids"),
		_("Becomes item on detonation"),
		_("Bleeding projectile"),
		_("Horizontal wander"),
		_("Vertical wander"),
		_("Affected by half gravity"),
		_("Penetrates liquid boundary (inf)")
	};


	for (int i = 0; i < 22; ++i) {
		shots_flags_checkboxes[i] = new wxCheckBox(mFrame, wxID_ANY, flags_strings[i]);
		flags_sizer->Add(shots_flags_checkboxes[i]);
	}

	column_2_sizer->Add(flags_sizer, 0, wxEXPAND | wxTOP | wxRIGHT | wxBOTTOM, 10);

	wxBoxSizer* media_impact_sizer = new wxBoxSizer(wxHORIZONTAL);
	
	media_impact_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Media impact:")), 0, wxALIGN_CENTER_VERTICAL);
	media_impact_sizer->AddSpacer(10);

	std::vector<wxString> shot_strings = DefaultNames::Instance()->GetArray(wxT("shot"));
	shot_strings.insert(shot_strings.begin(), GetName(wxT("shot"), -1));

	shots_media_impact_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, shot_strings.size(), &shot_strings[0]);
	media_impact_sizer->Add(shots_media_impact_choice, 0, wxALIGN_CENTER_VERTICAL);
	
	column_2_sizer->Add(media_impact_sizer);

	shots_static_sizer->Add(column_2_sizer, 0, wxEXPAND);

	shots_sizer->Add(shots_static_sizer);

	mainbox->Add(shots_sizer, 5, wxALL, 10);
	mainbox->Show(shots_sizer, false);
}

void PhysicsView::CreateWeapons()
{
	weapons_definitions_sizer = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* column_1_sizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* item_type_sizer = new wxBoxSizer(wxHORIZONTAL);

	std::vector<wxString> item_strings = DefaultNames::Instance()->GetArray(wxT("item"));
	item_strings.insert(item_strings.begin(), _("None"));

	item_type_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Item Type:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_item_type_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, item_strings.size(), &item_strings[0]);
	item_type_sizer->AddSpacer(10);
	item_type_sizer->Add(weapon_item_type_choice, 0, wxALIGN_CENTER_VERTICAL);

	column_1_sizer->Add(item_type_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* appearance_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Appearance"));
	wxFlexGridSizer* appearance_grid_sizer = new wxFlexGridSizer(2);
	appearance_grid_sizer->AddGrowableCol(0);
	appearance_grid_sizer->SetHGap(0);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Graphic Collection:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_collection_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(weapon_collection_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Color Table:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_color_table_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(weapon_color_table_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Idle:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_idle_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(weapon_idle_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Firing:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_firing_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(weapon_firing_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Reloading:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_reloading_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(weapon_reloading_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Charging:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_charging_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(weapon_charging_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Charged:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_charged_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(weapon_charged_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->AddSpacer(10);
	appearance_grid_sizer->AddSpacer(10);

	appearance_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Flash Intensity:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_flash_intensity_field = new wxTextCtrl(mFrame, wxID_ANY);
	appearance_grid_sizer->Add(weapon_flash_intensity_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_sizer->Add(appearance_grid_sizer, 0, wxEXPAND);

	column_1_sizer->AddSpacer(10);
	column_1_sizer->Add(appearance_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* timing_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Timing (all in ticks)"));
	wxFlexGridSizer* timing_grid_sizer = new wxFlexGridSizer(2);
	timing_grid_sizer->AddGrowableCol(0);
	timing_grid_sizer->SetHGap(10);

	timing_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Ready:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_ready_field = new wxTextCtrl(mFrame, wxID_ANY);
	timing_grid_sizer->Add(weapon_ready_field, 0, wxALIGN_CENTER_VERTICAL);	

	timing_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Await Reload:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_await_reload_field = new wxTextCtrl(mFrame, wxID_ANY);
	timing_grid_sizer->Add(weapon_await_reload_field, 0, wxALIGN_CENTER_VERTICAL);	

	timing_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Loading:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_loading_field = new wxTextCtrl(mFrame, wxID_ANY);
	timing_grid_sizer->Add(weapon_loading_field, 0, wxALIGN_CENTER_VERTICAL);	

	timing_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Finish Loading:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_finish_loading_field = new wxTextCtrl(mFrame, wxID_ANY);
	timing_grid_sizer->Add(weapon_finish_loading_field, 0, wxALIGN_CENTER_VERTICAL);

	timing_grid_sizer->AddSpacer(10);
	timing_grid_sizer->AddSpacer(10);

	timing_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Flash Decay:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_flash_decay_field = new wxTextCtrl(mFrame, wxID_ANY);
	timing_grid_sizer->Add(weapon_flash_decay_field, 0, wxALIGN_CENTER_VERTICAL);

	timing_sizer->Add(timing_grid_sizer, 0, wxEXPAND);

	column_1_sizer->AddSpacer(10);
	column_1_sizer->Add(timing_sizer, 0, wxEXPAND);

	weapons_definitions_sizer->Add(column_1_sizer);

	wxBoxSizer* column_2_sizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* weapon_class_sizer = new wxBoxSizer(wxHORIZONTAL);

	const wxString weapon_class_names[] = {
		_("None"),
		_("Melee"),
		_("Single Function"),
		_("Dual Function"),
		_("Two-Fisted"),
		_("Multipurpose")
	};

	weapon_class_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Weapon Class:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_class_choice = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, 6, weapon_class_names);
	weapon_class_sizer->AddSpacer(10);
	weapon_class_sizer->Add(weapon_class_choice, 0, wxALIGN_CENTER_VERTICAL);

	column_2_sizer->Add(weapon_class_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* height_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Height and Idle"));
	wxFlexGridSizer* height_grid_sizer = new wxFlexGridSizer(2);
	height_grid_sizer->AddGrowableCol(0);
	height_grid_sizer->SetHGap(10);

	height_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Idle Height:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_idle_height_field = new wxTextCtrl(mFrame, wxID_ANY);
	height_grid_sizer->Add(weapon_idle_height_field, 0, wxALIGN_CENTER_VERTICAL);	

	height_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Bob Amplitude:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_bob_amplitude_field = new wxTextCtrl(mFrame, wxID_ANY);
	height_grid_sizer->Add(weapon_bob_amplitude_field, 0, wxALIGN_CENTER_VERTICAL);	

	height_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Kick Height:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_kick_height_field = new wxTextCtrl(mFrame, wxID_ANY);
	height_grid_sizer->Add(weapon_kick_height_field, 0, wxALIGN_CENTER_VERTICAL);	

	height_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Reload Height:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_reload_height_field = new wxTextCtrl(mFrame, wxID_ANY);
	height_grid_sizer->Add(weapon_reload_height_field, 0, wxALIGN_CENTER_VERTICAL);

	height_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Idle Width:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_idle_width_field = new wxTextCtrl(mFrame, wxID_ANY);
	height_grid_sizer->Add(weapon_idle_width_field, 0, wxALIGN_CENTER_VERTICAL);

	height_sizer->Add(height_grid_sizer, 0, wxEXPAND);
	
	column_2_sizer->AddSpacer(10);
	column_2_sizer->Add(height_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* flags_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Flags"));
	static const wxString flags_strings[] = {
		_("Automatic"),
		_("Disappears after use"),
		_("Plays instant shell casing sound"),
		_("Overloads"),
		_("Random ammo on pickup"),
		_("Reloads in one hand"),
		_("Fires out of phase"),
		_("Fires under liquids"),
		_("Triggers share ammo"),
		_("Angular flipping on 2nd trigger")
	};

	for (int i = 0; i < 10; ++i) {
		weapon_flags_checkboxes[i] = new wxCheckBox(mFrame, wxID_ANY, flags_strings[i]);
		flags_sizer->Add(weapon_flags_checkboxes[i], 0, wxALIGN_CENTER_VERTICAL);
	}

	column_2_sizer->AddSpacer(10);
	column_2_sizer->Add(flags_sizer, 0, wxEXPAND);

	weapons_definitions_sizer->AddSpacer(10);
	weapons_definitions_sizer->Add(column_2_sizer);

	mainbox->Add(weapons_definitions_sizer, 5, wxALL, 10);
	mainbox->Show(weapons_definitions_sizer, false);

	weapons_triggers_sizer = new wxBoxSizer(wxVERTICAL);

	static const wxString trigger_names[] = {
		_("Main Trigger"),
		_("Secondary Trigger")
	};

	std::vector<wxString> shot_strings = DefaultNames::Instance()->GetArray(wxT("shot"));
	shot_strings.insert(shot_strings.begin(), _("None"));

	std::vector<wxString> sound_strings = DefaultNames::Instance()->GetArray(wxT("sound"));
	sound_strings.insert(sound_strings.begin(), _("None"));

	for (int i = 0; i < 2; ++i) {
		wxStaticBoxSizer* static_box_sizer = new wxStaticBoxSizer(wxHORIZONTAL, mFrame, trigger_names[i]);
		wxBoxSizer* column_1_sizer = new wxBoxSizer(wxVERTICAL);

		wxFlexGridSizer* left_grid_sizer = new wxFlexGridSizer(2);
		left_grid_sizer->AddGrowableCol(0);
		left_grid_sizer->SetHGap(10);

		left_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Projectile:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_projectile_choices[i] = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, shot_strings.size(), &shot_strings[0]);
		left_grid_sizer->Add(trigger_projectile_choices[i]);

		left_grid_sizer->AddSpacer(10);
		left_grid_sizer->AddSpacer(10);

		left_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Rounds/magazine:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_rounds_fields[i] = new wxTextCtrl(mFrame, wxID_ANY);
		left_grid_sizer->Add(trigger_rounds_fields[i], 0, wxALIGN_CENTER_VERTICAL);

		column_1_sizer->Add(left_grid_sizer, 0, wxEXPAND);

		wxStaticBoxSizer* sounds_sizer = new wxStaticBoxSizer(wxVERTICAL, mFrame, _("Sounds"));
		wxFlexGridSizer* sounds_grid_sizer = new wxFlexGridSizer(2);
		sounds_grid_sizer->AddGrowableCol(0);
		sounds_grid_sizer->SetHGap(10);

		sounds_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Firing:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_firing_choices[i] = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
		sounds_grid_sizer->Add(trigger_firing_choices[i]);

		sounds_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Click:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_click_choices[i] = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
		sounds_grid_sizer->Add(trigger_click_choices[i]);

		sounds_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Charging:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_charging_choices[i] = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
		sounds_grid_sizer->Add(trigger_charging_choices[i]);

		sounds_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Shell Casing:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_shell_casing_choices[i] = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
		sounds_grid_sizer->Add(trigger_shell_casing_choices[i]);

		sounds_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Reloading:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_reloading_choices[i] = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
		sounds_grid_sizer->Add(trigger_reloading_choices[i]);

		sounds_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Charged:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_charged_choices[i] = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
		sounds_grid_sizer->Add(trigger_charged_choices[i]);

		sounds_sizer->Add(sounds_grid_sizer);

		column_1_sizer->Add(sounds_sizer, 0, wxEXPAND | wxALL, 10);

		static_box_sizer->Add(column_1_sizer, 0, wxEXPAND);

		wxFlexGridSizer* right_grid_sizer = new wxFlexGridSizer(2);
		right_grid_sizer->AddGrowableCol(0);
		right_grid_sizer->SetHGap(10);

		right_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Ammo Type:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_ammo_type_choices[i] = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, item_strings.size(), &item_strings[0]);
		right_grid_sizer->Add(trigger_ammo_type_choices[i]);

		right_grid_sizer->AddSpacer(10);
		right_grid_sizer->AddSpacer(10);

		right_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Ticks/round:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_ticks_fields[i] = new wxTextCtrl(mFrame, wxID_ANY);
		right_grid_sizer->Add(trigger_ticks_fields[i], 0, wxALIGN_CENTER_VERTICAL);		

		right_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Recovery Ticks:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_recovery_fields[i] = new wxTextCtrl(mFrame, wxID_ANY);
		right_grid_sizer->Add(trigger_recovery_fields[i], 0, wxALIGN_CENTER_VERTICAL);		

		right_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Charging Ticks:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_charging_fields[i] = new wxTextCtrl(mFrame, wxID_ANY);
		right_grid_sizer->Add(trigger_charging_fields[i], 0, wxALIGN_CENTER_VERTICAL);		

		right_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Recoil Magnitude:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_recoil_fields[i] = new wxTextCtrl(mFrame, wxID_ANY);
		right_grid_sizer->Add(trigger_recoil_fields[i], 0, wxALIGN_CENTER_VERTICAL);		

		right_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Theta Error:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_theta_fields[i] = new wxTextCtrl(mFrame, wxID_ANY);
		right_grid_sizer->Add(trigger_theta_fields[i], 0, wxALIGN_CENTER_VERTICAL);		

		right_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("dx:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_dx_fields[i] = new wxTextCtrl(mFrame, wxID_ANY);
		right_grid_sizer->Add(trigger_dx_fields[i], 0, wxALIGN_CENTER_VERTICAL);		

		right_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("dz:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_dz_fields[i] = new wxTextCtrl(mFrame, wxID_ANY);
		right_grid_sizer->Add(trigger_dz_fields[i], 0, wxALIGN_CENTER_VERTICAL);

		right_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Burst Count:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_burst_count_fields[i] = new wxTextCtrl(mFrame, wxID_ANY);
		right_grid_sizer->Add(trigger_burst_count_fields[i], 0, wxALIGN_CENTER_VERTICAL);

		right_grid_sizer->AddSpacer(10);
		right_grid_sizer->AddSpacer(10);

		static const wxString shell_casing_type_strings[] = {
			_("None"),
			_("Assault Rifle"),
			_("Pistol Center"),
			_("Pistol Left"),
			_("Pistol Right"),
			_("SMG")
		};

		right_grid_sizer->Add(new wxStaticText(mFrame, wxID_ANY, _("Shell casing type:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_shell_casing_type_choices[i] = new wxChoice(mFrame, wxID_ANY, wxDefaultPosition, choiceSize, 6, shell_casing_type_strings);
		right_grid_sizer->Add(trigger_shell_casing_type_choices[i]);		

		static_box_sizer->AddSpacer(10);
		static_box_sizer->Add(right_grid_sizer, 0, wxEXPAND);
		
		if (i == 1) {
			weapons_triggers_sizer->AddSpacer(10);
		}

		weapons_triggers_sizer->Add(static_box_sizer);
	}

	mainbox->Add(weapons_triggers_sizer, 5, wxALL, 10);
	mainbox->Show(weapons_triggers_sizer, false);
}

bool PhysicsView::OnCreate(wxDocument* doc, long flags)
{
	wxString frameTitle = wxT("ShapeFusion : Physics : ");
	frameTitle.Append(doc->GetFilename());

	mFrame = wxGetApp().CreateChildFrame(doc, this, frameTitle, wxDefaultPosition, wxDefaultSize);

	mainbox = new wxBoxSizer(wxHORIZONTAL);
	tree = new wxTreeCtrl(mFrame, -1, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT);
	tree->DeleteAllItems();
	wxTreeItemId treeroot = tree->AddRoot(doc->GetFilename());
	mainbox->Add(tree, 2, wxEXPAND);

	// empty space
	dummy_sizer = new wxBoxSizer(wxVERTICAL);
	wxPanel* dummy_panel = new wxPanel(mFrame);
	dummy_sizer->Add(dummy_panel, 1, wxEXPAND);
	mainbox->Add(dummy_sizer, 5);

	CreateAliens();
	CreateEffects();
	CreatePhysicsConstants();
	CreateShots();
	CreateWeapons();

	mainbox->Show(dummy_sizer, false);
	mainbox->Show(shots_sizer, true);

	mainbox->Layout();
	mFrame->SetSizerAndFit(mainbox);

	mainbox->Show(dummy_sizer, true);
	mainbox->Show(shots_sizer, false);

	// because we changed size
	mFrame->Centre(wxBOTH);

	mFrame->Show(true);
	Activate(true);

	return true;
}

void PhysicsView::OnUpdate(wxView*, wxObject*)
{
	wxTreeItemId aliens = tree->AppendItem(tree->GetRootItem(), _("Aliens"), -1, -1, new PhysicsTreeItemData());
	unsigned int monster_count = static_cast<PhysicsDocument*>(GetDocument())->MonsterCount();
	for (unsigned int i = 0; i < monster_count; ++i) {
		wxTreeItemId alien = tree->AppendItem(aliens, GetName(wxT("alien"), i), -1, -1, new PhysicsTreeItemData(i, TREESECTION_MONSTERS));
		
		tree->AppendItem(alien, _("Appearance and Sounds"), -1, -1, new PhysicsTreeItemData(i, TREESECTION_MONSTERS_APPEARANCE));
		tree->AppendItem(alien, _("Combat Settings"), -1, -1, new PhysicsTreeItemData(i, TREESECTION_MONSTERS_COMBAT));
		tree->AppendItem(alien, _("Physical Constants"), -1, -1, new PhysicsTreeItemData(i, TREESECTION_MONSTERS_CONSTANTS));
		tree->AppendItem(alien, _("Behavior Settings"), -1, -1, new PhysicsTreeItemData(i, TREESECTION_MONSTERS_BEHAVIOR));
		tree->AppendItem(alien, _("Immunities and Weaknesses"), -1, -1, new PhysicsTreeItemData(i, TREESECTION_MONSTERS_IMMUNITIES));
	}

	wxTreeItemId effects = tree->AppendItem(tree->GetRootItem(), _("Effects"), -1, -1, new PhysicsTreeItemData());
	unsigned int effect_count = static_cast<PhysicsDocument*>(GetDocument())->EffectCount();
	for (unsigned int i = 0; i < effect_count; ++i) {
		tree->AppendItem(effects, GetName(wxT("effect"), i), -1, -1, new PhysicsTreeItemData(i, TREESECTION_EFFECTS));
	}

	wxTreeItemId shots = tree->AppendItem(tree->GetRootItem(), _("Shots"), -1, -1, new PhysicsTreeItemData());
	unsigned int projectile_count = static_cast<PhysicsDocument*>(GetDocument())->ProjectileCount();
	for (unsigned int i = 0; i < projectile_count; ++i) {
		tree->AppendItem(shots, GetName(wxT("shot"), i), -1 ,-1, new PhysicsTreeItemData(i, TREESECTION_PROJECTILES));
	}

	wxTreeItemId physics = tree->AppendItem(tree->GetRootItem(), _("Physics"), -1, -1, new PhysicsTreeItemData());
	tree->AppendItem(physics, _("Walking"), -1, -1, new PhysicsTreeItemData(0, TREESECTION_PHYSICS));
	tree->AppendItem(physics, _("Running"), -1, -1, new PhysicsTreeItemData(1, TREESECTION_PHYSICS));

	wxTreeItemId weapons = tree->AppendItem(tree->GetRootItem(), _("Weapons"), -1, -1, new PhysicsTreeItemData());
	unsigned int weapon_count = static_cast<PhysicsDocument*>(GetDocument())->WeaponCount();
	for (unsigned i = 0; i < weapon_count; ++i) {
		wxTreeItemId weapon = tree->AppendItem(weapons, GetName(wxT("weapon"), i), -1, -1, new PhysicsTreeItemData(i, TREESECTION_WEAPONS));
		
		tree->AppendItem(weapon, _("Weapon Definition"), -1, -1, new PhysicsTreeItemData(i, TREESECTION_WEAPONS_SETTINGS));
		tree->AppendItem(weapon, _("Trigger Settings"), -1, -1, new PhysicsTreeItemData(i, TREESECTION_WEAPONS_TRIGGERS));
	}
}

void PhysicsView::OnDraw(wxDC* dc)
{
}

bool PhysicsView::OnClose(bool deleteWindow)
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

void PhysicsView::OnSelectPhysicsConstants(int index)
{
	PhysicsConstants* constants = static_cast<PhysicsDocument*>(GetDocument())->GetPhysicsConstants(index);

	max_forward_velocity_field->ChangeValue(Format(constants->GetMaximumForwardVelocity()));
	max_backward_velocity_field->ChangeValue(Format(constants->GetMaximumBackwardVelocity()));
	max_perpendicular_velocity_field->ChangeValue(Format(constants->GetMaximumPerpendicularVelocity()));

	acceleration_field->ChangeValue(Format(constants->GetAcceleration()));
	deceleration_field->ChangeValue(Format(constants->GetDeceleration()));
	airborne_deceleration_field->ChangeValue(Format(constants->GetAirborneDeceleration()));
	gravitational_acceleration_field->ChangeValue(Format(constants->GetGravitationalAcceleration()));
	climbing_acceleration_field->ChangeValue(Format(constants->GetClimbingAcceleration()));
	terminal_velocity_field->ChangeValue(Format(constants->GetTerminalVelocity()));
	external_deceleration_field->ChangeValue(Format(constants->GetExternalDeceleration()));

	step_delta_field->ChangeValue(Format(constants->GetStepDelta()));
	step_amplitude_field->ChangeValue(Format(constants->GetStepAmplitude()));

	radius_field->ChangeValue(Format(constants->GetRadius()));
	height_field->ChangeValue(Format(constants->GetHeight()));

	angular_acceleration_field->ChangeValue(Format(constants->GetAngularAcceleration()));
	angular_deceleration_field->ChangeValue(Format(constants->GetAngularDeceleration()));
	maximum_angular_velocity_field->ChangeValue(Format(constants->GetMaximumAngularVelocity()));
	angular_recentering_velocity_field->ChangeValue(Format(constants->GetAngularRecenteringVelocity()));
	head_angular_velocity_field->ChangeValue(Format(constants->GetFastAngularVelocity()));
	head_angular_maximum_field->ChangeValue(Format(constants->GetFastAngularMaximum()));
	maximum_elevation_field->ChangeValue(Format(constants->GetMaximumElevation()));
	external_angular_deceleration_field->ChangeValue(Format(constants->GetExternalAngularDeceleration()));

	dead_height_field->ChangeValue(Format(constants->GetDeadHeight()));
	camera_height_field->ChangeValue(Format(constants->GetCameraHeight()));
	splash_height_field->ChangeValue(Format(constants->GetSplashHeight()));
	half_camera_separation_field->ChangeValue(Format(constants->GetHalfCameraSeparation()));
	
	mainbox->Show(physics_sizer, true);
}

void PhysicsView::OnSelectAlienAppearance(int index)
{
	MonsterDefinition* monster = static_cast<PhysicsDocument*>(GetDocument())->GetMonsterDefinition(index);

	alien_collection_field->ChangeValue(Format(monster->GetCollection()));
	alien_color_table_field->ChangeValue(Format(monster->GetColorTable()));
	
	alien_hit_field->ChangeValue(Format(monster->GetHitShapes()));
	alien_hard_dying_field->ChangeValue(Format(monster->GetHardDyingShape()));
	alien_soft_dying_field->ChangeValue(Format(monster->GetSoftDyingShape()));
	alien_hard_dead_field->ChangeValue(Format(monster->GetHardDeadShapes()));
	alien_soft_dead_field->ChangeValue(Format(monster->GetSoftDeadShapes()));
	alien_stationary_field->ChangeValue(Format(monster->GetStationaryShape()));
	alien_moving_field->ChangeValue(Format(monster->GetMovingShape()));
	alien_teleport_in_field->ChangeValue(Format(monster->GetTeleportInShape()));
	alien_teleport_out_field->ChangeValue(Format(monster->GetTeleportOutShape()));
	alien_melee_attack_shape_field->ChangeValue(Format(monster->GetMeleeAttack()->GetShape()));
	alien_ranged_attack_shape_field->ChangeValue(Format(monster->GetRangedAttack()->GetShape()));

	alien_activation_choice->SetSelection(monster->GetActivationSound() + 1);
	alien_friendly_activation_choice->SetSelection(monster->GetFriendlyActivationSound() + 1);
	alien_clear_choice->SetSelection(monster->GetClearSound() + 1);
	alien_kill_choice->SetSelection(monster->GetKillSound() + 1);
	alien_apology_choice->SetSelection(monster->GetApologySound() + 1);
	alien_friendly_fire_choice->SetSelection(monster->GetFriendlyFireSound() + 1);
	alien_flaming_choice->SetSelection(monster->GetFlamingSound() + 1);
	alien_random_choice->SetSelection(monster->GetRandomSound() + 1);

	alien_random_sound_mask_field->ChangeValue(Format(monster->GetRandomSoundMask()));
	alien_sound_pitch_field->ChangeValue(Format(monster->GetSoundPitch()));
	
	mainbox->Show(aliens_appearance_sizer, true);
}

void PhysicsView::OnSelectAlienCombat(int index)
{
	MonsterDefinition* monster = static_cast<PhysicsDocument*>(GetDocument())->GetMonsterDefinition(index);

	alien_attack_frequency_field->ChangeValue(Format(monster->GetAttackFrequency()));
	
	AttackDefinition* melee = monster->GetMeleeAttack();
	alien_melee_attack_type_choice->SetSelection(melee->GetType() + 1);
	alien_melee_attack_repetitions_field->ChangeValue(Format(melee->GetRepetitions()));
	// Anvil displays the raw angle units here...
	alien_melee_attack_error_field->ChangeValue(Format(static_cast<short>(std::floor(melee->GetError() * 512.0 / 360.0 + 0.5))));
	alien_melee_attack_range_field->ChangeValue(Format(melee->GetRange()));
	alien_melee_attack_sequence_field->ChangeValue(Format(melee->GetShape()));
	alien_melee_attack_dx_field->ChangeValue(Format(melee->GetDx()));
	alien_melee_attack_dy_field->ChangeValue(Format(melee->GetDy()));
	alien_melee_attack_dz_field->ChangeValue(Format(melee->GetDz()));

	AttackDefinition* ranged = monster->GetRangedAttack();
	alien_ranged_attack_type_choice->SetSelection(ranged->GetType() + 1);
	alien_ranged_attack_repetitions_field->ChangeValue(Format(ranged->GetRepetitions()));
	// Anvil displays the raw angle units here...
	alien_ranged_attack_error_field->ChangeValue(Format(static_cast<short>(std::floor(ranged->GetError() * 512.0 / 360.09 + 0.5))));
	alien_ranged_attack_range_field->ChangeValue(Format(ranged->GetRange()));
	alien_ranged_attack_sequence_field->ChangeValue(Format(ranged->GetShape()));
	alien_ranged_attack_dx_field->ChangeValue(Format(ranged->GetDx()));
	alien_ranged_attack_dy_field->ChangeValue(Format(ranged->GetDy()));
	alien_ranged_attack_dz_field->ChangeValue(Format(ranged->GetDz()));

	alien_shrapnel_radius_field->ChangeValue(Format(monster->GetShrapnelRadius()));
	
	DamageDefinition* shrapnel = monster->GetShrapnelDamage();
	alien_shrapnel_damage_type_choice->SetSelection(shrapnel->GetType() + 1);
	alien_shrapnel_base_damage_field->ChangeValue(Format(shrapnel->GetBase()));
	alien_shrapnel_random_damage_field->ChangeValue(Format(shrapnel->GetRandom()));
	alien_shrapnel_damage_scale_field->ChangeValue(Format(shrapnel->GetScale()));
	alien_shrapnel_alien_damage_checkbox->SetValue(shrapnel->GetAlien());

	alien_ranged_impact_effect_choice->SetSelection(monster->GetImpactEffect() + 1);
	alien_melee_impact_effect_choice->SetSelection(monster->GetMeleeImpactEffect() + 1);

	mainbox->Show(aliens_combat_sizer, true);
}

void PhysicsView::OnSelectAlienConstants(int index)
{
	MonsterDefinition* monster = static_cast<PhysicsDocument*>(GetDocument())->GetMonsterDefinition(index);

	alien_vitality_field->ChangeValue(Format(monster->GetVitality()));

	alien_radius_field->ChangeValue(Format(monster->GetRadius()));
	alien_height_field->ChangeValue(Format(monster->GetHeight()));

	alien_speed_field->ChangeValue(Format(monster->GetSpeed()));

	alien_terminal_velocity_field->ChangeValue(Format(monster->GetTerminalVelocity()));
	alien_gravity_field->ChangeValue(Format(monster->GetGravity()));

	alien_min_ledge_jump_field->ChangeValue(Format(monster->GetMinimumLedgeDelta()));
	alien_max_ledge_jump_field->ChangeValue(Format(monster->GetMaximumLedgeDelta()));
	alien_ext_velocity_scale_field->ChangeValue(Format(monster->GetExternalVelocityScale()));
	alien_hover_height_field->ChangeValue(Format(monster->GetPreferredHoverHeight()));
	
	alien_door_retry_mask_field->ChangeValue(Format(monster->GetDoorRetryMask()));

	alien_visual_range_field->ChangeValue(Format(monster->GetVisualRange()));
	alien_dark_visual_range_field->ChangeValue(Format(monster->GetDarkVisualRange()));
	alien_intelligence_field->ChangeValue(Format(monster->GetIntelligence()));

	alien_carrying_item_choice->SetSelection(monster->GetCarryingItemType() + 1);
	alien_contrail_effect_choice->SetSelection(monster->GetContrailEffect() + 1);

	mainbox->Show(aliens_constants_sizer, true);
}

void PhysicsView::OnSelectAlienBehavior(int index)
{
	MonsterDefinition* monster = static_cast<PhysicsDocument*>(GetDocument())->GetMonsterDefinition(index);

	alien_class_choice->SetSelection(monster->GetClass());

	for (int i = 0; i < 16; ++i) {
		alien_friends_checkboxes[i]->SetValue(monster->GetFriend(i));
		alien_enemies_checkboxes[i]->SetValue(monster->GetEnemy(i));
	}

	for (int i = 0; i < 27; ++i) {
		alien_flags_checkboxes[i]->SetValue(monster->GetFlag(i + 1));
	}

	mainbox->Show(aliens_behavior_sizer, true);
}

void PhysicsView::OnSelectAlienImmunities(int index)
{
	MonsterDefinition* monster = static_cast<PhysicsDocument*>(GetDocument())->GetMonsterDefinition(index);

	for (int i = 0; i < 24; ++i) {
		alien_immunities_checkboxes[i]->SetValue(monster->GetImmunity(i));
		alien_weaknesses_checkboxes[i]->SetValue(monster->GetWeakness(i));
	}

	mainbox->Show(aliens_immunities_sizer, true);
}

void PhysicsView::OnSelectEffect(int index)
{
	EffectDefinition* effect = static_cast<PhysicsDocument*>(GetDocument())->GetEffectDefinition(index);
	
	eff_collection_field->ChangeValue(Format(effect->GetCollection()));
	eff_color_table_field->ChangeValue(Format(effect->GetColorTable()));
	eff_sequence_field->ChangeValue(Format(effect->GetShape()));
	
	eff_delay_field->ChangeValue(Format(effect->GetDelay()));
	eff_pitch_field->ChangeValue(Format(effect->GetSoundPitch()));

	eff_delay_sound_choice->SetSelection(effect->GetDelaySound() + 1);
	
	eff_end_when_animation_loops_checkbox->SetValue(effect->GetEndWhenAnimationLoops());
	eff_end_when_transfer_animation_loops_checkbox->SetValue(effect->GetEndWhenTransferAnimationLoops());
	eff_sound_only_checkbox->SetValue(effect->GetSoundOnly());
	eff_media_effect_checkbox->SetValue(effect->GetMediaEffect());
	
	mainbox->Show(effects_sizer, true);
}

void PhysicsView::OnSelectShot(int index)
{
	ProjectileDefinition* projectile = static_cast<PhysicsDocument*>(GetDocument())->GetProjectileDefinition(index);
	
	shots_collection_field->ChangeValue(Format(projectile->GetCollection()));
	shots_color_table_field->ChangeValue(Format(projectile->GetColorTable()));
	shots_sequence_field->ChangeValue(Format(projectile->GetShape()));

	DamageDefinition* damage = projectile->GetDamage();

	shots_damage_type_choice->SetSelection(damage->GetType() + 1);
	shots_damage_base_field->ChangeValue(Format(damage->GetBase()));
	shots_damage_random_field->ChangeValue(Format(damage->GetRandom()));
	shots_damage_scale_field->ChangeValue(Format(damage->GetScale()));
	shots_alien_damage_checkbox->SetValue(damage->GetAlien());

	shots_flyby_sound_choice->SetSelection(projectile->GetFlybySound() + 1);
	shots_rebound_sound_choice->SetSelection(projectile->GetReboundSound() + 1);
	shots_sound_pitch_field->ChangeValue(Format(projectile->GetSoundPitch()));
	
	shots_radius_field->ChangeValue(Format(projectile->GetRadius()));
	shots_area_of_effect_field->ChangeValue(Format(projectile->GetAreaOfEffect()));
	shots_speed_field->ChangeValue(Format(projectile->GetSpeed()));
	shots_range_field->ChangeValue(Format(projectile->GetMaximumRange()));

	shots_detonation_effect_choice->SetSelection(projectile->GetDetonationEffect() + 1);
	shots_media_detonation_effect_choice->SetSelection(projectile->GetMediaDetonationEffect() + 1);
	shots_contrail_choice->SetSelection(projectile->GetContrailEffect() + 1);
	shots_contrail_ticks_field->ChangeValue(Format(projectile->GetTicksBetweenContrails()));
	shots_maximum_contrails_field->ChangeValue(Format(projectile->GetMaximumContrails()));

	for (int i = 0; i < 22; ++i) {
		shots_flags_checkboxes[i]->SetValue(projectile->GetFlag(i));
	}

	shots_media_impact_choice->SetSelection(projectile->GetMediaProjectilePromotion() + 1);

	mainbox->Show(shots_sizer, true);
}

void PhysicsView::OnSelectWeaponDefinition(int index)
{
	WeaponDefinition* weapon = static_cast<PhysicsDocument*>(GetDocument())->GetWeaponDefinition(index);

	weapon_item_type_choice->SetSelection(weapon->GetItemType() + 1);

	weapon_collection_field->ChangeValue(Format(weapon->GetCollection()));
	weapon_color_table_field->ChangeValue(Format(weapon->GetColorTable()));
	weapon_idle_field->ChangeValue(Format(weapon->GetIdleShape()));
	weapon_firing_field->ChangeValue(Format(weapon->GetFiringShape()));
	weapon_reloading_field->ChangeValue(Format(weapon->GetReloadingShape()));
	weapon_charging_field->ChangeValue(Format(weapon->GetChargingShape()));
	weapon_charged_field->ChangeValue(Format(weapon->GetChargedShape()));
	weapon_flash_intensity_field->ChangeValue(Format(weapon->GetFiringLightIntensity()));

	weapon_ready_field->ChangeValue(Format(weapon->GetReadyTicks()));
	weapon_await_reload_field->ChangeValue(Format(weapon->GetAwaitReloadTicks()));
	weapon_loading_field->ChangeValue(Format(weapon->GetLoadingTicks()));
	weapon_finish_loading_field->ChangeValue(Format(weapon->GetFinishLoadingTicks()));
	weapon_flash_decay_field->ChangeValue(Format(weapon->GetFiringIntensityDecayTicks()));

	weapon_class_choice->SetSelection(weapon->GetWeaponClass() + 1);
	
	weapon_idle_height_field->ChangeValue(Format(weapon->GetIdleHeight()));
	weapon_bob_amplitude_field->ChangeValue(Format(weapon->GetBobAmplitude()));
	weapon_kick_height_field->ChangeValue(Format(weapon->GetKickHeight()));
	weapon_reload_height_field->ChangeValue(Format(weapon->GetReloadHeight()));
	weapon_idle_width_field->ChangeValue(Format(weapon->GetIdleWidth()));

	for (int i = 0; i < 10; ++i) {
		weapon_flags_checkboxes[i]->SetValue(weapon->GetFlag(i >= 5 ? i + 1 : i));
	}

	mainbox->Show(weapons_definitions_sizer, true);
}

void PhysicsView::OnSelectWeaponTriggers(int index)
{
	WeaponDefinition* weapon = static_cast<PhysicsDocument*>(GetDocument())->GetWeaponDefinition(index);

	for (int i = 0; i < 2; ++i) {
		TriggerDefinition* trigger = (i == 0) ? weapon->GetPrimaryTrigger() : weapon->GetSecondaryTrigger();

		trigger_projectile_choices[i]->SetSelection(trigger->GetProjectileType() + 1);
		trigger_rounds_fields[i]->ChangeValue(Format(trigger->GetRoundsPerMagazine()));
		trigger_ammo_type_choices[i]->SetSelection(trigger->GetAmmunitionType() + 1);

		trigger_firing_choices[i]->SetSelection(trigger->GetFiringSound() + 1);
		trigger_click_choices[i]->SetSelection(trigger->GetClickSound() + 1);
		trigger_charging_choices[i]->SetSelection(trigger->GetChargingSound() + 1);
		trigger_shell_casing_choices[i]->SetSelection(trigger->GetShellCasingSound() + 1);
		trigger_reloading_choices[i]->SetSelection(trigger->GetReloadingSound() + 1);
		trigger_charged_choices[i]->SetSelection(trigger->GetChargedSound() + 1);

		trigger_ticks_fields[i]->ChangeValue(Format(trigger->GetTicksPerRound()));
		trigger_recovery_fields[i]->ChangeValue(Format(trigger->GetRecoveryTicks()));
		trigger_charging_fields[i]->ChangeValue(Format(trigger->GetChargingTicks()));
		trigger_recoil_fields[i]->ChangeValue(Format(trigger->GetRecoilMagnitude()));
		trigger_theta_fields[i]->ChangeValue(Format(trigger->GetThetaError()));
		trigger_dx_fields[i]->ChangeValue(Format(trigger->GetDx()));
		trigger_dz_fields[i]->ChangeValue(Format(trigger->GetDz()));
		trigger_burst_count_fields[i]->ChangeValue(Format(trigger->GetBurstCount()));

		trigger_shell_casing_type_choices[i]->SetSelection(trigger->GetShellCasingType() + 1);

	}

	mainbox->Show(weapons_triggers_sizer, true);
}

void PhysicsView::OnTreeSelect(wxTreeEvent& e)
{
	PhysicsTreeItemData* data = dynamic_cast<PhysicsTreeItemData*>(tree->GetItemData(e.GetItem()));
	if (data) {
		mainbox->Show(aliens_appearance_sizer, false);
		mainbox->Show(aliens_behavior_sizer, false);
		mainbox->Show(aliens_combat_sizer, false);
		mainbox->Show(aliens_constants_sizer, false);
		mainbox->Show(aliens_immunities_sizer, false);
		mainbox->Show(dummy_sizer, false);
		mainbox->Show(effects_sizer, false);
		mainbox->Show(shots_sizer, false);
		mainbox->Show(physics_sizer, false);
		mainbox->Show(weapons_definitions_sizer, false);
		mainbox->Show(weapons_triggers_sizer, false);

		int new_section = data->Section();
		switch (new_section) {
		case TREESECTION_PHYSICS:
			OnSelectPhysicsConstants(data->ID());
			break;
		case TREESECTION_EFFECTS:
			OnSelectEffect(data->ID());
			break;
		case TREESECTION_PROJECTILES:
			OnSelectShot(data->ID());
			break;
		case TREESECTION_MONSTERS_APPEARANCE:
			OnSelectAlienAppearance(data->ID());
			break;
		case TREESECTION_MONSTERS_COMBAT:
			OnSelectAlienCombat(data->ID());
			break;
		case TREESECTION_MONSTERS_CONSTANTS:
			OnSelectAlienConstants(data->ID());
			break;
		case TREESECTION_MONSTERS_BEHAVIOR:
			OnSelectAlienBehavior(data->ID());
			break;
		case TREESECTION_MONSTERS_IMMUNITIES:
			OnSelectAlienImmunities(data->ID());
			break;
		case TREESECTION_WEAPONS_SETTINGS:
			OnSelectWeaponDefinition(data->ID());
			break;
		case TREESECTION_WEAPONS_TRIGGERS:
			OnSelectWeaponTriggers(data->ID());
			break;
		default:
			mainbox->Show(dummy_sizer, true);
			break;
		}

		mainbox->Layout();
	}
}

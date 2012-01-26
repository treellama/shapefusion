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

#include "PhysicsView.h"
#include "PhysicsTreeItemData.h"

#include "../DefaultNames.h"

const wxSize choiceSize(200, -1);


void PhysicsView::CreateAliens()
{
	aliens_appearance_sizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticBoxSizer* appearance_sizer = new wxStaticBoxSizer(wxHORIZONTAL, main_panel, _("Appearance"));

	wxFlexGridSizer* appearance_grid_sizer = new wxFlexGridSizer(2);
	appearance_grid_sizer->AddGrowableCol(0);
	appearance_grid_sizer->SetHGap(10);
	appearance_grid_sizer->SetVGap(4);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Graphic Collection:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_collection_field = new wxTextCtrl(main_panel, FIELD_ALIEN_COLLECTION);
	appearance_grid_sizer->Add(alien_collection_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Color Table:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_color_table_field = new wxTextCtrl(main_panel, FIELD_ALIEN_COLOR_TABLE);
	appearance_grid_sizer->Add(alien_color_table_field, 0, wxALIGN_CENTER_VERTICAL);

	wxStaticText* sequence_label = new wxStaticText(main_panel, wxID_ANY, _("Sequence IDs:"));
//	wxFont font = sequence_label->GetFont();
//	font.SetWeight(wxFONTWEIGHT_BOLD);
//	sequence_label->SetFont(font);
	appearance_grid_sizer->Add(sequence_label, 0, wxALIGN_CENTER_VERTICAL);
	appearance_grid_sizer->AddSpacer(30);
	
	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Hit:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_hit_field = new wxTextCtrl(main_panel, FIELD_ALIEN_HIT);
	appearance_grid_sizer->Add(alien_hit_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Hard Dying:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_hard_dying_field = new wxTextCtrl(main_panel, FIELD_ALIEN_HARD_DYING);
	appearance_grid_sizer->Add(alien_hard_dying_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Soft Dying:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_soft_dying_field = new wxTextCtrl(main_panel, FIELD_ALIEN_SOFT_DYING);
	appearance_grid_sizer->Add(alien_soft_dying_field, 0, wxALIGN_CENTER_VERTICAL);
	
	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Hard Dead:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_hard_dead_field = new wxTextCtrl(main_panel, FIELD_ALIEN_HARD_DEAD);
	appearance_grid_sizer->Add(alien_hard_dead_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Soft Dead:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_soft_dead_field = new wxTextCtrl(main_panel, FIELD_ALIEN_SOFT_DEAD);
	appearance_grid_sizer->Add(alien_soft_dead_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Stationary:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_stationary_field = new wxTextCtrl(main_panel, FIELD_ALIEN_STATIONARY);
	appearance_grid_sizer->Add(alien_stationary_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Moving:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_moving_field = new wxTextCtrl(main_panel, FIELD_ALIEN_MOVING);
	appearance_grid_sizer->Add(alien_moving_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Teleport In:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_teleport_in_field = new wxTextCtrl(main_panel, FIELD_ALIEN_TELEPORT_IN);
	appearance_grid_sizer->Add(alien_teleport_in_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Teleport Out:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_teleport_out_field = new wxTextCtrl(main_panel, FIELD_ALIEN_TELEPORT_OUT);
	appearance_grid_sizer->Add(alien_teleport_out_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Melee Attack:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_melee_attack_shape_field = new wxTextCtrl(main_panel, FIELD_ALIEN_MELEE_ATTACK_SHAPE);
	appearance_grid_sizer->Add(alien_melee_attack_shape_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Ranged Attack:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_ranged_attack_shape_field = new wxTextCtrl(main_panel, FIELD_ALIEN_RANGED_ATTACK_SHAPE);
	appearance_grid_sizer->Add(alien_ranged_attack_shape_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_sizer->Add(appearance_grid_sizer);

	aliens_appearance_sizer->Add(appearance_sizer);
	aliens_appearance_sizer->AddSpacer(10);

	wxStaticBoxSizer* sounds_sizer = new wxStaticBoxSizer(wxHORIZONTAL, main_panel, _("Sounds"));

	wxFlexGridSizer* sounds_grid_sizer = new wxFlexGridSizer(2);
	sounds_grid_sizer->AddGrowableCol(0);
	sounds_grid_sizer->SetHGap(10);
	sounds_grid_sizer->SetVGap(4);

	std::vector<wxString> sound_strings = DefaultNames::Instance()->GetArray(wxT("sound"));
	sound_strings.insert(sound_strings.begin(), GetName(wxT("sound"), -1));

	sounds_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Activation:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_activation_choice = new wxChoice(main_panel, MENU_ALIEN_ACTIVATION, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	sounds_grid_sizer->Add(alien_activation_choice, 0, wxALIGN_CENTER_VERTICAL);

	sounds_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Friendly Activation:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_friendly_activation_choice = new wxChoice(main_panel, MENU_ALIEN_FRIENDLY_ACTIVATION, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	sounds_grid_sizer->Add(alien_friendly_activation_choice, 0, wxALIGN_CENTER_VERTICAL);

	sounds_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Clear:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_clear_choice = new wxChoice(main_panel, MENU_ALIEN_CLEAR, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	sounds_grid_sizer->Add(alien_clear_choice, 0, wxALIGN_CENTER_VERTICAL);

	sounds_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Kill:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_kill_choice = new wxChoice(main_panel, MENU_ALIEN_KILL, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	sounds_grid_sizer->Add(alien_kill_choice, 0, wxALIGN_CENTER_VERTICAL);

	sounds_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Apology:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_apology_choice = new wxChoice(main_panel, MENU_ALIEN_APOLOGY, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	sounds_grid_sizer->Add(alien_apology_choice, 0, wxALIGN_CENTER_VERTICAL);

	sounds_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Friendly Fire:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_friendly_fire_choice = new wxChoice(main_panel, MENU_ALIEN_FRIENDLY_FIRE, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	sounds_grid_sizer->Add(alien_friendly_fire_choice, 0, wxALIGN_CENTER_VERTICAL);

	sounds_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Flaming:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_flaming_choice = new wxChoice(main_panel, MENU_ALIEN_FLAMING, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	sounds_grid_sizer->Add(alien_flaming_choice, 0, wxALIGN_CENTER_VERTICAL);

	sounds_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Random:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_random_choice = new wxChoice(main_panel, MENU_ALIEN_RANDOM, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	sounds_grid_sizer->Add(alien_random_choice, 0, wxALIGN_CENTER_VERTICAL);

	sounds_grid_sizer->AddSpacer(10);
	sounds_grid_sizer->AddSpacer(10);

	sounds_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Random Sound Mask:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_random_sound_mask_field = new wxTextCtrl(main_panel, FIELD_ALIEN_RANDOM_SOUND_MASK);
	sounds_grid_sizer->Add(alien_random_sound_mask_field, 0, wxALIGN_CENTER_VERTICAL);

	sounds_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Sound Pitch:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_sound_pitch_field = new wxTextCtrl(main_panel, FIELD_ALIEN_SOUND_PITCH);
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

	attack_frequency_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Attack Frequency:")), 1, wxALIGN_CENTER_VERTICAL);
	attack_frequency_sizer->AddSpacer(10);
	alien_attack_frequency_field = new wxTextCtrl(main_panel, FIELD_ALIEN_ATTACK_FREQUENCY);
	attack_frequency_sizer->Add(alien_attack_frequency_field, 0, wxALIGN_CENTER_VERTICAL);

	combat_grid_sizer->Add(attack_frequency_sizer);
	combat_grid_sizer->AddSpacer(0);

	std::vector<wxString> shot_strings = DefaultNames::Instance()->GetArray(wxT("shot"));
	shot_strings.insert(shot_strings.begin(), GetName(wxT("shot"), -1));

	for (int i = 0; i < 2; ++i) {
		int id_offset = i * NUM_ALIEN_ATTACK_CONTROLS;
		wxStaticBoxSizer* attack_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, i == 0 ? _("Melee Attack") : _("Ranged Attack"));
		wxFlexGridSizer* attack_grid_sizer = new wxFlexGridSizer(2);
		attack_grid_sizer->AddGrowableCol(0);
		attack_grid_sizer->SetHGap(10);
		attack_grid_sizer->SetVGap(4);
		
		attack_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Type:")), 0, wxALIGN_CENTER_VERTICAL);
		alien_attack_type_choices[i] = new wxChoice(main_panel, MENU_ALIEN_ATTACK_TYPE + id_offset, wxDefaultPosition, choiceSize, shot_strings.size(), &shot_strings[0]);
		attack_grid_sizer->Add(alien_attack_type_choices[i], 0, wxALIGN_CENTER_VERTICAL);

		attack_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Repetitions:")), 0, wxALIGN_CENTER_VERTICAL);
		alien_attack_repetitions_fields[i] = new wxTextCtrl(main_panel, FIELD_ALIEN_ATTACK_REPETITIONS + id_offset);
		attack_grid_sizer->Add(alien_attack_repetitions_fields[i], 0, wxALIGN_CENTER_VERTICAL);

		attack_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Error:")), 0, wxALIGN_CENTER_VERTICAL);
		alien_attack_error_fields[i] = new wxTextCtrl(main_panel, FIELD_ALIEN_ATTACK_ERROR + id_offset);
		attack_grid_sizer->Add(alien_attack_error_fields[i], 0, wxALIGN_CENTER_VERTICAL);

		attack_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Range:")), 0, wxALIGN_CENTER_VERTICAL);
		alien_attack_range_fields[i] = new wxTextCtrl(main_panel, FIELD_ALIEN_ATTACK_RANGE + id_offset);
		attack_grid_sizer->Add(alien_attack_range_fields[i], 0, wxALIGN_CENTER_VERTICAL);

		attack_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Sequence ID:")), 0, wxALIGN_CENTER_VERTICAL);
		alien_attack_sequence_fields[i] = new wxTextCtrl(main_panel, FIELD_ALIEN_ATTACK_SEQUENCE + id_offset);
		attack_grid_sizer->Add(alien_attack_sequence_fields[i], 0, wxALIGN_CENTER_VERTICAL);

		attack_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("dx:")), 0, wxALIGN_CENTER_VERTICAL);
		alien_attack_dx_fields[i] = new wxTextCtrl(main_panel, FIELD_ALIEN_ATTACK_DX + id_offset);
		attack_grid_sizer->Add(alien_attack_dx_fields[i], 0, wxALIGN_CENTER_VERTICAL);

		attack_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("dy:")), 0, wxALIGN_CENTER_VERTICAL);
		alien_attack_dy_fields[i] = new wxTextCtrl(main_panel, FIELD_ALIEN_ATTACK_DY + id_offset);
		attack_grid_sizer->Add(alien_attack_dy_fields[i], 0, wxALIGN_CENTER_VERTICAL);

		attack_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("dz:")), 0, wxALIGN_CENTER_VERTICAL);
		alien_attack_dz_fields[i] = new wxTextCtrl(main_panel, FIELD_ALIEN_ATTACK_DZ + id_offset);
		attack_grid_sizer->Add(alien_attack_dz_fields[i], 0, wxALIGN_CENTER_VERTICAL);

		attack_sizer->Add(attack_grid_sizer, 0, wxEXPAND);

		combat_grid_sizer->Add(attack_sizer, 0, wxEXPAND);
	}

	wxStaticBoxSizer* shrapnel_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Shrapnel"));
	wxFlexGridSizer* shrapnel_grid_sizer = new wxFlexGridSizer(2);
	shrapnel_grid_sizer->AddGrowableCol(0);
	shrapnel_grid_sizer->SetHGap(10);
	shrapnel_grid_sizer->SetVGap(4);

	shrapnel_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Shrapnel Radius:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_shrapnel_radius_field = new wxTextCtrl(main_panel, FIELD_ALIEN_SHRAPNEL_RADIUS);
	shrapnel_grid_sizer->Add(alien_shrapnel_radius_field, 0, wxALIGN_CENTER_VERTICAL);

	std::vector<wxString> damage_strings = DefaultNames::Instance()->GetArray(wxT("damage"));
	damage_strings.insert(damage_strings.begin(), _("None"));

	shrapnel_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Damage Type:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_shrapnel_damage_type_choice = new wxChoice(main_panel, MENU_ALIEN_SHRAPNEL_DAMAGE_TYPE, wxDefaultPosition, choiceSize, damage_strings.size(), &damage_strings[0]);
	shrapnel_grid_sizer->Add(alien_shrapnel_damage_type_choice, 0, wxALIGN_CENTER_VERTICAL);

	shrapnel_sizer->Add(shrapnel_grid_sizer, 0, wxEXPAND);
	
	alien_shrapnel_alien_damage_checkbox = new wxCheckBox(main_panel, CB_ALIEN_SHRAPNEL_ALIEN_DAMAGE, _("Alien Damage (varies with level)"));
	shrapnel_sizer->Add(alien_shrapnel_alien_damage_checkbox, 0, wxEXPAND | wxTOP, 5);

	wxFlexGridSizer* shrapnel_grid_sizer2 = new wxFlexGridSizer(2);
	shrapnel_grid_sizer2->AddGrowableCol(0);
	shrapnel_grid_sizer2->SetHGap(10);
	shrapnel_grid_sizer2->SetVGap(4);

	// make the fields line up with the grid above
	shrapnel_grid_sizer2->AddSpacer(0);
	shrapnel_grid_sizer2->Add(choiceSize.GetWidth(), 0);

	shrapnel_grid_sizer2->Add(new wxStaticText(main_panel, wxID_ANY, _("Base Damage:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_shrapnel_base_damage_field = new wxTextCtrl(main_panel, FIELD_ALIEN_SHRAPNEL_BASE_DAMAGE);
	shrapnel_grid_sizer2->Add(alien_shrapnel_base_damage_field, 0, wxALIGN_CENTER_VERTICAL);

	shrapnel_grid_sizer2->Add(new wxStaticText(main_panel, wxID_ANY, _("Random Damage:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_shrapnel_random_damage_field = new wxTextCtrl(main_panel, FIELD_ALIEN_SHRAPNEL_RANDOM_DAMAGE);
	shrapnel_grid_sizer2->Add(alien_shrapnel_random_damage_field, 0, wxALIGN_CENTER_VERTICAL);

	shrapnel_grid_sizer2->Add(new wxStaticText(main_panel, wxID_ANY, _("Scale:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_shrapnel_damage_scale_field = new wxTextCtrl(main_panel, FIELD_ALIEN_SHRAPNEL_DAMAGE_SCALE);
	shrapnel_grid_sizer2->Add(alien_shrapnel_damage_scale_field, 0, wxALIGN_CENTER_VERTICAL);

	shrapnel_sizer->Add(shrapnel_grid_sizer2, 0, wxEXPAND);

	combat_grid_sizer->Add(shrapnel_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* impact_effects_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Impact Effects (hits on monster)"));
	wxFlexGridSizer* impact_effects_grid_sizer = new wxFlexGridSizer(2);
	impact_effects_grid_sizer->AddGrowableCol(0);
	impact_effects_grid_sizer->SetHGap(10);
	impact_effects_grid_sizer->SetVGap(4);

	std::vector<wxString> effect_strings = DefaultNames::Instance()->GetArray(wxT("effect"));
	effect_strings.insert(effect_strings.begin(), GetName(wxT("sound"), -1));

	impact_effects_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Ranged:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_ranged_impact_effect_choice = new wxChoice(main_panel, MENU_ALIEN_RANGED_IMPACT_EFFECT, wxDefaultPosition, choiceSize, effect_strings.size(), &effect_strings[0]);
	impact_effects_grid_sizer->Add(alien_ranged_impact_effect_choice, 0, wxALIGN_CENTER_VERTICAL);

	impact_effects_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Melee:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_melee_impact_effect_choice = new wxChoice(main_panel, MENU_ALIEN_MELEE_IMPACT_EFFECT, wxDefaultPosition, choiceSize, effect_strings.size(), &effect_strings[0]);
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

	vitality_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Vitality:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_vitality_field = new wxTextCtrl(main_panel, FIELD_ALIEN_VITALITY);
	vitality_grid_sizer->Add(alien_vitality_field, 0, wxALIGN_CENTER_VERTICAL);

	constants_grid_sizer->Add(vitality_grid_sizer, 0, wxEXPAND);

	wxFlexGridSizer* radius_height_sizer = new wxFlexGridSizer(2);
	radius_height_sizer->AddGrowableCol(0);
	radius_height_sizer->SetHGap(10);
	radius_height_sizer->SetVGap(4);

	radius_height_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Radius:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_radius_field = new wxTextCtrl(main_panel, FIELD_ALIEN_RADIUS);
	radius_height_sizer->Add(alien_radius_field, 0, wxALIGN_CENTER_VERTICAL);

	radius_height_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Height:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_height_field = new wxTextCtrl(main_panel, FIELD_ALIEN_HEIGHT);
	radius_height_sizer->Add(alien_height_field, 0, wxALIGN_CENTER_VERTICAL);
	
	constants_grid_sizer->Add(radius_height_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* movement_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Movement"));
	wxFlexGridSizer* movement_grid_sizer = new wxFlexGridSizer(2);
	movement_grid_sizer->AddGrowableCol(0);
	movement_grid_sizer->SetHGap(10);
	movement_grid_sizer->SetVGap(4);

	movement_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Speed:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_speed_field = new wxTextCtrl(main_panel, FIELD_ALIEN_SPEED);
	movement_grid_sizer->Add(alien_speed_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Terminal Velocity:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_terminal_velocity_field = new wxTextCtrl(main_panel, FIELD_ALIEN_TERMINAL_VELOCITY);
	movement_grid_sizer->Add(alien_terminal_velocity_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Gravity:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_gravity_field = new wxTextCtrl(main_panel, FIELD_ALIEN_GRAVITY);
	movement_grid_sizer->Add(alien_gravity_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->AddSpacer(10);
	movement_grid_sizer->AddSpacer(10);

	movement_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Min. Ledge Jump:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_min_ledge_jump_field = new wxTextCtrl(main_panel, FIELD_ALIEN_MIN_LEDGE_JUMP);
	movement_grid_sizer->Add(alien_min_ledge_jump_field, 0, wxALIGN_CENTER_VERTICAL);	

	movement_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Max. Ledge Jump:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_max_ledge_jump_field = new wxTextCtrl(main_panel, FIELD_ALIEN_MAX_LEDGE_JUMP);
	movement_grid_sizer->Add(alien_max_ledge_jump_field, 0, wxALIGN_CENTER_VERTICAL);	

	movement_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("External Velocity Scale:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_ext_velocity_scale_field = new wxTextCtrl(main_panel, FIELD_ALIEN_EXT_VELOCITY_SCALE);
	movement_grid_sizer->Add(alien_ext_velocity_scale_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Hover Height:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_hover_height_field = new wxTextCtrl(main_panel, FIELD_ALIEN_HOVER_HEIGHT);
	movement_grid_sizer->Add(alien_hover_height_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->AddSpacer(10);
	movement_grid_sizer->AddSpacer(10);

	movement_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Door Retry Mask:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_door_retry_mask_field = new wxTextCtrl(main_panel, FIELD_ALIEN_DOOR_RETRY_MASK);
	movement_grid_sizer->Add(alien_door_retry_mask_field, 0, wxALIGN_CENTER_VERTICAL);	

	movement_sizer->Add(movement_grid_sizer, 0, wxEXPAND);

	constants_grid_sizer->Add(movement_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* perception_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Perception"));
	wxFlexGridSizer* perception_grid_sizer = new wxFlexGridSizer(2);
	perception_grid_sizer->AddGrowableCol(0);
	perception_grid_sizer->SetHGap(10);
	perception_grid_sizer->SetVGap(4);

	perception_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Visual Range:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_visual_range_field = new wxTextCtrl(main_panel, FIELD_ALIEN_VISUAL_RANGE);
	perception_grid_sizer->Add(alien_visual_range_field, 0, wxALIGN_CENTER_VERTICAL);	

	perception_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Dark Visual Range:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_dark_visual_range_field = new wxTextCtrl(main_panel, FIELD_ALIEN_DARK_VISUAL_RANGE);
	perception_grid_sizer->Add(alien_dark_visual_range_field, 0, wxALIGN_CENTER_VERTICAL);

	perception_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Intelligence:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_intelligence_field = new wxTextCtrl(main_panel, FIELD_ALIEN_INTELLIGENCE);
	perception_grid_sizer->Add(alien_intelligence_field, 0, wxALIGN_CENTER_VERTICAL);

	perception_sizer->Add(perception_grid_sizer, 0, wxEXPAND);

	constants_grid_sizer->Add(perception_sizer, 0, wxEXPAND);

	aliens_constants_sizer->Add(constants_grid_sizer);
	aliens_constants_sizer->AddSpacer(10);

	wxFlexGridSizer* carrying_grid_sizer = new wxFlexGridSizer(2);
	carrying_grid_sizer->AddGrowableCol(0);
	carrying_grid_sizer->SetHGap(10);
	carrying_grid_sizer->SetVGap(4);

	std::vector<wxString> item_strings = DefaultNames::Instance()->GetArray(wxT("item"));
	item_strings.insert(item_strings.begin(), _("None"));

	carrying_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Carrying Item Type:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_carrying_item_choice = new wxChoice(main_panel, MENU_ALIEN_CARRYING_ITEM, wxDefaultPosition, choiceSize, item_strings.size(), &item_strings[0]);
	carrying_grid_sizer->Add(alien_carrying_item_choice, 0, wxALIGN_CENTER_VERTICAL);

	carrying_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Contrail Effect:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_contrail_effect_choice = new wxChoice(main_panel, MENU_ALIEN_CONTRAIL_EFFECT, wxDefaultPosition, choiceSize, effect_strings.size(), &effect_strings[0]);
	carrying_grid_sizer->Add(alien_contrail_effect_choice, 0, wxALIGN_CENTER_VERTICAL);

	aliens_constants_sizer->Add(carrying_grid_sizer);
	
	mainbox->Add(aliens_constants_sizer, 5, wxALL, 10);
	mainbox->Show(aliens_constants_sizer, false);

	aliens_behavior_sizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* class_sizer = new wxBoxSizer(wxHORIZONTAL);

	std::vector<wxString> class_strings = DefaultNames::Instance()->GetArray(wxT("class"));

	class_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Class:")), 0, wxALIGN_CENTER_VERTICAL);
	alien_class_choice = new wxChoice(main_panel, MENU_ALIEN_CLASS, wxDefaultPosition, choiceSize, class_strings.size(), &class_strings[0]);
	class_sizer->AddSpacer(10);
	class_sizer->Add(alien_class_choice, 0, wxALIGN_CENTER_VERTICAL);

	aliens_behavior_sizer->Add(class_sizer);

	wxStaticBoxSizer* friends_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Friends"));
	wxGridSizer* friends_grid_sizer = new wxGridSizer(4);
	friends_grid_sizer->SetVGap(2);

	for (int i = 0; i < 16; ++i) {
		alien_friends_checkboxes[i] = new wxCheckBox(main_panel, CB_ALIEN_FRIENDS + i, GetName(wxT("class"), i));

	}

	for (int col = 0; col < 4; ++col) {
		for (int row = 0; row < 4; ++row) {
			friends_grid_sizer->Add(alien_friends_checkboxes[row * 4 + col], 0, wxALIGN_CENTER_VERTICAL);
		}
	}
	
	friends_sizer->Add(friends_grid_sizer, 0, wxEXPAND);

	aliens_behavior_sizer->AddSpacer(10);
	aliens_behavior_sizer->Add(friends_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* enemies_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Enemies"));
	wxGridSizer* enemies_grid_sizer = new wxGridSizer(4);
	enemies_grid_sizer->SetVGap(2);

	for (int i = 0; i < 16; ++i) {
		alien_enemies_checkboxes[i] = new wxCheckBox(main_panel, CB_ALIEN_ENEMIES + i, GetName(wxT("class"), i));
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

	wxStaticBoxSizer* flags_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Flags"));
	wxGridSizer* flags_grid_sizer = new wxGridSizer(3);
	flags_grid_sizer->SetVGap(2);
	
	for (int i = 0; i < 27; ++i) {
		alien_flags_checkboxes[i] = new wxCheckBox(main_panel, CB_ALIEN_FLAGS + i, flags_strings[i]);
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

	wxStaticBoxSizer* immunities_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Immunities"));
	wxGridSizer* immunities_grid_sizer = new wxGridSizer(4);
	immunities_grid_sizer->SetVGap(2);
	
	for (int i = 0; i < 24; ++i) {
		alien_immunities_checkboxes[i] = new wxCheckBox(main_panel, CB_ALIEN_IMMUNITIES + i, GetName(wxT("damage"), i));
	}

	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 4; ++j) {
			immunities_grid_sizer->Add(alien_immunities_checkboxes[j * 6 + i], 0, wxALIGN_CENTER_VERTICAL);
		}
	}

	immunities_sizer->Add(immunities_grid_sizer, 0, wxEXPAND);

	aliens_immunities_sizer->Add(immunities_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* weaknesses_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Weaknesses"));
	wxGridSizer* weaknesses_grid_sizer = new wxGridSizer(4);
	weaknesses_grid_sizer->SetVGap(2);

	for (int i = 0; i < 24; ++i) {
		alien_weaknesses_checkboxes[i] = new wxCheckBox(main_panel, CB_ALIEN_WEAKNESSES + i, GetName(wxT("damage"), i));
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

	wxStaticBoxSizer* effects_static_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Effect"));
	
	wxFlexGridSizer* effects_grid_sizer = new wxFlexGridSizer(2);
	effects_grid_sizer->AddGrowableCol(0);
	effects_grid_sizer->SetHGap(10);
	effects_grid_sizer->SetVGap(4);
	
	effects_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Graphic collection:")), 0, wxALIGN_CENTER_VERTICAL);
	eff_collection_field = new wxTextCtrl(main_panel, FIELD_EFFECT_COLLECTION);
	effects_grid_sizer->Add(eff_collection_field, 0, wxALIGN_CENTER_VERTICAL);

	effects_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Color table:")), 0, wxALIGN_CENTER_VERTICAL);
	eff_color_table_field = new wxTextCtrl(main_panel, FIELD_EFFECT_COLOR_TABLE);
	effects_grid_sizer->Add(eff_color_table_field, 0, wxALIGN_CENTER_VERTICAL);

	effects_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Sequence:")), 0, wxALIGN_CENTER_VERTICAL);
	eff_sequence_field = new wxTextCtrl(main_panel, FIELD_EFFECT_SEQUENCE);
	effects_grid_sizer->Add(eff_sequence_field, 0, wxALIGN_CENTER_VERTICAL);

	effects_grid_sizer->AddSpacer(10);
	effects_grid_sizer->AddSpacer(10);

	effects_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Sound pitch:")), 0, wxALIGN_CENTER_VERTICAL);
	eff_pitch_field = new wxTextCtrl(main_panel, FIELD_EFFECT_PITCH);
	effects_grid_sizer->Add(eff_pitch_field, 0, wxALIGN_CENTER_VERTICAL);

	effects_grid_sizer->AddSpacer(10);
	effects_grid_sizer->AddSpacer(10);

	effects_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Delay:")), 0, wxALIGN_CENTER_VERTICAL);
	eff_delay_field = new wxTextCtrl(main_panel, wxID_ANY);
	eff_delay_field->Disable();
	effects_grid_sizer->Add(eff_delay_field, 0, wxALIGN_CENTER_VERTICAL);

	std::vector<wxString> delay_sound_choices = DefaultNames::Instance()->GetArray(wxT("sound"));
	delay_sound_choices.insert(delay_sound_choices.begin(), GetName(wxT("sound"), -1));

	effects_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Delay sound:")), 0, wxALIGN_CENTER_VERTICAL);
	eff_delay_sound_choice = new wxChoice(main_panel, MENU_EFFECT_DELAY_SOUND, wxDefaultPosition, choiceSize, delay_sound_choices.size(), &delay_sound_choices[0]);
	effects_grid_sizer->Add(eff_delay_sound_choice, 0, wxALIGN_CENTER_VERTICAL);

	effects_static_sizer->Add(effects_grid_sizer);
	
	effects_static_sizer->AddSpacer(10);

	eff_end_when_animation_loops_checkbox = new wxCheckBox(main_panel, CB_EFFECT_END_WHEN_ANIMATION_LOOPS, _("End when animation loops"));
	effects_static_sizer->Add(eff_end_when_animation_loops_checkbox);

	eff_end_when_transfer_animation_loops_checkbox = new wxCheckBox(main_panel, CB_EFFECT_END_WHEN_TRANSFER_ANIMATION_LOOPS, _("End when transfer animation loops"));
	effects_static_sizer->AddSpacer(2);
	effects_static_sizer->Add(eff_end_when_transfer_animation_loops_checkbox);

	eff_sound_only_checkbox = new wxCheckBox(main_panel, CB_EFFECT_SOUND_ONLY, _("Sound only"));
	effects_static_sizer->AddSpacer(2);
	effects_static_sizer->Add(eff_sound_only_checkbox);

	eff_media_effect_checkbox = new wxCheckBox(main_panel, CB_EFFECT_MEDIA_EFFECT, _("Media effect"));
	effects_static_sizer->AddSpacer(2);
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

	wxStaticBoxSizer* movement_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Movement"));
	wxFlexGridSizer* movement_grid_sizer = new wxFlexGridSizer(2);

	movement_grid_sizer->AddGrowableCol(0);
	movement_grid_sizer->SetHGap(10);
	movement_grid_sizer->SetVGap(4);

	movement_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Max. forward velocity:")), 0, wxALIGN_CENTER_VERTICAL);
	max_forward_velocity_field = new wxTextCtrl(main_panel, FIELD_MAX_FORWARD_VELOCITY);
	movement_grid_sizer->Add(max_forward_velocity_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Max. backward velocity:")), 0, wxALIGN_CENTER_VERTICAL);
	max_backward_velocity_field = new wxTextCtrl(main_panel, FIELD_MAX_BACKWARD_VELOCITY);
	movement_grid_sizer->Add(max_backward_velocity_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Max. perpendicular velocity:")), 0, wxALIGN_CENTER_VERTICAL);
	max_perpendicular_velocity_field = new wxTextCtrl(main_panel, FIELD_MAX_PERPENDICULAR_VELOCITY);
	movement_grid_sizer->Add(max_perpendicular_velocity_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->AddSpacer(10);
	movement_grid_sizer->AddSpacer(10);

	movement_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Acceleration:")), 0, wxALIGN_CENTER_VERTICAL);
	acceleration_field = new wxTextCtrl(main_panel, FIELD_ACCELERATION);
	movement_grid_sizer->Add(acceleration_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Deceleration:")), 0, wxALIGN_CENTER_VERTICAL);
	deceleration_field = new wxTextCtrl(main_panel, FIELD_DECELERATION);
	movement_grid_sizer->Add(deceleration_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Airborne deceleration:")), 0, wxALIGN_CENTER_VERTICAL);
	airborne_deceleration_field = new wxTextCtrl(main_panel, FIELD_AIRBORNE_DECELERATION);
	movement_grid_sizer->Add(airborne_deceleration_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Gravitational acceleration:")), 0, wxALIGN_CENTER_VERTICAL);
	gravitational_acceleration_field = new wxTextCtrl(main_panel, FIELD_GRAVITATIONAL_ACCELERATION);
	movement_grid_sizer->Add(gravitational_acceleration_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Climbing acceleration:")), 0, wxALIGN_CENTER_VERTICAL);
	climbing_acceleration_field = new wxTextCtrl(main_panel, FIELD_CLIMBING_ACCELERATION);
	movement_grid_sizer->Add(climbing_acceleration_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Terminal velocity:")), 0, wxALIGN_CENTER_VERTICAL);
	terminal_velocity_field = new wxTextCtrl(main_panel, FIELD_TERMINAL_VELOCITY);
	movement_grid_sizer->Add(terminal_velocity_field, 0, wxALIGN_CENTER_VERTICAL);	

	movement_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("External deceleration:")), 0, wxALIGN_CENTER_VERTICAL);
	external_deceleration_field = new wxTextCtrl(main_panel, FIELD_EXTERNAL_DECELERATION);
	movement_grid_sizer->Add(external_deceleration_field, 0, wxALIGN_CENTER_VERTICAL);

	movement_sizer->Add(movement_grid_sizer, 0, wxEXPAND);
	physics_sizer->Add(movement_sizer, 0, wxEXPAND);

	wxBoxSizer* steps_player_size_sizer = new wxBoxSizer(wxVERTICAL);
	wxStaticBoxSizer* steps_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Steps"));
	wxFlexGridSizer* steps_grid_sizer = new wxFlexGridSizer(2);
	steps_grid_sizer->AddGrowableCol(0);
	steps_grid_sizer->SetHGap(10);
	steps_grid_sizer->SetVGap(4);

	steps_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Step delta:")), 0, wxALIGN_CENTER_VERTICAL);
	step_delta_field = new wxTextCtrl(main_panel, FIELD_STEP_DELTA);
	steps_grid_sizer->Add(step_delta_field, 0, wxALIGN_CENTER_VERTICAL);

	steps_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Step amplitude:")), 0, wxALIGN_CENTER_VERTICAL);
	step_amplitude_field = new wxTextCtrl(main_panel, FIELD_STEP_AMPLITUDE);
	steps_grid_sizer->Add(step_amplitude_field, 0, wxALIGN_CENTER_VERTICAL);
	
	steps_sizer->Add(steps_grid_sizer, 0, wxEXPAND);

	steps_player_size_sizer->Add(steps_sizer, 0, wxEXPAND);
	steps_player_size_sizer->AddSpacer(10);

	wxStaticBoxSizer* size_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Player Size"));
	wxFlexGridSizer* size_grid_sizer = new wxFlexGridSizer(2);
	size_grid_sizer->AddGrowableCol(0);
	size_grid_sizer->SetHGap(10);
	size_grid_sizer->SetVGap(4);

	size_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Radius:")), 0, wxALIGN_CENTER_VERTICAL);
	radius_field = new wxTextCtrl(main_panel, FIELD_RADIUS);
	size_grid_sizer->Add(radius_field, 0, wxALIGN_CENTER_VERTICAL);

	size_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Height:")), 0, wxALIGN_CENTER_VERTICAL);
	height_field = new wxTextCtrl(main_panel, FIELD_HEIGHT);
	size_grid_sizer->Add(height_field, 0, wxALIGN_CENTER_VERTICAL);

	size_sizer->Add(size_grid_sizer, 0, wxEXPAND);
	
	steps_player_size_sizer->Add(size_sizer, 0, wxEXPAND);
	steps_player_size_sizer->AddStretchSpacer();

	physics_sizer->Add(steps_player_size_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* turning_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Turning/Head Movement"));
	wxFlexGridSizer* turning_grid_sizer = new wxFlexGridSizer(2);
	turning_grid_sizer->AddGrowableCol(0);
	turning_grid_sizer->SetHGap(10);
	turning_grid_sizer->SetVGap(4);
	
	turning_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Angular acceleration:")), 0, wxALIGN_CENTER_VERTICAL);
	angular_acceleration_field = new wxTextCtrl(main_panel, FIELD_ANGULAR_ACCELERATION);
	turning_grid_sizer->Add(angular_acceleration_field, 0, wxALIGN_CENTER_VERTICAL);

	turning_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Angular deceleration:")), 0, wxALIGN_CENTER_VERTICAL);
	angular_deceleration_field = new wxTextCtrl(main_panel, FIELD_ANGULAR_DECELERATION);
	turning_grid_sizer->Add(angular_deceleration_field, 0, wxALIGN_CENTER_VERTICAL);

	turning_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Maximum angular velocity:")), 0, wxALIGN_CENTER_VERTICAL);
	maximum_angular_velocity_field = new wxTextCtrl(main_panel, FIELD_MAXIMUM_ANGULAR_VELOCITY);
	turning_grid_sizer->Add(maximum_angular_velocity_field, 0, wxALIGN_CENTER_VERTICAL);

	turning_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Angular recentering velocity:")), 0, wxALIGN_CENTER_VERTICAL);
	angular_recentering_velocity_field = new wxTextCtrl(main_panel, FIELD_ANGULAR_RECENTERING_VELOCITY);
	turning_grid_sizer->Add(angular_recentering_velocity_field, 0, wxALIGN_CENTER_VERTICAL);

	turning_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Head angular velocity:")), 0, wxALIGN_CENTER_VERTICAL);
	head_angular_velocity_field = new wxTextCtrl(main_panel, FIELD_HEAD_ANGULAR_VELOCITY);
	turning_grid_sizer->Add(head_angular_velocity_field, 0, wxALIGN_CENTER_VERTICAL);

	turning_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Head angular maximum:")), 0, wxALIGN_CENTER_VERTICAL);
	head_angular_maximum_field = new wxTextCtrl(main_panel, FIELD_HEAD_ANGULAR_MAXIMUM);
	turning_grid_sizer->Add(head_angular_maximum_field, 0, wxALIGN_CENTER_VERTICAL);

	turning_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Maximum elevation:")), 0, wxALIGN_CENTER_VERTICAL);
	maximum_elevation_field = new wxTextCtrl(main_panel, FIELD_MAXIMUM_ELEVATION);
	turning_grid_sizer->Add(maximum_elevation_field, 0, wxALIGN_CENTER_VERTICAL);

	turning_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("External angular deceleration:")), 0, wxALIGN_CENTER_VERTICAL);
	external_angular_deceleration_field = new wxTextCtrl(main_panel, FIELD_EXTERNAL_ANGULAR_DECELERATION);
	turning_grid_sizer->Add(external_angular_deceleration_field, 0, wxALIGN_CENTER_VERTICAL);

	turning_sizer->Add(turning_grid_sizer, 0, wxEXPAND);
	physics_sizer->Add(turning_sizer, 0);

	wxStaticBoxSizer* camera_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Heights (for camera)"));
	wxFlexGridSizer* camera_grid_sizer = new wxFlexGridSizer(2);
	camera_grid_sizer->AddGrowableCol(0);
	camera_grid_sizer->SetHGap(10);
	camera_grid_sizer->SetVGap(4);

	camera_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Dead height:")), 0, wxALIGN_CENTER_VERTICAL);
	dead_height_field = new wxTextCtrl(main_panel, FIELD_DEAD_HEIGHT);
	camera_grid_sizer->Add(dead_height_field, 0, wxALIGN_CENTER_VERTICAL);

	camera_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Camera height:")), 0, wxALIGN_CENTER_VERTICAL);
	camera_height_field = new wxTextCtrl(main_panel, FIELD_CAMERA_HEIGHT);
	camera_grid_sizer->Add(camera_height_field, 0, wxALIGN_CENTER_VERTICAL);

	camera_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Splash height:")), 0, wxALIGN_CENTER_VERTICAL);
	splash_height_field = new wxTextCtrl(main_panel, FIELD_SPLASH_HEIGHT);
	camera_grid_sizer->Add(splash_height_field, 0, wxALIGN_CENTER_VERTICAL);

	camera_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Half camera separation:")), 0, wxALIGN_CENTER_VERTICAL);
	half_camera_separation_field = new wxTextCtrl(main_panel, FIELD_HALF_CAMERA_SEPARATION);
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
	
	wxStaticBoxSizer* shots_static_sizer = new wxStaticBoxSizer(wxHORIZONTAL, main_panel, _("Shot"));
	
	wxBoxSizer* column_1_sizer = new wxBoxSizer(wxVERTICAL);
	
	wxFlexGridSizer* shape_sizer = new wxFlexGridSizer(2);
	shape_sizer->AddGrowableCol(1);
	shape_sizer->SetHGap(10);
	shape_sizer->SetVGap(4);

	shape_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Graphic Collection:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_collection_field = new wxTextCtrl(main_panel, FIELD_SHOT_COLLECTION);
	shape_sizer->Add(shots_collection_field, 0, wxALIGN_CENTER_VERTICAL);

	shape_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Color Table:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_color_table_field = new wxTextCtrl(main_panel, FIELD_SHOT_COLOR_TABLE);
	shape_sizer->Add(shots_color_table_field, 0, wxALIGN_CENTER_VERTICAL);

	shape_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Sequence:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_sequence_field = new wxTextCtrl(main_panel, FIELD_SHOT_SEQUENCE);
	shape_sizer->Add(shots_sequence_field, 0, wxALIGN_CENTER_VERTICAL);

	column_1_sizer->Add(shape_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* damage_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Damage"));

	wxFlexGridSizer* damage_grid_sizer = new wxFlexGridSizer(2);
	damage_grid_sizer->AddGrowableCol(0);
	damage_grid_sizer->SetHGap(10);
	damage_grid_sizer->SetVGap(5);

	damage_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Damage Type:")), 0, wxALIGN_CENTER_VERTICAL);
	
	std::vector<wxString> damage_strings = DefaultNames::Instance()->GetArray(wxT("damage"));
	damage_strings.insert(damage_strings.begin(), _("None"));

	shots_damage_type_choice = new wxChoice(main_panel, MENU_SHOT_DAMAGE_TYPE, wxDefaultPosition, choiceSize, damage_strings.size(), &damage_strings[0]);
	damage_grid_sizer->Add(shots_damage_type_choice, 0, wxALIGN_CENTER_VERTICAL);

	damage_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Damage Base:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_damage_base_field = new wxTextCtrl(main_panel, FIELD_SHOT_DAMAGE_BASE);
	damage_grid_sizer->Add(shots_damage_base_field, 0, wxALIGN_CENTER_VERTICAL);

	damage_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Damage Random:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_damage_random_field = new wxTextCtrl(main_panel, FIELD_SHOT_DAMAGE_RANDOM);
	damage_grid_sizer->Add(shots_damage_random_field, 0, wxALIGN_CENTER_VERTICAL);

	damage_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Damage Scale:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_damage_scale_field = new wxTextCtrl(main_panel, FIELD_SHOT_DAMAGE_SCALE);
	damage_grid_sizer->Add(shots_damage_scale_field, 0, wxALIGN_CENTER_VERTICAL);
	
	damage_sizer->Add(damage_grid_sizer, 0, wxEXPAND);
	
	shots_alien_damage_checkbox = new wxCheckBox(main_panel, CB_SHOT_ALIEN_DAMAGE, _("Alien Damage (varies with level)"));
	damage_sizer->Add(shots_alien_damage_checkbox, 0, wxTOP, 5);

	column_1_sizer->Add(damage_sizer, 0, wxEXPAND | wxALL, 10);

	wxFlexGridSizer* shots_grid_sizer = new wxFlexGridSizer(2);
	shots_grid_sizer->AddGrowableCol(0);
	shots_grid_sizer->SetHGap(10);
	shots_grid_sizer->SetVGap(4);
	
	std::vector<wxString> sound_strings = DefaultNames::Instance()->GetArray(wxT("sound"));
	sound_strings.insert(sound_strings.begin(), GetName(wxT("sound"), -1));

	shots_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Flyby Sound:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_flyby_sound_choice = new wxChoice(main_panel, MENU_SHOT_FLYBY_SOUND, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	shots_grid_sizer->Add(shots_flyby_sound_choice, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Rebound Sound:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_rebound_sound_choice = new wxChoice(main_panel, MENU_SHOT_REBOUND_SOUND, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
	shots_grid_sizer->Add(shots_rebound_sound_choice, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Sound Pitch:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_sound_pitch_field = new wxTextCtrl(main_panel, FIELD_SHOT_SOUND_PITCH);
	shots_grid_sizer->Add(shots_sound_pitch_field, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->AddSpacer(5);
	shots_grid_sizer->AddSpacer(5);

	shots_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Radius:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_radius_field = new wxTextCtrl(main_panel, FIELD_SHOT_RADIUS);
	shots_grid_sizer->Add(shots_radius_field, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Area of Effect:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_area_of_effect_field = new wxTextCtrl(main_panel, FIELD_SHOT_AREA_OF_EFFECT);
	shots_grid_sizer->Add(shots_area_of_effect_field, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Speed:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_speed_field = new wxTextCtrl(main_panel, FIELD_SHOT_SPEED);
	shots_grid_sizer->Add(shots_speed_field, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Maximum Range:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_range_field = new wxTextCtrl(main_panel, FIELD_SHOT_RANGE);
	shots_grid_sizer->Add(shots_range_field, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->AddSpacer(5);
	shots_grid_sizer->AddSpacer(5);

	std::vector<wxString> effect_strings = DefaultNames::Instance()->GetArray(wxT("effect"));
	effect_strings.insert(effect_strings.begin(), GetName(wxT("sound"), -1));
	
	shots_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Detonation Effect:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_detonation_effect_choice = new wxChoice(main_panel, MENU_SHOT_DETONATION_EFFECT, wxDefaultPosition, choiceSize, effect_strings.size(), &effect_strings[0]);
	shots_grid_sizer->Add(shots_detonation_effect_choice, 0, wxALIGN_CENTER_VERTICAL);

	static const wxString media_effect_strings[] = {
		_("None"),
		_("Small Splash"),
		_("Medium Splash"),
	};

	shots_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Media Detonation Effect:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_media_detonation_effect_choice = new wxChoice(main_panel, MENU_SHOT_MEDIA_DETONATION_EFFECT, wxDefaultPosition, choiceSize, 3, media_effect_strings);
	shots_grid_sizer->Add(shots_media_detonation_effect_choice, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->AddSpacer(5);
	shots_grid_sizer->AddSpacer(5);

	shots_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Contrail:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_contrail_choice = new wxChoice(main_panel, MENU_SHOT_CONTRAIL, wxDefaultPosition, choiceSize, effect_strings.size(), &effect_strings[0]);
	shots_grid_sizer->Add(shots_contrail_choice, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Ticks between contrails:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_contrail_ticks_field = new wxTextCtrl(main_panel, FIELD_SHOT_CONTRAIL_TICKS);
	shots_grid_sizer->Add(shots_contrail_ticks_field, 0, wxALIGN_CENTER_VERTICAL);

	shots_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Maximum contrails:")), 0, wxALIGN_CENTER_VERTICAL);
	shots_maximum_contrails_field = new wxTextCtrl(main_panel, FIELD_SHOT_MAXIMUM_CONTRAILS);
	shots_grid_sizer->Add(shots_maximum_contrails_field, 0, wxALIGN_CENTER_VERTICAL);

	column_1_sizer->Add(shots_grid_sizer);

	shots_static_sizer->Add(column_1_sizer, 0, wxEXPAND);
	shots_static_sizer->AddSpacer(10);

	wxBoxSizer* column_2_sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* flags_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Flags"));

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
		shots_flags_checkboxes[i] = new wxCheckBox(main_panel, CB_SHOT_FLAGS + i, flags_strings[i]);
		if (i > 0) {
			flags_sizer->Add(shots_flags_checkboxes[i], 0, wxTOP, 2);
		} else {
			flags_sizer->Add(shots_flags_checkboxes[i]);
		}
	}

	column_2_sizer->Add(flags_sizer, 0, wxEXPAND | wxTOP | wxRIGHT | wxBOTTOM, 10);

	wxBoxSizer* media_impact_sizer = new wxBoxSizer(wxHORIZONTAL);
	
	media_impact_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Media impact:")), 0, wxALIGN_CENTER_VERTICAL);
	media_impact_sizer->AddSpacer(10);

	std::vector<wxString> shot_strings = DefaultNames::Instance()->GetArray(wxT("shot"));
	shot_strings.insert(shot_strings.begin(), GetName(wxT("shot"), -1));

	shots_media_impact_choice = new wxChoice(main_panel, MENU_SHOT_MEDIA_IMPACT, wxDefaultPosition, choiceSize, shot_strings.size(), &shot_strings[0]);
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

	item_type_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Item Type:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_item_type_choice = new wxChoice(main_panel, MENU_WEAPON_ITEM_TYPE, wxDefaultPosition, choiceSize, item_strings.size(), &item_strings[0]);
	item_type_sizer->AddSpacer(10);
	item_type_sizer->Add(weapon_item_type_choice, 0, wxALIGN_CENTER_VERTICAL);

	column_1_sizer->Add(item_type_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* appearance_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Appearance"));
	wxFlexGridSizer* appearance_grid_sizer = new wxFlexGridSizer(2);
	appearance_grid_sizer->AddGrowableCol(0);
	appearance_grid_sizer->SetHGap(10);
	appearance_grid_sizer->SetVGap(4);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Graphic Collection:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_collection_field = new wxTextCtrl(main_panel, FIELD_WEAPON_COLLECTION);
	appearance_grid_sizer->Add(weapon_collection_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Color Table:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_color_table_field = new wxTextCtrl(main_panel, FIELD_WEAPON_COLOR_TABLE);
	appearance_grid_sizer->Add(weapon_color_table_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Idle:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_idle_field = new wxTextCtrl(main_panel, FIELD_WEAPON_IDLE);
	appearance_grid_sizer->Add(weapon_idle_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Firing:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_firing_field = new wxTextCtrl(main_panel, FIELD_WEAPON_FIRING);
	appearance_grid_sizer->Add(weapon_firing_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Reloading:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_reloading_field = new wxTextCtrl(main_panel, FIELD_WEAPON_RELOADING);
	appearance_grid_sizer->Add(weapon_reloading_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Charging:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_charging_field = new wxTextCtrl(main_panel, FIELD_WEAPON_CHARGING);
	appearance_grid_sizer->Add(weapon_charging_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Charged:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_charged_field = new wxTextCtrl(main_panel, FIELD_WEAPON_CHARGED);
	appearance_grid_sizer->Add(weapon_charged_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_grid_sizer->AddSpacer(10);
	appearance_grid_sizer->AddSpacer(10);

	appearance_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Flash Intensity:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_flash_intensity_field = new wxTextCtrl(main_panel, FIELD_WEAPON_FLASH_INTENSITY);
	appearance_grid_sizer->Add(weapon_flash_intensity_field, 0, wxALIGN_CENTER_VERTICAL);

	appearance_sizer->Add(appearance_grid_sizer, 0, wxEXPAND);

	column_1_sizer->AddSpacer(10);
	column_1_sizer->Add(appearance_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* timing_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Timing (all in ticks)"));
	wxFlexGridSizer* timing_grid_sizer = new wxFlexGridSizer(2);
	timing_grid_sizer->AddGrowableCol(0);
	timing_grid_sizer->SetHGap(10);
	timing_grid_sizer->SetVGap(4);

	timing_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Ready:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_ready_field = new wxTextCtrl(main_panel, FIELD_WEAPON_READY);
	timing_grid_sizer->Add(weapon_ready_field, 0, wxALIGN_CENTER_VERTICAL);	

	timing_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Await Reload:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_await_reload_field = new wxTextCtrl(main_panel, FIELD_WEAPON_AWAIT_RELOAD);
	timing_grid_sizer->Add(weapon_await_reload_field, 0, wxALIGN_CENTER_VERTICAL);	

	timing_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Loading:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_loading_field = new wxTextCtrl(main_panel, FIELD_WEAPON_LOADING);
	timing_grid_sizer->Add(weapon_loading_field, 0, wxALIGN_CENTER_VERTICAL);	

	timing_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Finish Loading:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_finish_loading_field = new wxTextCtrl(main_panel, FIELD_WEAPON_FINISH_LOADING);
	timing_grid_sizer->Add(weapon_finish_loading_field, 0, wxALIGN_CENTER_VERTICAL);

	timing_grid_sizer->AddSpacer(10);
	timing_grid_sizer->AddSpacer(10);

	timing_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Flash Decay:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_flash_decay_field = new wxTextCtrl(main_panel, FIELD_WEAPON_FLASH_DECAY);
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

	weapon_class_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Weapon Class:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_class_choice = new wxChoice(main_panel, MENU_WEAPON_CLASS, wxDefaultPosition, choiceSize, 6, weapon_class_names);
	weapon_class_sizer->AddSpacer(10);
	weapon_class_sizer->Add(weapon_class_choice, 0, wxALIGN_CENTER_VERTICAL);

	column_2_sizer->Add(weapon_class_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* height_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Height and Idle"));
	wxFlexGridSizer* height_grid_sizer = new wxFlexGridSizer(2);
	height_grid_sizer->AddGrowableCol(0);
	height_grid_sizer->SetHGap(10);
	height_grid_sizer->SetVGap(4);

	height_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Idle Height:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_idle_height_field = new wxTextCtrl(main_panel, FIELD_WEAPON_IDLE_HEIGHT);
	height_grid_sizer->Add(weapon_idle_height_field, 0, wxALIGN_CENTER_VERTICAL);	

	height_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Bob Amplitude:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_bob_amplitude_field = new wxTextCtrl(main_panel, FIELD_WEAPON_BOB_AMPLITUDE);
	height_grid_sizer->Add(weapon_bob_amplitude_field, 0, wxALIGN_CENTER_VERTICAL);	

	height_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Kick Height:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_kick_height_field = new wxTextCtrl(main_panel, FIELD_WEAPON_KICK_HEIGHT);
	height_grid_sizer->Add(weapon_kick_height_field, 0, wxALIGN_CENTER_VERTICAL);	

	height_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Reload Height:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_reload_height_field = new wxTextCtrl(main_panel, FIELD_WEAPON_RELOAD_HEIGHT);
	height_grid_sizer->Add(weapon_reload_height_field, 0, wxALIGN_CENTER_VERTICAL);

	height_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Idle Width:")), 0, wxALIGN_CENTER_VERTICAL);
	weapon_idle_width_field = new wxTextCtrl(main_panel, FIELD_WEAPON_IDLE_WIDTH);
	height_grid_sizer->Add(weapon_idle_width_field, 0, wxALIGN_CENTER_VERTICAL);

	height_sizer->Add(height_grid_sizer, 0, wxEXPAND);
	
	column_2_sizer->AddSpacer(10);
	column_2_sizer->Add(height_sizer, 0, wxEXPAND);

	wxStaticBoxSizer* flags_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Flags"));
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
		weapon_flags_checkboxes[i] = new wxCheckBox(main_panel, CB_WEAPON_FLAGS + i, flags_strings[i]);
		if (i > 0) {
			flags_sizer->Add(weapon_flags_checkboxes[i], 0, wxALIGN_CENTER_VERTICAL | wxTOP, 2);
		} else {
			flags_sizer->Add(weapon_flags_checkboxes[i], 0, wxALIGN_CENTER_VERTICAL);
		}
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
		int id_offset = i * NUM_TRIGGER_IDS;
		wxStaticBoxSizer* static_box_sizer = new wxStaticBoxSizer(wxHORIZONTAL, main_panel, trigger_names[i]);
		wxBoxSizer* column_1_sizer = new wxBoxSizer(wxVERTICAL);

		wxFlexGridSizer* left_grid_sizer = new wxFlexGridSizer(2);
		left_grid_sizer->AddGrowableCol(0);
		left_grid_sizer->SetHGap(10);
		left_grid_sizer->SetVGap(4);

		left_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Projectile:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_projectile_choices[i] = new wxChoice(main_panel, MENU_TRIGGER_PROJECTILE + id_offset, wxDefaultPosition, choiceSize, shot_strings.size(), &shot_strings[0]);
		left_grid_sizer->Add(trigger_projectile_choices[i]);

		left_grid_sizer->AddSpacer(10);
		left_grid_sizer->AddSpacer(10);

		left_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Rounds/magazine:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_rounds_fields[i] = new wxTextCtrl(main_panel, FIELD_TRIGGER_ROUNDS + id_offset);
		left_grid_sizer->Add(trigger_rounds_fields[i], 0, wxALIGN_CENTER_VERTICAL);

		column_1_sizer->Add(left_grid_sizer, 0, wxEXPAND);

		wxStaticBoxSizer* sounds_sizer = new wxStaticBoxSizer(wxVERTICAL, main_panel, _("Sounds"));
		wxFlexGridSizer* sounds_grid_sizer = new wxFlexGridSizer(2);
		sounds_grid_sizer->AddGrowableCol(0);
		sounds_grid_sizer->SetHGap(10);
		sounds_grid_sizer->SetVGap(4);

		sounds_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Firing:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_firing_choices[i] = new wxChoice(main_panel, MENU_TRIGGER_FIRING + id_offset, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
		sounds_grid_sizer->Add(trigger_firing_choices[i]);

		sounds_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Click:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_click_choices[i] = new wxChoice(main_panel, MENU_TRIGGER_CLICK + id_offset, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
		sounds_grid_sizer->Add(trigger_click_choices[i]);

		sounds_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Charging:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_charging_choices[i] = new wxChoice(main_panel, MENU_TRIGGER_CHARGING + id_offset, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
		sounds_grid_sizer->Add(trigger_charging_choices[i]);

		sounds_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Shell Casing:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_shell_casing_choices[i] = new wxChoice(main_panel, MENU_TRIGGER_SHELL_CASING + id_offset, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
		sounds_grid_sizer->Add(trigger_shell_casing_choices[i]);

		sounds_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Reloading:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_reloading_choices[i] = new wxChoice(main_panel, MENU_TRIGGER_RELOADING + id_offset, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
		sounds_grid_sizer->Add(trigger_reloading_choices[i]);

		sounds_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Charged:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_charged_choices[i] = new wxChoice(main_panel, MENU_TRIGGER_CHARGED + id_offset, wxDefaultPosition, choiceSize, sound_strings.size(), &sound_strings[0]);
		sounds_grid_sizer->Add(trigger_charged_choices[i]);

		sounds_sizer->Add(sounds_grid_sizer);

		column_1_sizer->Add(sounds_sizer, 0, wxEXPAND | wxALL, 10);

		static_box_sizer->Add(column_1_sizer, 0, wxEXPAND);

		wxFlexGridSizer* right_grid_sizer = new wxFlexGridSizer(2);
		right_grid_sizer->AddGrowableCol(0);
		right_grid_sizer->SetHGap(10);
		right_grid_sizer->SetVGap(4);

		right_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Ammo Type:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_ammo_type_choices[i] = new wxChoice(main_panel, MENU_TRIGGER_AMMO_TYPE + id_offset, wxDefaultPosition, choiceSize, item_strings.size(), &item_strings[0]);
		right_grid_sizer->Add(trigger_ammo_type_choices[i]);

		right_grid_sizer->AddSpacer(10);
		right_grid_sizer->AddSpacer(10);

		right_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Ticks/round:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_ticks_fields[i] = new wxTextCtrl(main_panel, FIELD_TRIGGER_TICKS + id_offset);
		right_grid_sizer->Add(trigger_ticks_fields[i], 0, wxALIGN_CENTER_VERTICAL);		

		right_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Recovery Ticks:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_recovery_fields[i] = new wxTextCtrl(main_panel, FIELD_TRIGGER_RECOVERY + id_offset);
		right_grid_sizer->Add(trigger_recovery_fields[i], 0, wxALIGN_CENTER_VERTICAL);		

		right_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Charging Ticks:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_charging_fields[i] = new wxTextCtrl(main_panel, FIELD_TRIGGER_CHARGING + id_offset);
		right_grid_sizer->Add(trigger_charging_fields[i], 0, wxALIGN_CENTER_VERTICAL);		

		right_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Recoil Magnitude:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_recoil_fields[i] = new wxTextCtrl(main_panel, FIELD_TRIGGER_RECOIL + id_offset);
		right_grid_sizer->Add(trigger_recoil_fields[i], 0, wxALIGN_CENTER_VERTICAL);		

		right_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Theta Error:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_theta_fields[i] = new wxTextCtrl(main_panel, FIELD_TRIGGER_THETA + id_offset);
		right_grid_sizer->Add(trigger_theta_fields[i], 0, wxALIGN_CENTER_VERTICAL);		

		right_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("dx:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_dx_fields[i] = new wxTextCtrl(main_panel, FIELD_TRIGGER_DX + id_offset);
		right_grid_sizer->Add(trigger_dx_fields[i], 0, wxALIGN_CENTER_VERTICAL);		

		right_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("dz:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_dz_fields[i] = new wxTextCtrl(main_panel, FIELD_TRIGGER_DZ + id_offset);
		right_grid_sizer->Add(trigger_dz_fields[i], 0, wxALIGN_CENTER_VERTICAL);

		right_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Burst Count:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_burst_count_fields[i] = new wxTextCtrl(main_panel, FIELD_TRIGGER_BURST_COUNT + id_offset);
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

		right_grid_sizer->Add(new wxStaticText(main_panel, wxID_ANY, _("Shell casing type:")), 0, wxALIGN_CENTER_VERTICAL);
		trigger_shell_casing_type_choices[i] = new wxChoice(main_panel, MENU_TRIGGER_SHELL_CASING_TYPE + id_offset, wxDefaultPosition, choiceSize, 6, shell_casing_type_strings);
		right_grid_sizer->Add(trigger_shell_casing_type_choices[i]);		

		static_box_sizer->AddSpacer(20);
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
	main_panel = new wxPanel(mFrame);
	main_panel->Show();
	tree = new wxTreeCtrl(main_panel, -1, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT);
	tree->DeleteAllItems();
	wxTreeItemId treeroot = tree->AddRoot(doc->GetFilename());
	mainbox->Add(tree, 2, wxEXPAND);

	// empty space
	dummy_sizer = new wxBoxSizer(wxVERTICAL);
	wxPanel* dummy_panel = new wxPanel(main_panel);
	dummy_sizer->Add(dummy_panel, 1, wxEXPAND);
	mainbox->Add(dummy_sizer, 5);

	CreateAliens();
	CreateEffects();
	CreatePhysicsConstants();
	CreateShots();
	CreateWeapons();

	mainbox->Show(dummy_sizer, false);
	mainbox->Show(weapons_triggers_sizer, true);

	mainbox->Layout();
	main_panel->SetSizer(mainbox);
	mainbox->SetSizeHints(mFrame);
	mainbox->Fit(mFrame);

	menubar = mFrame->GetMenuBar();

	mainbox->Show(dummy_sizer, true);
	mainbox->Show(weapons_triggers_sizer, false);

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


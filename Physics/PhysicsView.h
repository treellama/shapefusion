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

#ifndef __PHYSICSVIEW_H__
#define __PHYSICSVIEW_H__

#include "wx/docview.h"
#include "wx/treectrl.h"
#include "PhysicsDocument.h"
#include "../ShapeFusionApp.h"

class PhysicsView : public wxView
{
	DECLARE_DYNAMIC_CLASS(PhysicsView)

private:
	enum {
		FIELD_ALIEN_COLLECTION,
		FIELD_ALIEN_COLOR_TABLE,
		FIELD_ALIEN_HIT,
		FIELD_ALIEN_HARD_DYING,
		FIELD_ALIEN_SOFT_DYING,
		FIELD_ALIEN_HARD_DEAD,
		FIELD_ALIEN_SOFT_DEAD,
		FIELD_ALIEN_STATIONARY,
		FIELD_ALIEN_MOVING,
		FIELD_ALIEN_TELEPORT_IN,
		FIELD_ALIEN_TELEPORT_OUT,
		FIELD_ALIEN_MELEE_ATTACK_SHAPE,
		FIELD_ALIEN_RANGED_ATTACK_SHAPE,

		MENU_ALIEN_ACTIVATION,
		MENU_ALIEN_FRIENDLY_ACTIVATION,
		MENU_ALIEN_CLEAR,
		MENU_ALIEN_KILL,
		MENU_ALIEN_APOLOGY,
		MENU_ALIEN_FRIENDLY_FIRE,
		MENU_ALIEN_FLAMING,
		MENU_ALIEN_RANDOM,

		FIELD_ALIEN_RANDOM_SOUND_MASK,
		FIELD_ALIEN_SOUND_PITCH,

		FIELD_ALIEN_ATTACK_FREQUENCY,

		MENU_ALIEN_ATTACK_TYPE,
		FIELD_ALIEN_ATTACK_REPETITIONS,
		FIELD_ALIEN_ATTACK_ERROR,
		FIELD_ALIEN_ATTACK_RANGE,
		FIELD_ALIEN_ATTACK_SEQUENCE,
		FIELD_ALIEN_ATTACK_DX,
		FIELD_ALIEN_ATTACK_DY,
		FIELD_ALIEN_ATTACK_DZ,
		NUM_ALIEN_ATTACK_CONTROLS = 8,

		FIELD_ALIEN_SHRAPNEL_RADIUS = MENU_ALIEN_ATTACK_TYPE + NUM_ALIEN_ATTACK_CONTROLS * 2,
		MENU_ALIEN_SHRAPNEL_DAMAGE_TYPE,
		FIELD_ALIEN_SHRAPNEL_BASE_DAMAGE,
		FIELD_ALIEN_SHRAPNEL_RANDOM_DAMAGE,
		FIELD_ALIEN_SHRAPNEL_DAMAGE_SCALE,
		CB_ALIEN_SHRAPNEL_ALIEN_DAMAGE,

		MENU_ALIEN_RANGED_IMPACT_EFFECT,
		MENU_ALIEN_MELEE_IMPACT_EFFECT,

		FIELD_ALIEN_VITALITY,
		FIELD_ALIEN_RADIUS,
		FIELD_ALIEN_HEIGHT,

		FIELD_ALIEN_SPEED,
		FIELD_ALIEN_TERMINAL_VELOCITY,
		FIELD_ALIEN_GRAVITY,

		FIELD_ALIEN_MIN_LEDGE_JUMP,
		FIELD_ALIEN_MAX_LEDGE_JUMP,
		FIELD_ALIEN_EXT_VELOCITY_SCALE,
		FIELD_ALIEN_HOVER_HEIGHT,
		FIELD_ALIEN_DOOR_RETRY_MASK,
	
		FIELD_ALIEN_VISUAL_RANGE,
		FIELD_ALIEN_DARK_VISUAL_RANGE,
		FIELD_ALIEN_INTELLIGENCE,
		
		MENU_ALIEN_CARRYING_ITEM,
		MENU_ALIEN_CONTRAIL_EFFECT,

		MENU_ALIEN_CLASS,
		CB_ALIEN_FRIENDS,
		CB_ALIEN_ENEMIES = CB_ALIEN_FRIENDS + 16,
		CB_ALIEN_FLAGS = CB_ALIEN_ENEMIES + 16,
		CB_ALIEN_IMMUNITIES = CB_ALIEN_FLAGS + 27,
		CB_ALIEN_WEAKNESSES = CB_ALIEN_IMMUNITIES + 24,

		FIELD_EFFECT_COLLECTION = CB_ALIEN_WEAKNESSES + 24,
		FIELD_EFFECT_COLOR_TABLE,
		FIELD_EFFECT_SEQUENCE,
		FIELD_EFFECT_PITCH,
		MENU_EFFECT_DELAY_SOUND,
		CB_EFFECT_END_WHEN_ANIMATION_LOOPS,
		CB_EFFECT_END_WHEN_TRANSFER_ANIMATION_LOOPS,
		CB_EFFECT_SOUND_ONLY,
		CB_EFFECT_MEDIA_EFFECT,

		CB_SHOT_FLAGS,
		CB_SHOT_ALIEN_DAMAGE = CB_SHOT_FLAGS + 22,

		FIELD_SHOT_COLLECTION,
		FIELD_SHOT_COLOR_TABLE,
		FIELD_SHOT_SEQUENCE,
		FIELD_SHOT_DAMAGE_BASE,
		FIELD_SHOT_DAMAGE_RANDOM,
		FIELD_SHOT_RADIUS,
		FIELD_SHOT_AREA_OF_EFFECT,
		FIELD_SHOT_SPEED,
		FIELD_SHOT_RANGE,
		FIELD_SHOT_CONTRAIL_TICKS,
		FIELD_SHOT_MAXIMUM_CONTRAILS,
		
		FIELD_SHOT_DAMAGE_SCALE,
		FIELD_SHOT_SOUND_PITCH,

		MENU_SHOT_DAMAGE_TYPE,
		MENU_SHOT_FLYBY_SOUND,
		MENU_SHOT_REBOUND_SOUND,
		MENU_SHOT_DETONATION_EFFECT,
		MENU_SHOT_MEDIA_DETONATION_EFFECT,
		MENU_SHOT_CONTRAIL,
		MENU_SHOT_MEDIA_IMPACT,

	};

	wxBoxSizer* mainbox;
	wxTreeCtrl* tree;
	wxBoxSizer* dummy_sizer;

	wxBoxSizer* aliens_appearance_sizer;

	wxTextCtrl* alien_collection_field;
	wxTextCtrl* alien_color_table_field;
	wxTextCtrl* alien_hit_field;
	wxTextCtrl* alien_hard_dying_field;
	wxTextCtrl* alien_soft_dying_field;
	wxTextCtrl* alien_hard_dead_field;
	wxTextCtrl* alien_soft_dead_field;
	wxTextCtrl* alien_stationary_field;
	wxTextCtrl* alien_moving_field;
	wxTextCtrl* alien_teleport_in_field;
	wxTextCtrl* alien_teleport_out_field;
	wxTextCtrl* alien_melee_attack_shape_field;
	wxTextCtrl* alien_ranged_attack_shape_field;

	wxChoice* alien_activation_choice;
	wxChoice* alien_friendly_activation_choice;
	wxChoice* alien_clear_choice;
	wxChoice* alien_kill_choice;
	wxChoice* alien_apology_choice;
	wxChoice* alien_friendly_fire_choice;
	wxChoice* alien_flaming_choice;
	wxChoice* alien_random_choice;

	wxTextCtrl* alien_random_sound_mask_field;
	wxTextCtrl* alien_sound_pitch_field;
	
	wxBoxSizer* aliens_combat_sizer;

	wxTextCtrl* alien_attack_frequency_field;

	wxChoice* alien_attack_type_choices[2];
	wxTextCtrl* alien_attack_repetitions_fields[2];
	wxTextCtrl* alien_attack_error_fields[2];
	wxTextCtrl* alien_attack_range_fields[2];
	wxTextCtrl* alien_attack_sequence_fields[2];
	wxTextCtrl* alien_attack_dx_fields[2];
	wxTextCtrl* alien_attack_dy_fields[2];
	wxTextCtrl* alien_attack_dz_fields[2];

	wxTextCtrl* alien_shrapnel_radius_field;
	wxChoice* alien_shrapnel_damage_type_choice;
	wxTextCtrl* alien_shrapnel_base_damage_field;
	wxTextCtrl* alien_shrapnel_random_damage_field;
	wxTextCtrl* alien_shrapnel_damage_scale_field;
	wxCheckBox* alien_shrapnel_alien_damage_checkbox;

	wxChoice* alien_ranged_impact_effect_choice;
	wxChoice* alien_melee_impact_effect_choice;

	wxBoxSizer* aliens_constants_sizer;

	wxTextCtrl* alien_vitality_field;
	wxTextCtrl* alien_radius_field;
	wxTextCtrl* alien_height_field;

	wxTextCtrl* alien_speed_field;
	wxTextCtrl* alien_terminal_velocity_field;
	wxTextCtrl* alien_gravity_field;

	wxTextCtrl* alien_min_ledge_jump_field;
	wxTextCtrl* alien_max_ledge_jump_field;
	wxTextCtrl* alien_ext_velocity_scale_field;
	wxTextCtrl* alien_hover_height_field;
	wxTextCtrl* alien_door_retry_mask_field;
	
	wxTextCtrl* alien_visual_range_field;
	wxTextCtrl* alien_dark_visual_range_field;
	wxTextCtrl* alien_intelligence_field;
	
	wxChoice* alien_carrying_item_choice;
	wxChoice* alien_contrail_effect_choice;

	wxBoxSizer* aliens_behavior_sizer;

	wxChoice* alien_class_choice;

	wxCheckBox* alien_friends_checkboxes[16];
	wxCheckBox* alien_enemies_checkboxes[16];
	wxCheckBox* alien_flags_checkboxes[27];

	wxBoxSizer* aliens_immunities_sizer;

	wxCheckBox* alien_immunities_checkboxes[24];
	wxCheckBox* alien_weaknesses_checkboxes[24];
	
	wxBoxSizer* effects_sizer;

	wxTextCtrl* eff_collection_field;
	wxTextCtrl* eff_color_table_field;
	wxTextCtrl* eff_sequence_field;
	wxTextCtrl* eff_pitch_field;

	wxTextCtrl* eff_delay_field;

	wxChoice* eff_delay_sound_choice;

	wxCheckBox* eff_end_when_animation_loops_checkbox;
	wxCheckBox* eff_end_when_transfer_animation_loops_checkbox;
	wxCheckBox* eff_sound_only_checkbox;
	wxCheckBox* eff_media_effect_checkbox;

	wxFlexGridSizer* physics_sizer;

	wxTextCtrl* max_forward_velocity_field;
	wxTextCtrl* max_backward_velocity_field;
	wxTextCtrl* max_perpendicular_velocity_field;

	wxTextCtrl* acceleration_field;
	wxTextCtrl* deceleration_field;
	wxTextCtrl* airborne_deceleration_field;
	wxTextCtrl* gravitational_acceleration_field;
	wxTextCtrl* climbing_acceleration_field;
	wxTextCtrl* terminal_velocity_field;
	wxTextCtrl* external_deceleration_field;

	wxTextCtrl* step_delta_field;
	wxTextCtrl* step_amplitude_field;

	wxTextCtrl* radius_field;
	wxTextCtrl* height_field;

	wxTextCtrl* angular_acceleration_field;
	wxTextCtrl* angular_deceleration_field;
	wxTextCtrl* maximum_angular_velocity_field;
	wxTextCtrl* angular_recentering_velocity_field;
	wxTextCtrl* head_angular_velocity_field;
	wxTextCtrl* head_angular_maximum_field;
	wxTextCtrl* maximum_elevation_field;
	wxTextCtrl* external_angular_deceleration_field;

	wxTextCtrl* dead_height_field;
	wxTextCtrl* camera_height_field;
	wxTextCtrl* splash_height_field;
	wxTextCtrl* half_camera_separation_field;

	wxBoxSizer* shots_sizer;

	wxTextCtrl* shots_collection_field;
	wxTextCtrl* shots_color_table_field;
	wxTextCtrl* shots_sequence_field;

	wxChoice* shots_damage_type_choice;
	wxTextCtrl* shots_damage_base_field;
	wxTextCtrl* shots_damage_random_field;
	wxTextCtrl* shots_damage_scale_field;
	wxCheckBox* shots_alien_damage_checkbox;

	wxChoice* shots_flyby_sound_choice;
	wxChoice* shots_rebound_sound_choice;
	wxTextCtrl* shots_sound_pitch_field;
	wxTextCtrl* shots_radius_field;
	wxTextCtrl* shots_area_of_effect_field;
	wxTextCtrl* shots_speed_field;
	wxTextCtrl* shots_range_field;
	wxChoice* shots_detonation_effect_choice;
	wxChoice* shots_media_detonation_effect_choice;
	wxChoice* shots_contrail_choice;
	wxTextCtrl* shots_contrail_ticks_field;
	wxTextCtrl* shots_maximum_contrails_field;

	wxCheckBox* shots_flags_checkboxes[22];

	wxChoice* shots_media_impact_choice;

	wxBoxSizer* weapons_definitions_sizer;

	wxChoice* weapon_item_type_choice;

	wxTextCtrl* weapon_collection_field;
	wxTextCtrl* weapon_color_table_field;
	wxTextCtrl* weapon_idle_field;
	wxTextCtrl* weapon_firing_field;
	wxTextCtrl* weapon_reloading_field;
	wxTextCtrl* weapon_charging_field;
	wxTextCtrl* weapon_charged_field;
	wxTextCtrl* weapon_flash_intensity_field;

	wxTextCtrl* weapon_ready_field;
	wxTextCtrl* weapon_await_reload_field;
	wxTextCtrl* weapon_loading_field;
	wxTextCtrl* weapon_finish_loading_field;
	wxTextCtrl* weapon_flash_decay_field;

	wxChoice* weapon_class_choice;

	wxTextCtrl* weapon_idle_height_field;
	wxTextCtrl* weapon_bob_amplitude_field;
	wxTextCtrl* weapon_kick_height_field;
	wxTextCtrl* weapon_reload_height_field;
	wxTextCtrl* weapon_idle_width_field;

	wxCheckBox* weapon_flags_checkboxes[10];

	wxBoxSizer* weapons_triggers_sizer;

	wxChoice* trigger_projectile_choices[2];
	wxTextCtrl* trigger_rounds_fields[2];
	wxChoice* trigger_ammo_type_choices[2];

	wxChoice* trigger_firing_choices[2];
	wxChoice* trigger_click_choices[2];
	wxChoice* trigger_charging_choices[2];
	wxChoice* trigger_shell_casing_choices[2];
	wxChoice* trigger_reloading_choices[2];
	wxChoice* trigger_charged_choices[2];
	
	wxTextCtrl* trigger_ticks_fields[2];
	wxTextCtrl* trigger_recovery_fields[2];
	wxTextCtrl* trigger_charging_fields[2];
	wxTextCtrl* trigger_recoil_fields[2];
	wxTextCtrl* trigger_theta_fields[2];
	wxTextCtrl* trigger_dx_fields[2];
	wxTextCtrl* trigger_dz_fields[2];
	wxTextCtrl* trigger_burst_count_fields[2];
	
	wxChoice* trigger_shell_casing_type_choices[2];
	
protected:
	DECLARE_EVENT_TABLE();

public:
	PhysicsView() { }
	~PhysicsView(){ }

	bool OnCreate(wxDocument* doc, long flags);
	void OnDraw(wxDC* dc);
	void OnUpdate(wxView* sender, wxObject* hint = (wxObject *) NULL);
	bool OnClose(bool deletewindow = false);
	
	// control callbacks
	void OnTreeSelect(wxTreeEvent& e);

private:
	void CreateAliens();
	void CreateEffects();
	void CreatePhysicsConstants();
	void CreateShots();
	void CreateWeapons();

	void OnSelectAlienAppearance(int index);
	void OnSelectAlienCombat(int index);
	void OnSelectAlienConstants(int index);
	void OnSelectAlienBehavior(int index);
	void OnSelectAlienImmunities(int index);
	void OnSelectPhysicsConstants(int index);
	void OnSelectEffect(int index);
	void OnSelectShot(int index);
	void OnSelectWeaponDefinition(int index);
	void OnSelectWeaponTriggers(int index);

	short GetSelection(); // index of current tree selection

	void EditAlienCheckboxes(wxCommandEvent& e);
	void EditAlienDoubles(wxCommandEvent& e);
	void EditAlienFields(wxCommandEvent& e);
	void EditAlienMenus(wxCommandEvent& e);

	void EditEffectCheckboxes(wxCommandEvent& e);
	void EditEffectFields(wxCommandEvent& e);
	void EditEffectDoubles(wxCommandEvent& e);
	void EditEffectMenus(wxCommandEvent& e);

	void EditShotCheckboxes(wxCommandEvent& e);
	void EditShotFields(wxCommandEvent& e);
	void EditShotDoubles(wxCommandEvent& e);
	void EditShotMenus(wxCommandEvent& e);

	wxFrame* mFrame;
};

#endif

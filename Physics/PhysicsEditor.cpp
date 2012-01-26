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

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/clipbrd.h"

#include <cmath>

#include "PhysicsView.h"
#include "PhysicsTreeItemData.h"
#include "../ShapeFusionMenus.h"

BEGIN_EVENT_TABLE(PhysicsView, wxView)
	EVT_MENU(EDIT_MENU_COPY, PhysicsView::MenuEditCopy)
	EVT_MENU(EDIT_MENU_PASTE, PhysicsView::MenuEditPaste)
        EVT_TREE_SEL_CHANGED(-1, PhysicsView::OnTreeSelect)
	EVT_COMMAND_RANGE(FIELD_ALIEN_COLLECTION, FIELD_ALIEN_RANGED_ATTACK_SHAPE, wxEVT_COMMAND_TEXT_UPDATED, PhysicsView::EditAlienFields)
	EVT_TEXT(FIELD_ALIEN_RANDOM_SOUND_MASK, PhysicsView::EditAlienFields)
	EVT_TEXT(FIELD_ALIEN_SOUND_PITCH, PhysicsView::EditAlienDoubles)
	EVT_COMMAND_RANGE(MENU_ALIEN_ACTIVATION, MENU_ALIEN_RANDOM, wxEVT_COMMAND_CHOICE_SELECTED, PhysicsView::EditAlienMenus)
	EVT_TEXT(FIELD_ALIEN_ATTACK_FREQUENCY, PhysicsView::EditAlienFields)
	EVT_CHOICE(MENU_ALIEN_ATTACK_TYPE, PhysicsView::EditAlienMenus)
	EVT_COMMAND_RANGE(FIELD_ALIEN_ATTACK_REPETITIONS, FIELD_ALIEN_ATTACK_DZ, wxEVT_COMMAND_TEXT_UPDATED, PhysicsView::EditAlienFields)
	EVT_CHOICE(MENU_ALIEN_ATTACK_TYPE + NUM_ALIEN_ATTACK_CONTROLS, PhysicsView::EditAlienMenus)
	EVT_COMMAND_RANGE(FIELD_ALIEN_ATTACK_REPETITIONS + NUM_ALIEN_ATTACK_CONTROLS, FIELD_ALIEN_ATTACK_DZ + NUM_ALIEN_ATTACK_CONTROLS, wxEVT_COMMAND_TEXT_UPDATED, PhysicsView::EditAlienFields)
	EVT_TEXT(FIELD_ALIEN_SHRAPNEL_RADIUS, PhysicsView::EditAlienFields)
	EVT_CHOICE(MENU_ALIEN_SHRAPNEL_DAMAGE_TYPE, PhysicsView::EditAlienMenus)
	EVT_COMMAND_RANGE(FIELD_ALIEN_SHRAPNEL_BASE_DAMAGE, FIELD_ALIEN_SHRAPNEL_RANDOM_DAMAGE, wxEVT_COMMAND_TEXT_UPDATED, PhysicsView::EditAlienFields)
	EVT_TEXT(FIELD_ALIEN_SHRAPNEL_DAMAGE_SCALE, PhysicsView::EditAlienDoubles)
	EVT_CHECKBOX(CB_ALIEN_SHRAPNEL_ALIEN_DAMAGE, PhysicsView::EditAlienCheckboxes)
	EVT_CHOICE(MENU_ALIEN_RANGED_IMPACT_EFFECT, PhysicsView::EditAlienMenus)
	EVT_CHOICE(MENU_ALIEN_MELEE_IMPACT_EFFECT, PhysicsView::EditAlienMenus)
	EVT_COMMAND_RANGE(FIELD_ALIEN_VITALITY, FIELD_ALIEN_MAX_LEDGE_JUMP, wxEVT_COMMAND_TEXT_UPDATED, PhysicsView::EditAlienFields)
	EVT_TEXT(FIELD_ALIEN_EXT_VELOCITY_SCALE, PhysicsView::EditAlienDoubles)
	EVT_COMMAND_RANGE(FIELD_ALIEN_HOVER_HEIGHT, FIELD_ALIEN_INTELLIGENCE, wxEVT_COMMAND_TEXT_UPDATED, PhysicsView::EditAlienFields)
	EVT_COMMAND_RANGE(MENU_ALIEN_CARRYING_ITEM, MENU_ALIEN_CLASS, wxEVT_COMMAND_CHOICE_SELECTED, PhysicsView::EditAlienMenus)
	EVT_COMMAND_RANGE(CB_ALIEN_FRIENDS, CB_ALIEN_WEAKNESSES + 23, wxEVT_COMMAND_CHECKBOX_CLICKED, PhysicsView::EditAlienCheckboxes)

	EVT_COMMAND_RANGE(FIELD_EFFECT_COLLECTION, FIELD_EFFECT_SEQUENCE, wxEVT_COMMAND_TEXT_UPDATED, PhysicsView::EditEffectFields)
	EVT_TEXT(FIELD_EFFECT_PITCH, PhysicsView::EditEffectDoubles)
	EVT_CHOICE(MENU_EFFECT_DELAY_SOUND, PhysicsView::EditEffectMenus)
	EVT_COMMAND_RANGE(CB_EFFECT_END_WHEN_ANIMATION_LOOPS, CB_EFFECT_MEDIA_EFFECT, wxEVT_COMMAND_CHECKBOX_CLICKED, PhysicsView::EditEffectCheckboxes)

	EVT_COMMAND_RANGE(CB_SHOT_FLAGS, CB_SHOT_ALIEN_DAMAGE, wxEVT_COMMAND_CHECKBOX_CLICKED, PhysicsView::EditShotCheckboxes)
	EVT_COMMAND_RANGE(FIELD_SHOT_COLLECTION, FIELD_SHOT_MAXIMUM_CONTRAILS, wxEVT_COMMAND_TEXT_UPDATED, PhysicsView::EditShotFields)
	EVT_COMMAND_RANGE(FIELD_SHOT_DAMAGE_SCALE, FIELD_SHOT_SOUND_PITCH, wxEVT_COMMAND_TEXT_UPDATED, PhysicsView::EditShotDoubles)
	EVT_COMMAND_RANGE(MENU_SHOT_DAMAGE_TYPE, MENU_SHOT_MEDIA_IMPACT, wxEVT_COMMAND_CHOICE_SELECTED, PhysicsView::EditShotMenus)

	EVT_COMMAND_RANGE(FIELD_MAX_FORWARD_VELOCITY, FIELD_HALF_CAMERA_SEPARATION, wxEVT_COMMAND_TEXT_UPDATED, PhysicsView::EditPhysicsConstants)

	EVT_COMMAND_RANGE(CB_WEAPON_FLAGS, CB_WEAPON_FLAGS + 9, wxEVT_COMMAND_CHECKBOX_CLICKED, PhysicsView::EditWeaponCheckboxes)
	EVT_COMMAND_RANGE(FIELD_WEAPON_COLLECTION, FIELD_WEAPON_FLASH_DECAY, wxEVT_COMMAND_TEXT_UPDATED, PhysicsView::EditWeaponFields)
	EVT_COMMAND_RANGE(FIELD_WEAPON_FLASH_INTENSITY, FIELD_WEAPON_IDLE_WIDTH, wxEVT_COMMAND_TEXT_UPDATED, PhysicsView::EditWeaponDoubles)
	EVT_COMMAND_RANGE(MENU_WEAPON_ITEM_TYPE, MENU_WEAPON_CLASS, wxEVT_COMMAND_CHOICE_SELECTED, PhysicsView::EditWeaponMenus)
	EVT_COMMAND_RANGE(FIELD_TRIGGER_ROUNDS, FIELD_TRIGGER_BURST_COUNT, wxEVT_COMMAND_TEXT_UPDATED, PhysicsView::EditTriggerFields)
	EVT_COMMAND_RANGE(FIELD_TRIGGER_ROUNDS + NUM_TRIGGER_IDS, FIELD_TRIGGER_BURST_COUNT + NUM_TRIGGER_IDS, wxEVT_COMMAND_TEXT_UPDATED, PhysicsView::EditTriggerFields)
	EVT_COMMAND_RANGE(MENU_TRIGGER_PROJECTILE, MENU_TRIGGER_SHELL_CASING_TYPE, wxEVT_COMMAND_CHOICE_SELECTED, PhysicsView::EditTriggerMenus)
	EVT_COMMAND_RANGE(MENU_TRIGGER_PROJECTILE + NUM_TRIGGER_IDS, MENU_TRIGGER_SHELL_CASING_TYPE + NUM_TRIGGER_IDS, wxEVT_COMMAND_CHOICE_SELECTED, PhysicsView::EditTriggerMenus)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(PhysicsView, wxView)


static inline wxString Format(double f) { 
	return wxString::Format(wxT("%0.4f"), f);
}

static inline wxString Format(unsigned short u) {
	return wxString::Format(wxT("%hu"), u);
}

static inline wxString Format(short i) {
	return wxString::Format(wxT("%hi"), i);
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
	
	for (int i = 0; i < 2; ++i) {
		AttackDefinition* attack = (i == 0) ? monster->GetMeleeAttack() : monster->GetRangedAttack();
		alien_attack_type_choices[i]->SetSelection(attack->GetType() + 1);
		alien_attack_repetitions_fields[i]->ChangeValue(Format(attack->GetRepetitions()));
		// Anvil displays the raw angle units here...
		alien_attack_error_fields[i]->ChangeValue(Format(static_cast<short>(std::floor(attack->GetError() * 512.0 / 360.0 + 0.5))));
		alien_attack_range_fields[i]->ChangeValue(Format(attack->GetRange()));
		alien_attack_sequence_fields[i]->ChangeValue(Format(attack->GetShape()));
		alien_attack_dx_fields[i]->ChangeValue(Format(attack->GetDx()));
		alien_attack_dy_fields[i]->ChangeValue(Format(attack->GetDy()));
		alien_attack_dz_fields[i]->ChangeValue(Format(attack->GetDz()));
	}

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

	int monster_class_index = 0;
	long monster_class = monster->GetClass();
	while (monster_class >>= 1) {
		monster_class_index++;
	}
	alien_class_choice->SetSelection(monster_class_index);

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

		switch (new_section) {
		case TREESECTION_MONSTERS:
		case TREESECTION_EFFECTS:
		case TREESECTION_PROJECTILES:
		case TREESECTION_PHYSICS:
		case TREESECTION_WEAPONS:
			menubar->Enable(EDIT_MENU_COPY, true);
			menubar->Enable(EDIT_MENU_PASTE, true);
			break;
		default:
			menubar->Enable(EDIT_MENU_COPY, false);
			menubar->Enable(EDIT_MENU_PASTE, false);
			break;
		}

		mainbox->Layout();
	}
}

short PhysicsView::GetSelection() {
	return static_cast<PhysicsTreeItemData*>(tree->GetItemData(tree->GetSelection()))->ID();
}

template<class T>
inline unsigned char* Serialize(T* element, size_t& size)
{
	size = T::kSize;
	unsigned char* data = new unsigned char[size];
	BigEndianBuffer buffer(data, size);
	element->SaveObject(buffer);
	return data;
}

void PhysicsView::MenuEditCopy(wxCommandEvent&) {
	if (wxTheClipboard->Open()) {

		PhysicsTreeItemData* data = static_cast<PhysicsTreeItemData*>(tree->GetItemData(tree->GetSelection()));
		PhysicsDocument* document = static_cast<PhysicsDocument*>(GetDocument());

		if (data) {
			size_t size = 0;
			unsigned char* buf = 0;
			wxDataFormat format;

			switch (data->Section()) {
			case TREESECTION_MONSTERS:
				buf = Serialize(document->GetMonsterDefinition(data->ID()), size);
				format.SetId(wxT("application/vnd.shapefusion.monster"));
				break;
			case TREESECTION_EFFECTS:
				buf = Serialize(document->GetEffectDefinition(data->ID()), size);
				format.SetId(wxT("application/vnd.shapefusion.effect"));
				break;
			case TREESECTION_PROJECTILES:
				buf = Serialize(document->GetProjectileDefinition(data->ID()), size);
				format.SetId(wxT("application/vnd.shapefusion.projectile"));
				break;
			case TREESECTION_PHYSICS:
				buf = Serialize(document->GetPhysicsConstants(data->ID()), size);
				format.SetId(wxT("application/vnd.shapefusion.physics"));
				break;
			case TREESECTION_WEAPONS:
				buf = Serialize(document->GetWeaponDefinition(data->ID()), size);
				format.SetId(wxT("application/vnd.shapefusion.weapon"));
				break;
			}

			wxCustomDataObject* dataObject = new wxCustomDataObject(format);
			dataObject->TakeData(size, buf);
			wxTheClipboard->SetData(dataObject);
				
			wxTheClipboard->Close();
		}
	}
}

void PhysicsView::MenuEditPaste(wxCommandEvent&) {
	if (wxTheClipboard->Open()) {
		PhysicsTreeItemData* data = static_cast<PhysicsTreeItemData*>(tree->GetItemData(tree->GetSelection()));
		PhysicsDocument* document = static_cast<PhysicsDocument*>(GetDocument());
		
		if (data) {
			wxDataFormat format;

			switch (data->Section()) {
			case TREESECTION_MONSTERS:
				format.SetId(wxT("application/vnd.shapefusion.monster"));
				break;
			case TREESECTION_EFFECTS:
				format.SetId(wxT("application/vnd.shapefusion.effect"));
				break;
			case TREESECTION_PROJECTILES:
				format.SetId(wxT("application/vnd.shapefusion.projectile"));
				break;
			case TREESECTION_PHYSICS:
				format.SetId(wxT("application/vnd.shapefusion.physics"));
				break;
			case TREESECTION_WEAPONS:
				format.SetId(wxT("application/vnd.shapefusion.weapon"));
				break;

			}

			wxCustomDataObject dataObject(format);
			if (wxTheClipboard->GetData(dataObject)) {
				BigEndianBuffer buffer(reinterpret_cast<unsigned char *>(dataObject.GetData()), dataObject.GetSize());
				
				switch (data->Section()) {
				case TREESECTION_MONSTERS:
					document->GetMonsterDefinition(data->ID())->LoadObject(buffer);
					break;
				case TREESECTION_EFFECTS:
					document->GetEffectDefinition(data->ID())->LoadObject(buffer);
					OnSelectEffect(data->ID());
					break;
				case TREESECTION_PROJECTILES:
					document->GetProjectileDefinition(data->ID())->LoadObject(buffer);
					OnSelectShot(data->ID());
					break;
				case TREESECTION_PHYSICS:
					document->GetPhysicsConstants(data->ID())->LoadObject(buffer);
					OnSelectPhysicsConstants(data->ID());
					break;
				case TREESECTION_WEAPONS:
					document->GetWeaponDefinition(data->ID())->LoadObject(buffer);
					break;
				}

				document->Modify(true);
			}
		}

		wxTheClipboard->Close();
	}
}

void PhysicsView::EditAlienCheckboxes(wxCommandEvent& e)
{
	MonsterDefinition* monster = static_cast<PhysicsDocument*>(GetDocument())->GetMonsterDefinition(GetSelection());

	int id = e.GetId();

	if (id == CB_ALIEN_SHRAPNEL_ALIEN_DAMAGE) {
		monster->GetShrapnelDamage()->SetAlien(e.IsChecked());
	} else if (id >= CB_ALIEN_FRIENDS && id < CB_ALIEN_ENEMIES) {
		monster->SetFriend(id - CB_ALIEN_FRIENDS, e.IsChecked());
	} else if (id >= CB_ALIEN_ENEMIES && id < CB_ALIEN_FLAGS) {
		monster->SetEnemy(id - CB_ALIEN_ENEMIES, e.IsChecked());
	} else if (id >= CB_ALIEN_FLAGS && id < CB_ALIEN_IMMUNITIES) {
		monster->SetFlag(id - CB_ALIEN_FLAGS + 1, e.IsChecked());
	} else if (id >= CB_ALIEN_IMMUNITIES && id < CB_ALIEN_WEAKNESSES) {
		monster->SetImmunity(id - CB_ALIEN_IMMUNITIES, e.IsChecked());
	} else if (id >= CB_ALIEN_WEAKNESSES && id <= CB_ALIEN_WEAKNESSES + 24) {
		monster->SetWeakness(id - CB_ALIEN_WEAKNESSES, e.IsChecked());
	}

	static_cast<PhysicsDocument*>(GetDocument())->Modify(true);
}

void PhysicsView::EditAlienDoubles(wxCommandEvent& e)
{
	MonsterDefinition* monster = static_cast<PhysicsDocument*>(GetDocument())->GetMonsterDefinition(GetSelection());

	double d = 0.0;
	if (e.GetString().ToDouble(&d)) {
		switch (e.GetId()) {
		case FIELD_ALIEN_SOUND_PITCH:
			monster->SetSoundPitch(d);
			break;
		case FIELD_ALIEN_SHRAPNEL_DAMAGE_SCALE:
			monster->GetShrapnelDamage()->SetScale(d);
			break;
		case FIELD_ALIEN_EXT_VELOCITY_SCALE:
			monster->SetExternalVelocityScale(d);
			break;
		}
	}

	static_cast<PhysicsDocument*>(GetDocument())->Modify(true);
}

void PhysicsView::EditAlienFields(wxCommandEvent& e)
{
	MonsterDefinition* monster = static_cast<PhysicsDocument*>(GetDocument())->GetMonsterDefinition(GetSelection());

	long v = 0;
	if (e.GetString().ToLong(&v)) {
		int id = e.GetId();
		AttackDefinition* attack = monster->GetMeleeAttack();

		if (id >= MENU_ALIEN_ATTACK_TYPE + NUM_ALIEN_ATTACK_CONTROLS && id <= FIELD_ALIEN_ATTACK_DZ + NUM_ALIEN_ATTACK_CONTROLS)
		{
			id -= NUM_ALIEN_ATTACK_CONTROLS;
			attack = monster->GetRangedAttack();
		}
		
		switch (id) {
			case FIELD_ALIEN_COLLECTION:
				if (v == -1) {
					monster->SetCollection(31);
					monster->SetColorTable(7);
					alien_collection_field->ChangeValue(wxT("31"));
					alien_color_table_field->ChangeValue(wxT("7"));
				} else {
					monster->SetCollection(v);
				}
				break;
			case FIELD_ALIEN_COLOR_TABLE:
				if (v == -1) {
					monster->SetCollection(31);
					monster->SetColorTable(7);
					alien_collection_field->ChangeValue(wxT("31"));
					alien_color_table_field->ChangeValue(wxT("7"));
				} else {
					monster->SetColorTable(v);
				}
				break;
			case FIELD_ALIEN_HIT:
				monster->SetHitShapes(v);
				break;
			case FIELD_ALIEN_HARD_DYING:
				monster->SetHardDyingShape(v);
				break;
			case FIELD_ALIEN_SOFT_DYING:
				monster->SetSoftDyingShape(v);
				break;
			case FIELD_ALIEN_HARD_DEAD:
				monster->SetHardDeadShapes(v);
				break;
			case FIELD_ALIEN_SOFT_DEAD:
				monster->SetSoftDeadShapes(v);
				break;
			case FIELD_ALIEN_STATIONARY:
				monster->SetStationaryShape(v);
				break;
			case FIELD_ALIEN_MOVING:
				monster->SetMovingShape(v);
				break;
			case FIELD_ALIEN_TELEPORT_IN:
				monster->SetTeleportInShape(v);
				break;
			case FIELD_ALIEN_TELEPORT_OUT:
				monster->SetTeleportOutShape(v);
				break;
			case FIELD_ALIEN_MELEE_ATTACK_SHAPE:
				monster->GetMeleeAttack()->SetShape(v);
				break;
			case FIELD_ALIEN_RANGED_ATTACK_SHAPE:
				monster->GetRangedAttack()->SetShape(v);
				break;
			case FIELD_ALIEN_RANDOM_SOUND_MASK:
				monster->SetRandomSoundMask(v);
				break;
			case FIELD_ALIEN_ATTACK_FREQUENCY:
				monster->SetAttackFrequency(v);
				break;
			case FIELD_ALIEN_ATTACK_REPETITIONS:
				attack->SetRepetitions(v);
				break;
			case FIELD_ALIEN_ATTACK_ERROR:
				attack->SetError(v / 512.0 * 360.0);
				break;
			case FIELD_ALIEN_ATTACK_RANGE:
				attack->SetRange(v);
				break;
			case FIELD_ALIEN_ATTACK_SEQUENCE:
				attack->SetShape(v);
				break;
			case FIELD_ALIEN_ATTACK_DX:
				attack->SetDx(v);
				break;
			case FIELD_ALIEN_ATTACK_DY:
				attack->SetDy(v);
				break;
			case FIELD_ALIEN_ATTACK_DZ:
				attack->SetDz(v);
				break;
			case FIELD_ALIEN_SHRAPNEL_RADIUS:
				monster->SetShrapnelRadius(v);
				break;
			case FIELD_ALIEN_SHRAPNEL_BASE_DAMAGE:
				monster->GetShrapnelDamage()->SetBase(v);
				break;
			case FIELD_ALIEN_SHRAPNEL_RANDOM_DAMAGE:
				monster->GetShrapnelDamage()->SetRandom(v);
				break;
			case FIELD_ALIEN_VITALITY:
				monster->SetVitality(v);
				break;
			case FIELD_ALIEN_RADIUS:
				monster->SetRadius(v);
				break;
			case FIELD_ALIEN_HEIGHT:
				monster->SetHeight(v);
				break;
			case FIELD_ALIEN_SPEED:
				monster->SetSpeed(v);
				break;
			case FIELD_ALIEN_TERMINAL_VELOCITY:
				monster->SetTerminalVelocity(v);
				break;
			case FIELD_ALIEN_GRAVITY:
				monster->SetGravity(v);
				break;				
			case FIELD_ALIEN_MIN_LEDGE_JUMP:
				monster->SetMinimumLedgeDelta(v);
				break;
			case FIELD_ALIEN_MAX_LEDGE_JUMP:
				monster->SetMaximumLedgeDelta(v);
				break;
			case FIELD_ALIEN_HOVER_HEIGHT:
				monster->SetPreferredHoverHeight(v);
				break;
			case FIELD_ALIEN_DOOR_RETRY_MASK:
				monster->SetDoorRetryMask(v);
				break;
			case FIELD_ALIEN_VISUAL_RANGE:
				monster->SetVisualRange(v);
				break;
			case FIELD_ALIEN_DARK_VISUAL_RANGE:
				monster->SetDarkVisualRange(v);
				break;
			case FIELD_ALIEN_INTELLIGENCE:
				monster->SetIntelligence(v);
				break;		
		}
	}
}

void PhysicsView::EditAlienMenus(wxCommandEvent& e)
{
	MonsterDefinition* monster = static_cast<PhysicsDocument*>(GetDocument())->GetMonsterDefinition(GetSelection());
	
	switch (e.GetId()) {
	case MENU_ALIEN_ACTIVATION:
		monster->SetActivationSound(e.GetSelection() - 1);
		break;
	case MENU_ALIEN_FRIENDLY_ACTIVATION:
		monster->SetFriendlyActivationSound(e.GetSelection() - 1);
		break;
	case MENU_ALIEN_CLEAR:
		monster->SetClearSound(e.GetSelection() - 1);
		break;
	case MENU_ALIEN_KILL:
		monster->SetKillSound(e.GetSelection() - 1);
		break;
	case MENU_ALIEN_APOLOGY:
		monster->SetApologySound(e.GetSelection() - 1);
		break;
	case MENU_ALIEN_FRIENDLY_FIRE:
		monster->SetFriendlyFireSound(e.GetSelection() - 1);
		break;
	case MENU_ALIEN_FLAMING:
		monster->SetFlamingSound(e.GetSelection() - 1);
		break;
	case MENU_ALIEN_RANDOM:
		monster->SetRandomSound(e.GetSelection() - 1);
		break;
	case MENU_ALIEN_ATTACK_TYPE:
		monster->GetMeleeAttack()->SetType(e.GetSelection() - 1);
		break;
	case MENU_ALIEN_ATTACK_TYPE + NUM_ALIEN_ATTACK_CONTROLS:
		monster->GetRangedAttack()->SetType(e.GetSelection() - 1);
		break;
	case MENU_ALIEN_SHRAPNEL_DAMAGE_TYPE:
		monster->GetShrapnelDamage()->SetType(e.GetSelection() - 1);
		break;
	case MENU_ALIEN_RANGED_IMPACT_EFFECT:
		monster->SetImpactEffect(e.GetSelection() - 1);
		break;
	case MENU_ALIEN_MELEE_IMPACT_EFFECT:
		monster->SetMeleeImpactEffect(e.GetSelection() - 1);
		break;
	case MENU_ALIEN_CARRYING_ITEM:
		monster->SetCarryingItemType(e.GetSelection() - 1);
		break;
	case MENU_ALIEN_CONTRAIL_EFFECT:
		monster->SetContrailEffect(e.GetSelection() - 1);
		break;
	case MENU_ALIEN_CLASS:
		monster->SetClass(1 << e.GetSelection());
		break;
	}

	static_cast<PhysicsDocument*>(GetDocument())->Modify(true);
}

void PhysicsView::EditEffectCheckboxes(wxCommandEvent& e)
{
	EffectDefinition* effect = static_cast<PhysicsDocument*>(GetDocument())->GetEffectDefinition(GetSelection());
	
	switch (e.GetId()) {
	case CB_EFFECT_END_WHEN_ANIMATION_LOOPS:
		effect->SetEndWhenAnimationLoops(e.IsChecked());
		break;
	case CB_EFFECT_END_WHEN_TRANSFER_ANIMATION_LOOPS:
		effect->SetEndWhenTransferAnimationLoops(e.IsChecked());
		break;
	case CB_EFFECT_SOUND_ONLY:
		effect->SetSoundOnly(e.IsChecked());
		break;
	case CB_EFFECT_MEDIA_EFFECT:
		effect->SetMediaEffect(e.IsChecked());
		break;
	}

	static_cast<PhysicsDocument*>(GetDocument())->Modify(true);	
}

void PhysicsView::EditEffectDoubles(wxCommandEvent& e)
{
	EffectDefinition* effect = static_cast<PhysicsDocument*>(GetDocument())->GetEffectDefinition(GetSelection());

	double d = 0.0;
	if (e.GetString().ToDouble(&d)) {
		switch (e.GetId()) {
		case FIELD_EFFECT_PITCH:
			effect->SetSoundPitch(d);
			break;
		}
	}

	static_cast<PhysicsDocument*>(GetDocument())->Modify(true);	
}

void PhysicsView::EditEffectFields(wxCommandEvent& e)
{
	EffectDefinition* effect = static_cast<PhysicsDocument*>(GetDocument())->GetEffectDefinition(GetSelection());

	long v = 0;
	if (e.GetString().ToLong(&v)) {
		switch (e.GetId()) {
		case FIELD_EFFECT_COLLECTION:
			if (v == -1) {
				effect->SetCollection(31);
				effect->SetColorTable(7);
				eff_collection_field->ChangeValue(wxT("31"));
				eff_color_table_field->ChangeValue(wxT("7"));
			} else {
				effect->SetCollection(v);
			}
			break;
		case FIELD_EFFECT_COLOR_TABLE:
			if (v == -1) {
				effect->SetCollection(31);
				effect->SetColorTable(7);
				eff_collection_field->ChangeValue(wxT("31"));
				eff_color_table_field->ChangeValue(wxT("7"));
			} else {
				effect->SetColorTable(v);
			}
			break;
		case FIELD_EFFECT_SEQUENCE:
			effect->SetShape(v);
			break;
		}
	}

	static_cast<PhysicsDocument*>(GetDocument())->Modify(true);
}

void PhysicsView::EditEffectMenus(wxCommandEvent& e)
{
	EffectDefinition* effect = static_cast<PhysicsDocument*>(GetDocument())->GetEffectDefinition(GetSelection());

	switch (e.GetId()) {
	case MENU_EFFECT_DELAY_SOUND:
		effect->SetDelaySound(e.GetSelection() - 1);
		break;
	}

	static_cast<PhysicsDocument*>(GetDocument())->Modify(true);
}

void PhysicsView::EditShotCheckboxes(wxCommandEvent& e)
{
	ProjectileDefinition* projectile = static_cast<PhysicsDocument*>(GetDocument())->GetProjectileDefinition(GetSelection());
	
	if (e.GetId() == CB_SHOT_ALIEN_DAMAGE) {
		projectile->GetDamage()->SetAlien(e.IsChecked());
	} else if (e.GetId() >= CB_SHOT_FLAGS && e.GetId() < CB_SHOT_FLAGS + 22) {
		projectile->SetFlag(e.GetId() - CB_SHOT_FLAGS, e.IsChecked());
	}
	
	static_cast<PhysicsDocument*>(GetDocument())->Modify(true);
}

void PhysicsView::EditShotFields(wxCommandEvent& e)
{
	ProjectileDefinition* projectile = static_cast<PhysicsDocument*>(GetDocument())->GetProjectileDefinition(GetSelection());

	long v = 0;
	if (e.GetString().ToLong(&v)) {
		switch (e.GetId()) {
		case FIELD_SHOT_COLLECTION:
			if (v == -1) {
				projectile->SetCollection(31);
				projectile->SetColorTable(7);
				shots_collection_field->ChangeValue(wxT("31"));
				shots_color_table_field->ChangeValue(wxT("7"));
			} else {
				projectile->SetCollection(v);
			}
			break;
		case FIELD_SHOT_COLOR_TABLE:
			if (v == -1) {
				projectile->SetCollection(31);
				projectile->SetColorTable(7);
				shots_collection_field->ChangeValue(wxT("31"));
				shots_color_table_field->ChangeValue(wxT("7"));
			} else {
				projectile->SetColorTable(v);
			}
			break;
		case FIELD_SHOT_SEQUENCE:
			projectile->SetShape(v);
			break;
		case FIELD_SHOT_DAMAGE_BASE:
			projectile->GetDamage()->SetBase(v);
			break;
		case FIELD_SHOT_DAMAGE_RANDOM:
			projectile->GetDamage()->SetRandom(v);
			break;
		case FIELD_SHOT_RADIUS:
			projectile->SetRadius(v);
			break;
		case FIELD_SHOT_AREA_OF_EFFECT:
			projectile->SetAreaOfEffect(v);
			break;
		case FIELD_SHOT_SPEED:
			projectile->SetSpeed(v);
			break;
		case FIELD_SHOT_RANGE:
			projectile->SetMaximumRange(v);
			break;
		case FIELD_SHOT_CONTRAIL_TICKS:
			projectile->SetTicksBetweenContrails(v);
			break;
		case FIELD_SHOT_MAXIMUM_CONTRAILS:
			projectile->SetMaximumContrails(v);
			break;
		}
	}
	static_cast<PhysicsDocument*>(GetDocument())->Modify(true);
}

void PhysicsView::EditShotDoubles(wxCommandEvent& e)
{
	ProjectileDefinition* projectile = static_cast<PhysicsDocument*>(GetDocument())->GetProjectileDefinition(GetSelection());
	
	double d = 0.0;

	if (e.GetString().ToDouble(&d)) {
		switch (e.GetId()) {
		case FIELD_SHOT_DAMAGE_SCALE:
			projectile->GetDamage()->SetScale(d);
			break;
		case FIELD_SHOT_SOUND_PITCH:
			projectile->SetSoundPitch(d);
			break;
		}
	}

	static_cast<PhysicsDocument*>(GetDocument())->Modify(true);
}

void PhysicsView::EditShotMenus(wxCommandEvent& e)
{
	ProjectileDefinition* projectile = static_cast<PhysicsDocument*>(GetDocument())->GetProjectileDefinition(GetSelection());

	switch (e.GetId()) {
	case MENU_SHOT_DAMAGE_TYPE:
		projectile->GetDamage()->SetType(e.GetSelection() - 1);
		break;
	case MENU_SHOT_FLYBY_SOUND:
		projectile->SetFlybySound(e.GetSelection() - 1);
		break;
	case MENU_SHOT_REBOUND_SOUND:
		projectile->SetReboundSound(e.GetSelection() - 1);
		break;
	case MENU_SHOT_DETONATION_EFFECT:
		projectile->SetDetonationEffect(e.GetSelection() - 1);
		break;
	case MENU_SHOT_MEDIA_DETONATION_EFFECT:
		projectile->SetMediaDetonationEffect(e.GetSelection() - 1);
		break;
	case MENU_SHOT_CONTRAIL:
		projectile->SetContrailEffect(e.GetSelection() - 1);
		break;
	case MENU_SHOT_MEDIA_IMPACT:
		projectile->SetMediaProjectilePromotion(e.GetSelection() - 1);
		break;
	}

	static_cast<PhysicsDocument*>(GetDocument())->Modify(true);	
}

void PhysicsView::EditPhysicsConstants(wxCommandEvent& e)
{
	PhysicsConstants* constants = static_cast<PhysicsDocument*>(GetDocument())->GetPhysicsConstants(GetSelection());

	double d = 0.0;
	if (e.GetString().ToDouble(&d)) {
		switch (e.GetId()) {
		case FIELD_MAX_FORWARD_VELOCITY:
			constants->SetMaximumForwardVelocity(d);
			break;
		case FIELD_MAX_BACKWARD_VELOCITY:
			constants->SetMaximumBackwardVelocity(d);
			break;
		case FIELD_MAX_PERPENDICULAR_VELOCITY:
			constants->SetMaximumPerpendicularVelocity(d);
			break;

		case FIELD_ACCELERATION:
			constants->SetAcceleration(d);
			break;
		case FIELD_DECELERATION:
			constants->SetDeceleration(d);
			break;
		case FIELD_AIRBORNE_DECELERATION:
			constants->SetAirborneDeceleration(d);
			break;
		case FIELD_GRAVITATIONAL_ACCELERATION:
			constants->SetGravitationalAcceleration(d);
			break;
		case FIELD_CLIMBING_ACCELERATION:
			constants->SetClimbingAcceleration(d);
			break;
		case FIELD_TERMINAL_VELOCITY:
			constants->SetTerminalVelocity(d);
			break;
		case FIELD_EXTERNAL_DECELERATION:
			constants->SetExternalDeceleration(d);
			break;

		case FIELD_STEP_DELTA:
			constants->SetStepDelta(d);
			break;
		case FIELD_STEP_AMPLITUDE:
			constants->SetStepAmplitude(d);
			break;

		case FIELD_RADIUS:
			constants->SetRadius(d);
			break;
		case FIELD_HEIGHT:
			constants->SetHeight(d);
			break;

		case FIELD_ANGULAR_ACCELERATION:
			constants->SetAngularAcceleration(d);
			break;
		case FIELD_ANGULAR_DECELERATION:
			constants->SetAngularDeceleration(d);
			break;
		case FIELD_MAXIMUM_ANGULAR_VELOCITY:
			constants->SetMaximumAngularVelocity(d);
			break;
		case FIELD_ANGULAR_RECENTERING_VELOCITY:
			constants->SetAngularRecenteringVelocity(d);
			break;
		case FIELD_HEAD_ANGULAR_VELOCITY:
			constants->SetFastAngularVelocity(d);
			break;
		case FIELD_HEAD_ANGULAR_MAXIMUM:
			constants->SetFastAngularMaximum(d);
			break;
		case FIELD_MAXIMUM_ELEVATION:
			constants->SetMaximumElevation(d);
			break;
		case FIELD_EXTERNAL_ANGULAR_DECELERATION:
			constants->SetExternalAngularDeceleration(d);
			break;

		case FIELD_DEAD_HEIGHT:
			constants->SetDeadHeight(d);
			break;
		case FIELD_CAMERA_HEIGHT:
			constants->SetCameraHeight(d);
			break;
		case FIELD_SPLASH_HEIGHT:
			constants->SetSplashHeight(d);
			break;
		case FIELD_HALF_CAMERA_SEPARATION:
			constants->SetHalfCameraSeparation(d);
			break;
		}
	}
	
	static_cast<PhysicsDocument*>(GetDocument())->Modify(true);	
}

void PhysicsView::EditWeaponCheckboxes(wxCommandEvent& e)
{
	WeaponDefinition* weapon = static_cast<PhysicsDocument*>(GetDocument())->GetWeaponDefinition(GetSelection());

	int flag = e.GetId() - CB_WEAPON_FLAGS;
	
	weapon->SetFlag(flag >= 5 ? flag + 1: flag, e.IsChecked());

	static_cast<PhysicsDocument*>(GetDocument())->Modify(true);	
}

void PhysicsView::EditWeaponFields(wxCommandEvent& e)
{
	WeaponDefinition* weapon = static_cast<PhysicsDocument*>(GetDocument())->GetWeaponDefinition(GetSelection());

	long v = 0;
	if (e.GetString().ToLong(&v)) {
		switch (e.GetId()) {
		case FIELD_WEAPON_COLLECTION:
			if (v == -1) {
				weapon->SetCollection(31);
				weapon->SetColorTable(7);
				weapon_collection_field->ChangeValue(wxT("31"));
				weapon_color_table_field->ChangeValue(wxT("7"));
			} else {
				weapon->SetCollection(v);
			}
			break;
		case FIELD_WEAPON_COLOR_TABLE:
			if (v == -1) {
				weapon->SetCollection(31);
				weapon->SetColorTable(7);
				weapon_collection_field->ChangeValue(wxT("31"));
				weapon_color_table_field->ChangeValue(wxT("7"));
			} else {
				weapon->SetColorTable(v);
			}
			break;
		case FIELD_WEAPON_IDLE:
			weapon->SetIdleShape(v);
			break;
		case FIELD_WEAPON_FIRING:
			weapon->SetFiringShape(v);
			break;
		case FIELD_WEAPON_RELOADING:
			weapon->SetReloadingShape(v);
			break;
		case FIELD_WEAPON_CHARGING:
			weapon->SetChargingShape(v);
			break;
		case FIELD_WEAPON_CHARGED:
			weapon->SetChargedShape(v);
			break;
		case FIELD_WEAPON_READY:
			weapon->SetReadyTicks(v);
			break;
		case FIELD_WEAPON_AWAIT_RELOAD:
			weapon->SetAwaitReloadTicks(v);
			break;
		case FIELD_WEAPON_LOADING:
			weapon->SetLoadingTicks(v);
			break;
		case FIELD_WEAPON_FINISH_LOADING:
			weapon->SetFinishLoadingTicks(v);
			break;
		case FIELD_WEAPON_FLASH_DECAY:
			weapon->SetFiringIntensityDecayTicks(v);
			break;
		}
	}

	static_cast<PhysicsDocument*>(GetDocument())->Modify(true);	

}

void PhysicsView::EditWeaponDoubles(wxCommandEvent& e)
{
	WeaponDefinition* weapon = static_cast<PhysicsDocument*>(GetDocument())->GetWeaponDefinition(GetSelection());

	double d = 0.0;
	if (e.GetString().ToDouble(&d)) {
		switch (e.GetId()) {
		case FIELD_WEAPON_FLASH_INTENSITY:
			weapon->SetFiringLightIntensity(d);
			break;
		case FIELD_WEAPON_IDLE_HEIGHT:
			weapon->SetIdleHeight(d);
			break;
		case FIELD_WEAPON_BOB_AMPLITUDE:
			weapon->SetBobAmplitude(d);
			break;
		case FIELD_WEAPON_KICK_HEIGHT:
			weapon->SetKickHeight(d);
			break;
		case FIELD_WEAPON_RELOAD_HEIGHT:
			weapon->SetReloadHeight(d);
			break;
		case FIELD_WEAPON_IDLE_WIDTH:
			weapon->SetIdleWidth(d);
			break;
		}
	}

	static_cast<PhysicsDocument*>(GetDocument())->Modify(true);	
}

void PhysicsView::EditWeaponMenus(wxCommandEvent& e)
{
	WeaponDefinition* weapon = static_cast<PhysicsDocument*>(GetDocument())->GetWeaponDefinition(GetSelection());

	switch (e.GetId()) {
	case MENU_WEAPON_ITEM_TYPE:
		weapon->SetItemType(e.GetSelection() - 1);
		break;
	case MENU_WEAPON_CLASS:
		weapon->SetWeaponClass(e.GetSelection() - 1);
		break;
	}

	static_cast<PhysicsDocument*>(GetDocument())->Modify(true);	
}

void PhysicsView::EditTriggerFields(wxCommandEvent& e)
{
	WeaponDefinition* weapon = static_cast<PhysicsDocument*>(GetDocument())->GetWeaponDefinition(GetSelection());

	long v = 0;
	if (e.GetString().ToLong(&v)) {
		int id = e.GetId();
		TriggerDefinition* trigger = weapon->GetPrimaryTrigger();

		if (id >= FIELD_TRIGGER_ROUNDS + NUM_TRIGGER_IDS) {
			id -= NUM_TRIGGER_IDS;
			trigger = weapon->GetSecondaryTrigger();
		}

		switch (id) {
		case FIELD_TRIGGER_ROUNDS:
			trigger->SetRoundsPerMagazine(v);
			break;
		case FIELD_TRIGGER_TICKS:
			trigger->SetTicksPerRound(v);
			break;
		case FIELD_TRIGGER_RECOVERY:
			trigger->SetRecoveryTicks(v);
			break;
		case FIELD_TRIGGER_CHARGING:
			trigger->SetChargingTicks(v);
			break;
		case FIELD_TRIGGER_RECOIL:
			trigger->SetRecoilMagnitude(v);
			break;
		case FIELD_TRIGGER_THETA:
			trigger->SetThetaError(v);
			break;
		case FIELD_TRIGGER_DX:
			trigger->SetDx(v);
			break;
		case FIELD_TRIGGER_DZ:
			trigger->SetDz(v);
			break;
		case FIELD_TRIGGER_BURST_COUNT:
			trigger->SetBurstCount(v);
			break;
		}
	}

	static_cast<PhysicsDocument*>(GetDocument())->Modify(true);
}

void PhysicsView::EditTriggerMenus(wxCommandEvent& e)
{
	WeaponDefinition* weapon = static_cast<PhysicsDocument*>(GetDocument())->GetWeaponDefinition(GetSelection());

	int id = e.GetId();
	TriggerDefinition* trigger = weapon->GetPrimaryTrigger();
	
	if (id >= FIELD_TRIGGER_ROUNDS + NUM_TRIGGER_IDS) {
		id -= NUM_TRIGGER_IDS;
		trigger = weapon->GetSecondaryTrigger();
	}

	switch (id) {
	case MENU_TRIGGER_PROJECTILE:
		trigger->SetProjectileType(e.GetSelection() - 1);
		break;
	case MENU_TRIGGER_AMMO_TYPE:
		trigger->SetAmmunitionType(e.GetSelection() - 1);
		break;
	case MENU_TRIGGER_FIRING:
		trigger->SetFiringSound(e.GetSelection() - 1);
		break;
	case MENU_TRIGGER_CLICK:
		trigger->SetClickSound(e.GetSelection() - 1);
		break;
	case MENU_TRIGGER_CHARGING:
		trigger->SetChargingSound(e.GetSelection() - 1);
		break;
	case MENU_TRIGGER_SHELL_CASING:
		trigger->SetShellCasingSound(e.GetSelection() - 1);
		break;
	case MENU_TRIGGER_RELOADING:
		trigger->SetReloadingSound(e.GetSelection() - 1);
		break;
	case MENU_TRIGGER_CHARGED:
		trigger->SetChargedSound(e.GetSelection() - 1);
		break;
	case MENU_TRIGGER_SHELL_CASING_TYPE:
		trigger->SetShellCasingType(e.GetSelection() - 1);
		break;
	}

	static_cast<PhysicsDocument*>(GetDocument())->Modify(true);
}

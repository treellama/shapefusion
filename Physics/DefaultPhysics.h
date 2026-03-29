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

#ifndef DEFAULTPHYSICS_H
#define DEFAULTPHYSICS_H

#include <cstdint>

using word = uint32_t;

static constexpr int16_t SHORT_MIN = INT16_MIN;
static constexpr int16_t SHORT_MAX = INT16_MAX;

using fixed = int32_t;
static constexpr fixed FIXED_ONE = 1 << 16;
static constexpr fixed FIXED_ONE_HALF = FIXED_ONE / 2;

using angle = int16_t;
static constexpr angle NUMBER_OF_ANGLES = 1 << 9;
static constexpr angle HALF_CIRCLE = NUMBER_OF_ANGLES / 2;
static constexpr angle QUARTER_CIRCLE = NUMBER_OF_ANGLES / 4;

static constexpr int16_t NONE = -1;
static constexpr uint16_t UNONE = UINT16_MAX;

static constexpr int TICKS_PER_SECOND = 30;

using world_distance = int16_t;
static constexpr world_distance WORLD_ONE = 1 << 10;
static constexpr world_distance WORLD_ONE_HALF = WORLD_ONE / 2;
static constexpr world_distance WORLD_ONE_FOURTH = WORLD_ONE / 4;
static constexpr world_distance WORLD_THREE_FOURTHS = WORLD_ONE * 3 / 4;

/*
 * map.h
 */

#define FLAG(b) (1<<(b))
enum /* damage types */
{
	_damage_explosion,
	_damage_electrical_staff,
	_damage_projectile,
	_damage_absorbed,
	_damage_flame,
	_damage_hound_claws,
	_damage_alien_projectile,
	_damage_hulk_slap,
	_damage_compiler_bolt,
	_damage_fusion_bolt,
	_damage_hunter_bolt,
	_damage_fist,
	_damage_teleporter,
	_damage_defender,
	_damage_yeti_claws,
	_damage_yeti_projectile,
	_damage_crushing,
	_damage_lava,
	_damage_suffocation,
	_damage_goo,
	_damage_energy_drain,
	_damage_oxygen_drain,
	_damage_hummer_bolt,
	_damage_shotgun_projectile
};

enum /* damage flags */
{
	_alien_damage= 0x1 /* will be decreased at lower difficulty levels */
};

struct damage_definition
{
	short type, flags;
	
	short base, random;
	fixed scale;
};

/*
 * game_sound.h
 */

enum
{
	NUMBER_OF_SOUND_VOLUME_LEVELS= 8,
	
	MAXIMUM_SOUND_VOLUME_BITS= 8,
	MAXIMUM_SOUND_VOLUME= 1<<MAXIMUM_SOUND_VOLUME_BITS
};

enum // sound sources
{
	_8bit_22k_source,
	_16bit_22k_source,
	
	NUMBER_OF_SOUND_SOURCES
};

enum // frequencies
{
	_lower_frequency= FIXED_ONE-FIXED_ONE/8,
	_normal_frequency= FIXED_ONE,
	_higher_frequency= FIXED_ONE+FIXED_ONE/8
};

/* ---------- sound codes */

enum /* ambient sound codes */
{
	_ambient_snd_water,
	_ambient_snd_sewage,
	_ambient_snd_lava,
	_ambient_snd_goo,
	_ambient_snd_under_media,
	_ambient_snd_wind,
	_ambient_snd_waterfall,
	_ambient_snd_siren,
	_ambient_snd_fan,
	_ambient_snd_spht_door,
	_ambient_snd_spht_platform,
	_ambient_snd_heavy_spht_door,
	_ambient_snd_heavy_spht_platform,
	_ambient_snd_light_machinery,
	_ambient_snd_heavy_machinery,
	_ambient_snd_transformer,
	_ambient_snd_sparking_transformer,
	_ambient_snd_machine_binder,
	_ambient_snd_machine_bookpress,
	_ambient_snd_machine_puncher,
	_ambient_snd_electric,
	_ambient_snd_alarm,
	_ambient_snd_night_wind,
	_ambient_snd_pfhor_door,
	_ambient_snd_pfhor_platform,
	_ambient_snd_alien_noise1,
	_ambient_snd_alien_noise2,
	_ambient_snd_jjaro_noise,
	
	NUMBER_OF_AMBIENT_SOUND_DEFINITIONS
};

enum /* random sound codes */
{
	_random_snd_water_drip,
	_random_snd_surface_explosion,
	_random_snd_underground_explosion,
	_random_snd_owl,
	_random_snd_jjaro_creak,

	NUMBER_OF_RANDOM_SOUND_DEFINITIONS
};

enum /* sound codes */
{
	_snd_startup,
	_snd_teleport_in,
	_snd_teleport_out,
	_snd_body_being_crunched,
	_snd_jjaro_creak,
	_snd_absorbed,

	_snd_breathing,
	_snd_oxygen_warning,
	_snd_suffocation,

	_snd_energy_refuel,
	_snd_oxygen_refuel,
	_snd_cant_toggle_switch,
	_snd_switch_on,
	_snd_switch_off,
	_snd_puzzle_switch,
	_snd_chip_insertion,
	_snd_pattern_buffer,
	_snd_destroy_control_panel,
	
	_snd_adjust_volume,
	_snd_got_powerup,
	_snd_got_item,

	_snd_bullet_ricochet,
	_snd_metallic_ricochet,
	_snd_empty_gun,

	_snd_spht_door_opening,
	_snd_spht_door_closing,
	_snd_spht_door_obstructed,
	
	_snd_spht_platform_starting,
	_snd_spht_platform_stopping,

	_snd_owl,
	_snd_smg_firing,
	_snd_smg_reloading,
	
	_snd_heavy_spht_platform_starting,
	_snd_heavy_spht_platform_stopping,

	_snd_fist_hitting,
	
	_snd_magnum_firing,
	_snd_magnum_reloading,

	_snd_assault_rifle_firing,
	_snd_grenade_launcher_firing,
	_snd_grenade_exploding,
	_snd_grenade_flyby,
	
	_snd_fusion_firing,
	_snd_fusion_exploding,
	_snd_fusion_flyby,
	_snd_fusion_charging,

	_snd_rocket_exploding,
	_snd_rocket_flyby,
	_snd_rocket_firing,
	
	_snd_flamethrower,

	_snd_body_falling,
	_snd_body_exploding,
	_snd_bullet_hitting_flesh,

	_snd_fighter_activate,
	_snd_fighter_wail,
	_snd_fighter_scream,
	_snd_fighter_chatter,
	_snd_fighter_attack,
	_snd_fighter_projectile_hit,
	_snd_fighter_projectile_flyby,

	_snd_compiler_attack,
	_snd_compiler_death,
	_snd_compiler_hit,
	_snd_compiler_projectile_flyby,
	_snd_compiler_projectile_hit,
	
	_snd_cyborg_moving,
	_snd_cyborg_attack,
	_snd_cyborg_hit,
	_snd_cyborg_death,
	_snd_cyborg_projectile_bounce,
	_snd_cyborg_projectile_hit,
	_snd_cyborg_projectile_flyby,

	_snd_hummer_activate,
	_snd_hummer_start_attack,
	_snd_hummer_attack,
	_snd_hummer_dying,
	_snd_hummer_death,
	_snd_hummer_projectile_hit,
	_snd_hummer_projectile_flyby,

	_snd_human_wail,
	_snd_human_scream,
	_snd_human_hit,
	_snd_human_chatter,
	_snd_assimilated_human_chatter,
	_snd_human_trash_talk,
	_snd_human_apology,
	_snd_human_activation,
	_snd_human_clear,
	_snd_human_stop_shooting_me_you_bastard,
	_snd_human_area_secure,
	_snd_kill_the_player,
	
	_snd_water,
	_snd_sewage,
	_snd_lava,
	_snd_goo,
	_snd_under_media,
	_snd_wind,
	_snd_waterfall,
	_snd_siren,
	_snd_fan,
	_snd_spht_door,
	_snd_spht_platform,
	_snd_jjaro_noise,
	_snd_heavy_spht_platform,
	_snd_light_machinery,
	_snd_heavy_machinery,
	_snd_transformer,
	_snd_sparking_transformer,

	_snd_water_drip,
	
	_snd_walking_in_water,
	_snd_exit_water,
	_snd_enter_water,
	_snd_small_water_splash,
	_snd_medium_water_splash,
	_snd_large_water_splash,

	_snd_walking_in_lava,
	_snd_enter_lava,
	_snd_exit_lava,
	_snd_small_lava_splash,
	_snd_medium_lava_splash,
	_snd_large_lava_splash,

	_snd_walking_in_sewage,
	_snd_exit_sewage,
	_snd_enter_sewage,
	_snd_small_sewage_splash,
	_snd_medium_sewage_splash,
	_snd_large_sewage_splash,

	_snd_walking_in_goo,
	_snd_exit_goo,
	_snd_enter_goo,
	_snd_small_goo_splash,
	_snd_medium_goo_splash,
	_snd_large_goo_splash,

	_snd_major_fusion_firing,
	_snd_major_fusion_charged,

	_snd_assault_rifle_reloading,
	_snd_assault_rifle_shell_casings,
	
	_snd_shotgun_firing,
	_snd_shotgun_reloading,
	
	_snd_ball_bounce,
	_snd_you_are_it,
	_snd_got_ball,
	
	_snd_computer_interface_logon,
	_snd_computer_interface_logout,
	_snd_computer_interface_page,

	_snd_heavy_spht_door,
	_snd_heavy_spht_door_opening,
	_snd_heavy_spht_door_closing,
	_snd_heavy_spht_door_open,
	_snd_heavy_spht_door_closed,
	_snd_heavy_spht_door_obstructed,

	_snd_hunter_activate,
	_snd_hunter_attack,
	_snd_hunter_dying,
	_snd_hunter_landing,
	_snd_hunter_exploding,
	_snd_hunter_projectile_hit,
	_snd_hunter_projectile_flyby,

	_snd_enforcer_activate,
	_snd_enforcer_attack,
	_snd_enforcer_projectile_hit,
	_snd_enforcer_projectile_flyby,

	_snd_yeti_melee_attack,
	_snd_yeti_melee_attack_hit,
	_snd_yeti_projectile_attack,
	_snd_yeti_projectile_sewage_attack_hit,
	_snd_yeti_projectile_sewage_flyby,
	_snd_yeti_projectile_lava_attack_hit,
	_snd_yeti_projectile_lava_flyby,
	_snd_yeti_dying,

	_snd_machine_binder,
	_snd_machine_bookpress,
	_snd_machine_puncher,
	_snd_electric,
	_snd_alarm,
	_snd_night_wind,
	
	_snd_surface_explosion,
	_snd_underground_explosion,

	_snd_defender_attack,
	_snd_defender_hit,
	_snd_defender_flyby,
	_snd_defender_being_hit,
	_snd_defender_exploding,

	_snd_tick_chatter,
	_snd_tick_falling,
	_snd_tick_flapping,
	_snd_tick_exploding,

	_snd_ceiling_lamp_exploding,

	_snd_pfhor_platform_starting,
	_snd_pfhor_platform_stopping,
	_snd_pfhor_platform,

	_snd_pfhor_door_opening,
	_snd_pfhor_door_closing,
	_snd_pfhor_door_obstructed,
	_snd_pfhor_door,

	_snd_pfhor_switch_off,
	_snd_pfhor_switch_on,

	_snd_juggernaut_firing,
	_snd_juggernaut_warning,
	_snd_juggernaut_exploding,
	_snd_juggernaut_preparing_to_fire,

	_snd_enforcer_exploding,

	_snd_alien_noise1,
	_snd_alien_noise2,

	_snd_fusion_human_wail,
	_snd_fusion_human_scream,
	_snd_fusion_human_hit,
	_snd_fusion_human_chatter,
	_snd_assimilated_fusion_human_chatter,
	_snd_fusion_human_trash_talk,
	_snd_fusion_human_apology,
	_snd_fusion_human_activation,
	_snd_fusion_human_clear,
	_snd_fusion_human_stop_shooting_me_you_bastard,
	_snd_fusion_human_area_secure,
	_snd_fusion_kill_the_player,

	NUMBER_OF_SOUND_DEFINITIONS
};

/*
 * sound_definitions.h
 */

enum /* sound behaviors */
{
	_sound_is_quiet,
	_sound_is_normal,
	_sound_is_loud,
	NUMBER_OF_SOUND_BEHAVIOR_DEFINITIONS
};

enum /* sound chances */
{
	_ten_percent= 32768*9/10,
	_twenty_percent= 32768*8/10,
	_thirty_percent= 32768*7/10,
	_fourty_percent= 32768*6/10,
	_fifty_percent= 32768*5/10,
	_sixty_percent= 32768*4/10,
	_seventy_percent= 32768*3/10,
	_eighty_percent= 32768*2/10,
	_ninty_percent= 32768*1/10,
	_always= 0
};

/*
 * shape_descriptors.h
 */

typedef short shape_descriptor; /* [clut.3] [collection.5] [shape.8] */

#define DESCRIPTOR_SHAPE_BITS 8
#define DESCRIPTOR_COLLECTION_BITS 5
#define DESCRIPTOR_CLUT_BITS 3

#define MAXIMUM_COLLECTIONS (1<<DESCRIPTOR_COLLECTION_BITS)
#define MAXIMUM_SHAPES_PER_COLLECTION (1<<DESCRIPTOR_SHAPE_BITS)
#define MAXIMUM_CLUTS_PER_COLLECTION (1<<DESCRIPTOR_CLUT_BITS)

/* ---------- collections */

enum /* collection numbers */
{
	_collection_interface, // 0
	_collection_weapons_in_hand, // 1
	_collection_juggernaut, // 2
	_collection_tick, // 3
	_collection_rocket, // 4
	_collection_hunter, // 5
	_collection_player, // 6
	_collection_items, // 7
	_collection_trooper, // 8
	_collection_fighter, // 9
	_collection_defender, // 10
	_collection_yeti, // 11
	_collection_civilian, // 12
	_collection_vacuum_civilian, // 13
	_collection_enforcer, // 14
	_collection_hummer, // 15
	_collection_compiler, // 16
	_collection_walls1, // 17
	_collection_walls2, // 18
	_collection_walls3, // 19
	_collection_walls4, // 20 
	_collection_walls5, // 21 
	_collection_scenery1, // 22
	_collection_scenery2, // 23
	_collection_scenery3, // 24
	_collection_scenery4, // 25 pathways
	_collection_scenery5, // 26 alien
	_collection_landscape1, // 27 day
	_collection_landscape2, // 28 night
	_collection_landscape3, // 29 moon
	_collection_landscape4, // 30 
	_collection_cyborg, // 31
	
	NUMBER_OF_COLLECTIONS
};

/* ---------- macros */

#define GET_DESCRIPTOR_SHAPE(d) ((d)&(word)(MAXIMUM_SHAPES_PER_COLLECTION-1))
#define GET_DESCRIPTOR_COLLECTION(d) (((d)>>DESCRIPTOR_SHAPE_BITS)&(word)((1<<(DESCRIPTOR_COLLECTION_BITS+DESCRIPTOR_CLUT_BITS))-1))
#define BUILD_DESCRIPTOR(collection,shape) (((collection)<<DESCRIPTOR_SHAPE_BITS)|(shape))

#define BUILD_COLLECTION(collection,clut) ((collection)|(word)((clut)<<DESCRIPTOR_COLLECTION_BITS))
#define GET_COLLECTION_CLUT(collection) (((collection)>>DESCRIPTOR_COLLECTION_BITS)&(word)(MAXIMUM_CLUTS_PER_COLLECTION-1))
#define GET_COLLECTION(collection) ((collection)&(MAXIMUM_COLLECTIONS-1))

/*
 * media.h
 */

enum /* media detonation types */
{
	_small_media_detonation_effect,
	_medium_media_detonation_effect,
	_large_media_detonation_effect,
	_large_media_emergence_effect,
	NUMBER_OF_MEDIA_DETONATION_TYPES
};

/*
 * effects.h
 */

enum /* effect types */
{
	_effect_rocket_explosion,
	_effect_rocket_contrail,
	_effect_grenade_explosion,
	_effect_grenade_contrail,
	_effect_bullet_ricochet,
	_effect_alien_weapon_ricochet,
	_effect_flamethrower_burst,
	_effect_fighter_blood_splash,
	_effect_player_blood_splash,
	_effect_civilian_blood_splash,
	_effect_assimilated_civilian_blood_splash,
	_effect_enforcer_blood_splash,
	_effect_compiler_bolt_minor_detonation,
	_effect_compiler_bolt_major_detonation,
	_effect_compiler_bolt_major_contrail,
	_effect_fighter_projectile_detonation,
	_effect_fighter_melee_detonation,
	_effect_hunter_projectile_detonation,
	_effect_hunter_spark,
	_effect_minor_fusion_detonation,
	_effect_major_fusion_detonation,
	_effect_major_fusion_contrail,
	_effect_fist_detonation,
	_effect_minor_defender_detonation,
	_effect_major_defender_detonation,
	_effect_defender_spark,
	_effect_trooper_blood_splash,
	_effect_water_lamp_breaking,
	_effect_lava_lamp_breaking,
	_effect_sewage_lamp_breaking,
	_effect_alien_lamp_breaking,
	_effect_metallic_clang,
	_effect_teleport_object_in,
	_effect_teleport_object_out,
	_effect_small_water_splash,
	_effect_medium_water_splash,
	_effect_large_water_splash,
	_effect_large_water_emergence,
	_effect_small_lava_splash,
	_effect_medium_lava_splash,
	_effect_large_lava_splash,
	_effect_large_lava_emergence,
	_effect_small_sewage_splash,
	_effect_medium_sewage_splash,
	_effect_large_sewage_splash,
	_effect_large_sewage_emergence,
	_effect_small_goo_splash,
	_effect_medium_goo_splash,
	_effect_large_goo_splash,
	_effect_large_goo_emergence,
	_effect_minor_hummer_projectile_detonation,
	_effect_major_hummer_projectile_detonation,
	_effect_durandal_hummer_projectile_detonation,
	_effect_hummer_spark,
	_effect_cyborg_projectile_detonation,
	_effect_cyborg_blood_splash,
	_effect_minor_fusion_dispersal,
	_effect_major_fusion_dispersal,
	_effect_overloaded_fusion_dispersal,
	_effect_sewage_yeti_blood_splash,
	_effect_sewage_yeti_projectile_detonation,
	_effect_water_yeti_blood_splash,
	_effect_lava_yeti_blood_splash,
	_effect_lava_yeti_projectile_detonation,
	_effect_yeti_melee_detonation,
	_effect_juggernaut_spark,
	_effect_juggernaut_missile_contrail,
	_effect_small_jjaro_splash,
	_effect_medium_jjaro_splash,
	_effect_large_jjaro_splash,
	_effect_large_jjaro_emergence,
	_effect_vacuum_civilian_blood_splash,
	_effect_assimilated_vacuum_civilian_blood_splash,
	NUMBER_OF_EFFECT_TYPES
};

/*
 * items.h
 */

enum /* item types */
{
	_i_knife,
	_i_magnum,
	_i_magnum_magazine,
	_i_plasma_pistol,
	_i_plasma_magazine,
	_i_assault_rifle,
	_i_assault_rifle_magazine,
	_i_assault_grenade_magazine,
	_i_missile_launcher,
	_i_missile_launcher_magazine,
	_i_invisibility_powerup,
	_i_invincibility_powerup,
	_i_infravision_powerup,
	_i_alien_shotgun,
	_i_alien_shotgun_magazine,
	_i_flamethrower,
	_i_flamethrower_canister,
	_i_extravision_powerup,
	_i_oxygen_powerup,
	_i_energy_powerup,
	_i_double_energy_powerup,
	_i_triple_energy_powerup,
	_i_shotgun,
	_i_shotgun_magazine,
	_i_spht_door_key,
	_i_uplink_chip,

	BALL_ITEM_BASE,
	_i_light_blue_ball= BALL_ITEM_BASE,
	_i_red_ball,
	_i_violet_ball,
	_i_yellow_ball,
	_i_brown_ball,
	_i_orange_ball,
	_i_blue_ball, // heh heh
	_i_green_ball,
	
	_i_smg,
	_i_smg_ammo,
	
	NUMBER_OF_DEFINED_ITEMS
};

/*
 * monsters.h
 */
enum /* monster types */
{
	_monster_marine,
	_monster_tick_energy,
	_monster_tick_oxygen,
	_monster_tick_kamakazi,
	_monster_compiler_minor,
	_monster_compiler_major,
	_monster_compiler_minor_invisible,
	_monster_compiler_major_invisible,
	_monster_fighter_minor,
	_monster_fighter_major,
	_monster_fighter_minor_projectile,
	_monster_fighter_major_projectile,
	_civilian_crew,
	_civilian_science,
	_civilian_security,
	_civilian_assimilated,
	_monster_hummer_minor, // slow hummer
	_monster_hummer_major, // fast hummer
	_monster_hummer_big_minor, // big hummer
	_monster_hummer_big_major, // angry hummer
	_monster_hummer_possessed, // hummer from durandal
	_monster_cyborg_minor,
	_monster_cyborg_major,
	_monster_cyborg_flame_minor,
	_monster_cyborg_flame_major,
	_monster_enforcer_minor,
	_monster_enforcer_major,
	_monster_hunter_minor,
	_monster_hunter_major,
	_monster_trooper_minor,
	_monster_trooper_major,
	_monster_mother_of_all_cyborgs,
	_monster_mother_of_all_hunters,
	_monster_sewage_yeti,
	_monster_water_yeti,
	_monster_lava_yeti,
	_monster_defender_minor,
	_monster_defender_major,
	_monster_juggernaut_minor,
	_monster_juggernaut_major,
	_monster_tiny_fighter,
	_monster_tiny_bob,
	_monster_tiny_yeti,
	_vacuum_civilian_crew,
	_vacuum_civilian_science,
	_vacuum_civilian_security,
	_vacuum_civilian_assimilated,
	NUMBER_OF_MONSTER_TYPES
};

enum /* monster flags */
{
	_monster_was_promoted= 0x1,
	_monster_was_demoted= 0x2,
	_monster_has_never_been_activated= 0x4,
	_monster_is_blind= 0x8,
	_monster_is_deaf= 0x10,
	_monster_teleports_out_when_deactivated= 0x20
};

/*
 * projectiles.h
 */
enum /* projectile types */
{
	_projectile_rocket,
	_projectile_grenade,
	_projectile_pistol_bullet,
	_projectile_rifle_bullet,
	_projectile_shotgun_bullet,
	_projectile_staff,
	_projectile_staff_bolt,
	_projectile_flamethrower_burst,
	_projectile_compiler_bolt_minor,
	_projectile_compiler_bolt_major,
	_projectile_alien_weapon,
	_projectile_fusion_bolt_minor,
	_projectile_fusion_bolt_major,
	_projectile_hunter,
	_projectile_fist,
	_projectile_unused,
	_projectile_armageddon_electricity,
	_projectile_juggernaut_rocket,
	_projectile_trooper_bullet,
	_projectile_trooper_grenade,
	_projectile_minor_defender,
	_projectile_major_defender,
	_projectile_juggernaut_missile,
	_projectile_minor_energy_drain,
	_projectile_major_energy_drain,
	_projectile_oxygen_drain,
	_projectile_minor_hummer,
	_projectile_major_hummer,
	_projectile_durandal_hummer,
	_projectile_minor_cyborg_ball,
	_projectile_major_cyborg_ball,
	_projectile_ball,
	_projectile_minor_fusion_dispersal,
	_projectile_major_fusion_dispersal,
	_projectile_overloaded_fusion_dispersal,
	_projectile_yeti,
	_projectile_sewage_yeti,
	_projectile_lava_yeti,
	_projectile_smg_bullet,
	NUMBER_OF_PROJECTILE_TYPES
};

/*
 * weapons.h
 */
enum {
	_projectile_ball_dropped= 1000
};

enum { /* Weapons */
	_weapon_fist,
	_weapon_pistol,
	_weapon_plasma_pistol,
	_weapon_assault_rifle,
	_weapon_missile_launcher,
	_weapon_flamethrower,
	_weapon_alien_shotgun,
	_weapon_shotgun,
	_weapon_ball, // or something
	_weapon_smg,
	MAXIMUM_NUMBER_OF_WEAPONS,
};

enum {
	_shape_weapon_idle,
	_shape_weapon_charging,
	_shape_weapon_firing
};

enum {
	_primary_weapon,
	_secondary_weapon,
	NUMBER_OF_TRIGGERS
};

/*
 * effect_definitions.h
 */
enum /* flags */
{
	_end_when_animation_loops= 0x0001,
	_end_when_transfer_animation_loops= 0x0002,
	_sound_only= 0x0004, /* play the animationÕs initial sound and nothing else */
	_make_twin_visible= 0x0008,
	_media_effect= 0x0010
};

/* ---------- structures */

struct effect_definition
{
	short collection, shape;

	fixed sound_pitch;
	
	word flags;
	short delay, delay_sound;
};

extern effect_definition effect_definitions[NUMBER_OF_EFFECT_TYPES];

/*
 * monster_definitions.h
 */

/* ---------- macros */

#define TYPE_IS_NEUTRAL(definition,type) (!((definition->friends|definition->enemies)&monster_definitions[type].monster_class))
#define TYPE_IS_ENEMY(definition,type) (definition->enemies&monster_definitions[type].monster_class)
#define TYPE_IS_FRIEND(definition,type) (definition->friends&monster_definitions[type].monster_class)

/* ---------- constants */

enum /* monster classes */
{
	_class_player_bit,
	_class_human_civilian_bit,
	_class_madd_bit,
	_class_possessed_hummer_bit,
		
	_class_defender_bit,

	_class_fighter_bit,
	_class_trooper_bit,
	_class_hunter_bit,
	_class_enforcer_bit,
	_class_juggernaut_bit,
	_class_hummer_bit,
	
	_class_compiler_bit,
	_class_cyborg_bit,
	_class_assimilated_civilian_bit,

	_class_tick_bit,
	_class_yeti_bit,

	_class_player= 1<<_class_player_bit,
	_class_human_civilian= 1<<_class_human_civilian_bit,
	_class_madd= 1<<_class_madd_bit,
	_class_possessed_hummer= 1<<_class_possessed_hummer_bit,
	_class_human= _class_player|_class_human_civilian|_class_madd|_class_possessed_hummer,
	
	_class_defender= 1<<_class_defender_bit,

	_class_fighter= 1<<_class_fighter_bit,
	_class_trooper= 1<<_class_trooper_bit,
	_class_hunter= 1<<_class_hunter_bit,
	_class_enforcer= 1<<_class_enforcer_bit,
	_class_juggernaut= 1<<_class_juggernaut_bit,
	_class_pfhor= _class_fighter|_class_trooper|_class_hunter|_class_enforcer|_class_juggernaut,
	
	_class_compiler= 1<<_class_compiler_bit,
	_class_cyborg= 1<<_class_cyborg_bit,
	_class_assimilated_civilian= 1<<_class_assimilated_civilian_bit,
	_class_hummer= 1<<_class_hummer_bit,
	_class_client= _class_compiler|_class_assimilated_civilian|_class_cyborg|_class_hummer,
	
	_class_tick= 1<<_class_tick_bit,
	_class_yeti= 1<<_class_yeti_bit,
	_class_native= _class_tick|_class_yeti,

	_class_hostile_alien= _class_pfhor|_class_client,
	_class_neutral_alien= _class_native
};

enum /* intelligence: maximum polygon switches before losing lock */
{
	_intelligence_low= 2,
	_intelligence_average= 3,
	_intelligence_high= 8
};

enum /* door retry masks */
{
	_slow_door_retry_mask= 63,
	_normal_door_retry_mask= 31,
	_fast_door_retry_mask= 15,
	_vidmaster_door_retry_mask= 3
};

enum /* flags */
{
	_monster_is_omniscent= 0x1, /* ignores line-of-sight during find_closest_appropriate_target() */
	_monster_flys= 0x2,
	_monster_is_alien= 0x4, /* moves slower on slower levels, etc. */
	_monster_major= 0x8, /* type -1 is minor */
	_monster_minor= 0x10, /* type +1 is major */
	_monster_cannot_be_dropped= 0x20, /* low levels cannot skip this monster */
	_monster_floats= 0x40, /* exclusive from flys; forces the monster to take +¶h gradually */
	_monster_cannot_attack= 0x80, /* monster has no weapons and cannot attack (runs constantly to safety) */
	_monster_uses_sniper_ledges= 0x100, /* sit on ledges and hurl shit at the player (ranged attack monsters only) */
	_monster_is_invisible= 0x200, /* this monster uses _xfer_invisibility */
	_monster_is_subtly_invisible= 0x400, /* this monster uses _xfer_subtle_invisibility */
	_monster_is_kamakazi= 0x800, /* monster does shrapnel damage and will suicide if close enough to target */
	_monster_is_berserker= 0x1000, /* below 1/4 vitality this monster goes berserk */
	_monster_is_enlarged= 0x2000, /* monster is 1.25 times normal height */
	_monster_has_delayed_hard_death= 0x4000, /* always dies soft, then switches to hard */
	_monster_fires_symmetrically= 0x8000, /* fires at ±dy, simultaneously */
	_monster_has_nuclear_hard_death= 0x10000, /* playerÕs screen whites out and slowly recovers */
	_monster_cant_fire_backwards= 0x20000, /* monster canÕt turn more than 135¡ to fire */
	_monster_can_die_in_flames= 0x40000, /* uses humanoid flaming body shape */
	_monster_waits_with_clear_shot= 0x80000, /* will sit and fire (slowly) if we have a clear shot */
	_monster_is_tiny= 0x100000, /* 0.25-size normal height */
	_monster_attacks_immediately= 0x200000, /* monster will try an attack immediately */
	_monster_is_not_afraid_of_water= 0x400000,
	_monster_is_not_afraid_of_sewage= 0x800000,
	_monster_is_not_afraid_of_lava= 0x1000000,
	_monster_is_not_afraid_of_goo= 0x2000000,
	_monster_can_teleport_under_media= 0x4000000,
	_monster_chooses_weapons_randomly= 0x8000000
	/* monsters unable to open doors have door retry masks of NONE */
	/* monsters unable to switch levels have min,max ledge deltas of 0 */
	/* monsters unstopped by bullets have hit frames of NONE */
};

enum /* monster speeds (world_distance per tick); also used for projectiles */
{
	_speed_slow= WORLD_ONE/120,
	_speed_medium= WORLD_ONE/80,
	_speed_almost_fast= WORLD_ONE/70,
	_speed_fast= WORLD_ONE/40,
	_speed_superfast1= WORLD_ONE/30,
	_speed_superfast2= WORLD_ONE/28,
	_speed_superfast3= WORLD_ONE/26,
	_speed_superfast4= WORLD_ONE/24,
	_speed_superfast5= WORLD_ONE/22,
	_speed_blinding= WORLD_ONE/20,
	_speed_insane= WORLD_ONE/10
};

#define NORMAL_MONSTER_GRAVITY (WORLD_ONE/120)
#define NORMAL_MONSTER_TERMINAL_VELOCITY (WORLD_ONE/14)

/* ---------- monster definition structures */

struct attack_definition
{
	short type;
	short repetitions;
	angle error; /* ±error is added to the firing angle */
	world_distance range; /* beyond which we cannot attack */
	short attack_shape; /* attack occurs when keyframe is displayed */
	
	world_distance dx, dy, dz; /* +dy is right, +dx is out, +dz is up */
};

struct monster_definition /* <128 bytes */
{
	short collection;
	
	short vitality;
	unsigned long immunities, weaknesses;
	unsigned long flags;

	long monster_class; /* our class */
	long friends, enemies; /* bit fields of what classes we consider friendly and what types we donÕt like */

	fixed sound_pitch;
	short activation_sound, friendly_activation_sound, clear_sound;
	short kill_sound, apology_sound, friendly_fire_sound;
	short flaming_sound; /* the scream we play when we go down in flames */
	short random_sound, random_sound_mask; /* if moving and locked play this sound if we get time and our mask comes up */

	short carrying_item_type; /* an item type we might drop if we donÕt explode */

	world_distance radius, height;
	world_distance preferred_hover_height;
	world_distance minimum_ledge_delta, maximum_ledge_delta;
	fixed external_velocity_scale;
	short impact_effect, melee_impact_effect, contrail_effect;

	short half_visual_arc, half_vertical_visual_arc;
	world_distance visual_range, dark_visual_range;
	short intelligence;
	short speed, gravity, terminal_velocity;
	short door_retry_mask;
	short shrapnel_radius; /* no shrapnel if NONE */
	struct damage_definition shrapnel_damage;

	shape_descriptor hit_shapes;
	shape_descriptor hard_dying_shape, soft_dying_shape; /* minus dead frame */
	shape_descriptor hard_dead_shapes, soft_dead_shapes; /* NONE for vanishing */
	shape_descriptor stationary_shape, moving_shape;
	shape_descriptor teleport_in_shape, teleport_out_shape;
	
	/* which type of attack the monster actually uses is determined at attack time; typically
		melee attacks will occur twice as often as ranged attacks because the monster will be
		stopped (and stationary monsters attack twice as often as moving ones) */
	short attack_frequency;
	struct attack_definition melee_attack;
	struct attack_definition ranged_attack;
};

extern monster_definition monster_definitions[NUMBER_OF_MONSTER_TYPES];

/*
 * physics_models.h
 */

enum /* models */
{
	_model_game_walking,
	_model_game_running,
	NUMBER_OF_PHYSICS_MODELS
};

/* ---------- structures */

struct physics_constants
{
	fixed maximum_forward_velocity, maximum_backward_velocity, maximum_perpendicular_velocity;
	fixed acceleration, deceleration, airborne_deceleration; /* forward, backward and perpendicular */
	fixed gravitational_acceleration, climbing_acceleration, terminal_velocity;
	fixed external_deceleration;
	
	fixed angular_acceleration, angular_deceleration, maximum_angular_velocity, angular_recentering_velocity;
	fixed fast_angular_velocity, fast_angular_maximum; /* for head movements */
	fixed maximum_elevation; /* positive and negative */
	fixed external_angular_deceleration;
	
	/* step_length is distance between adjacent nodes in the actorÕs phase */
	fixed step_delta, step_amplitude;
	fixed radius, height, dead_height, camera_height, splash_height;
	
	fixed half_camera_separation;
};

extern physics_constants physics_models[NUMBER_OF_PHYSICS_MODELS];

/*
 * projectile_definitions.h
 */

enum /* projectile flags */
{
	_guided= 0x0001,
	_stop_when_animation_loops= 0x0002,
	_persistent= 0x0004, /* does stops doing damage and stops moving against a target, but doesn't vanish */
	_alien_projectile= 0x0008, /* does less damage and moves slower on lower levels */
	_affected_by_gravity= 0x0010,
	_no_horizontal_error= 0x0020,
	_no_vertical_error= 0x0040,
	_can_toggle_control_panels= 0x0080,
	_positive_vertical_error= 0x0100,
	_melee_projectile= 0x0200, /* can use a monsterÕs custom melee detonation */
	_persistent_and_virulent= 0x0400, /* keeps moving and doing damage after a successful hit */
	_usually_pass_transparent_side= 0x0800,
	_sometimes_pass_transparent_side= 0x1000,
	_doubly_affected_by_gravity= 0x2000,
	_rebounds_from_floor= 0x4000, /* unless v.z<kvzMIN */
	_penetrates_media= 0x8000, /* huh uh huh ... i said penetrate */
	_becomes_item_on_detonation= 0x10000, /* item type in .permutation field of projectile */
	_bleeding_projectile= 0x20000, /* can use a monsterÕs custom bleeding detonation */
	_horizontal_wander= 0x40000, /* random horizontal error perpendicular to direction of movement */
	_vertical_wander= 0x80000, /* random vertical movement perpendicular to direction of movement */
	_affected_by_half_gravity= 0x100000,
	_projectile_passes_media_boundary= 0x200000
};

/* ---------- structures */

struct projectile_definition
{
	short collection, shape; /* collection can be NONE (invisible) */
	short detonation_effect, media_detonation_effect;
	short contrail_effect, ticks_between_contrails, maximum_contrails; /* maximum of NONE is infinite */
	short media_projectile_promotion;

	world_distance radius; /* can be zero and will still hit */
	world_distance area_of_effect; /* one target if ==0 */
	struct damage_definition damage;

	unsigned long flags;

	world_distance speed;
	world_distance maximum_range;

	fixed sound_pitch;	
	short flyby_sound, rebound_sound;
};

extern projectile_definition projectile_definitions[NUMBER_OF_PROJECTILE_TYPES];

/*
 * weapon_definitions.h
 */
/* ---------- constants */
#define NORMAL_WEAPON_DZ (20)

enum /* weapon classes */
{
	_melee_class, /* normal weapon, no ammunition, both triggers do the same thing */
	_normal_class, /* normal weapon, one ammunition type, both triggers do the same thing */
	_dual_function_class, /* normal weapon, one ammunition type, trigger does something different */
	_twofisted_pistol_class, /* two can be held at once (differnet triggers), same ammunition */
	_multipurpose_class /* two weapons in one (assault rifle, grenade launcher), two different
		ammunition types with two separate triggers; secondary ammunition is discrete (i.e., it
		is never loaded explicitly but appears in the weapon) */
};

enum /* weapon flags */
{
	_no_flags= 0x0,
	_weapon_is_automatic= 0x01,
	_weapon_disappears_after_use= 0x02,
	_weapon_plays_instant_shell_casing_sound= 0x04,
	_weapon_overloads= 0x08,
	_weapon_has_random_ammo_on_pickup= 0x10,
	_powerup_is_temporary= 0x20,
	_weapon_reloads_in_one_hand= 0x40,
	_weapon_fires_out_of_phase= 0x80,
	_weapon_fires_under_media= 0x100,
	_weapon_triggers_share_ammo= 0x200,
	_weapon_secondary_has_angular_flipping= 0x400
};

enum {
	_weapon_in_hand_collection= 1,
	_fist_idle= 0,
	_fist_punching,
	_pistol_idle,
	_pistol_firing,
	_pistol_reloading,
	_shotgun_idle,
	_shotgun_firing,
	_shotgun_reloading,
	_assault_rifle_idle,
	_assault_rifle_firing,
	_assault_rifle_reloading,
	_fusion_idle,
	_fusion_firing,
	_missile_launcher_idle,
	_missile_launcher_firing,
	_flamethrower_idle,
	_flamethrower_transit,
	_flamethrower_firing,
	_pistol_shell_casing,
	_assault_rifle_shell_casing,
	_fusion_charged,
	_alien_weapon_idle,
	_alien_weapon_firing,
	_smg_idle,
	_smg_firing,
	_smg_reloading,
	_smg_shell_casing
};

/* ---------- shell casings */

enum // shell casing types
{
	_shell_casing_assault_rifle,
	_shell_casing_pistol,
	_shell_casing_pistol_left,
	_shell_casing_pistol_right,
	_shell_casing_smg,
	
	NUMBER_OF_SHELL_CASING_TYPES
};

/* ---------- structures */

struct trigger_definition {
	short rounds_per_magazine;
	short ammunition_type;
	short ticks_per_round;
	short recovery_ticks;
	short charging_ticks;
	world_distance recoil_magnitude;
	short firing_sound;
	short click_sound;
	short charging_sound;
	short shell_casing_sound;
	short reloading_sound;
	short charged_sound;
	short projectile_type;
	short theta_error;
	short dx, dz;
	short shell_casing_type;
	short burst_count;
};

struct weapon_definition {
	short item_type;
	short powerup_type;
	short weapon_class;
	short flags;

	fixed firing_light_intensity;
	short firing_intensity_decay_ticks;

	/* weapon will come up to FIXED_ONE when fired; idle_height±bob_amplitude should be in
		the range [0,FIXED_ONE] */
	fixed idle_height, bob_amplitude, kick_height, reload_height;
	fixed idle_width, horizontal_amplitude;

	/* each weapon has three basic animations: idle, firing and reloading.  sounds and frames
		are pulled from the shape collection.  for automatic weapons the firing animation loops
		until the trigger is released or the gun is empty and the gun begins rising as soon as
		the trigger is depressed and is not lowered until the firing animation stops.  for single
		shot weapons the animation loops once; the weapon is raised and lowered as soon as the
		firing animation terminates */
	short collection;
	short idle_shape, firing_shape, reloading_shape;
	short unused;
	short charging_shape, charged_shape;

	/* How long does it take to ready the weapon? */
	/* load_rounds_tick is the point which you actually load them. */
	short ready_ticks, await_reload_ticks, loading_ticks, finish_loading_ticks, powerup_ticks;

	struct trigger_definition weapons_by_trigger[NUMBER_OF_TRIGGERS];
};

extern weapon_definition weapon_definitions[10];

#endif

#include "DefaultPhysics.h"

/*
 * effect_definitions.h
 */
effect_definition effect_definitions[NUMBER_OF_EFFECT_TYPES]=
	{
		/* rocket explosion, contrail */
		{_collection_rocket, 1, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{_collection_rocket, 2, _normal_frequency, _end_when_animation_loops, 0, NONE},

		/* grenade explosion, contrail */
		{_collection_rocket, 9, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{_collection_rocket, 4, _normal_frequency, _end_when_animation_loops, 0, NONE},

		/* bullet ricochet */
		{_collection_rocket, 13, _normal_frequency, _end_when_animation_loops, 0, NONE},

		/* _effect_alien_weapon_ricochet */
		{_collection_rocket, 5, _normal_frequency, _end_when_animation_loops, 0, NONE},

		/* flame thrower burst */
		{_collection_rocket, 6, _normal_frequency, _end_when_animation_loops, 0, NONE},
	
		/* fighter blood splash */
		{_collection_fighter, 8, _normal_frequency, _end_when_animation_loops, 0, NONE},
	
		/* player blood splash */
		{_collection_rocket, 10, _normal_frequency, _end_when_animation_loops, 0, NONE},
	
		/* civilian blood splash, assimilated civilian blood splash */
		{_collection_civilian, 7, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{BUILD_COLLECTION(_collection_civilian, 3), 12, _normal_frequency, _end_when_animation_loops, 0, NONE},
	
		/* enforcer blood splash */
		{_collection_enforcer, 5, _normal_frequency, _end_when_animation_loops, 0, NONE},
	
		/* _effect_compiler_bolt_minor_detonation, _effect_compiler_bolt_major_detonation,
		   _effect_compiler_bolt_major_contrail */
		{_collection_compiler, 6, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{BUILD_COLLECTION(_collection_compiler, 1), 6, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{BUILD_COLLECTION(_collection_compiler, 1), 5, _normal_frequency, _end_when_animation_loops, 0, NONE},
	
		/* _effect_fighter_projectile_detonation, _effect_fighter_melee_detonation */
		{BUILD_COLLECTION(_collection_fighter, 0), 10, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{BUILD_COLLECTION(_collection_fighter, 0), 11, _normal_frequency, _sound_only, 0, NONE},
	
		/* _effect_hunter_projectile_detonation, _effect_hunter_spark */
		{BUILD_COLLECTION(_collection_hunter, 0), 4, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{BUILD_COLLECTION(_collection_hunter, 0), 8, _normal_frequency, _end_when_animation_loops, 0, NONE},
	
		/* _effect_minor_fusion_detonation, _effect_major_fusion_detonation, _effect_major_fusion_contrail */
		{BUILD_COLLECTION(_collection_rocket, 0), 14, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{BUILD_COLLECTION(_collection_rocket, 0), 15, _higher_frequency, _end_when_animation_loops, 0, NONE},
		{BUILD_COLLECTION(_collection_rocket, 0), 16, _higher_frequency, _end_when_animation_loops, 0, NONE},
	
		/* _effect_fist_detonation */
		{_collection_rocket, 17, _normal_frequency, _sound_only, 0, NONE},

		/* _effect_minor_defender_detonation, _effect_major_defender_detonation, _effect_defender_spark */
		{BUILD_COLLECTION(_collection_defender, 0), 5, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{BUILD_COLLECTION(_collection_defender, 1), 5, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{BUILD_COLLECTION(_collection_defender, 0), 7, _normal_frequency, _end_when_animation_loops, 0, NONE},
	
		/* _effect_trooper_blood_splash */
		{_collection_trooper, 8, _normal_frequency, _end_when_animation_loops, 0, NONE},
	
		/* _effect_lamp_breaking */
		{_collection_scenery1, 22, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{_collection_scenery2, 18, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{_collection_scenery3, 16, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{_collection_scenery5, 8, _normal_frequency, _end_when_animation_loops, 0, NONE},
	
		/* _effect_metallic_clang */
		{_collection_rocket, 23, _normal_frequency, _sound_only, 0, NONE},
	
		/* _effect_teleport_in, _effect_teleport_out */
		{_collection_items, 0, _normal_frequency, _end_when_transfer_animation_loops|_make_twin_visible, TICKS_PER_SECOND, _snd_teleport_in},
		{_collection_items, 0, _normal_frequency, _end_when_transfer_animation_loops, 0, NONE},
	
		/* _effect_small_water_splash, _effect_medium_water_splash, _effect_large_water_splash, _effect_large_water_emergence */
		{_collection_scenery1, 0, _normal_frequency, _end_when_animation_loops|_media_effect, 0, NONE},
		{_collection_scenery1, 1, _normal_frequency, _end_when_animation_loops|_media_effect, 0, NONE},
		{_collection_scenery1, 2, _normal_frequency, _end_when_animation_loops|_sound_only, NONE},
		{_collection_scenery1, 3, _normal_frequency, _end_when_animation_loops|_sound_only, NONE},
	
		/* _effect_small_lava_splash, _effect_medium_lava_splash, _effect_large_lava_splash, _effect_large_lava_emergence */
		{_collection_scenery2, 0, _normal_frequency, _end_when_animation_loops|_media_effect, 0, NONE},
		{_collection_scenery2, 1, _normal_frequency, _end_when_animation_loops|_media_effect, 0, NONE},
		{_collection_scenery2, 2, _normal_frequency, _end_when_animation_loops|_sound_only, 0, NONE},
		{_collection_scenery2, 17, _normal_frequency, _end_when_animation_loops|_sound_only, 0, NONE},

		/* _effect_small_sewage_splash, _effect_medium_sewage_splash, _effect_large_sewage_splash, _effect_large_sewage_emergence */
		{_collection_scenery3, 0, _normal_frequency, _end_when_animation_loops|_media_effect, 0, NONE},
		{_collection_scenery3, 1, _normal_frequency, _end_when_animation_loops|_media_effect, 0, NONE},
		{_collection_scenery3, 2, _normal_frequency, _end_when_animation_loops|_sound_only, 0, NONE},
		{_collection_scenery3, 3, _normal_frequency, _end_when_animation_loops|_sound_only, 0, NONE},

		/* _effect_small_goo_splash, _effect_medium_goo_splash, _effect_large_goo_splash, _effect_large_goo_emergence */
		{_collection_scenery5, 0, _normal_frequency, _end_when_animation_loops|_media_effect, 0, NONE},
		{_collection_scenery5, 1, _normal_frequency, _end_when_animation_loops|_media_effect, 0, NONE},
		{_collection_scenery5, 2, _normal_frequency, _end_when_animation_loops|_sound_only, 0, NONE},
		{_collection_scenery5, 3, _normal_frequency, _end_when_animation_loops|_sound_only, 0, NONE},
	
		/* _effect_minor_hummer_projectile_detonation, _effect_major_hummer_projectile_detonation,
		   _effect_durandal_hummer_projectile_detonation, _effect_hummer_spark */
		{BUILD_COLLECTION(_collection_hummer, 0), 6, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{BUILD_COLLECTION(_collection_hummer, 1), 6, _higher_frequency, _end_when_animation_loops, 0, NONE},
		{BUILD_COLLECTION(_collection_hummer, 4), 6, _lower_frequency, _end_when_animation_loops, 0, NONE},
		{BUILD_COLLECTION(_collection_hummer, 0), 7, _normal_frequency, _end_when_animation_loops, 0, NONE},
	
		/* _effect_cyborg_projectile_detonation, _effect_cyborg_blood_splash */
		{_collection_cyborg, 7, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{_collection_cyborg, 8, _normal_frequency, _end_when_animation_loops, 0, NONE},

		/* 	_effect_minor_fusion_dispersal, _effect_major_fusion_dispersal, _effect_overloaded_fusion_dispersal */
		{BUILD_COLLECTION(_collection_rocket, 0), 19, _normal_frequency, _end_when_animation_loops|_sound_only, 0, NONE},
		{BUILD_COLLECTION(_collection_rocket, 0), 20, _higher_frequency, _end_when_animation_loops|_sound_only, 0, NONE},
		{BUILD_COLLECTION(_collection_rocket, 0), 21, _lower_frequency, _end_when_animation_loops|_sound_only, 0, NONE},

		/* _effect_sewage_yeti_blood_splash, _effect_sewage_yeti_projectile_detonation */
		{BUILD_COLLECTION(_collection_yeti, 0), 5, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{BUILD_COLLECTION(_collection_yeti, 0), 11, _normal_frequency, _end_when_animation_loops, 0, NONE},
	
		/* _effect_water_yeti_blood_splash */
		{BUILD_COLLECTION(_collection_yeti, 1), 5, _normal_frequency, _end_when_animation_loops, 0, NONE},
	
		/* _effect_lava_yeti_blood_splash, _effect_lava_yeti_projectile_detonation */
		{BUILD_COLLECTION(_collection_yeti, 2), 5, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{BUILD_COLLECTION(_collection_yeti, 2), 7, _normal_frequency, _end_when_animation_loops, 0, NONE},

		/* _effect_yeti_melee_detonation */
		{_collection_yeti, 8, _normal_frequency, _sound_only, 0, NONE},
	
		/* _effect_juggernaut_spark, _effect_juggernaut_missile_contrail */
		{_collection_juggernaut, 3, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{_collection_rocket, 24, _normal_frequency, _end_when_animation_loops, 0, NONE},

		/* _effect_small_jjaro_splash, _effect_medium_jjaro_splash, _effect_large_jjaro_splash, _effect_large_jjaro_emergence */
		{_collection_scenery4, 0, _normal_frequency, _end_when_animation_loops|_media_effect, 0, NONE},
		{_collection_scenery4, 1, _normal_frequency, _end_when_animation_loops|_media_effect, 0, NONE},
		{_collection_scenery4, 2, _normal_frequency, _end_when_animation_loops|_sound_only, 0, NONE},
		{_collection_scenery4, 3, _normal_frequency, _end_when_animation_loops|_sound_only, 0, NONE},

		/* vacuum civilian blood splash, vacuum assimilated civilian blood splash */
		{_collection_vacuum_civilian, 7, _normal_frequency, _end_when_animation_loops, 0, NONE},
		{BUILD_COLLECTION(_collection_vacuum_civilian, 3), 12, _normal_frequency, _end_when_animation_loops, 0, NONE},
	};

/*
 * monster_definitions.h
 */
monster_definition monster_definitions[NUMBER_OF_MONSTER_TYPES]=
	{
		{ /* _monster_marine (canÕt be used as a regular monster) */
		  _collection_player, /* shape collection */
		  20, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_cannot_be_dropped|_monster_can_die_in_flames, /* flags */

		  _class_player,	
		  _class_human, /* friends */
		  -1, /* enemies */

		  _normal_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming death sound */
		  NONE, 0, /* random sound, random sound mask */
		
		  NONE, /* carrying item type */

		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  0, 0,
		  (3*FIXED_ONE)/4, /* external velocity scale */
		  _effect_player_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */

		  0, 0, /* half visual arc, half vertical visual arc */
		  0, 0, /* visual range, dark visual range */
		  _intelligence_low, /* intelligence */
		  _speed_almost_fast, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _normal_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  0, /* being hit */
		  0, 0, /* dying hard (popping), dying soft (falling) */
		  0, 0, /* hard dead frames, soft dead frames */
		  0, 0, /* stationary shape, moving shape (no permutations) */
		  0, 0, /* teleport in shape, teleport out shape */
		},
	
		{ /* _monster_tick_minor */
		  BUILD_COLLECTION(_collection_tick, 0), /* shape collection */
		  0, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_minor|_monster_flys|_monster_has_delayed_hard_death|_monster_cannot_attack, /* flags */
		
		  _class_tick, /* class */
		  -1, /* friends */
		  0, /* enemies */
		
		  _normal_frequency, /* sound pitch */
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming death sound */
		  _snd_tick_chatter, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE_HALF, WORLD_ONE_HALF, /* radius, height */
		  0, /* preferred hover height */
		  SHORT_MIN, SHORT_MAX, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE, /* external velocity scale */
		  NONE, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 15*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_low, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  NONE, /* door retry mask */
		  NONE, {NONE, _alien_damage, 30, 10, FIXED_ONE}, /* shrapnel radius, shrapnel damage */
		
		  0, /* being hit */
		  6, 3, /* dying hard (popping), dying soft (falling) */
		  5, 5, /* hard dead frames, soft dead frames */
		  1, 1, /* stationary shape, moving shape (no permutations) */
		  NONE, NONE, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  NONE, /* ranged attack type */
		  }
		},

		{ /* _monster_tick_major */
		  BUILD_COLLECTION(_collection_tick, 0), /* shape collection */
		  0, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_major|_monster_flys|_monster_has_delayed_hard_death|_monster_cannot_attack, /* flags */
		
		  _class_tick, /* class */
		  -1, /* friends */
		  0, /* enemies */
		
		  _higher_frequency, /* sound pitch */
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming death sound */
		  _snd_tick_chatter, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/5, WORLD_ONE_FOURTH, /* radius, height */
		  0, /* preferred hover height */
		  SHORT_MIN, SHORT_MAX, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE, /* external velocity scale */
		  NONE, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 15*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_low, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  NONE, /* door retry mask */
		  2*WORLD_ONE, {_damage_explosion, _alien_damage, 40, 20, FIXED_ONE}, /* shrapnel radius, shrapnel damage */
		
		  0, /* being hit */
		  4, 3, /* dying hard (popping), dying soft (falling) */
		  5, 5, /* hard dead frames, soft dead frames */
		  1, 1, /* stationary shape, moving shape (no permutations) */
		  NONE, NONE, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  NONE, /* ranged attack type */
		  }
		},

		{ /* _monster_tick_kamakazi */
		  BUILD_COLLECTION(_collection_tick, 0), /* shape collection */
		  0, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_flys|_monster_is_kamakazi|_monster_has_delayed_hard_death, /* flags */
		
		  _class_tick, /* class */
		  0, /* friends */
		  -1, /* enemies */
		
		  _normal_frequency, /* sound pitch */
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming death sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/5, WORLD_ONE_FOURTH, /* radius, height */
		  0, /* preferred hover height */
		  -5*WORLD_ONE, 5*WORLD_ONE, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE, /* external velocity scale */
		  NONE, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 15*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_low, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  NONE, /* door retry mask */
		  WORLD_ONE, {_damage_explosion, _alien_damage, 30, 10, FIXED_ONE}, /* shrapnel radius, shrapnel damage */
		
		  0, /* being hit */
		  6, 4, /* dying hard (popping), dying soft (falling) */
		  5, 5, /* hard dead frames, soft dead frames */
		  0, 1, /* stationary shape, moving shape (no permutations) */
		  NONE, NONE, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  _projectile_minor_energy_drain, /* melee attack type */
			  5000, /* repetitions */
			  0, /* error */
			  WORLD_ONE, /* range */
			
			  2, /* melee attack shape */

			  0, 0, WORLD_ONE_HALF, /* dx, dy, dz */
		  },
		
		  /* ranged attack */
		  {
			  NONE, /* ranged attack type */
		  }
		},

		{ /* _monster_compiler_minor */
		  BUILD_COLLECTION(_collection_compiler, 0), /* shape collection */
		  160, FLAG(_damage_flame)|FLAG(_damage_lava), FLAG(_damage_fusion_bolt), /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_minor|_monster_floats|_monster_can_teleport_under_media, /* flags */

		  _class_compiler, /* class */
		  _class_compiler, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming dying sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/5, WORLD_ONE, /* radius, height */
		  0, /* preferred hover height */
		  SHORT_MIN, SHORT_MAX, /* minimum ledge delta, maximum ledge delta */
		  0, /* external velocity scale */
		  NONE, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 3*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast2, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  7, /* being hit */
		  NONE, 2, /* dying hard (popping), dying soft (falling) */
		  NONE, NONE, /* hard dead frames, soft dead frames */
		  0, 3, /* stationary shape, moving shape */
		  0, 0, /* teleport in shape, teleport out shape */
		
		  3*TICKS_PER_SECOND, /* attack frequency */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_compiler_bolt_minor, /* ranged attack type */
			  0, /* repetitions */
			  NUMBER_OF_ANGLES/200, /* error angle */
			  20*WORLD_ONE, /* range */
			  1, /* ranged attack shape */
			
			  WORLD_ONE/16, 0, WORLD_ONE_HALF+WORLD_ONE_FOURTH/2, /* dx, dy, dz */
		  }
		},

		{ /* _monster_compiler_major */
		  BUILD_COLLECTION(_collection_compiler, 1), /* shape collection */
		  200, FLAG(_damage_flame)|FLAG(_damage_lava), FLAG(_damage_fusion_bolt), /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_major|_monster_floats|_monster_can_teleport_under_media, /* flags */
	
		  _class_compiler, /* class */
		  _class_compiler, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _higher_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming dying sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/5, WORLD_ONE, /* radius, height */
		  0, /* preferred hover height */
		  SHORT_MIN, SHORT_MAX, /* minimum ledge delta, maximum ledge delta */
		  0, /* external velocity scale */
		  NONE, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 3*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast3, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  7, /* being hit */
		  NONE, 2, /* dying hard (popping), dying soft (falling) */
		  NONE, NONE, /* hard dead frames, soft dead frames */
		  0, 3, /* stationary shape, moving shape */
		  0, 0, /* teleport in shape, teleport out shape */
		
		  4*TICKS_PER_SECOND, /* attack frequency */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_compiler_bolt_major, /* ranged attack type */
			  0, /* repetitions */
			  0, /* error angle */
			  20*WORLD_ONE, /* range */
			  1, /* ranged attack shape */
			
			  WORLD_ONE/16, 0, WORLD_ONE_HALF+WORLD_ONE_FOURTH/2, /* dx, dy, dz */
		  }
		},
	
		{ /* _monster_compiler_minor_invisible */
		  BUILD_COLLECTION(_collection_compiler, 0), /* shape collection */
		  160, FLAG(_damage_flame)|FLAG(_damage_lava), FLAG(_damage_fusion_bolt), /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_minor|_monster_floats|_monster_is_invisible|_monster_can_teleport_under_media, /* flags */
	
		  _class_compiler, /* class */
		  _class_compiler, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming dying sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/5, WORLD_ONE, /* radius, height */
		  0, /* preferred hover height */
		  SHORT_MIN, SHORT_MAX, /* minimum ledge delta, maximum ledge delta */
		  0, /* external velocity scale */
		  NONE, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 3*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast4, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  7, /* being hit */
		  NONE, 2, /* dying hard (popping), dying soft (falling) */
		  NONE, NONE, /* hard dead frames, soft dead frames */
		  0, 3, /* stationary shape, moving shape */
		  0, 0, /* teleport in shape, teleport out shape */
		
		  3*TICKS_PER_SECOND, /* attack frequency */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_compiler_bolt_minor, /* ranged attack type */
			  0, /* repetitions */
			  NUMBER_OF_ANGLES/200, /* error angle */
			  20*WORLD_ONE, /* range */
			  1, /* ranged attack shape */
			
			  WORLD_ONE/16, 0, WORLD_ONE_HALF+WORLD_ONE_FOURTH/2, /* dx, dy, dz */
		  }
		},
	
		{ /* _monster_compiler_major_invisible */
		  BUILD_COLLECTION(_collection_compiler, 1), /* shape collection */
		  200, FLAG(_damage_flame)|FLAG(_damage_lava), FLAG(_damage_fusion_bolt), /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_major|_monster_floats|_monster_is_subtly_invisible|_monster_can_teleport_under_media, /* flags */
	
		  _class_compiler, /* class */
		  _class_compiler, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _higher_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming dying sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/5, WORLD_ONE, /* radius, height */
		  0, /* preferred hover height */
		  SHORT_MIN, SHORT_MAX, /* minimum ledge delta, maximum ledge delta */
		  0, /* external velocity scale */
		  NONE, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 3*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast5, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  7, /* being hit */
		  NONE, 2, /* dying hard (popping), dying soft (falling) */
		  NONE, NONE, /* hard dead frames, soft dead frames */
		  0, 3, /* stationary shape, moving shape */
		  0, 0, /* teleport in shape, teleport out shape */
		
		  4*TICKS_PER_SECOND, /* attack frequency */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_compiler_bolt_major, /* ranged attack type */
			  0, /* repetitions */
			  0, /* error angle */
			  20*WORLD_ONE, /* range */
			  1, /* ranged attack shape */
			
			  WORLD_ONE/16, 0, WORLD_ONE_HALF+WORLD_ONE_FOURTH/2, /* dx, dy, dz */
		  }
		},
	
		{ /* _monster_fighter (minor) */
		  BUILD_COLLECTION(_collection_fighter, 0), /* shape collection */
		  40, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_minor|_monster_can_die_in_flames, /* flags */
		
		  _class_fighter, /* class */
		  _class_pfhor, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  _snd_fighter_activate, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  _snd_fighter_wail, /* dying flaming */
		  _snd_fighter_chatter, 15, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -4*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  (3*FIXED_ONE)/4, /* external velocity scale */
		  _effect_fighter_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 2*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast1, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _normal_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  4, /* being hit */
		  1, 3, /* dying hard (popping), dying soft (falling) */
		  6, 5, /* hard dead frames, soft dead frames */
		  7, 0, /* stationary shape, moving shape */
		  12, 12, /* teleport in shape, teleport out shape */
		
		  4*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  _projectile_staff, /* melee attack type */
			  0, /* repetitions */
			  0, /* error */
			  WORLD_ONE, /* range */
			
			  2, /* melee attack shape */

			  0, 0, WORLD_ONE_HALF+WORLD_ONE_FOURTH/2, /* dx, dy, dz */
		  },
		
		  /* ranged attack */
		  {
			  NONE, /* ranged attack type */
			  0, /* repetitions */
			  0, /* error angle */
			  12*WORLD_ONE, /* range */
			  3, /* ranged attack shape */
			
			  0, 0, WORLD_ONE_HALF+WORLD_ONE_FOURTH/2, /* dx, dy, dz */
		  }
		},

		{ /* _monster_fighter (major) */
		  BUILD_COLLECTION(_collection_fighter, 1), /* shape collection */
		  80, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_major|_monster_is_berserker|_monster_can_die_in_flames, /* flags */
	
		  _class_fighter, /* class */
		  _class_pfhor, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _lower_frequency, /* sound pitch */	
		  _snd_fighter_activate, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  _snd_fighter_wail, /* dying flaming */
		  _snd_fighter_chatter, 15, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -4*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  (3*FIXED_ONE)/4, /* external velocity scale */
		  _effect_fighter_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 3*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast2, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _normal_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  4, /* being hit */
		  1, 3, /* dying hard (popping), dying soft (falling) */
		  6, 5, /* hard dead frames, soft dead frames */
		  7, 0, /* stationary shape, moving shape */
		  12, 12, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  _projectile_staff, /* melee attack type */
			  0, /* repetitions */
			  0, /* error */
			  WORLD_ONE, /* range */
			
			  2, /* melee attack shape */

			  0, 0, WORLD_ONE_HALF+WORLD_ONE_FOURTH/2, /* dx, dy, dz */
		  },
		
		  /* ranged attack */
		  {
			  NONE, /* ranged attack type */
			  0, /* repetitions */
			  0, /* error angle */
			  12*WORLD_ONE, /* range */
			  3, /* ranged attack shape */
			
			  0, 0, WORLD_ONE_HALF+WORLD_ONE_FOURTH/2, /* dx, dy, dz */
		  }
		},

		{ /* _monster_fighter (minor projectile) */
		  BUILD_COLLECTION(_collection_fighter, 2), /* shape collection */
		  80, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_minor|_monster_uses_sniper_ledges|_monster_can_die_in_flames, /* flags */
	
		  _class_fighter, /* class */
		  _class_pfhor, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  _snd_fighter_activate, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  _snd_fighter_wail, /* dying flaming */
		  _snd_fighter_chatter, 15, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -4*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  (3*FIXED_ONE)/4, /* external velocity scale */
		  _effect_fighter_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 3*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast3, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _normal_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  4, /* being hit */
		  1, 3, /* dying hard (popping), dying soft (falling) */
		  6, 5, /* hard dead frames, soft dead frames */
		  7, 0, /* stationary shape, moving shape */
		  12, 12, /* teleport in shape, teleport out shape */
		
		  4*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  _projectile_staff, /* melee attack type */
			  0, /* repetitions */
			  0, /* error */
			  WORLD_ONE, /* range */
			
			  2, /* melee attack shape */

			  WORLD_ONE/16, 0, WORLD_ONE_FOURTH, /* dx, dy, dz */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_staff_bolt, /* ranged attack type */
			  0, /* repetitions */
			  NUMBER_OF_ANGLES/150, /* error angle */
			  12*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  WORLD_ONE/16, 0, WORLD_ONE_FOURTH, /* dx, dy, dz */
		  }
		},

		{ /* _monster_fighter (major projectile) */
		  BUILD_COLLECTION(_collection_fighter, 3), /* shape collection */
		  80, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_major|_monster_uses_sniper_ledges|_monster_is_berserker|_monster_can_die_in_flames, /* flags */
	
		  _class_fighter, /* class */
		  _class_pfhor, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _higher_frequency, /* sound pitch */	
		  _snd_fighter_activate, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  _snd_fighter_wail, /* dying flaming */
		  _snd_fighter_chatter, 15, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -4*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  (3*FIXED_ONE)/4, /* external velocity scale */
		  _effect_fighter_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 5*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast4, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _normal_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  4, /* being hit */
		  1, 3, /* dying hard (popping), dying soft (falling) */
		  6, 5, /* hard dead frames, soft dead frames */
		  7, 0, /* stationary shape, moving shape */
		  12, 12, /* teleport in shape, teleport out shape */
		
		  3*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  _projectile_staff, /* melee attack type */
			  1, /* repetitions */
			  0, /* error */
			  WORLD_ONE, /* range */
			
			  2, /* melee attack shape */

			  WORLD_ONE/16, 0, WORLD_ONE_FOURTH, /* dx, dy, dz */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_staff_bolt, /* ranged attack type */
			  1, /* repetitions */
			  NUMBER_OF_ANGLES/150, /* error angle */
			  12*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  WORLD_ONE/16, 0, WORLD_ONE_FOURTH, /* dx, dy, dz */
		  }
		},

		{ /* _civilian_crew "bob" */
		  BUILD_COLLECTION(_collection_civilian, 0), /* shape collection */
		  20, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_attacks_immediately|_monster_is_omniscent|_monster_cannot_be_dropped|_monster_waits_with_clear_shot|_monster_can_die_in_flames|_monster_uses_sniper_ledges, /* flags */

		  _class_human_civilian, /* class */	
		  _class_human, /* friends */
		  (_class_hostile_alien^_class_assimilated_civilian)|_class_native, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  _snd_human_activation, _snd_kill_the_player, _snd_human_clear, _snd_human_trash_talk, _snd_human_apology, _snd_human_stop_shooting_me_you_bastard, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  _snd_human_wail, /* dying flaming */
		  _snd_human_chatter, 0x1f, /* random sound, random sound mask */

		  _i_magnum_magazine, /* carrying item type */
	
		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -2*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE, /* external velocity scale */
		  _effect_civilian_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  10, /* being hit */
		  2, 1, /* dying hard (popping), dying soft (falling) */
		  4, 3, /* hard dead frames, soft dead frames */
		  6, 0, /* stationary shape, moving shape */
		  9, 8, /* teleport in shape, teleport out shape */
		
		  3*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_pistol_bullet, /* ranged attack type */
			  1, /* repetitions */
			  NUMBER_OF_ANGLES/150, /* error angle */
			  10*WORLD_ONE, /* range */
			  5, /* ranged attack shape */
			
			  0, 0, WORLD_ONE*3/4, /* dx, dy, dz */
		  }
		},

		{ /* _civilian_science "fred" */
		  BUILD_COLLECTION(_collection_civilian, 1), /* shape collection */
		  25, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_attacks_immediately|_monster_is_omniscent|_monster_cannot_be_dropped|_monster_waits_with_clear_shot|_monster_can_die_in_flames|_monster_uses_sniper_ledges, /* flags */

		  _class_human_civilian, /* class */	
		  _class_human|_class_assimilated_civilian, /* friends */
		  (_class_hostile_alien^_class_assimilated_civilian)|_class_native, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  _snd_human_activation, _snd_kill_the_player, _snd_human_clear, _snd_human_trash_talk, _snd_human_apology, _snd_human_stop_shooting_me_you_bastard, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  _snd_human_wail, /* dying flaming */
		  _snd_human_chatter, 0x1f, /* random sound, random sound mask */

		  _i_magnum_magazine, /* carrying item type */
	
		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -2*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE, /* external velocity scale */
		  _effect_civilian_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  10, /* being hit */
		  2, 1, /* dying hard (popping), dying soft (falling) */
		  4, 3, /* hard dead frames, soft dead frames */
		  6, 0, /* stationary shape, moving shape */
		  9, 8, /* teleport in shape, teleport out shape */
		
		  3*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_pistol_bullet, /* ranged attack type */
			  2, /* repetitions */
			  NUMBER_OF_ANGLES/150, /* error angle */
			  13*WORLD_ONE, /* range */
			  5, /* ranged attack shape */
			
			  0, 0, WORLD_ONE*3/4, /* dx, dy, dz */
		  }
		},

		{ /* _civilian_security "steve" */
		  BUILD_COLLECTION(_collection_civilian, 2), /* shape collection */
		  30, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_attacks_immediately|_monster_is_omniscent|_monster_cannot_be_dropped|_monster_waits_with_clear_shot|_monster_can_die_in_flames|_monster_uses_sniper_ledges, /* flags */

		  _class_human_civilian, /* class */	
		  _class_human|_class_assimilated_civilian, /* friends */
		  (_class_hostile_alien^_class_assimilated_civilian)|_class_native, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  _snd_human_activation, _snd_kill_the_player, _snd_human_clear, _snd_human_trash_talk, _snd_human_apology, _snd_human_stop_shooting_me_you_bastard, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  _snd_human_wail, /* dying flaming */
		  _snd_human_chatter, 0x1f, /* random sound, random sound mask */

		  _i_magnum, /* carrying item type */
	
		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -2*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE, /* external velocity scale */
		  _effect_civilian_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  10, /* being hit */
		  2, 1, /* dying hard (popping), dying soft (falling) */
		  4, 3, /* hard dead frames, soft dead frames */
		  6, 0, /* stationary shape, moving shape */
		  9, 8, /* teleport in shape, teleport out shape */
		
		  TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_pistol_bullet, /* ranged attack type */
			  5, /* repetitions */
			  NUMBER_OF_ANGLES/150, /* error angle */
			  17*WORLD_ONE, /* range */
			  5, /* ranged attack shape */
			
			  0, 0, WORLD_ONE*3/4, /* dx, dy, dz */
		  }
		},

		{ /* _civilian_assimilated "evil bob" */
		  BUILD_COLLECTION(_collection_civilian, 3), /* shape collection */
		  30, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_is_kamakazi|_monster_can_die_in_flames, /* flags */
		
		  _class_assimilated_civilian,
		  _class_pfhor, /* friends */
		  _class_player|_class_defender, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, _snd_human_stop_shooting_me_you_bastard, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  _snd_human_wail, /* dying flaming */
		  _snd_assimilated_human_chatter, 0xf, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -2*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE, /* external velocity scale */
		  _effect_assimilated_civilian_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  15*WORLD_ONE, WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  WORLD_ONE, {_damage_explosion, _alien_damage, 80, 40, FIXED_ONE}, /* shrapnel radius, shrapnel damage  */
		
		  10, /* being hit */
		  11, NONE, /* dying hard (popping), dying soft (falling) */
		  4, 0, /* hard dead frames, soft dead frames */
		  6, 0, /* stationary shape, moving shape */
		  8, NONE, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  NONE, /* ranged attack type */
		  }
		},

		{ /* _monster_hummer_minor (small hummer) */
		  BUILD_COLLECTION(_collection_hummer, 0), /* shape collection */
		  40, 0, FLAG(_damage_fusion_bolt)|FLAG(_damage_compiler_bolt)|FLAG(_damage_electrical_staff), /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_flys|_monster_minor|_monster_has_delayed_hard_death, /* flags */
		
		  _class_hummer, /* class */
		  _class_pfhor|_class_client, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
		
		  _normal_frequency, /* sound pitch */
		  _snd_hummer_activate, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming death sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/4, WORLD_ONE_HALF, /* radius, height */
		  WORLD_ONE_FOURTH, /* preferred hover height */
		  -5*WORLD_ONE, 5*WORLD_ONE, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE, /* external velocity scale */
		  _effect_hummer_spark, _effect_metallic_clang, _effect_rocket_contrail, /* impact effect, melee impact effect, contrail effect */
	
		  HALF_CIRCLE, QUARTER_CIRCLE, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 15*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_low, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _normal_door_retry_mask, /* door retry mask */
		  NONE, {NONE, _alien_damage, 30, 10, FIXED_ONE}, /* shrapnel radius, shrapnel damage */
		
		  0, /* being hit */
		  3, 2, /* dying hard (popping), dying soft (falling) */
		  4, NONE, /* hard dead frames, soft dead frames */
		  0, 0, /* stationary shape, moving shape (no permutations) */
		  0, 0, /* teleport in shape, teleport out shape */
		
		  3*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_minor_hummer, /* ranged attack type */
			  0, /* repetitions */
			  3, /* error angle */
			  12*WORLD_ONE, /* range */
			  1, /* ranged attack shape */
			
			  0, 0, 0, /* dx, dy, dz */
		  }
		},

		{ /* _monster_hummer_major (big hummer) */
		  BUILD_COLLECTION(_collection_hummer, 1), /* shape collection */
		  60, 0, FLAG(_damage_fusion_bolt)|FLAG(_damage_compiler_bolt)|FLAG(_damage_electrical_staff), /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_flys|_monster_major|_monster_has_delayed_hard_death, /* flags */
		
		  _class_hummer, /* class */
		  _class_pfhor|_class_client, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
		
		  _normal_frequency, /* sound pitch */
		  _snd_hummer_activate, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming death sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/4, WORLD_ONE_HALF, /* radius, height */
		  WORLD_ONE_FOURTH, /* preferred hover height */
		  -5*WORLD_ONE, 5*WORLD_ONE, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE, /* external velocity scale */
		  _effect_hummer_spark, _effect_metallic_clang, _effect_rocket_contrail, /* impact effect, melee impact effect, contrail effect */
	
		  HALF_CIRCLE, QUARTER_CIRCLE, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 15*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_low, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _normal_door_retry_mask, /* door retry mask */
		  NONE, {NONE, _alien_damage, 30, 10, FIXED_ONE}, /* shrapnel radius, shrapnel damage */
		
		  0, /* being hit */
		  3, 2, /* dying hard (popping), dying soft (falling) */
		  4, NONE, /* hard dead frames, soft dead frames */
		  0, 0, /* stationary shape, moving shape (no permutations) */
		  0, 0, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_minor_hummer, /* ranged attack type */
			  2, /* repetitions */
			  5, /* error angle */
			  12*WORLD_ONE, /* range */
			  1, /* ranged attack shape */
			
			  0, 0, 0, /* dx, dy, dz */
		  }
		},

		{ /* _monster_hummer_big_minor (big hummer) */
		  BUILD_COLLECTION(_collection_hummer, 2), /* shape collection */
		  40, 0, FLAG(_damage_fusion_bolt)|FLAG(_damage_compiler_bolt)|FLAG(_damage_electrical_staff), /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_flys|_monster_minor|_monster_has_delayed_hard_death, /* flags */
		
		  _class_hummer, /* class */
		  _class_pfhor|_class_client, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
		
		  _higher_frequency, /* sound pitch */
		  _snd_hummer_activate, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming death sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/4, WORLD_ONE_HALF, /* radius, height */
		  WORLD_ONE_FOURTH, /* preferred hover height */
		  -5*WORLD_ONE, 5*WORLD_ONE, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE, /* external velocity scale */
		  _effect_hummer_spark, _effect_metallic_clang, _effect_rocket_contrail, /* impact effect, melee impact effect, contrail effect */
	
		  HALF_CIRCLE, QUARTER_CIRCLE, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 15*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_low, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _normal_door_retry_mask, /* door retry mask */
		  NONE, {NONE, _alien_damage, 30, 10, FIXED_ONE}, /* shrapnel radius, shrapnel damage */
		
		  0, /* being hit */
		  3, 2, /* dying hard (popping), dying soft (falling) */
		  4, NONE, /* hard dead frames, soft dead frames */
		  0, 0, /* stationary shape, moving shape (no permutations) */
		  0, 0, /* teleport in shape, teleport out shape */
		
		  3*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_minor_hummer, /* ranged attack type */
			  0, /* repetitions */
			  3, /* error angle */
			  12*WORLD_ONE, /* range */
			  1, /* ranged attack shape */
			
			  0, 0, 0, /* dx, dy, dz */
		  }
		},

		{ /* _monster_hummer_big_major (angry hummer) */
		  BUILD_COLLECTION(_collection_hummer, 3), /* shape collection */
		  60, 0, FLAG(_damage_fusion_bolt)|FLAG(_damage_compiler_bolt)|FLAG(_damage_electrical_staff), /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_flys|_monster_major|_monster_has_delayed_hard_death, /* flags */
		
		  _class_hummer, /* class */
		  _class_pfhor|_class_client, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
		
		  _higher_frequency, /* sound pitch */
		  _snd_hummer_activate, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming death sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/4, WORLD_ONE_HALF, /* radius, height */
		  WORLD_ONE_FOURTH, /* preferred hover height */
		  -5*WORLD_ONE, 5*WORLD_ONE, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE, /* external velocity scale */
		  _effect_hummer_spark, _effect_metallic_clang, _effect_rocket_contrail, /* impact effect, melee impact effect, contrail effect */
	
		  HALF_CIRCLE, QUARTER_CIRCLE, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 15*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_low, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _normal_door_retry_mask, /* door retry mask */
		  NONE, {NONE, _alien_damage, 30, 10, FIXED_ONE}, /* shrapnel radius, shrapnel damage */
		
		  0, /* being hit */
		  3, 2, /* dying hard (popping), dying soft (falling) */
		  4, NONE, /* hard dead frames, soft dead frames */
		  0, 0, /* stationary shape, moving shape (no permutations) */
		  0, 0, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_minor_hummer, /* ranged attack type */
			  2, /* repetitions */
			  5, /* error angle */
			  12*WORLD_ONE, /* range */
			  1, /* ranged attack shape */
			
			  0, 0, 0, /* dx, dy, dz */
		  }
		},

		{ /* _monster_hummer_possessed (hummer from durandal) */
		  BUILD_COLLECTION(_collection_hummer, 4), /* shape collection */
		  60, 0, FLAG(_damage_fusion_bolt)|FLAG(_damage_compiler_bolt)|FLAG(_damage_electrical_staff), /* vitality, immunities, weaknesses */
		  _monster_flys|_monster_has_delayed_hard_death|_monster_attacks_immediately, /* flags */
		
		  _class_possessed_hummer, /* class */
		  _class_human, /* friends */
		  _class_pfhor|_class_client|_class_native, /* enemies */
		
		  _lower_frequency, /* sound pitch */
		  _snd_hummer_activate, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming death sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/4, WORLD_ONE_HALF, /* radius, height */
		  WORLD_ONE_FOURTH, /* preferred hover height */
		  -5*WORLD_ONE, 5*WORLD_ONE, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE, /* external velocity scale */
		  _effect_hummer_spark, _effect_metallic_clang, _effect_rocket_contrail, /* impact effect, melee impact effect, contrail effect */
	
		  HALF_CIRCLE, QUARTER_CIRCLE, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 15*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_low, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _normal_door_retry_mask, /* door retry mask */
		  NONE, {NONE, _alien_damage, 30, 10, FIXED_ONE}, /* shrapnel radius, shrapnel damage */
		
		  0, /* being hit */
		  3, 2, /* dying hard (popping), dying soft (falling) */
		  4, NONE, /* hard dead frames, soft dead frames */
		  0, 0, /* stationary shape, moving shape (no permutations) */
		  0, 0, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_durandal_hummer, /* ranged attack type */
			  1, /* repetitions */
			  5, /* error angle */
			  12*WORLD_ONE, /* range */
			  1, /* ranged attack shape */
			
			  0, 0, 0, /* dx, dy, dz */
		  }
		},

		{ /* _monster_cyborg_minor */
		  BUILD_COLLECTION(_collection_cyborg, 0), /* shape collection */
		  300, 0, FLAG(_damage_fusion_bolt), /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_minor|_monster_uses_sniper_ledges, /* flags */
		
		  _class_cyborg, /* class */
		  _class_cyborg, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming dying sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/4, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -WORLD_ONE, WORLD_ONE/4, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE/4, /* external velocity scale */
		  _effect_cyborg_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 3*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast5, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  WORLD_ONE, {_damage_explosion, _alien_damage, 60, 0, FIXED_ONE}, /* shrapnel radius, shrapnel damage */
		
		  0, /* being hit */
		  3, NONE, /* dying hard (popping), dying soft (falling) */
		  5, NONE, /* hard dead frames, soft dead frames */
		  0, 1, /* stationary shape, moving shape */
		  0, 0, /* teleport in shape, teleport out shape */
		
		  4*TICKS_PER_SECOND, /* attack frequency */
		
		  /* melee attack */
		  {
			  NONE,
		  },
		
		  /* ranged attack */
		  {
			  _projectile_minor_cyborg_ball, /* ranged attack type */
			  0, /* repetitions */
			  0, /* error angle */
			  10*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  WORLD_ONE/16, 0, WORLD_ONE_HALF+WORLD_ONE_FOURTH/2, /* dx, dy, dz */
		  }
		},
	
		{ /* _monster_cyborg_major */
		  BUILD_COLLECTION(_collection_cyborg, 1), /* shape collection */
		  450, 0, FLAG(_damage_fusion_bolt), /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_major|_monster_uses_sniper_ledges, /* flags */
		
		  _class_cyborg, /* class */
		  _class_cyborg, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming dying sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/4, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -WORLD_ONE, WORLD_ONE/4, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE/4, /* external velocity scale */
		  _effect_cyborg_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 3*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast4, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  WORLD_ONE, {_damage_explosion, _alien_damage, 60, 0, FIXED_ONE}, /* shrapnel radius, shrapnel damage */
		
		  0, /* being hit */
		  3, NONE, /* dying hard (popping), dying soft (falling) */
		  5, NONE, /* hard dead frames, soft dead frames */
		  0, 1, /* stationary shape, moving shape */
		  0, 0, /* teleport in shape, teleport out shape */
		
		  3*TICKS_PER_SECOND, /* attack frequency */
		
		  /* melee attack */
		  {
			  NONE,
		  },
		
		  /* ranged attack */
		  {
			  _projectile_minor_cyborg_ball, /* ranged attack type */
			  1, /* repetitions */
			  0, /* error angle */
			  10*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  WORLD_ONE/16, 0, WORLD_ONE_HALF+WORLD_ONE_FOURTH/2, /* dx, dy, dz */
		  }
		},
	
		{ /* _monster_cyborg_flame_minor */
		  BUILD_COLLECTION(_collection_cyborg, 0), /* shape collection */
		  300, 0, FLAG(_damage_fusion_bolt), /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_minor|_monster_uses_sniper_ledges, /* flags */
		
		  _class_cyborg, /* class */
		  _class_cyborg, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _lower_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming dying sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/4, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -WORLD_ONE, WORLD_ONE/4, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE/4, /* external velocity scale */
		  _effect_cyborg_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 3*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast4, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  WORLD_ONE, {_damage_explosion, _alien_damage, 60, 0, FIXED_ONE}, /* shrapnel radius, shrapnel damage */
		
		  0, /* being hit */
		  3, NONE, /* dying hard (popping), dying soft (falling) */
		  5, NONE, /* hard dead frames, soft dead frames */
		  0, 1, /* stationary shape, moving shape */
		  0, 0, /* teleport in shape, teleport out shape */
		
		  4*TICKS_PER_SECOND, /* attack frequency */
		
		  /* melee attack */
		  {
			  _projectile_flamethrower_burst, /* ranged attack type */
			  15, /* repetitions */
			  0, /* error angle */
			  2*WORLD_ONE, /* range */
			  4, /* ranged attack shape */
			
			  WORLD_ONE/16, 0, WORLD_ONE_HALF+WORLD_ONE_FOURTH/2, /* dx, dy, dz */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_major_cyborg_ball, /* ranged attack type */
			  0, /* repetitions */
			  0, /* error angle */
			  10*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  WORLD_ONE/16, 0, WORLD_ONE_HALF+WORLD_ONE_FOURTH/2, /* dx, dy, dz */
		  }
		},
	
		{ /* _monster_cyborg_flame_major */
		  BUILD_COLLECTION(_collection_cyborg, 0), /* shape collection */
		  450, 0, FLAG(_damage_fusion_bolt), /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_major|_monster_uses_sniper_ledges, /* flags */
		
		  _class_cyborg, /* class */
		  _class_cyborg, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _lower_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming dying sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/4, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -WORLD_ONE, WORLD_ONE/4, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE/4, /* external velocity scale */
		  _effect_cyborg_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 3*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast4, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  WORLD_ONE, {_damage_explosion, _alien_damage, 60, 0, FIXED_ONE}, /* shrapnel radius, shrapnel damage */
		
		  0, /* being hit */
		  3, NONE, /* dying hard (popping), dying soft (falling) */
		  5, NONE, /* hard dead frames, soft dead frames */
		  0, 1, /* stationary shape, moving shape */
		  0, 0, /* teleport in shape, teleport out shape */
		
		  3*TICKS_PER_SECOND, /* attack frequency */
		
		  /* melee attack */
		  {
			  _projectile_flamethrower_burst, /* ranged attack type */
			  15, /* repetitions */
			  0, /* error angle */
			  2*WORLD_ONE, /* range */
			  4, /* ranged attack shape */
			
			  WORLD_ONE/16, 0, WORLD_ONE_HALF+WORLD_ONE_FOURTH/2, /* dx, dy, dz */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_major_cyborg_ball, /* ranged attack type */
			  0, /* repetitions */
			  0, /* error angle */
			  10*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  WORLD_ONE/16, 0, WORLD_ONE_HALF+WORLD_ONE_FOURTH/2, /* dx, dy, dz */
		  }
		},
	
		{ /* _monster_enforcer_minor */
		  BUILD_COLLECTION(_collection_enforcer, 0), /* shape collection */
		  120, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_minor|_monster_uses_sniper_ledges|_monster_can_die_in_flames|_monster_waits_with_clear_shot, /* flags */
		
		  _class_enforcer, /* class */
		  _class_pfhor, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  _snd_enforcer_activate, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  _snd_fighter_wail, /* flaming dying sound */
		  NONE, 0, /* random sound, random sound mask */

		  _i_alien_shotgun, /* carrying item type */
	
		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -2*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  (3*FIXED_ONE)/4, /* external velocity scale */
		  _effect_enforcer_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 3*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast4, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  0, /* being hit */
		  6, 3, /* dying hard (popping), dying soft (falling) */
		  7, 4, /* hard dead frames, soft dead frames */
		  0, 1, /* stationary shape, moving shape */
		  0, 0, /* teleport in shape, teleport out shape */
		
		  4*TICKS_PER_SECOND, /* attack frequency */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_alien_weapon, /* ranged attack type */
			  8, /* repetitions */
			  2, /* error angle */
			  15*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  WORLD_ONE/16, 0, WORLD_ONE_HALF+WORLD_ONE_FOURTH/2, /* dx, dy, dz */
		  }
		},
	
		{ /* _monster_enforcer_major */
		  BUILD_COLLECTION(_collection_enforcer, 1), /* shape collection */
		  160, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_major|_monster_uses_sniper_ledges|_monster_can_die_in_flames|_monster_waits_with_clear_shot, /* flags */
	
		  _class_enforcer, /* class */
		  _class_pfhor, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _higher_frequency, /* sound pitch */	
		  _snd_enforcer_activate, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  _snd_fighter_wail, /* flaming dying sound */
		  NONE, 0, /* random sound, random sound mask */

		  _i_alien_shotgun, /* carrying item type */
	
		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -2*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  (3*FIXED_ONE)/4, /* external velocity scale */
		  _effect_enforcer_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 3*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  0, /* being hit */
		  6, 3, /* dying hard (popping), dying soft (falling) */
		  7, 4, /* hard dead frames, soft dead frames */
		  0, 1, /* stationary shape, moving shape */
		  0, 0, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_alien_weapon, /* ranged attack type */
			  12, /* repetitions */
			  5, /* error angle */
			  20*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  WORLD_ONE/16, 0, WORLD_ONE_HALF+WORLD_ONE_FOURTH/2, /* dx, dy, dz */
		  }
		},
	
		{ /* _monster_hunter_minor */
		  BUILD_COLLECTION(_collection_hunter, 0), /* shape collection */
		  200, FLAG(_damage_flame), FLAG(_damage_fusion_bolt), /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_uses_sniper_ledges|_monster_minor, /* flags */

		  _class_hunter, /* class */
		  _class_pfhor, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming death sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE_HALF, /* external velocity scale */
		  _effect_hunter_spark, _effect_metallic_clang, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 4*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast3, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _normal_door_retry_mask, /* door retry mask */
		  2*WORLD_ONE, {_damage_explosion, _alien_damage, 60, 30, FIXED_ONE}, /* shrapnel radius, shrapnel damage */
		
		  7, /* being hit */
		  3, 9, /* dying hard (popping), dying soft (falling) */
		  6, 10, /* hard dead frames, soft dead frames */
		  1, 0, /* stationary shape, moving shape (no permutations) */
		  1, 1, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_hunter, /* ranged attack type */
			  2, /* repetitions */
			  3, /* error angle */
			  12*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  0, WORLD_ONE/8, WORLD_ONE_HALF+WORLD_ONE_FOURTH, /* dx, dy, dz */
		  }
		},
	
		{ /* _monster_hunter_major */
		  BUILD_COLLECTION(_collection_hunter, 1), /* shape collection */
		  300, FLAG(_damage_flame), FLAG(_damage_fusion_bolt), /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_uses_sniper_ledges|_monster_major, /* flags */

		  _class_hunter, /* class */
		  _class_pfhor, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _higher_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming death sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE_HALF, /* external velocity scale */
		  _effect_hunter_spark, _effect_metallic_clang, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 4*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast5, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _normal_door_retry_mask, /* door retry mask */
		  2*WORLD_ONE, {_damage_explosion, _alien_damage, 60, 30, FIXED_ONE}, /* shrapnel radius, shrapnel damage */
		
		  7, /* being hit */
		  3, 9, /* dying hard (popping), dying soft (falling) */
		  6, 10, /* hard dead frames, soft dead frames */
		  1, 0, /* stationary shape, moving shape (no permutations) */
		  1, 1, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_hunter, /* ranged attack type */
			  5, /* repetitions */
			  3, /* error angle */
			  12*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  0, WORLD_ONE/8, WORLD_ONE_HALF+WORLD_ONE_FOURTH, /* dx, dy, dz */
		  }
		},

		{ /* _monster_trooper_minor */
		  BUILD_COLLECTION(_collection_trooper, 0), /* shape collection */
		  110, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_minor|_monster_uses_sniper_ledges|_monster_is_berserker|_monster_can_die_in_flames, /* flags */
		
		  _class_trooper, /* class */
		  _class_pfhor, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  _snd_fighter_activate, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  _snd_fighter_wail, /* dying flaming */
		  _snd_fighter_chatter, 15, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -4*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  (3*FIXED_ONE)/4, /* external velocity scale */
		  _effect_trooper_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast3, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _normal_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  4, /* being hit */
		  NONE, 3, /* dying hard (popping), dying soft (falling) */
		  0, 7, /* hard dead frames, soft dead frames */
		  1, 0, /* stationary shape, moving shape */
		  1, 1, /* teleport in shape, teleport out shape */
		
		  4*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  _projectile_trooper_bullet, /* melee attack type */
			  3, /* repetitions */
			  30, /* error */
			  3*WORLD_ONE, /* range */
			
			  2, /* melee attack shape */

			  0, -WORLD_ONE/10, WORLD_ONE_HALF-WORLD_ONE_FOURTH/4, /* dx, dy, dz */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_trooper_grenade, /* ranged attack type */
			  0, /* repetitions */
			  10, /* error angle */
			  10*WORLD_ONE, /* range */
			  9, /* ranged attack shape */
			
			  -WORLD_ONE/10, WORLD_ONE/8, WORLD_ONE_HALF-WORLD_ONE_FOURTH/8, /* dx, dy, dz */
		  }
		},

		{ /* _monster_trooper_major */
		  BUILD_COLLECTION(_collection_trooper, 1), /* shape collection */
		  200, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_major|_monster_uses_sniper_ledges|_monster_is_berserker|_monster_can_die_in_flames, /* flags */
		
		  _class_trooper, /* class */
		  _class_pfhor, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _lower_frequency, /* sound pitch */	
		  _snd_fighter_activate, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  _snd_fighter_wail, /* dying flaming */
		  _snd_fighter_chatter, 15, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -4*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  (3*FIXED_ONE)/4, /* external velocity scale */
		  _effect_trooper_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast3, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _fast_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  4, /* being hit */
		  NONE, 3, /* dying hard (popping), dying soft (falling) */
		  0, 7, /* hard dead frames, soft dead frames */
		  1, 0, /* stationary shape, moving shape */
		  1, 1, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  _projectile_trooper_bullet, /* melee attack type */
			  8, /* repetitions */
			  10, /* error */
			  3*WORLD_ONE, /* range */
			
			  2, /* melee attack shape */

			  -WORLD_ONE/10, WORLD_ONE/8, WORLD_ONE_HALF-WORLD_ONE_FOURTH/8, /* dx, dy, dz */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_trooper_grenade, /* ranged attack type */
			  1, /* repetitions */
			  5, /* error angle */
			  12*WORLD_ONE, /* range */
			  9, /* ranged attack shape */
			
			  0, -WORLD_ONE/10, WORLD_ONE_HALF-WORLD_ONE_FOURTH/4, /* dx, dy, dz */
		  }
		},

		{ /* _monster_mother_of_all_cyborgs */
		  BUILD_COLLECTION(_collection_cyborg, 0), /* shape collection */
		  1500, 0, FLAG(_damage_fusion_bolt), /* vitality, immunities, weaknesses */
		  _monster_is_enlarged|_monster_is_alien|_monster_cannot_be_dropped|_monster_uses_sniper_ledges, /* flags */
		
		  _class_cyborg, /* class */
		  0, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _lower_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming dying sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/3, WORLD_ONE + WORLD_ONE/5, /* radius, height */
		  0, /* preferred hover height */
		  -WORLD_ONE, WORLD_ONE/4, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE/4, /* external velocity scale */
		  _effect_cyborg_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 3*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast4, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  3*WORLD_ONE, {_damage_explosion, _alien_damage, 140, 40, FIXED_ONE}, /* shrapnel radius, shrapnel damage */
		
		  0, /* being hit */
		  3, NONE, /* dying hard (popping), dying soft (falling) */
		  5, NONE, /* hard dead frames, soft dead frames */
		  0, 1, /* stationary shape, moving shape */
		  0, 0, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency */
		
		  /* melee attack */
		  {
			  _projectile_flamethrower_burst, /* ranged attack type */
			  15, /* repetitions */
			  0, /* error angle */
			  2*WORLD_ONE, /* range */
			  4, /* ranged attack shape */
			
			  WORLD_ONE/16, 0, WORLD_ONE_HALF+WORLD_ONE_FOURTH, /* dx, dy, dz */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_major_cyborg_ball, /* ranged attack type */
			  0, /* repetitions */
			  0, /* error angle */
			  10*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  WORLD_ONE/16, 0, WORLD_ONE_HALF+WORLD_ONE_FOURTH, /* dx, dy, dz */
		  }
		},
	
		{ /* _monster_mother_of_all_hunters */
		  BUILD_COLLECTION(_collection_hunter, 2), /* shape collection */
		  1500, FLAG(_damage_flame), FLAG(_damage_fusion_bolt), /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_is_enlarged|_monster_uses_sniper_ledges|_monster_cannot_be_dropped, /* flags */

		  _class_hunter, /* class */
		  _class_pfhor, /* friends */
		  _class_human|_class_native|_class_defender, /* enemies */
	
		  _lower_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming death sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/4, WORLD_ONE+WORLD_ONE/6, /* radius, height */
		  0, /* preferred hover height */
		  -WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE_HALF, /* external velocity scale */
		  _effect_hunter_spark, _effect_metallic_clang, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 4*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast1, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _normal_door_retry_mask, /* door retry mask */
		  4*WORLD_ONE, {_damage_explosion, _alien_damage, 140, 50, FIXED_ONE}, /* shrapnel radius, shrapnel damage */
		
		  7, /* being hit */
		  3, NONE, /* dying hard (popping), dying soft (falling) */
		  6, 8, /* hard dead frames, soft dead frames */
		  1, 0, /* stationary shape, moving shape (no permutations) */
		  1, 1, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_hunter, /* ranged attack type */
			  5, /* repetitions */
			  3, /* error angle */
			  12*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  0, WORLD_ONE/8, WORLD_ONE_HALF+WORLD_ONE_FOURTH, /* dx, dy, dz */
		  }
		},

		{ /* _monster_sewage_yeti */
		  BUILD_COLLECTION(_collection_yeti, 0), /* shape collection */
		  100, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_is_not_afraid_of_sewage|_monster_is_alien|_monster_is_berserker, /* flags */
		
		  _class_yeti, /* class */
		  _class_yeti, /* friends */
		  _class_pfhor|_class_human|_class_defender, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* dying flaming */
		  NONE, 15, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/4, WORLD_ONE-1, /* radius, height */
		  0, /* preferred hover height */
		  -2*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  3*FIXED_ONE/4, /* external velocity scale */
		  _effect_sewage_yeti_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_low, /* intelligence */
		  _speed_superfast5, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _slow_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  12, /* being hit */
		  NONE, 3, /* dying hard (popping), dying soft (falling) */
		  NONE, 4, /* hard dead frames, soft dead frames */
		  0, 1, /* stationary shape, moving shape */
		  NONE, NONE, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  _projectile_yeti, /* melee attack type */
			  0, /* repetitions */
			  0, /* error */
			  WORLD_ONE, /* range */
			  13, /* melee attack shape */

			  0, 0, 4*WORLD_ONE/5, /* dx, dy, dz */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_sewage_yeti, /* ranged attack type */
			  0, /* repetitions */
			  NUMBER_OF_ANGLES/150, /* error angle */
			  12*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  WORLD_ONE/3, WORLD_ONE/6, 4*WORLD_ONE/5, /* dx, dy, dz */
		  }
		},

		{ /* _monster_water_yeti */
		  BUILD_COLLECTION(_collection_yeti, 1), /* shape collection */
		  250, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_is_not_afraid_of_water|_monster_is_alien|_monster_is_berserker, /* flags */
		
		  _class_yeti, /* class */
		  _class_yeti, /* friends */
		  _class_pfhor|_class_human|_class_defender, /* enemies */
	
		  _normal_frequency, /* sound pitch */
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* dying flaming */
		  NONE, 15, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/4, WORLD_ONE-1, /* radius, height */
		  0, /* preferred hover height */
		  -2*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  3*FIXED_ONE/4, /* external velocity scale */
		  _effect_water_yeti_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_low, /* intelligence */
		  _speed_superfast5, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _slow_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  12, /* being hit */
		  NONE, 3, /* dying hard (popping), dying soft (falling) */
		  NONE, 4, /* hard dead frames, soft dead frames */
		  0, 1, /* stationary shape, moving shape */
		  NONE, NONE, /* teleport in shape, teleport out shape */
		
		  TICKS_PER_SECOND/2, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  _projectile_yeti, /* melee attack type */
			  1, /* repetitions */
			  0, /* error */
			  WORLD_ONE, /* range */
			  13, /* melee attack shape */

			  0, WORLD_ONE/6, 4*WORLD_ONE/5, /* dx, dy, dz */
		  },
		
		  /* ranged attack */
		  {
			  NONE
		  }
		},

		{ /* _monster_lava_yeti */
		  BUILD_COLLECTION(_collection_yeti, 2), /* shape collection */
		  200, FLAG(_damage_flame)|FLAG(_damage_alien_projectile)|FLAG(_damage_fusion_bolt)|FLAG(_damage_lava), 0, /* vitality, immunities, weaknesses */
		  _monster_is_not_afraid_of_lava|_monster_is_alien|_monster_is_berserker, /* flags */
		
		  _class_yeti, /* class */
		  _class_yeti, /* friends */
		  _class_pfhor|_class_human|_class_defender, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* dying flaming */
		  NONE, 15, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/4, WORLD_ONE-1, /* radius, height */
		  0, /* preferred hover height */
		  -2*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  3*FIXED_ONE/4, /* external velocity scale */
		  _effect_lava_yeti_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_low, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _slow_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  12, /* being hit */
		  NONE, 3, /* dying hard (popping), dying soft (falling) */
		  NONE, 4, /* hard dead frames, soft dead frames */
		  0, 1, /* stationary shape, moving shape */
		  NONE, NONE, /* teleport in shape, teleport out shape */
		
		  TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  _projectile_yeti, /* melee attack type */
			  0, /* repetitions */
			  0, /* error */
			  WORLD_ONE, /* range */
			  13, /* melee attack shape */

			  0, 0, 4*WORLD_ONE/5, /* dx, dy, dz */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_lava_yeti, /* ranged attack type */
			  1, /* repetitions */
			  NUMBER_OF_ANGLES/150, /* error angle */
			  12*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  WORLD_ONE/3, WORLD_ONE/6, 4*WORLD_ONE/5, /* dx, dy, dz */
		  }
		},

		{ /* _monster_defender_minor */
		  BUILD_COLLECTION(_collection_defender, 0), /* shape collection */
		  160, 0, FLAG(_damage_fusion_bolt), /* vitality, immunities, weaknesses */
		  _monster_flys|_monster_waits_with_clear_shot, /* flags */

		  _class_defender, /* class */
		  _class_defender, /* friends */
		  _class_pfhor|_class_client|_class_native, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming dying sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/5, WORLD_ONE, /* radius, height */
		  WORLD_ONE/4, /* preferred hover height */
		  SHORT_MIN, SHORT_MAX, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE, /* external velocity scale */
		  NONE, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 3*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  3, /* being hit */
		  NONE, 6, /* dying hard (popping), dying soft (falling) */
		  NONE, NONE, /* hard dead frames, soft dead frames */
		  0, 0, /* stationary shape, moving shape */
		  8, 8, /* teleport in shape, teleport out shape */
		
		  3*TICKS_PER_SECOND, /* attack frequency */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_minor_defender, /* ranged attack type */
			  2, /* repetitions */
			  NUMBER_OF_ANGLES/200, /* error angle */
			  20*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  WORLD_ONE/8, -WORLD_ONE/4+WORLD_ONE/10, WORLD_ONE_HALF, /* dx, dy, dz */
		  }
		},

		{ /* _monster_defender_major */
		  BUILD_COLLECTION(_collection_defender, 1), /* shape collection */
		  240, 0, FLAG(_damage_fusion_bolt), /* vitality, immunities, weaknesses */
		  _monster_flys|_monster_waits_with_clear_shot, /* flags */

		  _class_defender, /* class */
		  _class_defender, /* friends */
		  _class_pfhor|_class_client|_class_native, /* enemies */
	
		  _higher_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming dying sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/5, WORLD_ONE, /* radius, height */
		  WORLD_ONE/4, /* preferred hover height */
		  SHORT_MIN, SHORT_MAX, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE, /* external velocity scale */
		  NONE, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 3*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  3, /* being hit */
		  NONE, 6, /* dying hard (popping), dying soft (falling) */
		  NONE, NONE, /* hard dead frames, soft dead frames */
		  0, 0, /* stationary shape, moving shape */
		  8, 8, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_major_defender, /* ranged attack type */
			  4, /* repetitions */
			  NUMBER_OF_ANGLES/100, /* error angle */
			  20*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  WORLD_ONE/8, -WORLD_ONE/4+WORLD_ONE/10, WORLD_ONE_HALF, /* dx, dy, dz */
		  }
		},

		{ /* _monster_juggernaut_minor */
		  BUILD_COLLECTION(_collection_juggernaut, 0), /* shape collection */
		  2500, 0, FLAG(_damage_fusion_bolt), /* vitality, immunities, weaknesses */
		  _monster_minor|_monster_is_alien|_monster_cant_fire_backwards|_monster_has_nuclear_hard_death|
		  _monster_has_delayed_hard_death|_monster_cannot_be_dropped|_monster_fires_symmetrically|
		  _monster_chooses_weapons_randomly|_monster_flys, /* flags */

		  _class_juggernaut, /* class */
		  _class_juggernaut, /* friends */
		  _class_human|_class_client|_class_native, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming dying sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE, 2*WORLD_ONE, /* radius, height */
		  WORLD_ONE, /* preferred hover height */
		  SHORT_MIN, SHORT_MAX, /* minimum ledge delta, maximum ledge delta */
		  0, /* external velocity scale */
		  _effect_juggernaut_spark, _effect_metallic_clang, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 3*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY/4, NORMAL_MONSTER_TERMINAL_VELOCITY/4, /* gravity, terminal velocity */
		  NONE, /* door retry mask */
		  5*WORLD_ONE, {_damage_explosion, _alien_damage, 350, 50, FIXED_ONE}, /* shrapnel radius, shrapnel damage */
		
		  NONE, /* being hit */
		  6, 5, /* dying hard (popping), dying soft (falling) */
		  8, 8, /* hard dead frames, soft dead frames */
		  0, 0, /* stationary shape, moving shape */
		  7, 7, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency */
		
		  /* melee attack */
		  {
			  _projectile_alien_weapon, /* melee attack type */
			  10, /* repetitions */
			  5, /* error */
			  15*WORLD_ONE, /* range */
			  1, /* melee attack shape */

			  WORLD_ONE/4, WORLD_ONE_HALF+WORLD_ONE/8, WORLD_ONE-WORLD_ONE/4-WORLD_ONE/16, /* dx, dy, dz */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_juggernaut_missile, /* ranged attack type */
			  0, /* repetitions */
			  40, /* error angle */
			  25*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  0, WORLD_ONE_HALF, WORLD_ONE+WORLD_ONE_HALF, /* dx, dy, dz */
		  }
		},

		{ /* _monster_juggernaut_major */
		  BUILD_COLLECTION(_collection_juggernaut, 1), /* shape collection */
		  5000, 0, FLAG(_damage_fusion_bolt), /* vitality, immunities, weaknesses */
		  _monster_major|_monster_is_alien|_monster_cant_fire_backwards|_monster_has_nuclear_hard_death|
		  _monster_has_delayed_hard_death|_monster_cannot_be_dropped|_monster_fires_symmetrically|
		  _monster_chooses_weapons_randomly|_monster_flys, /* flags */

		  _class_juggernaut, /* class */
		  _class_juggernaut, /* friends */
		  _class_human|_class_client|_class_native, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* flaming dying sound */
		  NONE, 0, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE, 2*WORLD_ONE, /* radius, height */
		  WORLD_ONE, /* preferred hover height */
		  SHORT_MIN, SHORT_MAX, /* minimum ledge delta, maximum ledge delta */
		  0, /* external velocity scale */
		  _effect_juggernaut_spark, _effect_metallic_clang, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 3*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY/4, NORMAL_MONSTER_TERMINAL_VELOCITY/4, /* gravity, terminal velocity */
		  NONE, /* door retry mask */
		  5*WORLD_ONE, {_damage_explosion, _alien_damage, 350, 50, FIXED_ONE}, /* shrapnel radius, shrapnel damage */
		
		  NONE, /* being hit */
		  6, 5, /* dying hard (popping), dying soft (falling) */
		  8, 8, /* hard dead frames, soft dead frames */
		  0, 0, /* stationary shape, moving shape */
		  7, 7, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency */
		
		  /* melee attack */
		  {
			  _projectile_alien_weapon, /* melee attack type */
			  20, /* repetitions */
			  5, /* error */
			  15*WORLD_ONE, /* range */
			  1, /* melee attack shape */

			  WORLD_ONE/4, WORLD_ONE_HALF+WORLD_ONE/8, WORLD_ONE-WORLD_ONE/4-WORLD_ONE/16, /* dx, dy, dz */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_juggernaut_missile, /* ranged attack type */
			  1, /* repetitions */
			  40, /* error angle */
			  25*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  0, WORLD_ONE_HALF, WORLD_ONE+WORLD_ONE_HALF, /* dx, dy, dz */
		  }
		},

		{ /* _monster_tiny_fighter */
		  BUILD_COLLECTION(_collection_fighter, 1), /* shape collection */
		  40, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_is_tiny|_monster_is_berserker|_monster_can_die_in_flames, /* flags */
	
		  _class_fighter, /* class */
		  _class_pfhor, /* friends */
		  (_class_human&~_class_player)|_class_native|_class_defender, /* enemies */
	
		  FIXED_ONE+FIXED_ONE_HALF, /* sound pitch */	
		  _snd_fighter_activate, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  _snd_fighter_wail, /* dying flaming */
		  _snd_fighter_chatter, 15, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/12, (4*WORLD_ONE)/12, /* radius, height */
		  0, /* preferred hover height */
		  -8*WORLD_ONE, WORLD_ONE/6, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE/2, /* external velocity scale */
		  _effect_fighter_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, 3*WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_fast, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _normal_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  4, /* being hit */
		  1, 3, /* dying hard (popping), dying soft (falling) */
		  6, 5, /* hard dead frames, soft dead frames */
		  7, 0, /* stationary shape, moving shape */
		  12, 12, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  _projectile_staff, /* melee attack type */
			  0, /* repetitions */
			  0, /* error */
			  WORLD_ONE, /* range */
			
			  2, /* melee attack shape */

			  0, 0, WORLD_ONE/5, /* dx, dy, dz */
		  },
		
		  /* ranged attack */
		  {
			  NONE, /* ranged attack type */
		  }
		},

		{ /* _monster_tiny_bob */
		  BUILD_COLLECTION(_collection_civilian, 0), /* shape collection */
		  10, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_attacks_immediately|_monster_is_omniscent|_monster_cannot_be_dropped|_monster_waits_with_clear_shot|_monster_can_die_in_flames|_monster_uses_sniper_ledges|_monster_is_tiny, /* flags */

		  _class_human_civilian, /* class */	
		  _class_human, /* friends */
		  (_class_hostile_alien^_class_assimilated_civilian)|_class_native, /* enemies */
	
		  FIXED_ONE+FIXED_ONE_HALF, /* sound pitch */	
		  _snd_human_activation, _snd_kill_the_player, _snd_human_clear, _snd_human_trash_talk, _snd_human_apology, _snd_human_stop_shooting_me_you_bastard, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  _snd_human_wail, /* dying flaming */
		  _snd_human_chatter, 0x1f, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/12, (4*WORLD_ONE)/12, /* radius, height */
		  0, /* preferred hover height */
		  -WORLD_ONE, WORLD_ONE/6, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE/2, /* external velocity scale */
		  _effect_civilian_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_superfast2, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  10, /* being hit */
		  2, 1, /* dying hard (popping), dying soft (falling) */
		  4, 3, /* hard dead frames, soft dead frames */
		  6, 0, /* stationary shape, moving shape */
		  9, 8, /* teleport in shape, teleport out shape */
		
		  3*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_pistol_bullet, /* ranged attack type */
			  1, /* repetitions */
			  NUMBER_OF_ANGLES/150, /* error angle */
			  10*WORLD_ONE, /* range */
			  5, /* ranged attack shape */
			
			  0, 0, WORLD_ONE/5, /* dx, dy, dz */
		  }
		},

		{ /* _monster_tiny_yeti */
		  BUILD_COLLECTION(_collection_yeti, 2), /* shape collection */
		  100, FLAG(_damage_flame)|FLAG(_damage_alien_projectile)|FLAG(_damage_fusion_bolt)|FLAG(_damage_lava), 0, /* vitality, immunities, weaknesses */
		  _monster_is_not_afraid_of_lava|_monster_is_berserker|_monster_is_tiny, /* flags */
		
		  _class_yeti, /* class */
		  _class_yeti, /* friends */
		  (_class_human&~_class_player)|_class_pfhor, /* enemies */
	
		  FIXED_ONE+FIXED_ONE_HALF, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, NONE, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  NONE, /* dying flaming */
		  NONE, 15, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/12, (4*WORLD_ONE)/12, /* radius, height */
		  0, /* preferred hover height */
		  -WORLD_ONE, WORLD_ONE/6, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE/2, /* external velocity scale */
		  _effect_lava_yeti_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_low, /* intelligence */
		  _speed_superfast2, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _slow_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  12, /* being hit */
		  NONE, 3, /* dying hard (popping), dying soft (falling) */
		  NONE, 4, /* hard dead frames, soft dead frames */
		  0, 1, /* stationary shape, moving shape */
		  NONE, NONE, /* teleport in shape, teleport out shape */
		
		  TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  _projectile_yeti, /* melee attack type */
			  0, /* repetitions */
			  0, /* error */
			  WORLD_ONE, /* range */
			  13, /* melee attack shape */

			  0, 0, WORLD_ONE/5, /* dx, dy, dz */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_lava_yeti, /* ranged attack type */
			  1, /* repetitions */
			  NUMBER_OF_ANGLES/150, /* error angle */
			  12*WORLD_ONE, /* range */
			  2, /* ranged attack shape */
			
			  0, 0, WORLD_ONE/5, /* dx, dy, dz */
		  }
		},

		{ /* _civilian_crew "bob" */
		  BUILD_COLLECTION(_collection_vacuum_civilian, 0), /* shape collection */
		  20, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_attacks_immediately|_monster_is_omniscent|_monster_cannot_be_dropped|_monster_waits_with_clear_shot|_monster_can_die_in_flames|_monster_uses_sniper_ledges, /* flags */

		  _class_human_civilian, /* class */	
		  _class_human, /* friends */
		  (_class_hostile_alien^_class_assimilated_civilian)|_class_native, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  _snd_fusion_human_activation, _snd_fusion_kill_the_player, _snd_fusion_human_clear, _snd_fusion_human_trash_talk, _snd_fusion_human_apology, _snd_fusion_human_stop_shooting_me_you_bastard, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  _snd_fusion_human_wail, /* dying flaming */
		  _snd_fusion_human_chatter, 0x1f, /* random sound, random sound mask */

		  _i_plasma_magazine, /* carrying item type */
	
		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -2*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE, /* external velocity scale */
		  _effect_vacuum_civilian_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  10, /* being hit */
		  2, 1, /* dying hard (popping), dying soft (falling) */
		  4, 3, /* hard dead frames, soft dead frames */
		  6, 0, /* stationary shape, moving shape */
		  9, 8, /* teleport in shape, teleport out shape */
		
		  3*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_fusion_bolt_minor, /* ranged attack type */
			  1, /* repetitions */
			  NUMBER_OF_ANGLES/150, /* error angle */
			  10*WORLD_ONE, /* range */
			  5, /* ranged attack shape */
			
			  0, 0, WORLD_ONE*3/4, /* dx, dy, dz */
		  }
		},

		{ /* _civilian_science "fred" */
		  BUILD_COLLECTION(_collection_vacuum_civilian, 1), /* shape collection */
		  25, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_attacks_immediately|_monster_is_omniscent|_monster_cannot_be_dropped|_monster_waits_with_clear_shot|_monster_can_die_in_flames|_monster_uses_sniper_ledges, /* flags */
 
		  _class_human_civilian, /* class */	
		  _class_human|_class_assimilated_civilian, /* friends */
		  (_class_hostile_alien^_class_assimilated_civilian)|_class_native, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  _snd_fusion_human_activation, _snd_fusion_kill_the_player, _snd_fusion_human_clear, _snd_fusion_human_trash_talk, _snd_fusion_human_apology, _snd_fusion_human_stop_shooting_me_you_bastard, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  _snd_fusion_human_wail, /* dying flaming */
		  _snd_fusion_human_chatter, 0x1f, /* random sound, random sound mask */

		  _i_plasma_magazine, /* carrying item type */
	
		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -2*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE, /* external velocity scale */
		  _effect_vacuum_civilian_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  10, /* being hit */
		  2, 1, /* dying hard (popping), dying soft (falling) */
		  4, 3, /* hard dead frames, soft dead frames */
		  6, 0, /* stationary shape, moving shape */
		  9, 8, /* teleport in shape, teleport out shape */
		
		  3*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_fusion_bolt_minor, /* ranged attack type */
			  2, /* repetitions */
			  NUMBER_OF_ANGLES/150, /* error angle */
			  13*WORLD_ONE, /* range */
			  5, /* ranged attack shape */
			
			  0, 0, WORLD_ONE*3/4, /* dx, dy, dz */
		  }
		},

		{ /* _civilian_security "steve" */
		  BUILD_COLLECTION(_collection_vacuum_civilian, 2), /* shape collection */
		  30, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_attacks_immediately|_monster_is_omniscent|_monster_cannot_be_dropped|_monster_waits_with_clear_shot|_monster_can_die_in_flames|_monster_uses_sniper_ledges, /* flags */

		  _class_human_civilian, /* class */	
		  _class_human|_class_assimilated_civilian, /* friends */
		  (_class_hostile_alien^_class_assimilated_civilian)|_class_native, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  _snd_fusion_human_activation, _snd_fusion_kill_the_player, _snd_fusion_human_clear, _snd_fusion_human_trash_talk, _snd_fusion_human_apology, _snd_fusion_human_stop_shooting_me_you_bastard, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  _snd_fusion_human_wail, /* dying flaming */
		  _snd_fusion_human_chatter, 0x1f, /* random sound, random sound mask */

		  _i_plasma_pistol, /* carrying item type */
	
		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -2*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE, /* external velocity scale */
		  _effect_vacuum_civilian_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  30*WORLD_ONE, WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  NONE, {NONE, 0, 0, 0}, /* shrapnel radius, shrapnel damage */
		
		  10, /* being hit */
		  2, 1, /* dying hard (popping), dying soft (falling) */
		  4, 3, /* hard dead frames, soft dead frames */
		  6, 0, /* stationary shape, moving shape */
		  9, 8, /* teleport in shape, teleport out shape */
		
		  TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  _projectile_fusion_bolt_minor, /* ranged attack type */
			  5, /* repetitions */
			  NUMBER_OF_ANGLES/150, /* error angle */
			  17*WORLD_ONE, /* range */
			  5, /* ranged attack shape */
			
			  0, 0, WORLD_ONE*3/4, /* dx, dy, dz */
		  }
		},

		{ /* _civilian_assimilated "evil bob" */
		  BUILD_COLLECTION(_collection_vacuum_civilian, 3), /* shape collection */
		  30, 0, 0, /* vitality, immunities, weaknesses */
		  _monster_is_alien|_monster_is_kamakazi|_monster_can_die_in_flames, /* flags */
		
		  _class_assimilated_civilian,
		  _class_pfhor, /* friends */
		  _class_player|_class_defender, /* enemies */
	
		  _normal_frequency, /* sound pitch */	
		  NONE, NONE, NONE, NONE, NONE, _snd_fusion_human_stop_shooting_me_you_bastard, /* sounds: activation, friendly activation, clear, kill, apology, friendly-fire */
		  _snd_fusion_human_wail, /* dying flaming */
		  _snd_assimilated_fusion_human_chatter, 0xf, /* random sound, random sound mask */

		  NONE, /* carrying item type */
	
		  WORLD_ONE/5, (4*WORLD_ONE)/5, /* radius, height */
		  0, /* preferred hover height */
		  -2*WORLD_ONE, WORLD_ONE/3, /* minimum ledge delta, maximum ledge delta */
		  FIXED_ONE, /* external velocity scale */
		  _effect_assimilated_vacuum_civilian_blood_splash, NONE, NONE, /* impact effect, melee impact effect, contrail effect */
	
		  QUARTER_CIRCLE, QUARTER_CIRCLE/3, /* half visual arc, half vertical visual arc */
		  15*WORLD_ONE, WORLD_ONE, /* visual range, dark visual range */
		  _intelligence_high, /* intelligence */
		  _speed_blinding, /* speed */
		  NORMAL_MONSTER_GRAVITY, NORMAL_MONSTER_TERMINAL_VELOCITY, /* gravity, terminal velocity */
		  _vidmaster_door_retry_mask, /* door retry mask */
		  WORLD_ONE, {_damage_explosion, _alien_damage, 80, 40, FIXED_ONE}, /* shrapnel radius, shrapnel damage  */
		
		  10, /* being hit */
		  11, NONE, /* dying hard (popping), dying soft (falling) */
		  4, 0, /* hard dead frames, soft dead frames */
		  6, 0, /* stationary shape, moving shape */
		  8, NONE, /* teleport in shape, teleport out shape */
		
		  2*TICKS_PER_SECOND, /* attack frequency (for both melee and ranged attacks) */
		
		  /* melee attack */
		  {
			  NONE, /* melee attack type */
		  },
		
		  /* ranged attack */
		  {
			  NONE, /* ranged attack type */
		  }
		},
	};

/*
 * physics_models.h
 */

physics_constants physics_models[NUMBER_OF_PHYSICS_MODELS]=
	{
		/* game walking */
		{
			FIXED_ONE/14, FIXED_ONE/17, FIXED_ONE/20, /* max forward, backward and perpendicular velocity */
			FIXED_ONE/200, FIXED_ONE/100, FIXED_ONE/180, /* acceleration, deceleration, airborne deceleration */
			FIXED_ONE/400, FIXED_ONE/300, FIXED_ONE/7, /* gravity, normal acceleration, terminal velocity */
			FIXED_ONE/200, /* external deceleration */
	
			(5*FIXED_ONE)/8, (5*FIXED_ONE)/4, 6*FIXED_ONE, (3*FIXED_ONE)/4, /* angular acceleration, deceleration, max */
			QUARTER_CIRCLE*FIXED_ONE/6, QUARTER_CIRCLE*FIXED_ONE, /* fast angular v, max */
			QUARTER_CIRCLE*FIXED_ONE/3, /* maximum elevation */
			FIXED_ONE/3, /* external angular deceleration */
	
			FIXED_ONE/20, FIXED_ONE/10, /* step delta, step amplitude */
			FIXED_ONE/4, (4*FIXED_ONE)/5, FIXED_ONE/4, (1*FIXED_ONE)/5, /* radius, height, dead height, viewpoint height */
			FIXED_ONE/2, /* splash height */
			FIXED_ONE/32 /* camera separation */
		},

		/* game running */
		{
			FIXED_ONE/8, FIXED_ONE/12, FIXED_ONE/13, /* max forward, backward and perpendicular velocity */
			FIXED_ONE/100, FIXED_ONE/50, FIXED_ONE/180, /* acceleration, deceleration, airborne deceleration */
			FIXED_ONE/400, FIXED_ONE/200, FIXED_ONE/7, /* gravity, normal acceleration, terminal velocity */
			FIXED_ONE/200, /* external deceleration */
	
			(5*FIXED_ONE)/4, (5*FIXED_ONE)/2, 10*FIXED_ONE, (3*FIXED_ONE)/2, /* angular acceleration, deceleration, max */
			QUARTER_CIRCLE*FIXED_ONE/6, QUARTER_CIRCLE*FIXED_ONE, /* fast angular v, max */
			QUARTER_CIRCLE*FIXED_ONE/3, /* maximum elevation */
			FIXED_ONE/3, /* external angular deceleration */
	
			FIXED_ONE/20, FIXED_ONE/10, /* step delta, step amplitude */
			FIXED_ONE/4, (4*FIXED_ONE)/5, FIXED_ONE/4, (1*FIXED_ONE)/5, /* radius, height, dead height, viewpoint height */
			FIXED_ONE/2, /* splash height */
			FIXED_ONE/32 /* camera separation */
		},
	};

/*
 * projectile_definitions.h
 */
projectile_definition projectile_definitions[NUMBER_OF_PROJECTILE_TYPES]=
	{
		{	/* playerÕs rocket */
			_collection_rocket, 0, /* collection number, shape number */
			_effect_rocket_explosion, NONE, /* detonation effect, media_detonation_effect */
			_effect_rocket_contrail, 1, NONE, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */
			
			WORLD_ONE/8, /* radius */
			WORLD_ONE+WORLD_ONE_HALF, /* area-of-effect */
			{_damage_explosion, 0, 250, 50}, /* damage */
		
			_can_toggle_control_panels|_guided, /* flags */
		
			WORLD_ONE/4, /* speed */
			NONE, /* maximum range */
		
			_normal_frequency, /* sound pitch */
			_snd_rocket_flyby, NONE, /* flyby sound, rebound sound */
		},
	
		{	/* playerÕs grenade */
			_collection_rocket, 3, /* collection number, shape number */
			_effect_grenade_explosion, _medium_media_detonation_effect, /* detonation effect, media_detonation_effect */
			_effect_grenade_contrail, 1, 8, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */
		
			0, /* radius */
			WORLD_THREE_FOURTHS, /* area-of-effect */
			{_damage_explosion, 0, 80, 20}, /* damage */
		
			_affected_by_gravity|_can_toggle_control_panels, /* flags */
		
			WORLD_ONE/4, /* speed */
			NONE, /* maximum range */
		
			_normal_frequency, /* sound pitch */
			_snd_grenade_flyby, NONE, /* flyby sound, rebound sound */
		},
	
		{	/* playerÕs pistol bullet */
			NONE, 0, /* collection number, shape number */
			_effect_bullet_ricochet, _small_media_detonation_effect, /* detonation effect, media_detonation_effect */
			NONE, 0, 0, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			0, /* radius */
			0, /* area-of-effect */
			{_damage_projectile, 0, 20, 8}, /* damage */
		
			_bleeding_projectile|_usually_pass_transparent_side, /* flags */
		
			WORLD_ONE, /* speed */
			NONE, /* maximum range */

			_normal_frequency, /* sound pitch */
			NONE, NONE, /* flyby sound, rebound sound */
		},
	
		{	/* playerÕs rifle bullet */
			NONE, 0, /* collection number, shape number */
			_effect_bullet_ricochet, _small_media_detonation_effect, /* detonation effect, media_detonation_effect */
			NONE, 0, 0, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			0, /* radius */
			0, /* area-of-effect */
			{_damage_projectile, 0, 9, 6}, /* damage */
		
			_bleeding_projectile|_usually_pass_transparent_side, /* flags */
		
			WORLD_ONE, /* speed */
			NONE, /* maximum range */

			_normal_frequency, /* sound pitch */
			NONE, NONE, /* flyby sound, rebound sound */
		},

		{	/* playerÕs shotgun bullet */
			NONE, 0, /* collection number, shape number */
			_effect_bullet_ricochet, _small_media_detonation_effect, /* detonation effect, media_detonation_effect */
			NONE, 0, 0, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			0, /* radius */
			0, /* area-of-effect */
			{_damage_shotgun_projectile, 0, 20, 4}, /* damage */
		
			_bleeding_projectile|_can_toggle_control_panels|_usually_pass_transparent_side, /* flags */
		
			WORLD_ONE, /* speed */
			NONE, /* maximum range */

			_normal_frequency, /* sound pitch */
			NONE, NONE, /* flyby sound, rebound sound */
		},

		{	/* electrical melee staff */
			NONE, 0, /* collection number, shape number */
			_effect_fighter_melee_detonation, NONE, /* detonation effect, media_detonation_effect */
			NONE, 0, 0, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			0, /* radius */
			0, /* area-of-effect */
			{_damage_electrical_staff, _alien_damage, 20, 5}, /* damage */
		
			_sometimes_pass_transparent_side|_alien_projectile|_melee_projectile|_penetrates_media, /* flags */
		
			WORLD_ONE_HALF, /* speed */
			WORLD_ONE, /* maximum range */

			_normal_frequency, /* sound pitch */
			NONE, NONE, /* flyby sound, rebound sound */
		},
	
		{	/* electrical melee staff projectile */
			BUILD_COLLECTION(_collection_fighter, 2), 9, /* collection number, shape number */
			_effect_fighter_projectile_detonation, _small_media_detonation_effect, /* detonation effect, media_detonation_effect */
			NONE, 0, 0, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			0, /* radius */
			0, /* area-of-effect */
			{_damage_electrical_staff, _alien_damage, 30, 5}, /* damage */
		
			_sometimes_pass_transparent_side|_alien_projectile, /* flags */
		
			WORLD_ONE/8, /* speed */
			NONE, /* maximum range */

			_normal_frequency, /* sound pitch */
			_snd_fighter_projectile_flyby, NONE, /* flyby sound, rebound sound */
		},
	
		{	/* playerÕs flame thrower burst */
			_collection_rocket, 6, /* collection number, shape number */
			NONE, NONE, /* detonation effect, media_detonation_effect */
			NONE, 0, 0, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */
		
			WORLD_ONE/3, /* radius */
			0, /* area-of-effect */
			{_damage_flame, 0, 8, 4}, /* damage */
		
			_sometimes_pass_transparent_side|_stop_when_animation_loops|_persistent, /* flags */
		
			WORLD_ONE/3, /* speed */
			NONE, /* maximum range */
		
			_normal_frequency, /* sound pitch */
			NONE, NONE, /* flyby sound, rebound sound */
		},

		{	/* _projectile_compiler_bolt_minor */
			BUILD_COLLECTION(_collection_compiler, 0), 4, /* collection number, shape number */
			_effect_compiler_bolt_minor_detonation, _small_media_detonation_effect, /* detonation effect, media_detonation_effect */
			NONE, 0, 0, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			0, /* radius */
			0, /* area-of-effect */
			{_damage_compiler_bolt, _alien_damage, 40, 10}, /* damage */
		
			_sometimes_pass_transparent_side|_alien_projectile, /* flags */
		
			WORLD_ONE/8, /* speed */
			NONE, /* maximum range */

			_normal_frequency, /* sound pitch */
			_snd_compiler_projectile_flyby, NONE, /* flyby sound, rebound sound */
		},

		{	/* _projectile_compiler_bolt_major */
			BUILD_COLLECTION(_collection_compiler, 1), 4, /* collection number, shape number */
			_effect_compiler_bolt_major_detonation, _small_media_detonation_effect, /* detonation effect, media_detonation_effect */
			_effect_compiler_bolt_major_contrail, 0, NONE, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			0, /* radius */
			0, /* area-of-effect */
			{_damage_compiler_bolt, _alien_damage, 40, 10}, /* damage */
		
			_sometimes_pass_transparent_side|_alien_projectile|_guided, /* flags */
		
			WORLD_ONE/12, /* speed */
			NONE, /* maximum range */

			_higher_frequency, /* sound pitch */
			_snd_compiler_projectile_flyby, NONE, /* flyby sound, rebound sound */
		},

		{	/* alien weapon */
			_collection_rocket, 22, /* collection number, shape number */
			_effect_alien_weapon_ricochet, _small_media_detonation_effect, /* detonation effect, media_detonation_effect */
			NONE, 0, 0, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			WORLD_ONE/10, /* radius */
			0, /* area-of-effect */
			{_damage_alien_projectile, _alien_damage, 20, 8}, /* damage */
		
			_usually_pass_transparent_side|_alien_projectile|_can_toggle_control_panels, /* flags */
		
			WORLD_ONE/4, /* speed */
			NONE, /* maximum range */

			_snd_enforcer_projectile_flyby, NONE, /* flyby sound, rebound sound */
		},
	
		{	/* _projectile_fusion_minor */
			_collection_rocket, 11, /* collection number, shape number */
			_effect_minor_fusion_detonation, _small_media_detonation_effect, /* detonation effect, media_detonation_effect */
			NONE, 0, NONE, /* contrail effect, ticks between contrails, maximum contrails */
			_projectile_minor_fusion_dispersal, /* media projectile promotion */

			WORLD_ONE/20, /* radius */
			0, /* area-of-effect */
			{_damage_fusion_bolt, 0, 30, 10}, /* damage */
		
			_usually_pass_transparent_side, /* flags */
		
			WORLD_ONE/4, /* speed */
			NONE, /* maximum range */

			_normal_frequency, /* sound pitch */
			_snd_fusion_flyby, NONE, /* flyby sound, rebound sound */
		},

		{	/* _projectile_fusion_major */
			_collection_rocket, 12, /* collection number, shape number */
			_effect_major_fusion_detonation, _medium_media_detonation_effect, /* detonation effect, media_detonation_effect */
			_effect_major_fusion_contrail, 0, NONE, /* contrail effect, ticks between contrails, maximum contrails */
			_projectile_major_fusion_dispersal, /* media projectile promotion */

			WORLD_ONE/10, /* radius */
			0, /* area-of-effect */
			{_damage_fusion_bolt, 0, 80, 20}, /* damage */
		
			_sometimes_pass_transparent_side|_can_toggle_control_panels, /* flags */
		
			WORLD_ONE/3, /* speed */
			NONE, /* maximum range */

			_higher_frequency, /* sound pitch */
			_snd_fusion_flyby, NONE, /* flyby sound, rebound sound */
		},

		{	/* _projectile_hunter */
			BUILD_COLLECTION(_collection_hunter, 0), 5, /* collection number, shape number */
			_effect_hunter_projectile_detonation, _small_media_detonation_effect, /* detonation effect, media_detonation_effect */
			NONE, 0, NONE, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			0, /* radius */
			0, /* area-of-effect */
			{_damage_hunter_bolt, 0, 15, 5}, /* damage */
		
			_usually_pass_transparent_side|_alien_projectile, /* flags */
		
			WORLD_ONE/4, /* speed */
			NONE, /* maximum range */

			_normal_frequency, /* sound pitch */
			_snd_hunter_projectile_flyby, NONE, /* flyby sound, rebound sound */
		},

		{	/* _projectile_fist */
			NONE, 0, /* collection number, shape number */
			_effect_fist_detonation, NONE, /* detonation effect, media_detonation_effect */
			NONE, 0, 0, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			WORLD_ONE/4, /* radius */
			0, /* area-of-effect */
			{_damage_fist, 0, 50, 10}, /* damage (will be scaled by playerÕs velocity) */
		
			_usually_pass_transparent_side|_can_toggle_control_panels|_melee_projectile|_penetrates_media, /* flags */
		
			(3*WORLD_ONE)/4, /* speed */
			(3*WORLD_ONE)/4, /* maximum range */

			_normal_frequency, /* sound pitch */
			NONE, NONE, /* flyby sound, rebound sound */
		},

		{	/* _projectile_unused */
			0
		},

		{	/* _projectile_armageddon_electricity */
			0
		},

		{ /* _projectile_juggernaut_rocket */
		  _collection_rocket, 0, /* collection number, shape number */
		  _effect_rocket_explosion, _medium_media_detonation_effect, /* detonation effect, media_detonation_effect */
		  _effect_rocket_contrail, 1, NONE, /* contrail effect, ticks between contrails, maximum contrails */
		  NONE, /* media projectile promotion */
		
		  WORLD_ONE/8, /* radius */
		  WORLD_ONE+WORLD_ONE_HALF, /* area-of-effect */
		  {_damage_explosion, _alien_damage, 250, 50}, /* damage */
		
		  _guided|_can_toggle_control_panels, /* flags */
		
		  WORLD_ONE/4, /* speed */
		  NONE, /* maximum range */
		
		  _normal_frequency, /* sound pitch */
		  NONE, NONE, /* flyby sound, rebound sound */
		},

		{ /* _projectile_trooper_bullet */
		  NONE, 0, /* collection number, shape number */
		  _effect_bullet_ricochet, _small_media_detonation_effect, /* detonation effect, media_detonation_effect */
		  NONE, 0, 0, /* contrail effect, ticks between contrails, maximum contrails */
		  NONE, /* media projectile promotion */

		  0, /* radius */
		  0, /* area-of-effect */
		  {_damage_projectile, _alien_damage, 15, 4}, /* damage */
		
		  _bleeding_projectile|_usually_pass_transparent_side, /* flags */
		
		  WORLD_ONE, /* speed */
		  NONE, /* maximum range */

		  _normal_frequency, /* sound pitch */
		  NONE, NONE, /* flyby sound, rebound sound */
		},
	
		{ /* _projectile_trooper_grenade */
		  _collection_trooper, 5, /* collection number, shape number */
		  _effect_grenade_explosion, _medium_media_detonation_effect, /* detonation effect, media_detonation_effect */
		  _effect_grenade_contrail, 1, 8, /* contrail effect, ticks between contrails, maximum contrails */
		  NONE, /* media projectile promotion */
		
		  0, /* radius */
		  WORLD_THREE_FOURTHS, /* area-of-effect */
		  {_damage_explosion, _alien_damage, 40, 20}, /* damage */
		
		  _affected_by_gravity|_can_toggle_control_panels, /* flags */
		
		  WORLD_ONE/5, /* speed */
		  NONE, /* maximum range */
		
		  _normal_frequency, /* sound pitch */
		  NONE, NONE, /* flyby sound, rebound sound */
		},
	
		{ /* _projectile_minor_defender */
		  BUILD_COLLECTION(_collection_defender, 0), 4, /* collection number, shape number */
		  _effect_minor_defender_detonation, _small_media_detonation_effect, /* detonation effect, media_detonation_effect */
		  NONE, 0, 0, /* contrail effect, ticks between contrails, maximum contrails */
		  NONE, /* media projectile promotion */

		  WORLD_ONE/4, /* radius */
		  0, /* area-of-effect */
		  {_damage_defender, 0, 30, 8}, /* damage */
		
		  _usually_pass_transparent_side, /* flags */
		
		  WORLD_ONE/8, /* speed */
		  NONE, /* maximum range */

		  _normal_frequency, /* sound pitch */
		  _snd_defender_flyby, NONE, /* flyby sound, rebound sound */
		},
	
		{ /* _projectile_major_defender */
		  BUILD_COLLECTION(_collection_defender, 1), 4, /* collection number, shape number */
		  _effect_major_defender_detonation, _small_media_detonation_effect, /* detonation effect, media_detonation_effect */
		  NONE, 0, 0, /* contrail effect, ticks between contrails, maximum contrails */
		  NONE, /* media projectile promotion */

		  WORLD_ONE/4, /* radius */
		  0, /* area-of-effect */
		  {_damage_defender, 0, 30, 8}, /* damage */
		
		  _usually_pass_transparent_side|_guided, /* flags */
		
		  WORLD_ONE/6, /* speed */
		  NONE, /* maximum range */

		  _higher_frequency, /* sound pitch */
		  _snd_defender_flyby, NONE, /* flyby sound, rebound sound */
		},

		{ /* _projectile_juggernaut_missile */
		  _collection_juggernaut, 4, /* collection number, shape number */
		  _effect_grenade_explosion, _medium_media_detonation_effect, /* detonation effect, media_detonation_effect */
		  _effect_juggernaut_missile_contrail, 2, NONE, /* contrail effect, ticks between contrails, maximum contrails */
		  NONE, /* media projectile promotion */
		
		  0, /* radius */
		  WORLD_THREE_FOURTHS, /* area-of-effect */
		  {_damage_explosion, _alien_damage, 40, 20}, /* damage */
		
		  _affected_by_half_gravity|_can_toggle_control_panels|_guided|_positive_vertical_error, /* flags */
		
		  WORLD_ONE/5, /* speed */
		  NONE, /* maximum range */
		
		  _normal_frequency, /* sound pitch */
		  NONE, NONE, /* flyby sound, rebound sound */
		},

		{	/* _projectile_minor_energy_drain */
			NONE, 0, /* collection number, shape number */
			NONE, NONE, /* detonation effect, media_detonation_effect */
			NONE, 0, 0, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			WORLD_ONE/8, /* radius */
			0, /* area-of-effect */
			{_damage_energy_drain, 0, 4, 0}, /* damage (will be scaled by playerÕs velocity) */
		
			_melee_projectile|_penetrates_media, /* flags */
		
			(3*WORLD_ONE)/4, /* speed */
			(3*WORLD_ONE)/4, /* maximum range */

			_normal_frequency, /* sound pitch */
			NONE, NONE, /* flyby sound, rebound sound */
		},

		{	/* _projectile_major_energy_drain */
			NONE, 0, /* collection number, shape number */
			NONE, NONE, /* detonation effect, media_detonation_effect */
			NONE, 0, 0, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			WORLD_ONE/8, /* radius */
			0, /* area-of-effect */
			{_damage_energy_drain, 0, 8, 0}, /* damage (will be scaled by playerÕs velocity) */
		
			_melee_projectile|_penetrates_media, /* flags */
		
			(3*WORLD_ONE)/4, /* speed */
			(3*WORLD_ONE)/4, /* maximum range */

			_normal_frequency, /* sound pitch */
			NONE, NONE, /* flyby sound, rebound sound */
		},

		{	/* _projectile_oxygen_drain */
			NONE, 0, /* collection number, shape number */
			NONE, NONE, /* detonation effect, media_detonation_effect */
			NONE, 0, 0, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			WORLD_ONE/8, /* radius */
			0, /* area-of-effect */
			{_damage_oxygen_drain, 0, 4, 0}, /* damage (will be scaled by playerÕs velocity) */
		
			_melee_projectile|_penetrates_media, /* flags */
		
			(3*WORLD_ONE)/4, /* speed */
			(3*WORLD_ONE)/4, /* maximum range */

			_normal_frequency, /* sound pitch */
			NONE, NONE, /* flyby sound, rebound sound */
		},

		{	/* _projectile_hummer_slow */
			BUILD_COLLECTION(_collection_hummer, 0), 5, /* collection number, shape number */
			_effect_minor_hummer_projectile_detonation, _small_media_detonation_effect, /* detonation effect, media_detonation_effect */
			NONE, 0, NONE, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			0, /* radius */
			0, /* area-of-effect */
			{_damage_hummer_bolt, 0, 15, 5}, /* damage */
		
			_usually_pass_transparent_side|_alien_projectile, /* flags */
		
			WORLD_ONE/8, /* speed */
			NONE, /* maximum range */

			_normal_frequency, /* sound pitch */
			_snd_hummer_projectile_flyby, NONE, /* flyby sound, rebound sound */
		},

		{	/* _projectile_hummer_fast */
			BUILD_COLLECTION(_collection_hummer, 1), 5, /* collection number, shape number */
			_effect_major_hummer_projectile_detonation, _small_media_detonation_effect, /* detonation effect, media_detonation_effect */
			NONE, 0, NONE, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			0, /* radius */
			0, /* area-of-effect */
			{_damage_hummer_bolt, 0, 15, 5}, /* damage */
		
			_usually_pass_transparent_side|_alien_projectile, /* flags */
		
			WORLD_ONE/6, /* speed */
			NONE, /* maximum range */

			_higher_frequency, /* sound pitch */
			_snd_hummer_projectile_flyby, NONE, /* flyby sound, rebound sound */
		},

		{	/* _projectile_hummer_durandal */
			BUILD_COLLECTION(_collection_hummer, 1), 5, /* collection number, shape number */
			_effect_durandal_hummer_projectile_detonation, _small_media_detonation_effect, /* detonation effect, media_detonation_effect */
			NONE, 0, NONE, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			0, /* radius */
			0, /* area-of-effect */
			{_damage_hummer_bolt, 0, 15, 5}, /* damage */
		
			_guided|_usually_pass_transparent_side|_alien_projectile, /* flags */
		
			WORLD_ONE/8, /* speed */
			NONE, /* maximum range */

			_lower_frequency, /* sound pitch */
			_snd_hummer_projectile_flyby, NONE, /* flyby sound, rebound sound */
		},

		{	/* _projectile_minor_cyborg_ball */
			BUILD_COLLECTION(_collection_cyborg, 0), 6, /* collection number, shape number */
			_effect_grenade_explosion, _medium_media_detonation_effect, /* detonation effect, media_detonation_effect */
			_effect_grenade_contrail, 1, 8, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			WORLD_ONE/8, /* radius */
			WORLD_ONE, /* area-of-effect */
			{_damage_explosion, 0, 20, 10}, /* damage */
		
			_can_toggle_control_panels|_sometimes_pass_transparent_side|_alien_projectile|_rebounds_from_floor|_doubly_affected_by_gravity, /* flags */
		
			WORLD_ONE/10, /* speed */
			NONE, /* maximum range */

			_normal_frequency, /* sound pitch */
			_snd_cyborg_projectile_flyby, _snd_cyborg_projectile_bounce, /* flyby sound, rebound sound */
		},

		{	/* _projectile_major_cyborg_ball */
			BUILD_COLLECTION(_collection_cyborg, 1), 6, /* collection number, shape number */
			_effect_grenade_explosion, _medium_media_detonation_effect, /* detonation effect, media_detonation_effect */
			_effect_grenade_contrail, 1, 8, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			WORLD_ONE/8, /* radius */
			WORLD_ONE, /* area-of-effect */
			{_damage_explosion, 0, 40, 10}, /* damage */
		
			_guided|_can_toggle_control_panels|_sometimes_pass_transparent_side|_alien_projectile|_rebounds_from_floor|_doubly_affected_by_gravity, /* flags */
		
			WORLD_ONE/8, /* speed */
			NONE, /* maximum range */

			_lower_frequency, /* sound pitch */
			_snd_cyborg_projectile_flyby, _snd_cyborg_projectile_bounce, /* flyby sound, rebound sound */
		},

		{	/* _projectile_ball */
			BUILD_COLLECTION(_collection_player, 0), 0, /* collection number, shape number */
			NONE, NONE, /* detonation effect, media_detonation_effect */
			NONE, 1, 8, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			WORLD_ONE/4, /* radius */
			NONE, /* area-of-effect */
			{NONE, 0, 40, 10}, /* damage */
		
			_persistent_and_virulent|_penetrates_media|_becomes_item_on_detonation|_can_toggle_control_panels|_rebounds_from_floor|_doubly_affected_by_gravity|_penetrates_media, /* flags */
		
			0, /* speed */
			NONE, /* maximum range */

			_normal_frequency, /* sound pitch */
			NONE, _snd_ball_bounce, /* flyby sound, rebound sound */
		},

		{	/* _projectile_minor_fusion_dispersal */
			_collection_rocket, 11, /* collection number, shape number */
			_effect_minor_fusion_dispersal, NONE, /* detonation effect, media_detonation_effect */
			NONE, 0, NONE, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			WORLD_ONE/20, /* radius */
			WORLD_ONE, /* area-of-effect */
			{_damage_fusion_bolt, 0, 30, 10}, /* damage */
		
			0, /* flags */
		
			WORLD_ONE/4, /* speed */
			NONE, /* maximum range */

			_normal_frequency, /* sound pitch */
			_snd_fusion_flyby, NONE, /* flyby sound, rebound sound */
		},

		{	/* _projectile_major_fusion_dispersal */
			_collection_rocket, 12, /* collection number, shape number */
			_effect_major_fusion_dispersal, NONE, /* detonation effect, media_detonation_effect */
			NONE, 0, NONE, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			WORLD_ONE/10, /* radius */
			2*WORLD_ONE, /* area-of-effect */
			{_damage_fusion_bolt, 0, 80, 20}, /* damage */
		
			0, /* flags */
		
			WORLD_ONE/3, /* speed */
			NONE, /* maximum range */

			_higher_frequency, /* sound pitch */
			_snd_fusion_flyby, NONE, /* flyby sound, rebound sound */
		},

		{	/* _projectile_overloaded_fusion_dispersal */
			_collection_rocket, 12, /* collection number, shape number */
			_effect_overloaded_fusion_dispersal, NONE, /* detonation effect, media_detonation_effect */
			NONE, 0, NONE, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			WORLD_ONE/10, /* radius */
			4*WORLD_ONE, /* area-of-effect */
			{_damage_fusion_bolt, 0, 500, 0}, /* damage */
		
			0, /* flags */
		
			WORLD_ONE/3, /* speed */
			NONE, /* maximum range */

			_lower_frequency, /* sound pitch */
			_snd_fusion_flyby, NONE, /* flyby sound, rebound sound */
		},

		{	/* _projectile_yeti */
			NONE, 0, /* collection number, shape number */
			_effect_yeti_melee_detonation, NONE, /* detonation effect, media_detonation_effect */
			NONE, 0, 0, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			0, /* radius */
			0, /* area-of-effect */
			{_damage_yeti_claws, _alien_damage, 20, 5}, /* damage */
		
			_sometimes_pass_transparent_side|_alien_projectile|_melee_projectile|_penetrates_media, /* flags */
		
			WORLD_ONE_HALF, /* speed */
			WORLD_ONE, /* maximum range */

			_normal_frequency, /* sound pitch */
			NONE, NONE, /* flyby sound, rebound sound */
		},
	
		{	/* _projectile_sewage_yeti */
			BUILD_COLLECTION(_collection_yeti, 0), 10, /* collection number, shape number */
			_effect_sewage_yeti_projectile_detonation, _small_media_detonation_effect, /* detonation effect, media_detonation_effect */
			NONE, 0, NONE, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			0, /* radius */
			0, /* area-of-effect */
			{_damage_yeti_projectile, 0, 15, 5}, /* damage */
		
			_usually_pass_transparent_side|_alien_projectile|_affected_by_half_gravity, /* flags */
		
			WORLD_ONE/8, /* speed */
			NONE, /* maximum range */

			_normal_frequency, /* sound pitch */
			_snd_yeti_projectile_sewage_flyby, NONE, /* flyby sound, rebound sound */
		},

		{	/* _projectile_lava_yeti */
			BUILD_COLLECTION(_collection_yeti, 2), 6, /* collection number, shape number */
			_effect_lava_yeti_projectile_detonation, _small_media_detonation_effect, /* detonation effect, media_detonation_effect */
			NONE, 0, NONE, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			0, /* radius */
			0, /* area-of-effect */
			{_damage_flame, 0, 30, 10}, /* damage */
		
			_usually_pass_transparent_side|_alien_projectile, /* flags */
		
			WORLD_ONE/8, /* speed */
			NONE, /* maximum range */

			_normal_frequency, /* sound pitch */
			_snd_yeti_projectile_lava_flyby, NONE, /* flyby sound, rebound sound */
		},

		{	/* _projectile_smg_bullet */
			NONE, 0, /* collection number, shape number */
			_effect_bullet_ricochet, _small_media_detonation_effect, /* detonation effect, media_detonation_effect */
			NONE, 0, 0, /* contrail effect, ticks between contrails, maximum contrails */
			NONE, /* media projectile promotion */

			0, /* radius */
			0, /* area-of-effect */
			{_damage_projectile, 0, 9, 6}, /* damage */
		
			_bleeding_projectile|_usually_pass_transparent_side|_projectile_passes_media_boundary, /* flags */
		
			WORLD_ONE, /* speed */
			NONE, /* maximum range */

			_normal_frequency, /* sound pitch */
			NONE, NONE, /* flyby sound, rebound sound */
		},
	};


/*
 * weapon_definitions.h
 */

weapon_definition weapon_definitions[]=
	{
		/* Fist*/
		{
			/* item type, powerup type, item class, item flags */
			_i_knife, NONE, _melee_class, _weapon_fires_under_media,

			FIXED_ONE_HALF, 0, /* firing intensity, firing decay */

			/* idle height, bob amplitude, kick height, reload height */
			FIXED_ONE+FIXED_ONE/15, FIXED_ONE/15, FIXED_ONE/16, 0,
		
			/* horizontal positioning.. */
			FIXED_ONE_HALF, 0,

			/* collection, idle, firing, reloading shapes; shell casing, charging, charged */
			_weapon_in_hand_collection,
			_fist_idle, _fist_punching, _fist_idle,
			NONE,
			NONE, NONE,

			/* ready/await/load/finish/powerup rounds ticks */
			TICKS_PER_SECOND/4, 0, 0, 0, 0,

			{
				{
					/* rounds per magazine */
					1, 
	
					/* Ammunition type */
					NONE, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					NONE, TICKS_PER_SECOND/3, 0,
				
					/* recoil magnitude */
					0,
				
					/* firing, click, charging, shell casing sound, reloading sound */
					NONE, NONE, NONE, NONE, NONE, NONE,
				
					/* projectile type */
					_projectile_fist,
				
					/* theta error */
					0,
				
					/* dx, dz */
					0, 0,
				
					/* shell casing type */
					NONE, 

					/* burst count */
					0
				},
				{
					/* rounds per magazine */
					1, 
	
					/* Ammunition type */
					NONE, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					NONE, TICKS_PER_SECOND/3, 0,
				
					/* recoil magnitude */
					0,
				
					/* firing, click, charging, shell casing, reload sound */
					NONE, NONE, NONE, NONE, NONE, NONE,
				
					/* projectile type */
					_projectile_fist,
				
					/* theta error */
					0,
				
					/* dx, dz */
					0, 0,
				
					/* shell casing type */
					NONE, 

					/* burst count */
					0
				}
			}
		},
	
		/* Magnum .45 "mega class"- dual fisted */
		{
			/* item type, powerup type, item class, item flags */
			_i_magnum, NONE, _twofisted_pistol_class, _weapon_fires_out_of_phase,

			3*FIXED_ONE/4, TICKS_PER_SECOND/8, /* firing intensity, firing decay */

			/* idle height, bob amplitude, kick height, reload height */
			FIXED_ONE+FIXED_ONE/15, FIXED_ONE/25, FIXED_ONE/8, FIXED_ONE,
		
			/* horizontal positioning.. */
			FIXED_ONE_HALF, 0,

			/* collection, idle, firing, reloading shapes; shell casing, charging, charged */
			_weapon_in_hand_collection,
			_pistol_idle, _pistol_firing, _pistol_reloading,
			NONE,
			NONE, NONE,

			/* ready/await/load/finish/powerup rounds ticks */
			TICKS_PER_SECOND/3, 5, 5, 5, 0, // was NONE

			{
				{
					/* rounds per magazine */
					8, 
	
					/* Ammunition type */
					_i_magnum_magazine, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					NONE, TICKS_PER_SECOND/3, 0,
				
					/* recoil magnitude */
					10,
				
					/* firing, click, charging, shell casing, reload sound */
					_snd_magnum_firing, _snd_empty_gun, NONE, NONE, _snd_magnum_reloading, NONE,
				
					/* projectile type */
					_projectile_pistol_bullet,
				
					/* theta error */
					1,
				
					/* dx, dz */
					(WORLD_ONE_FOURTH/6), -NORMAL_WEAPON_DZ,		/* Primary */
				
					/* shell casing type */
					_shell_casing_pistol, 

					/* burst count */
					0
				},
			
				/* left weapon (for consistency)... */
				{
					/* rounds per magazine */
					8, 
	
					/* Ammunition type */
					_i_magnum_magazine, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					NONE, TICKS_PER_SECOND/3, 0,
				
					/* recoil magnitude */
					10,
				
					/* firing, click, charging, shell casing, reload sound */
					_snd_magnum_firing, _snd_empty_gun, NONE, NONE, _snd_magnum_reloading, NONE,
				
					/* projectile type */
					_projectile_pistol_bullet,
				
					/* theta error */
					1,
				
					/* dx, dz */
					-(WORLD_ONE_FOURTH/6), -NORMAL_WEAPON_DZ,		/* Primary */
				
					/* shell casing type */
					_shell_casing_pistol,

					/* burst count */
					0
				}
			}
		},

		/* Fusion Pistol */
		{
			/* item type, powerup type, item class, item flags */
			_i_plasma_pistol, NONE, _dual_function_class, _weapon_overloads | _weapon_fires_under_media | _weapon_triggers_share_ammo,

			3*FIXED_ONE/4, TICKS_PER_SECOND/3, /* firing intensity, firing decay */

			/* idle height, bob amplitude, kick height, reload height */
			FIXED_ONE, FIXED_ONE/25, FIXED_ONE/8, FIXED_ONE,
		
			/* horizontal positioning.. */
			FIXED_ONE_HALF, 0,

			/* collection, idle, firing, reloading shapes; shell casing, charging, charged */
			_weapon_in_hand_collection,
			_fusion_idle, _fusion_firing, NONE,
			NONE,
			NONE, _fusion_charged,

			/* ready/await/load/finish/powerup rounds ticks */
			TICKS_PER_SECOND/3, TICKS_PER_SECOND/2, TICKS_PER_SECOND/2, TICKS_PER_SECOND/2, 0,

			{
				{
					/* rounds per magazine */
					20, 
	
					/* Ammunition type */
					_i_plasma_magazine, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					NONE, TICKS_PER_SECOND/6, 0,
				
					/* recoil magnitude */
					5,
				
					/* firing, click, charging, shell casing, reload sound */
					_snd_fusion_firing, _snd_empty_gun, _snd_fusion_charging, NONE, NONE, NONE,
				
					/* projectile type */
					_projectile_fusion_bolt_minor,
				
					/* theta error */
					1,
				
					/* dx, dz */
					0, -4*NORMAL_WEAPON_DZ,
				
					/* shell casing type */
					NONE, 

					/* burst count */
					0
				},
				{
					/* rounds per magazine */
					20,  // this should not be used...
	
					/* Ammunition type */
					_i_plasma_magazine, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					5, 4, TICKS_PER_SECOND/2,
				
					/* recoil magnitude */
					20,
				
					/* firing, click, charging, shell casing, reload sound */
					_snd_major_fusion_firing, _snd_empty_gun, _snd_fusion_charging, NONE, NONE, _snd_major_fusion_charged,
				
					/* projectile type */
					_projectile_fusion_bolt_major,
				
					/* theta error */
					1,
				
					/* dx, dz */
					0, 0,
				
					/* shell casing type */
					NONE, 

					/* burst count */
					0
				}
			}
		},

		/* Assault Rifle */
		{
			/* item type, powerup type, item class, item flags */
			_i_assault_rifle, NONE, _multipurpose_class, _weapon_is_automatic,

			3*FIXED_ONE/4, TICKS_PER_SECOND/5, /* firing intensity, firing decay */


			/* idle height, bob amplitude, kick height, reload height */
			FIXED_ONE+FIXED_ONE/6, FIXED_ONE/35, FIXED_ONE/16, 3*FIXED_ONE/4,

			/* horizontal positioning.. */
			FIXED_ONE_HALF, 0,
		
			/* collection, idle, firing, reloading shapes; shell casing, charging, charged */
			_weapon_in_hand_collection,
			_assault_rifle_idle, _assault_rifle_firing, _assault_rifle_reloading,
			NONE,
			NONE, NONE,

			/* ready/await/load/finish/powerup rounds ticks */
			TICKS_PER_SECOND/2, TICKS_PER_SECOND/3, TICKS_PER_SECOND/3, TICKS_PER_SECOND/3, 0,

			{
				{
					/* rounds per magazine */
					52, 
	
					/* Ammunition type */
					_i_assault_rifle_magazine, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					NONE, 0, 0,
				
					/* recoil magnitude */
					5,
				
					/* firing, click, charging, shell casing, reload sound */
					_snd_assault_rifle_firing, _snd_empty_gun, NONE, _snd_assault_rifle_shell_casings, _snd_assault_rifle_reloading, NONE,
				
					/* projectile type */
					_projectile_rifle_bullet,
				
					/* theta error */
					10,
				
					/* dx, dz */
					0, -NORMAL_WEAPON_DZ,
				
					/* shell casing type */
					_shell_casing_assault_rifle,

					/* burst count */
					0
				},
				{
					/* rounds per magazine */
					7, 
	
					/* Ammunition type */
					_i_assault_grenade_magazine, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					TICKS_PER_SECOND/6, (3*TICKS_PER_SECOND)/4 - TICKS_PER_SECOND/6, 0,
				
					/* recoil magnitude */
					40,
				
					/* firing, click, charging, shell casing, reload sound */
					_snd_grenade_launcher_firing, _snd_empty_gun, NONE, NONE, NONE, NONE,
				
					/* projectile type */
					_projectile_grenade,
				
					/* theta error */
					0,
				
					/* dx, dz */
					0, -5*NORMAL_WEAPON_DZ,
				
					/* shell casing type */
					NONE, 

					/* burst count */
					0
				}
			}
		},

		/* Rocket Launcher */
		{
			/* item type, powerup_type, item class, item flags */
			_i_missile_launcher, NONE, _normal_class, _no_flags,

			FIXED_ONE, TICKS_PER_SECOND/3, /* firing intensity, firing decay */

			/* idle height, bob amplitude, kick height, reload height */
			(3*FIXED_ONE)/4, FIXED_ONE/50, FIXED_ONE/20, FIXED_ONE,
		
			/* horizontal positioning.. */
			(FIXED_ONE_HALF/4), 0,

			/* collection, idle, firing, reloading shapes; shell casing, charging, charged */
			_weapon_in_hand_collection,
			_missile_launcher_idle, _missile_launcher_firing, NONE,
			NONE,
			NONE, NONE,

			/* ready/await/load/finish/powerup rounds ticks */
			TICKS_PER_SECOND, TICKS_PER_SECOND, TICKS_PER_SECOND, TICKS_PER_SECOND, 0,

			{
				{
					/* rounds per magazine */
					2, 
	
					/* Ammunition type */
					_i_missile_launcher_magazine, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					TICKS_PER_SECOND/2, TICKS_PER_SECOND/10, 0,
				
					/* recoil magnitude */
					100,
				
					/* firing, click, charging, shell casing, reload sound */
					_snd_rocket_firing, _snd_empty_gun, NONE, NONE, NONE, NONE,
				
					/* projectile type */
					_projectile_rocket,
				
					/* theta error */
					0,
				
					/* dx, dz */
					-WORLD_ONE_FOURTH, 0,
				
					/* shell casing type */
					NONE, 

					/* burst count */
					0
				},
			
				/* unused */
				{
					/* rounds per magazine */
					1, 
	
					/* Ammunition type */
					NONE, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					NONE, TICKS_PER_SECOND/3, 0,
				
					/* recoil magnitude */
					0,
				
					/* firing, click, charging, shell casing, reload sound */
					NONE, NONE, NONE, NONE, NONE, NONE,
				
					/* projectile type */
					_projectile_fist,
				
					/* theta error */
					0,
				
					/* dx, dz */
					0, 0,
				
					/* shell casing type */
					NONE, 

					/* burst count */
					0
				}
			}
		},

		/* flamethrower */
		{
			/* item type, powerup type, item class, item flags */
			_i_flamethrower, NONE, _normal_class, _weapon_is_automatic,

			3*FIXED_ONE/4, TICKS_PER_SECOND/3, /* firing intensity, firing decay */

			/* idle height, bob amplitude, kick height, reload height */
			FIXED_ONE, FIXED_ONE/35, FIXED_ONE/15, FIXED_ONE/2,
		
			/* horizontal positioning.. */
			FIXED_ONE_HALF, 0,

			/* collection, idle, firing, reloading shapes; shell casing, charging, charged */
			_weapon_in_hand_collection,
			_flamethrower_idle, _flamethrower_firing, NONE,
			NONE,
			NONE, NONE,

			/* ready/await/load/finish/powerup rounds ticks */
			TICKS_PER_SECOND, (2*TICKS_PER_SECOND)/3, (2*TICKS_PER_SECOND)/3, (2*TICKS_PER_SECOND)/3, 0,

			{
				{
					/* rounds per magazine */
					7*TICKS_PER_SECOND, 
	
					/* Ammunition type */
					_i_flamethrower_canister, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					NONE, TICKS_PER_SECOND/3, 0,
				
					/* recoil magnitude */
					2,
				
					/* firing, click, charging, shell casing, reload sound */
					_snd_flamethrower, NONE, NONE, NONE, NONE, NONE,
				
					/* projectile type */
					_projectile_flamethrower_burst,
				
					/* theta error */
					0,
				
					/* dx, dz */
					20, -50,
				
					/* shell casing type */
					NONE, 

					/* burst count */
					0
				},
			
				/* unused */
				{
					/* rounds per magazine */
					1, 
	
					/* Ammunition type */
					NONE, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					NONE, TICKS_PER_SECOND/3, 0,
				
					/* recoil magnitude */
					0,
				
					/* firing, click, charging, shell casing, reload sound */
					NONE, NONE, NONE, NONE, NONE, NONE,
				
					/* projectile type */
					_projectile_fist,
				
					/* theta error */
					0,
				
					/* dx, dz */
					0, 0,
				
					/* shell casing type */
					NONE, 

					/* burst count */
					0
				}
			}
		},

		/* alien weapon */
		{
			/* item type, powerup type, item class, item flags */
			_i_alien_shotgun, NONE, _multipurpose_class, _weapon_is_automatic | _weapon_disappears_after_use | _weapon_has_random_ammo_on_pickup | _weapon_triggers_share_ammo | _weapon_secondary_has_angular_flipping,

			3*FIXED_ONE/4, TICKS_PER_SECOND/5, /* firing intensity, firing decay */

			/* idle height, bob amplitude, kick height, reload height */
			FIXED_ONE+FIXED_ONE/6, FIXED_ONE/35, FIXED_ONE/16, 3*FIXED_ONE/4,

			/* horizontal positioning.. */
			FIXED_ONE_HALF, 0,
		
			/* collection, idle, firing, reloading shapes; shell casing, charging, charged */
			_weapon_in_hand_collection,
			_alien_weapon_idle, _alien_weapon_firing, NONE,
			NONE,
			NONE, NONE,

			/* ready/await/load/finish/powerup rounds ticks */
			TICKS_PER_SECOND/2, TICKS_PER_SECOND/3, TICKS_PER_SECOND/3, TICKS_PER_SECOND/3, 0,

			{
				{
					/* rounds per magazine */
					2000, 
	
					/* Ammunition type */
					_i_alien_shotgun_magazine, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					NONE, 0, 0,
				
					/* recoil magnitude */
					5,
				
					/* firing, click, charging, shell casing, reload sound */
					_snd_enforcer_attack, NONE, NONE, NONE, NONE, NONE,
				
					/* projectile type */
					_projectile_alien_weapon,
				
					/* theta error */
					1,
				
					/* dx, dz */
					0, -8*NORMAL_WEAPON_DZ,
				
					/* shell casing type */
					NONE,

					/* burst count */
					0
				},

				{
					/* rounds per magazine */
					50, 
	
					/* Ammunition type */
					_i_alien_shotgun_magazine, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					NONE, 0, 0,
				
					/* recoil magnitude */
					5,
				
					/* firing, click, charging, shell casing, reload sound */
					_snd_enforcer_attack, NONE, NONE, NONE, NONE, NONE,
				
					/* projectile type */
					_projectile_alien_weapon,
				
					/* theta error */
					1,
				
					/* dx, dz */
					0, -8*NORMAL_WEAPON_DZ,
				
					/* shell casing type */
					NONE,

					/* burst count */
					0
				}
			}
		},

		#define SHOTGUN_BURST_COUNT 10
#define SHOTGUN_SPREAD 5
		/* Shotgun- dual fisted */
		{
			/* item type, powerup type, item class, item flags */
			_i_shotgun, NONE, _twofisted_pistol_class, _weapon_reloads_in_one_hand,

			3*FIXED_ONE/4, TICKS_PER_SECOND/8, /* firing intensity, firing decay */

			/* idle height, bob amplitude, kick height, reload height */
			FIXED_ONE+FIXED_ONE/15, FIXED_ONE/25, FIXED_ONE/8, FIXED_ONE,
		
			/* horizontal positioning.. */
			FIXED_ONE_HALF, 0,

			/* collection, idle, firing, reloading shapes; shell casing, charging, charged */
			_weapon_in_hand_collection,
			_shotgun_idle, _shotgun_firing, _shotgun_reloading,
			NONE,
			NONE, NONE,

			/* ready/await/load/finish/powerup rounds ticks */
			TICKS_PER_SECOND/3, 5, 5, 5, 0, // was NONE

			{
				{
					/* rounds per magazine */
					SHOTGUN_BURST_COUNT, 
	
					/* Ammunition type */
					_i_shotgun_magazine, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					NONE, TICKS_PER_SECOND/3, 0,
				
					/* recoil magnitude */
					25,
				
					/* firing, click, charging, shell casing, reload sound */
					_snd_shotgun_firing, _snd_empty_gun, NONE, NONE, _snd_shotgun_reloading, NONE,
				
					/* projectile type */
					_projectile_shotgun_bullet,
				
					/* theta error */
					SHOTGUN_SPREAD,
				
					/* dx, dz */
					(WORLD_ONE_FOURTH/6), -NORMAL_WEAPON_DZ,		/* Primary */
				
					/* shell casing type */
					NONE, 

					/* burst count */
					SHOTGUN_BURST_COUNT
				},
			
				/* left weapon (for consistency)... */
				{
					/* rounds per magazine */
					SHOTGUN_BURST_COUNT, 
	
					/* Ammunition type */
					_i_shotgun_magazine, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					NONE, TICKS_PER_SECOND/3, 0,
				
					/* recoil magnitude */
					25,
				
					/* firing, click, charging, shell casing, reload sound */
					_snd_shotgun_firing, _snd_empty_gun, NONE, NONE, _snd_shotgun_reloading, NONE,
				
					/* projectile type */
					_projectile_shotgun_bullet,
				
					/* theta error */
					SHOTGUN_SPREAD,
				
					/* dx, dz */
					(WORLD_ONE_FOURTH/6), -NORMAL_WEAPON_DZ,		/* Primary */
				
					/* shell casing type */
					NONE, 

					/* burst count */
					SHOTGUN_BURST_COUNT
				}
			}
		},
	
		/* The Ball- Don't Drop It. */
		{
			/* item type, powerup type, item class, item flags */
			_i_red_ball, NONE, _normal_class, 0,

			0, 0, /* firing intensity, firing decay */

			/* idle height, bob amplitude, kick height, reload height */
			FIXED_ONE+FIXED_ONE/15, FIXED_ONE/15, FIXED_ONE/16, 0,
		
			/* horizontal positioning.. */
			FIXED_ONE_HALF, 0,

			/* collection, idle, firing, reloading shapes; shell casing, charging, charged */
			_collection_player,
			30, 30, 30,
			NONE,
			NONE, NONE,

			/* ready/await/load/finish/powerup rounds ticks */
			TICKS_PER_SECOND/4, 0, 0, 0, 0,

			{
				{
					/* rounds per magazine */
					1, 
	
					/* Ammunition type */
					NONE, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					NONE, TICKS_PER_SECOND/3, 0,
				
					/* recoil magnitude */
					0,
				
					/* firing, click, charging, shell casing, reload sound */
					NONE, NONE, NONE, NONE, NONE, NONE,
				
					/* projectile type */
					_projectile_ball_dropped,
				
					/* theta error */
					0,
				
					/* dx, dz */
					0, -150,		/* Primary */

					/* shell casing type */
					NONE, 
				
					/* burst count */
					0
				},
			
				{
					/* rounds per magazine */
					1, 
	
					/* Ammunition type */
					NONE, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					NONE, TICKS_PER_SECOND/3, 0,
				
					/* recoil magnitude */
					0,
				
					/* firing, click, charging, shell casing, reload sound */
					NONE, NONE, NONE, NONE, NONE, NONE,
				
					/* projectile type */
					_projectile_ball_dropped,
				
					/* theta error */
					0,
				
					/* dx, dz */
					0, 0,		/* Primary */

					/* shell casing type */
					NONE, 
				
					/* burst count */
					0
				}
			}
		},

		/* The New SMG. */
		{
			/* item type, powerup type, item class, item flags */
			_i_smg, NONE, _normal_class, _weapon_is_automatic | _weapon_fires_under_media,

			3*FIXED_ONE/4, TICKS_PER_SECOND/5, /* firing intensity, firing decay */

			/* idle height, bob amplitude, kick height, reload height */
			FIXED_ONE+FIXED_ONE/6, FIXED_ONE/35, FIXED_ONE/16, 3*FIXED_ONE/4,

			/* horizontal positioning.. */
			FIXED_ONE_HALF, 0,
		
			/* collection, idle, firing, reloading shapes; shell casing, charging, charged */
			_weapon_in_hand_collection,
			_smg_idle, _smg_firing, _smg_reloading,	
		
			NONE,
			NONE, NONE,

			/* ready/await/load/finish/powerup rounds ticks */
			TICKS_PER_SECOND/2, TICKS_PER_SECOND/3, TICKS_PER_SECOND/3, TICKS_PER_SECOND/3, 0,

			{
				{
					/* rounds per magazine */
					32, 
	
					/* Ammunition type */
					_i_smg_ammo, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					NONE, 0, 0,
				
					/* recoil magnitude */
					5,
				
					/* firing, click, charging, shell casing, reload sound */
					_snd_smg_firing, _snd_empty_gun, NONE, _snd_assault_rifle_shell_casings, _snd_smg_reloading, NONE,
				
					/* projectile type */
					_projectile_smg_bullet,
				
					/* theta error */
					3,
				
					/* dx, dz */
					0, -NORMAL_WEAPON_DZ,
				
					/* shell casing type */
					_shell_casing_smg,

					/* burst count */
					2
				},
				{
					/* rounds per magazine */
					32, 
	
					/* Ammunition type */
					_i_smg_ammo, 
				
					/* Ticks per round, recovery ticks, charging ticks */
					NONE, 0, 0,
				
					/* recoil magnitude */
					5,
				
					/* firing, click, charging, shell casing, reload sound */
					_snd_smg_firing, _snd_empty_gun, NONE, _snd_assault_rifle_shell_casings, _snd_smg_reloading, NONE,
				
					/* projectile type */
					_projectile_smg_bullet,
				
					/* theta error */
					3,
				
					/* dx, dz */
					0, -NORMAL_WEAPON_DZ,
				
					/* shell casing type */
					_shell_casing_smg,

					/* burst count */
					2
				}
			}
		}
	};

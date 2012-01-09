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

#include "DefaultNames.h"

#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <wx/stdpaths.h>

static const wxString alienNames[] = {
	_("Marine"),
	_("Minor Tick"),
	_("Major Tick"),
	_("Kamikaze Tick"),
	_("Minor S'pht"),
	_("Major S'pht"),
	_("Minor Invisible S'pht"),
	_("Major Invisible S'pht"),
	_("Minor Pfhor"),
	_("Major Pfhor"),
	_("Minor Projectile Pfhor"),
	_("Major Projectile Pfhor"),
	_("Green Bob"),
	_("Blue Bob"),
	_("Security Bob"),
	_("Assimilated Bob"),
	_("Minor Drone"),
	_("Major Drone"),
	_("Big Minor Drone"),
	_("Big Angry Drone"),
	_("Possessed Drone"),
	_("Minor Cyborg"),
	_("Major Cyborg"),
	_("Minor Flame Cyborg"),
	_("Major Flame Cyborg"),
	_("Minor Enforcer"),
	_("Major Enforcer"),
	_("Minor Hunter"),
	_("Major Hunter"),
	_("Minor Trooper"),
	_("Major Trooper"),
	_("Mother of All Cyborgs"),
	_("Mother of All Hunters"),
	_("Sewage F'lickta"),
	_("Water F'lickta"),
	_("Lava F'lickta"),
	_("Minor S'pht'Kr"),
	_("Major S'pht'Kr"),
	_("Minor Juggernaut"),
	_("Major Juggernaut"),
	_("Tiny P'fhor"),
	_("Tiny Bob"),
	_("Tiny Flick'ta"),
	_("Green VacBob"),
	_("Blue VacBob"),
	_("Security VacBob"),
	_("Assimilated VacBob"),
};

static const wxString collectionNames[] = {
	_("Interface graphics"),
	_("Weapons in hand"),
	_("Juggernaut"),
	_("Tick"),
	_("Projectiles & explosions"),
	_("Hunter"),
	_("Player"),
	_("Items"),
	_("Trooper"),
	_("Pfhor fighter"),
	_("Defender"),
	_("Yeti"),
	_("Bob"),
	_("Vacuum Bob"),
	_("Enforcer"),
	_("Hummer"),
	_("Compiler"),
	_("Walls 1 (water)"),
	_("Walls 2 (lava)"),
	_("Walls 3 (sewage)"),
	_("Walls 4 (jiaro)"),
	_("Walls 5 (pfhor)"),
	_("Scenery 1 (water)"),
	_("Scenery 2 (lava)"),
	_("Scenery 3 (sewage)"),
	_("Scenery 4 (jiaro)"),
	_("Scenery 5 (pfhor)"),
	_("Landscape 1 (daytime)"),
	_("Landscape 2 (nighttime)"),
	_("Landscape 3 (moon)"),
	_("Landscape 4 (space)"),
	_("Cyborg"),
};

static const wxString classNames[] = {
	_("Player"),
	_("Human Civilian"),
	_("Madd"),
	_("Possessed Hummer"),
	_("Defender"),
	_("Fighter"),
	_("Trooper"),
	_("Hunter"),
	_("Enforcer"),
	_("Juggernaut"),
	_("Hummer"),
	_("Compiler"),
	_("Cyborg"),
	_("Assimilated Civilian"),
	_("Tick"),
	_("Yeti"),
};

static const wxString damageNames[] = {
	_("Explosion"),
	_("Electrical Staff"),
	_("Projectile"),
	_("Absorbed"),
	_("Flame"),
	_("Hound Claws"),
	_("Alien Projectile"),
	_("Hulk Slap"),
	_("Compiler Bolt"),
	_("Fusion Bolt"),
	_("Hunter Bolt"),
	_("Fist"),
	_("Teleporter"),
	_("Defender"),
	_("Yeti Claws"),
	_("Yeti Projectile"),
	_("Crushing"),
	_("Lava"),
	_("Suffocation"),
	_("Goo"),
	_("Energy Drain"),
	_("Oxygen Drain"),
	_("Hummer Bolt"),
	_("Shotgun Projectile")	
};

static const wxString effectNames[] = {
	_("Missile Explosion"),
	_("Missile Contrail"),
	_("Grenade Explosion"),
	_("Grenade Contrail"),
	_("Bullet Ricochet"),
	_("Alien Weapon Impact"),
	_("Flamethrower Burst"),
	_("Pfhor Hit"),
	_("Marine Hit"),
	_("Bob Hit"),
	_("Assimilated Bob Hit"),
	_("Enforcer Hit"),
	_("S'pht Bolt Impact"),
	_("Seeking S'pht Bolt Impact"),
	_("S'pht Bolt Contrail"),
	_("Staff-Bolt Impact"),
	_("Staff Impact"),
	_("Hunter Bolt Impact"),
	_("Hunter Hit"),
	_("Plasma Impact"),
	_("Plasma Overload Impact"),
	_("Plasma Contrail"),
	_("Fist Impact"),
	_("S'pht'Kr Bolt Impact"),
	_("Major S'pht'Kr Bolt Impact"),
	_("S'pht'Kr Hit"),
	_("Trooper Hit"),
	_("Lamp Breaking 1"),
	_("Lamp Breaking 2"),
	_("Lamp Breaking 3"),
	_("Lamp Breaking 4"),
	_("Clank (melee hit)"),
	_("Teleport In"),
	_("Teleport Out"),
	_("Small Water Splash"),
	_("Medium Water Splash"),
	_("Large Water Splash"),
	_("Large Water Emergence"),
	_("Small Lava Splash"),
	_("Medium Lava Splash"),
	_("Large Lava Splash"),
	_("Large Lava Emergence"),
	_("Small Sewage Splash"),
	_("Medium Sewage Splash"),
	_("Large Sewage Splash"),
	_("Large Sewage Emergence"),
	_("Small Goo Splash"),
	_("Medium Goo Splash"),
	_("Large Goo Splash"),
	_("Large Goo Emergence"),
	_("Drone Bolt Impact"),
	_("Fast Drone Bolt Impact"),
	_("Possessed Drone Bolt Impact"),
	_("Drone Hit"),
	_("Cyborg Grenade Detonation"),
	_("Cyborg Hit"),
	_("Fusion Dispersal"),
	_("Major Fusion Dispersal"),
	_("Overloaded Fusion Dispersal"),
	_("F'lickta Hit"),
	_("F'lickta Glob Impact"),
	_("Water Flick'ta Hit"),
	_("Lava Flick'ta Hit"),
	_("Flick'ta Lava Glob Impact"),
	_("Flick'ta Claw Impact"),
	_("Juggernaut Hit"),
	_("Juggernaut Missile Contrail"),
	_("Small Special Splash"),
	_("Medium Special Splash"),
	_("Large Special Splash"),
	_("Special Emergence"),
	_("VacBob Hit"),
	_("Assimilated VacBob Hit"),
};

static const wxString itemNames[] = {
	_("Knife"),
	_("Magnum"),
	_("Magnum Magazine"),
	_("Plasma Pistol"),
	_("Plasma Magazine"),
	_("Assault Rifle"),
	_("Assault Rifle Magazine"),
	_("Assault Grenade Magazine"),
	_("Missile Launcher"),
	_("Missile Launcher Magazine"),
	_("Invisibility Powerup"),
	_("Invincibility Powerup"),
	_("Infravision Powerup"),
	_("Alien Shotgun"),
	_("Alien Shotgun Magazine"),
	_("Flamethrower"),
	_("Flamethrower Canister"),
	_("Extravision Powerup"),
	_("Oxygen Powerup"),
	_("Energy Powerup"),
	_("Double Energy Powerup"),
	_("Triple Energy Powerup"),
	_("Shotgun"),
	_("Shotgun Magazine"),
	_("Spht Door Key"),
	_("Uplink Chip"),
	_("Light Blue Ball"),
	_("Red Ball"),
	_("Violet Ball"),
	_("Yellow Ball"),
	_("Brown Ball"),
	_("Orange Ball"),
	_("Blue Ball"),
	_("Green Ball"),
	_("Smg"),
	_("Smg Ammo"),
};

static const wxString soundNames[] = {
	_("Startup"),
	_("Teleport in"),
	_("Teleport out"),
	_("Body being crunched"),
 	_("Jjaro ship creak"),
	_("Absorbed"),
	_("Breathing"),
 	_("Oxygen warning"),
	_("Suffocation"),
	_("Energy refuel"),
	_("Oxygen refuel"),
	_("Can't toggle switch"),
	_("Switch on"),
	_("Switch off"),
	_("Puzzle switch"),
	_("Chip insertion"),
	_("Pattern buffer"),
	_("Destroy control panel"),
	_("Adjust volume"),
	_("Got powerup"),
	_("Got item"),
	_("Bullet ricochet"),
	_("Metallic ricochet"),
	_("Empty gun"),
	_("S'pht door opening"),
	_("S'pht door closing"),
	_("S'pht door obstructed"),
	_("S'pht platform starting"),
	_("S'pht platform stopping"),
	_("Owl"),
	_("SMG firing"),
	_("SMG reloading"),
	_("Heavy s'pht platform starting"),
	_("Heavy s'pht platform stopping"),
	_("Fist hitting"),
	_("Magnum firing"),
	_("Magnum reloading"),
	_("Assault rifle firing"),
	_("Grenade launcher firing"),
	_("Grenade exploding"),
	_("Grenade flyby"),
	_("Fusion firing"),
	_("Fusion exploding"),
	_("Fusion flyby"),
	_("Fusion charging"),
	_("Rocket exploding"),
	_("Rocket flyby"),
	_("Rocket firing"),
	_("Flamethrower"),
	_("Body falling"),
	_("Body exploding"),
	_("Bullet hitting flesh"),
	_("Fighter activate"),
	_("Fighter wail"),
	_("Fighter scream"),
	_("Fighter chatter"),
	_("Fighter attack"),
	_("Fighter projectile hit"),
	_("Fighter projectile flyby"),
	_("Compiler attack"),
	_("Compiler death"),
	_("Compiler hit"),
	_("Compiler projectile flyby"),
	_("Compiler projectile hit"),
	_("Cyborg moving"),
	_("Cyborg attack"),
	_("Cyborg hit"),
	_("Cyborg death"),
	_("Cyborg projectile bounce"),
	_("Cyborg projectile hit"),
	_("Cyborg projectile flyby"),
	_("Hummer activate"),
	_("Hummer start attack"),
	_("Hummer attack"),
	_("Hummer dying"),
	_("Hummer death"),
	_("Hummer projectile hit"),
	_("Hummer projectile flyby"),
	_("Human wail"),
	_("Human scream"),
	_("Human hit"),
	_("Human chatter"),
	_("Assimilated human chatter"),
	_("Human trash talk"),
	_("Human apology"),
	_("Human activation"),
	_("Human clear"),
	_("Human stop shooting me you bastard"),
	_("Human area secure"),
	_("Kill the player"),
	_("Water"),
	_("Sewage"),
	_("Lava"),
	_("Goo"),
	_("Under media"),
	_("Wind"),
	_("Waterfall"),
	_("Siren"),
	_("Fan"),
	_("S'pht door"),
	_("S'pht platform"),
	_("Alien harmonics"),
	_("Heavy s'pht platform"),
	_("Light machinery"),
	_("Heavy machinery"),
	_("Transformer"),
	_("Sparking transformer"),
	_("Water drip"),
	_("Walking in water"),
	_("Exit water"),
	_("Enter water"),
	_("Small water splash"),
	_("Medium water splash"),
	_("Large water splash"),
	_("Walking in lava"),
	_("Enter lava"),
	_("Exit lava"),
	_("Small lava splash"),
	_("Medium lava splash"),
	_("Large lava splash"),
	_("Walking in sewage"),
	_("Exit sewage"),
	_("Enter sewage"),
	_("Small sewage splash"),
	_("Medium sewage splash"),
	_("Large sewage splash"),
	_("Walking in goo"),
	_("Exit goo"),
	_("Enter goo"),
	_("Small goo splash"),
	_("Medium goo splash"),
	_("Large goo splash"),
	_("Major fusion firing"),
	_("Major fusion charged"),
	_("Assault rifle reloading"),
	_("Assault rifle shell casings"),
	_("Shotgun firing"),
	_("Shotgun reloading"),
	_("Ball bounce"),
	_("You are it"),
	_("Got ball"),
	_("Computer interface logon"),
	_("Computer interface logout"),
	_("Computer interface page"),
	_("Heavy s'pht door"),
	_("Heavy s'pht door opening"),
	_("Heavy s'pht door closing"),
	_("Heavy s'pht door open"),
	_("Heavy s'pht door closed"),
	_("Heavy s'pht door obstructed"),
	_("Hunter activate"),
	_("Hunter attack"),
	_("Hunter dying"),
	_("Hunter landing"),
	_("Hunter exploding"),
	_("Hunter projectile hit"),
	_("Hunter projectile flyby"),
	_("Enforcer activate"),
	_("Enforcer attack"),
	_("Enforcer projectile hit"),
	_("Enforcer projectile flyby"),
	_("Yeti melee attack"),
	_("Yeti melee attack hit"),
	_("Yeti projectile attack"),
	_("Yeti projectile sewage attack hit"),
	_("Yeti projectile sewage flyby"),
	_("Yeti projectile lava attack hit"),
	_("Yeti projectile lava flyby"),
	_("Yeti dying"),
	_("Machine binder"),
	_("Machine bookpress"),
	_("Machine puncher"),
	_("Electric"),
	_("Alarm"),
	_("Night wind"),
	_("Surface explosion"),
	_("Underground explosion"),
	_("Defender attack"),
	_("Defender hit"),
	_("Defender flyby"),
	_("Defender being hit"),
	_("Defender exploding"),
	_("Tick chatter"),
	_("Tick falling"),
	_("Tick flapping"),
	_("Tick exploding"),
	_("Ceiling lamp exploding"),
	_("Pfhor platform starting"),
	_("Pfhor platform stopping"),
	_("Pfhor platform"),
	_("Pfhor door opening"),
	_("Pfhor door closing"),
	_("Pfhor door obstructed"),
	_("Pfhor door"),
	_("Pfhor switch off"),
	_("Pfhor switch on"),
	_("Juggernaut firing"),
	_("Juggernaut warning"),
	_("Juggernaut exploding"),
	_("Juggernaut preparing to fire"),
	_("Enforcer exploding"),
	_("Alien noise 1"),
	_("Alien noise 2"),
	_("Civilian fusion wail"),
	_("Civilian fusion scream"),
	_("Civilian fusion hit"),
	_("Civilian fusion chatter"),
	_("Assimilated civilian fusion chatter"),
	_("Civilian fusion trash talk"),
	_("Civilian fusion apology"),
	_("Civilian fusion activation"),
	_("Civilian fusion clear"),
	_("Civilian fusion stop shooting me you bastard"),
	_("Civilian fusion area secure"),
	_("Civilian fusion kill the player"),
};

static const wxString shotNames[] = {
	_("Missile"),
	_("Grenade"),
	_("Pistol Bullet"),
	_("Assault Rifle Bullet"),
	_("Shotgun Shot"),
	_("Pfhor Staff"),
	_("Pfhor Projectile"),
	_("Flamethrower"),
	_("S'pht Bolt"),
	_("Seeking S'pht Bolt"),
	_("Alien Weapon Blast"),
	_("Fusion Bolt"),
	_("Fusion Overload Bolt"),
	_("Hunter Bolt"),
	_("Fist Punch"),
	_("unused 1"),
	_("unused 2"),
	_("Missile 2"),
	_("Trooper Machine Gun"),
	_("Trooper Grenade"),
	_("S'pht'Kr Bolt"),
	_("Major S'pht'Kr Bolt"),
	_("Juggernaut Missile"),
	_("Energy Drain"),
	_("Major Energy Drain"),
	_("Oxygen Drain"),
	_("Drone Bolt"),
	_("Fast Drone Bolt"),
	_("Possessed Drone Bolt"),
	_("Bouncing Grenade"),
	_("Seeking Grenade"),
	_("Ball"),
	_("Fusion Media Backblast"),
	_("Fusion Overload Media Backblast"),
	_("Fusion Overload Self-Destruct"),
	_("Flick'ta Claw"),
	_("Flick'ta Glob"),
	_("Flick'ta Lava Glob"),
	_("SMG Bullet"),
};

static const wxString weaponNames[] = {
	_("Fist"),
	_("Pistol"),
	_("Plasma Pistol"),
	_("Assault Rifle"),
	_("Missile Launcher"),
	_("Flamethrower"),
	_("Alien Weapon"),
	_("Shotgun"),
	_("The Ball"),
	_("SMG")
};

DefaultNames* DefaultNames::mInstance;

DefaultNames::DefaultNames() 
{
	for (int i = 0; i < 47; ++i) {
		mMap[wxT("alien")][i] = alienNames[i];
	}

	for (int i = 0; i < 16; ++i) {
		mMap[wxT("class")][i] = classNames[i];
	}

	for (int i = 0; i < 32; ++i) {
		mMap[wxT("collection")][i] = collectionNames[i];
	}
	
	for (int i = 0; i < 24; ++i) {
		mMap[wxT("damage")][i] = damageNames[i];
	}

	for (int i = 0; i < 73; ++i) {
		mMap[wxT("effect")][i] = effectNames[i];
	}

	for (int i = 0; i < 36; ++i) {
		mMap[wxT("item")][i] = itemNames[i];
	}

	for (int i = 0; i < 215; ++i) {
		mMap[wxT("sound")][i] = soundNames[i];
	}

	for (int i = 0; i < 39; ++i) {
		mMap[wxT("shot")][i] = shotNames[i];
	}

	for (int i = 0; i < 10; ++i) {
		mMap[wxT("weapon")][i] = weaponNames[i];
	}

	wxTextFile namesFile(wxStandardPaths::Get().GetResourcesDir() + wxT("/DefaultNames.txt"));
	
	if (namesFile.Exists()) {
		if (namesFile.Open()) {
			for (wxString str = namesFile.GetFirstLine(); !namesFile.Eof(); str = namesFile.GetNextLine()) {
				str.Trim(true);
				str.Trim(false);

				if (str[0] == '#') {
					continue;
				}

				// parse into three tokens
				wxStringTokenizer tokenizer(str);
				if (tokenizer.CountTokens() >= 3) {
					wxString key = tokenizer.GetNextToken();
					wxString indexStr = tokenizer.GetNextToken();
					wxString value = tokenizer.GetString();
					
					long index = 0;
					if (!indexStr.ToLong(&index)) {
						continue;
					}

					mMap[key][index] = value;
				}
			}
			
			namesFile.Close();
		}
	}
}

wxString DefaultNames::Get(const wxString& set, int index)
{
	std::map<wxString, std::map<int, wxString> >::const_iterator it = mMap.find(set);
	if (it != mMap.end()) {
		std::map<int, wxString>::const_iterator kvp = it->second.find(index);
		if (kvp != it->second.end()) {
			return kvp->second;
		}
	}

	if (index == -1) {
		return _("None");
	} else {
		return set + wxString::Format(wxT(" %i"), index);
	}
}

std::vector<wxString> DefaultNames::GetArray(const wxString& set)
{
	std::vector<wxString> v;

	int size = mMap[set].rbegin()->first + 1;
	if (size > 0) {
		v.resize(size);
		
		for (int i = 0; i < size; ++i) {
			v[i] = GetName(set, i);
		}
	}

	return v;
}

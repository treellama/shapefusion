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
	_("Interface"),
	_("Weapons in Hand"),
	_("Juggernaut"),
	_("Tick"),
	_("Explosion Effects"),
	_("Hunter"),
	_("Marine"),
	_("Items"),
	_("Trooper"),
	_("Pfhor fighter"),
	_("S'pht'Kr"),
	_("F'lickta"),
	_("Bob"),
	_("VacBob (Inf only)"),
	_("Enforcer"),
	_("Drone"),
	_("S'pht"),
	_("Walls 1 (Water)"),
	_("Walls 2 (Lava)"),
	_("Walls 3 (Sewage)"),
	_("Walls 4 (Jjaro)"),
	_("Walls 5 (Pfhor)"),
	_("Scenery 1 (Water)"),
	_("Scenery 2 (Lava)"),
	_("Scenery 3 (Sewage)"),
	_("Scenery 4 (Jjaro)"),
	_("Scenery 5 (Pfhor)"),
	_("Landscape 1 (Day)"),
	_("Landscape 2 (Night)"),
	_("Landscape 3 (Moon)"),
	_("Landscape 4 (Space)"),
	_("Cyborg"),
};

static const wxString classNames[] = {
	_("Marine"),
	_("Bob"),
	_("Marathon Defense Drone"),
	_("Possessed Drone"),
	_("S'pht'Kr"),
	_("Pfhor Fighter"),
	_("Pfhor Trooper"),
	_("Hunter"),
	_("Enforcer"),
	_("Juggernaut"),
	_("Drone"),
	_("S'pht"),
	_("Cyborg"),
	_("Assimilated Bob"),
	_("Tick"),
	_("F'lickta"),
};

static const wxString damageNames[] = {
	_("Explosion"),
	_("Electrical Staff"),
	_("Projectile"),
	_("Absorbed"),
	_("Flame"),
	_("Claws"),
	_("Alien Projectile"),
	_("Hulk Slap"),
	_("Compiler Bolt"),
	_("Fusion Bolt"),
	_("Hunter Bolt"),
	_("Fist"),
	_("Teleporter"),
	_("S'pht'Kr Bolt"),
	_("Flick'ta Claws"),
	_("Flick'ta Projectile"),
	_("Crushing"),
	_("Lava"),
	_("Suffocation"),
	_("Goo"),
	_("Energy Drain"),
	_("Oxygen Drain"),
	_("Drone Bolt"),
	_("Shotgun"),
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
	_("None"),
	_("Fist"),
	_("Magnum Pistol"),
	_("Magnum Magazine"),
	_("Plasma Pistol"),
	_("Plasma Battery"),
	_("Assault Rifle"),
	_("Assault Rifle Clip"),
	_("Grenade Clip"),
	_("Missile Launcher"),
	_("Missile Launcher Clip"),
	_("Invisibility Powerup"),
	_("Invincibility Powerup"),
	_("Infravision Powerup"),
	_("Alien Weapon"),
	_("Alien Weapon Magazine"),
	_("Flamethrower"),
	_("Flamethrower Canister"),
	_("Extravision Powerup"),
	_("Oxygen Powerup"),
	_("Energy Powerup"),
	_("Double Energy Powerup"),
	_("Triple Energy Powerup"),
	_("Shotgun"),
	_("Shotgun Magazine"),
	_("S'pht Door Key"),
	_("Uplink Chip"),
	_("Light Blue Ball"),
	_("The Ball"),
	_("Violet Ball"),
	_("Yellow Ball"),
	_("Brown Ball"),
	_("Orange Ball"),
	_("Blue Ball"),
	_("Green Ball"),
	_("SMG"),
	_("SMG Ammo"),
};

static const wxString soundNames[] = {
	_("Startup"),
	_("Teleport In"),
	_("Teleport Out"),
	_("Crunch"),
	_("Creak (Inf only)"),
	_("Absorbed"),
	_("Unused 1"),
	_("Oxygen Warning"),
	_("Suffocation"),
	_("Energy Refuel"),
	_("Oxygen Refuel"),
	_("Can't Toggle Switch"),
	_("Switch On"),
	_("Switch Off"),
	_("Puzzle Switch"),
	_("Chip Insertion"),
	_("Pattern Buffer"),
	_("Destroy Control Panel"),
	_("Adjust Volume"),
	_("Got Powerup"),
	_("Got Item"),
	_("Bullet Ricochet"),
	_("Metallic Ricochet"),
	_("Empty Gun"),
	_("S'pht Door Opening"),
	_("S'pht Door Closing"),
	_("S'pht Door Obstructed"),
	_("S'pht Platform Starting"),
	_("S'pht Platform Stopping"),
	_("Loon"),
	_("SMG firing (Inf only)"),
	_("SMG reloading (Inf only)"),
	_("Heavy S'pht Platform Starting"),
	_("Heavy S'pht Platform Stopping"),
	_("Fist Hitting"),
	_("Magnum Firing"),
	_("Magnum Reloading"),
	_("Assault Rifle Firing"),
	_("Grenade Launcher Firing"),
	_("Grenade Exploding"),
	_("Grenade Flyby"),
	_("Fusion Firing"),
	_("Fusion Exploding"),
	_("Fusion Flyby"),
	_("Fusion Charging"),
	_("Rocket Expoding"),
	_("Rocket Flyby"),
	_("Rocket Firing"),
	_("Flamethrower"),
	_("Body Falling"),
	_("Body Exploding"),
	_("Bullet Hitting Flesh"),
	_("Fighter Activate"),
	_("Fighter Wail"),
	_("Fighter Scream"),
	_("Fighter Chatter"),
	_("Fighter Attack"),
	_("Fighter Projectile Hit"),
	_("Fighter Projectile Flyby"),
	_("S'pht Attack"),
	_("S'pht Death"),
	_("S'pht Hit"),
	_("S'pht Projectile Flyby"),
	_("S'pht Projectile Hit"),
	_("Cyborg Moving"),
	_("Cyborg Attack"),
	_("Cyborg Hit"),
	_("Cyborg Death"),
	_("Cyborg Projectile Bounce"),
	_("Cyborg Projectile Hit"),
	_("Unused 2"),
	_("Drone Activate"),
	_("Drone Start Attack"),
	_("Drone Attack"),
	_("Drone Dying"),
	_("Drone Death"),
	_("Drone Projectile Hit"),
	_("Drone Projectile Flyby"),
	_("Human Wail"),
	_("Human Scream"),
	_("Human Hit"),
	_("Human Chatter"),
	_("Assimilated Human Chatter"),
	_("Human Trash Talk"),
	_("Human Apology"),
	_("Human Activation"),
	_("Human Clear"),
	_("Human Stop Shooting Me You Bastard"),
	_("Human Area Secure"),
	_("Kill The Player!"),
	_("Water"),
	_("Sewage"),
	_("Lava"),
	_("Goo"),
	_("Under Stuff"),
	_("Wind"),
	_("Waterfall"),
	_("Siren"),
	_("Fan"),
	_("S'pht Door"),
	_("S'pht Platform"),
	_("Alien Harmonics (Inf only)"),
	_("Heavy S'pht Platform"),
	_("Light Machinery"),
	_("Heavy Machinery"),
	_("Transformer"),
	_("Sparking Transformer"),
	_("Water Drip"),
	_("Walking in Water"),
	_("Exit Water"),
	_("Enter Water"),
	_("Small Water Splash"),
	_("Medium Water Splash (unused)"),
	_("Large Water Splash"),
	_("Walking in Lava (unused)"),
	_("Exit Lava"),
	_("Enter Lava"),
	_("Small Lava Splash"),
	_("Medium Lava Splash (unused)"),
	_("Large Lava Splash"),
	_("Walking in Sewage (unused)"),
	_("Exit Sewage"),
	_("Enter Sewage"),
	_("Small Sewage Splash"),
	_("Medium Sewage Splash (unused)"),
	_("Large Sewage Splash"),
	_("Walking in Goo (unused)"),
	_("Exit Goo (unused)"),
	_("Enter Goo (unused)"),
	_("Small Goo Splash (unused)"),
	_("Medium Goo Splash (unused)"),
	_("Large Goo Splash (unused)"),
	_("Major Fusion Firing"),
	_("Major Fusion Charged"),
	_("Assault Rifle Reloading"),
	_("Assault Rifle Shell Casings"),
	_("Shotgun Firing"),
	_("Shotgun Reloading"),
	_("Ball Bounce"),
	_("You Are It"),
	_("Got The Ball"),
	_("Computer Login"),
	_("Computer Logout"),
	_("Computer Page"),
	_("Heavy S'pht Door"),
	_("Heavy S'pht Door Opening"),
	_("Heavy S'pht Door Closing"),
	_("Heavy S'pht Door Open"),
	_("Heavy S'pht Door Closed"),
	_("Heavy S'pht Door Obstructed"),
	_("Hunter Activate (unused)"),
	_("Hunter Attack"),
	_("Hunter Dying (unused)"),
	_("Hunter Landing"),
	_("Hunter Exploding"),
	_("Hunter Projectile Hit"),
	_("Hunter Projectile Flyby"),
	_("Enforcer Activate"),
	_("Enforcer Attack"),
	_("Enforcer Projectile Hit"),
	_("Enforcer Projctile Flyby"),
	_("F'lickta Melee Attack"),
	_("F'lickta Melee Attack Hit"),
	_("F'lickta Projectile Attack"),
	_("F'lickta Sewage Attack Hit"),
	_("F'lickta Sewage Flyby"),
	_("F'lickta Lava Attack Hit"),
	_("F'lickta Lava Flyby"),
	_("F'lickta Dying"),
	_("Machine Binder"),
	_("Machine Bookpress"),
	_("Machine Puncher"),
	_("Electric"),
	_("Alarm"),
	_("Night Wind"),
	_("Surface Explosion"),
	_("Underground Explosion"),
	_("S'pht'Kr Attack"),
	_("S'pht'Kr Hit"),
	_("S'pht'Kr Flyby"),
	_("S'pht'Kr Being Hit"),
	_("S'pht'Kr Exploding"),
	_("Tick Chatter"),
	_("Tick Falling"),
	_("Tick Flapping"),
	_("Tick Exploding"),
	_("Lamp Exploding"),
	_("Pfhor Platform Start"),
	_("Pfhor Platform Stop"),
	_("Pfhor Platform"),
	_("Pfhor Door Opening"),
	_("Pfhor Door Closing"),
	_("Pfhor Door Obstructed"),
	_("Pfhor Door"),
	_("Pfhor Switch Off"),
	_("Pfhor Switch On"),
	_("Juggernaut Firing"),
	_("Juggernaut Warning"),
	_("Juggernaut Exploding"),
	_("Juggernaut Preparing to Fire"),
	_("Enforcer Exploding"),
	_("Alien Noise 1"),
	_("Alien Noise 2"),
	_("VacBob Wail"),
	_("VacBob Scream"),
	_("VacBob Hit"),
	_("VacBob Chatter"),
	_("Assimilated VacBob Chatter"),
	_("VacBob Trash Talk"),
	_("VacBob Apology"),
	_("VacBob Activation"),
	_("VacBob Clear"),
	_("VacBob Stop Shooting Me You Bastard"),
	_("VacBob Area Secure"),
	_("VacBob Kill The Player"),
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

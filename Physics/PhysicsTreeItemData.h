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
#ifndef PHYSICSTREEITEMDATA_H
#define PHYSICSTREEITEMDATA_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "wx/treectrl.h"

enum {
	TREESECTION_MONSTERS = 1,
	TREESECTION_MONSTERS_APPEARANCE,
	TREESECTION_MONSTERS_COMBAT,
	TREESECTION_MONSTERS_CONSTANTS,
	TREESECTION_MONSTERS_BEHAVIOR,
	TREESECTION_MONSTERS_IMMUNITIES,
	TREESECTION_EFFECTS,
	TREESECTION_PROJECTILES,
	TREESECTION_PHYSICS,
	TREESECTION_WEAPONS,
	TREESECTION_WEAPONS_SETTINGS,
	TREESECTION_WEAPONS_TRIGGERS,
};

class PhysicsTreeItemData : public wxTreeItemData {
private:
	int id;
	int section;

public:
	PhysicsTreeItemData(int id=-1, int sect=-1);
	~PhysicsTreeItemData();

	int ID(void) const { return id; }
	int Section(void) const { return section; }
};

#endif

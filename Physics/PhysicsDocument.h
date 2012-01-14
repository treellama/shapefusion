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

#ifndef __PHYSICSDOCUMENT_H__
#define __PHYSICSDOCUMENT_H__

#include "wx/docview.h"
#include "wx/cmdproc.h"

#include <vector>

#include <stdint.h>

#include "PhysicsElements.h"

class PhysicsDocument : public wxDocument, public PhysicsElement
{
	DECLARE_DYNAMIC_CLASS(PhysicsDocument)

private:
	std::vector<MonsterDefinition> mMonsterDefinitions;
	std::vector<EffectDefinition> mEffectDefinitions;
	std::vector<ProjectileDefinition> mProjectileDefinitions;
	std::vector<PhysicsConstants> mConstants;
	std::vector<WeaponDefinition> mWeaponDefinitions;

	static const int MAXIMUM_WADFILE_NAME_LENGTH = 64;
	unsigned char wadfile_name[MAXIMUM_WADFILE_NAME_LENGTH];
	
	static const int kEntryHeaderSize = 16;

public:
	// counts
	unsigned int MonsterCount() const { return mMonsterDefinitions.size(); }
	unsigned int EffectCount() const { return mEffectDefinitions.size(); }
	unsigned int ProjectileCount() const { return mProjectileDefinitions.size(); }
	unsigned int PhysicsConstantsCount() const { return mConstants.size(); }
	unsigned int WeaponCount() const { return mWeaponDefinitions.size(); }

	// element accessors
	PhysicsConstants* GetPhysicsConstants(unsigned int id) { 
		return &mConstants[id];
	}

	EffectDefinition* GetEffectDefinition(unsigned int id) {
		return &mEffectDefinitions[id];
	}

	ProjectileDefinition* GetProjectileDefinition(unsigned int id) {
		return &mProjectileDefinitions[id];
	}
	
	MonsterDefinition* GetMonsterDefinition(unsigned int id) {
		return &mMonsterDefinitions[id];
	}

	WeaponDefinition* GetWeaponDefinition(unsigned int id) {
		return &mWeaponDefinitions[id];
	}

	PhysicsDocument();
	~PhysicsDocument();

	bool DoOpenDocument(const wxString& file);

#if wxUSE_STD_IOSTREAM
	wxSTD ostream& SaveObject(wxSTD ostream& stream);
	wxSTD istream& LoadObject(wxSTD istream& stream);
#else
	wxOutputStream& SaveObject(wxOutputStream& stream);
	wxInputStream& LoadObject(wxInputStream& stream);
#endif

	unsigned long SizeInFile();
	template<class T> void WriteTag(BigEndianBuffer& buffer, long& offset, unsigned long tag, const std::vector<T>& data, bool last = false);
};

#endif

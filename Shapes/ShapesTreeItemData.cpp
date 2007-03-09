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
#include "ShapesTreeItemData.h"

ShapesTreeItemData::ShapesTreeItemData(int id, int vers, int sect, int seq):
	coll_id(id), version(vers), section(sect), sequence(seq)
{

}

ShapesTreeItemData::~ShapesTreeItemData(void)
{

}

int ShapesTreeItemData::CollID(void) const
{
	return coll_id;
}

int ShapesTreeItemData::Version(void) const
{
	return version;
}

int ShapesTreeItemData::Section(void) const
{
	return section;
}

int ShapesTreeItemData::Sequence(void) const
{
	return sequence;
}

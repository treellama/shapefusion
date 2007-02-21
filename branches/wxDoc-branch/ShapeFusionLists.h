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

#ifndef SHAPEFUSIONLISTS_H
#define SHAPEFUSIONLISTS_H

//#include "ShapesDocument.h"

WX_DECLARE_LIST(short, shortList);
WX_DECLARE_LIST(int, intList);
WX_DECLARE_LIST(long, longList);

WX_DECLARE_LIST(wxBitmap, wxBitmapList);
WX_DECLARE_LIST(wxPoint, wxPointList);

class ShapesColor;
class ShapesColorTable;
class ShapesSequence;
class ShapesFrame;
class ShapesBitmap;
WX_DECLARE_LIST(ShapesColor, ShapesColorList);
WX_DECLARE_LIST(ShapesColorTable, ShapesColorTableList);
WX_DECLARE_LIST(ShapesSequence, ShapesSequenceList);
WX_DECLARE_LIST(ShapesFrame, ShapesFrameList);
WX_DECLARE_LIST(ShapesBitmap, ShapesBitmapList);

#endif
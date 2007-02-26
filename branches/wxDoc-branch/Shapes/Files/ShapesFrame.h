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

#define SIZEOF_low_level_shape_definition	36

// a frame, aka "low level shape definition"
class ShapesFrame : public ShapesElement
{
private:
	bool		mXmirror,
				mYmirror,
				mKeypointObscured;
	double		mMinimumLightIntensity;
	short		mBitmapIndex;
	// bitmap scale factor. Computed when loading Shapes
	// using world_* fields and associated bitmap dimensions
	int			mScaleFactor;
	// logical origin
	short		mOriginX, mOriginY;
	// keypoint. Used in player legs shapes to specify where to attach torso shapes
	short		mKeyX, mKeyY;
	// FIXME do we really need to store these fields here?
	// scaled bitmap rectangle in world coordinates. The physical bitmap rectangle
	// is scaled around the frame origin position, which gets translated to (0,0).
	// Computed as:
	//   world_left = -scale_factor * origin_x
	//   world_top = scale_factor * origin_y
	//   world_right = scale_factor * (width - origin_x)
	//   world_bottom = -scale_factor * (height - origin_y)
	short		mWorldLeft, mWorldRight, mWorldTop, mWorldBottom;
	// scaled keypoint position in world coordinates. Computed as:
	//   world_x0 = scale_factor * (key_x - origin_x)
	//   world_y0 = -scale_factor * (key_y - origin_y)
	short		mWorldX0, mWorldY0;
	// list of sequences referencing this frame
	intList		mUsers;
	
	wxInt32 mOffset;
public:

	ShapesFrame(wxInt32 offset, bool verbose = false);
	~ShapesFrame(void);
	
	void CalculateScaleFactor(ShapesBitmapList *bitmaps, short chunkPixelToWorld);
	
    wxOutputStream& SaveObject(wxOutputStream& stream);
    BigEndianBuffer& LoadObject(BigEndianBuffer& stream);
};

WX_DECLARE_LIST(ShapesFrame, ShapesFrameList);


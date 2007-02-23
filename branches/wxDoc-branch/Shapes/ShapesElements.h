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

#ifndef SHAPESELEMENT_H
#define SHAPESELEMENT_H

#include "ShapeFusionLists.h"

#define COLLECTIONS_PER_FILE				32
#define COLLECTION_VERSION					3
// on-file struct sizes
#define SIZEOF_collection_header			32
#define SIZEOF_collection_definition		544
#define SIZEOF_rgb_color_value				8
#define SIZEOF_bitmap_definition			30
#define SIZEOF_low_level_shape_definition	36
#define SIZEOF_high_level_shape_definition	88
// NOTE about SIZEOF_high_level_shape_definition. The original engine
// sets SIZEOF_high_level_shape_definition=90 because the first frame
// index is included in the high_level_shape_definition. I don't like
// this, so I do my way (but be careful!)

// color flags
enum {
	SELF_LUMINESCENT_COLOR	= 1 << 7
};
// sequence types (ShpSequence.number_of_views). Nobody
// seems to know what these values really mean, they just
// tell the actual number of views in a redundant way
enum {
	ANIMATED_1 = 1,		// simple isotropic animation
	ANIMATED_2TO8 = 2,	// 8 view animation
	ANIMATED_3TO4 = 3,	// 4 view animation
	ANIMATED_4 = 4,		// 4 view animation
	ANIMATED_5TO8 = 5,	// 8 view animation
	ANIMATED_8 = 8,		// 8 view animation
	ANIMATED_3TO5 = 9,	// 5 view animation
	UNANIMATED = 10,	// no animation, choose a random frame
	ANIMATED_5 = 11		// 5 view animation
};
// frame flags
enum {
	X_MIRROR			= 1 << 15,	// mirror along vertical axis
	Y_MIRROR			= 1 << 14,	// mirror along horizontal axis
	KEYPOINT_OBSCURED	= 1 << 13	// "host obscures parasite" (RenderPlaceObjs.cpp)
};
// bitmap flags
enum {
	COLUMN_ORDER			= 1 << 15,
	TRANSPARENCY_ENABLED	= 1 << 14
};
// chunk types. Bitmap encoding seems to depend on this setting
enum {
	_unused_collection = 0,	// plain
	_wall_collection,		// plain
	_object_collection,		// RLE
	_interface_collection,	// plain
	_scenery_collection		// RLE
};
// internal-use utility constants
enum {
	COLL_VERSION_8BIT = 0,
	COLL_VERSION_TRUECOLOR
};

class ShapesElement /*: public wxObject*/
{
private:
	bool	mVerboseLoading;

protected:
	bool	mGoodData;
	
public:
	ShapesElement(bool verbose);
	~ShapesElement(void);
	
	bool IsGoodData();
	bool IsVerbose();
};

// a color
class ShapesColor : public ShapesElement
{
private:
	bool			mLuminescent;
	unsigned char	mValue;
	unsigned short	mRed, mGreen, mBlue;

public:
	ShapesColor(bool verbose = false);
	~ShapesColor(void);
	
#if wxUSE_STD_IOSTREAM
    wxSTD ostream& SaveObject(wxSTD ostream& stream);
    wxSTD istream& LoadObject(wxSTD istream& stream);
#else
    wxOutputStream& SaveObject(wxOutputStream& stream);
    wxInputStream& LoadObject(wxInputStream& stream);
#endif
};

WX_DECLARE_LIST(ShapesColor, ShapesColorList);

// a color table
class ShapesColorTable : public ShapesElement
{
private:
	ShapesColorList		mColors;
	
	wxInt32 mColorCount,
			mOffset;
	
public:
	ShapesColorTable::ShapesColorTable(wxInt32 colorcount, wxInt32 offset, bool verbose = false);
	~ShapesColorTable(void);
	
#if wxUSE_STD_IOSTREAM
    wxSTD ostream& SaveObject(wxSTD ostream& stream);
    wxSTD istream& LoadObject(wxSTD istream& stream);
#else
    wxOutputStream& SaveObject(wxOutputStream& stream);
    wxInputStream& LoadObject(wxInputStream& stream);
#endif
};

WX_DECLARE_LIST(ShapesColorTable, ShapesColorTableList);

// a sequence, aka "high level shape definition"
class ShapesSequence : public ShapesElement
{
private:
	short			mType;
	unsigned short	mFlags;
	wxString		mName;
	short			mNumberOfViews,
					mFramesPerView,
					mTicksPerFrame,
					mKeyFrame,
					mTransferMode,
					mTransferModePeriod,
					mFirstFrameSound,
					mKeyFrameSound,
					mLastFrameSound,
					mPixelsToWorld,
					mLoopFrame;
	shortList		mFrameIndexes;
	
	wxInt32 mOffset;
public:

	ShapesSequence(wxInt32 offset, bool verbose = false);
	~ShapesSequence(void);
	

#if wxUSE_STD_IOSTREAM
    wxSTD ostream& SaveObject(wxSTD ostream& stream);
    wxSTD istream& LoadObject(wxSTD istream& stream);
#else
    wxOutputStream& SaveObject(wxOutputStream& stream);
    wxInputStream& LoadObject(wxInputStream& stream);
#endif
};

WX_DECLARE_LIST(ShapesSequence, ShapesSequenceList);

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
	
#if wxUSE_STD_IOSTREAM
    wxSTD ostream& SaveObject(wxSTD ostream& stream);
    wxSTD istream& LoadObject(wxSTD istream& stream);
#else
    wxOutputStream& SaveObject(wxOutputStream& stream);
    wxInputStream& LoadObject(wxInputStream& stream);
#endif
};

WX_DECLARE_LIST(ShapesFrame, ShapesFrameList);

// a bitmap
class ShapesBitmap : public ShapesElement
{
private:
	short			mWidth, mHeight,
					mBytesPerRow,	// width for uncompressed bitmaps, -1 for compressed ones
					mBitDepth;		// 8
	bool			mColumnOrder,	// store in column-order format
					mTransparent;
	unsigned char	*mPixels;
	// list of frames referencing this bitmap
	intList			mUsers;
	
	wxInt32 mOffset;
	
public:

	ShapesBitmap(wxInt32 offset, bool verbose = false);
	~ShapesBitmap(void);

#if wxUSE_STD_IOSTREAM
    wxSTD ostream& SaveObject(wxSTD ostream& stream);
    wxSTD istream& LoadObject(wxSTD istream& stream);
#else
    wxOutputStream& SaveObject(wxOutputStream& stream);
    wxInputStream& LoadObject(wxInputStream& stream);
#endif
};

WX_DECLARE_LIST(ShapesBitmap, ShapesBitmapList);

// a Shapes chunk
class ShapesChunk : public ShapesElement
{
private:
	short			mVersion;	// COLLECTION_VERSION (same for all Marathon games)
	short			mType;
	unsigned short	mFlags;		// unused; 0 in Durandal/Infinity, 1 in Rubicon and others
	short			mPixels_to_world;

	ShapesColorTableList	mColortables;
	ShapesSequenceList		mSequences;
	ShapesFrameList			mFrames;
	ShapesBitmapList		mBitmaps;

	wxInt32 mOffset, mLength;

public:
	ShapesChunk(wxInt32 offset, wxInt32 length, bool verbose = false);
	~ShapesChunk(void);

#if wxUSE_STD_IOSTREAM
    wxSTD ostream& SaveObject(wxSTD ostream& stream);
    wxSTD istream& LoadObject(wxSTD istream& stream);
#else
    wxOutputStream& SaveObject(wxOutputStream& stream);
    wxInputStream& LoadObject(wxInputStream& stream);
#endif
};

// a Shapes collection
class ShapesCollection : public ShapesElement
{
private:
	short			mStatus;
	unsigned short	mFlags;
	ShapesChunk		*mChunks[2];	// chunks for 8-bit and truecolor game
	
public:
	ShapesCollection(bool verbose = false);
	~ShapesCollection(void);
	
#if wxUSE_STD_IOSTREAM
    wxSTD ostream& SaveObject(wxSTD ostream& stream);
    wxSTD istream& LoadObject(wxSTD istream& stream);
#else
    wxOutputStream& SaveObject(wxOutputStream& stream);
    wxInputStream& LoadObject(wxInputStream& stream);
#endif
};

WX_DECLARE_LIST(ShapesCollection, ShapesCollectionList);

#endif
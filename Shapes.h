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

//
// Shapes
// Shapes class representing what's inside a Marathon shapes file.
// Note that, for keeping things simple and providing compatibility,
// the class doesn't store everything using the original on-disk format.
// For example, flags are expanded to bool fields and fixed point values
// are converted to doubles. Every bitmap is converted to a simple block
// of width*height pixels, regardless of its compression and pixel order.
// Conversions take place only when actually loading and writing files.
//

#ifndef SHAPESFILE_H
#define SHAPESFILE_H

#include <fstream>
#include <string>
#include <vector>
using std::vector;

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

// a color
typedef struct {
	bool			self_luminescent;
	unsigned char	value;
	unsigned short	red, green, blue;
} ShpColor;
// color flags
enum {
	SELF_LUMINESCENT_COLOR	= 1 << 7
};

// a color table
typedef struct {
	vector<ShpColor>	colors;
} ShpColorTable;

// a sequence, aka "high level shape definition"
typedef struct {
	short			type;
	unsigned short	flags;
	char			name[33];
	short			number_of_views,
					frames_per_view,
					ticks_per_frame,
					key_frame,
					transfer_mode,
					transfer_mode_period,
					first_frame_sound,
					key_frame_sound,
					last_frame_sound,
					pixels_to_world,
					loop_frame;
	vector<short>	frame_indexes;
} ShpSequence;
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

// a frame, aka "low level shape definition"
typedef struct {
	bool		x_mirror,
				y_mirror,
				keypoint_obscured;
	double		minimum_light_intensity;
	short		bitmap_index;
	// bitmap scale factor. Computed when loading Shapes
	// using world_* fields and associated bitmap dimensions
	int			scale_factor;
	// logical origin
	short		origin_x, origin_y;
	// keypoint. Used in player legs shapes to specify where to attach torso shapes
	short		key_x, key_y;
	// FIXME do we really need to store these fields here?
	// scaled bitmap rectangle in world coordinates. The physical bitmap rectangle
	// is scaled around the frame origin position, which gets translated to (0,0).
	// Computed as:
	//   world_left = -scale_factor * origin_x
	//   world_top = scale_factor * origin_y
	//   world_right = scale_factor * (width - origin_x)
	//   world_bottom = -scale_factor * (height - origin_y)
	short		world_left, world_right, world_top, world_bottom;
	// scaled keypoint position in world coordinates. Computed as:
	//   world_x0 = scale_factor * (key_x - origin_x)
	//   world_y0 = -scale_factor * (key_y - origin_y)
	short		world_x0, world_y0;
	// list of sequences referencing this frame
	vector<int>	users;
} ShpFrame;
// frame flags
enum {
	X_MIRROR			= 1 << 15,	// mirror along vertical axis
	Y_MIRROR			= 1 << 14,	// mirror along horizontal axis
	KEYPOINT_OBSCURED	= 1 << 13	// "host obscures parasite" (RenderPlaceObjs.cpp)
};

// a bitmap
typedef struct {
	short			width, height,
					bytes_per_row,	// width for uncompressed bitmaps, -1 for compressed ones
					bit_depth;		// 8
	bool			column_order,	// store in column-order format
					enable_transparency;
	unsigned char	*pixels;
	// list of frames referencing this bitmap
	vector<int>		users;
} ShpBitmap;
// bitmap flags
enum {
	COLUMN_ORDER			= 1 << 15,
	TRANSPARENCY_ENABLED	= 1 << 14
};

// a Shapes chunk
typedef struct {
	short			version;	// COLLECTION_VERSION (same for all Marathon games)
	short			type;
	unsigned short	flags;		// unused; 0 in Durandal/Infinity, 1 in Rubicon and others
	short			pixels_to_world;

	vector<ShpColorTable>	ctabs;
	vector<ShpSequence>		sequences;
	vector<ShpFrame>		frames;
	vector<ShpBitmap>		bitmaps;

	long			size;		// FIXME just to debug file save routines, remove when ok
} ShpChunk;
// chunk types. Bitmap encoding seems to depend on this setting
enum {
	_unused_collection = 0,	// plain
	_wall_collection,		// plain
	_object_collection,		// RLE
	_interface_collection,	// plain
	_scenery_collection		// RLE
};

// a Shapes collection
typedef struct {
	short			status;
	unsigned short	flags;
	ShpChunk		*chunks[2];	// chunks for 8-bit and truecolor game
} ShpCollection;

// internal-use utility constants
enum {
	COLL_VERSION_8BIT = 0,
	COLL_VERSION_TRUECOLOR
};

// class representing the contents of a Marathon shapes file
class Shapes {
private:
	vector<ShpCollection>	collections;
	bool					good_data;
	bool					verbose_file_load;

	int LoadShapesChunk(unsigned int id, ShpChunk *pc, std::ifstream &is, long offset, long expected_size);

public:
	Shapes(void);
	~Shapes(void);

	// file I/O
	Shapes(std::string path);
	int WriteToFile(std::string path);
	bool GoodData(void) const;
	// collection data access
	int CollGlobalStatus(unsigned int id) const;
	unsigned int CollGlobalFlags(unsigned int id) const;
	bool CollDefined(unsigned int id, unsigned int version) const;
	int CollVersion(unsigned int id, unsigned int version) const;
	int CollType(unsigned int id, unsigned int version) const;
	unsigned int CollFlags(unsigned int id, unsigned int version) const;
	int CollScaleFactor(unsigned int id, unsigned int version) const;
	unsigned int CollBitmapCount(unsigned int id, unsigned int version) const;
	unsigned int CollCTCount(unsigned int id, unsigned int version) const;
	unsigned int CollFrameCount(unsigned int id, unsigned int version) const;
	unsigned int CollSeqCount(unsigned int id, unsigned int version) const;
	ShpColorTable *GetColorTable(unsigned int coll, unsigned int version, unsigned int ct) const;
	ShpBitmap *GetBitmap(unsigned int coll, unsigned int version, unsigned int bitmap) const;
	ShpFrame *GetFrame(unsigned int coll, unsigned int version, unsigned int frame) const;
	ShpSequence *GetSeq(unsigned int coll, unsigned int version, unsigned int seq) const;
	// collection alteration
	void InsertColorTable(ShpColorTable ct, unsigned int coll, unsigned int version);
	void DeleteColorTable(unsigned int coll, unsigned int version, unsigned int ct);
	void InsertBitmap(ShpBitmap b, unsigned int coll, unsigned int version);
	void DeleteBitmap(unsigned int coll, unsigned int version, unsigned int b);
	void InsertFrame(ShpFrame f, unsigned int coll, unsigned int version);
	void DeleteFrame(unsigned int coll, unsigned int version, unsigned int f);
	void InsertSequence(ShpSequence s, unsigned int coll, unsigned int version);
	void DeleteSequence(unsigned int coll, unsigned int version, unsigned int s);
	// utilities
	unsigned int CollectionSizeInFile(unsigned int id, unsigned int version) const;
};

int CalcActualNumberOfViews(int t);

#endif

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

#ifndef SHAPESLOADERS_H
#define SHAPESLOADERS_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <vector>
using std::vector;
#include "../BigEndianBuffer.h"

#define COLLECTIONS_PER_FILE		32

#define SIZEOF_collection_header	32

class ShapesElement /*: public wxObject*/
{
private:
	bool	mVerboseLoading;

protected:
	// So that subclasses can change their status
	bool	mGoodData;

public:
	ShapesElement(bool verbose) : mVerboseLoading(verbose), mGoodData(false) {}
	~ShapesElement(void) {}
	
	bool IsGood() const {return mGoodData;}
	bool IsVerbose() const {return mVerboseLoading;}
};

// internal-use utility constants
enum {
	COLL_VERSION_8BIT = 0,
	COLL_VERSION_TRUECOLOR
};

// a color
class ShapesColor : public ShapesElement
{
private:
	bool			mLuminescent;
	unsigned char	mValue;
	unsigned short	mRed, mGreen, mBlue;

public:
	// Constructor / Destructor
	ShapesColor(bool verbose = false);
	~ShapesColor(void);
	
	// Accessors
	bool Luminescent(void) const {return mLuminescent;}
	unsigned char Value(void) const {return mValue;}
	unsigned short Red(void) const {return mRed;}
	unsigned short Green(void) const {return mGreen;}
	unsigned short Blue(void) const {return mBlue;}
	
	// Utilities
    BigEndianBuffer& SaveObject(BigEndianBuffer& buffer);
    BigEndianBuffer& LoadObject(BigEndianBuffer& buffer);
};

// a color table
class ShapesColorTable : public ShapesElement
{
private:
	vector<ShapesColor*>	mColors;

public:
	ShapesColorTable(bool verbose = false);
	~ShapesColorTable(void);

	unsigned int ColorCount(void) const {return mColors.size();}
	ShapesColor *GetColor(unsigned int index) const {return mColors[index];}
	
	unsigned int SizeInFile() const;
	
    BigEndianBuffer& SaveObject(BigEndianBuffer& buffer);
    BigEndianBuffer& LoadObject(BigEndianBuffer& buffer, unsigned int offset, unsigned int color_count);
};

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
	vector<int>		mUsers;

public:
	// Constructor / Destructor
	ShapesBitmap(bool verbose = false);
	ShapesBitmap(wxImage image, ShapesColorTable *colortable);
	~ShapesBitmap(void);

	// Accessors
	short Width(void) const {return mWidth;}
	short Height(void) const {return mHeight;}
	short BytesPerRow(void) const {return mBytesPerRow;}
	short BitDepth(void) const {return mBitDepth;}
	bool IsColumnOrdered(void) const {return mColumnOrder;}
	bool IsTransparent(void) const {return mTransparent;}
	unsigned char* Pixels(void) const {return mPixels;}
	
	// Mutators
	void SetWidth(short w) {mWidth = w;}
	void SetHeight(short h) {mHeight = h;}
	void SetBytesPerRow(short b) {mBytesPerRow = b;}
	void SetBitDepth(short b) {mBitDepth = b;}
	void SetColumnOrdered(bool b) {mColumnOrder = b;}
	void SetTransparent(bool n) {mTransparent = n;}

	// Utilities
	unsigned int SizeInFile() const;
	
    BigEndianBuffer& SaveObject(BigEndianBuffer& buffer);
    BigEndianBuffer& LoadObject(BigEndianBuffer& buffer, unsigned int offset);
};

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
	vector<int>	mUsers;
public:
	// Constructor / Destructor
	ShapesFrame(bool verbose = false);
	~ShapesFrame(void);

	// Accessors
	bool IsXmirrored(void) const {return mXmirror;}
	bool IsYmirrored(void) const {return mYmirror;}
	bool IsKeypointObscured(void) const {return mKeypointObscured;}
	double MinimumLightIntensity(void) const {return mMinimumLightIntensity;}
	int ScaleFactor(void) const {return mScaleFactor;}
	short BitmapIndex(void) const {return mBitmapIndex;}
	short OriginX(void) const {return mOriginX;}
	short OriginY(void) const {return mOriginY;}
	short KeyX(void) const {return mKeyX;}
	short KeyY(void) const {return mKeyY;}
	short WorldLeft(void) const {return mWorldLeft;}
	short WorldRight(void) const {return mWorldRight;}
	short WorldTop(void) const {return mWorldTop;}
	short WorldBottom(void) const {return mWorldBottom;}
	short WorldX0(void) const {return mWorldX0;}
	short WorldY0(void) const {return mWorldY0;}

	// Mutators
	void SetXmirrored(bool b) {mXmirror = b;}
	void SetYmirrored(bool b) {mYmirror = b;}
	void SetKeypointObscured(bool b) {mKeypointObscured = b;}
	void SetMinimumLightIntensity(double v) {mMinimumLightIntensity = v;}
	void SetBitmapIndex(short i) {mBitmapIndex = i;}
	void SetScaleFactor(int s) {mScaleFactor = s;}
	void SetOriginX(short x) {mOriginX = x;}
	void SetOriginY(short y) {mOriginY = y;}
	void SetKeyX(short x) {mKeyX = x;}
	void SetKeyY(short y) {mKeyY = y;}
	void SetWorldLeft(short s) {mWorldLeft = s;}
	void SetWorldRight(short s) {mWorldRight = s;}
	void SetWorldTop(short s) {mWorldTop = s;}
	void SetWorldBottom(short s)  {mWorldBottom = s;}
	void SetWorldX0(short s) {mWorldX0 = s;}
	void SetWorldY0(short s) {mWorldY0 = s;}

	// Utilities
	unsigned int SizeInFile() const;

    BigEndianBuffer& SaveObject(BigEndianBuffer& buffer);
    BigEndianBuffer& LoadObject(BigEndianBuffer& buffer, unsigned int offset);
};

// sequence types (ShapesSequence.number_of_views). Nobody
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
//FIXME This could be made private
public:
	vector<short>	mFrameIndexes;

public:
	// Constructor / Destructor
	ShapesSequence(bool verbose = false);
	~ShapesSequence(void);

	// Accessors
	short Type(void) const {return mType;}
	unsigned short Flags(void) const {return mFlags;}
	wxString Name(void) const {return mName;}
	short NumberOfViews(void) const {return mNumberOfViews;}
	short FramesPerView(void) const {return mFramesPerView;}
	short TicksPerFrame(void) const {return mTicksPerFrame;}
	short KeyFrame(void) const {return mKeyFrame;}
	short TransferMode(void) const {return mTransferMode;}
	short TransferModePeriod(void) const {return mTransferModePeriod;}
	short FirstFrameSound(void) const {return mFirstFrameSound;}
	short KeyFrameSound(void) const {return mKeyFrameSound;}
	short LastFrameSound(void) const {return mLastFrameSound;}
	short PixelsToWorld(void) const {return mPixelsToWorld;}
	short LoopFrame(void) const {return mLoopFrame;}

	// Mutators
	void SetType(short t) {mType = t;}
	void SetFlags(unsigned short f) {mFlags = f;}
	void SetName(wxString name) {mName = name;}
	void SetNumberOfViews(short n) {mNumberOfViews = n;}
	void SetFramesPerView(short n) {mFramesPerView = n;}
	void SetTicksPerFrame(short n) {mTicksPerFrame = n;}
	void SetKeyFrame(short n) {mKeyFrame = n;}
	void SetTransferMode(short n) {mTransferMode = n;}
	void SetTransferModePeriod(short n) {mTransferModePeriod = n;}
	void SetFirstFrameSound(short n) {mFirstFrameSound = n;}
	void SetKeyFrameSound(short n) {mKeyFrameSound = n;}
	void SetLastFrameSound(short n) {mLastFrameSound = n;}
	void SetPixelsToWorld(short n) {mPixelsToWorld = n;}
	void SetLoopFrame(short n) {mLoopFrame = n;}
	
	unsigned int FrameIndexCount(void) const {return mFrameIndexes.size();} 
	short GetFrameIndex(unsigned int index) const {return mFrameIndexes[index];} 
	void SetFrameIndex(unsigned int index, short value) {mFrameIndexes[index] = value;} 
	
	// Utilities
	unsigned int SizeInFile() const;
	
    BigEndianBuffer& SaveObject(BigEndianBuffer& buffer);
    BigEndianBuffer& LoadObject(BigEndianBuffer& buffer, long offset);
};

int ActualNumberOfViews(int t);

// chunk types. Bitmap encoding seems to depend on this setting
enum {
	_unused_collection = 0,	// plain
	_wall_collection,		// plain
	_object_collection,		// RLE
	_interface_collection,	// plain
	_scenery_collection		// RLE
};

// a Shapes chunk
class ShapesChunk : public ShapesElement
{
private:
	short			mVersion;	// COLLECTION_VERSION (same for all Marathon games)
	short			mType;
	unsigned short	mFlags;		// unused; 0 in Durandal/Infinity, 1 in Rubicon and others
	short			mPixelsToWorld;

	vector<ShapesColorTable*>	mColorTables;
	vector<ShapesSequence*>		mSequences;
	vector<ShapesFrame*>		mFrames;
	vector<ShapesBitmap*>		mBitmaps;

public:
	// Constructor / Destructor
	ShapesChunk(bool verbose = false);
	~ShapesChunk(void);

	// Chunk Data access
	int Version() const {return mVersion;}
	int Type() const {return mType;}
	int Flags() const {return mFlags;}
	int ScaleFactor() const {return mPixelsToWorld;}
	unsigned int ColorTableCount() const {return mColorTables.size();}
	unsigned int BitmapCount() const {return mBitmaps.size();}
	unsigned int FrameCount() const {return mFrames.size();}
	unsigned int SequenceCount() const {return mSequences.size();}
	ShapesColorTable* GetColorTable(unsigned int index) const;
	ShapesBitmap* GetBitmap(unsigned int index) const;
	ShapesFrame* GetFrame(unsigned int index) const;
	ShapesSequence* GetSequence(unsigned int index) const;
	// Chunk Alteration
	void InsertColorTable(ShapesColorTable *ct);
	void DeleteColorTable(unsigned int ct);
	void InsertBitmap(ShapesBitmap *b);
	void DeleteBitmap(unsigned int b);
	void InsertFrame(ShapesFrame *f);
	void DeleteFrame(unsigned int f);
	void InsertSequence(ShapesSequence *s);
	void DeleteSequence(unsigned int s);
	
	// Utilities
	unsigned int SizeInFile() const;

    BigEndianBuffer& SaveObject(BigEndianBuffer& stream);
    BigEndianBuffer& LoadObject(BigEndianBuffer& stream);
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

	// Accessors
	int Status(void) const {return mStatus;}
	int Flags(void) const {return mFlags;}

	// Collection Data access
	bool Defined(unsigned int chunk) const;
	int Version(unsigned int chunk) const;
	int Type(unsigned int chunk) const;
	int Flags(unsigned int chunk) const;
	int ScaleFactor(unsigned int chunk) const;
	int ColorTableCount(unsigned int chunk) const;
	int BitmapCount(unsigned int chunk) const;
	int FrameCount(unsigned int chunk) const;
	int SequenceCount(unsigned int chunk) const;
	ShapesColorTable* GetColorTable(unsigned int chunk, unsigned int index) const;
	ShapesBitmap* GetBitmap(unsigned int chunk, unsigned int index) const;
	ShapesFrame* GetFrame(unsigned int chunk, unsigned int index) const;
	ShapesSequence* GetSequence(unsigned int chunk, unsigned int index) const;
	
	// collection alteration
	void InsertColorTable(ShapesColorTable *ct, unsigned int chunk);
	void DeleteColorTable(unsigned int chunk, unsigned int ct);
	void InsertBitmap(ShapesBitmap *b, unsigned int chunk);
	void DeleteBitmap(unsigned int chunk, unsigned int b);
	void InsertFrame(ShapesFrame *f, unsigned int chunk);
	void DeleteFrame(unsigned int chunk, unsigned int f);
	void InsertSequence(ShapesSequence *s, unsigned int chunk);
	void DeleteSequence(unsigned int chunk, unsigned int s);
	
	// Utilities
	unsigned int SizeInFile(unsigned int chunk) const;
	
#if wxUSE_STD_IOSTREAM
    wxSTD ostream& SaveObject(wxSTD ostream& stream);
    wxSTD istream& LoadObject(wxSTD istream& stream);
#else
    wxOutputStream& SaveObject(wxOutputStream& stream);
    wxInputStream& LoadObject(wxInputStream& stream);
#endif
};

#endif


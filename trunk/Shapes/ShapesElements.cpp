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
#include <fstream>
#include "ShapesElements.h"
#include "utilities.h"
#include "../LittleEndianBuffer.h"
#include <algorithm>

// on-file struct sizes
#define SIZEOF_collection_definition		544
#define SIZEOF_rgb_color_value				8
#define SIZEOF_bitmap_definition			30
#define SIZEOF_low_level_shape_definition	36
#define SIZEOF_high_level_shape_definition	88
// NOTE about SIZEOF_high_level_shape_definition. The original engine
// sets SIZEOF_high_level_shape_definition=90 because the first frame
// index is included in the high_level_shape_definition. I don't like
// this, so I do my way (but be careful!)
#define COLLECTION_VERSION					3

// color flags
enum {
	SELF_LUMINESCENT_COLOR	= 1 << 7
};
// bitmap flags
enum {
	COLUMN_ORDER			= 1 << 15,
	TRANSPARENCY_ENABLED	= 1 << 14
};
// frame flags
enum {
	X_MIRROR			= 1 << 15,	// mirror along vertical axis
	Y_MIRROR			= 1 << 14,	// mirror along horizontal axis
	KEYPOINT_OBSCURED	= 1 << 13	// "host obscures parasite" (RenderPlaceObjs.cpp)
};

#define FOUR_CHARS_TO_INT(a,b,c,d) (((unsigned int)(a) << 24) | ((unsigned int)(b) << 16) | ((unsigned int)(c) << 8) | (unsigned int)(d))

ShapesColor::ShapesColor(bool verbose): ShapesElement(verbose)
{

}

ShapesColor::ShapesColor(unsigned int r, unsigned int g, unsigned int b, unsigned int value, bool luminescent, bool verbose):
	 ShapesElement(verbose), mLuminescent(luminescent), mValue(value), mRed(r), mGreen(g), mBlue(b)
{

}

ShapesColor::~ShapesColor(void)
{

}

bool ShapesColor::operator==(const ShapesColor& other) const
{
	return mLuminescent == other.mLuminescent 
		&& mValue == other.mValue
		&& mRed == other.mRed
		&& mGreen == other.mGreen 
		&& mBlue == other.mBlue;
}

BigEndianBuffer& ShapesColor::SaveObject(BigEndianBuffer& buffer)
{
	unsigned char	flags = mLuminescent ? SELF_LUMINESCENT_COLOR : 0;
	
	buffer.WriteUChar(flags);
	buffer.WriteUChar(mValue);
	buffer.WriteUShort(mRed);
	buffer.WriteUShort(mGreen);
	buffer.WriteUShort(mBlue);
	
	return buffer;
}

BigEndianBuffer& ShapesColor::LoadObject(BigEndianBuffer& buffer)
{
	unsigned char	flags;

	flags = buffer.ReadUChar();
	mValue = buffer.ReadUChar();
	mRed = buffer.ReadUShort();
	mGreen = buffer.ReadUShort();
	mBlue = buffer.ReadUShort();
	
	mLuminescent = flags & SELF_LUMINESCENT_COLOR;
	
	mGoodData = true;
	return buffer;
}

ShapesColorTable::ShapesColorTable(bool verbose): ShapesElement(verbose)
{

}

ShapesColorTable::ShapesColorTable(std::ifstream& ifs, wxString file_ext): ShapesElement(false)
{
	// FIXME better error checking and reporting
	if (file_ext == wxString(wxT("act"))) {
		// Photoshop binary color table file (Adobe Color Table, .act)
		ifs.seekg(0, std::ios::end);
		unsigned int	fileSize = ifs.tellg(),
						colorCount = 0;
		ifs.seekg(0, std::ios::beg);

		if (fileSize >= 3*256+4) {
			// extra info found - get the color count from that
			BigEndianBuffer	extraInfo(4);

			ifs.seekg(3 * 256);
			ifs.read((char *)extraInfo.Data(), 4);
			colorCount = (unsigned int)extraInfo.ReadShort();
			if (colorCount > 256)
				colorCount = 256;
		} else if (fileSize == 3*256) {
			// no extra info - exactly 256 colors and no transparent color
			colorCount = 256;
		} else {
			// we need at least 3*256 bytes
			wxLogError(wxT("This Adobe Color Table file has an invalid size: will not try to load it."));
			return;
		}
		ifs.seekg(0, std::ios::beg);
		for (unsigned int value = 0; value < colorCount; value++) {
			unsigned char	rgb[3];
			ifs.read((char *)rgb, 3);
			ShapesColor		*newColor = new ShapesColor(rgb[0]<<8, rgb[1]<<8, rgb[2]<<8, value);
			mColors.push_back(newColor);
		}
	} else if (file_ext == wxString(wxT("gpl"))) {
		// Gimp ASCII palette file
		unsigned int	value = 0;

		while (ifs.good() && value < 256) {
			char			buffer[256] = "";
			unsigned int	red, green, blue;

			ifs.getline(buffer, 255);
			if (sscanf(buffer, "%u %u %u", &red, &green, &blue) == 3) {
				ShapesColor	*newColor = new ShapesColor(red<<8, green<<8, blue<<8, value);

				mColors.push_back(newColor);
				value++;
			}
		}
	}
}

ShapesColorTable::~ShapesColorTable(void)
{
	for (unsigned int i = 0; i < mColors.size(); i++)
		delete mColors[i];
}

static bool compareShapesColorPtrs(ShapesColor* rhs, ShapesColor* lhs)
{
	return (*rhs == *lhs);
}

bool ShapesColorTable::operator==(const ShapesColorTable& other) const
{
	if (mColors.size() == other.mColors.size()) {
		return std::equal(mColors.begin(), mColors.end(), other.mColors.begin(), compareShapesColorPtrs);
	} else {
		return false;
	}
}

BigEndianBuffer& ShapesColorTable::SaveObject(BigEndianBuffer& stream)
{
	for (unsigned int i = 0; i < mColors.size(); i++) {
		ShapesColor	*color = mColors[i];

		color->SaveObject(stream);
	}
	return stream;
}

BigEndianBuffer& ShapesColorTable::SavePatch(BigEndianBuffer& buffer, int index)
{
	buffer.WriteLong(FOUR_CHARS_TO_INT('c','t','a','b'));
	buffer.WriteLong(index);
	return SaveObject(buffer);
}

BigEndianBuffer& ShapesColorTable::LoadObject(BigEndianBuffer& buffer, unsigned int offset, unsigned int color_count)
{
	buffer.Position(offset);

	for (unsigned int i = 0; i < color_count; i++) {
		ShapesColor	*color = new ShapesColor(IsVerbose());

		color->LoadObject(buffer);
		
		if (!color->IsGood()) {
			wxLogError(wxT("[ShapesColorTable] Error loading color table"));
			mGoodData = false;
			return buffer;
		}
		mColors.push_back(color);
	}
	
	mGoodData = true;
	return buffer;
}

// export a color table to Gimp ASCII format
int ShapesColorTable::SaveToGimp(wxString path) const
{
	std::ofstream	cts(path.fn_str(), std::ios::binary);

	if (cts.good()) {
		cts << "GIMP Palette\n";
		cts << "Name: ShapeFusion exported palette\n";
		cts << "#\n";
		for (unsigned int i = 0; i < ColorCount(); i++) {
			ShapesColor	*color = GetColor(i);

			cts << (color->Red() >> 8) << ' ';
			cts << (color->Green() >> 8) << ' ';
			cts << (color->Blue() >> 8) << '\n';
		}
		cts.close();
		return 0;
	} else {
		return -1;
	}
}

// export a color table to Photoshop binary format
// (MacOS file type is '8BCT', extension '.act', Adobe Color Table)
int ShapesColorTable::SaveToPhotoshop(wxString path) const
{
	std::ofstream	cts(path.fn_str(), std::ios::binary);

	if (cts.good()) {
		BigEndianBuffer	actData(3*256+4);
		
		actData.WriteZeroes(3*256+4);
		actData.Position(0);
		// write the RGB byte triplets for our colors
		for (unsigned int i = 0; i < ColorCount(); i++) {
			ShapesColor *color = GetColor(i);
			
			actData.WriteChar(color->Red() >> 8);
			actData.WriteChar(color->Green() >> 8);
			actData.WriteChar(color->Blue() >> 8);
		}
		// write the extra info at the end:
		// number of colors and index of the transparent color
		actData.Position(3*256);
		actData.WriteShort(ColorCount());
		actData.WriteShort(0);
		cts.write((char *)actData.Data(), actData.Size());
		cts.close();
		return 0;
	} else {
		return -1;
	}
}

ShapesBitmap::ShapesBitmap(bool verbose): ShapesElement(verbose), mPixels(NULL)
{
}

ShapesBitmap::ShapesBitmap(wxImage image, ShapesColorTable *colortable):
	ShapesElement(false), mPixels(NULL)
{
	FromImage(image, colortable);
}

ShapesBitmap::~ShapesBitmap(void)
{
	if (mPixels)
		delete mPixels;
	mPixels = NULL;
}

bool ShapesBitmap::operator==(const ShapesBitmap& other) const
{
	if (mWidth == other.mWidth 
	    && mHeight == other.mHeight
	    && mBytesPerRow == other.mBytesPerRow
	    && mBitDepth == other.mBitDepth
	    && mColumnOrder == other.mColumnOrder
	    && mTransparent == other.mTransparent) {
		return std::equal(mPixels, mPixels + (mWidth * mHeight), other.mPixels);
	} else {
		return false;
	}
}

unsigned int ShapesBitmap::SizeInFile(void) const
{
	unsigned int size = 0;

	// scanline pointer placeholder
	if (mColumnOrder)
		size += 4 * mWidth;
	else
		size += 4 * mHeight;
	if (mBytesPerRow == -1) {
		// compressed
		size += mWidth * 4;
		for (int x = 0; x < mWidth; x++) {
			unsigned char	*pp = mPixels + x;
			int				p0 = -1,
				p1;
			
			for (int y = 0; y < mHeight; y++) {
				if (*pp != 0) {
					p0 = y;
					break;
				}
				pp += mWidth;
			}
			if (p0 == -1)
				continue;	// no opaque mPixels in this column
			p1 = p0;
			pp = mPixels + x + mWidth * (mHeight - 1);
			for (int y = mHeight - 1; y >= 0; y--) {
				if (*pp != 0) {
					p1 = y;
					break;
				}
				pp -= mWidth;
			}
			size += p1 - p0 + 1;
		}
	} else {
		// plain
		size += mWidth * mHeight;
	}

	return size;
}

BigEndianBuffer& ShapesBitmap::SaveObject(BigEndianBuffer& buffer)
{
	short	flags = 0;

	if (mColumnOrder)
		flags |= COLUMN_ORDER;
	if (mTransparent)
		flags |= TRANSPARENCY_ENABLED;
	buffer.WriteShort(mWidth);
	buffer.WriteShort(mHeight);
	buffer.WriteShort(mBytesPerRow);
	buffer.WriteShort(flags);
	buffer.WriteShort(mBitDepth);
	buffer.WriteZeroes(20 + 4 * (mColumnOrder ? mWidth : mHeight));
	if (mBytesPerRow == -1) {
		// compress
		for (int x = 0; x < mWidth; x++) {
			unsigned char	*pp = mPixels + x;
			int				p0 = -1, p1;
			
			for (int y = 0; y < mHeight; y++) {
				if (*pp != 0) {
					p0 = y;
					break;
				}
				pp += mWidth;
			}
			if (p0 == -1) {
				// no opaque pixels in this column
				buffer.WriteShort(0);
				buffer.WriteShort(0);
			} else {
				// found opaque pixels, go on
				p1 = p0;
				pp = mPixels + x + mWidth * (mHeight - 1);
				for (int y = mHeight - 1; y >= 0; y--) {
					if (*pp != 0) {
						p1 = y;
						break;
					}
					pp -= mWidth;
				}
				buffer.WriteShort(p0);
				buffer.WriteShort(p1 + 1);
				pp = mPixels + x + p0 * mWidth;
				for (int y = p0; y <= p1; y++) {
					buffer.WriteChar(*pp);
					pp += mWidth;
				}
			}
		}
	} else {
		if (mColumnOrder) {
			for (int x = 0; x < mWidth; x++) {
				for (int y = 0; y < mHeight; y++)
					buffer.WriteChar(*(mPixels + x + y * mWidth));
			}
		} else {
			buffer.WriteBlock(mWidth * mHeight, mPixels);
		}
	}
	return buffer;
}

BigEndianBuffer& ShapesBitmap::SavePatch(BigEndianBuffer& buffer, int index)
{
	buffer.WriteLong(FOUR_CHARS_TO_INT('b', 'm', 'a', 'p'));
	buffer.WriteLong(index);
	buffer.WriteLong(SizeInFile() + SIZEOF_bitmap_definition);
	return SaveObject(buffer);
}

BigEndianBuffer& ShapesBitmap::LoadObject(BigEndianBuffer& buffer, unsigned int offset)
{
	buffer.Position(offset);

	mWidth = buffer.ReadShort();
	mHeight = buffer.ReadShort();
	mBytesPerRow = buffer.ReadShort();

	if (mWidth < 0) {
		wxLogError(wxT("[ShapesBitmap] Invalid bitmap width %d"), mWidth);
		return buffer;
	}
	if (mHeight < 0) {
		wxLogError(wxT("[ShapesBitmap] Invalid bitmap height %d"), mHeight);
		return buffer;
	}
	if (mBytesPerRow < -1) {
		wxLogError(wxT("[ShapesBitmap] Invalid bitmap bytes-per-row %d"), mBytesPerRow);
		return buffer;
	}

	short	flags = buffer.ReadShort();

	mColumnOrder = flags & COLUMN_ORDER;
	mTransparent = flags & TRANSPARENCY_ENABLED;

	mBitDepth = buffer.ReadShort();
	if (mBitDepth != 8) {
		wxLogError(wxT("[ShapesBitmap] Invalid bitmap depth %d"), mBitDepth);
		return buffer;
	}

	if (IsVerbose()) {
		wxLogDebug(wxT("[ShapesBitmap]         Width:		%d"), mWidth);
		wxLogDebug(wxT("[ShapesBitmap]         Height:		%d"), mHeight);
		wxLogDebug(wxT("[ShapesBitmap]         Bytes/Row:	%d"), mBytesPerRow);
		wxLogDebug(wxT("[ShapesBitmap]         Flags:		%d"), flags);
		wxLogDebug(wxT("[ShapesBitmap]         Bit Depth:	%d"), mBitDepth);
	}

	// skip unused fields and placeholders
	unsigned int	numscanlines = (mColumnOrder ? mWidth : mHeight);

	buffer.Position(buffer.Position() + 20 + numscanlines * 4);

	// load pixel data
	mPixels = new unsigned char[mWidth * mHeight];
	if (mPixels == NULL) {
		wxLogError(wxT("[ShapesBitmap] Could not allocate pixel buffer"));
		return buffer;
	}
	if (mBytesPerRow > -1) {
		// uncompressed bitmap
		if (mColumnOrder) {
			// column order
			unsigned char	*dstp;
			
			for (int x = 0; x < mWidth; x++) {
				dstp = mPixels + x;
				for (int y = 0; y < mHeight; y++) {
					*dstp = buffer.ReadUChar();
					dstp += mWidth;
				}
			}
		} else {
			// row order
			buffer.ReadBlock(mWidth * mHeight, mPixels);
		}
	} else {
		// compressed bitmap (always column order)
		memset(mPixels, 0, mWidth * mHeight);
		for (int x = 0; x < mWidth; x++) {
			short			p0, p1;
			unsigned char	*dstp;

			p0 = buffer.ReadShort();
			p1 = buffer.ReadShort();
			dstp = mPixels + x + p0 * mWidth;
			while (p0 != p1) {
				*dstp = buffer.ReadUChar();
				dstp += mWidth;
				p0++;
			}
		}
	}

	mGoodData = true;
	return buffer;
}

// export the ShapesBitmap to an indexed BMP file specified by path
void ShapesBitmap::SaveToBMP(wxString path, ShapesColorTable *colorTable) const
{
	std::ofstream	stream(path.fn_str(), std::ios::binary);

	if (stream.good()) {
		unsigned int	colorCount = colorTable->ColorCount();
		unsigned long	paddedWidth = (mWidth + 3) & 0xfffffffc;

		// header
		LittleEndianBuffer	headerBlock(54);

		headerBlock.WriteChar('B');
		headerBlock.WriteChar('M');
		headerBlock.WriteULong(54 + 4*colorCount + paddedWidth * mHeight);	// file size
		headerBlock.WriteULong(0);	// reserved
		headerBlock.WriteULong(54 + 4*colorCount);	// raster data offset
		headerBlock.WriteULong(40);	// info header size
		headerBlock.WriteULong(mWidth);
		headerBlock.WriteULong(mHeight);
		headerBlock.WriteUShort(1);	// plane count
		headerBlock.WriteUShort(8);	// bits per pixel
		headerBlock.WriteULong(0);	// no compression
		headerBlock.WriteULong(0);	// compressed size of image
		headerBlock.WriteULong(0);
		headerBlock.WriteULong(0);
		headerBlock.WriteULong(colorCount);	// FIXME
		headerBlock.WriteULong(0);	// FIXME
		stream.write((const char *)headerBlock.Data(), headerBlock.Size());
		
		// palette
		LittleEndianBuffer	paletteBlock(4*colorCount);

		for (unsigned int i = 0; i < colorCount; i++) {
			ShapesColor	*color = colorTable->GetColor(i);

			paletteBlock.WriteUChar(color->Blue() >> 8);
			paletteBlock.WriteUChar(color->Green() >> 8);
			paletteBlock.WriteUChar(color->Red() >> 8);
			paletteBlock.WriteUChar(0);
		}
		stream.write((const char *)paletteBlock.Data(), paletteBlock.Size());

		// 8-bit raster data
		LittleEndianBuffer	rasterBlock(paddedWidth * mHeight);

		for (int y = 0; y < mHeight; y++) {
			rasterBlock.WriteBlock(mWidth, mPixels + (mHeight - y - 1) * mWidth);
			rasterBlock.WriteZeroes(paddedWidth - mWidth);
		}
		stream.write((const char *)rasterBlock.Data(), rasterBlock.Size());

		stream.close();
	}
}

// export the ShapesBitmap mask to a 1-bit BMP file specified by path
void ShapesBitmap::SaveMaskToBMP(wxString path) const
{
	std::ofstream	stream(path.fn_str(), std::ios::binary);

	if (stream.good()) {
		unsigned long	rowBytes = ((mWidth + 31) & 0xffffffe0) >> 3;

		// header
		LittleEndianBuffer	headerBlock(54);
		
		headerBlock.WriteChar('B');
		headerBlock.WriteChar('M');
		headerBlock.WriteULong(54 + 4*2 + rowBytes * mHeight);	// file size
		headerBlock.WriteULong(0);	// reserved
		headerBlock.WriteULong(54 + 4*2);	// raster data offset
		headerBlock.WriteULong(40);	// info header size
		headerBlock.WriteULong(mWidth);
		headerBlock.WriteULong(mHeight);
		headerBlock.WriteUShort(1);	// plane count
		headerBlock.WriteUShort(1);	// bits per pixel
		headerBlock.WriteULong(0);	// no compression
		headerBlock.WriteULong(0);	// compressed size of image
		headerBlock.WriteULong(0);
		headerBlock.WriteULong(0);
		headerBlock.WriteULong(2);
		headerBlock.WriteULong(0);
		stream.write((const char *)headerBlock.Data(), headerBlock.Size());

		// black & white palette
		LittleEndianBuffer	paletteBlock(4*2);

		paletteBlock.WriteUChar(0);
		paletteBlock.WriteUChar(0);
		paletteBlock.WriteUChar(0);
		paletteBlock.WriteUChar(0);
		paletteBlock.WriteUChar(255);
		paletteBlock.WriteUChar(255);
		paletteBlock.WriteUChar(255);
		paletteBlock.WriteUChar(0);
		stream.write((const char *)paletteBlock.Data(), paletteBlock.Size());

		// 1-bit raster data
		LittleEndianBuffer	rasterBlock(rowBytes * mHeight);
		
		rasterBlock.WriteZeroes(rowBytes * mHeight);
		for (unsigned int y = 0; (int)y < mHeight; y++) {
			unsigned char	*p = mPixels + y * mWidth,
							bit = 128,
							byte = 0;

			rasterBlock.Position((mHeight - y - 1) * rowBytes);
			for (unsigned int x = 0; (int)x < mWidth; x++) {
				if (*p++ != 0)
					byte |= bit;
				bit >>= 1;
				if (bit == 0) {
					bit = 128;
					rasterBlock.WriteUChar(byte);
					byte = 0;
				}
			}
			if (bit != 128)
				rasterBlock.WriteUChar(byte);
		}
		stream.write((const char *)rasterBlock.Data(), rasterBlock.Size());

		stream.close();
	 }
}

void ShapesBitmap::ClipboardCopy(ShapesColorTable* colortable) const
{
	if (wxTheClipboard->Open()) {
		// create an RGBA wxImage
		wxImage image;
		image.Create(mWidth, mHeight, false);

		unsigned char* src = mPixels;
		unsigned char* dst = image.GetData();
		for (int x = 0; x < mWidth; ++x) {
			for (int y = 0; y < mHeight; ++y) {
				unsigned char c = *src++;
				ShapesColor* color = colortable->GetColor(c);
				*dst++ = color->Red();
				*dst++ = color->Green();
				*dst++ = color->Blue();
			}
		}

		wxTheClipboard->SetData(new wxBitmapDataObject(wxBitmap(image)));
		wxTheClipboard->Close();
	}
}

void ShapesBitmap::ClipboardPaste(ShapesColorTable* colortable)
{
	if (wxTheClipboard->Open()) {
		wxBitmapDataObject clipboardData;
		if (wxTheClipboard->GetData(clipboardData)) {
			FromImage(clipboardData.GetBitmap().ConvertToImage(), colortable);
		}
		wxTheClipboard->Close();
	}
}

void ShapesBitmap::FromImage(wxImage image, ShapesColorTable* colortable)
{
	mWidth = image.GetWidth();
	mHeight = image.GetHeight();
	mBytesPerRow = image.GetWidth();
	mBitDepth = 8;
	mColumnOrder = true;
	mTransparent = false;
	
	if (mPixels) {
		delete mPixels;
	}

	unsigned char* srcpixels = image.GetData(), *src = srcpixels, *dst;

	mPixels = new unsigned char[mWidth * mHeight];
	if (mPixels == NULL) {
		wxLogError(wxT("Could not allocate new %dx%d bitmap\n"), mWidth, mHeight);
		return;
	}
	dst = mPixels;
	// quantize from 8-bit RGB pixels to an indexed bitmap
	for (int i = 0; i < mWidth * mHeight; i++) {
		unsigned char 	r = *src++, g = *src++, b = *src++,
						best_value = 0;
		float			min_dist = 0;

		for (unsigned int j = 0; j < colortable->ColorCount(); j++) {
			unsigned short	ct_r = colortable->GetColor(j)->Red(),
							ct_g = colortable->GetColor(j)->Green(),
							ct_b = colortable->GetColor(j)->Blue();
			float			dist = ColourDistance(r/255.0, g/255.0, b/255.0,
													ct_r/65535.0, ct_g/65535.0, ct_b/65535.0);

			if (dist < min_dist || j == 0) {
				min_dist = dist;
				best_value = colortable->GetColor(j)->Value();
			}
		}
		*dst++ = best_value;
		if (best_value == 0)
			mTransparent = true;	// guess the user will want transparency
	}
}

ShapesFrame::ShapesFrame(bool verbose): ShapesElement(verbose)
{
	// initialize values to something reasonable
	mBitmapIndex = -1;
	mXmirror = mYmirror = mKeypointObscured = false;
	mMinimumLightIntensity = 0;
	mOriginX = mOriginY = mKeyX = mKeyY = 0;
	mScaleFactor = 0;
	mWorldLeft = mWorldRight = mWorldTop = mWorldBottom = 0;
	mWorldX0 = mWorldY0 = 0;
}

ShapesFrame::~ShapesFrame(void)
{

}

bool ShapesFrame::operator==(const ShapesFrame& other) const
{
	return mXmirror == other.mXmirror
		&& mYmirror == other.mYmirror
		&& mKeypointObscured == other.mKeypointObscured
		&& mMinimumLightIntensity == other.mMinimumLightIntensity
		&& mBitmapIndex == other.mBitmapIndex
		&& mScaleFactor == other.mScaleFactor
		&& mOriginX == other.mOriginX
		&& mOriginY == other.mOriginY
		&& mKeyX == other.mKeyX
		&& mKeyY == other.mKeyY;
}

BigEndianBuffer& ShapesFrame::SaveObject(BigEndianBuffer& buffer)
{
	unsigned short	flags = 0;
	float			mli_integer, mli_fractional;
	long			min_light_intensity = 0;
	
	if (mXmirror)
		flags |= X_MIRROR;
	if (mYmirror)
		flags |= Y_MIRROR;
	if (mKeypointObscured)
		flags |= KEYPOINT_OBSCURED;
		
	min_light_intensity = static_cast<long>(mMinimumLightIntensity * 65536.0 + 0.5); // convert float to 16.16 fixed

	buffer.WriteUShort(flags);
	buffer.WriteLong(min_light_intensity);
	buffer.WriteShort(mBitmapIndex);
	buffer.WriteShort(mOriginX);
	buffer.WriteShort(mOriginY);
	buffer.WriteShort(mKeyX);
	buffer.WriteShort(mKeyY);
	buffer.WriteShort(mWorldLeft);
	buffer.WriteShort(mWorldRight);
	buffer.WriteShort(mWorldTop);
	buffer.WriteShort(mWorldBottom);
	buffer.WriteShort(mWorldX0);
	buffer.WriteShort(mWorldY0);
	buffer.WriteZeroes(8);
	
	return buffer;
}

BigEndianBuffer& ShapesFrame::SavePatch(BigEndianBuffer& buffer, int index)
{
	buffer.WriteLong(FOUR_CHARS_TO_INT('l','l','s','h'));
	buffer.WriteLong(index);
	return SaveObject(buffer);
}

BigEndianBuffer& ShapesFrame::LoadObject(BigEndianBuffer& buffer, unsigned int offset)
{
	unsigned short	flags;
	wxInt32			mli_fixed;

	buffer.Position(offset);

	flags = buffer.ReadUShort();

	mXmirror = flags & X_MIRROR;
	mYmirror = flags & Y_MIRROR;
	mKeypointObscured = flags & KEYPOINT_OBSCURED;
	
	mli_fixed = buffer.ReadLong();
	
	mMinimumLightIntensity = mli_fixed / 65536.0; // convert 16.16 fixed to float
	
	mBitmapIndex = buffer.ReadShort();
	mOriginX = buffer.ReadShort();
	mOriginY = buffer.ReadShort();
	mKeyX = buffer.ReadShort();
	mKeyY = buffer.ReadShort();
	mWorldLeft = buffer.ReadShort();
	mWorldRight = buffer.ReadShort();
	mWorldTop = buffer.ReadShort();
	mWorldBottom = buffer.ReadShort();
	mWorldX0 = buffer.ReadShort();
	mWorldY0 = buffer.ReadShort();
	
	if (IsVerbose()) {
		wxLogDebug(wxT("[ShapesFrame]         Flags:			%d"), flags);
		wxLogDebug(wxT("[ShapesFrame]         Min. Light Intensity:	%f"), mMinimumLightIntensity);
		wxLogDebug(wxT("[ShapesFrame]         Bitmap Index:	%d"), mBitmapIndex);
		wxLogDebug(wxT("[ShapesFrame]         Origin (X):		%d"), mOriginX);
		wxLogDebug(wxT("[ShapesFrame]         Origin (Y):		%d"), mOriginY);
		wxLogDebug(wxT("[ShapesFrame]         Key (X):		%d"), mKeyX);
		wxLogDebug(wxT("[ShapesFrame]         Key (Y):		%d"), mKeyY);
		wxLogDebug(wxT("[ShapesFrame]         World (Left):	%d"), mWorldLeft);
		wxLogDebug(wxT("[ShapesFrame]         World (Right):	%d"), mWorldRight);
		wxLogDebug(wxT("[ShapesFrame]         World (Top):	%d"), mWorldTop);
		wxLogDebug(wxT("[ShapesFrame]         World (Bottom):	%d"), mWorldBottom);
		wxLogDebug(wxT("[ShapesFrame]         World (X0):		%d"), mWorldX0);
		wxLogDebug(wxT("[ShapesFrame]         World (Y0):		%d"), mWorldY0);
	}

	mGoodData = true;
	return buffer;
}

ShapesSequence::ShapesSequence(bool verbose): ShapesElement(verbose)
{
	// initialize values to something reasonable
	mType = 0;
	mFlags = 0;
	mName = _T("new sequence");
	mNumberOfViews = UNANIMATED;
	mFramesPerView = 0;
	mTicksPerFrame = 1;
	mKeyFrame = 0;
	mTransferMode = 0;
	mTransferModePeriod = 1;
	mFirstFrameSound = mKeyFrameSound = mLastFrameSound = -1;
	mPixelsToWorld = 0;
	mLoopFrame = 0;
}

ShapesSequence::~ShapesSequence(void)
{

}

bool ShapesSequence::operator==(const ShapesSequence& other) const
{
	if (mType == other.mType 
	    && mFlags == other.mFlags 
	    && mName == other.mName
	    && mNumberOfViews == other.mNumberOfViews
	    && mFramesPerView == other.mFramesPerView
	    && mTicksPerFrame == other.mTicksPerFrame
	    && mKeyFrame == other.mKeyFrame
	    && mTransferMode == other.mTransferMode
	    && mTransferModePeriod == other.mTransferModePeriod
	    && mFirstFrameSound == other.mFirstFrameSound
	    && mKeyFrameSound == other.mKeyFrameSound
	    && mLastFrameSound == other.mLastFrameSound
	    && mPixelsToWorld == other.mPixelsToWorld
	    && mLoopFrame == other.mLoopFrame
	    && mFrameIndexes.size() == other.mFrameIndexes.size()) {
		return std::equal(mFrameIndexes.begin(), mFrameIndexes.end(), other.mFrameIndexes.begin());
	} else {
		return false;
	}
}

unsigned int ShapesSequence::SizeInFile() const
{
	return 2 * (FrameIndexCount() + 1);
}

BigEndianBuffer& ShapesSequence::SaveObject(BigEndianBuffer& buffer)
{
	wxCSConv	seqnameconv(wxFONTENCODING_MACROMAN);
	char		name[33] = "";

	buffer.WriteShort(mType);
	buffer.WriteUShort(mFlags);
	buffer.WriteChar(mName.Length());
	strncpy(name, seqnameconv.cWC2MB(mName.wc_str(*wxConvCurrent)), 33);
	buffer.WriteBlock(33, (unsigned char *)name);
	buffer.WriteShort(mNumberOfViews);
	buffer.WriteShort(mFramesPerView);
	buffer.WriteShort(mTicksPerFrame);
	buffer.WriteShort(mKeyFrame);
	buffer.WriteShort(mTransferMode);
	buffer.WriteShort(mTransferModePeriod);
	buffer.WriteShort(mFirstFrameSound);
	buffer.WriteShort(mKeyFrameSound);
	buffer.WriteShort(mLastFrameSound);
	buffer.WriteShort(mPixelsToWorld);
	buffer.WriteShort(mLoopFrame);
	buffer.WriteZeroes(28);
	for (unsigned int i = 0; i < mFrameIndexes.size(); i++)
		buffer.WriteShort(mFrameIndexes[i]);
	buffer.WriteShort(0);
	
	return buffer;
}

BigEndianBuffer& ShapesSequence::SavePatch(BigEndianBuffer& buffer, int index)
{
	buffer.WriteLong(FOUR_CHARS_TO_INT('h','l','s','h'));
	buffer.WriteLong(index);
	buffer.WriteLong(SizeInFile() + SIZEOF_high_level_shape_definition);
	return SaveObject(buffer);
}

BigEndianBuffer& ShapesSequence::LoadObject(BigEndianBuffer& buffer, long offset)
{
	buffer.Position(offset);
	mType = buffer.ReadShort();
	mFlags = buffer.ReadUShort();
	
	// the mName is a Mac Pascal string, not a C string (length,chars)
	unsigned char	namelen = buffer.ReadUChar();

	if (namelen > 32) {
		wxLogError(wxT("[ShapesSequence] Sequence name too long (%d/32)"), namelen);
		return buffer;
	}

	char		name[33];
	wxCSConv	seqnameconv(wxFONTENCODING_MACROMAN);

	buffer.ReadBlock(33, (unsigned char *)name);
	name[namelen] = 0;
	mName = wxString(seqnameconv.cMB2WC(name), *wxConvCurrent, namelen);

	mNumberOfViews = buffer.ReadShort();
	mFramesPerView = buffer.ReadShort();
	mTicksPerFrame = buffer.ReadShort();
	mKeyFrame = buffer.ReadShort();
	mTransferMode = buffer.ReadShort();
	mTransferModePeriod = buffer.ReadShort();
	mFirstFrameSound = buffer.ReadShort();
	mKeyFrameSound = buffer.ReadShort();
	mLastFrameSound = buffer.ReadShort();
	mPixelsToWorld = buffer.ReadShort();
	mLoopFrame = buffer.ReadShort();
	buffer.Position(buffer.Position() + 28);

	if (IsVerbose()) {
		wxLogDebug(wxT("[ShapesSequence]         Type:					%d"), mType);
		wxLogDebug(wxT("[ShapesSequence]         Flags:					%d"), mFlags);
		wxLogDebug(wxT("[ShapesSequence]         Name:					%s"), mName.c_str());
		wxLogDebug(wxT("[ShapesSequence]         Number of Views:		%d"), mNumberOfViews);
		wxLogDebug(wxT("[ShapesSequence]         Frames/Views:			%d"), mFramesPerView);
		wxLogDebug(wxT("[ShapesSequence]         Ticks/Frame:			%d"), mTicksPerFrame);
		wxLogDebug(wxT("[ShapesSequence]         Key Frame:				%d"), mKeyFrame);
		wxLogDebug(wxT("[ShapesSequence]         Transfer Mode:			%d"), mTransferMode);
		wxLogDebug(wxT("[ShapesSequence]         Transfer Mode Period:	%d"), mTransferModePeriod);
		wxLogDebug(wxT("[ShapesSequence]         First Frame Sound:		%d"), mFirstFrameSound);
		wxLogDebug(wxT("[ShapesSequence]         Key Frame Sound:		%d"), mKeyFrameSound);
		wxLogDebug(wxT("[ShapesSequence]         Last Frame Sound:		%d"), mLastFrameSound);
		wxLogDebug(wxT("[ShapesSequence]         Pixels to World:		%d"), mPixelsToWorld);
		wxLogDebug(wxT("[ShapesSequence]         Loop Frame:			%d"), mLoopFrame);
	}

	if (mNumberOfViews < 0 || mFramesPerView < 0) {
		wxLogError(wxT("[ShapesSequence] Invalid sequence type parameters: numberOfViews=%d, framesPerView=%d"),
						mNumberOfViews, mFramesPerView);
		return buffer;
	}
	// guess these shouldn't be < 0, but RED Shapes have a case with mKeyFrame=-1
	if (mKeyFrame < -1 || mLoopFrame < -1) {
		wxLogError(wxT("[ShapesSequence] Invalid key/loop frame values in sequence data: keyFrame=%d, loopFrame=%d"),
						mKeyFrame, mLoopFrame);
		return buffer;
	}
	if (mFirstFrameSound < -1 || mKeyFrameSound < -1 || mLastFrameSound < -1) {
		wxLogError(wxT("[ShapesSequence] Invalid sound values in sequence data: firstFrameSound=%d, keyFrameSound=%d, lastFrameSound=%d"),
						mFirstFrameSound, mKeyFrameSound, mLastFrameSound);
		return buffer;
	}

	// load frame indexes
	int	n = ActualNumberOfViews(mNumberOfViews) * mFramesPerView;

	if (n > 0) {
		for (int k = 0; k < n; k++)
			mFrameIndexes.push_back(buffer.ReadShort());
	}

	buffer.ReadShort();	// terminating index (usually 0 but can be garbage)

	mGoodData = true;
	return buffer;
}

// given a high_level_shape_definition.mNumberOfViews value,
// return the real number of views
int ActualNumberOfViews(int t)
{
	switch (t) {
		case UNANIMATED:
		case ANIMATED_1:
			return 1;
		case ANIMATED_3TO4:
		case ANIMATED_4:
			return 4;
		case ANIMATED_3TO5:
		case ANIMATED_5:
			return 5;
		case ANIMATED_2TO8:
		case ANIMATED_5TO8:
		case ANIMATED_8:
			return 8;
		default:
			wxLogError(wxT("[ShapesSequence] Unknown sequence type %d, don't know the number of views"), t);
			return t;
	}
	return -1;
}

ShapesChunk::ShapesChunk(bool verbose): ShapesElement(verbose)
{
	
}

ShapesChunk::~ShapesChunk(void)
{
	Clear();
}

void ShapesChunk::Clear(void)
{
	unsigned int i;

	for (i = 0; i < mColorTables.size(); i++)
		delete mColorTables[i];
	for (i = 0; i < mSequences.size(); i++)
		delete mSequences[i];
	for (i = 0; i < mFrames.size(); i++)
		delete mFrames[i];
	for (i = 0; i < mBitmaps.size(); i++)
		delete mBitmaps[i];

	mColorTables.clear();
	mSequences.clear();
	mFrames.clear();
	mBitmaps.clear();

	mGoodData = false;
}

static bool compareColorTablePtrs(ShapesColorTable* lhs, ShapesColorTable* rhs) {
	return *lhs == *rhs;
}

static bool compareSequencePtrs(ShapesSequence* lhs, ShapesSequence* rhs) {
	return *lhs == *rhs;
}

static bool compareFramePtrs(ShapesFrame* lhs, ShapesFrame* rhs) {
	return *lhs == *rhs;
}

static bool compareBitmapPtrs(ShapesBitmap* lhs, ShapesBitmap* rhs) {
	return *lhs == *rhs;
}

bool ShapesChunk::operator==(const ShapesChunk& other) const
{
	if (mVersion == other.mVersion
	    && mType == other.mType
	    && mFlags == other.mFlags 
	    && mPixelsToWorld == other.mPixelsToWorld
	    && mColorTables.size() == other.mColorTables.size() 
	    && mSequences.size() == other.mSequences.size()
	    && mFrames.size() == other.mFrames.size()
	    && mBitmaps.size() == other.mBitmaps.size()) {
		if (!std::equal(mColorTables.begin(), mColorTables.end(), other.mColorTables.begin(), compareColorTablePtrs))
			return false;
		if (!std::equal(mSequences.begin(), mSequences.end(), other.mSequences.begin(), compareSequencePtrs))
			return false;
		if (!std::equal(mFrames.begin(), mFrames.end(), other.mFrames.begin(), compareFramePtrs))
			return false;
		return (std::equal(mBitmaps.begin(), mBitmaps.end(), other.mBitmaps.begin(), compareBitmapPtrs));
	} else {
		return false;
	}
}

ShapesColorTable* ShapesChunk::GetColorTable(unsigned int index) const
{
	if (index < 0 || index > mColorTables.size())
		return NULL;
	return mColorTables[index];
}

ShapesBitmap* ShapesChunk::GetBitmap(unsigned int index) const
{
	if (index < 0 || index > mBitmaps.size())
		return NULL;
	return mBitmaps[index];
}

ShapesFrame* ShapesChunk::GetFrame(unsigned int index) const
{
	if (index < 0 || index > mFrames.size())
		return NULL;
	return mFrames[index];
}

ShapesSequence* ShapesChunk::GetSequence(unsigned int index) const
{
	if (index < 0 || index > mSequences.size())
		return NULL;
	return mSequences[index];
}

void ShapesChunk::InsertColorTable(ShapesColorTable *ct)
{
	mColorTables.push_back(ct);
}

void ShapesChunk::DeleteColorTable(unsigned int ct)
{
	mColorTables.erase(mColorTables.begin() + ct);
}

void ShapesChunk::InsertBitmap(ShapesBitmap *b)
{
	mBitmaps.push_back(b);
}

void ShapesChunk::DeleteBitmap(unsigned int b)
{
	if (b < mBitmaps.size()) {
		// preserve existing frame-bitmap associations and associate
		// a null bitmap to frames using the bitmap we're deleting
		for (unsigned int i = 0; i < mFrames.size(); i++) {
			short	bitmap_index = mFrames[i]->BitmapIndex();

			if (bitmap_index == (int)b)
				mFrames[i]->SetBitmapIndex(-1);
			else if (bitmap_index > (int)b)
				mFrames[i]->SetBitmapIndex(bitmap_index - 1);
		}
		// now actually delete the bitmap
		mBitmaps.erase(mBitmaps.begin() + b);
	}
}

void ShapesChunk::InsertFrame(ShapesFrame *f)
{
	mFrames.push_back(f);
}

void ShapesChunk::DeleteFrame(unsigned int f)
{
	if (f < mFrames.size()) {
		// preserve existing sequence-frame associations and
		// unreference this frame index from any sequence using it
		for (unsigned int i = 0; i < mSequences.size(); i++) {
			for (unsigned int j = 0; j < mSequences[i]->FrameIndexCount(); j++) {
				short frame_index = mSequences[i]->GetFrameIndex(j);

				if (frame_index == (int)f)
					mSequences[i]->SetFrameIndex(j, -1);
				else if (frame_index > (int)f)
					mSequences[i]->SetFrameIndex(j, frame_index - 1);
			}
		}
		// now actually delete the frame
		mFrames.erase(mFrames.begin() + f);
	}
}

void ShapesChunk::InsertSequence(ShapesSequence *s)
{
	mSequences.push_back(s);
}

void ShapesChunk::DeleteSequence(unsigned int s)
{
	if (s < mSequences.size())
		mSequences.erase(mSequences.begin() + s);
}

void ShapesChunk::ClipboardCopy()
{
	if (wxTheClipboard->Open()) {
		size_t size = SizeInFile();
		unsigned char* data = new unsigned char[size];
		BigEndianBuffer buffer(data, size);
		
		SaveObject(buffer);
		
		wxCustomDataObject* dataObject = new wxCustomDataObject(wxDataFormat(wxT("application/vnd.shapefusion.shapeschunk")));
		dataObject->TakeData(size, data);
		
		wxTheClipboard->SetData(dataObject);

		wxTheClipboard->Close();
	}
}

void ShapesChunk::ClipboardPaste()
{
	if (wxTheClipboard->Open()) {
		wxCustomDataObject dataObject(wxDataFormat(wxT("application/vnd.shapefusion.shapeschunk")));
		if (wxTheClipboard->GetData(dataObject)) {
			BigEndianBuffer buffer(reinterpret_cast<unsigned char *>(dataObject.GetData()), dataObject.GetSize());

			Clear();

			LoadObject(buffer);
		}

		wxTheClipboard->Close();
	}
}

unsigned int ShapesChunk::SizeInFile(void) const
{
	unsigned int	bitmap_count = BitmapCount(),
					frame_count = FrameCount(),
					sequence_count = SequenceCount(),
					color_table_count = ColorTableCount(),
					size;
	
	// size of our definition
	size = SIZEOF_collection_definition;
	// add contribute of sequence offset table
	size += 4 * sequence_count;
	// add contribute of bitmap offset table
	size += 4 * bitmap_count;
	// add contribute of frame offset table
	size += 4 * frame_count;
	// add contribute of color tables
	if (color_table_count > 0)
		size += SIZEOF_rgb_color_value * color_table_count * GetColorTable(0)->ColorCount();
	// add contribute of bitmaps
	size += SIZEOF_bitmap_definition * bitmap_count;
	for (unsigned int i = 0; i < bitmap_count; i++) {
		ShapesBitmap	*bitmap = mBitmaps[i];

		size += bitmap->SizeInFile();
	}		
	// add contribute of frame definitions
	size += SIZEOF_low_level_shape_definition * frame_count;
	// add contribute of sequence definitions (and following frame indexes)
	size += SIZEOF_high_level_shape_definition * sequence_count;
	for (unsigned int i = 0 ; i < sequence_count ; i++) {
		ShapesSequence	*seq = mSequences[i];

		size += seq->SizeInFile();
	}

	return size;
}

unsigned int ShapesChunk::SizeInPatch(const ShapesChunk* other) const {
	unsigned int size = 4; // 'cldf'

	size += SIZEOF_collection_definition;
	
	for (unsigned int i = 0; i < mColorTables.size(); ++i) {
		if (other == NULL || i >= other->mColorTables.size() || *mColorTables[i] != *other->mColorTables[i]) {
			size += SIZEOF_rgb_color_value * mColorTables[i]->ColorCount() + 8;
		}
	}

	for (unsigned int i = 0; i < mSequences.size(); ++i) {
		if (other == NULL || i >= other->mSequences.size() || *mSequences[i] != *other->mSequences[i]) {
			size += SIZEOF_high_level_shape_definition + mSequences[i]->SizeInPatch();
		}
	}


	for (unsigned int i = 0; i < mFrames.size(); ++i) {
		if (other == NULL || i >= other->mFrames.size() || *mFrames[i] != *other->mFrames[i]) {
			size += SIZEOF_low_level_shape_definition + 8;
		}
	}
	
	for (unsigned int i = 0; i < mBitmaps.size(); ++i) {
		if (other == NULL || i >= other->mBitmaps.size() || *mBitmaps[i] != *other->mBitmaps[i]) {
			size += SIZEOF_bitmap_definition + mBitmaps[i]->SizeInPatch();
		}
	}

	return size;
}

BigEndianBuffer& ShapesChunk::SaveObject(BigEndianBuffer& buffer)
{
	unsigned int	bitmap_count = BitmapCount(),
					frame_count = FrameCount(),
					sequence_count = SequenceCount(),
					i;
	long			sequence_table_offset,
					sequence_offsets[sequence_count],
					frame_table_offset,
					frame_offsets[frame_count],
					bitmap_table_offset,
					bitmap_offsets[bitmap_count];
				
	// skip the collection definition, we'll fill it at the end
	buffer.Position(SIZEOF_collection_definition);
	// write color tables
	for (i = 0; i < ColorTableCount(); i++)
		mColorTables[i]->SaveObject(buffer);
	
	// write sequences
	sequence_table_offset = buffer.Position();
	if (sequence_count > 0) {
		buffer.Position(buffer.Position() + sequence_count * 4);
		
		for (i = 0; i < sequence_count; i++) {
			sequence_offsets[i] = buffer.Position();
			
			mSequences[i]->SaveObject(buffer);
		}
	}
	// write frames
	frame_table_offset = buffer.Position();
	buffer.Position(buffer.Position() + frame_count * 4);
	for (i = 0; i < frame_count; i++) {
		frame_offsets[i] = buffer.Position();
		
		mFrames[i]->SaveObject(buffer);
	}
	
	// write bitmaps
	bitmap_table_offset = buffer.Position();
	buffer.Position(buffer.Position() + bitmap_count * 4);
	for (i = 0; i < bitmap_count; i++) {
		bitmap_offsets[i] = buffer.Position();
		
		mBitmaps[i]->SaveObject(buffer);
	}
	
	// go back and write the collection definition (with correct offsets)
	buffer.Position(0);
	buffer.WriteShort(mVersion);
	buffer.WriteShort(mType);
	buffer.WriteUShort(mFlags);
	buffer.WriteShort(GetColorTable(0)->ColorCount());
	buffer.WriteShort(ColorTableCount());
	buffer.WriteLong(SIZEOF_collection_definition);
	buffer.WriteShort(sequence_count);
	buffer.WriteLong(sequence_table_offset);
	buffer.WriteShort(frame_count);
	buffer.WriteLong(frame_table_offset);
	buffer.WriteShort(bitmap_count);
	buffer.WriteLong(bitmap_table_offset);
	buffer.WriteShort(mPixelsToWorld);
	buffer.WriteLong(SizeInFile());
	buffer.WriteZeroes(506);
	
	// fill offset tables
	if (bitmap_count > 0) {
		buffer.Position(bitmap_table_offset);
		for (i = 0; i < bitmap_count; i++)
			buffer.WriteLong(bitmap_offsets[i]);
	}
	if (frame_count > 0) {
		buffer.Position(frame_table_offset);
		for (i = 0; i < frame_count; i++)
			buffer.WriteLong(frame_offsets[i]);
	}
	if (sequence_count > 0) {
		buffer.Position(sequence_table_offset);
		for (i = 0; i < sequence_count; i++)
			buffer.WriteLong(sequence_offsets[i]);
	}
	
	return buffer;
}

BigEndianBuffer& ShapesChunk::SavePatch(BigEndianBuffer& buffer, const ShapesChunk* other)
{
	buffer.WriteLong(FOUR_CHARS_TO_INT('c','l','d','f'));

	// collection header
	buffer.WriteShort(mVersion);
	buffer.WriteShort(mType);
	buffer.WriteUShort(mFlags);
	buffer.WriteShort(GetColorTable(0)->ColorCount());
	buffer.WriteShort(ColorTableCount());
	buffer.WriteLong(SIZEOF_collection_definition);
	buffer.WriteShort(SequenceCount());
	buffer.WriteLong(0);
	buffer.WriteShort(FrameCount());
	buffer.WriteLong(0);
	buffer.WriteShort(BitmapCount());
	buffer.WriteLong(0);
	buffer.WriteShort(mPixelsToWorld);
	buffer.WriteLong(0);
	buffer.WriteZeroes(506);

	for (unsigned int i = 0; i < mColorTables.size(); ++i) {
		if (other == NULL || i >= other->mColorTables.size() || *mColorTables[i] != *other->mColorTables[i]) {
			mColorTables[i]->SavePatch(buffer, i);
		}
	}

	for (unsigned int i = 0; i < mSequences.size(); ++i) {
		if (other == NULL || i >= other->mSequences.size() || *mSequences[i] != *other->mSequences[i]) {
			mSequences[i]->SavePatch(buffer, i);
		}
	}

	for (unsigned int i = 0; i < mFrames.size(); ++i) {
		if (other == NULL || i >= other->mFrames.size() || *mFrames[i] != *other->mFrames[i]) {
			mFrames[i]->SavePatch(buffer, i);
		}
	}

	for (unsigned int i = 0; i < mBitmaps.size(); ++i) {
		if (other == NULL || i >= other->mBitmaps.size() || *mBitmaps[i] != *other->mBitmaps[i]) {
			mBitmaps[i]->SavePatch(buffer, i);
		}
	}

	return buffer;
}

BigEndianBuffer& ShapesChunk::LoadObject(BigEndianBuffer& buffer)
{
	short	color_count,
			clut_count,
			bitmap_count,
			high_level_shape_count,
			low_level_shape_count,
			i;
	long	color_table_offset,
			high_level_shape_offset_table_offset,
			low_level_shape_offset_table_offset,
			bitmap_offset_table_offset,
			oldpos,
			offset,
			size;

	mVersion = buffer.ReadShort();
	mType = buffer.ReadShort();
	mFlags = buffer.ReadUShort();
	color_count = buffer.ReadShort();
	clut_count = buffer.ReadShort();
	color_table_offset = buffer.ReadLong();
	high_level_shape_count = buffer.ReadShort();
	high_level_shape_offset_table_offset = buffer.ReadLong();
	low_level_shape_count = buffer.ReadShort();
	low_level_shape_offset_table_offset = buffer.ReadLong();
	bitmap_count = buffer.ReadShort();
	bitmap_offset_table_offset = buffer.ReadLong();
	mPixelsToWorld = buffer.ReadShort();
	size = buffer.ReadLong();

	// validate values
	if (mVersion != COLLECTION_VERSION) {
		wxLogError(wxT("[ShapesChunk] Unknown collection version %d"), mVersion);
		return buffer;
	}

	if ((unsigned long)size != buffer.Size()) {
		wxLogError(wxT("[ShapesChunk] Chunk size mismatch (%ld/%d): this may not be a Marathon shapes file"), size, buffer.Size());
		return buffer;
	}
	if (color_table_offset < SIZEOF_collection_definition
		|| color_table_offset >= size
		|| high_level_shape_offset_table_offset < SIZEOF_collection_definition
		|| high_level_shape_offset_table_offset >= size
		|| low_level_shape_offset_table_offset < SIZEOF_collection_definition
		|| low_level_shape_offset_table_offset >= size
		|| bitmap_offset_table_offset < SIZEOF_collection_definition
		|| bitmap_offset_table_offset >= size) {
		wxLogError(wxT("[ShapesChunk] Invalid offsets in collection definition: this may not be a Marathon shapes file"));
		return buffer;
	}
	if (color_count < 0 || clut_count < 0 || high_level_shape_count < 0 || low_level_shape_count < 0 || bitmap_count < 0) {
		wxLogError(wxT("[ShapesChunk] Invalid object counts in collection definition: this may not be a Marathon shapes file"));
		return buffer;
	}

	if (IsVerbose()) {
		wxLogDebug(wxT("[ShapesChunk]         Version: %d"), mVersion);
		wxLogDebug(wxT("[ShapesChunk]         Type:    %d"), mType);
		wxLogDebug(wxT("[ShapesChunk]         Flags:   %d"), mFlags);
		wxLogDebug(wxT("[ShapesChunk]         %d color tables, %d colors per table"), clut_count, color_count);
		wxLogDebug(wxT("[ShapesChunk]         %d sequences"), high_level_shape_count);
		wxLogDebug(wxT("[ShapesChunk]         %d frames"), low_level_shape_count);
		wxLogDebug(wxT("[ShapesChunk]         %d bitmaps"), bitmap_count);
	}

	// load color tables
	for (i = 0; i < clut_count; i++) {
		ShapesColorTable	*color_table = new ShapesColorTable(IsVerbose());

		if (IsVerbose())
			wxLogDebug(wxT("[ShapesChunk] Loading colortable %d/%d"), i+1, clut_count);

		oldpos = buffer.Position();

		color_table->LoadObject(buffer, color_table_offset + i * color_count * SIZEOF_rgb_color_value, color_count);

		buffer.Position(oldpos);
		
		// we stop if an error occured
		if (!color_table->IsGood()) {
			wxLogError(wxT("[ShapesChunk] Error loading color table %d... Dropped"), i);
			return buffer;
		}
		
		mColorTables.push_back(color_table);
	}
	
	// load bitmaps, decoding compressed ones
	buffer.Position(bitmap_offset_table_offset);
	for (i = 0; i < bitmap_count; i++) {
		offset = buffer.ReadLong();
		if (offset < SIZEOF_collection_definition || offset >= size) {
			wxLogError(wxT("[ShapesChunk] Invalid bitmap offset: this may not be a Marathon shapes file"));
			return buffer;
		}
		
		ShapesBitmap	*bitmap = new ShapesBitmap(IsVerbose());
		if (IsVerbose())
			wxLogDebug(wxT("[ShapesChunk] Loading bitmap %d/%d"), i+1, bitmap_count);
			
		oldpos = buffer.Position();
		
		bitmap->LoadObject(buffer, offset);
		
		buffer.Position(oldpos);
		
		// we stop if an error occured
		if (!bitmap->IsGood()) {
			wxLogError(wxT("[ShapesDocument] Error loading bitmap %d... Dropped"), i);
			return buffer;
		}
		
		mBitmaps.push_back(bitmap);
	}

	// load sequences
	buffer.Position(high_level_shape_offset_table_offset);
	for (i = 0; i < high_level_shape_count; i++) {
		offset = buffer.ReadLong();
		if (offset < SIZEOF_collection_definition || offset >= size) {
			wxLogError(wxT("[ShapesChunk] Invalid sequence offset: this may not be a Marathon shapes file"));
			return buffer;
		}
		
		ShapesSequence	*sequence = new ShapesSequence(IsVerbose());
		if (IsVerbose())
			wxLogDebug(wxT("[ShapesChunk] Loading sequence %d/%d"), i+1, high_level_shape_count);
		
		oldpos = buffer.Position();
		
		sequence->LoadObject(buffer, offset);
		
		buffer.Position(oldpos);
		
		// we stop if an error occured
		if (!sequence->IsGood()) {
			wxLogError(wxT("[ShapesDocument] Error loading sequence... Dropped"));
			return buffer;
		}
		
		mSequences.push_back(sequence);
	}
	
	// load frames
	buffer.Position(low_level_shape_offset_table_offset);
	for (i = 0; i < low_level_shape_count; i++) {
		offset = buffer.ReadLong();
		if (offset < SIZEOF_collection_definition || offset >= size) {
			wxLogError(wxT("[ShapesChunk] Invalid frame offset: this may not be a Marathon shapes file"));
			return buffer;
		}
		
		ShapesFrame	*frame = new ShapesFrame(IsVerbose());
		if (IsVerbose())
			wxLogDebug(wxT("[ShapesChunk] Loading frame %d/%d"), i+1, low_level_shape_count);

		oldpos = buffer.Position();
		
		frame->LoadObject(buffer, offset);
		
		buffer.Position(oldpos);
		// calculate scale factor from world_* fields and associated bitmap dimensions.
		// If this fails, default to collection global scale factor
		if (frame->BitmapIndex() >= 0 && frame->BitmapIndex() < (int)mBitmaps.size()) {
			int	bitmapWidth = mBitmaps[frame->BitmapIndex()]->Width();

			if (bitmapWidth > 0)
				frame->SetScaleFactor((frame->WorldRight() - frame->WorldLeft()) / bitmapWidth);
			else
				frame->SetScaleFactor(mPixelsToWorld);
		} else {
			frame->SetScaleFactor(mPixelsToWorld);
		}
		
		// store if correct
		if (!frame->IsGood()) {
			wxLogError(wxT("[ShapesDocument] Error loading frame %d... Dropped"), i);
			return buffer;
		}
		
		mFrames.push_back(frame);
	}
	
	mGoodData = true;
	return buffer;
}

ShapesCollection::ShapesCollection(bool verbose): ShapesElement(verbose)
{
	mChunks[0] = NULL;
	mChunks[1] = NULL;
}

ShapesCollection::~ShapesCollection(void)
{
	if (mChunks[0])
		delete mChunks[0];
	if (mChunks[1])
		delete mChunks[1];
}

bool ShapesCollection::Defined(unsigned int chunk) const
{
	if (chunk > COLL_VERSION_TRUECOLOR)
		return false;
	return mChunks[chunk] != NULL;
}

int ShapesCollection::Version(unsigned int chunk) const
{
	return (Defined(chunk) ? mChunks[chunk]->Version() : 0);
}

int ShapesCollection::Type(unsigned int chunk) const
{
	return (Defined(chunk) ? mChunks[chunk]->Type() : 0);
}

int ShapesCollection::Flags(unsigned int chunk) const
{
	return (Defined(chunk) ? mChunks[chunk]->Flags() : 0);
}

int ShapesCollection::ScaleFactor(unsigned int chunk) const
{
	return (Defined(chunk) ? mChunks[chunk]->ScaleFactor() : 0);
}

int ShapesCollection::ColorTableCount(unsigned int chunk) const
{
	return (Defined(chunk) ? mChunks[chunk]->ColorTableCount() : 0);
}

int ShapesCollection::BitmapCount(unsigned int chunk) const
{
	return (Defined(chunk) ? mChunks[chunk]->BitmapCount() : 0);
}

int ShapesCollection::FrameCount(unsigned int chunk) const
{
	return (Defined(chunk) ? mChunks[chunk]->FrameCount() : 0);
}

int ShapesCollection::SequenceCount(unsigned int chunk) const
{
	return (Defined(chunk) ? mChunks[chunk]->SequenceCount() : 0);
}

ShapesColorTable* ShapesCollection::GetColorTable(unsigned int chunk, unsigned int index) const
{
	return (Defined(chunk) ? mChunks[chunk]->GetColorTable(index) : NULL);
}

ShapesBitmap* ShapesCollection::GetBitmap(unsigned int chunk, unsigned int index) const
{
	return (Defined(chunk) ? mChunks[chunk]->GetBitmap(index) : NULL);
}

ShapesFrame* ShapesCollection::GetFrame(unsigned int chunk, unsigned int index) const
{
	return (Defined(chunk) ? mChunks[chunk]->GetFrame(index) : NULL);
}

ShapesSequence* ShapesCollection::GetSequence(unsigned int chunk, unsigned int index) const
{
	return (Defined(chunk) ? mChunks[chunk]->GetSequence(index) : NULL);
}

ShapesChunk* ShapesCollection::GetChunk(unsigned int chunk) const
{
	return (Defined(chunk) ? mChunks[chunk] : NULL);
}

void ShapesCollection::InsertColorTable(ShapesColorTable *ct, unsigned int chunk)
{
	if (Defined(chunk))
		mChunks[chunk]->InsertColorTable(ct);
}

void ShapesCollection::DeleteColorTable(unsigned int chunk, unsigned int ct)
{
	if (Defined(chunk))
		mChunks[chunk]->DeleteColorTable(ct);
}

void ShapesCollection::InsertBitmap(ShapesBitmap *b, unsigned int chunk)
{
	if (Defined(chunk))
		mChunks[chunk]->InsertBitmap(b);
}

void ShapesCollection::DeleteBitmap(unsigned int chunk, unsigned int b)
{
	if (Defined(chunk))
		mChunks[chunk]->DeleteBitmap(b);
}

void ShapesCollection::InsertFrame(ShapesFrame *f, unsigned int chunk)
{
	if (Defined(chunk))
		mChunks[chunk]->InsertFrame(f);
}

void ShapesCollection::DeleteFrame(unsigned int chunk, unsigned int f)
{
	if (Defined(chunk))
		mChunks[chunk]->DeleteFrame(f);
}

void ShapesCollection::InsertSequence(ShapesSequence *s, unsigned int chunk)
{
	if (Defined(chunk))
		mChunks[chunk]->InsertSequence(s);
}

void ShapesCollection::DeleteSequence(unsigned int chunk, unsigned int s)
{
	if (Defined(chunk))
		mChunks[chunk]->DeleteSequence(s);
}

// calculate how much space a collection is going
// to take when encoded to its on-file format.
unsigned int ShapesCollection::SizeInFile(unsigned int chunk) const
{
	if (!Defined(chunk))
		return 0;

	unsigned int	size = 0;

	size += mChunks[chunk]->SizeInFile();
	return size;
}

#if wxUSE_STD_IOSTREAM
wxSTD ostream& ShapesCollection::SaveObject(wxSTD ostream& stream)
#else
wxOutputStream& ShapesCollection::SaveObject(wxOutputStream& stream)
#endif
{
	for (unsigned int i = 0; i < 2 ; i++) {
		if (Defined(i)) {
			BigEndianBuffer	chunkbuffer(mChunks[i]->SizeInFile());

			mChunks[i]->SaveObject(chunkbuffer);
#if wxUSE_STD_IOSTREAM
			stream.write((char *)chunkbuffer.Data(), chunkbuffer.Size());
#else
			stream.Write((char *)chunkbuffer.Data(), chunkbuffer.Size());
#endif
		}
	}
	return stream;
}

#if wxUSE_STD_IOSTREAM
wxSTD ostream& ShapesCollection::SavePatch(wxSTD ostream& stream, const ShapesCollection& other, int index, int depth)
#else
wxOutputStream& ShapesCollection::SavePatch(wxOutputStream& stream, const ShapesCollection& other, int index, int depth)
#endif
{
	bool diff = Defined(depth) && (other.mChunks[depth] == NULL || *mChunks[depth] != *other.mChunks[depth]);
	unsigned int size = 12;
	if (diff) {
		size += mChunks[depth]->SizeInPatch(other.mChunks[depth]);
	}

	BigEndianBuffer chunkbuffer(size);
	chunkbuffer.WriteLong(index);
	chunkbuffer.WriteLong(depth ? 16 : 8);
	if (diff) {
		mChunks[depth]->SavePatch(chunkbuffer, other.mChunks[depth]);
	}
	chunkbuffer.WriteLong(FOUR_CHARS_TO_INT('e','n','d','c'));
#if wxUSE_STD_IOSTREAM
	stream.write((char *)chunkbuffer.Data(), chunkbuffer.Size());
#else
	stream.Write((char *)chunkbuffer.Data(), chunkbuffer.Size());
#endif

	return stream;
}

#if wxUSE_STD_IOSTREAM
wxSTD istream& ShapesCollection::LoadObject(wxSTD istream& stream)
#else
wxInputStream& ShapesCollection::LoadObject(wxInputStream& stream)
#endif
{
	BigEndianBuffer	coll_header(SIZEOF_collection_header);

#if wxUSE_STD_IOSTREAM
	stream.read((char *)coll_header.Data(), coll_header.Size());
#else
	stream.Read((char *)coll_header.Data(), coll_header.Size());
#endif

#if wxUSE_STD_IOSTREAM
	stream.seekg(0, std::ios::end);
	wxInt32 filesize = stream.tellg();
	stream.seekg(0, std::ios::beg);
#else
	wxInt32 filesize = stream.GetSize();
#endif
	
	long	offset8, length8,
			offset16, length16;
	
	mStatus = coll_header.ReadShort();
	mFlags = coll_header.ReadUShort();	
	offset8 = coll_header.ReadLong();
	length8 = coll_header.ReadLong();
	offset16 = coll_header.ReadLong();
	length16 = coll_header.ReadLong();
	
	if (offset8 < -1 || length8 < 0 || offset16 < -1 || length16 < 0)
		return stream;
	if ((offset8 + length8) > filesize)
		return stream;
	if ((offset16 + length16) > filesize)
		return stream;
	
	if (IsVerbose()) {
		wxLogDebug(wxT("[ShapesCollection]     Status: %d"), mStatus);
		wxLogDebug(wxT("[ShapesCollection]     Flags:  %d"), mFlags);
	}
	
	// is there the 8-bit version?
	if (offset8 != -1) {
		if (IsVerbose())
			wxLogDebug(wxT("[ShapesCollection]     8-bit chunk present"));
		
		BigEndianBuffer chunkbuffer(length8);
		
#if wxUSE_STD_IOSTREAM
		stream.seekg(offset8, std::ios::beg);
		stream.read((char *)chunkbuffer.Data(), chunkbuffer.Size());
#else
		stream.SeekI(offset8, wxFromStart);
		stream.Read((char *)chunkbuffer.Data(), chunkbuffer.Size());
#endif
		
		ShapesChunk	*pc = new ShapesChunk(IsVerbose());
		pc->LoadObject(chunkbuffer);
		
		if (!pc->IsGood()) {
			wxLogError(wxT("[ShapesCollection] Error loading 8-bit chunk... Dropped"));
			return stream;
		}
		mChunks[0] = pc;
	}
	
	// is there the 16-bit version?
	if (offset16 != -1) {
		if (IsVerbose())
			wxLogDebug(wxT("[ShapesCollection]     16/32-bit chunk present"));
		
		BigEndianBuffer chunkbuffer(length16);
		
#if wxUSE_STD_IOSTREAM
		stream.seekg(offset16, std::ios::beg);
		stream.read((char *)chunkbuffer.Data(), chunkbuffer.Size());
#else
		stream.SeekI(offset16, wxFromStart);
		stream.Read((char *)chunkbuffer.Data(), chunkbuffer.Size());
#endif
		
		ShapesChunk	*pc = new ShapesChunk(IsVerbose());
		pc->LoadObject(chunkbuffer);
		
		if (!pc->IsGood()) {
			wxLogError(wxT("[ShapesCollection] Error loading 16/32-bit chunk... Dropped"));
			return stream;
		}
		mChunks[1] = pc;
	}
	
	mGoodData = true;
	return stream;
}


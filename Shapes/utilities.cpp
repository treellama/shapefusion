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
#include "ShapesElements.h"
#include "utilities.h"

// convert an 8-bit ShapesBitmap to a RGB wxImage using the provided color table.
// <white_transparency> renders transparent pixels as white instead of using
// the chroma-key color. NOTE: this routine assumes valid pointers.
wxImage ShapesBitmapToImage(ShapesBitmap *bp, ShapesColorTable *ct, bool white_transparency)
{
	int				w = bp->Width(),
					h = bp->Height();
	bool			transparency_enabled = bp->IsTransparent();
	wxImage			img(w, h);
	unsigned char	*imgbuf = img.GetData(),
					*inp = bp->Pixels(),
					*outp = imgbuf;
	unsigned int	colors_per_table = ct->ColorCount();

	for (int i = 0; i < w * h; i++) {
		unsigned char	value = *inp++;

		if (value == 0 && transparency_enabled && white_transparency) {
			*outp++ = 255;
			*outp++ = 255;
			*outp++ = 255;
		} else if (value < colors_per_table) {
			ShapesColor	*color = ct->GetColor(value);

			*outp++ = color->Red() >> 8;
			*outp++ = color->Green() >> 8;
			*outp++ = color->Blue() >> 8;
		} else {
			wxLogError(wxT("[utilities ShapesBitmapToImage] Pixel value %u with just %u colors/table. Aborting"),
							value, colors_per_table);
			break;
		}
	}
	return img;
}

// create a wxBitmap thumbnail of the given wxImage. If the major wxImage
// dimension is greater than the specified thumbnail size, the wxImage
// will be scaled down (keeping its aspect ratio). If the wxImage is smaller,
// it will be just converted to a wxBitmap.
wxBitmap ImageThumbnail(wxImage& img, int tn_size, double max_upscale)
{
	auto w = static_cast<int>(img.GetWidth() * max_upscale);
	auto h = static_cast<int>(img.GetHeight() * max_upscale);

	auto new_h = h;
	auto new_w = w;

	if (w > tn_size || h > tn_size) {
		if (w > h) {
			new_w = tn_size;
			new_h = new_w * h / w;
			if (new_h < 1)
				new_h = 1;
		} else {
			new_h = tn_size;
			new_w = new_h * w / h;
			if (new_w < 1)
				new_w = 1;
		}
	}

	img.Rescale(new_w, new_h, wxIMAGE_QUALITY_HIGH);
	return wxBitmap(img);
}

// create the "bad item" (red 'X') thumbnail
wxBitmap BadThumbnail(int tn_size)
{
	wxImage			newimg(tn_size, tn_size);
	unsigned char	*imgbuf = newimg.GetData(), *p = imgbuf;

	for (int y = 0; y < tn_size; y++) {
		for (int x = 0; x < tn_size; x++) {
			if (x == y || (tn_size - x - 1) == y || (x - 1) == y || (x + 1) == y
				|| (tn_size - x - 2) == y || (tn_size - x) == y) {
				*p++ = 255;
				*p++ = 0;
				*p++ = 0;
			} else {
				*p++ = 255;
				*p++ = 255;
				*p++ = 255;
			}
		}
	}
	return wxBitmap(newimg);
}

// compute (squared) distance between given RGB colours (range [0;1]).
// This is used to quantize imported bitmaps against collection palettes.
// The formula seems to work quite well even for photos and very bad
// destination palettes. It was taken from the article "Colour metric"
// by T. Riemersma, available under a Creative Commons license at
// http://www.compuphase.com/cmetric.htm.
float ColourDistance(float r1, float g1, float b1, float r2, float g2, float b2)
{
	float	rMean = (r1 + r2) / 2.0,
			deltaR = r1 - r2,
			deltaG = g1 - g2,
			deltaB = b1 - b2;
	
	return (2.0+rMean)*deltaR*deltaR + 4.0*deltaG*deltaG + (2.0+1.0-rMean)*deltaB*deltaB;
}


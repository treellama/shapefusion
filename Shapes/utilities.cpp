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
#include <iostream>
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
wxBitmap ImageThumbnail(wxImage &img, int tn_size, bool filtering)
{
	int	w = img.GetWidth(),
		h = img.GetHeight();

	// scale the wxImage down to thumbnail size if larger
	if (w > tn_size || h > tn_size) {
		int	new_w, new_h;

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

		if (filtering) {
			// wx doesn't allow nice scaling, so we supply here. The thing works this way:
			// 1) calculate where each source pixel will end up in the final image
			// 2) add the source pixel value to the destination pixel
			// 3) divide each destination pixel by the number of pixels
			//    that were added there.
			// It's nothing more than a brutal pixel-level average, but results are quite good.
			wxImage			scaledimg(new_w, new_h);
			unsigned char	*src = img.GetData(),
							*srcp = src,
							*dst = scaledimg.GetData();
			unsigned int	pixcount = new_w * new_h,
							*tempbuf = new unsigned int[pixcount * 3],
							*countbuf = new unsigned int[pixcount];

			memset(tempbuf, 0, pixcount * 3 * sizeof(unsigned int));
			memset(countbuf, 0, pixcount * sizeof(unsigned int));
			// sum
			for (int y = 0; y < h; y++) {
				unsigned int	dsty = (y * new_h) / h * new_w;

				for (int x = 0; x < w; x++) {
					unsigned int	i = x * new_w / w + dsty,
									*tempp = &tempbuf[3 * i];

					*tempp++ += *srcp++;
					*tempp++ += *srcp++;
					*tempp += *srcp++;
					countbuf[i]++;
				}
			}
			// divide
			unsigned int	*tempp = tempbuf,
							*countp = countbuf;
			unsigned char	*dstp = dst;

			for (unsigned int i = 0; i < pixcount; i++) {
				unsigned int	count = *countp++;

				if (count == 0) {
					*dstp++ = 0;
					*dstp++ = 0;
					*dstp++ = 0;
				} else {
					*dstp++ = *tempp++ / count;
					*dstp++ = *tempp++ / count;
					*dstp++ = *tempp++ / count;
				}
			}

			delete[] tempbuf;
			delete[] countbuf;
			return wxBitmap(scaledimg);
		} else {
			// ugly (but fast and simple) wx scaler
			img.Rescale(new_w, new_h);
			return wxBitmap(img);
		}
	} else {
		return wxBitmap(img);
	}
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

float min(float a, float b)
{
	return (a > b) ? b : a;
}

float max(float a, float b)
{
	return (a > b) ? a : b;
}

// RGB -> HSV color transform. Ranges: [0,1]
void RGB2HSV(float r, float g, float b, float *hue, float *sat, float *val)
{
	float	v = max(max(r, g), b),
			x = min(min(r, g), b);

	*val = v;
	if (v == x) {
		*hue = 0;
		*sat = 0;
	} else {
		float	f = (r == x) ? (g - b) : ((g == x) ? (b - r) : (r - g));
		int		i = (r == x) ? 3 : ((g == x) ? 5 : 1);

		*hue = (i - f / (v - x)) / 6;
		*sat = (v - x) / v;
	}
}


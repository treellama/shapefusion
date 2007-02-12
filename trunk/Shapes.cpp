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
#include <stdio.h>
#include <math.h>
#include "Shapes.h"
#include "BigEndianBuffer.h"

char	*collnames[] = {	"Interface graphics",
							"Weapons in hand",
							"Juggernaut",
							"Tick",
							"Projectiles & explosions",
							"Hunter",
							"Player",
							"Items",
							"Trooper",
							"Pfhor fighter",
							"Defender",
							"Yeti",
							"Bob",
							"Vacuum Bob",
							"Enforcer",
							"Hummer",
							"Compiler",
							"Walls 1 (water)",
							"Walls 2 (lava)",
							"Walls 3 (sewage)",
							"Walls 4 (jiaro)",
							"Walls 5 (pfhor)",
							"Scenery 1 (water)",
							"Scenery 2 (lava)",
							"Scenery 3 (sewage)",
							"Scenery 4 (jiaro)",
							"Scenery 5 (pfhor)",
							"Landscape 1",
							"Landscape 2",
							"Landscape 3",
							"Landscape 4",
							"Cyborg" };

Shapes::Shapes(void): verbose_file_load(false)
{
	// TODO free allocated memory!
	// (ShpChunks and bitmap pixels)
}

// load shapes from given file
Shapes::Shapes(std::string path): good_data(false), verbose_file_load(false)
{
	std::ifstream	is(path.c_str(), std::ios::in);
	unsigned int	filesize;

	if (!is.good()) {
		std::cerr << "[Shapes] Error opening file " << path << "\n";
		return;
	}

	// get file size (for field validation)
	is.seekg(0, std::ios::end);
	filesize = is.tellg();
	is.seekg(0);

	// load collections
	BigEndianBuffer	raw_headers(SIZEOF_collection_header * COLLECTIONS_PER_FILE);

	is.read((char *)raw_headers.Data(), raw_headers.Size());
	for (unsigned int i = 0; i < COLLECTIONS_PER_FILE; i++) {
		ShpCollection	c;
		long			offset8, length8, offset16, length16;

		if (verbose_file_load)
			std::cerr << "[Shapes] Loading collection " << i << "\n";
		// collection header
		raw_headers.Position(i * SIZEOF_collection_header);
		c.status = raw_headers.ReadShort();
		c.flags = raw_headers.ReadUShort();
		offset8 = raw_headers.ReadLong();
		length8 = raw_headers.ReadLong();
		offset16 = raw_headers.ReadLong();
		length16 = raw_headers.ReadLong();
		if (offset8 < -1 || length8 < 0 || offset16 < -1 || length16 < 0
			|| (offset8 + length8) > (int)filesize || (offset16 + length16) > (int)filesize) {
			std::cerr << "[Shapes] Invalid collection header: this may not be a Marathon shapes file\n";
			is.close();
			return;
		}
		if (verbose_file_load) {
			std::cerr << "[Shapes]     Status: " << c.status << "\n";
			std::cerr << "[Shapes]     Flags:  " << c.flags << "\n";
		}
		c.chunks[0] = c.chunks[1] = NULL;

		// is there the 8-bit version?
		if (offset8 != -1) {
			if (verbose_file_load)
				std::cerr << "[Shapes]     8-bit chunk present" << "\n";
			ShpChunk	*pc = new ShpChunk;
			if (LoadShapesChunk(i, pc, is, offset8, length8) == -1) {
				delete pc;
				is.close();
				return;
			} else {
				c.chunks[0] = pc;
			}
		}

		// is there the 16-bit version?
		if (offset16 != -1) {
			if (verbose_file_load)
				std::cerr << "[Shapes]     16/32-bit chunk present" << "\n";
			ShpChunk	*pc = new ShpChunk;
			if (LoadShapesChunk(i, pc, is, offset16, length16) == -1) {
				delete pc;
				is.close();
				return;
			} else {
				c.chunks[1] = pc;
			}
		}
		
		// store
		collections.push_back(c);
	}
	good_data = true;
	is.close();
}

int Shapes::LoadShapesChunk(unsigned int id, ShpChunk *pc, std::ifstream &is, long offset, long expected_size)
{
	BigEndianBuffer	chunkbuffer(expected_size);

	is.seekg(offset);
	is.read((char *)chunkbuffer.Data(), chunkbuffer.Size());
	
	// load collection_definition
	pc->version = chunkbuffer.ReadShort();
	pc->type = chunkbuffer.ReadShort();
	pc->flags = chunkbuffer.ReadUShort();
	short	color_count = chunkbuffer.ReadShort(),
			clut_count = chunkbuffer.ReadShort();
	long	color_table_offset = chunkbuffer.ReadLong();
	short	high_level_shape_count = chunkbuffer.ReadShort();
	long	high_level_shape_offset_table_offset = chunkbuffer.ReadLong();
	short	low_level_shape_count = chunkbuffer.ReadShort();
	long	low_level_shape_offset_table_offset = chunkbuffer.ReadLong();
	short	bitmap_count = chunkbuffer.ReadShort();
	long	bitmap_offset_table_offset = chunkbuffer.ReadLong();
	pc->pixels_to_world = chunkbuffer.ReadShort();
	long	size = chunkbuffer.ReadLong();

	// validate values
	if (pc->version != COLLECTION_VERSION) {
		std::cerr << "[Shapes] Unknown collection version " << pc->version << "\n";
		return -1;
	}
	if (size != expected_size) {
		std::cerr << "[Shapes] Chunk size mismatch (" << size << "/" << expected_size << "): this may not be a Marathon shapes file\n";
		return -1;
	}
	if (color_table_offset < SIZEOF_collection_definition
		|| color_table_offset >= size
		|| high_level_shape_offset_table_offset < SIZEOF_collection_definition
		|| high_level_shape_offset_table_offset >= size
		|| low_level_shape_offset_table_offset < SIZEOF_collection_definition
		|| low_level_shape_offset_table_offset >= size
		|| bitmap_offset_table_offset < SIZEOF_collection_definition
		|| bitmap_offset_table_offset >= size) {
		std::cerr << "[Shapes] Invalid offsets in collection definition: this may not be a Marathon shapes file\n";
		return -1;
	}
	if (color_count < 0 || clut_count < 0 || high_level_shape_count < 0 || low_level_shape_count < 0 || bitmap_count < 0) {
		std::cerr << "[Shapes] Invalid object counts in collection definition: this may not be a Marathon shapes file\n";
		return -1;
	}

	if (verbose_file_load) {
		std::cerr << "[Shapes]         Version: " << pc->version << "\n";
		std::cerr << "[Shapes]         Type:    " << pc->type << "\n";
		std::cerr << "[Shapes]         Flags:   " << pc->flags << "\n";
		std::cerr << "[Shapes]         " << clut_count << " color tables, " << color_count << " colors per table\n";
		std::cerr << "[Shapes]         " << high_level_shape_count << " sequences\n";
		std::cerr << "[Shapes]         " << low_level_shape_count << " frames\n";
		std::cerr << "[Shapes]         " << bitmap_count << " bitmaps\n";
	}

	pc->size = size;
	
	// load color tables
	chunkbuffer.Position(color_table_offset);
	for (short j = 0; j < clut_count; j++) {
		ShpColorTable	ctable;

		for (short k = 0; k < color_count; k++) {
			ShpColor		color;
			unsigned char	flags;

			flags = chunkbuffer.ReadUChar();
			color.self_luminescent = flags & SELF_LUMINESCENT_COLOR;
			color.value = chunkbuffer.ReadUChar();
			color.red = chunkbuffer.ReadUShort();
			color.green = chunkbuffer.ReadUShort();
			color.blue = chunkbuffer.ReadUShort();
			ctable.colors.push_back(color);
		}
		pc->ctabs.push_back(ctable);
	}

	vector<long>	offsets;

	// load bitmaps, decoding compressed ones
	if (bitmap_count > 0) {
		chunkbuffer.Position(bitmap_offset_table_offset);
		offsets.clear();
		for (short j = 0; j < bitmap_count; j++)
			offsets.push_back(chunkbuffer.ReadLong());
		for (short j = 0; j < bitmap_count; j++) {
			if (offsets[j] < SIZEOF_collection_definition || offsets[j] >= size) {
				std::cerr << "[Shapes] Invalid bitmap offset: this may not be a Marathon shapes file\n";
				return -1;
			}
			chunkbuffer.Position(offsets[j]);
			ShpBitmap	btmp;
			short		flags;

			btmp.width = chunkbuffer.ReadShort();
			if (btmp.width < 0) {
				std::cerr << "[Shapes] Invalid bitmap width\n";
				continue;
			}
			btmp.height = chunkbuffer.ReadShort();
			if (btmp.height < 0) {
				std::cerr << "[Shapes] Invalid bitmap height\n";
				continue;
			}
			btmp.bytes_per_row = chunkbuffer.ReadShort();
			if (btmp.bytes_per_row < -1) {
				std::cerr << "[Shapes] Invalid bitmap bytes-per-row\n";
				continue;
			}
			flags = chunkbuffer.ReadShort();
			btmp.column_order = flags & COLUMN_ORDER;
			btmp.enable_transparency = flags & TRANSPARENCY_ENABLED;
			btmp.bit_depth = chunkbuffer.ReadShort();
			if (btmp.bit_depth != 8) {
				std::cerr << "[Shapes] Invalid bitmap depth " << btmp.bit_depth << "\n";
				continue;
			}
			// skip unused fields and placeholders
			unsigned int	numscanlines = btmp.column_order ? btmp.width : btmp.height;

			chunkbuffer.Position(chunkbuffer.Position() + 20 + numscanlines * 4);
			// load pixel data
			btmp.pixels = new unsigned char[btmp.width * btmp.height];
			if (btmp.bytes_per_row > -1) {
				// uncompressed bitmap
				if (flags & COLUMN_ORDER) {
					// column order
					unsigned char	*dstp;

					for (int x = 0; x < btmp.width; x++) {
						dstp = btmp.pixels + x;
						for (int y = 0; y < btmp.height; y++) {
							*dstp = chunkbuffer.ReadUChar();
							dstp += btmp.width;
						}
					}
				} else {
					// row order
					chunkbuffer.ReadBlock(btmp.width * btmp.height, btmp.pixels);
				}
			} else {
				// compressed bitmap (always column order)
				memset(btmp.pixels, 0, btmp.width * btmp.height);
				for (int x = 0; x < btmp.width; x++) {
					short			p0, p1;
					unsigned char	*dstp;

					p0 = chunkbuffer.ReadShort();
					p1 = chunkbuffer.ReadShort();
					dstp = btmp.pixels + x + p0 * btmp.width;
					while (p0 != p1) {
						*dstp = chunkbuffer.ReadUChar();
						dstp += btmp.width;
						p0++;
					}
				}
			}
			pc->bitmaps.push_back(btmp);
		}
	}

	// load HLS
	if (high_level_shape_count > 0) {
		chunkbuffer.Position(high_level_shape_offset_table_offset);
		offsets.clear();
		for (short j = 0; j < high_level_shape_count; j++)
			offsets.push_back(chunkbuffer.ReadLong());
		for (short j = 0; j < high_level_shape_count; j++) {
			if (offsets[j] < SIZEOF_collection_definition || offsets[j] >= size) {
				std::cerr << "[Shapes] Invalid sequence offset: this may not be a Marathon shapes file\n";
				return -1;
			}
			chunkbuffer.Position(offsets[j]);

			ShpSequence	seq;

			seq.type = chunkbuffer.ReadShort();
			seq.flags = chunkbuffer.ReadUShort();
			// the name is a Mac Pascal string, not a C string (length,chars)
			unsigned char	namelen = chunkbuffer.ReadUChar();
			memset(seq.name, 0, 33);
			chunkbuffer.ReadBlock(32+1, (unsigned char *)seq.name);
			if (namelen <= 32)
				seq.name[namelen] = 0;
			seq.number_of_views = chunkbuffer.ReadShort();
			seq.frames_per_view = chunkbuffer.ReadShort();
			seq.ticks_per_frame = chunkbuffer.ReadShort();
			seq.key_frame = chunkbuffer.ReadShort();
			seq.transfer_mode = chunkbuffer.ReadShort();
			seq.transfer_mode_period = chunkbuffer.ReadShort();
			seq.first_frame_sound = chunkbuffer.ReadShort();
			seq.key_frame_sound = chunkbuffer.ReadShort();
			seq.last_frame_sound = chunkbuffer.ReadShort();
			seq.pixels_to_world = chunkbuffer.ReadShort();
			seq.loop_frame = chunkbuffer.ReadShort();
			chunkbuffer.Position(chunkbuffer.Position() + 28);

			// load frame indexes
			int	n = CalcActualNumberOfViews(seq.number_of_views) * seq.frames_per_view;

			if (n > 0) {
				for (int k = 0; k < n; k++)
					seq.frame_indexes.push_back(chunkbuffer.ReadShort());
			}
			chunkbuffer.ReadShort();	// terminating index (usually 0 but can be garbage)

			pc->sequences.push_back(seq);
		}
	}
	
	// load LLS
	if (low_level_shape_count > 0) {
		chunkbuffer.Position(low_level_shape_offset_table_offset);
		offsets.clear();
		for (short j = 0; j < low_level_shape_count; j++)
			offsets.push_back(chunkbuffer.ReadLong());
		for (short j = 0; j < low_level_shape_count; j++) {
			if (offsets[j] < SIZEOF_collection_definition || offsets[j] >= size) {
				std::cerr << "[Shapes] Invalid frame offset " << offsets[j] << ": this may not be a Marathon shapes file\n";
				return -1;
			}
			chunkbuffer.Position(offsets[j]);
			ShpFrame		frame;
			unsigned short	flags;
			long			mli_fixed;

			flags = chunkbuffer.ReadUShort();
			frame.x_mirror = flags & X_MIRROR;
			frame.y_mirror = flags & Y_MIRROR;
			frame.keypoint_obscured = flags & KEYPOINT_OBSCURED;
			mli_fixed = chunkbuffer.ReadLong();
			frame.minimum_light_intensity = ((mli_fixed >> 16) & 0xffff) + (float)(mli_fixed & 0xffff) / 65536.0;	// convert fixed point [0,1] to double
			frame.bitmap_index = chunkbuffer.ReadShort();
			frame.origin_x = chunkbuffer.ReadShort();
			frame.origin_y = chunkbuffer.ReadShort();
			frame.key_x = chunkbuffer.ReadShort();
			frame.key_y = chunkbuffer.ReadShort();
			frame.world_left = chunkbuffer.ReadShort();
			frame.world_right = chunkbuffer.ReadShort();
			frame.world_top = chunkbuffer.ReadShort();
			frame.world_bottom = chunkbuffer.ReadShort();
			frame.world_x0 = chunkbuffer.ReadShort();
			frame.world_y0 = chunkbuffer.ReadShort();
			// calculate scale factor from world_* fields and associated bitmap dimensions.
			// If this fails, default to collection global scale factor
			if (frame.bitmap_index >= 0 && frame.bitmap_index < (int)pc->bitmaps.size()) {
				int	bw = pc->bitmaps[frame.bitmap_index].width;

				if (bw > 0)
					frame.scale_factor = (frame.world_right - frame.world_left) / bw;
				else
					frame.scale_factor = pc->pixels_to_world;
			} else {
				frame.scale_factor = pc->pixels_to_world;
			}
			pc->frames.push_back(frame);
		}
	}

	
	return size;
}

// write shapes data to a file
int Shapes::WriteToFile(std::string path)
{
	std::ofstream	os(path.c_str(), std::ios::out);

	if (!os.good()) {
		std::cerr << "[Shapes] Error opening file " << path << "\n";
		return -1;
	}

	// calculate collection sizes
	unsigned int	coll_sizes[COLLECTIONS_PER_FILE * 2];

	for (unsigned int i = 0; i < COLLECTIONS_PER_FILE; i++) {
		coll_sizes[i * 2] = CollectionSizeInFile(i, COLL_VERSION_8BIT);
		coll_sizes[i * 2 + 1] = CollectionSizeInFile(i, COLL_VERSION_TRUECOLOR);
	}

	// compose and write the collection header block
	{
		BigEndianBuffer	raw_headers(SIZEOF_collection_header * COLLECTIONS_PER_FILE);
		long			running_offset = SIZEOF_collection_header * COLLECTIONS_PER_FILE;

		for (unsigned int i = 0; i < COLLECTIONS_PER_FILE; i++) {
			raw_headers.WriteShort(collections[i].status);
			raw_headers.WriteUShort(collections[i].flags);
			// 8-bit version
			if (CollDefined(i, COLL_VERSION_8BIT)) {
				raw_headers.WriteLong(running_offset);
				raw_headers.WriteLong(coll_sizes[i * 2]);
				running_offset += coll_sizes[i * 2];
			} else {
				raw_headers.WriteLong(-1);
				raw_headers.WriteLong(0);
			}
			// truecolor version
			if (CollDefined(i, COLL_VERSION_TRUECOLOR)) {
				raw_headers.WriteLong(running_offset);
				raw_headers.WriteLong(coll_sizes[i * 2 + 1]);
				running_offset += coll_sizes[i * 2 + 1];
			} else {
				raw_headers.WriteLong(-1);
				raw_headers.WriteLong(0);
			}
			raw_headers.WriteZeroes(12);
		}
		os.write((char *)raw_headers.Data(), raw_headers.Size());
	}

	// compose and write collection blocks. I tried to reflect the order found
	// in Marathon 2 Shapes (color tables, sequences, frames, bitmaps)
	for (unsigned int i = 0; i < COLLECTIONS_PER_FILE; i++) {
		for (unsigned int v = 0; v < 2; v++) {
			if (CollDefined(i, v)) {
				BigEndianBuffer	chunkbuffer(coll_sizes[i * 2 + v]);
				ShpChunk		*chunkp = collections[i].chunks[v];
				unsigned int	bitmap_count = chunkp->bitmaps.size(),
								frame_count = chunkp->frames.size(),
								sequence_count = chunkp->sequences.size();
				long			sequence_table_offset,
								sequence_offsets[sequence_count],
								frame_table_offset,
								frame_offsets[frame_count],
								bitmap_table_offset,
								bitmap_offsets[bitmap_count];

				// skip the collection definition, we'll fill it at the end
				chunkbuffer.Position(SIZEOF_collection_definition);
				// write color tables
				for (unsigned int j = 0; j < chunkp->ctabs.size(); j++) {
					for (unsigned int k = 0; k < chunkp->ctabs[j].colors.size(); k++) {
						unsigned char	flags = chunkp->ctabs[j].colors[k].self_luminescent ? SELF_LUMINESCENT_COLOR : 0;

						chunkbuffer.WriteUChar(flags);
						chunkbuffer.WriteUChar(chunkp->ctabs[j].colors[k].value);
						chunkbuffer.WriteUShort(chunkp->ctabs[j].colors[k].red);
						chunkbuffer.WriteUShort(chunkp->ctabs[j].colors[k].green);
						chunkbuffer.WriteUShort(chunkp->ctabs[j].colors[k].blue);
					}
				}
				// write sequences
				sequence_table_offset = chunkbuffer.Position();
				if (sequence_count > 0) {
					chunkbuffer.Position(chunkbuffer.Position() + sequence_count * 4);
					for (unsigned int j = 0; j < sequence_count; j++) {
						ShpSequence	*sequence = &chunkp->sequences[j];

						sequence_offsets[j] = chunkbuffer.Position();
						chunkbuffer.WriteShort(sequence->type);
						chunkbuffer.WriteUShort(sequence->flags);
						chunkbuffer.WriteChar(strlen(sequence->name));
						chunkbuffer.WriteBlock(33, (unsigned char *)sequence->name);
						chunkbuffer.WriteShort(sequence->number_of_views);
						chunkbuffer.WriteShort(sequence->frames_per_view);
						chunkbuffer.WriteShort(sequence->ticks_per_frame);
						chunkbuffer.WriteShort(sequence->key_frame);
						chunkbuffer.WriteShort(sequence->transfer_mode);
						chunkbuffer.WriteShort(sequence->transfer_mode_period);
						chunkbuffer.WriteShort(sequence->first_frame_sound);
						chunkbuffer.WriteShort(sequence->key_frame_sound);
						chunkbuffer.WriteShort(sequence->last_frame_sound);
						chunkbuffer.WriteShort(sequence->pixels_to_world);
						chunkbuffer.WriteShort(sequence->loop_frame);
						chunkbuffer.WriteZeroes(28);
						for (unsigned int f = 0; f < sequence->frame_indexes.size(); f++)
							chunkbuffer.WriteShort(sequence->frame_indexes[f]);
						chunkbuffer.WriteShort(0);
					}
				}
				// write frames
				frame_table_offset = chunkbuffer.Position();
				chunkbuffer.Position(chunkbuffer.Position() + frame_count * 4);
				for (unsigned int j = 0; j < frame_count; j++) {
					ShpFrame		*frame = &chunkp->frames[j];
					unsigned short	flags = 0;
					double			mli_integer, mli_fractional;
					long			min_light_intensity = 0;

					frame_offsets[j] = chunkbuffer.Position();
					if (frame->x_mirror)
						flags |= X_MIRROR;
					if (frame->y_mirror)
						flags |= Y_MIRROR;
					if (frame->keypoint_obscured)
						flags |= KEYPOINT_OBSCURED;
					// double to fixed
					mli_fractional = modf(frame->minimum_light_intensity, &mli_integer);
					min_light_intensity |= (((short)mli_integer) << 16) & 0xffff0000;
					min_light_intensity |= (short)(mli_fractional * 0xffff) & 0x0000ffff;
					chunkbuffer.WriteUShort(flags);
					chunkbuffer.WriteLong(min_light_intensity);
					chunkbuffer.WriteShort(frame->bitmap_index);
					chunkbuffer.WriteShort(frame->origin_x);
					chunkbuffer.WriteShort(frame->origin_y);
					chunkbuffer.WriteShort(frame->key_x);
					chunkbuffer.WriteShort(frame->key_y);
					chunkbuffer.WriteShort(frame->world_left);
					chunkbuffer.WriteShort(frame->world_right);
					chunkbuffer.WriteShort(frame->world_top);
					chunkbuffer.WriteShort(frame->world_bottom);
					chunkbuffer.WriteShort(frame->world_x0);
					chunkbuffer.WriteShort(frame->world_y0);
					chunkbuffer.WriteZeroes(8);
				}
				// write bitmaps
				bitmap_table_offset = chunkbuffer.Position();
				chunkbuffer.Position(chunkbuffer.Position() + bitmap_count * 4);
				for (unsigned int j = 0; j < bitmap_count; j++) {
					ShpBitmap	*bitmap = &chunkp->bitmaps[j];
					short		flags = 0;

					bitmap_offsets[j] = chunkbuffer.Position();
					if (bitmap->column_order)
						flags |= COLUMN_ORDER;
					if (bitmap->enable_transparency)
						flags |= TRANSPARENCY_ENABLED;
					chunkbuffer.WriteShort(bitmap->width);
					chunkbuffer.WriteShort(bitmap->height);
					chunkbuffer.WriteShort(bitmap->bytes_per_row);
					chunkbuffer.WriteShort(flags);
					chunkbuffer.WriteShort(bitmap->bit_depth);
					chunkbuffer.WriteZeroes(20 + 4 * (bitmap->column_order ? bitmap->width : bitmap->height));
					if (bitmap->bytes_per_row == -1) {
						// compress
						for (int x = 0; x < bitmap->width; x++) {
							unsigned char	*pp = bitmap->pixels + x;
							int				p0 = -1,
											p1;

							for (int y = 0; y < bitmap->height; y++) {
								if (*pp != 0) {
									p0 = y;
									break;
								}
								pp += bitmap->width;
							}
							if (p0 == -1) {
								// no opaque pixels in this column
								chunkbuffer.WriteShort(0);
								chunkbuffer.WriteShort(0);
							} else {
								// found opaque pixels, go on
								p1 = p0;
								pp = bitmap->pixels + x + bitmap->width * (bitmap->height - 1);
								for (int y = bitmap->height - 1; y >= 0; y--) {
									if (*pp != 0) {
										p1 = y;
										break;
									}
									pp -= bitmap->width;
								}
								chunkbuffer.WriteShort(p0);
								chunkbuffer.WriteShort(p1 + 1);
								pp = bitmap->pixels + x + p0 * bitmap->width;
								for (int y = p0; y <= p1; y++) {
									chunkbuffer.WriteChar(*pp);
									pp += bitmap->width;
								}
							}
						}
					} else {
						if (bitmap->column_order) {
							for (int x = 0; x < bitmap->width; x++) {
								for (int y = 0; y < bitmap->height; y++)
									chunkbuffer.WriteChar(*(bitmap->pixels + x + y * bitmap->width));
							}
						} else {
							chunkbuffer.WriteBlock(bitmap->width * bitmap->height, bitmap->pixels);
						}
					}
				}
				// go back and write the collection definition (with correct offsets)
				chunkbuffer.Position(0);
				chunkbuffer.WriteShort(chunkp->version);
				chunkbuffer.WriteShort(chunkp->type);
				chunkbuffer.WriteUShort(chunkp->flags);
				chunkbuffer.WriteShort(chunkp->ctabs[0].colors.size());
				chunkbuffer.WriteShort(chunkp->ctabs.size());
				chunkbuffer.WriteLong(SIZEOF_collection_definition);
				chunkbuffer.WriteShort(sequence_count);
				chunkbuffer.WriteLong(sequence_table_offset);
				chunkbuffer.WriteShort(frame_count);
				chunkbuffer.WriteLong(frame_table_offset);
				chunkbuffer.WriteShort(bitmap_count);
				chunkbuffer.WriteLong(bitmap_table_offset);
				chunkbuffer.WriteShort(chunkp->pixels_to_world);
				chunkbuffer.WriteLong(coll_sizes[i * 2 + v]);
				chunkbuffer.WriteZeroes(506);
				// fill offset tables
				if (bitmap_count > 0) {
					chunkbuffer.Position(bitmap_table_offset);
					for (unsigned int j = 0; j < bitmap_count; j++)
						chunkbuffer.WriteLong(bitmap_offsets[j]);
				}
				if (frame_count > 0) {
					chunkbuffer.Position(frame_table_offset);
					for (unsigned int j = 0; j < frame_count; j++)
						chunkbuffer.WriteLong(frame_offsets[j]);
				}
				if (sequence_count > 0) {
					chunkbuffer.Position(sequence_table_offset);
					for (unsigned int j = 0; j < sequence_count; j++)
						chunkbuffer.WriteLong(sequence_offsets[j]);
				}
				
				os.write((char *)chunkbuffer.Data(), chunkbuffer.Size());
			}
		}
	}
	
	os.close();
	return 0;
}

Shapes::~Shapes(void)
{
	for (unsigned int i = 0; i < collections.size(); i++) {
		if (collections[i].chunks[0] != NULL)
			delete collections[i].chunks[0];
		if (collections[i].chunks[1] != NULL)
			delete collections[i].chunks[1];
	}
}

/**** collection data access ****/

int Shapes::CollGlobalStatus(unsigned int id) const
{
	return collections[id].status;
}

unsigned int Shapes::CollGlobalFlags(unsigned int id) const
{
	return collections[id].flags;
}

// returns wether a collection slot is occupied by a collection definition
bool Shapes::CollDefined(unsigned int id, unsigned int version) const
{
	if (id >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return false;
	return collections[id].chunks[version] != NULL;
}

int Shapes::CollVersion(unsigned int id, unsigned int version) const
{
	if (id >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return 0;
	return collections[id].chunks[version]->version;
}

int Shapes::CollType(unsigned int id, unsigned int version) const
{
	if (id >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return 0;
	return collections[id].chunks[version]->type;
}

unsigned int Shapes::CollFlags(unsigned int id, unsigned int version) const
{
	if (id >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return 0;
	return collections[id].chunks[version]->flags;
}

int Shapes::CollScaleFactor(unsigned int id, unsigned int version) const
{
	if (id >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return 0;
	return collections[id].chunks[version]->pixels_to_world;
}

unsigned int Shapes::CollBitmapCount(unsigned int id, unsigned int version) const
{
	if (id >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return 0;
	return collections[id].chunks[version]->bitmaps.size();
}

unsigned int Shapes::CollCTCount(unsigned int id, unsigned int version) const
{
	if (id >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return 0;
	return collections[id].chunks[version]->ctabs.size();
}

unsigned int Shapes::CollFrameCount(unsigned int id, unsigned int version) const
{
	if (id >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return 0;
	return collections[id].chunks[version]->frames.size();
}

unsigned int Shapes::CollSeqCount(unsigned int id, unsigned int version) const
{
	if (id >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return 0;
	return collections[id].chunks[version]->sequences.size();
}

ShpColorTable *Shapes::GetColorTable(unsigned int coll, unsigned int version, unsigned int ct) const
{
	if (coll >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return NULL;

	ShpChunk	*chunk = collections[coll].chunks[version];

	if (chunk == NULL)
		return NULL;
	if (ct >= chunk->ctabs.size())
		return NULL;
	return &chunk->ctabs[ct];
}

ShpBitmap *Shapes::GetBitmap(unsigned int coll, unsigned int version, unsigned int bitmap) const
{
	if (coll >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return NULL;

	ShpChunk	*chunk = collections[coll].chunks[version];

	if (chunk == NULL)
		return NULL;
	if (bitmap >= chunk->bitmaps.size())
		return NULL;
	return &chunk->bitmaps[bitmap];
}

ShpFrame *Shapes::GetFrame(unsigned int coll, unsigned int version, unsigned int frame) const
{
	if (coll >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return NULL;

	ShpChunk	*chunk = collections[coll].chunks[version];

	if (chunk == NULL)
		return NULL;
	if (frame >= chunk->frames.size())
		return NULL;
	return &chunk->frames[frame];
}

ShpSequence *Shapes::GetSeq(unsigned int coll, unsigned int version, unsigned int seq) const
{
	if (coll >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return NULL;

	ShpChunk	*chunk = collections[coll].chunks[version];

	if (chunk == NULL)
		return NULL;
	if (seq >= chunk->sequences.size())
		return NULL;
	return &chunk->sequences[seq];
}

/**** collection alteration ****/

void Shapes::InsertColorTable(ShpColorTable ct, unsigned int coll, unsigned int version)
{
	if (coll >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return;

	ShpChunk	*chunk = collections[coll].chunks[version];

	if (chunk != NULL)
		chunk->ctabs.push_back(ct);
}

void Shapes::DeleteColorTable(unsigned int coll, unsigned int version, unsigned int ct)
{
	if (coll >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return;

	ShpChunk	*chunk = collections[coll].chunks[version];

	if (chunk != NULL)
		chunk->ctabs.erase(chunk->ctabs.begin() + ct);
}

void Shapes::InsertBitmap(ShpBitmap b, unsigned int coll, unsigned int version)
{
	if (coll >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return;

	ShpChunk	*chunk = collections[coll].chunks[version];

	if (chunk != NULL)
		chunk->bitmaps.push_back(b);
}

void Shapes::DeleteBitmap(unsigned int coll, unsigned int version, unsigned int b)
{
	if (coll >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return;

	ShpChunk	*chunk = collections[coll].chunks[version];

	if (chunk != NULL) {
		// preserve existing frame-bitmap associations and associate
		// a null bitmap to frames using the bitmap we're deleting
		for (unsigned int i = 0; i < chunk->frames.size(); i++) {
			if (chunk->frames[i].bitmap_index == (int)b)
				chunk->frames[i].bitmap_index = -1;
			else if (chunk->frames[i].bitmap_index > (int)b)
				chunk->frames[i].bitmap_index--;
		}
		// now actually delete the bitmap
		chunk->bitmaps.erase(chunk->bitmaps.begin() + b);
	}
}

void Shapes::InsertFrame(ShpFrame f, unsigned int coll, unsigned int version)
{
	if (coll >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return;

	ShpChunk	*chunk = collections[coll].chunks[version];

	if (chunk != NULL)
		chunk->frames.push_back(f);
}

void Shapes::DeleteFrame(unsigned int coll, unsigned int version, unsigned int f)
{
	if (coll >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return;

	ShpChunk	*chunk = collections[coll].chunks[version];

	if (chunk != NULL) {
		// preserve existing sequence-frame associations and
		// unreference this frame index from any sequence using it
		for (unsigned int i = 0; i < chunk->sequences.size(); i++) {
			vector<short>	*indexes = &chunk->sequences[i].frame_indexes;

			for (unsigned int j = 0; j < indexes->size(); j++) {
				if ((*indexes)[j] == (int)f)
					(*indexes)[j] = -1;
				else if ((*indexes)[j] > (int)f)
					(*indexes)[j]--;
			}
		}
		// now actually delete the frame
		chunk->frames.erase(chunk->frames.begin() + f);
	}
}

void Shapes::InsertSequence(ShpSequence s, unsigned int coll, unsigned int version)
{
	if (coll >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return;

	ShpChunk	*chunk = collections[coll].chunks[version];

	if (chunk != NULL)
		chunk->sequences.push_back(s);
}

void Shapes::DeleteSequence(unsigned int coll, unsigned int version, unsigned int s)
{
	if (coll >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return;

	ShpChunk	*chunk = collections[coll].chunks[version];

	if (chunk != NULL)
		chunk->sequences.erase(chunk->sequences.begin() + s);
}

// return whether the class contains valid things
// e.g. this would be false after loading a damaged Shapes file
bool Shapes::GoodData(void) const
{
	return good_data;
}

/**** utilities ****/

// given a high_level_shape_definition.number_of_views value,
// return the real number of views
int CalcActualNumberOfViews(int t)
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
			std::cerr << "[Shapes] Unknown sequence type " << t << ", don't know the number of views\n";
			return t;
	}
	return -1;
}

// calculate how much space a collection is going
// to take when encoded to its on-file format.
unsigned int Shapes::CollectionSizeInFile(unsigned int id, unsigned int version) const
{
	if (id >= collections.size() || version > COLL_VERSION_TRUECOLOR)
		return 0;
	if (!CollDefined(id, version))
		return 0;

	ShpChunk		*chunkp = collections[id].chunks[version];
	unsigned int	size = SIZEOF_collection_definition,
					bitmap_count = chunkp->bitmaps.size(),
					frame_count = chunkp->frames.size(),
					sequence_count = chunkp->sequences.size();

	// add contribute of color tables
	if (chunkp->ctabs.size() > 0)
		size += SIZEOF_rgb_color_value * chunkp->ctabs.size() * chunkp->ctabs[0].colors.size();
	// add contribute of bitmap offset table
	size += 4 * bitmap_count;
	// add contribute of bitmaps (we'll have to compress them if necessary)
	size += SIZEOF_bitmap_definition * bitmap_count;
	for (unsigned int i = 0; i < bitmap_count; i++) {
		ShpBitmap	*bitmapp = &chunkp->bitmaps[i];

		// scanline pointer placeholder
		if (bitmapp->column_order)
			size += 4 * bitmapp->width;
		else
			size += 4 * bitmapp->height;
		if (bitmapp->bytes_per_row == -1) {
			// compressed
			size += bitmapp->width * 4;
			for (int x = 0; x < bitmapp->width; x++) {
				unsigned char	*pp = bitmapp->pixels + x;
				int				p0 = -1,
								p1;

				for (int y = 0; y < bitmapp->height; y++) {
					if (*pp != 0) {
						p0 = y;
						break;
					}
					pp += bitmapp->width;
				}
				if (p0 == -1)
					continue;	// no opaque pixels in this column
				p1 = p0;
				pp = bitmapp->pixels + x + bitmapp->width * (bitmapp->height - 1);
				for (int y = bitmapp->height - 1; y >= 0; y--) {
					if (*pp != 0) {
						p1 = y;
						break;
					}
					pp -= bitmapp->width;
				}
				size += p1 - p0 + 1;
			}
		} else {
			// plain
			size += bitmapp->width * bitmapp->height;
		}
	}
	// add contribute of frame offset table
	size += 4 * frame_count;
	// add contribute of frame definitions
	size += SIZEOF_low_level_shape_definition * frame_count;
	// add contribute of sequence offset table
	size += 4 * sequence_count;
	// add contribute of sequence definitions (and following frame indexes)
	size += SIZEOF_high_level_shape_definition * sequence_count;
	for (unsigned int i = 0; i < sequence_count; i++)
		size += 2 * (chunkp->sequences[i].frame_indexes.size() + 1);

	return size;
}


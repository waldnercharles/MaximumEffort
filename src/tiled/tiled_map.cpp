#include "tiled_map.h"
#include "common.h"
#include "game.h"

CF_Sprite *
make_atlas_from_png(Png png, int tile_width, int tile_height, size_t *count)
{
	int n;
	CF_Sprite *s;

	n = (png.w / tile_width * png.h / tile_height);
	s = (CF_Sprite *)cf_alloc(sizeof(CF_Sprite) * n);

	if (count)
	{
		*count = n;
	}

	auto pix = (Pixel *)alloca(sizeof(Pixel) * tile_width * tile_height);

	for (int y = 0; y < png.h; y += tile_height)
	{
		for (int x = 0; x < png.w; x += tile_width)
		{
			for (int yy = 0; yy < tile_height; yy++)
			{
				for (int xx = 0; xx < tile_width; xx++)
				{
					int pix_index = yy * tile_width + xx;
					int png_pix_index = (y + yy) * png.w + x + xx;
					pix[pix_index] = png.pix[png_pix_index];
				}
			}

			*s = cf_make_easy_sprite_from_pixels(pix, tile_width, tile_height);
			s++;
		}
	}

	return s - n;
}

TiledMap load_tiled_map(const char *path)
{
	void *buffer;
	size_t buffer_size;

	TiledMap tiled_map;
	buffer = cf_fs_read_entire_file_to_memory(path, &buffer_size);

	tiled_map.map = cute_tiled_load_map_from_memory(
		buffer,
		(int)buffer_size,
		nullptr
	);
	tiled_map.pos = cf_v2(0, 0);

	cf_free(buffer);

	cute_tiled_tileset_t *tileset = tiled_map.map->tilesets;
	while (tileset)
	{
		// Ensure the count rather than capacity in case our png fails to load
		tiled_map.atlas.ensure_count(tileset->firstgid + tileset->tilecount);

		Png png;
		CF_Result result = cf_png_cache_load(tileset->image.ptr, &png);
		if (cf_is_error(result))
		{
			tileset = tileset->next;
			continue;
		}

		size_t count;
		CF_Sprite *sprites = make_atlas_from_png(
			png,
			tileset->tilewidth,
			tileset->tileheight,
			&count
		);

		for (int i = 0; i < count; i++)
		{
			tiled_map.atlas[tileset->firstgid + i] = sprites[i];
		}

		cf_free(sprites);
		tileset = tileset->next;
	}

	return tiled_map;
}

void unload_tiled_map(TiledMap tiled_map)
{
	if (tiled_map.map)
	{
		cute_tiled_free_map(tiled_map.map);
		tiled_map.map = nullptr;

		for (int i = 0; i < tiled_map.atlas.count(); i++)
		{
			cf_easy_sprite_unload(&(tiled_map.atlas[i]));
		}
		tiled_map.atlas.clear();
	}
}

void TiledMap::draw()
{
	if (!map || !map->layers)
	{
		return;
	}

	v2 cam_center = cf_camera_peek_position();
	v2 cam_dimensions = cf_camera_peek_dimensions();

	cute_tiled_layer_t *layer = map->layers;

	// We're going to offset our calculations by half the map,
	// so that its origin is (0,0)
	v2 half_map_size = cf_v2(map->width, map->height) / 2.f;
	v2 tile_size = cf_v2(map->tilewidth, map->tileheight);

	v2 map_center = half_map_size * tile_size;

	v2 half_dim = cam_dimensions / 2.f;
	v2 top_rgt = (cam_center + half_dim) / tile_size + cf_v2(1, 1);
	v2 bot_lft = (cam_center - half_dim) / tile_size - cf_v2(1, 1);

	while (layer)
	{
		int *data = layer->data;
		int data_count = layer->data_count;

		// Maps are saved top to bottom, left to right.
		for (int y = top_rgt.y; y >= bot_lft.y; y--)
		{
			for (int x = bot_lft.x; x <= top_rgt.x; x++)
			{
				int i = ((half_map_size.y - y) * map->width) + x +
						half_map_size.x;

				if (i < 0 || i > data_count)
				{
					continue;
				}

				int gid = data[i];
				if (gid == 0)
				{
					continue;
				}

				CF_Sprite s = atlas[gid];

				s32 pos_x = i % map->width * map->tilewidth;
				s32 pos_y = i / map->width * map->tileheight;

				s.transform.p = cf_v2(
					pos_x - (int)map_center.x,
					(int)map_center.y - pos_y
				);
				cf_draw_sprite(&s);
			}
		}

		layer = layer->next;
	}
}
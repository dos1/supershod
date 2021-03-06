/*! \file common.c
 *  \brief Common stuff that can be used by all gamestates.
 */
/*
 * Copyright (c) Sebastian Krzyszkowiak <dos@dosowisko.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "common.h"
#include <libsuperderpy.h>
#include <math.h>

bool GlobalEventHandler(struct Game* game, ALLEGRO_EVENT* ev) {
	if ((ev->type == ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_F)) {
		game->config.fullscreen = !game->config.fullscreen;
		if (game->config.fullscreen) {
			SetConfigOption(game, "SuperDerpy", "fullscreen", "1");
			al_hide_mouse_cursor(game->display);
		} else {
			SetConfigOption(game, "SuperDerpy", "fullscreen", "0");
			al_show_mouse_cursor(game->display);
		}
		al_set_display_flag(game->display, ALLEGRO_FULLSCREEN_WINDOW, game->config.fullscreen);
		SetupViewport(game, game->viewport_config);
		PrintConsole(game, "Fullscreen toggled");
	}

	return false;
}

struct CommonResources* CreateGameData(struct Game* game) {
	struct CommonResources* data = calloc(1, sizeof(struct CommonResources));

	data->shader = al_create_shader(ALLEGRO_SHADER_GLSL);
	PrintConsole(game, "VERTEX: %d", al_attach_shader_source_file(data->shader, ALLEGRO_VERTEX_SHADER, GetDataFilePath(game, "vertex.glsl")));
	PrintConsole(game, "%s", al_get_shader_log(data->shader));
	PrintConsole(game, "PIXEL: %d", al_attach_shader_source_file(data->shader, ALLEGRO_PIXEL_SHADER, GetDataFilePath(game, "pixel.glsl")));
	PrintConsole(game, "%s", al_get_shader_log(data->shader));
	al_build_shader(data->shader);

	data->supersample = al_load_sample(GetDataFilePath(game, "super.flac"));
	data->shodsample = al_load_sample(GetDataFilePath(game, "shod.flac"));

	data->super = al_create_sample_instance(data->supersample);
	data->shod = al_create_sample_instance(data->shodsample);

	al_attach_sample_instance_to_mixer(data->super, game->audio.fx);
	al_attach_sample_instance_to_mixer(data->shod, game->audio.fx);

	data->screen = al_load_bitmap(GetDataFilePath(game, "screen.png"));

	return data;
}

void WhiteNoise(struct Game* game) {
	ALLEGRO_BITMAP* bitmap = al_get_target_bitmap();
	al_lock_bitmap(bitmap, ALLEGRO_LOCK_WRITEONLY, 0);
	float val;
	int width, height;
	width = al_get_bitmap_width(bitmap);
	height = al_get_bitmap_height(bitmap);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			val = rand() / (float)RAND_MAX;
			val += 0.2;
			val = fmin(val, 1);
			al_put_pixel(i, j, al_map_rgb_f(val, val, val));
		}
	}
	al_unlock_bitmap(bitmap);
}

void DestroyGameData(struct Game* game) {
	al_destroy_shader(game->data->shader);
	al_destroy_bitmap(game->data->screen);
	al_destroy_sample_instance(game->data->super);
	al_destroy_sample_instance(game->data->shod);
	al_destroy_sample(game->data->supersample);
	al_destroy_sample(game->data->shodsample);
	free(game->data);
}

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

static int intlog(double base, double x) {
	return ceil(log(x) / log(base));
}

static float npow2(struct Game* game, float a) {
	//	if (al_get_display_option(game->display, ALLEGRO_SUPPORT_NPOT_BITMAP)) {
	//		return a;
	//	}
	return pow(2, intlog(2, a));
}

void DrawCRTScreen(struct Game* game) {
	al_set_target_backbuffer(game->display);
	al_use_shader(game->data->shader);
	int x, y, w, h;
	al_get_clipping_rectangle(&x, &y, &w, &h);
	float res[2] = {w, h};
	al_set_shader_float_vector("res", 2, res, 1);
	float offset[2] = {x, y};
	al_set_shader_float_vector("offset", 2, offset, 1);
	float res2[2] = {game->viewport.width, game->viewport.height};
	al_set_shader_float_vector("res2", 2, res2, 1);
	float pow2[2] = {npow2(game, game->viewport.width), npow2(game, game->viewport.height)};
	al_set_shader_float_vector("pow2", 2, pow2, 1);
	al_draw_bitmap(game->data->fb, 0, 0, 0);
	al_use_shader(NULL);
	al_draw_scaled_bitmap(game->data->screen, 0, 0, 640, 360, 0, 0, game->viewport.width, game->viewport.height, 0);
}

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
		al_set_display_flag(game->display, ALLEGRO_FRAMELESS, game->config.fullscreen);
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

	data->hsample = al_load_sample(GetDataFilePath(game, "h.flac"));
	data->dsample = al_load_sample(GetDataFilePath(game, "d.flac"));

	data->h = al_create_sample_instance(data->hsample);
	data->d = al_create_sample_instance(data->dsample);

	al_attach_sample_instance_to_mixer(data->super, game->audio.fx);
	al_attach_sample_instance_to_mixer(data->shod, game->audio.fx);
	al_attach_sample_instance_to_mixer(data->h, game->audio.fx);
	al_attach_sample_instance_to_mixer(data->d, game->audio.fx);

	al_set_sample_instance_gain(data->h, 1.2);
	al_set_sample_instance_gain(data->d, 1.2);

	data->screen = al_load_bitmap(GetDataFilePath(game, "screen.png"));
	data->fb = CreateNotPreservedBitmap(game->viewport.width, game->viewport.height);

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
	al_destroy_bitmap(game->data->fb);
	al_destroy_sample_instance(game->data->super);
	al_destroy_sample_instance(game->data->shod);
	al_destroy_sample(game->data->supersample);
	al_destroy_sample(game->data->shodsample);
	free(game->data);
}

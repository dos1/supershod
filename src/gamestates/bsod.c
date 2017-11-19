/*! \file empty.c
 *  \brief Empty gamestate.
 */
/*
 * Copyright (c) Sebastian Krzyszkowiak <dos@dosowisko.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../common.h"
#include <libsuperderpy.h>

struct GamestateResources {
	// This struct is for every resource allocated and used by your gamestate.
	// It gets created on load and then gets passed around to all other function calls.
	int counter;
};

int Gamestate_ProgressCount = 1; // number of loading steps as reported by Gamestate_Load

void Gamestate_Logic(struct Game* game, struct GamestateResources* data) {
	// Called 60 times per second. Here you should do all your game logic.
	if (data->counter < 36) {
		data->counter++;
	}
}

void Gamestate_Draw(struct Game* game, struct GamestateResources* data) {
	// Called as soon as possible, but no sooner than next Gamestate_Logic call.
	// Draw everything to the screen here.

	ALLEGRO_TRANSFORM trans;
	al_identity_transform(&trans);
	al_use_transform(&trans);

	if (!game->_priv.font_bsod) {
		game->_priv.font_bsod = al_create_builtin_font();
	}

	al_set_target_backbuffer(game->display);
	al_clear_to_color(al_map_rgb(0, 0, 170));

	if (data->counter == 36) {
		const char* header = "SUPERSHOD HD";

		al_draw_filled_rectangle(al_get_display_width(game->display) / 2 - al_get_text_width(game->_priv.font_bsod, header) / 2 - 4, (int)(al_get_display_height(game->display) * 0.32), 4 + al_get_display_width(game->display) / 2 + al_get_text_width(game->_priv.font_bsod, header) / 2, (int)(al_get_display_height(game->display) * 0.32) + al_get_font_line_height(game->_priv.font_bsod), al_map_rgb(170, 170, 170));

		al_draw_text(game->_priv.font_bsod, al_map_rgb(0, 0, 170), al_get_display_width(game->display) / 2, (int)(al_get_display_height(game->display) * 0.32), ALLEGRO_ALIGN_CENTRE, header);

		const char* header2 = "A fatal exception 0xD3RP has occured at 0028:M00F11NZ in GST SD(01) +";

		al_draw_text(game->_priv.font_bsod, al_map_rgb(255, 255, 255), al_get_display_width(game->display) / 2, (int)(al_get_display_height(game->display) * 0.32 + 2 * al_get_font_line_height(game->_priv.font_bsod) * 1.25), ALLEGRO_ALIGN_CENTRE, header2);
		al_draw_textf(game->_priv.font_bsod, al_map_rgb(255, 255, 255), al_get_display_width(game->display) / 2 - al_get_text_width(game->_priv.font_bsod, header2) / 2, (int)(al_get_display_height(game->display) * 0.32 + 3 * al_get_font_line_height(game->_priv.font_bsod) * 1.25), ALLEGRO_ALIGN_LEFT, "%p and system just doesn't know what went wrong.", game);

		al_draw_text(game->_priv.font_bsod, al_map_rgb(255, 255, 255), al_get_display_width(game->display) / 2, (int)(al_get_display_height(game->display) * 0.32 + 5 * al_get_font_line_height(game->_priv.font_bsod) * 1.25), ALLEGRO_ALIGN_CENTRE, "No more game.");

		al_draw_text(game->_priv.font_bsod, al_map_rgb(255, 255, 255), al_get_display_width(game->display) / 2 - al_get_text_width(game->_priv.font_bsod, header2) / 2, (int)(al_get_display_height(game->display) * 0.32 + 7 * al_get_font_line_height(game->_priv.font_bsod) * 1.25), ALLEGRO_ALIGN_LEFT, "* Press any key to terminate this error.");
		al_draw_text(game->_priv.font_bsod, al_map_rgb(255, 255, 255), al_get_display_width(game->display) / 2 - al_get_text_width(game->_priv.font_bsod, header2) / 2, (int)(al_get_display_height(game->display) * 0.32 + 8 * al_get_font_line_height(game->_priv.font_bsod) * 1.25), ALLEGRO_ALIGN_LEFT, "* Press any key to destroy all muffins in the world.");
		al_draw_text(game->_priv.font_bsod, al_map_rgb(255, 255, 255), al_get_display_width(game->display) / 2 - al_get_text_width(game->_priv.font_bsod, header2) / 2, (int)(al_get_display_height(game->display) * 0.32 + 9 * al_get_font_line_height(game->_priv.font_bsod) * 1.25), ALLEGRO_ALIGN_LEFT, "* Just kidding, please press any key anyway.");

		al_draw_text(game->_priv.font_bsod, al_map_rgb(255, 255, 255), al_get_display_width(game->display) / 2 - al_get_text_width(game->_priv.font_bsod, header2) / 2, (int)(al_get_display_height(game->display) * 0.32 + 11 * al_get_font_line_height(game->_priv.font_bsod) * 1.25), ALLEGRO_ALIGN_LEFT, "This is fatal error. My bad.");

		al_draw_text(game->_priv.font_bsod, al_map_rgb(255, 255, 255), al_get_display_width(game->display) / 2, (int)(al_get_display_height(game->display) * 0.32 + 13 * al_get_font_line_height(game->_priv.font_bsod) * 1.25), ALLEGRO_ALIGN_CENTRE, "Press any key to quit _");
	}

	al_use_transform(&game->projection);
}

void Gamestate_ProcessEvent(struct Game* game, struct GamestateResources* data, ALLEGRO_EVENT* ev) {
	// Called for each event in Allegro event queue.
	// Here you can handle user input, expiring timers etc.
	if ((ev->type == ALLEGRO_EVENT_KEY_DOWN) || (ev->type == ALLEGRO_EVENT_TOUCH_BEGIN)) {
		UnloadCurrentGamestate(game); // mark this gamestate to be stopped and unloaded
		// When there are no active gamestates, the engine will quit.
	}
}

void* Gamestate_Load(struct Game* game, void (*progress)(struct Game*)) {
	// Called once, when the gamestate library is being loaded.
	// Good place for allocating memory, loading bitmaps etc.
	struct GamestateResources* data = malloc(sizeof(struct GamestateResources));
	(*progress)(game);
	return data;
}

void Gamestate_Unload(struct Game* game, struct GamestateResources* data) {
	// Called when the gamestate library is being unloaded.
	// Good place for freeing all allocated memory and resources.
	free(data);
}

void Gamestate_Start(struct Game* game, struct GamestateResources* data) {
	// Called when this gamestate gets control. Good place for initializing state,
	// playing music etc.
	data->counter = 0;
}

void Gamestate_Stop(struct Game* game, struct GamestateResources* data) {
	// Called when gamestate gets stopped. Stop timers, music etc. here.
}

void Gamestate_Pause(struct Game* game, struct GamestateResources* data) {
	// Called when gamestate gets paused (so only Draw is being called, no Logic not ProcessEvent)
	// Pause your timers here.
}

void Gamestate_Resume(struct Game* game, struct GamestateResources* data) {
	// Called when gamestate gets resumed. Resume your timers here.
}

// Ignore this for now.
// TODO: Check, comment, refine and/or remove:
void Gamestate_Reload(struct Game* game, struct GamestateResources* data) {}

/*! \file empty.c
 *  \brief Empty gamestate.
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

#include "../common.h"
#include <libsuperderpy.h>

struct GamestateResources {
	// This struct is for every resource allocated and used by your gamestate.
	// It gets created on load and then gets passed around to all other function calls.
	ALLEGRO_BITMAP* logo;
	ALLEGRO_BITMAP *shod, *shod2, *shod3, *shod4;
	ALLEGRO_BITMAP* time;
	struct Timeline* timeline;
	int blink_counter;
	int blinks;
	bool showtime;
	bool showsuper;
	bool allowed;

	bool player1;
	bool player2;
	bool player3;
	bool player4;

	ALLEGRO_SAMPLE* sample;
	ALLEGRO_SAMPLE_INSTANCE* ambient;
	ALLEGRO_SAMPLE* clicks;
	ALLEGRO_SAMPLE_INSTANCE* click;
};

int Gamestate_ProgressCount = 8; // number of loading steps as reported by Gamestate_Load

static bool ShowTime(struct Game* game, struct TM_Action* action, enum TM_ActionState state) {
	struct GamestateResources* data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		if (data->allowed) {
			data->showtime = true;
			al_play_sample_instance(data->click);
		}
	}
	return true;
}

static bool HideTime(struct Game* game, struct TM_Action* action, enum TM_ActionState state) {
	struct GamestateResources* data = action->arguments->value;
	if (state == TM_ACTIONSTATE_START) {
		if (data->allowed) {
			data->showtime = false;
			al_play_sample_instance(data->click);
		}
	}
	return true;
}

void Gamestate_Logic(struct Game* game, struct GamestateResources* data) {
	// Called 60 times per second. Here you should do all your game logic.
	data->blink_counter++;
	if (data->blink_counter > 60) {
		data->blinks++;
		data->blink_counter = 0;
	}
	if (data->showsuper) {
		if (data->blink_counter == 21) {
			al_play_sample_instance((data->blinks % 2) ? game->data->shod : game->data->super);
		}

		if ((data->blinks == 6) && (data->blink_counter == 19)) {
			SwitchCurrentGamestate(game, "noise");
		}
	}
	TM_Process(data->timeline);
}

void Gamestate_Draw(struct Game* game, struct GamestateResources* data) {
	// Called as soon as possible, but no sooner than next Gamestate_Logic call.
	// Draw everything to the screen here.

	al_set_target_bitmap(game->data->fb);
	al_clear_to_color(al_map_rgba(0, 0, 0, 0));
	al_draw_scaled_bitmap(data->shod, 0, 0, 1920, 1080, 0, 0, game->viewport.width, game->viewport.height, 0);

	if (data->player2) {
		al_draw_scaled_bitmap(data->shod2, 0, 0, 1920, 1080, 0, 0, game->viewport.width, game->viewport.height, 0);
	}
	if (data->player3) {
		al_draw_scaled_bitmap(data->shod3, 0, 0, 1920, 1080, 0, 0, game->viewport.width, game->viewport.height, 0);
	}
	if (data->player4) {
		al_draw_scaled_bitmap(data->shod4, 0, 0, 1920, 1080, 0, 0, game->viewport.width, game->viewport.height, 0);
	}

	if (data->showtime || data->showsuper) {
		al_draw_filled_rectangle(0, 0, game->viewport.width, game->viewport.height, al_map_rgba(0, 0, 0, 92 - (rand() % 4)));
	}

	if (data->showtime) {
		al_draw_scaled_bitmap(data->time, 0, 0, 1920, 1080, 0, 0, game->viewport.width, game->viewport.height, 0);
	}

	if (data->showsuper) {
		if (data->blink_counter >= 20) {
			al_draw_scaled_bitmap(data->logo, 0, 0, 1920, 1080, 0, 0, game->viewport.width, game->viewport.height, 0);
		}
	}

	al_set_target_backbuffer(game->display);

	al_draw_bitmap(game->data->fb, 0, 0, 0);

	//al_draw_scaled_bitmap(game->data->screen, 0, 0, 640, 360, 0, 0, 320, 180, 0);
}

void Gamestate_ProcessEvent(struct Game* game, struct GamestateResources* data, ALLEGRO_EVENT* ev) {
	// Called for each event in Allegro event queue.
	// Here you can handle user input, expiring timers etc.
	TM_HandleEvent(data->timeline, ev);
	if ((ev->type == ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
		UnloadCurrentGamestate(game); // mark this gamestate to be stopped and unloaded
		// When there are no active gamestates, the engine will quit.
	}

	if ((ev->type == ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ENTER)) {
		if (data->allowed) {
			TM_CleanQueue(data->timeline);
			TM_AddAction(data->timeline, ShowTime, TM_AddToArgs(NULL, 1, data), "ShowTime");
			TM_AddDelay(data->timeline, 4000);
			TM_AddAction(data->timeline, HideTime, TM_AddToArgs(NULL, 1, data), "HideTime");
		}
	}

	if (data->allowed) {
		if (((ev->type == ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_SPACE)) || (ev->type == ALLEGRO_EVENT_TOUCH_BEGIN)) {
			data->showtime = false;
			data->showsuper = true;
			data->blinks = 0;
			data->blink_counter = 20;
			data->allowed = false;
		}
	}
	if (ev->type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {
		if (ev->joystick.id == al_get_joystick(0)) {
			data->player2 = true;
		}
		if (ev->joystick.id == al_get_joystick(1)) {
			data->player3 = true;
		}
		if (ev->joystick.id == al_get_joystick(2)) {
			data->player4 = true;
		}
	}
	if (ev->type == ALLEGRO_EVENT_KEY_DOWN) {
		if (ev->keyboard.keycode == ALLEGRO_KEY_2) {
			data->player2 = true;
		}
		if (ev->keyboard.keycode == ALLEGRO_KEY_3) {
			data->player3 = true;
		}
		if (ev->keyboard.keycode == ALLEGRO_KEY_4) {
			data->player4 = true;
		}
	}
}

void* Gamestate_Load(struct Game* game, void (*progress)(struct Game*)) {
	// Called once, when the gamestate library is being loaded.
	// Good place for allocating memory, loading bitmaps etc.
	struct GamestateResources* data = malloc(sizeof(struct GamestateResources));
	data->logo = al_load_bitmap(GetDataFilePath(game, "logo.png"));
	progress(game);
	data->time = al_load_bitmap(GetDataFilePath(game, "time.png"));
	progress(game);
	data->shod = al_load_bitmap(GetDataFilePath(game, "shod.png"));
	progress(game);
	data->shod2 = al_load_bitmap(GetDataFilePath(game, "shod2.png"));
	progress(game);
	data->shod3 = al_load_bitmap(GetDataFilePath(game, "shod3.png"));
	progress(game);
	data->shod4 = al_load_bitmap(GetDataFilePath(game, "shod4.png"));
	progress(game);
	data->timeline = TM_Init(game, "supershod");

	data->sample = al_load_sample(GetDataFilePath(game, "ambient.flac"));
	data->ambient = al_create_sample_instance(data->sample);
	al_attach_sample_instance_to_mixer(data->ambient, game->audio.music);
	progress(game);

	data->clicks = al_load_sample(GetDataFilePath(game, "click.flac"));
	data->click = al_create_sample_instance(data->clicks);
	al_attach_sample_instance_to_mixer(data->click, game->audio.fx);

	progress(game); // report that we progressed with the loading, so the engine can draw a progress bar
	return data;
}

void Gamestate_Unload(struct Game* game, struct GamestateResources* data) {
	// Called when the gamestate library is being unloaded.
	// Good place for freeing all allocated memory and resources.
	al_destroy_bitmap(data->logo);
	al_destroy_bitmap(data->shod);
	al_destroy_bitmap(data->shod2);
	al_destroy_bitmap(data->shod3);
	al_destroy_bitmap(data->shod4);
	al_destroy_bitmap(data->time);
	TM_Destroy(data->timeline);

	al_destroy_sample_instance(data->ambient);
	al_destroy_sample_instance(data->click);
	al_destroy_sample(data->sample);
	al_destroy_sample(data->clicks);

	free(data);
}

void Gamestate_Start(struct Game* game, struct GamestateResources* data) {
	// Called when this gamestate gets control. Good place for initializing state,
	// playing music etc.
	data->blink_counter = 0;
	data->blinks = 0;
	data->showtime = false;
	data->allowed = true;
	data->showsuper = false;

	data->player1 = true;
	data->player2 = false;
	data->player3 = false;
	data->player4 = false;

	TM_AddDelay(data->timeline, 3000);
	TM_AddAction(data->timeline, ShowTime, TM_AddToArgs(NULL, 1, data), "ShowTime");
	TM_AddDelay(data->timeline, 4000);
	TM_AddAction(data->timeline, HideTime, TM_AddToArgs(NULL, 1, data), "HideTime");

	/*
	TM_AddDelay(data->timeline, 15000);
	TM_AddAction(data->timeline, ShowMove, TM_AddToArgs(NULL, 1, data), "ShowMove");
	TM_AddDelay(data->timeline, 6000);
	TM_AddAction(data->timeline, ShowMove2, TM_AddToArgs(NULL, 1, data), "ShowMove2");
	TM_AddDelay(data->timeline, 4000);
	TM_AddAction(data->timeline, ShowMove3, TM_AddToArgs(NULL, 1, data), "ShowMove3");
	TM_AddDelay(data->timeline, 4000);
	TM_AddAction(data->timeline, ShowMove4, TM_AddToArgs(NULL, 1, data), "ShowMove4");
	TM_AddDelay(data->timeline, 4000);
	TM_AddAction(data->timeline, ShowMove5, TM_AddToArgs(NULL, 1, data), "ShowMove5");
	*/

	al_play_sample_instance(data->ambient);
	al_set_sample_instance_playmode(data->ambient, ALLEGRO_PLAYMODE_LOOP);
	//al_set_sample_instance_gain(data->ambient, 0.9);
}

void Gamestate_Stop(struct Game* game, struct GamestateResources* data) {
	// Called when gamestate gets stopped. Stop timers, music etc. here.
	al_stop_sample_instance(data->ambient);
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

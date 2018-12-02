#define LIBSUPERDERPY_DATA_TYPE struct CommonResources
#include <libsuperderpy.h>

struct CommonResources {
	// Fill in with common data accessible from all gamestates.
	ALLEGRO_SHADER* shader;
	ALLEGRO_BITMAP* screen;
	ALLEGRO_SAMPLE *supersample, *shodsample;
	ALLEGRO_SAMPLE_INSTANCE *super, *shod;

	ALLEGRO_BITMAP *small, *blur, *output;
	ALLEGRO_SHADER* kawese_shader;
};

struct CommonResources* CreateGameData(struct Game* game);
void DestroyGameData(struct Game* game);
void WhiteNoise(struct Game* game);
bool GlobalEventHandler(struct Game* game, ALLEGRO_EVENT* ev);
void Compositor(struct Game* game, struct Gamestate* gamestates);

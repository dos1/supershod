#define LIBSUPERDERPY_DATA_TYPE struct CommonResources
#include <libsuperderpy.h>
#include <libwebsockets.h>

struct CommonResources {
	// Fill in with common data accessible from all gamestates.
	ALLEGRO_SHADER* shader;
	ALLEGRO_BITMAP *screen, *fb;
	ALLEGRO_SAMPLE *supersample, *shodsample, *hsample, *dsample;
	ALLEGRO_SAMPLE_INSTANCE *super, *shod, *h, *d;
	bool ws;
	struct lws* ws_socket;
	struct lws_context* ws_context;
	bool ws_connected;
	char* ws_buffer;
};

typedef enum {
	WEBSOCKET_EVENT_INCOMING_MESSAGE = 2048,
	WEBSOCKET_EVENT_CONNECTING,
	WEBSOCKET_EVENT_CONNECTED,
	WEBSOCKET_EVENT_DISCONNECTED,
} WEBSOCKET_EVENT_TYPE;

typedef enum {
	SUPERSHOD_EVENT_RESET = 1024,
	SUPERSHOD_EVENT_PLAYERS,
	SUPERSHOD_EVENT_STILLPLAYING,
	SUPERSHOD_EVENT_LOST,
	SUPERSHOD_EVENT_WON,
	SUPERSHOD_EVENT_PING
} SUPERSHOD_EVENT_TYPE;

void WebSocketConnect(struct Game* game);
void WebSocketDisconnect(struct Game* game);
void WebSocketSend(struct Game* game, char* msg);
void PreLogic(struct Game* game, double delta);

struct CommonResources* CreateGameData(struct Game* game);
void DestroyGameData(struct Game* game);
void WhiteNoise(struct Game* game);
bool GlobalEventHandler(struct Game* game, ALLEGRO_EVENT* ev);
void DrawCRTScreen(struct Game* game);

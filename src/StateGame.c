#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Print.h"
#include <string.h>
#include "MapInfo.h"
#include "Music.h"
#include "Keys.h"

IMPORT_MAP(map);
IMPORT_MAP(level01);
IMPORT_MAP(level02);
IMPORT_MAP(level03);
IMPORT_MAP(level04);
IMPORT_MAP(level05);
IMPORT_MAP(level06);
IMPORT_MAP(maikel1);
IMPORT_MAP(maikel2);
IMPORT_MAP(level07);

IMPORT_TILES(blackfont);
IMPORT_TILES(font);
IMPORT_MAP(window);
DECLARE_MUSIC(level1);

#define BANKED_MAP(MAP, SECONDS) {BANK(MAP), &MAP, SECONDS}
#define LEVELS_END {0, 0}

#define BLACK_TILE 0

struct MapInfoBanked {
	UINT8 bank;
	struct MapInfo* map;
	UINT8 seconds;
};

const struct MapInfoBanked levels[] = {
	BANKED_MAP(level01, 30),
	BANKED_MAP(level02, 45),
	BANKED_MAP(level03, 60),
	BANKED_MAP(level04, 70),
	BANKED_MAP(level05, 80),
	BANKED_MAP(level06, 80),
	BANKED_MAP(maikel1, 100),
	BANKED_MAP(maikel2, 100),
	BANKED_MAP(level07, 85),

	LEVELS_END
};

#define INITIAL_ROPE_LENGTH 64

UINT8 current_level = 0;

UINT8 coll_tiles[] = {1, 2,4,5,6,7,13,15,50,51,52,53, 0};
UINT8 highscore[] = { 0,0,0,0,0,0,0,0 };

UINT8 rope_length;
UINT8 sushi_collected;
UINT8 num_clients;
UINT8 clients_collected;
UINT8 countdown;
UINT8 ticks;
UINT8 level_complete;

#define MAX_COLLECTABLES 10
UINT16 collectables_taken[MAX_COLLECTABLES + 1];

void InitRope(void) BANKED;

void LocateStuff(UINT8 map_bank, struct MapInfo* map, UINT8* start_x, UINT8* start_y) NONBANKED {
	UINT8 * data, __save_bank = CURRENT_BANK;
	SWITCH_ROM(map_bank);
	data = map->data;
	for(UINT8 y = 0; y < map->height; ++ y) {
		for(UINT8 x = 0; x < map->width; ++ x) {
			UINT8 tile = *data++;
			if(tile == 252) {          //client
				num_clients++;
			} else if (tile == 255) {  //player
				*start_x = x;
				*start_y = y;
			}
		}
	}
	SWITCH_ROM(__save_bank);
}

const UINT8 CHECKED_TILE = 75;
const UINT8 UNCHECKED_TILE = 74;
const UINT8 SUSHI_TILE= 86;
const UINT8 NOSUSHI_TILE = 87;
void RefreshSushies(void) BANKED {
#if defined(NINTENDO)
	UINT8 i;

	if (sushi_collected)
	{
		set_win_tile_xy(6, 0, SUSHI_TILE);
	}
	else
	{
		set_win_tile_xy(6, 0, NOSUSHI_TILE);
	}

	for(i = 0; i != clients_collected; ++i) {
		set_win_tile_xy(19 - i, 0, CHECKED_TILE);
	}
	for(; i != num_clients; ++i) {
		set_win_tile_xy(19 - i, 0, UNCHECKED_TILE);
	}
#endif
}

void START(void) {
	static UINT8 start_x, start_y;
	const struct MapInfoBanked* level = &levels[current_level];
	highscore[current_level] = 0;

	rope_length = INITIAL_ROPE_LENGTH;
	sushi_collected = 0;
	memset(collectables_taken, 0, sizeof(collectables_taken));
	scroll_top_movement_limit = 72;
	num_clients = 0;
	clients_collected = 0;
	countdown = level->seconds + 1;
	ticks = 59; //next frame will update the value
	level_complete = 0;

	LocateStuff(level->bank, level->map, &start_x, &start_y);
	scroll_target = SpriteManagerAdd(SpritePlayer, start_x << 3, (start_y - 1) << 3);
	InitScroll(level->bank, level->map, coll_tiles, 0);

	InitRope();

	INIT_HUD(window);
	RefreshSushies();
	
	//INIT_CONSOLE(font, 3, 2);
	INIT_FONT(font, PRINT_WIN);
	PRINT(19 - num_clients - 6, 0, "CLIENTS");
	

	INIT_SOUND();
	PlayMusic(level1, 1);
}

void UPDATE(void) {
	if(!level_complete) {
		ticks ++;
		if(ticks == 60) {
			ticks = 0;
			countdown --;
			PRINT_POS(2, 0);
			Printf("%d", (UINT16)countdown);
			if(countdown == 99 || countdown == 9) {
				Printf(" ");
			} else if(countdown == 0) {
				//Time up!
				SetState(StateTimeUp);
				HIDE_WIN;
			}
		}
	} else {
		if(KEY_TICKED(J_START)) {
			current_level ++;
			if(levels[current_level].map == 0)
				SetState(StateGameWin);
			else
				SetState(StateGame);
			HIDE_WIN;
		}
	}
}

UINT8 IsCollected(Sprite* collectable) BANKED {
	UINT8 i;
	for(i = 1; i != collectables_taken[0] + 1; ++i) {
		if(collectables_taken[i] == collectable->unique_id)
			return i;
	}
	return 255;
}

void TakeCollectable(Sprite* collectable) BANKED {
	collectables_taken[++ collectables_taken[0]] = collectable->unique_id;
	RefreshSushies();
}

void DoAnimLevelEnd(void);
void CheckLevelComplete(void) BANKED {
	if(clients_collected == num_clients) {
		highscore[current_level] = countdown;
		DoAnimLevelEnd();
	}
}

extern Sprite* player_ptr;
void ShowVictoryAnim(void) BANKED;
void DoAnimLevelEnd(void) {
	UINT8 top_bar_start    = (((player_ptr->y                     ) >> 3) - 1) % DEVICE_SCREEN_BUFFER_HEIGHT;
	UINT8 bottom_bar_start = (((player_ptr->y + player_ptr->coll_h) >> 3) + 1) % DEVICE_SCREEN_BUFFER_HEIGHT;

	UINT8 n_bars = 15;

	UINT8 x = scroll_x >> 3;

	UINT8 i;
	Sprite* spr;
	HIDE_WIN;
	SPRITEMANAGER_ITERATE(i, spr) {
		if(spr != player_ptr)
			SpriteManagerRemove(i);
	}
	
	for(i = n_bars + 1; i != 0; --i) {
		for(int j = 0; j < 21; ++j) {
			UpdateMapTile(PRINT_BKG, (x + j) & 0x1F, top_bar_start - i, 0, BLACK_TILE, NULL); 
			UpdateMapTile(PRINT_BKG, (x + j) & 0x1F, bottom_bar_start + i, 0, BLACK_TILE, NULL); 
		}
		wait_vbl_done();
	}
	ShowVictoryAnim();
	INIT_FONT(blackfont, PRINT_WIN);
	print_target = PRINT_BKG;
	PRINT_POS(x + 2 , (top_bar_start - 3) % DEVICE_SCREEN_BUFFER_HEIGHT);

	Printf(" LVL SCORE %d00  ", (INT16)highscore[current_level]);

	PRINT(x + 5, (top_bar_start - 1) % DEVICE_SCREEN_BUFFER_HEIGHT, "GOOD JOB!");	
	level_complete = 1;
}

#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Print.h"
#include <string.h>
#include <stdlib.h>
#include "MapInfo.h"
#include "Music.h"
#include "Keys.h"

IMPORT_MAP(level01);
IMPORT_MAP(level02);
IMPORT_MAP(level03);
IMPORT_MAP(level04);
IMPORT_MAP(level05);
IMPORT_MAP(level06);
IMPORT_MAP(maikel1);
IMPORT_MAP(maikel2);
IMPORT_MAP(level07);

IMPORT_FONT(blackfont);
IMPORT_FONT(font);
#if defined(NINTENDO)
IMPORT_MAP(window);
#endif
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

UINT8 coll_tiles[] = { 1, 4, 5, 6, 7, 13, 15, 50, 51, 52, 53, 0 };
UINT8 highscore[]  = { 0, 0, 0,0, 0, 0, 0, 0, 0 };

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
void InitHUD(void) BANKED;

void LocateStuff(UINT8 map_bank, struct MapInfo* map, UINT8* start_x, UINT8* start_y) NONBANKED {
	UINT8 __save_bank = CURRENT_BANK;
	SWITCH_ROM(map_bank);
	for (UINT8 * data = map->data + (map->height * map->width) - 1; data != map->data - 1; --data) {
		switch (*data) {
			case 252:
				++num_clients;
				break;
			case 255:
				*start_x = (UINT16)(data - map->data) % map->width;
				*start_y = (UINT16)(data - map->data) / map->width;
				break;
		}
	}
	SWITCH_ROM(__save_bank);
}

#if defined(NINTENDO)
#define CHECKED_TILE   75
#define UNCHECKED_TILE 74
#define SUSHI_TILE     86
#define NOSUSHI_TILE   87
#elif defined(SEGA)
#define CHECKED_TILE   3
#define UNCHECKED_TILE 5
#define SUSHI_TILE     1
#define NOSUSHI_TILE   2

extern const UINT8 hud_tiles[]; 
#endif
void RefreshSushies(void) NONBANKED {
	UINT8 i;
#if defined(NINTENDO)
	set_win_tile_xy(6, 0, (sushi_collected) ? SUSHI_TILE : NOSUSHI_TILE);

	for(i = 0; i != clients_collected; ++i) {
		set_win_tile_xy(19 - i, 0, CHECKED_TILE);
	}
	for(; i != num_clients; ++i) {
		set_win_tile_xy(19 - i, 0, UNCHECKED_TILE);
	}
#elif defined(SEGA)
	UINT8 __save = CURRENT_BANK;
	SWITCH_ROM(spriteDataBanks[SpriteHUD]);

	set_sprite_native_data(spriteIdxs[SpriteHUD] + 6, 1, hud_tiles + ((sushi_collected) ? (SUSHI_TILE << 5) : (NOSUSHI_TILE << 5)));

	for (i = 0; i != 4; ++i) {
		if (i < clients_collected) {
			set_sprite_native_data((spriteIdxs[SpriteHUD] + 1) + (i << 1), 1, hud_tiles + (CHECKED_TILE << 5));
		} else if (i < num_clients) {
			set_sprite_native_data((spriteIdxs[SpriteHUD] + 1) + (i << 1), 1, hud_tiles + (UNCHECKED_TILE << 5));
		} else {
			set_sprite_native_data((spriteIdxs[SpriteHUD] + 1) + (i << 1), 1, hud_tiles + (NOSUSHI_TILE << 5));
		}
	}	

	SWITCH_ROM(__save);
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

#if defined(NINTENDO)
	INIT_HUD(window);
#elif defined(SEGA)
	InitHUD();
#endif

	RefreshSushies();
	
	//INIT_CONSOLE(font, 3, 2);
	INIT_FONT_EX(font, PRINT_WIN);
	PRINT(19 - num_clients - 6, 0, "CLIENTS");
	

	INIT_SOUND();
	PlayMusic(level1, 1);
}

#if defined(SEGA)
BANKREF_EXTERN(font)
extern const UINT8 font_tiles[];
void RefreshTimer(INT16 value) NONBANKED {
	static UINT8 buffer[5];
	UINT8 __save = CURRENT_BANK;
	SWITCH_ROM(BANK(font));

	itoa(value, buffer, 10);
	if (buffer[1]) {
		set_sprite_native_data(spriteIdxs[SpriteHUD] + 2, 1, font_tiles + (font_recode_table[buffer[0] - ' '] << 5));
		set_sprite_native_data(spriteIdxs[SpriteHUD] + 4, 1, font_tiles + (font_recode_table[buffer[1] - ' '] << 5));
	} else {
		set_sprite_native_data(spriteIdxs[SpriteHUD] + 2, 1, font_tiles + (font_recode_table['0' - ' '] << 5));
		set_sprite_native_data(spriteIdxs[SpriteHUD] + 4, 1, font_tiles + (font_recode_table[buffer[0] - ' '] << 5));
	}

	SWITCH_ROM(__save);
}
#endif

void UPDATE(void) {
	if(!level_complete) {
		ticks ++;
		if(ticks == 60) {
			ticks = 0;
			countdown--;
#if defined(NINTENDO)
			PRINT_POS(2, 0);
			Printf("%d", (UINT16)countdown);
#elif defined(SEGA)
			RefreshTimer(countdown);
#endif
			if(countdown == 99 || countdown == 9) {
				Printf(" ");
			} else if(countdown == 0) {
				//Time up!
				SetState(StateTimeUp);
				HIDE_HUD;
			}
		}
	} else {
		if(KEY_TICKED(J_START | J_A | J_B)) {
			current_level ++;
			if(levels[current_level].map == 0)
				SetState(StateGameWin);
			else
				SetState(StateGame);
			HIDE_HUD;
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
	HIDE_HUD;
	SPRITEMANAGER_ITERATE(i, spr) {
		if(spr != player_ptr)
			SpriteManagerRemove(i);
	}
	
	for(i = n_bars + 1; i != 0; --i) {
		for(int j = 0; j < DEVICE_SCREEN_WIDTH; ++j) {
			UpdateMapTile(PRINT_BKG, x + j, top_bar_start - i, 0, BLACK_TILE, NULL); 
			UpdateMapTile(PRINT_BKG, x + j, bottom_bar_start + i, 0, BLACK_TILE, NULL); 
		}
		wait_vbl_done();
	}
	ShowVictoryAnim();
	INIT_FONT_EX(blackfont, PRINT_WIN);
	print_target = PRINT_BKG;
	PRINT_POS(x + ((DEVICE_SCREEN_WIDTH - 16) / 2), (top_bar_start - 3) % DEVICE_SCREEN_BUFFER_HEIGHT);

	Printf(" LVL SCORE %d00 ", (INT16)highscore[current_level]);

	PRINT(x + ((DEVICE_SCREEN_WIDTH - 10) / 2) + 1, (top_bar_start - 1) % DEVICE_SCREEN_BUFFER_HEIGHT, "GOOD JOB!");	
	level_complete = 1;
}

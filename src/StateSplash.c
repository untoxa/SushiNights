#include "Banks/SetAutoBank.h"
#include "main.h"
#include "Scroll.h"
#include "Keys.h"
#include "ZGBMain.h"
#include "SpriteManager.h"
#include "Print.h"
#include "Music.h"
#include "Screen.h"

IMPORT_MAP(mainmenu);
IMPORT_TILES(blackfont);
DECLARE_MUSIC(start);

extern UINT8 current_level;

void PlayerMenuSetAnim(Sprite* sprite, UINT8 idx) BANKED;

void START(void) {
	InitScroll(BANK(mainmenu), &mainmenu, 0, 0);
	Sprite* sprite = SpriteManagerAdd(SpritePlayerMenu, 122 + (TILE_X_OFFSET * 8), 56 + (TILE_Y_OFFSET * 8));
	PlayerMenuSetAnim(sprite, 0);
	sprite->mirror = V_MIRROR;
	
	INIT_FONT(blackfont, PRINT_BKG);
	PRINT(0 + TILE_X_OFFSET, 13 + TILE_Y_OFFSET, " PRESS START ");	
	PRINT(1 + TILE_X_OFFSET, 16 + TILE_Y_OFFSET, "MONIS GAMES");
	PRINT(2 + TILE_X_OFFSET, 17 + TILE_Y_OFFSET, "GBCOMPO21");
	PlayMusic(start, 1);
}

void UPDATE(void) {
	if(KEY_TICKED(J_START | J_A | J_B)) {
		current_level = 0;
		SetState(StateGame);
//		SetState(StateGameWin); //FOR TESTING
	}
}
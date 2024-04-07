#include "Banks/SetAutoBank.h"
#include "main.h"
#include "Scroll.h"
#include "Keys.h"
#include "ZGBMain.h"
#include "SpriteManager.h"
#include "Print.h"
#include "savegame.h"
#include "Screen.h"

IMPORT_FONT(blackfont);
IMPORT_MAP(victory);
extern UINT8 anim_idle_menu[];

extern UINT8 highscore[];
extern UINT8 current_level;

void PlayerMenuSetAnim(Sprite* sprite, UINT8 idx) BANKED;
UINT16 totalScore = 0;

void START(void) {
	InitScroll(BANK(victory), &victory, 0, 0);		
	Sprite* sprite = SpriteManagerAdd(SpritePlayerMenu, 64 + (TILE_X_OFFSET * 8), 64 + (TILE_Y_OFFSET * 8));
	PlayerMenuSetAnim(sprite, 0);

//	sprite = SpriteManagerAdd(SpritePlayerMenu, 60 + (TILE_X_OFFSET * 8),  12 + (TILE_Y_OFFSET * 8));
//	PlayerMenuSetAnim(sprite, 3);
//	sprite = SpriteManagerAdd(SpritePlayerMenu, 80 + (TILE_X_OFFSET * 8),  12 + (TILE_Y_OFFSET * 8));
//	PlayerMenuSetAnim(sprite, 3);
//	sprite = SpriteManagerAdd(SpritePlayerMenu, 100 + (TILE_X_OFFSET * 8), 12 + (TILE_Y_OFFSET * 8));
//	PlayerMenuSetAnim(sprite, 3);
	 
	sprite = SpriteManagerAdd(SpritePlayerMenu, 2 + (TILE_X_OFFSET * 8),   42 + (TILE_Y_OFFSET * 8));
	PlayerMenuSetAnim(sprite, 1);

	sprite = SpriteManagerAdd(SpritePlayerMenu, 140 + (TILE_X_OFFSET * 8), 42 + (TILE_Y_OFFSET * 8));
	PlayerMenuSetAnim(sprite, 2);
	
	INIT_FONT_EX(blackfont, PRINT_BKG);
	PRINT(6 + TILE_X_OFFSET, 13 +  TILE_Y_OFFSET, " YOU WIN");

	totalScore = 0;
	for (UINT8 i = 0; i < current_level; i++)
	{
		totalScore += highscore[i];
	}
	
	PRINT_POS(3 + TILE_X_OFFSET, 4 +  TILE_Y_OFFSET);
	Printf("SCORE:  %d00 ", totalScore);

#ifdef USE_SAVEGAME
	ENABLE_RAM;
	if (savegame.best_record < totalScore) savegame.best_record = totalScore;
	PRINT_POS(3 + TILE_X_OFFSET, 6 +  TILE_Y_OFFSET);
	Printf("BEST:   %d00 ", savegame.best_record);
	DISABLE_RAM;
#endif
}

void UPDATE(void) {
	if(KEY_TICKED(J_START | J_A | J_B)) {
		SetState(StateSplash);
	}
}
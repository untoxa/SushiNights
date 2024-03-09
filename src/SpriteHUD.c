#include "Banks/SetAutoBank.h"
#include "SpriteManager.h"
#include "ZGBMain.h"

extern UINT8 next_oam_idx;

#if defined(NINTENDO)

#include "MetaSpriteInfo.h"

BANKREF(hud)
const struct MetaSpriteInfo hud = {0, 0, 0, NULL, 0, NULL, 0, NULL};
const metasprite_t hud_metasprite0[] = {METASPR_TERM};

#define HUD_X 0
#define HUD_Y 0

#elif defined(SEGA)

#if defined(MASTERSYSTEM)
#define HUD_X (DEVICE_SPRITE_PX_OFFSET_X + 8)
#else
#define HUD_X DEVICE_SPRITE_PX_OFFSET_X
#endif
#define HUD_Y (DEVICE_SPRITE_PX_OFFSET_Y + DEVICE_SCREEN_PX_HEIGHT - 16)

#endif

BANKREF_EXTERN(hud)
extern const metasprite_t hud_metasprite0[];

void InitHUD(void) BANKED {
	SpriteManagerLoad(SpriteHUD);
}

void RenderHUD(void) NONBANKED {
	UINT8 __save = CURRENT_BANK;
	SWITCH_ROM(BANK(hud));
	next_oam_idx += move_metasprite_ex(hud_metasprite0, spriteIdxs[SpriteHUD], 0, next_oam_idx, HUD_X, HUD_Y);
	SWITCH_ROM(__save);
}

void START(void) {
}

void UPDATE(void) {
}

void DESTROY(void) {
}
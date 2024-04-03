#include "Banks/SetAutoBank.h"

#include "main.h"
#include "Scroll.h"
#include "Keys.h"
#include "ZGBMain.h"
#include "SGB.h"
#include "Palette.h"
#include "Sounds.h"
#include "Music.h"

IMPORT_MAP(splash_zgb);
IMPORT_BORDER(border);

static UINT8 n_frames;

void START(void) {
	INIT_SOUND();
	LOAD_SGB_BORDER(border);
	SetBorderColor(RGB(0, 0, 0));
	n_frames = 0;
	InitScroll(BANK(splash_zgb), &splash_zgb, 0, 0);
}

void UPDATE(void) {
	n_frames ++;

#ifdef NINTENDO
	if(n_frames == 30) {
		ExecuteSFX(BANK(FX_SHEEP), FX_SHEEP, SFX_MUTE_MASK(FX_SHEEP), SFX_PRIORITY_NORMAL);
	}
#endif

	if(n_frames == 240 || KEY_TICKED(J_START | J_A | J_B)) {
		SetState(StateSplash);
	}
}
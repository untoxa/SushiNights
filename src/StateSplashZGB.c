#include "Banks/SetAutoBank.h"
#include "main.h"
#include "Scroll.h"
#include "Keys.h"
#include "ZGBMain.h"
#include "SGB.h"

IMPORT_MAP(splash_zgb);
IMPORT_MAP(border);

static UINT8 n_frames;

void START(void) {
	LOAD_SGB_BORDER(border);
	n_frames = 0;
	InitScroll(BANK(splash_zgb), &splash_zgb, 0, 0);
}

void UPDATE(void) {
	n_frames ++;

	if(n_frames == 120 || KEY_TICKED(J_START | J_A | J_B)) {
		SetState(StateSplash);
	}
}
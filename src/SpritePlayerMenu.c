#include "Banks/SetAutoBank.h"
#include "SpriteManager.h"
#include "Vector.h"

static const UINT8 anim_idle_menu[] = VECTOR(0, 1);
static const UINT8 anim_frog[]      = VECTOR(2, 3);
static const UINT8 anim_stag[]      = VECTOR(4, 5);
static const UINT8 anim_sushi[]     = VECTOR(6);
static const UINT8 anim_roll[]      = VECTOR(7);

static UINT8 current_sprite;
static UINT8 tick, timer;

void PlayerMenuSetAnim(Sprite* sprite, UINT8 idx) BANKED {
	switch(current_sprite = idx) {
		case 0: SetSpriteAnim(sprite, anim_idle_menu, 6u); break;
		case 1: SetSpriteAnim(sprite, anim_frog, 6u); break;
		case 2: SetSpriteAnim(sprite, anim_stag, 6u); break;
		case 3: SetSpriteAnim(sprite, anim_sushi, 6u); break;
		case 4: SetSpriteAnim(sprite, anim_roll, 6u); break;
	}
}

void START(void) {
	tick = timer = 0;
}

void UPDATE(void) {
	if ((++tick & 0x3f) == 0) {
		if ((++timer & 0x0f) == 0) {
			if (++current_sprite > 2) current_sprite = 0; 
	   		PlayerMenuSetAnim(THIS, current_sprite);
		}
	}
	
}

void DESTROY(void) {
}
#include "Banks/SetAutoBank.h"

#include "SpriteManager.h"
#include "Sound.h"
#include "Vector.h"

DECLARE_SFX(FX_PICKUP);

extern Sprite* player_ptr;
extern UINT8 sushi_collected;
UINT8 IsCollected(Sprite* collectable) BANKED;
void TakeCollectable(Sprite* powerup) BANKED;
void DeliverSushi(Sprite* client) BANKED;

static const UINT8 anim_idle[]      = VECTOR(0, 1);
static const UINT8 anim_idle_alt[]  = VECTOR(4, 5);
static const UINT8 anim_happy[]     = VECTOR(2, 3);
static const UINT8 anim_happy_alt[] = VECTOR(6, 7);

#define ANIM(A) (((THIS->x >> 3) & 0x2) == 0) ? A : A ## _alt

void START(void) {
	if(IsCollected(THIS) == 255) {
		SetSpriteAnim(THIS, ANIM(anim_idle), 4);
	} else {
		SetSpriteAnim(THIS, ANIM(anim_happy), 4);
	}
}

void UPDATE(void) {
	if(sushi_collected && (THIS->anim_data == anim_idle || THIS->anim_data == anim_idle_alt)) {
		if(CheckCollision(THIS, player_ptr)) {
			SetSpriteAnim(THIS, ANIM(anim_happy), 4);
			sushi_collected = 0;
			TakeCollectable(THIS);
			ExecuteSFX(BANK(FX_PICKUP), FX_PICKUP, SFX_MUTE_MASK(FX_PICKUP), SFX_PRIORITY_NORMAL);
			
			DeliverSushi(THIS);
		}
	}
}

void DESTROY(void) {
}
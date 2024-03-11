#include "Banks/SetAutoBank.h"
#include "Math.h"
#include "SpriteManager.h"
#include "Vector.h"

static const UINT8 anim_bubble[] = VECTOR(0,1,2,2);


void START(void) {
	SetSpriteAnim(THIS, anim_bubble,18u);
}

void UPDATE(void) {
	if(U_LESS_THAN(2,THIS->anim_frame))
		SpriteManagerRemoveSprite(THIS);

}

void DESTROY(void) {
}
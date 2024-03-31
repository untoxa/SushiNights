#include "Banks/SetAutoBank.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "Scroll.h"
#include "Trig.h"
#include "ZGBMain.h"
#include "Scroll.h"
#include <string.h>
#include "Keys.h"
#include "Sounds.h"

extern UINT8 next_oam_idx;
extern UINT8 rope_length;

typedef struct {
	UINT8 dist;
	INT8 ang;
	UINT8 hooked;
	UINT8 max_length;
	UINT8 done;
} CUSTOM_DATA;

Sprite* hook_ptr = 0;
extern Sprite* player_ptr;
void HookPlayer(UINT16 x, UINT16 y, INT8 ang, UINT8 radius) BANKED;

void InitRope(void) BANKED {
	SpriteManagerLoad(SpriteHook);
}

void START(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	data->dist = 0;
	if(KEY_PRESSED(J_UP)){
		data->ang = -64;
	} else {
		data->ang = player_ptr->mirror == NO_MIRROR ? -32 : 128 + 32;
	}
	data->hooked = 0;
	data->max_length = rope_length;
	data->done = 0;

	hook_ptr = THIS;
}

metasprite_t hook_rope[] = {
#if defined(MASTERSYSTEM)
	METASPR_ITEM(0, 8, 2, 0),
#else
	METASPR_ITEM(0, 0, 2, 0),
#endif
	METASPR_ITEM(0, 0, 2, 0),
	METASPR_ITEM(0, 0, 2, 0),
	METASPR_ITEM(0, 0, 2, 0),
	METASPR_TERM
};

void DrawRope(void) {
	static UINT8 i, start_x, start_y;
	static INT8 step_x, step_y;
	
	i = (player_ptr->coll_w >> 1) - 1;

	start_x = player_ptr->x - scroll_x + DEVICE_SPRITE_PX_OFFSET_X + i;
	step_x = ((THIS->x - player_ptr->x - i) >> 2);

	start_y = player_ptr->y - scroll_y + DEVICE_SPRITE_PX_OFFSET_Y;
	step_y = (THIS->y - player_ptr->y) >> 2;


	hook_rope[3].dy = hook_rope[2].dy = hook_rope[1].dy = step_y;
	hook_rope[3].dx = hook_rope[2].dx = hook_rope[1].dx = step_x;

	next_oam_idx += move_metasprite_ex(hook_rope, spriteIdxs[SpriteHook], 0, next_oam_idx, start_x, start_y);
}

const UINT8 hook_collidable[256] = {
	[12] = 1, [14] = 1,  [17] = 1, [19] = 1, [21] = 1, [23] = 1, [32] = 1, [34] = 1,  [36] = 1,  [37] = 1, [38] = 1 
};

void UPDATE(void) {
	static fixed radius;
	static fixed tmp_x;
	static fixed tmp_y;
	UINT8 coll_tile;
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	if(!data->hooked) {
		data->dist += 5;
		radius.w = ((SIN(data->dist) * data->max_length) >> 6);

		if((INT16)radius.w >= 0) {
			tmp_x.w = COS(data->ang) * radius.w;
			tmp_y.w = SIN(data->ang) * radius.w;

			THIS->x = player_ptr->x + (player_ptr->coll_w >> 1) + (INT8)tmp_x.h;
			THIS->y = player_ptr->y + (INT8)tmp_y.h;

			if(!data->done && THIS->x < scroll_w && THIS->y < scroll_h) { 
				coll_tile = GetScrollTile(THIS->x >> 3, THIS->y >> 3);
				if(scroll_collisions[coll_tile] == 1 || (hook_collidable[coll_tile])) {
					if (coll_tile == 2){
						if(data->dist < 64) {
							data->dist = 128 - data->dist;
						}
					} else {
						data->hooked = 1;
						HookPlayer(THIS->x, THIS->y, data->ang, radius.l);
						ExecuteSFX(BANK(FX_HIT), FX_HIT, SFX_MUTE_MASK(FX_HIT), SFX_PRIORITY_NORMAL);
					}
				} 
			}
		} else {
			SpriteManagerRemove(THIS_IDX);
		}
	}

	DrawRope();
}

void RetireHook(Sprite* hook, INT8 ang, INT8 radius) BANKED {
	CUSTOM_DATA* data = (CUSTOM_DATA*)hook->custom_data;
	data->hooked = 0;
	data->ang = ang > 0 ? 128 - (ang - 64) : (-64 - ang);
	data->max_length = radius;
	data->done = 1;
	if(data->dist < 64) {
		data->dist = 128 - data->dist;
	}
}

void DESTROY(void) {
	hook_ptr = 0;
}
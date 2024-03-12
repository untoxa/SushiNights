#include "ZGBMain.h"
#include "Math.h"

#ifdef NDEBUG
UINT8 next_state = StateSplashZGB;
#else
UINT8 next_state = StateGame;
#endif

UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile) {
	if(current_state == StateGame) {
		if((0xffu - *tile_ptr) < N_SPRITE_TYPES) {
			*tile = 0;
			return 0xffu - *tile_ptr;
		}
		*tile = *tile_ptr;
	}

	return 0xffu;
}
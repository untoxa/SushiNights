#ifndef SOUNDS_H
#define SOUNDS_h

#include "Sound.h"

BANKREF_EXTERN(FX_JUMP)
extern void __mute_mask_FX_JUMP;
extern const UINT8 FX_JUMP[];

BANKREF_EXTERN(FX_PICKUP)
extern void __mute_mask_FX_PICKUP;
extern const UINT8 FX_PICKUP[];

BANKREF_EXTERN(FX_HIT)
extern void __mute_mask_FX_HIT;
extern const UINT8 FX_HIT[];

BANKREF_EXTERN(FX_HIT_WALL)
extern void __mute_mask_FX_HIT_WALL;
extern const UINT8 FX_HIT_WALL[];

#endif
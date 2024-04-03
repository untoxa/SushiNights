#ifndef SOUNDS_H
#define SOUNDS_h

#include "Sound.h"

BANKREF_EXTERN(FX_JUMP)
extern const UINT8 FX_JUMP[];
extern void __mute_mask_FX_JUMP;

BANKREF_EXTERN(FX_PICKUP)
extern const UINT8 FX_PICKUP[];
extern void __mute_mask_FX_PICKUP;

BANKREF_EXTERN(FX_HIT)
extern const UINT8 FX_HIT[];
extern void __mute_mask_FX_HIT;

BANKREF_EXTERN(FX_HIT_WALL)
extern const UINT8 FX_HIT_WALL[];
extern void __mute_mask_FX_HIT_WALL;

BANKREF_EXTERN(FX_SHEEP)
extern const uint8_t FX_SHEEP[];
extern void __mute_mask_FX_SHEEP;

#endif
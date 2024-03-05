#include "Banks/SetAutoBank.h"
#include "Sound.h"

#if defined(NINTENDO)

BANKREF(FX_JUMP)
void AT(CHANNEL_1) __mute_mask_FX_JUMP;
const UINT8 FX_JUMP[] = { 0xA1, 0b11111000, 0x37, 0x80, 0xf4, 0xb7, 0x84, 0x01, 0b00000111};

BANKREF(FX_PICKUP)
void AT(CHANNEL_1) __mute_mask_FX_PICKUP;
const UINT8 FX_PICKUP[] = { 0xA1, 0b11111000, 0x45, 0x00, 0xff, 0x1f, 0x86, 0x01, 0b00000111};

BANKREF(FX_HIT)
void AT(CHANNEL_4) __mute_mask_FX_HIT;
const UINT8 FX_HIT[] = { 0xA1, 0b11110011, 0x0f, 0xf2, 0x64, 0x80, 0x01, 0b00000111};

BANKREF(FX_HIT_WALL)
void AT(CHANNEL_1) __mute_mask_FX_HIT_WALL;
const UINT8 FX_HIT_WALL[] = { 0xA1, 0b11111000, 0x1B, 0x00, 0xf1, 0x73, 0x86, 0x01, 0b00000111};

#elif defined(SEGA)

BANKREF(FX_JUMP)
void AT(CHANNEL_1) __mute_mask_FX_JUMP;
const UINT8 FX_JUMP[] = { 0xA1, 0b11111000, 0x37, 0x80, 0xf4, 0xb7, 0x84, 0x01, 0b00000111};

BANKREF(FX_PICKUP)
void AT(CHANNEL_1) __mute_mask_FX_PICKUP;
const UINT8 FX_PICKUP[] = { 0xA1, 0b11111000, 0x45, 0x00, 0xff, 0x1f, 0x86, 0x01, 0b00000111};

BANKREF(FX_HIT)
void AT(CHANNEL_4) __mute_mask_FX_HIT;
const UINT8 FX_HIT[] = { 0xA1, 0b11111011, 0x0f, 0xf2, 0x64, 0x80, 0x01, 0b00000111};

BANKREF(FX_HIT_WALL)
void AT(CHANNEL_1) __mute_mask_FX_HIT_WALL;
const UINT8 FX_HIT_WALL[] = { 0xA1, 0b11111000, 0x1B, 0x00, 0xf1, 0x73, 0x86, 0x01, 0b00000111};

#endif 

#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#define STATES \
_STATE(StateSplashZGB)\
_STATE(StateSplash)\
_STATE(StateGame)\
_STATE(StateGameWin)\
_STATE(StateTimeUp)\
STATE_DEF_END

#define SPRITES \
_SPRITE(SpritePlayer,      player,       V_MIRROR)\
_SPRITE(SpriteHook,        hook,         FLIP_NONE)\
_SPRITE(SpritePowerupRope, powerup_rope, FLIP_NONE)\
_SPRITE(SpriteClient,      client,       FLIP_NONE)\
_SPRITE(SpriteSushi,       sushi,        FLIP_NONE)\
_SPRITE(SpritePlayerMenu,  playermenu,   V_MIRROR)\
_SPRITE(SpriteBubble,      bubble,       FLIP_NONE)\
_SPRITE(SpriteHUD,         hud,          FLIP_NONE)\
SPRITE_DEF_END

#include "ZGBMain_Init.h"

#endif
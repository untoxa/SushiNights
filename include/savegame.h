#ifndef SAVEGAME_H
#define SAVEGAME_H

#include "SRAM.h"

typedef struct {
	SAVEGAME_HEADER;

  //Whatever content you want to store in external ram

} Savegame;

extern Savegame savegame;

#endif

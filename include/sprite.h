#ifndef SPRITE_H
#define SPRITE_H

#include <nds/ndstypes.h>
#include <nds/arm9/sprite.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Sprite {
    int x;
    int y;
    u16* gfx;
} Sprite;

Sprite* createSprite(OamState *oam, int x, int y);

void renderSprite(Sprite *sprite, OamState *oam, int oamId);

#ifdef __cplusplus
}
#endif

#endif
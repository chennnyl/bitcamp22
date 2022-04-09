#ifndef SPRITE_H
#define SPRITE_H

#include <nds/ndstypes.h>
#include <nds/arm9/sprite.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A structure to represent an image in screen space
 * 
 */
typedef struct Sprite {
    int x; /**The x coordinate of the sprite in pixels*/
    int y; /**The y coordinate of the sprite in pixels*/
    u16* gfx; /**A pointer to the space in VRAM the sprite's image occupies*/
} Sprite;

Sprite* createSprite(OamState *oam, int x, int y);

void renderSprite(Sprite *sprite, OamState *oam, int oamId);

#ifdef __cplusplus
}
#endif

#endif
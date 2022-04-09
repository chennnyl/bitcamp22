#include <nds/arm9/sprite.h>
#include "sprite.h"

/**
 * @brief Create a Sprite object with an initial sprite of a square colored SPRITE_PALLETE[1]
 * 
 * @param oam libnds OAM reference
 * @param x Initial x position of the sprite
 * @param y Initial y position of the sprite
 * @return Sprite* 
 */
Sprite* createSprite(OamState *oam, int x, int y) {
    u16 *gfx = oamAllocateGfx(oam, SpriteSize_16x16, SpriteColorFormat_256Color);
    for(int i = 0; i < 16*16/2; i++) {
        gfx[i] = 1 | (1 << 8);
    }

    Sprite *sp = malloc(sizeof(Sprite));
    sp->gfx = gfx;
    sp->x = x;
    sp->y = y;

    return sp;
}

/**
 * @brief Render a sprite to one of the screens
 * 
 * @param sprite Sprite to render
 * @param oam libnds OAM reference
 * @param oamId OAM id to render the sprite to
 */
void renderSprite(Sprite *sprite, OamState *oam, int oamId) {
    oamSet(
        oam, oamId, sprite->x, sprite->y, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, sprite->gfx, -1, false, false, false, false, false
    );
}
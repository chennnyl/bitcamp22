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
    bool shown;
    SpriteSize size;
} Sprite;

/**
 * @brief Create a Sprite object with an initial sprite of a square colored SPRITE_PALLETE[1]
 * 
 * @param oam libnds OAM reference
 * @param x Initial x position of the sprite
 * @param y Initial y position of the sprite
 * @param size Sprite size to use
 * @return Sprite* 
 */
Sprite* createSprite(OamState *oam, int x, int y, SpriteSize size);

/**
 * @brief Render a sprite to one of the screens
 * 
 * @param sprite Sprite to render
 * @param oam libnds OAM reference
 * @param oamId OAM id to render the sprite to
 * @param hide Whether or not to show the sprite
 */
void renderSprite(Sprite *sprite, OamState *oam, int oamId, bool hide);


/**
 * @brief Check whether a sprite is on screen (should be rendered)
 * 
 * @param sprite 
 * @return true The sprite is on screen
 * @return false The sprite is not on screen
 */

bool sprite_on_screen(Sprite *sprite);

#ifdef __cplusplus
}
#endif

#endif
#include <nds.h>
#include <nds/arm9/sprite.h>
#include <nds/arm9/video.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sprite.h"

int main(void) {
    videoSetMode(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_SPRITE);
    oamInit(&oamMain, SpriteMapping_1D_32, false);

    Sprite *sprite = createSprite(&oamMain, 128, 32);

    SPRITE_PALETTE[1] = RGB15(31, 0, 0);
    // SPRITE_PALETTE[2] = RGB15(0, 31, 31);

    while(1) {
        renderSprite(sprite, &oamMain, 0);
        sprite->x = (sprite->x + 1) % 270;
        swiWaitForVBlank();
        oamUpdate(&oamMain);
    }

    return 0;
}
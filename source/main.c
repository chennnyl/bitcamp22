#define DEBUG true

#include <nds.h>
#include <nds/arm9/sprite.h>
#include <nds/arm9/video.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <nds/arm9/console.h>

#include "sprite.h"
#include "gfx/image.h"
#include "gfx/palette.h"

void debug(void) {
    consoleDebugInit(DebugDevice_NOCASH);
}

int main(void) {
    videoSetMode(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_SPRITE);
    oamInit(&oamMain, SpriteMapping_1D_32, false);
    
    if(DEBUG) {
        debug();
    }

    Sprite *sprite = createSprite(&oamMain, 128, 32);

    // SPRITE_PALETTE[1] = RGB15(31, 0, 0);
    // SPRITE_PALETTE[2] = RGB15(0, 31, 31);
    for(u16 col = 0; col < 16; col++) {
        SPRITE_PALETTE[col] = palette[col];
    }
    for(u16 i = 0; i < 16*16/2; i++) {
        sprite->gfx[i] = image[i];
    }

    while(1) {
        renderSprite(sprite, &oamMain, 0);
        sprite->x = (sprite->x + 1) % 270;

        fprintf(stderr, "%x\n", sprite->x);

        swiWaitForVBlank();
        oamUpdate(&oamMain);
    }

    return 0;
}
#include <nds.h>
#include <nds/arm9/sprite.h>
#include <nds/arm9/video.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    videoSetMode(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_SPRITE);
    oamInit(&oamMain, SpriteMapping_1D_32, false);

    u16* gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
    for(int i = 0; i < 16*16/2; i++) {
        gfx[i] = 1 | (1 << 8);
    }

    SPRITE_PALETTE[1] = RGB15(31, 0, 0);
    // SPRITE_PALETTE[2] = RGB15(0, 31, 31);

    while(1) {
        oamSet(
            &oamMain, 0, 128, 96, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, gfx, -1, false, false, false, false, false
        );
        swiWaitForVBlank();
        oamUpdate(&oamMain);
    }

    return 0;
}
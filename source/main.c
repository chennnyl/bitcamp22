#define DEBUG true

#include <nds.h>
#include <nds/arm9/sprite.h>
#include <nds/arm9/video.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <nds/arm9/console.h>

#include "sprite.h"
#include "physics.h"
#include "render.h"
#include "gfx/gfx.h"

/**
 * @brief Initialize debug state
 * 
 */
void debug(void) {
    consoleDebugInit(DebugDevice_NOCASH);
}

Scene* globalScene;
Physics* globalPhysics;


int main(void) {
    videoSetMode(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_SPRITE);
    oamInit(&oamMain, SpriteMapping_1D_32, false);
    
    if(DEBUG) {
        debug();
    }

    Sprite *sprite = createSprite(&oamMain, 128, 32);

    for(u16 col = 0; col < 16; col++) {
        SPRITE_PALETTE[col] = palette[col];
    }
    for(u16 i = 0; i < 16*16/2; i++) {
        sprite->gfx[i] = image[i];
    }

    globalScene = createScene();
    globalPhysics = globalScene->engine;

    // this needs to get actually defined
    Collider* coll = phys_col_Construct(globalPhysics, VEC2_ZERO, VEC2_IDENT);
    PhysicsObject* obj = createPhysicsObject(coll, sprite);
    scene_add_obj(obj, globalScene);

    while(1) {
        renderScene(globalScene);

        swiWaitForVBlank();
        oamUpdate(&oamMain);
    }

    return 0;
}
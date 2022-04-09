#define DEBUG true

#include <nds.h>
#include <nds/ndstypes.h>
#include <nds/timers.h>
#include <nds/arm9/sprite.h>
#include <nds/arm9/video.h>
#include <nds/arm9/console.h>
#include <nds/arm9/input.h>
#include <stdio.h>

#include "gfx/gfx.h"
#include "render.h"

/**
 * @brief Initialize debug state
 * 
 */
void debug(void) {
    consoleDebugInit(DebugDevice_NOCASH);
}

Scene* globalScene;
Physics* globalPhysics;
touchPosition touch;
uint32 keyState;
bool wasTouched;


void physicsStep(void);
void inputProcess(void);

int main(void) {
    wasTouched = false;
    videoSetMode(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_SPRITE);
    oamInit(&oamMain, SpriteMapping_1D_32, false);
    
    if(DEBUG) debug();

    Sprite *sprite = createSprite(&oamMain, 0, 0);
    Sprite *sprite2 = createSprite(&oamMain, 0, 0);

    sprite2->gfx = sprite->gfx;
    for(u16 col = 0; col < 16; col++) {
        SPRITE_PALETTE[col] = palette[col];
    }
    for(u16 i = 0; i < 16*16/2; i++) {
        sprite->gfx[i] = image[i];
    }

    globalScene = createScene();
    globalPhysics = globalScene->engine;

    Collider* coll = phys_col_Construct(globalPhysics, intVector(6, 1), VEC2_IDENT);

    RigidPhysicsObject* obj = createRigidPhysicsObject(RigidbodyCreator(globalPhysics, VEC2_ZERO, VEC2_IDENT, inttof32(1)), sprite);
    PhysicsObject* obj2 = createPhysicsObject(coll, sprite2);
    scene_add_rigid_obj(obj, globalScene);
    scene_add_obj(obj2, globalScene);

    fprintf(stderr, "%d\n", f32toint(inttof32(-10)));

    timerStart(0, ClockDivider_1024, TIMER_FREQ_1024(30), physicsStep);

    while(1) {
        renderScene(globalScene);

        inputProcess();
        swiWaitForVBlank();
        oamUpdate(&oamMain);
    }

    return 0;
}

void inputProcess(void) {
    touchRead(&touch);
    scanKeys();

    keyState = keysCurrent();

    if(keyState & KEY_RIGHT) globalScene->camera.x += 1;
    if(keyState & KEY_UP) globalScene->camera.y += 1;
    if(keyState & KEY_LEFT) globalScene->camera.x -= 1;
    if(keyState & KEY_DOWN) globalScene->camera.y -= 1;

    if((touch.px||touch.py) && !wasTouched) {
        wasTouched = true;
        
        Point touchPoint = { touch.px, touch.py };
        Vector2 destinationPoint = screenToWorld(globalScene->camera, touchPoint);

        Collider* coll = phys_col_Construct(globalPhysics, destinationPoint, VEC2_IDENT);
        Sprite* spr = createSprite(&oamMain, 0, 0);
        PhysicsObject* phy = createPhysicsObject(coll, spr);

        scene_add_obj(phy, globalScene);

    } else if(!(touch.px || touch.py) && wasTouched) {
        wasTouched = false;
    }
}

void physicsStep(void) {
}
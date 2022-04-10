#define DEBUG true

#include <nds.h>
#include <nds/ndstypes.h>
#include <nds/timers.h>
#include <nds/arm9/sprite.h>
#include <nds/arm9/math.h>
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
Sprite* touchIndicator;
touchPosition touch;
touchPosition lastTouch;
uint32 keyState;
bool wasTouched;


void physicsStep(void);
void inputProcess(void);

int main(void) {
    wasTouched = false;
    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_SPRITE);
    vramSetBankD(VRAM_D_SUB_SPRITE);
    oamInit(&oamMain, SpriteMapping_1D_32, false);
    oamInit(&oamSub, SpriteMapping_1D_32, false);
    
    if(DEBUG) debug();

    Sprite *sprite = createSprite(&oamMain, 0, 0, SpriteSize_16x16);
    
    touchIndicator = createSprite(&oamSub, 128, 96, SpriteSize_8x8);

    // sprite2->gfx = sprite->gfx;
    for(u16 col = 0; col < 16; col++) {
        SPRITE_PALETTE[col] = palette[col];
        SPRITE_PALETTE_SUB[col] = palette[col];
    }

    for(u16 i = 0; i < 16*16/2; i++) {
        sprite->gfx[i] = image[i];
    }
    for(u16 i = 0; i < 8*8/2; i++) {
        touchIndicator->gfx[i] = image[i];
    }

    globalScene = createScene();
    globalPhysics = phys_Construct(floattof32(-10.0f));
    globalScene->engine = globalPhysics;

    // Collider* coll = phys_col_Construct(globalPhysics, intVector(6, 1), VEC2_IDENT);

    RigidPhysicsObject* obj = createRigidPhysicsObject(RigidbodyCreator(globalPhysics, VEC2_ZERO, VEC2_IDENT, inttof32(2)), sprite);
    // PhysicsObject* obj2 = createPhysicsObject(coll, sprite2);
    scene_add_rigid_obj(obj, globalScene);
    // scene_add_obj(obj2, globalScene);

    addSpriteToScene(globalScene, obj->sprite);
    // addSpriteToScene(globalScene, obj2->sprite);

    timerStart(0, ClockDivider_1024, TIMER_FREQ_1024(30), physicsStep);

    while(1) {
        update_objects(globalScene);
        renderScene(globalScene);

        inputProcess();
        swiWaitForVBlank();
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
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
    
        touchIndicator->x = touch.px - 4;
        touchIndicator->y = touch.py - 4;

    } else if(!(touch.px || touch.py) && wasTouched) {
        wasTouched = false;
        
        Point touchPoint = { touchIndicator->x, touchIndicator->y };
        Vector2 destinationPoint = screenToWorld(globalScene->camera, touchPoint);

        // Collider* coll = phys_col_Construct(globalPhysics, destinationPoint, VEC2_IDENT);
        Sprite* spr = createSprite(&oamMain, 0, 0, SpriteSize_16x16);
        RigidPhysicsObject* phy = createRigidPhysicsObject(RigidbodyCreator(globalPhysics, destinationPoint, VEC2_IDENT, inttof32(10)), spr);
        
        Vector2 direction = vec2_sub(intVector(touchIndicator->x, -touchIndicator->y), intVector(lastTouch.px, -lastTouch.py));
        fprintf(stderr, "<%d %d> <%d %d>\n", touchIndicator->x, touchIndicator->y, lastTouch.px, lastTouch.py);
        fprintf(stderr, "%ld\n", f32toint(vec2_mag(direction)));

        phys_rb_addForce(phy->rb, vec2_scale(direction, vec2_mag(direction)));

        scene_add_rigid_obj(phy, globalScene);

        addSpriteToScene(globalScene, spr);

    }
    if(wasTouched) lastTouch = touch;
    renderSprite(touchIndicator, &oamSub, 0, !wasTouched);
    oamSet(&oamSub, 1, touch.px-4, touch.py-4, 0, 0, SpriteSize_8x8, SpriteColorFormat_256Color, touchIndicator->gfx, -1, false, !wasTouched, false, false, false);

}

void physicsStep(void) {
    const static fixed32 step = floattof32(1.0f/10.0f);
    phys_step(globalScene->engine, step);
}
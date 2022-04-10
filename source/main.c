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

Sprite* floorSprite;
RigidPhysicsObject* compositeFloor;

void initRigids(void) {
    floorSprite = createSprite(&oamMain, 0, 0, SpriteSize_32x16);

    dmaCopy(floorsp, floorSprite->gfx, FLOORSP_SIZE);

    compositeFloor = createRigidPhysicsObject(RigidbodyCreator(globalPhysics, intVector(-128, -80), intVector(256, 16), 0, floattof32(0.2f)), NULL);

    scene_add_rigid_obj(compositeFloor, globalScene);
    // addSpriteToScene(globalScene, floorSprite);
}


void physicsStep(void);
void inputProcess(void);

int main(void) {
    wasTouched = false;
    videoSetMode(MODE_5_2D);
    videoSetModeSub(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_SPRITE);
    vramSetBankB(VRAM_B_MAIN_BG_0x06000000);
    vramSetBankD(VRAM_D_SUB_SPRITE);
    oamInit(&oamMain, SpriteMapping_1D_32, false);
    oamInit(&oamSub, SpriteMapping_1D_32, false);
    int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_512x512, 0, 0);
    for(u64 i = 512*180/2; i < 512*192/2; i+=512/2) {
        dmaCopy(floorsp, bgGetGfxPtr(bg3)+i, 512);
    }
    dmaCopy(palette, BG_PALETTE, PALETTE_SIZE);
    dmaCopy(palette, SPRITE_PALETTE, PALETTE_SIZE);
    dmaCopy(palette, SPRITE_PALETTE_SUB, PALETTE_SIZE);
    
    if(DEBUG) debug();

    Sprite *sprite = createSprite(&oamMain, 0, 0, SpriteSize_16x16);
    
    touchIndicator = createSprite(&oamSub, 128, 96, SpriteSize_8x8);

    dmaCopy(image, sprite->gfx, 256);
    dmaCopy(image, touchIndicator->gfx, 64);

    globalScene = createScene();
    globalPhysics = phys_Construct(floattof32(10.0f), floattof32(1.0f));
    globalScene->engine = globalPhysics;

    initRigids();

    // Collider* coll = phys_col_Construct(globalPhysics, intVector(6, 1), VEC2_IDENT);

    //RigidPhysicsObject* obj = createRigidPhysicsObject(RigidbodyCreator(globalPhysics, VEC2_ZERO, ((Vector2) {inttof32(16), inttof32(16)}), inttof32(2), floattof32(0.2f)), sprite);
    // PhysicsObject* obj2 = createPhysicsObject(coll, sprite2);
    //scene_add_rigid_obj(obj, globalScene);
    // scene_add_obj(obj2, globalScene);

    //addSpriteToScene(globalScene, obj->sprite);
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
        RigidPhysicsObject* phy = createRigidPhysicsObject(RigidbodyCreator(globalPhysics, destinationPoint, intVector(16, 16), inttof32(1), floattof32(0.2f)), spr);
        
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
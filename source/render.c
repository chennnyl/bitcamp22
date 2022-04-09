#include <stdlib.h>
#include <stdio.h>
#include <nds/ndstypes.h>
#include <nds/arm9/math.h>

#include "render.h"
#include "exitcodes.h"
#include "util.h"
#include "dynamicarray.h"

#define inttof32(n)   ((n) * (1 << 12))

Scene* createScene(void) {
    Scene* scene = malloc(sizeof(Scene));
    Camera camera;
    camera.x = 0;
    camera.y = 0;
    Physics* physics = phys_Construct(floattof32(9.0f));
    DArray* physicsArray = DArrayInit();
    DArray* rigidPhysicsArray = DArrayInit();
    if(NULL == physicsArray || NULL == rigidPhysicsArray) {
        fprintf(stderr, "[Log] array failed to initialize\n");
        WHERE
        exit(OUT_OF_MEMORY_ERROR);
    }

    scene->camera = camera;
    scene->engine = physics;
    scene->physicsObjects = physicsArray;
    scene->rigidPhysicsObjects = rigidPhysicsArray;

    return scene;
}

PhysicsObject* createPhysicsObject(Collider* collider, Sprite* sprite) {
    PhysicsObject* object = malloc(sizeof(PhysicsObject));
    object->collider = collider;
    object->sprite = sprite;

    return object;
}

RigidPhysicsObject* createRigidPhysicsObject(Rigidbody* rb, Sprite* sprite) {
    RigidPhysicsObject* object = malloc(sizeof(RigidPhysicsObject));
    object->rb = rb;
    object->sprite = sprite;

    return object;
}

void scene_add_obj(PhysicsObject* object, Scene* scene) {
    // TODO: move to DArray
    DArrayAppend(scene->physicsObjects, object);
}

void scene_add_rigid_obj(RigidPhysicsObject* object, Scene* scene) {
    // TODO: move to DArray
    DArrayAppend(scene->rigidPhysicsObjects, object);
}

void renderScene(Scene* scene) {
    for(u32 i = 0; i < scene->physicsObjects->size; i++) {
        // TODO: move to DArray
        PhysicsObject* obj = DArrayGet(scene->physicsObjects, i);
        Point position = worldToScreen(scene->camera, phys_col_getPos(obj->collider));
        obj->sprite->x = position.x;
        obj->sprite->y = position.y;

        if(sprite_on_screen(obj->sprite)) renderSprite(obj->sprite, &oamMain, i);
    }
    // fprintf(stderr, "%ld ", i);
}

Point worldToScreen(Camera camera, Vector2 vec) {
    Point rawPosition = vec_to_point(vec);
    return (Point){
        rawPosition.x - camera.x + 128,
        camera.y - rawPosition.y + 96
    };
}

Vector2 screenToWorld(Camera camera, Point point) {
    return intVector(
        camera.x+point.x-128,
        camera.y+96-point.y
    );
}
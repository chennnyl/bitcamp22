#include <stdlib.h>
#include <stdio.h>

#include "render.h"
#include "exitcodes.h"
#include "util.h"

Scene* createScene(void) {
    Scene* scene = malloc(sizeof(Scene));
    Camera camera;
    camera.x = 0;
    camera.y = 0;
    Physics* physics = phys_Construct();
    DynamicArray* arr = DynamicArrayInit(malloc(sizeof(DynamicArray)), 0);
    if(arr == NULL) {
        fprintf(stderr, "[Log] array failed to initialize\n");
        WHERE
        exit(OUT_OF_MEMORY_ERROR);
    }

    scene->camera = camera;
    scene->engine = physics;
    scene->objects = arr;

    return scene;
}

PhysicsObject* createPhysicsObject(Collider* collider, Sprite* sprite) {
    PhysicsObject* object = malloc(sizeof(PhysicsObject));
    object->collider = collider;
    object->sprite = sprite;

    return object;
}

void scene_add_obj(PhysicsObject* object, Scene* scene) {
    DynamicArraySet(scene->objects, scene->objects->cur_size, object);
}

void renderScene(Scene* scene) {
    for(u32 i = 0; i < scene->objects->cur_size; i++) {
        PhysicsObject* obj = DynamicArrayGet(scene->objects, i);
        Point position = vec_to_point(phys_col_getPos(obj->collider));
        obj->sprite->x = position.x - scene->camera.x + 128;
        obj->sprite->y = scene->camera.y - position.y + 96;

        if(sprite_on_screen(obj->sprite)) renderSprite(obj->sprite, &oamMain, i);
    }
}

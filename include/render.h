#ifndef RENDERH
#define RENDERH

#include "physics.h"
#include "sprite.h"
#include <nds/arm9/dynamicArray.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct PhysicsObject {
    Collider* collider;
    Sprite* sprite;
} PhysicsObject;

typedef Point Camera;

typedef struct Scene {
   Camera camera;
   Physics* engine;
   DynamicArray* objects;
} Scene;

/**
 * @brief Create a Scene object
 * 
 * @return Scene* 
 */
Scene* createScene(void);

/**
 * @brief Create a PhysicsObject object
 * 
 * @param collider The collider of the object
 * @param sprite The sprite to render the PhysicsObject with
 * @return PhysicsObject* 
 */
PhysicsObject* createPhysicsObject(Collider* collider, Sprite* sprite);

/**
 * @brief Add a PhysicsObject to a scene
 * 
 * @param object 
 * @param scene 
 */
void scene_add_obj(PhysicsObject* object, Scene* scene);

/**
 * @brief Render all visible sprites in a scene
 * 
 * @param scene 
 */
void renderScene(Scene* scene);

Point worldToScreen(Camera camera, Vector2 vec);
Vector2 screenToWorld(Camera camera, Point point);

#ifdef __cplusplus
}
#endif

#endif
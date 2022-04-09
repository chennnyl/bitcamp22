#ifndef RENDERH
#define RENDERH

#include "physics.h"
#include "sprite.h"
#include "dynamicarray.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct PhysicsObject {
    Collider* collider;
    Sprite* sprite;
} PhysicsObject;

typedef struct RigidPhysicsObject {
    Rigidbody* rb;
    Sprite* sprite;
} RigidPhysicsObject;

typedef Point Camera;

typedef struct Scene {
   Camera camera;
   Physics* engine;
   DArray* sprites;
   DArray* physicsObjects;
   DArray* rigidPhysicsObjects;
} Scene;

/**
 * @brief Create a Scene object
 * 
 * @return Scene* 
 */
Scene* createScene(void);

void addSpriteToScene(Scene* scene, Sprite* sprite);

/**
 * @brief Create a PhysicsObject object
 * 
 * @param collider The collider of the object
 * @param sprite The sprite to render the PhysicsObject with
 * @return PhysicsObject* 
 */
PhysicsObject* createPhysicsObject(Collider* collider, Sprite* sprite);

RigidPhysicsObject* createRigidPhysicsObject(Rigidbody* collider, Sprite* sprite);

/**
 * @brief Add a PhysicsObject to a scene
 * 
 * @param object 
 * @param scene 
 */
void scene_add_obj(PhysicsObject* object, Scene* scene);

void scene_add_rigid_obj(RigidPhysicsObject* object, Scene* scene);

/**
 * @brief Update physics objects to match sprites
 * 
 * @param scene 
 */
void update_objects(Scene* scene);
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
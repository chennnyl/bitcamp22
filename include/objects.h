#ifndef OBJECTS_H
#define OBJECTS_H

#include "physics.h"
#include "render.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct RigidPhysicsObject {
    Rigidbody* rb;
    Sprite* sprite;
} RigidPhysicsObject;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OBJECTS_H */
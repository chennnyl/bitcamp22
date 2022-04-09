#ifndef VEC_H
#define VEC_H

#include <nds/ndstypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef s32 fixed32;

typedef struct Vector2 {
    fixed32 x;
    fixed32 y;
} Vector2;

Vector2 vec2_add(Vector2 v1, Vector2 v2);
Vector2 vec2_sub(Vector2 v1, Vector2 v2);
Vector2 vec2_scale(Vector2 vec, fixed32 scalar);

Vector2 vec2_norm(Vector2 vec);
fixed32 vec2_sqrmag(Vector2 vec);
fixed32 vec2_mag(Vector2 vec);
fixed32 vec2_dot(Vector2 v1, Vector2 v2);


#ifdef __cplusplus
}
#endif
#endif
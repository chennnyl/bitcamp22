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

typedef struct Point {
    int x;
    int y;
} Point;

#define intVector(x, y) (Vector2){ inttof32(x), inttof32(y) }

/**
 * @brief Convert a fixed point Vector2 to an integer coordinate 
 * 
 * @param vec 
 * @return Point 
 */
Point vec_to_point(Vector2 vec);

/**
 * @brief Add two vectors
 * 
 * @param v1 First vector to add
 * @param v2 Second vector to add
 * @return Vector2 
 */
Vector2 vec2_add(Vector2 v1, Vector2 v2);

/**
 * @brief Subtract one vector from another
 * 
 * @param v1 Vector to subtract from
 * @param v2 Vector to subtract
 * @return Vector2 
 */
Vector2 vec2_sub(Vector2 v1, Vector2 v2);

/**
 * @brief Scale a vector by a scalar
 * 
 * @param vec Vector to scale
 * @param scalar Scalar to scale by
 * @return Vector2 
 */
Vector2 vec2_scale(Vector2 vec, fixed32 scalar);

/**
 * @brief Normalize a vector
 * 
 * @param vec
 * @return Vector2 
 */
Vector2 vec2_norm(Vector2 vec);

/**
 * @brief Get the squared magnitude of a vector (faster)
 * 
 * @param vec 
 * @return fixed32 
 */
fixed32 vec2_sqrmag(Vector2 vec);

/**
 * @brief Get the magnitude of a vector
 * 
 * @param vec 
 * @return fixed32 
 */
fixed32 vec2_mag(Vector2 vec);

/**
 * @brief Find the dot product of two vectors
 * 
 * @param v1 First vector to multiply
 * @param v2 Second vector to multiply
 * @return fixed32
 */
fixed32 vec2_dot(Vector2 v1, Vector2 v2);

/**A vector of <0, 0>*/
#define VEC2_ZERO ((Vector2) { 0, 0 })
/**A vector of <1, 1>*/
#define VEC2_IDENT ((Vector2) { 1, 1 })


#ifdef __cplusplus
}
#endif
#endif
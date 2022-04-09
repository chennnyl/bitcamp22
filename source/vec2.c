/**
 * @file vec2.c
 * @author Gavin Tantleff
 * @brief implements vec2.h, vector functions
 * @version 0.1
 * @date 2022-04-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "vec2.h"

#include <nds/arm9/math.h>

Vector2 vec2_add(Vector2 v1, Vector2 v2) {
    return (Vector2) {v1.x + v2.x, v1.y + v2.y};
};

Vector2 vec2_sub(Vector2 v1, Vector2 v2) {
    return (Vector2) {v1.x - v2.x, v1.y - v2.y};
};

Vector2 vec2_scale(Vector2 vec, fixed24 scalar) {
    return (Vector2) {mulf32(vec.x, scalar), mulf32(vec.y, scalar)};
};

Vector2 vec2_norm(Vector2 vec) {
    fixed24 mag = vec2_mag(vec);
    return (Vector2) {divf32(vec.x, mag), divf32(vec.y, mag)};
};

fixed24 vec2_sqrmag(Vector2 vec) {
    return mulf32(vec.x, vec.x) + multf32(vec.y, vec.y);
};

fixed24 vec2_mag(Vector2 vec) {
    return sqrtf32(mulf32(vec.x, vec.x) + multf32(vec.y, vec.y));
};

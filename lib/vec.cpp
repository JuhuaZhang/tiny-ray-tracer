#include "vec.h"
#include <cmath>

vec3 operator+(const vec3 &v1, const vec3 &v2) {
    return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

vec3 operator-(const vec3 &v1, const vec3 &v2) {
    return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

vec3 operator*(float c, const vec3 &v1) {
    return {v1.x * c, v1.y * c, v1.z * c};
}

float dot(const vec3 &v1, const vec3 &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec3 cross(const vec3 &v1, const vec3 &v2) {
    return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x};
}

vec3 normalize(const vec3 &v) {
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return {v.x / len, v.y / len, v.z / len};
}

float len(const vec3 &v) {
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return len;
}

vec4 operator+(const vec4 &v1, const vec4 &v2) {
    return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

vec4 operator-(const vec4 &v1, const vec4 &v2) {
    return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

float dot(const vec4 &v1, const vec4 &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

vec4 normalize(const vec4 &v) {
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    return {v.x / len, v.y / len, v.z / len, v.w / len};
}

vec3 to_vec3(const vec4 &v) { return {v.x, v.y, v.z}; }

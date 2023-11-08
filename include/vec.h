#ifndef VEC_H
#define VEC_H

class vec3 {
public:
    float x, y, z;

    vec3() : x(0.0f), y(0.0f), z(0.0f) {}

    vec3(float x, float y, float z) : x(x), y(y), z(z) {}
};

vec3 operator+(const vec3 &v1, const vec3 &v2);

vec3 operator-(const vec3 &v1, const vec3 &v2);

vec3 operator*(float c, const vec3 &v2);

float dot(const vec3 &v1, const vec3 &v2);

vec3 cross(const vec3 &v1, const vec3 &v2);

vec3 normalize(const vec3 &v);

float len(const vec3 &v);

// homogeneous coordinates
class vec4 {
public:
    float x, y, z, w; // equal to r, g, b ,a

    vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

    vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    vec4(vec3 v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
};

vec4 operator+(const vec4 &v1, const vec4 &v2);

vec4 operator-(const vec4 &v1, const vec4 &v2);

float dot(const vec4 &v1, const vec4 &v2);

vec4 normalize(const vec4 &v);

vec3 to_vec3(const vec4 &v);

#endif // VEC_H
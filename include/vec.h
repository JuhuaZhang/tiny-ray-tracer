#ifndef VEC_H
#define VEC_H

class Vec3 {
public:
    float x, y, z;

    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}

    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
};

Vec3 operator+(const Vec3& v1, const Vec3& v2);

Vec3 operator-(const Vec3& v1, const Vec3& v2);

Vec3 operator*(float c, const Vec3& v2);

float dot(const Vec3& v1, const Vec3& v2);

Vec3 cross(const Vec3& v1, const Vec3& v2);

Vec3 normalize(const Vec3& v);

float len(const Vec3& v);

// homogeneous coordinates
class Vec4 {
public:
    float x, y, z, w; // equal to r, g, b ,a

    Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

    Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    Vec4(Vec3 v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
};

Vec4 operator+(const Vec4& v1, const Vec4& v2);

Vec4 operator-(const Vec4& v1, const Vec4& v2);

float dot(const Vec4& v1, const Vec4& v2);

Vec4 normalize(const Vec4& v);

Vec3 to_vec3(const Vec4& v);

Vec4 linear_to_srgb(Vec4 color);

Vec4 srgb_to_linear(Vec4 color);

Vec4 color_mapping(Vec4 color);

Vec3 clamp(Vec3 color);

Vec3 expose(Vec3 color, float exposure);

#endif // VEC_H
#include "vec.h"
#include <cmath>

Vec3 operator+(const Vec3& v1, const Vec3& v2) {
    return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

Vec3 operator-(const Vec3& v1, const Vec3& v2) {
    return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

Vec3 operator*(float c, const Vec3& v1) {
    return { v1.x * c, v1.y * c, v1.z * c };
}

float dot(const Vec3& v1, const Vec3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3 cross(const Vec3& v1, const Vec3& v2) {
    return { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x };
}

Vec3 normalize(const Vec3& v) {
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return { v.x / len, v.y / len, v.z / len };
}

float len(const Vec3& v) {
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return len;
}

Vec4 operator+(const Vec4& v1, const Vec4& v2) {
    return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
}

Vec4 operator-(const Vec4& v1, const Vec4& v2) {
    return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w };
}

float dot(const Vec4& v1, const Vec4& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

Vec4 normalize(const Vec4& v) {
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    return { v.x / len, v.y / len, v.z / len, v.w / len };
}

Vec3 to_vec3(const Vec4& v) { return { v.x, v.y, v.z }; }

Vec4 linear_to_srgb(Vec4 color) {
    Vec4 srgb_color = color;
    srgb_color.x = (srgb_color.x <= 0.0031308f)
        ? (12.92f * srgb_color.x)
        : (1.055f * pow(srgb_color.x, 1.0f / 2.4f) - 0.055f);
    srgb_color.y = (srgb_color.y <= 0.0031308f)
        ? (12.92f * srgb_color.y)
        : (1.055f * pow(srgb_color.y, 1.0f / 2.4f) - 0.055f);
    srgb_color.z = (srgb_color.z <= 0.0031308f)
        ? (12.92f * srgb_color.z)
        : (1.055f * pow(srgb_color.z, 1.0f / 2.4f) - 0.055f);
    return srgb_color;
}

Vec4 srgb_to_linear(Vec4 color) {
    // for each channel, if value < 0, convert to 0, if value > 1, convert to 1
    color.x = (color.x < 0) ? 0 : color.x;
    color.x = (color.x > 1) ? 1 : color.x;
    color.y = (color.y < 0) ? 0 : color.y;
    color.y = (color.y > 1) ? 1 : color.y;
    color.z = (color.z < 0) ? 0 : color.z;
    color.z = (color.z > 1) ? 1 : color.z;

    // apply srgb to linear conversion
    color.x = (color.x <= 0.04045f) ? (color.x / 12.92f)
        : pow((color.x + 0.055f) / 1.055f, 2.4f);
    color.y = (color.y <= 0.04045f) ? (color.y / 12.92f)
        : pow((color.y + 0.055f) / 1.055f, 2.4f);
    color.z = (color.z <= 0.04045f) ? (color.z / 12.92f)
        : pow((color.z + 0.055f) / 1.055f, 2.4f);

    // convert (0-1) to (0-255)
    color.x = color.x * 255.0f; // + 0.5f;
    color.y = color.y * 255.0f; // + 0.5f;
    color.z = color.z * 255.0f; // + 0.5f;

    return color;
}

Vec4 color_mapping(Vec4 color) {
    // convert (0-1) to (0-255)
    color.x = color.x * 255.0f; // + 0.5f;
    color.y = color.y * 255.0f; // + 0.5f;
    color.z = color.z * 255.0f; // + 0.5f;

    return color;
}

Vec3 clamp(Vec3 color) {
    color.x = (color.x < 0) ? 0 : color.x;
    color.x = (color.x > 1) ? 1 : color.x;
    color.y = (color.y < 0) ? 0 : color.y;
    color.y = (color.y > 1) ? 1 : color.y;
    color.z = (color.z < 0) ? 0 : color.z;
    color.z = (color.z > 1) ? 1 : color.z;

    return color;
}

Vec3 expose(Vec3 color, float exposure) {
    color.x = 1 - exp(-color.x * exposure);
    color.y = 1 - exp(-color.y * exposure);
    color.z = 1 - exp(-color.z * exposure);

    return color;
}

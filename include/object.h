#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>
#include "vec.h"

// pixel to generate image
class Pixel
{
public:
    int x, y;
    float r, g, b, a;

    Pixel() : x(0), y(0), r(0), g(0), b(0), a(0) {}

    Pixel(int x, int y, float r, float g, float b, float a) : x(x), y(y), r(r), g(g), b(b), a(a) {}

    Pixel(int x, int y, Vec4 color) : x(x), y(y), r(color.x), g(color.y), b(color.z), a(color.w) {}
};

// contain image information and pixels
class Image
{
public:
    int w;
    int h;

    bool is_expose = false;
    float expose = -1.0f;

    bool is_shiny = false;
    float shininess = 1.0f;
    bool is_transparent = false;
    float transparency = 0.0f;

    std::string image_name;
    std::vector<Pixel> pixels;

    Image() : w(int(0.0f)), h(int(0.0f)), pixels() {}

    Image(float w, float h, std::string image_name) : w(int(w)), h(int(h)), image_name(std::move(image_name)), pixels() {}
};

class Object
{
public:
    Vec3 location;
    Vec4 color;

    bool is_shiny = false;
    float shininess = 1.0f;
    bool is_transparent = false;
    float transparency = 0.0f;

    Object() : location(Vec3()), color(Vec4()) {}

    Object(Vec3 location, Vec4 color) : location(location), color(color) {}

    virtual float intersection(Vec3 c, Vec3 ray) = 0;

    virtual Vec3 compute_normal(Vec3 p) = 0;

    virtual ~Object() = default;
};

class Sphere : public Object
{
public:
    float radius;

    Sphere() : Object(), radius(0.0f) {}

    Sphere(Vec3 location, Vec4 color, float radius) : Object(location, color), radius(radius) {}

    float intersection(Vec3 c, Vec3 ray) override;

    Vec3 compute_normal(Vec3 p) override;
};

class Plane : public Object
{
public:
    Vec4 params; // a, b, c, d

    Plane() : Object(), params(Vec4()) {}

    Plane(Vec4 params, Vec4 color) : Object(Vec3(), color), params(params) {}

    float intersection(Vec3 c, Vec3 ray) override;

    Vec3 compute_normal(Vec3 p) override;
};

class Triangle : public Object
{
public:
    Vec3 v1, v2, v3;

    Triangle() : Object(), v1(Vec3()), v2(Vec3()), v3(Vec3()) {}

    Triangle(Vec3 v1, Vec3 v2, Vec3 v3, Vec4 color) : Object(Vec3(), color), v1(v1), v2(v2), v3(v3) {}

    float intersection(Vec3 c, Vec3 ray) override;

    Vec3 compute_normal(Vec3 p) override;
};

class Raytracer
{
public:
    Vec3 eye;
    Vec3 forward;
    Vec3 right;
    Vec3 up;
    int n; // for aa

    Raytracer() : eye(Vec3(0, 0, 0)), forward(Vec3(0, 0, -1)), right(Vec3(1, 0, 0)), up(Vec3(0, 1, 0)), n(1) {}
};
#endif // OBJECT_H
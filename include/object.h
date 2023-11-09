#ifndef OBJECT_H
#define OBJECT_H

#include "vec.h"

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

#endif // OBJECT_H
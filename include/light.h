#ifndef LIGHT_H
#define LIGHT_H

#include "vec.h"

class Light
{
public:
    Vec4 color;

    Light() : color(Vec4()) {}

    Light(Vec4 color) : color(color) {}

    virtual Vec3 get_light_dir(Vec3 p) = 0;

    virtual Vec3 compute_diffuse_color(Vec3 p, Vec3 n, Vec4 color) = 0;

    virtual ~Light() = default;
};

class Sun : public Light
{
public:
    Vec3 direction;

    Sun() : Light(), direction(Vec3()) {}

    Sun(Vec3 location, Vec4 color) : Light(color), direction(location) {}

    Vec3 get_light_dir(Vec3 p) override;

    Vec3 compute_diffuse_color(Vec3 p, Vec3 n, Vec4 color) override;
};

class Bulb : public Light
{
public:
    Vec3 location;

    Bulb() : Light(), location(Vec3()) {}

    Bulb(Vec3 location, Vec4 color) : Light(color), location(location) {}

    Vec3 get_light_dir(Vec3 p) override;

    Vec3 compute_diffuse_color(Vec3 p, Vec3 n, Vec4 color) override;
};

#endif // LIGHT_H
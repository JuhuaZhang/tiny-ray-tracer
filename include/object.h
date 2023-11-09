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

    Pixel(int x, int y, vec4 color) : x(x), y(y), r(color.x), g(color.y), b(color.z), a(color.w) {}
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
    vec3 location;
    vec4 color;

    bool is_shiny = false;
    float shininess = 1.0f;
    bool is_transparent = false;
    float transparency = 0.0f;

    Object() : location(vec3()), color(vec4()) {}

    Object(vec3 location, vec4 color) : location(location), color(color) {}

    virtual float intersection(vec3 c, vec3 ray) = 0;

    virtual vec3 compute_normal(vec3 p) = 0;

    virtual ~Object() = default;
};

class Sphere : public Object
{
public:
    float radius;

    Sphere() : Object(), radius(0.0f) {}

    Sphere(vec3 location, vec4 color, float radius) : Object(location, color), radius(radius) {}

    float intersection(vec3 c, vec3 ray) override;

    vec3 compute_normal(vec3 p) override;
};

class Plane : public Object
{
public:
    vec4 params; // a, b, c, d

    Plane() : Object(), params(vec4()) {}

    Plane(vec4 params, vec4 color) : Object(vec3(), color), params(params) {}

    float intersection(vec3 c, vec3 ray) override;

    vec3 compute_normal(vec3 p) override;
};

class Triangle : public Object
{
public:
    vec3 v1, v2, v3;

    Triangle() : Object(), v1(vec3()), v2(vec3()), v3(vec3()) {}

    Triangle(vec3 v1, vec3 v2, vec3 v3, vec4 color) : Object(vec3(), color), v1(v1), v2(v2), v3(v3) {}

    float intersection(vec3 c, vec3 ray) override;

    vec3 compute_normal(vec3 p) override;
};

class Light
{
public:
    vec4 color;

    Light() : color(vec4()) {}

    Light(vec4 color) : color(color) {}

    virtual vec3 get_light_dir(vec3 p) = 0;

    virtual vec3 compute_diffuse_color(vec3 p, vec3 n, vec4 color) = 0;

    virtual ~Light() = default;
};

class Sun : public Light
{
public:
    vec3 direction;

    Sun() : Light(), direction(vec3()) {}

    Sun(vec3 location, vec4 color) : Light(color), direction(location) {}

    vec3 get_light_dir(vec3 p) override;

    vec3 compute_diffuse_color(vec3 p, vec3 n, vec4 color) override;
};

class Bulb : public Light
{
public:
    vec3 location;

    Bulb() : Light(), location(vec3()) {}

    Bulb(vec3 location, vec4 color) : Light(color), location(location) {}

    vec3 get_light_dir(vec3 p) override;

    vec3 compute_diffuse_color(vec3 p, vec3 n, vec4 color) override;
};

class ray_tracer
{
public:
    vec3 eye;
    vec3 forward;
    vec3 right;
    vec3 up;
    int n; // for aa

    ray_tracer() : eye(vec3(0, 0, 0)), forward(vec3(0, 0, -1)), right(vec3(1, 0, 0)), up(vec3(0, 1, 0)), n(1) {}
};

// release memory allocated by parse_content()
void cleanup_objects(std::vector<Object*>& objects, std::vector<Sun*>& lights, std::vector<Bulb*>& bulbs);

vec4 compute_color(vec3 r0, vec3 ray, float n_t, Object* object, std::vector<Light*>& lights, std::vector<Object*>& objects, Image& img, int depth);

vec3 trace_ray(vec3 r0, vec3 ray, Object* object, std::vector<Light*>& lights, std::vector<Object*>& objects, Image& img, int depth);
#endif // OBJECT_H
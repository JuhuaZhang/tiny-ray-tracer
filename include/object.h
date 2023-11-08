#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include "vec.h"

// pixel to generate image
class pixel {
public:
    int x, y;
    float r, g, b, a;

    pixel() : x(0), y(0), r(0), g(0), b(0), a(0) {}

    pixel(int x, int y, float r, float g, float b, float a) : x(x), y(y), r(r), g(g), b(b), a(a) {}

    pixel(int x, int y, vec4 color) : x(x), y(y), r(color.x), g(color.y), b(color.z), a(color.w) {}
};

// contain image information and pixels
class image {
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
    std::vector<pixel> pixels;

    image() : w(int(0.0f)), h(int(0.0f)), pixels() {}

    image(float w, float h, std::string image_name) : w(int(w)), h(int(h)),
                                                 image_name(
                                                         std::move(image_name)),
                                                 pixels() {}
};

// object of the file
class obj {
public:
    vec3 location;
    vec4 color;

    bool is_shiny = false;
    float shininess = 1.0f;
    bool is_transparent = false;
    float transparency = 0.0f;

    obj() : location(vec3()), color(vec4()) {}

    obj(vec3 location, vec4 color) : location(location), color(color) {}

    virtual float intersection(vec3 c, vec3 ray) = 0;

    virtual ~obj() = default;
};

class sphere : public obj {
public:
    float radius;

    sphere() : obj(), radius(0.0f) {}

    sphere(vec3 location, vec4 color, float radius) : obj(location, color), radius(radius) {}

    float intersection(vec3 c, vec3 ray) override;
};

class plane : public obj {
public:
    vec4 params; // a, b, c, d

    plane() : obj(), params(vec4()) {}

    plane(vec4 params, vec4 color) : obj(vec3(), color), params(params) {}

    float intersection(vec3 c, vec3 ray) override;
};

class triangle : public obj {
public:
    vec3 v1, v2, v3;

    triangle() : obj(), v1(vec3()), v2(vec3()), v3(vec3()) {}

    triangle(vec3 v1, vec3 v2, vec3 v3, vec4 color) : obj(vec3(), color), v1(v1), v2(v2), v3(v3) {}

    float intersection(vec3 c, vec3 ray) override;
};

class lgt {
public:
    vec3 direction;
    vec4 color;

    lgt() : direction(vec3()), color(vec4()) {}

    lgt(vec3 location, vec4 color) : direction(location), color(color) {}
};

class bulb {
public:
    vec3 location;
    vec4 color;

    bulb() : location(vec3()), color(vec4()) {}

    bulb(vec3 location, vec4 color) : location(location), color(color) {}
};

class ray_tracer {
public:
    vec3 eye;
    vec3 forward;
    vec3 right;
    vec3 up;
    int n; // for aa

    ray_tracer() : eye(vec3(0, 0, 0)), forward(vec3(0, 0, -1)),
                   right(vec3(1, 0, 0)), up(vec3(0, 1, 0)), n(1) {}
};


// release memory allocated by parse_content()
void cleanup_objects(std::vector<obj *> &objects, std::vector<lgt *> &lights,
                     std::vector<bulb *> &bulbs);

vec4 compute_color(vec3 r0, vec3 ray, float n_t, obj *object,
                   std::vector<lgt *> &lights, std::vector<bulb *> &bulbs,
                   std::vector<obj *> &objects, image &img, int depth);

vec4 linear_to_srgb(vec4 color);

vec4 srgb_to_linear(vec4 color);

vec4 color_mapping(vec4 color);

vec3 trace_ray(vec3 r0, vec3 ray, obj *object, std::vector<lgt *> &lights,
               std::vector<bulb *> &bulbs,
               std::vector<obj *> &objects, image &img, int depth);
#endif // OBJECT_H
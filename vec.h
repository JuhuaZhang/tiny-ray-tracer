#ifndef VEC_H
#define VEC_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <limits>
#include "lodepng.h"

using namespace std;

// position vector
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

// color and homogeneous coordinates
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

// not sure
vec3 to_vec3(const vec4 &v);

// read the content of the input file
vector<vector<string>> read_file(const string &fileName);

// pixel to generate image
class pixel {
public:
    int x, y;
    float r, g, b, a;

    pixel() : x(0), y(0), r(0), g(0), b(0), a(0) {}

    pixel(int x, int y, float r, float g, float b, float a) : x(x), y(y), r(r),
                                                              g(g), b(b),
                                                              a(a) {}

    pixel(int x, int y, vec4 color) : x(x), y(y), r(color.x), g(color.y),
                                      b(color.z), a(color.w) {}
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

    string image_name;
    vector<pixel> pixels;

    image() : w(int(0.0f)), h(int(0.0f)), pixels() {}

    image(float w, float h, string image_name) : w(int(w)), h(int(h)),
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

    virtual ~obj() = default; // make it polymorphic
};

class sphere : public obj {
public:
    float radius;

    sphere() : obj(), radius(0.0f) {}

    sphere(vec3 location, vec4 color, float radius) : obj(location, color),
                                                      radius(radius) {}
};

class plane : public obj {
public:
    vec4 params; // a, b, c, d

    plane() : obj(), params(vec4()) {}

    plane(vec4 params, vec4 color) : obj(vec3(), color), params(params) {}
};

class triangle : public obj {
public:
    vec3 v1, v2, v3;

    triangle() : obj(), v1(vec3()), v2(vec3()), v3(vec3()) {}

    triangle(vec3 v1, vec3 v2, vec3 v3, vec4 color) : obj(vec3(), color),
                                                      v1(v1), v2(v2), v3(v3) {}
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

// parse the content, covert to image, objects and so on
void
parse_content(vector<vector<string>> &contents, vector<obj *> &objects,
              vector<lgt *> &lights, vector<bulb *> &bulbs,
              image &img, vec4 &cur_color, ray_tracer &tracer,
              vector<vec3> &vertices);

// release memory allocated by parse_content()
void cleanup_objects(vector<obj *> &objects, vector<lgt *> &lights,
                     vector<bulb *> &bulbs);

void generate_image(image &img);

// calculate the intersection point of ray and object, return t
float intersection(vec3 c, vec3 ray, obj *object);

vec4 compute_color(vec3 r0, vec3 ray, float n_t, obj *object,
                   vector<lgt *> &lights, vector<bulb *> &bulbs,
                   vector<obj *> &objects, image &img, int depth);

vec4 linear_to_srgb(vec4 color);

vec4 srgb_to_linear(vec4 color);

vec4 color_mapping(vec4 color);

vec3 trace_ray(vec3 r0, vec3 ray, obj *object, vector<lgt *> &lights,
               vector<bulb *> &bulbs,
               vector<obj *> &objects, image &img, int depth);

#endif // VEC_H
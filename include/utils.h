#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

#include "vec.h"
#include "object.h"
#include "light.h"

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

// raytracing config
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

// read the content of the input file
std::vector<std::vector<std::string>> read_file(const std::string& fileName);

// parse the content, covert to image, objects and so on
void
parse_content(std::vector<std::vector<std::string>>& contents,
     Image& img, Raytracer& tracer, std::vector<Object*>& objects, std::vector<Light*>& lightings);

void generate_image(Image& img);

// release memory allocated by parse_content()
void cleanup_objects(std::vector<Object*>& objects, std::vector<Light*>& lights);

Vec4 compute_color(Vec3 r0, Vec3 ray, float n_t, Object* object, std::vector<Light*>& lights, std::vector<Object*>& objects, Image& img, int depth);

Vec3 trace_ray(Vec3 r0, Vec3 ray, Object* object, std::vector<Light*>& lights, std::vector<Object*>& objects, Image& img, int depth);

#endif // UTILS_H
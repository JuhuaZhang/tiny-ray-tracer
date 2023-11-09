#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

#include "vec.h"
#include "object.h"
#include "light.h"

// read the content of the input file
std::vector<std::vector<std::string>> read_file(const std::string& fileName);

// parse the content, covert to image, objects and so on
void
parse_content(std::vector<std::vector<std::string>>& contents,
    std::vector<Object*>& objects, Image& img, Raytracer& tracer, std::vector<Light*>& lightings);

void generate_image(Image& img);

// release memory allocated by parse_content()
void cleanup_objects(std::vector<Object*>& objects, std::vector<Sun*>& lights, std::vector<Bulb*>& bulbs);

Vec4 compute_color(Vec3 r0, Vec3 ray, float n_t, Object* object, std::vector<Light*>& lights, std::vector<Object*>& objects, Image& img, int depth);

Vec3 trace_ray(Vec3 r0, Vec3 ray, Object* object, std::vector<Light*>& lights, std::vector<Object*>& objects, Image& img, int depth);

#endif // UTILS_H
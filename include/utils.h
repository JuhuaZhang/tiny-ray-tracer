#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

#include "vec.h"
#include "object.h"

// read the content of the input file
std::vector<std::vector<std::string>> read_file(const std::string &fileName);

// parse the content, covert to image, objects and so on
void
parse_content(std::vector<std::vector<std::string>> &contents, std::vector<obj *> &objects,
              std::vector<lgt *> &lights, std::vector<bulb *> &bulbs,
              image &img, vec4 &cur_color, ray_tracer &tracer,
              std::vector<vec3> &vertices);

void generate_image(image &img);

#endif // UTILS_H
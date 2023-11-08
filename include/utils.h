#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include "vec.h"
#include "object.h"

// using namespace std;

// read the content of the input file
vector<vector<string>> read_file(const string &fileName);

// parse the content, covert to image, objects and so on
void
parse_content(vector<vector<string>> &contents, vector<obj *> &objects,
              vector<lgt *> &lights, vector<bulb *> &bulbs,
              image &img, vec4 &cur_color, ray_tracer &tracer,
              vector<vec3> &vertices);

void generate_image(image &img);

#endif // UTILS_H
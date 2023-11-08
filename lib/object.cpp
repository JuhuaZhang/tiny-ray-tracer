#include <iostream>
#include <utility>
#include <algorithm>
#include <limits>
#include <cmath>

#include "object.h"

using namespace std;

void cleanup_objects(vector<obj *> &objects, vector<lgt *> &lights,
                     vector<bulb *> &bulbs) {
    for (obj *object: objects) {
        delete object;
    }
    objects.clear();

    for (lgt *light: lights) {
        delete light;
    }
    lights.clear();

    for (bulb *bulb: bulbs) {
        delete bulb;
    }
}

float sphere::intersection(vec3 r0, vec3 ray){

        vec3 rd = normalize(ray);
        bool inside = (len(this->location - r0) < this->radius);
        float tc = dot((this->location - r0), rd) / len(rd);

        if ((!inside) && (tc < 0)) { return -1; } // no intersection

        float d_square = len(r0 + tc * rd - this->location) * len(r0 + tc * rd - this->location);
        float delta = this->radius * this->radius - d_square;

        if ((!inside) && (delta < 0)) { return -1; } // no intersection

        float t_offset = sqrt(delta) / len(rd);

        return inside ? tc+t_offset : tc-t_offset;
}

vec3 sphere::compute_normal(vec3 p){
    return normalize(p - this->location);
}

float plane::intersection(vec3 r0, vec3 ray){
    
    vec3 n = normalize(vec3(this->params.x, this->params.y, this->params.z));
    vec3 p;
    // avoid division by zero
    if (this->params.x != 0)
        p = vec3(-1.0f * this->params.w / this->params.x, 0, 0);
    else if (this->params.y != 0)
        p = vec3(0, -1.0f * this->params.w / this->params.y, 0);
    else if (this->params.z != 0)
        p = vec3(0, 0, -1.0f * this->params.w / this->params.z);

    vec3 rd = normalize(ray);
    float t = dot((p - r0), n) / dot(rd, n);

    return t > 0 ? t : -1;
}

vec3 plane::compute_normal(vec3 p){
    return normalize(vec3(this->params.x, this->params.y, this->params.z));
}

float triangle::intersection(vec3 r0, vec3 ray){
    // Möller-Trumbore algorithm
    vec3 edge1 = this->v2 - this->v1;
    vec3 edge2 = this->v3 - this->v1;
    vec3 pvec = cross(normalize(ray), edge2);
    float det = dot(edge1, pvec);

    if (fabs(det) < 1e-8) { return -1; }

    float inv_det = 1.0f / det;
    vec3 tvec = r0 - this->v1;
    float u = dot(tvec, pvec) * inv_det;

    if (u < 0 || u > 1) { return -1; }

    vec3 qvec = cross(tvec, edge1);
    float v = dot(normalize(ray), qvec) * inv_det;

    if (v < 0 || u + v > 1) { return -1; }

    float t = dot(edge2, qvec) * inv_det;

    return t > 0 ? t : -1;
}

vec3 triangle::compute_normal(vec3 p){
    return normalize(cross(this->v1 - this->v2, this->v1 - this->v3));
}

vec3 lgt::get_light_dir(vec3 p){
    return this->direction;
}

vec3 bulb::get_light_dir(vec3 p){
    return normalize(this->location - p);
}

// used after computing the intersection
vec4 linear_to_srgb(vec4 color) {
    vec4 srgb_color = color;
    srgb_color.x = (srgb_color.x <= 0.0031308f)
                   ? (12.92f * srgb_color.x)
                   : (1.055f * pow(srgb_color.x, 1.0f / 2.4f) - 0.055f);
    srgb_color.y = (srgb_color.y <= 0.0031308f)
                   ? (12.92f * srgb_color.y)
                   : (1.055f * pow(srgb_color.y, 1.0f / 2.4f) - 0.055f);
    srgb_color.z = (srgb_color.z <= 0.0031308f)
                   ? (12.92f * srgb_color.z)
                   : (1.055f * pow(srgb_color.z, 1.0f / 2.4f) - 0.055f);
    return srgb_color;
}

// used when reading the file
vec4 srgb_to_linear(vec4 color) {
    // for each channel, if value < 0, convert to 0, if value > 1, convert to 1
    color.x = (color.x < 0) ? 0 : color.x;
    color.x = (color.x > 1) ? 1 : color.x;
    color.y = (color.y < 0) ? 0 : color.y;
    color.y = (color.y > 1) ? 1 : color.y;
    color.z = (color.z < 0) ? 0 : color.z;
    color.z = (color.z > 1) ? 1 : color.z;

    // apply srgb to linear conversion
    color.x = (color.x <= 0.04045f) ? (color.x / 12.92f)
                                    : pow((color.x + 0.055f) / 1.055f, 2.4f);
    color.y = (color.y <= 0.04045f) ? (color.y / 12.92f)
                                    : pow((color.y + 0.055f) / 1.055f, 2.4f);
    color.z = (color.z <= 0.04045f) ? (color.z / 12.92f)
                                    : pow((color.z + 0.055f) / 1.055f, 2.4f);

    // convert (0-1) to (0-255)
    color.x = color.x * 255.0f; // + 0.5f;
    color.y = color.y * 255.0f; // + 0.5f;
    color.z = color.z * 255.0f; // + 0.5f;

    return color;
}

vec4 color_mapping(vec4 color) {
    // convert (0-1) to (0-255)
    color.x = color.x * 255.0f; // + 0.5f;
    color.y = color.y * 255.0f; // + 0.5f;
    color.z = color.z * 255.0f; // + 0.5f;

    return color;
}

vec3 clamp(vec3 color) {
    color.x = (color.x < 0) ? 0 : color.x;
    color.x = (color.x > 1) ? 1 : color.x;
    color.y = (color.y < 0) ? 0 : color.y;
    color.y = (color.y > 1) ? 1 : color.y;
    color.z = (color.z < 0) ? 0 : color.z;
    color.z = (color.z > 1) ? 1 : color.z;

    return color;
}

float clamp(float v) {
    v = (v < 0) ? 0 : v;
    v = (v > 1) ? 1 : v;
    return v;
}

vec3 expose(vec3 color, float exposure) {
    color.x = 1 - exp(-color.x * exposure);
    color.y = 1 - exp(-color.y * exposure);
    color.z = 1 - exp(-color.z * exposure);

    return color;
}

bool check_occlusion(vec3 p, obj* self_object, vector<obj *> &objects, light * lght){
    for(auto *object: objects){
        if(object != self_object){
            if (object->intersection(p, lght->get_light_dir(p)) != -1)
                return true;
        }
    }
    return false;
}

vec4 compute_color(vec3 r0, vec3 ray, float n_t, obj *object,
                   vector<lgt *> &lights, vector<bulb *> &bulbs,
                   vector<obj *> &objects, image &img, int depth) {
    vec3 p = r0 + n_t * normalize(ray);

    vector<vec3> colors;
    vec3 n = object->compute_normal(p);

    if (dot(n, ray) > 0)
        n = -1.0f * n;

    // check if the point is in shadow
    for (auto light: lights) {
        if (!check_occlusion(p, object, objects, light)) {
            vec3 x = normalize(light->direction); // incoming light direction
            vec3 cur_color = vec3((object->color.x * light->color.x),
                                  (object->color.y * light->color.y),
                                  (object->color.z * light->color.z));
            cur_color = max(dot(n, x), 0.0f) * cur_color;
            colors.push_back(cur_color);
        }
    }

    for (auto bulb: bulbs) {
        if (!check_occlusion(p, object, objects, bulb)) {
            vec3 x = bulb->get_light_dir(p);
            vec3 cur_color = vec3((object->color.x * bulb->color.x),
                                  (object->color.y * bulb->color.y),
                                  (object->color.z * bulb->color.z));
            cur_color = max(dot(n, x), 0.0f) * cur_color;
            vec3 dis = bulb->location - p;
            float d = sqrt(dis.x * dis.x + dis.y * dis.y + dis.z * dis.z);
            cur_color = (1.0f / (d * d)) * cur_color;
            colors.push_back(cur_color);
        }
    }

    // diffuse light
    vec3 diffuse_color = vec3(0, 0, 0);
    for (auto color: colors) {
        diffuse_color = diffuse_color + color;
        if (img.is_expose)
            diffuse_color = expose(diffuse_color, img.expose);
        diffuse_color = clamp(diffuse_color);
    }
    vec3 specular_color = vec3(0, 0, 0);
    if (object->is_shiny) {
        vec3 reflect_dir = normalize(ray) - 2.0f * dot(n, normalize(ray)) * n;
        reflect_dir = normalize(reflect_dir);
        specular_color = object->shininess *
                         trace_ray(p, reflect_dir, object, lights, bulbs,
                                   objects, img, depth + 1);
        specular_color = clamp(specular_color);
    }

    vec4 final_color = {clamp(diffuse_color + specular_color), object->color.w};

    return final_color;
}

vec3 trace_ray(vec3 r0, vec3 ray, obj *object, vector<lgt *> &lights,
               vector<bulb *> &bulbs,
               vector<obj *> &objects, image &img, int depth) {
    if (depth > 4) {
        return {0, 0, 0};
    }

    // Find the nearest intersection point and the corresponding object
    float min_t = std::numeric_limits<float>::max();
    obj *nearest_object = nullptr;

    for (obj *obj: objects) {
        if (obj != object) {
            float temp = obj->intersection(r0, ray);
            if (temp > 0.0f && temp < min_t) {
                min_t = temp;
                nearest_object = obj;
            }
        }
    }

    if (nearest_object != nullptr) {
        return to_vec3(
                compute_color(r0, ray, min_t, nearest_object, lights,
                              bulbs, objects, img, depth));
    }
    else {
        return {0, 0, 0};
    }
}
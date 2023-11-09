#include <iostream>
#include <utility>
#include <algorithm>
#include <limits>
#include <cmath>

#include "object.h"

using namespace std;

void cleanup_objects(vector<Object*>& objects, vector<Sun*>& lights, vector<Bulb*>& bulbs) {
    for (Object* object : objects) {
        delete object;
    }
    objects.clear();

    for (Sun* light : lights) {
        delete light;
    }
    lights.clear();

    for (Bulb* bulb : bulbs) {
        delete bulb;
    }
}

float Sphere::intersection(vec3 r0, vec3 ray) {

    vec3 rd = normalize(ray);
    bool inside = (len(this->location - r0) < this->radius);
    float tc = dot((this->location - r0), rd) / len(rd);

    if ((!inside) && (tc < 0)) { return -1; } // no intersection

    float d_square = len(r0 + tc * rd - this->location) * len(r0 + tc * rd - this->location);
    float delta = this->radius * this->radius - d_square;

    if ((!inside) && (delta < 0)) { return -1; } // no intersection

    float t_offset = sqrt(delta) / len(rd);

    return inside ? tc + t_offset : tc - t_offset;
}

vec3 Sphere::compute_normal(vec3 p) {
    return normalize(p - this->location);
}

float Plane::intersection(vec3 r0, vec3 ray) {

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

vec3 Plane::compute_normal(vec3 p) {
    return normalize(vec3(this->params.x, this->params.y, this->params.z));
}

float Triangle::intersection(vec3 r0, vec3 ray) {
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

vec3 Triangle::compute_normal(vec3 p) {
    return normalize(cross(this->v1 - this->v2, this->v1 - this->v3));
}

vec3 Sun::get_light_dir(vec3 p) {
    return  normalize(this->direction);
}

vec3 Sun::compute_diffuse_color(vec3 p, vec3 n, vec4 color) {
    vec3 light_dir = this->get_light_dir(p);
    vec3 diffuse_color = vec3(
        (color.x * this->color.x),
        (color.y * this->color.y),
        (color.z * this->color.z));
    diffuse_color = max(dot(n, light_dir), 0.0f) * diffuse_color;

    return diffuse_color;
}

vec3 Bulb::get_light_dir(vec3 p) {
    return normalize(this->location - p);
}

vec3 Bulb::compute_diffuse_color(vec3 p, vec3 n, vec4 color) {
    vec3 light_dir = this->get_light_dir(p);
    vec3 diffuse_color = vec3(
        (color.x * this->color.x),
        (color.y * this->color.y),
        (color.z * this->color.z));
    diffuse_color = max(dot(n, light_dir), 0.0f) * diffuse_color;
    vec3 dis = this->location - p;
    float d = sqrt(dis.x * dis.x + dis.y * dis.y + dis.z * dis.z);
    diffuse_color = (1.0f / (d * d)) * diffuse_color;

    return diffuse_color;
}

bool check_occlusion(vec3 p, Object* self_object, vector<Object*>& objects, Light* lght) {
    for (auto* object : objects) {
        if (object != self_object) {
            if (object->intersection(p, lght->get_light_dir(p)) != -1)
                return true;
        }
    }
    return false;
}

vec4 compute_color(vec3 r0, vec3 ray, float n_t, Object* object, vector<Light*>& lights, vector<Object*>& objects, Image& img, int depth) {
    vec3 p = r0 + n_t * normalize(ray);
    vector<vec3> colors;

    vec3 n = object->compute_normal(p);
    if (dot(n, ray) > 0) n = -1.0f * n;

    // check if the point is in shadow
    for (auto light : lights) {
        if (!check_occlusion(p, object, objects, light)) {
            colors.push_back(light->compute_diffuse_color(p, n, object->color));
        }
    }

    // diffuse light
    vec3 diffuse_color = vec3(0, 0, 0);
    for (auto color : colors) {
        diffuse_color = diffuse_color + color;
        if (img.is_expose)
            diffuse_color = expose(diffuse_color, img.expose);
        diffuse_color = clamp(diffuse_color);
    }
    vec3 specular_color = vec3(0, 0, 0);
    if (object->is_shiny) {
        vec3 reflect_dir = normalize(ray) - 2.0f * dot(n, normalize(ray)) * n;
        reflect_dir = normalize(reflect_dir);
        specular_color = object->shininess * trace_ray(p, reflect_dir, object, lights, objects, img, depth + 1);
        specular_color = clamp(specular_color);
    }
    vec4 final_color = { clamp(diffuse_color + specular_color), object->color.w };
    return final_color;
}

vec3 trace_ray(vec3 r0, vec3 ray, Object* object, vector<Light*>& lights,
    vector<Object*>& objects, Image& img, int depth) {
    if (depth > 4) {
        return { 0, 0, 0 };
    }

    // Find the nearest intersection point and the corresponding object
    float min_t = std::numeric_limits<float>::max();
    Object* nearest_object = nullptr;

    for (Object* obj : objects) {
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
            compute_color(r0, ray, min_t, nearest_object, lights, objects, img, depth));
    }
    else {
        return { 0, 0, 0 };
    }
}
#include "light.h"
#include <cmath>

Vec3 Sun::get_light_dir(Vec3 p) {
    return  normalize(this->direction);
}

Vec3 Sun::compute_diffuse_color(Vec3 p, Vec3 n, Vec4 color) {
    Vec3 light_dir = this->get_light_dir(p);
    Vec3 diffuse_color = Vec3(
        (color.x * this->color.x),
        (color.y * this->color.y),
        (color.z * this->color.z));
    diffuse_color = fmax(dot(n, light_dir), 0.0f) * diffuse_color;

    return diffuse_color;
}

Vec3 Bulb::get_light_dir(Vec3 p) {
    return normalize(this->location - p);
}

Vec3 Bulb::compute_diffuse_color(Vec3 p, Vec3 n, Vec4 color) {
    Vec3 light_dir = this->get_light_dir(p);
    Vec3 diffuse_color = Vec3(
        (color.x * this->color.x),
        (color.y * this->color.y),
        (color.z * this->color.z));
    diffuse_color = fmax(dot(n, light_dir), 0.0f) * diffuse_color;
    Vec3 dis = this->location - p;
    float d = sqrt(dis.x * dis.x + dis.y * dis.y + dis.z * dis.z);
    diffuse_color = (1.0f / (d * d)) * diffuse_color;

    return diffuse_color;
}
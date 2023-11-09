#include <cmath>
#include "object.h"

using namespace std;

float Sphere::intersection(Vec3 r0, Vec3 ray) {

    Vec3 rd = normalize(ray);
    bool inside = (len(this->location - r0) < this->radius);
    float tc = dot((this->location - r0), rd) / len(rd);

    if ((!inside) && (tc < 0)) { return -1; } // no intersection

    float d_square = len(r0 + tc * rd - this->location) * len(r0 + tc * rd - this->location);
    float delta = this->radius * this->radius - d_square;

    if ((!inside) && (delta < 0)) { return -1; } // no intersection

    float t_offset = sqrt(delta) / len(rd);

    return inside ? tc + t_offset : tc - t_offset;
}

Vec3 Sphere::compute_normal(Vec3 p) {
    return normalize(p - this->location);
}

float Plane::intersection(Vec3 r0, Vec3 ray) {

    Vec3 n = normalize(Vec3(this->params.x, this->params.y, this->params.z));
    Vec3 p;
    // avoid division by zero
    if (this->params.x != 0)
        p = Vec3(-1.0f * this->params.w / this->params.x, 0, 0);
    else if (this->params.y != 0)
        p = Vec3(0, -1.0f * this->params.w / this->params.y, 0);
    else if (this->params.z != 0)
        p = Vec3(0, 0, -1.0f * this->params.w / this->params.z);

    Vec3 rd = normalize(ray);
    float t = dot((p - r0), n) / dot(rd, n);

    return t > 0 ? t : -1;
}

Vec3 Plane::compute_normal(Vec3 p) {
    return normalize(Vec3(this->params.x, this->params.y, this->params.z));
}

float Triangle::intersection(Vec3 r0, Vec3 ray) {
    // Möller-Trumbore algorithm
    Vec3 edge1 = this->v2 - this->v1;
    Vec3 edge2 = this->v3 - this->v1;
    Vec3 pvec = cross(normalize(ray), edge2);
    float det = dot(edge1, pvec);

    if (fabs(det) < 1e-8) { return -1; }

    float inv_det = 1.0f / det;
    Vec3 tvec = r0 - this->v1;
    float u = dot(tvec, pvec) * inv_det;

    if (u < 0 || u > 1) { return -1; }

    Vec3 qvec = cross(tvec, edge1);
    float v = dot(normalize(ray), qvec) * inv_det;

    if (v < 0 || u + v > 1) { return -1; }

    float t = dot(edge2, qvec) * inv_det;

    return t > 0 ? t : -1;
}

Vec3 Triangle::compute_normal(Vec3 p) {
    return normalize(cross(this->v1 - this->v2, this->v1 - this->v3));
}
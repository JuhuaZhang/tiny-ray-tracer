#include "vec.h"

vec3 operator+(const vec3 &v1, const vec3 &v2) {
    return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; // brace initialization
}

vec3 operator-(const vec3 &v1, const vec3 &v2) {
    return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

vec3 operator*(float c, const vec3 &v1) {
    return {v1.x * c, v1.y * c, v1.z * c};
}

float dot(const vec3 &v1, const vec3 &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec3 cross(const vec3 &v1, const vec3 &v2) {
    return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x};
}

vec3 normalize(const vec3 &v) {
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return {v.x / len, v.y / len, v.z / len};
}

float len(const vec3 &v) {
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return len;
}

vec4 operator+(const vec4 &v1, const vec4 &v2) {
    return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

vec4 operator-(const vec4 &v1, const vec4 &v2) {
    return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

float dot(const vec4 &v1, const vec4 &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

vec4 normalize(const vec4 &v) {
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    return {v.x / len, v.y / len, v.z / len, v.w / len};
}

vec3 to_vec3(const vec4 &v) { return {v.x, v.y, v.z}; }

// read the file
vector<vector<string>> read_file(const string &fileName) {

    ifstream input;
    input.open(fileName);
    vector<vector<string>> contents;

    if (input.is_open()) {
        string aline;

        // read line by line
        while (getline(input, aline)) {
            vector<string> content;
            string word;
            size_t i = 0;

            // remove the space in the front
            while (i < aline.size() && (aline[i] == ' ' || aline[i] == '\t')) {
                i++;
            }

            for (; i < aline.size(); i++) {
                if (aline[i] == ' ' || aline[i] == '\t') {
                    while (i < aline.size() &&
                           (aline[i] == ' ' || aline[i] == '\t')) {
                        i++;
                    }
                    i--;
                    content.push_back(word);
                    word = "";
                }
                else {
                    word += aline[i];
                }
            }

            // last word
            if (!word.empty()) {
                content.push_back(word);
            }

            // skip the empty line
            if (!content.empty()) {
                contents.push_back(content);
            }
        }
        input.close();
    }
    else {
        cerr << "Can't open: " << fileName << "." << endl;
        exit(-1);
    }
    return contents;
}

void parse_content(vector<vector<string>> &contents, vector<obj *> &objects,
                   vector<lgt *> &lights, vector<bulb *> &bulbs, image &img,
                   vec4 &cur_color, ray_tracer &tracer,
                   vector<vec3> &vertices) {
    for (auto &content: contents) {
        if (content.size() >= 4 && content[0] == "png") {
            img.w = stoi(content[1]);
            img.h = stoi(content[2]);
            img.image_name = content[3];
        }
        else if (content.size() >= 5 && content[0] == "sphere") {
            auto *new_sphere =
                    new sphere(vec3(stof(content[1]), stof(content[2]),
                                    stof(content[3])),
                               cur_color, stof(content[4]));

            if (img.is_shiny) {
                new_sphere->is_shiny = true;
                new_sphere->shininess = img.shininess;
            }
            if (img.is_transparent) {
                new_sphere->is_transparent = true;
                new_sphere->transparency = img.transparency;
            }

            objects.push_back(new_sphere);
        }
        else if (content.size() >= 4 && content[0] == "sun") {
            lgt *new_light =
                    new lgt(vec3(stof(content[1]), stof(content[2]),
                                 stof(content[3])),
                            cur_color);
            lights.push_back(new_light);
        }
        else if (content.size() >= 4 && content[0] == "color") {
            vec4 temp_color = {stof(content[1]), stof(content[2]),
                               stof(content[3]),
                               1.0f};
            //            cur_color = color_mapping(temp_color);
            cur_color = temp_color;
        }
        else if (content.size() >= 4 && content[0] == "bulb") {
            bulb *new_bulb =
                    new bulb(vec3(stof(content[1]), stof(content[2]),
                                  stof(content[3])),
                             cur_color);
            bulbs.push_back(new_bulb);
        }
        else if (content.size() >= 2 && content[0] == "expose") {
            img.is_expose = true;
            img.expose = stof(content[1]);
        }
        else if (content.size() >= 5 && content[0] == "plane") {
            auto *new_plane = new plane(vec4(stof(content[1]), stof(content[2]),
                                             stof(content[3]),
                                             stof(content[4])),
                                        cur_color);
            objects.push_back(new_plane);
        }
        else if (content.size() >= 4 && content[0] == "eye") {
            vec3 new_eye = {stof(content[1]), stof(content[2]),
                            stof(content[3])};
            tracer.eye = new_eye;
        }
        else if (content.size() >= 4 && content[0] == "forward") {
            vec3 new_forward = {stof(content[1]), stof(content[2]),
                                stof(content[3])};
            tracer.forward = new_forward;
            // change up and right vectors to be perpendicular to the new forward
            // vector Keep up as close to the original up as possible
            vec3 original_up = tracer.up; // Store the original up vector

            // Step 1: Project the original up vector onto the new forward vector
            vec3 proj_up_on_forward =
                    (dot(original_up, new_forward) /
                     dot(new_forward, new_forward)) *
                    new_forward;

            // Step 2: Subtract the projection from the original up vector to get the
            // new up vector
            vec3 new_up = original_up - proj_up_on_forward;
            tracer.up = normalize(new_up); // Normalize the new up vector

            // Step 3: Compute the new right vector using the cross product of the new
            // up and new forward vectors
            vec3 new_right = cross(new_up, new_forward);
            tracer.right =
                    -1 * normalize(new_right); // Normalize the new right vector
        }
        else if (content.size() >= 4 && content[0] == "up") {
            vec3 input_up = {stof(content[1]), stof(content[2]),
                             stof(content[3])};
            // use the closest possible up that is perpendicular to the existing
            // forward. then change the right vector to be perpendicular to forward
            // and up.
            vec3 existing_forward =
                    tracer.forward; // Store the existing forward vector

            // Step 1: Project the input_up vector onto the existing forward vector
            vec3 proj_input_up_on_forward =
                    (dot(input_up, existing_forward) /
                     dot(existing_forward, existing_forward)) *
                    existing_forward;

            // Step 2: Subtract the projection from the input_up vector to get the new
            // up vector
            vec3 new_up = input_up - proj_input_up_on_forward;
            tracer.up = normalize(new_up); // Normalize the new up vector

            // Step 3: Compute the new right vector using the cross product of the new
            // up and existing forward vectors
            vec3 new_right = -1 * cross(new_up, existing_forward);
            tracer.right = normalize(
                    new_right); // Normalize the new right vector
        }
        else if (content.size() >= 2 && content[0] == "shininess") {
            img.is_shiny = true;
            img.shininess = stof(content[1]);
        }
        else if (content.size() >= 4 && content[0] == "xyz") {
            vec3 new_vertex(stof(content[1]), stof(content[2]),
                            stof(content[3]));
            vertices.push_back(new_vertex);
        }
        else if (content.size() >= 4 && content[0] == "trif") {
            vector<int> idx;
            for (size_t i = 1; i < content.size(); i++) {
                int tmp = stoi(content[i]);
                idx.push_back(tmp > 0 ? tmp - 1 : int(vertices.size()) + tmp);
            }
            auto *new_triangle = new triangle(vertices[idx[0]],
                                              vertices[idx[1]],
                                              vertices[idx[2]], cur_color);
            objects.push_back(new_triangle);
        }
        else if (content.size() >= 2 && content[0] == "transparency") {
            img.is_transparent = true;
            img.transparency = stof(content[1]);
        }
    }
}

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

void generate_image(image &img) {
    auto *im = (unsigned char *) calloc(img.w * img.h * 4,
                                        sizeof(unsigned char));

    for (size_t i = 0; i < img.pixels.size(); i++) {
        im[((img.pixels[i].y * img.w) + img.pixels[i].x) * 4 + 0] =
                char(img.pixels[i].r * 255.0f);
        im[((img.pixels[i].y * img.w) + img.pixels[i].x) * 4 + 1] =
                char(img.pixels[i].g * 255.0f);
        im[((img.pixels[i].y * img.w) + img.pixels[i].x) * 4 + 2] =
                char(img.pixels[i].b * 255.0f);
        im[((img.pixels[i].y * img.w) + img.pixels[i].x) * 4 + 3] =
                char(img.pixels[i].a * 255.0f);
    }
    lodepng_encode32_file(img.image_name.c_str(), im, img.w, img.h);
    free(im);
}

float intersection(vec3 r0, vec3 ray, obj *object) {
    // determine the object type
    if (auto *s = dynamic_cast<sphere *>(object)) {
        vec3 c = s->location;
        vec3 rd = normalize(ray);
        float r = s->radius;

        // evaluate if the ray originates inside the sphere
        bool inside = (len(c - r0) < r);
        float tc = dot((c - r0), rd) / len(rd);

        if ((!inside) && (tc < 0)) {
            return -1; // no intersection
        }

        float d_square = len(r0 + tc * rd - c) * len(r0 + tc * rd - c);
        if ((!inside) && (r * r < d_square)) {
            return -1; // no intersection
        }

        float t_offset = sqrt(r * r - d_square) / len(rd);

        if (inside) {
            return tc + t_offset;
        }
        else {
            return tc - t_offset;
        }
    }
    else if (auto *s = dynamic_cast<plane *>(object)) {
        vec3 n = normalize(vec3(s->params.x, s->params.y, s->params.z));

        vec3 p;
        // avoid division by zero
        if (s->params.x != 0)
            p = vec3(-1.0f * s->params.w / s->params.x, 0, 0);
        else if (s->params.y != 0)
            p = vec3(0, -1.0f * s->params.w / s->params.y, 0);
        else if (s->params.z != 0)
            p = vec3(0, 0, -1.0f * s->params.w / s->params.z);

        vec3 rd = normalize(ray);
        float t = dot((p - r0), n) / dot(rd, n);

        if (t > 0)
            return t;
        else
            return -1;
    }
    else if (auto *s = dynamic_cast<triangle *>(object)) {
        // Möller-Trumbore algorithm
        vec3 edge1 = s->v2 - s->v1;
        vec3 edge2 = s->v3 - s->v1;
        vec3 pvec = cross(normalize(ray), edge2);
        float det = dot(edge1, pvec);

        if (fabs(det) < 1e-8) {
            return -1;
        }

        float inv_det = 1.0f / det;
        vec3 tvec = r0 - s->v1;
        float u = dot(tvec, pvec) * inv_det;

        if (u < 0 || u > 1) {
            return -1;
        }

        vec3 qvec = cross(tvec, edge1);
        float v = dot(normalize(ray), qvec) * inv_det;

        if (v < 0 || u + v > 1) {
            return -1;
        }

        float t = dot(edge2, qvec) * inv_det;

        if (t > 0) {
            return t;
        }
        else {
            return -1;
        }
    }

    return -1;
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
    // for each channel, if value < 0, convert to 0, if value > 1, convert to 1
    //    color.x = (color.x < 0) ? 0 : color.x;
    //    color.x = (color.x > 1) ? 1 : color.x;
    //    color.y = (color.y < 0) ? 0 : color.y;
    //    color.y = (color.y > 1) ? 1 : color.y;
    //    color.z = (color.z < 0) ? 0 : color.z;
    //    color.z = (color.z > 1) ? 1 : color.z;

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

vec4 compute_color(vec3 r0, vec3 ray, float n_t, obj *object,
                   vector<lgt *> &lights, vector<bulb *> &bulbs,
                   vector<obj *> &objects, image &img, int depth) {
    vec3 p = r0 + n_t * normalize(ray);

    // check if the camera is inside the sphere, camera location is r0
    float is_inside = 1.0f; // not inside

    vector<vec3> colors;
    vec3 n;

    if (auto *s = dynamic_cast<sphere *>(object)) {
        // check inside
        if (len(r0 - s->location) < s->radius)
            is_inside = -1.0f;

        n = normalize(p - s->location);
    }
    else if (auto *s = dynamic_cast<plane *>(object)) {
        n = normalize(vec3(s->params.x, s->params.y, s->params.z));
    }
    else if (auto *s = dynamic_cast<triangle *>(object)) {
        n = normalize(cross(s->v3 - s->v1, s->v2 - s->v1));
        if (dot(n, ray) > 0)
            n = -1.0f * n;
    }
    else return {0, 0, 0, 1};
    // check if the point is in shadow
    for (auto light: lights) {
        bool in_shadow = false;
        for (auto *o: objects) {
            if (o != object) {
                if (intersection(p, light->direction, o) != -1) {
                    in_shadow = true;
                    break;
                }
            }
        }
        if (!in_shadow) {
            vec3 x = normalize(light->direction); // incoming light direction
            vec3 cur_color = vec3((object->color.x * light->color.x),
                                  (object->color.y * light->color.y),
                                  (object->color.z * light->color.z));
            cur_color = max(dot(n, x), 0.0f) * cur_color;
            colors.push_back(cur_color);
        }
    }

    for (auto bulb: bulbs) {
        bool in_shadow = false;
        vec3 x = is_inside * normalize(bulb->location - p);
        for (auto *o: objects) {
            if (o != object) {
                if (intersection(p, x, o) != -1 && is_inside == -1.0f) {
                    in_shadow = true;
                    break;
                }
            }
        }
        if (!in_shadow) {
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

//    if (object->is_transparent) {
//        float ior = 1.458f; // index of refraction
//        float cosi = clamp(dot(ray, n));
//        float etai = 1.0f, etat = ior;
//        vec3 normal = n;
//        if (cosi > 0) {
//            std::swap(etai, etat);
//            normal = -1.0f * n;
//        }
//        float eta = etai / etat;
//        float k = 1 - eta * eta * (1 - cosi * cosi);
//        vec3 refracted_dir = k < 0 ? vec3(0, 0, 0) :
//                             eta * ray + (eta * cosi - sqrt(k)) * normal;
//
//        vec3 refracted_color = trace_ray(p, refracted_dir, object, lights,
//                                         bulbs, objects, img, depth + 1);
//
//        // Fresnel effect using Schlick's approximation
//        float R0 = (etat - etai) / (etat + etai);
//        R0 *= R0;
//        float c = 1 - (cosi < 0 ? -cosi : dot(refracted_dir, normal));
//        float fresnel = R0 + (1 - R0) * pow(c, 5);
//        vec3 tmp = fresnel * specular_color + (1 - fresnel) * refracted_color;
//        final_color = vec4(tmp, object->transparency * object->color.w);
//    }

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
            float temp = intersection(r0, ray, obj);
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

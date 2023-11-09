#include "utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include "lodepng.h"

using namespace std;

// read the file
vector<vector<string>> read_file(const string& fileName) {
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

void parse_content(vector<vector<string>>& contents, Image& image, Raytracer& tracer, vector<Object*>& objects, vector<Light*>& lights) {

    vector<Vec3> vertices; // store vertices to form a triangle
    Vec4 cur_color(1.0f, 1.0f, 1.0f, 1.0f); // default color

    for (auto& content : contents) {
        if (content[0] == "png" && content.size() >= 4) {
            image.w = stoi(content[1]);
            image.h = stoi(content[2]);
            image.image_name = content[3];
        }
        else if (content[0] == "sphere" && content.size() >= 5) {
            auto* new_sphere = new Sphere(Vec3(stof(content[1]), stof(content[2]), stof(content[3])), cur_color, stof(content[4]));

            if (image.is_shiny) {
                new_sphere->is_shiny = true;
                new_sphere->shininess = image.shininess;
            }
            if (image.is_transparent) {
                new_sphere->is_transparent = true;
                new_sphere->transparency = image.transparency;
            }

            objects.emplace_back(new_sphere);
        }
        else if (content[0] == "sun" && content.size() >= 4) {
            Sun* new_sun = new Sun(Vec3(stof(content[1]), stof(content[2]), stof(content[3])), cur_color);
            lights.emplace_back(new_sun);
        }
        else if (content[0] == "color" && content.size() >= 4) {
            Vec4 temp_color = { stof(content[1]), stof(content[2]), stof(content[3]), 1.0f };
            cur_color = temp_color;
        }
        else if (content[0] == "bulb" && content.size() >= 4) {
            Bulb* new_bulb = new Bulb(Vec3(stof(content[1]), stof(content[2]), stof(content[3])), cur_color);
            lights.emplace_back(new_bulb);
        }
        else if (content[0] == "expose" && content.size() >= 2) {
            image.is_expose = true;
            image.expose = stof(content[1]);
        }
        else if (content[0] == "plane" && content.size() >= 5) {
            auto* new_plane = new Plane(Vec4(stof(content[1]), stof(content[2]), stof(content[3]), stof(content[4])), cur_color);
            objects.emplace_back(new_plane);
        }
        else if (content[0] == "eye" && content.size() >= 4) {
            Vec3 new_eye = { stof(content[1]), stof(content[2]),stof(content[3]) };
            tracer.eye = new_eye;
        }
        else if (content[0] == "forward" && content.size() >= 4) {
            Vec3 new_forward = { stof(content[1]), stof(content[2]),stof(content[3]) };
            tracer.forward = new_forward;
            // Change up and right vectors to be perpendicular to the new forward.
            Vec3 original_up = tracer.up;
            Vec3 proj_up_on_forward = (dot(original_up, new_forward) / dot(new_forward, new_forward)) * new_forward;
            Vec3 new_up = original_up - proj_up_on_forward;
            tracer.up = normalize(new_up);
            Vec3 new_right = cross(new_up, new_forward);
            tracer.right = -1 * normalize(new_right);
        }
        else if (content[0] == "up" && content.size() >= 4) {
            Vec3 input_up = { stof(content[1]), stof(content[2]), stof(content[3]) };
            // Use the closest possible up that is perpendicular to the existing forward. 
            // Then change the right vector to be perpendicular to forward and up.
            Vec3 existing_forward = tracer.forward;
            Vec3 proj_input_up_on_forward = (dot(input_up, existing_forward) / dot(existing_forward, existing_forward)) * existing_forward;
            Vec3 new_up = input_up - proj_input_up_on_forward;
            tracer.up = normalize(new_up);
            Vec3 new_right = -1 * cross(new_up, existing_forward);
            tracer.right = normalize(new_right);
        }
        else if (content[0] == "shininess" && content.size() >= 2) {
            image.is_shiny = true;
            image.shininess = stof(content[1]);
        }
        else if (content[0] == "xyz" && content.size() >= 4) {
            Vec3 new_vertex(stof(content[1]), stof(content[2]), stof(content[3]));
            vertices.emplace_back(new_vertex);
        }
        else if (content[0] == "trif" && content.size() >= 4) {
            vector<int> idx;
            for (size_t i = 1; i < content.size(); i++) {
                int tmp = stoi(content[i]);
                idx.emplace_back(tmp > 0 ? tmp - 1 : int(vertices.size()) + tmp);
            }
            auto* new_triangle = new Triangle(vertices[idx[0]], vertices[idx[1]], vertices[idx[2]], cur_color);
            objects.emplace_back(new_triangle);
        }
        else if (content[0] == "transparency" && content.size() >= 2) {
            image.is_transparent = true;
            image.transparency = stof(content[1]);
        }
    }
}

void generate_image(Image& image) {
    auto* img = ( unsigned char* )calloc(image.w * image.h * 4,
        sizeof(unsigned char));

    for (size_t i = 0; i < image.pixels.size(); i++) {
        img[((image.pixels[i].y * image.w) + image.pixels[i].x) * 4 + 0] =
            char(image.pixels[i].r * 255.0f);
        img[((image.pixels[i].y * image.w) + image.pixels[i].x) * 4 + 1] =
            char(image.pixels[i].g * 255.0f);
        img[((image.pixels[i].y * image.w) + image.pixels[i].x) * 4 + 2] =
            char(image.pixels[i].b * 255.0f);
        img[((image.pixels[i].y * image.w) + image.pixels[i].x) * 4 + 3] =
            char(image.pixels[i].a * 255.0f);
    }
    lodepng_encode32_file(image.image_name.c_str(), img, image.w, image.h);
    free(img);
}

bool check_occlusion(Vec3 p, Object* self_object, vector<Object*>& objects, Light* lght) {
    for (auto* object : objects) {
        if (object != self_object) {
            if (object->intersection(p, lght->get_light_dir(p)) != -1)
                return true;
        }
    }
    return false;
}

Vec4 compute_color(Vec3 r0, Vec3 ray, float n_t, Object* object, vector<Light*>& lights, vector<Object*>& objects, Image& image, int depth) {
    Vec3 p = r0 + n_t * normalize(ray);
    vector<Vec3> colors;

    Vec3 n = object->compute_normal(p);
    if (dot(n, ray) > 0) n = -1.0f * n;

    // check if the point is in shadow
    for (auto light : lights) {
        if (!check_occlusion(p, object, objects, light)) {
            colors.push_back(light->compute_diffuse_color(p, n, object->color));
        }
    }

    // diffuse light
    Vec3 diffuse_color = Vec3(0, 0, 0);
    for (auto color : colors) {
        diffuse_color = diffuse_color + color;
        if (image.is_expose)
            diffuse_color = expose(diffuse_color, image.expose);
        diffuse_color = clamp(diffuse_color);
    }

    Vec3 specular_color = Vec3(0, 0, 0);
    if (object->is_shiny) {
        Vec3 reflect_dir = normalize(ray) - 2.0f * dot(n, normalize(ray)) * n;
        reflect_dir = normalize(reflect_dir);
        specular_color = object->shininess * trace_ray(p, reflect_dir, object, lights, objects, image, depth + 1);
        specular_color = clamp(specular_color);
    }
    Vec4 final_color = { clamp(diffuse_color + specular_color), object->color.w };
    return final_color;
}

Vec3 trace_ray(Vec3 r0, Vec3 ray, Object* object, vector<Light*>& lights, vector<Object*>& objects, Image& image, int depth) {
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
            compute_color(r0, ray, min_t, nearest_object, lights, objects, image, depth));
    }
    else {
        return { 0, 0, 0 };
    }
}

void cleanup_objects(vector<Object*>& objects, vector<Light*>& lights) {
    for (Object* object : objects) {
        delete object;
    }
    objects.clear();

    for (Light* light : lights) {
        delete light;
    }
    lights.clear();
}
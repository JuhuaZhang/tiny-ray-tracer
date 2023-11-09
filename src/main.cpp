#include <iostream>
#include <vector>
#include <unordered_map>
#include "vec.h"
#include "object.h"
#include "light.h"
#include "utils.h"

using namespace std;

int main(int argc, char* argv [ ]) {

    if (argc != 2) {
        cerr << "Usage: ./program file.txt" << endl;
        return -1;
    }

    auto contents = read_file(argv[1]);

    Image image;
    Raytracer raytracer;
    vector<Object*> objects;
    vector<Light*> lights;

    parse_content(contents, image, raytracer, objects, lights);

    int max_wh = max(image.w, image.h);
    for (int x = 0; x < image.w; x++) {
        for (int y = 0; y < image.h; y++) {
            // make ray
            float sx = (2.0f * float(x) - float(image.w)) / float(max_wh);
            float sy = (float(image.h) - 2.0f * float(y)) / float(max_wh);
            Vec3 ray = raytracer.forward + sx * raytracer.right + sy * raytracer.up;
            float n_t = -1; // nearest t
            vector<float> t;
            // a map correspond to t and obj
            unordered_map<float, Object*> obj_map;

            for (Object* object : objects) {
                float temp = object->intersection(raytracer.eye, ray);
                t.push_back(temp);
                obj_map[temp] = object;
            }

            // find the nearest t
            for (auto to : t) {
                if (to > 0) {
                    if (n_t == -1) n_t = to;
                    else n_t = min(n_t, to);
                }
            }

            if (n_t > 0) {
                Vec4 obj_color;

                if (lights.empty())
                    obj_color = Vec4(0, 0, 0, 1);
                else
                    obj_color = linear_to_srgb(
                        compute_color(raytracer.eye, ray, n_t, obj_map[n_t], lights, objects, image, 1));
                Pixel new_pix(x, y, obj_color);
                image.pixels.push_back(new_pix);
            }
        }
    }

    cleanup_objects(objects, lights);

    generate_image(image);
}

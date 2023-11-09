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

    if ( input.is_open() ) {
        string aline;

        // read line by line
        while ( getline(input, aline) ) {
            vector<string> content;
            string word;
            size_t i = 0;

            // remove the space in the front
            while ( i < aline.size() && (aline[i] == ' ' || aline[i] == '\t') ) {
                i++;
            }

            for ( ; i < aline.size(); i++ ) {
                if ( aline[i] == ' ' || aline[i] == '\t' ) {
                    while ( i < aline.size() &&
                        (aline[i] == ' ' || aline[i] == '\t') ) {
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
            if ( !word.empty() ) {
                content.push_back(word);
            }

            // skip the empty line
            if ( !content.empty() ) {
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

void parse_content(vector<vector<string>>& contents, vector<obj*>& objects,
    vector<lgt*>& lights, vector<bulb*>& bulbs, image& img,
    vec4& cur_color, ray_tracer& tracer,
    vector<vec3>& vertices) {
    for ( auto& content : contents ) {
        if ( content.size() >= 4 && content[0] == "png" ) {
            img.w = stoi(content[1]);
            img.h = stoi(content[2]);
            img.image_name = content[3];
        }
        else if ( content.size() >= 5 && content[0] == "sphere" ) {
            auto* new_sphere =
                new sphere(vec3(stof(content[1]), stof(content[2]),
                    stof(content[3])),
                    cur_color, stof(content[4]));

            if ( img.is_shiny ) {
                new_sphere->is_shiny = true;
                new_sphere->shininess = img.shininess;
            }
            if ( img.is_transparent ) {
                new_sphere->is_transparent = true;
                new_sphere->transparency = img.transparency;
            }

            objects.push_back(new_sphere);
        }
        else if ( content.size() >= 4 && content[0] == "sun" ) {
            lgt* new_light =
                new lgt(vec3(stof(content[1]), stof(content[2]),
                    stof(content[3])),
                    cur_color);
            lights.push_back(new_light);
        }
        else if ( content.size() >= 4 && content[0] == "color" ) {
            vec4 temp_color = { stof(content[1]), stof(content[2]),
                               stof(content[3]),
                               1.0f };
            //            cur_color = color_mapping(temp_color);
            cur_color = temp_color;
        }
        else if ( content.size() >= 4 && content[0] == "bulb" ) {
            bulb* new_bulb =
                new bulb(vec3(stof(content[1]), stof(content[2]),
                    stof(content[3])),
                    cur_color);
            bulbs.push_back(new_bulb);
        }
        else if ( content.size() >= 2 && content[0] == "expose" ) {
            img.is_expose = true;
            img.expose = stof(content[1]);
        }
        else if ( content.size() >= 5 && content[0] == "plane" ) {
            auto* new_plane = new plane(vec4(stof(content[1]), stof(content[2]),
                stof(content[3]),
                stof(content[4])),
                cur_color);
            objects.push_back(new_plane);
        }
        else if ( content.size() >= 4 && content[0] == "eye" ) {
            vec3 new_eye = { stof(content[1]), stof(content[2]),
                            stof(content[3]) };
            tracer.eye = new_eye;
        }
        else if ( content.size() >= 4 && content[0] == "forward" ) {
            vec3 new_forward = { stof(content[1]), stof(content[2]),
                                stof(content[3]) };
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
        else if ( content.size() >= 4 && content[0] == "up" ) {
            vec3 input_up = { stof(content[1]), stof(content[2]),
                             stof(content[3]) };
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
        else if ( content.size() >= 2 && content[0] == "shininess" ) {
            img.is_shiny = true;
            img.shininess = stof(content[1]);
        }
        else if ( content.size() >= 4 && content[0] == "xyz" ) {
            vec3 new_vertex(stof(content[1]), stof(content[2]),
                stof(content[3]));
            vertices.push_back(new_vertex);
        }
        else if ( content.size() >= 4 && content[0] == "trif" ) {
            vector<int> idx;
            for ( size_t i = 1; i < content.size(); i++ ) {
                int tmp = stoi(content[i]);
                idx.push_back(tmp > 0 ? tmp - 1 : int(vertices.size()) + tmp);
            }
            auto* new_triangle = new triangle(vertices[idx[0]],
                vertices[idx[1]],
                vertices[idx[2]], cur_color);
            objects.push_back(new_triangle);
        }
        else if ( content.size() >= 2 && content[0] == "transparency" ) {
            img.is_transparent = true;
            img.transparency = stof(content[1]);
        }
    }
}

void generate_image(image& img) {
    auto* im = ( unsigned char* )calloc(img.w * img.h * 4,
        sizeof(unsigned char));

    for ( size_t i = 0; i < img.pixels.size(); i++ ) {
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

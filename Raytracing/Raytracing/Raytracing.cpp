// Raytracing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "color.h"
#include "vec3.h"
#include "ray.h"
#include <iostream>

//sphere hits
//solved by finding the roots of a quadratic equation
//one root for glancing, two roots for going through, imaginary roots for no intersection
//the equation to find the ray sphere intersection is (P-C) * (P-C) = r^2
//expanded into the (P(t) - C) * (P(t) - C) = r^2
//and then further because P(t) = A + t*b; where A is origin and b is direction
double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared(); //equal to dot(r.direction(),r.direction());
    auto half_b = dot(oc, r.direction()); // equal to b = 2.0 * dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius; // dot(oc, oc) - radius * radius;
    auto discriminant = half_b * half_b - a * c;//  b* b - 4 * a * c;
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return(-half_b - sqrt(discriminant)) / (a);
    }
}


//create a ray color function that evaluates a background
color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));//compute normal direction toward center of sphere
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    


    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            //The ray scans from bottom left and gradually evaluates upwards.
            ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

#include <omp.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "common/Canvas.h"
#include "common/rtweekend.h"
#include "common/hittable_list.h"
#include "common/Sphere.h"
#include "common/camera.h"
#include "common/material.h"

using std::make_shared;
using std::shared_ptr;

Vec3 RayColor(const Ray &ray, const Hittable &world, int depth)
{
  HitRecord hit_rec;

  // If we've exceeded the ray bounce limit, no more light is gathered.
  if (depth <= 0) return Vec3(0, 0, 0);

  if (world.Hit(ray, 0.001f, infinity, hit_rec)) {
    Ray scattered;
    Vec3 attenuation;
    if (hit_rec.mat_ptr_->Scatter(ray, hit_rec, attenuation, scattered))
      return attenuation * RayColor(scattered, world, depth - 1);
    else
      return Vec3(0.0f, 0.0f, 0.0f);
  }

  // if not hitting, calc sky background color.
  Vec3 unit_direction = UnitVector(ray.Direction());
  auto t              = 0.5f * (unit_direction.y + 1.0);
  return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
}

HittableList random_scene()
{
  HittableList world;

  world.Add(make_shared<Sphere>(Vec3(0, -1000, 0), 1000, make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5))));

  int i = 1;
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      Vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
      if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = Vec3::random() * Vec3::random();
          world.Add(make_shared<Sphere>(center, 0.2, make_shared<Lambertian>(albedo)));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = Vec3::random(.5, 1);
          auto fuzz   = random_double(0, .5);
          world.Add(make_shared<Sphere>(center, 0.2, make_shared<Metal>(albedo, fuzz)));
        } else {
          // glass
          world.Add(make_shared<Sphere>(center, 0.2, make_shared<Dielectric>(1.5)));
        }
      }
    }
  }

  world.Add(make_shared<Sphere>(Vec3(0, 1, 0), 1.0, make_shared<Dielectric>(1.5)));

  world.Add(make_shared<Sphere>(Vec3(-4, 1, 0), 1.0, make_shared<Lambertian>(Vec3(0.4, 0.2, 0.1))));

  world.Add(make_shared<Sphere>(Vec3(4, 1, 0), 1.0, make_shared<Metal>(Vec3(0.7, 0.6, 0.5), 0.0)));

  return world;
}

struct RenderTarget {
  int width                   = 1600;
  int height                  = 800;
  int samples_per_pixel       = 20;
  int max_depth               = 100;
  std::string output_filename = "image.ppm";
};

void RayTracingThreadFunc(int thread_id, Canvas &canvas, const RenderTarget &render_target, HittableList &world, Camera &camera, int start, int end)
{
  for (int y = end - 1; y >= start; y--) {
    for (int x = 0; x < render_target.width; x++) {
      Vec3 color(0, 0, 0);
      for (int s = 0; s < render_target.samples_per_pixel; ++s) {
        // Random shake of UV, random shake of UV, and random sampling 100 times.
        auto u = (x + random_double()) / (render_target.width);
        auto v = (render_target.height - y + random_double()) / (render_target.height);

        // Scatter light by using the camera as the origin of the light
        Ray ray = camera.GetRay(u, v);
        color += RayColor(ray, world, render_target.max_depth);
      }

      color = anti_aliasing(color, render_target.samples_per_pixel);
      color = gamma_correct(color);
      color *= 255.99;

      canvas[x][y] = color;
    }
  }
}

void StartRayTracingThreads(RenderTarget &render_target, Canvas &canvas, HittableList &world, Camera &camera)
{
  const int no_max_thread = std::thread::hardware_concurrency();

  std::vector<std::thread> calc_threads;

  int steps = render_target.height / no_max_thread;
  for (int i = 0; i < no_max_thread; i++) {
    int start = i * steps;
    int end   = (i + 1) * steps;
    if (i == no_max_thread - 1) {
      end = render_target.height;
    }
    calc_threads.push_back(std::thread(RayTracingThreadFunc, i, std::ref(canvas), std::ref(render_target), std::ref(world), std::ref(camera), start, end));
  }

  for (int i = 0; i < no_max_thread; i++) {
    calc_threads[i].join();
  }
}

int main()
{
  RenderTarget render_target;

  std::ofstream ppm_file(render_target.output_filename, std::ios::binary);
  ppm_file << "P6\n" << render_target.width << " " << render_target.height << "\n255\n";

  Canvas canvas(render_target.width, render_target.height);

  HittableList world = random_scene();

  world.Add(make_shared<Sphere>(Vec3(0, 1, 0), 1.0, make_shared<Dielectric>(1.5)));
  world.Add(make_shared<Sphere>(Vec3(-4, 1, 0), 1.0, make_shared<Lambertian>(Vec3(0.4, 0.2, 0.1))));
  world.Add(make_shared<Sphere>(Vec3(4, 1, 0), 1.0, make_shared<Metal>(Vec3(0.7, 0.6, 0.5), 0.0)));

  const double aspect_ratio = double(render_target.width) / render_target.height;

  const Vec3 lookfrom        = {13, 2, 3};
  const Vec3 lookat          = {0, 0, 0};
  const Vec3 up              = {0.0, 1.0, 0.0};
  const double dist_to_focus = 10.0;
  const double aperture      = 0.1;

  Camera camera = {lookfrom, lookat, up, 20, aspect_ratio, aperture, dist_to_focus};

  auto start = std::chrono::steady_clock::now();

  bool usedMultiThread = false;

  if (usedMultiThread) {
    StartRayTracingThreads(render_target, canvas, world, camera);
  } else {
#pragma omp parallel for num_threads(16)
    for (int y = render_target.height - 1; y >= 0; y--) {
      for (int x = 0; x < render_target.width; x++) {
        Vec3 color(0, 0, 0);
        for (int s = 0; s < render_target.samples_per_pixel; ++s) {
          // Random shake of UV, random shake of UV, and random sampling 100 times.
          auto u = (x + random_double()) / (render_target.width);
          auto v = (render_target.height - y + random_double()) / (render_target.height);

          // Scatter light by using the camera as the origin of the light
          Ray ray = camera.GetRay(u, v);
          color += RayColor(ray, world, render_target.max_depth);
        }

        color = anti_aliasing(color, render_target.samples_per_pixel);
        color = gamma_correct(color);
        color *= 255.99;

        canvas[x][y] = color;
      }
    }
  }

  std::cout << render_target.width << "x" << render_target.height << "\nDone.\n";

  ppm_file.write(canvas.ConvertToPpmData(), canvas.GetDataSize());
  ppm_file.flush();
  ppm_file.close();

  int w, h, channel;
  unsigned char *data = stbi_load(render_target.output_filename.c_str(), &w, &h, &channel, 0);

  std::string output_path = "image.png";
  stbi_write_png(output_path.c_str(), w, h, channel, data, 0);

  std::string show_output = "start mspaint " + output_path;

  auto end = std::chrono::steady_clock::now();
  std::cout << (usedMultiThread ? "Custom MultiThread-" : "Openmp-") << "Total cost:" << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.f
            << "s" << std::endl;

  system(show_output.c_str());
}
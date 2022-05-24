#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <iostream>
#include <vector>
#include <random>

#include "camera.h"
#include "sphere.h"
#include "light.h"
#include "scene.h"

#include <SDL2/SDL.h>

#include <omp.h>

#define SAMPLES_PER_PIXEL 16

struct VectorUchar {
    uint8_t x;
    uint8_t y;
    uint8_t z;
};

void render(uint32_t width, uint32_t height, SDL_Renderer* renderer, Scene* scene, Camera camera) {
    glm::vec2 uvMult = 2.0f / glm::vec2(width, height);

    std::vector<VectorUchar> colors;
    colors.resize(width * height);
    
    #pragma omp parallel for collapse(2)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            glm::vec3 color(0.0f, 0.0f, 0.0f);
            for (int i = 0; i < SAMPLES_PER_PIXEL; i++) {
                float u = x + ((double) rand() / (double) RAND_MAX);
                float v = y + ((double) rand() / (double) RAND_MAX);

                color += scene->raytrace(camera, glm::vec2(u, v) * uvMult - 1.0f);
            }

            color /= SAMPLES_PER_PIXEL;

            colors[y * width + x].x = (int)(color.x * 255.999);
            colors[y * width + x].y = (int)(color.y * 255.999);
            colors[y * width + x].z = (int)(color.z * 255.999);

            

            /*glm::vec2 uv = glm::vec2(x + 0.5f, y + 0.5f) * uvMult - 1.0f;
            Ray_t ray = camera.CreateRay(uv);
            size_t bestIndex = -1;
            RayHit bestResult;
            bestResult.t = INFINITY;
            for (size_t i = 0; i < objects.size(); ++i) {
                RayHit hitResult;
                if (objects[i].hit(ray, hitResult)) {
                    if (hitResult.t < bestResult.t) {
                        bestResult = hitResult;
                        bestIndex = i;
                    }
                }
            }

            if (bestIndex != -1) {

                glm::vec3 norm = glm::normalize(bestResult.hitNormal);
                glm::vec3 lightDir = glm::normalize(light.m_pLightPos - bestResult.hitPoint);
                
                float diff = glm::max(glm::dot(norm, lightDir), 0.0f);
                glm::vec3 diffuse = light.m_pLightColor * (diff * objects[bestIndex].m_pMaterial.diffuse);

                glm::vec3 viewDir = glm::normalize(ray.origin - bestResult.hitPoint);
                glm::vec3 reflectDir = glm::reflect(-lightDir, norm);
                float spec = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), objects[bestIndex].m_pMaterial.shininess);
                glm::vec3 specular = light.m_pLightColor * (spec * objects[bestIndex].m_pMaterial.specular);
    
                glm::vec3 result = (objects[bestIndex].m_pMaterial.ambient * light.m_pLightColor) + diffuse + specular;

                result = glm::clamp(result, 0.0f, 1.0f);
                
                //SDL_SetRenderDrawColor(renderer, (int) (result.x * 255.999), (int) (result.y * 255.999), (int) (result.z * 255.999), SDL_ALPHA_OPAQUE);

                //if (SDL_RenderDrawPoint(renderer, x, y) != 0) {
                    //std::cerr << "IDK" << std::endl;
                //}
                
                //std::cout << (int)(result.x * 255.999)<< ' ' << (int)(result.y * 255.999) << ' ' << (int)(result.z * 255.999) << '\n';
            } else {
                //SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                //std::cout << 255 << ' ' << 255 << ' ' << 255 << '\n';
            }*/
            
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            VectorUchar color = colors[y * width + x];
            std::cout << (int) color.x << ' ' << (int) color.y << ' ' << (int) color.z << '\n';
        }
    }
}
#include <chrono>
int main() {


    const uint32_t width = 512;
    const uint32_t height = 512;
    const double aspectRatio = width / height;
    
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    
    SDL_Window* window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

    if (!window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return -1;
    }

    Material material1(2.0f, glm::vec3(1.0f, 0.5f, 0.01f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    Material material2(0.02f, glm::vec3(0.30f, 0.15f, 0.75f), glm::vec3(0.75f, 0.25f, 0.5f), glm::vec3(0.1f, 0.2f, 0.25f));

    std::cout << "P3\n" << width << ' ' << height << '\n' << 255 << std::endl;
    glm::vec4 clearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
    Sphere sphere(material1, glm::vec3(0.0f, 0.0f, -15.0f), 4.0f);
    glm::vec2 uvMult = 2.0f / glm::vec2(width, height);

    Scene* scene = new Scene();
    
    Camera camera(glm::vec3(0.0f), cameraFront, cameraUp,45.0f, aspectRatio, 0.1f, 100.0f);
    //glm::vec3 sphereColor(1.0f, 0.5f, 0.01f);
    //glm::vec3 sphere2Color(0.0f, 0.5f, 0.5f);
    glm::vec3 lightPos(20.0f, -20.0f, 0.0f);
    glm::vec3 lightColor(0.5f, 0.2f, 0.0f);
    //glm::vec3 diffuseColor(0.5f, 0.0f, 0.0f);
    //glm::vec3 specColor(1.0f, 1.0f, 1.0f);
    //const float shininess = 16.0f;
    float lightPower;


    Light light(lightPos, lightColor);
    scene->addLight(light);
    
    Sphere sphere2(material2, glm::vec3(2.0f, 2.0f, -20.0f), 4.0f);
    std::vector<Sphere> spheres = {sphere, sphere2};
    scene->addObject(&sphere);
    scene->addObject(&sphere2);

    auto start = std::chrono::high_resolution_clock::now();
    //for (int i = 0; i < 5; i++) {
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    //SDL_RenderClear(renderer);
    
    render(width, height, renderer, scene, camera);
    //camera.Move(glm::vec3(0.0f, 1.0f, 0.0f));
    //spheres[0].m_pRadius += 0.25f;
    //SDL_RenderPresent(renderer);

    //SDL_Delay(2);
    //}

    auto stop = std::chrono::high_resolution_clock::now();

    delete scene;

    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << std::endl;

    std::cout << std::flush;
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
        if (window) {
            SDL_DestroyWindow(window);
        }

    SDL_Quit();

    return 0;
}
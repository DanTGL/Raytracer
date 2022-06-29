#pragma once

#include <vector>
#include "sphere.h"
#include "light.h"
#include "ray.h"
#include "camera.h"

#define MAX_DEPTH 2

class Scene {
public:
    Scene();

    glm::vec3 raytrace(Ray_t* ray, int depth=0);

    void addObject(HittableObject* obj);
    void addLight(Light light);
    
protected:
    std::vector<HittableObject*> m_pObjects;
    std::vector<Light> m_pLights;
};
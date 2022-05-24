#pragma once

#include <vector>
#include "sphere.h"
#include "light.h"
#include "ray.h"
#include "camera.h"

class Scene {
public:
    Scene();

    glm::vec3 raytrace(Camera camera, glm::vec2 uv);

    void addObject(HittableObject* obj);
    void addLight(Light light);
    
protected:
    std::vector<HittableObject*> m_pObjects;
    std::vector<Light> m_pLights;
};
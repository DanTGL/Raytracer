#pragma once

#include <glm/glm.hpp>
#include "ray.h"
#include "material.h"

struct RayHit {
    glm::vec3 hitNormal;
    glm::vec3 hitPoint;
    double t;
};

class HittableObject {
public:
    virtual bool hit(Ray &ray, RayHit &hitResult) = 0;

    glm::vec3 m_pPosition;
    Material m_pMaterial;
    virtual ~HittableObject() {}
};


#pragma once

#include "hittable.h"


class Sphere : public HittableObject {
public:
    Sphere(Material material, glm::vec3 pos, float radius);
    virtual bool hit(Ray &ray, RayHit &hitResult);
    float m_pRadius;
};
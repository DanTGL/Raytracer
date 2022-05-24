#include "sphere.h"

#include <glm/gtx/norm.hpp>
#include <iostream>

Sphere::Sphere(Material material, glm::vec3 pos, float radius) {
    m_pMaterial = material;
    m_pPosition = pos;
    m_pRadius = radius;
}

bool Sphere::hit(Ray &ray, RayHit &hitResult) {
    
    double den = glm::pow(glm::dot(ray.dir, (ray.origin - m_pPosition)), 2) - glm::distance2(ray.origin, m_pPosition) + glm::pow(m_pRadius, 2);

    if (den < 0) {
        //std::cerr << den << std::endl;
        return false;
    }
    

    glm::vec3 oc = ray.origin - m_pPosition;
    double a = glm::length2(ray.dir);
    double term1 = -glm::dot(ray.dir, (ray.origin - m_pPosition));

    float t = term1 - glm::sqrt(term1);

    hitResult.t = t;
    hitResult.hitPoint = ray.origin + (t * ray.dir);
    hitResult.hitNormal = -(hitResult.hitPoint - m_pPosition) / m_pRadius;
    

    return true;
}
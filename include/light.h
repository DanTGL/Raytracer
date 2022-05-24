#pragma once

#include <glm/glm.hpp>

class Light {
public:
    glm::vec3 m_pLightPos;
    glm::vec3 m_pLightColor;
    
    Light(glm::vec3 lightPos, glm::vec3 lightColor) : m_pLightPos(lightPos), m_pLightColor(lightColor) {}
};
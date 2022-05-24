#pragma once

#include <glm/glm.hpp>

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    Material() {}
    Material(float shininess, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : shininess(shininess), ambient(ambient), diffuse(diffuse), specular(specular) {}
};
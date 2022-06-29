#include "scene.h"

Scene::Scene() {
}

void Scene::addObject(HittableObject* obj) {
    m_pObjects.push_back(obj);
}

void Scene::addLight(Light light) {
    m_pLights.push_back(light);
}

glm::vec3 Scene::raytrace(Ray_t* ray, int depth) {
    if (depth >= MAX_DEPTH) return glm::vec3(0.0);
    
    size_t bestIndex = -1;
    RayHit bestResult;
    bestResult.t = INFINITY;
    for (size_t i = 0; i < m_pObjects.size(); ++i) {
        RayHit hitResult;
        if (m_pObjects[i]->hit(*ray, hitResult)) {
            if (hitResult.t < bestResult.t) {
                bestResult = hitResult;
                bestIndex = i;
            }
        }
    }

    if (bestIndex != -1) {

        glm::vec3 norm = glm::normalize(bestResult.hitNormal);
        glm::vec3 lightDir = glm::normalize(m_pLights[0].m_pLightPos - bestResult.hitPoint);

        float diff = glm::max(glm::dot(norm, lightDir), 0.0f);
        glm::vec3 diffuse = m_pLights[0].m_pLightColor * (diff * m_pObjects[bestIndex]->m_pMaterial.diffuse);

        glm::vec3 viewDir = glm::normalize(ray->origin - bestResult.hitPoint);
        glm::vec3 reflectDir = glm::reflect(-lightDir, norm);
        float spec = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), m_pObjects[bestIndex]->m_pMaterial.shininess);
        glm::vec3 specular = m_pLights[0].m_pLightColor * (spec * m_pObjects[bestIndex]->m_pMaterial.specular);

        glm::vec3 result = (m_pObjects[bestIndex]->m_pMaterial.ambient * m_pLights[0].m_pLightColor) + diffuse + specular;

        result = glm::clamp(result, 0.0f, 1.0f);

        return result;

        //SDL_SetRenderDrawColor(renderer, (int) (result.x * 255.999), (int) (result.y * 255.999), (int) (result.z * 255.999), SDL_ALPHA_OPAQUE);

        //if (SDL_RenderDrawPoint(renderer, x, y) != 0) {
            //std::cerr << "IDK" << std::endl;
        //}

        //std::cout << (int)(result.x * 255.999) << ' ' << (int)(result.y * 255.999) << ' ' << (int)(result.z * 255.999) << '\n';
    } else {
        //SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        return glm::vec3(1.0, 1.0, 1.0);
        //std::cout << 255 << ' ' << 255 << ' ' << 255 << '\n';
    }
}
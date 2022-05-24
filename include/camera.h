#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ray.h"

class Camera {
public:
    Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float fovDeg, float aspectRatio, float near, float far);

    void RefreshAspectRatio(float newAspectRatio);
    void RefreshView();

    Ray CreateRay(glm::vec2 uv);

    void Move(glm::vec3 delta);

private:
    glm::mat4 m_pView;
    glm::mat4 m_pProjectionInverse;
    glm::mat4 m_pCameraToWorld;
    glm::vec3 m_pPos;
    glm::vec3 m_pFront;
    glm::vec3 m_pUp;
    float m_pFov;
    float m_pNear;
    float m_pFar;
    glm::vec3 m_pDir;


    glm::vec3 m_pRayOrigin;
};
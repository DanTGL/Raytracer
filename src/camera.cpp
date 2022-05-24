#include "camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float fovDeg, float aspectRatio, float near, float far) {
    m_pFov = glm::radians(fovDeg);
    m_pNear = near;
    m_pFar = far;

    m_pPos = pos;
    m_pFront = front;
    m_pUp = up;

    m_pProjectionInverse = glm::inverse(glm::perspective(m_pFov, aspectRatio, near, far));
    RefreshView();
}

void Camera::RefreshAspectRatio(float newAspectRatio) {
    m_pProjectionInverse = glm::inverse(glm::perspective(m_pFov, newAspectRatio, m_pNear, m_pFar));
    m_pCameraToWorld = m_pProjectionInverse * glm::inverse(m_pView);
}

void Camera::RefreshView() {
    m_pDir = m_pPos + m_pFront;
    m_pView = glm::lookAt(m_pPos, m_pDir, m_pUp);
    m_pCameraToWorld = m_pProjectionInverse * glm::inverse(m_pView);

    m_pRayOrigin = glm::vec3((m_pProjectionInverse * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) * glm::inverse(m_pView));

}

Ray Camera::CreateRay(glm::vec2 uv) {
    Ray_t ray;
    
    ray.origin = m_pRayOrigin;
    glm::vec3 direction = m_pProjectionInverse * glm::vec4(uv, 0.0f, 1.0f);
    ray.dir = glm::normalize(glm::vec3(glm::inverse(m_pView) * glm::vec4(direction, 0.0f)));

    return ray;
}

void Camera::Move(glm::vec3 delta) {
    this->m_pPos += delta;
    this->m_pDir -= 0.2f;
    this->RefreshView();
}

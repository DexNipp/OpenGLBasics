#include "Camera.h"

Camera::Camera(float fov, float aspectRatio, float near, float far) 
	: m_FOV(fov), m_AspectRatio(aspectRatio), m_Near(near), m_Far(far), m_Projection(glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far)) 
{
}

void Camera::TranslateView(const glm::vec3 trans) {
	m_View = glm::translate(m_View, trans);
}

void Camera::ViewLookAt(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
	m_View = glm::lookAt(v1, v2, v3);
}

void Camera::SetPosition(const glm::vec3 position) {
	m_Position = position;
}

void Camera::SetFOV(const float fov) {
	m_FOV = fov;
	RecalculateProjection();
}

void Camera::SetAspectRatio(const float aspectRation) {
	m_AspectRatio = aspectRation;
	RecalculateProjection();
}

void Camera::SetNear(const float near) {
	m_Near = near;
	RecalculateProjection();
}

void Camera::SetFar(const float far) {
	m_Far = far;
	RecalculateProjection();
}

void Camera::RecalculateProjection() {

	m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
}
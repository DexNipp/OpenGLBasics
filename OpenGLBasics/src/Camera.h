#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:
	Camera() = default;
	Camera(float fov, float aspectRatio, float near, float far);

	virtual ~Camera() = default;

	const glm::mat4 GetProjection() const { return m_Projection; }
	const glm::mat4 GetView() const { return m_View; }
	void TranslateView(glm::vec3 trans);
	void ViewLookAt(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
	glm::mat4 GetViewProjection() const { return m_Projection * m_View; }

	void SetPosition(const glm::vec3 position);
	glm::vec3 GetPosition() const { return m_Position; }

	
	void SetFOV(const float fov);
	void SetAspectRatio(const float aspectRation);
	void SetNear(const float near);
	void SetFar(const float far);

private:
	void RecalculateProjection();

private:
	float m_FOV, m_AspectRatio, m_Near, m_Far;
	
	glm::mat4 m_Projection = glm::mat4(1.0f);
	glm::mat4 m_View = glm::mat4(1.0f);

	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

	float m_Pitch = 0.0f, m_Yaw = 0.0f;
};
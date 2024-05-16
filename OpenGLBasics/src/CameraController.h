#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>
#include "Camera.h"
#include <GLFW/glfw3.h>

class CameraController {
public:
    CameraController(Camera& camera, float movementSpeed = 10.5f, float mouseSensitivity = 0.1f)
        : m_Camera(camera), m_MovementSpeed(movementSpeed), m_MouseSensitivity(mouseSensitivity)
        {
            m_LastX = 1920.0f / 2.0f;
            m_LastY = 1080.0f / 2.0f;
            m_FirstMouse = true;
        }

    void ProcessKeyboardInput(GLFWwindow* window, float deltaTime) {

        float velocity = m_MovementSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            m_Camera.Position += m_Camera.Front * velocity;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            m_Camera.Position -= m_Camera.Front * velocity;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            m_Camera.Position -= m_Camera.Right * velocity;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            m_Camera.Position += m_Camera.Right * velocity;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            m_Camera.Position += m_Camera.Up * velocity;
    }

    void ProcessMouseMovement(double win_xpos, double win_ypos) {

        float xpos = static_cast<float>(win_xpos);
        float ypos = static_cast<float>(win_ypos);
        if (m_FirstMouse)
        {
            m_LastX = xpos;
            m_LastY = ypos;
            m_FirstMouse = false;
        }

        float xoffset = xpos - m_LastX;
        float yoffset = m_LastY - ypos; // reversed since y-coordinates go from bottom to top

        m_LastX = xpos;
        m_LastY = ypos;

        xoffset *= m_MouseSensitivity;
        yoffset *= m_MouseSensitivity;

        m_Camera.Yaw += xoffset;
        m_Camera.Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        
        if (m_Camera.Pitch > 89.0f)
            m_Camera.Pitch = 89.0f;
        else if (m_Camera.Pitch < -89.0f)
            m_Camera.Pitch = -89.0f;
        

        // update Front, Right and Up Vectors using the updated Euler angles
        m_Camera.UpdateCameraVectors();
    }

    void ProcessMouseScroll(double yoffset) {

        m_Camera.Fov -= (float)yoffset;
        if (m_Camera.Fov < 1.0f)
            m_Camera.Fov = 1.0f;
        if (m_Camera.Fov > 120.0f)
            m_Camera.Fov = 120.0f;
    }

private:
    Camera& m_Camera;
    float m_MovementSpeed;
    float m_MouseSensitivity;
    float m_FirstMouse;
    float m_LastX;
    float m_LastY;
};
#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>
#include "glm/gtc/matrix_transform.hpp"

struct Axis {
    static constexpr glm::vec3 PositiveX{  1,  0,  0 };
    static constexpr glm::vec3 NegativeX{ -1,  0,  0 };
    static constexpr glm::vec3 PositiveY{  0,  1,  0 };
    static constexpr glm::vec3 NegativeY{  0, -1,  0 };
    static constexpr glm::vec3 PositiveZ{  0,  0,  1 };
    static constexpr glm::vec3 NegativeZ{  0,  0, -1 };
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;

// TODO: Make Camera able to maintain itself in main loop. Add setters and getters for private vars and OnUpdate 

class Camera {
public:
 
    float Fov;
    float AspectRatio;
    float zFar;
    float zNear;

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Right;
    glm::vec3 Up;
    glm::vec3 WorldUp;

    // euler Angles
    float Yaw;
    float Pitch;

    Camera(float fov, float aspectRatio, float zFar, float zNear)
        : Fov(fov), AspectRatio(aspectRatio), zFar(zFar), zNear(zNear), Position(0.0f),
          Yaw(YAW), Pitch(PITCH), Front(Axis::NegativeZ), WorldUp(Axis::PositiveY)
    {
       
        UpdateCameraVectors();
    }

    glm::mat4 getProjectionMatrix() const {
        return glm::perspective(glm::radians(Fov), AspectRatio, zNear, zFar);
    }

    inline glm::mat4 getViewMatrix() const {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void UpdateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
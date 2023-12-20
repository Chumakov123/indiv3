#pragma once
#include <vector>

#include <glm/glm.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;

class Camera
{
public:
 
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
       
    glm::mat4 GetViewMatrix();

    glm::vec3 GetPosition();
 
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    void ProcessMouseMovement(float xpos, float ypos, bool constrainPitch = true);
  
private:

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float lastX = 450, lastY = 450;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    void updateCameraVectors();
};
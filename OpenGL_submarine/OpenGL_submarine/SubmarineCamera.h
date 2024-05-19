#pragma once
#ifndef SUBMARINECAMERA_H
#define SUBMARINECAMERA_H
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Submarine;

class SubmarineCamera
{
public:
    SubmarineCamera(Submarine* submarine, float distanceFromTarget, float pitch, float yaw);

    

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    glm::vec3 GetPosition();
    void updatePosition();
private:
    Submarine* submarine;
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float distanceFromTarget;
    float pitch;
    float yaw;
};
#endif

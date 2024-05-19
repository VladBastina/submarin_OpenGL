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

    

    glm::mat4 GetViewMatrix() ;
     glm::mat4 GetProjectionMatrix(float aspectRatio) const;
    glm::vec3 GetPosition() const;
    void updatePosition();
    void SetDistanceFromTarget(float distance);
    void SetPitch(float pitch);
    void SetYaw(float yaw);
    void calculateZoom(float yOffset);
    void MouseControl(float xpos, float ypos);
private:
    glm::vec3 calculateNewPoint(glm::vec3 startPoint, glm::vec3 direction, float distance);
    bool thirdPerson;
    Submarine* submarine;
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float distanceFromTarget;
    float angle;
    float pitch;
    float yaw;
    float roll;
};
#endif

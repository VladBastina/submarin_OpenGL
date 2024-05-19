#include "SubmarineCamera.h"
#include "Submarine.h"


SubmarineCamera::SubmarineCamera(Submarine* submarine, float distanceFromTarget, float pitch, float yaw)
    : submarine(submarine), distanceFromTarget(distanceFromTarget), pitch(pitch), yaw(yaw), up(glm::vec3(0.0f, 1.0f, 0.0f))
{
    updatePosition();
}
glm::mat4 SubmarineCamera::GetViewMatrix() const
{
    return glm::lookAt(position, target, up);
}

glm::mat4 SubmarineCamera::GetProjectionMatrix() const
{
    return glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
}

glm::vec3 SubmarineCamera::GetPosition()
{
    return position;
}
void SubmarineCamera::updatePosition()
{
    float horizontalDistance = distanceFromTarget * cos(glm::radians(pitch));
    float verticalDistance = distanceFromTarget * sin(glm::radians(pitch));
    float offsetX = horizontalDistance * sin(glm::radians(yaw + submarine->RotationYaw()));
    float offsetZ = horizontalDistance * cos(glm::radians(yaw + submarine->RotationYaw()));
    position = glm::vec3(submarine->GetPosition().x - offsetX, submarine->GetPosition().y + verticalDistance, submarine->GetPosition().z - offsetZ);
    target = submarine->GetPosition();
}
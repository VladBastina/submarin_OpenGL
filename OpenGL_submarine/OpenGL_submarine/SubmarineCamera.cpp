#include "SubmarineCamera.h"
#include "Submarine.h"


SubmarineCamera::SubmarineCamera(Submarine* submarine, float distanceFromTarget, float pitch, float yaw)
    : submarine(submarine), distanceFromTarget(distanceFromTarget),angle(0.0f), pitch(pitch), yaw(yaw), up(glm::vec3(0.0f, 1.0f, 0.0f))
{
    updatePosition();
}
glm::mat4 SubmarineCamera::GetViewMatrix() {
    return glm::lookAt(position, target, up);
}

glm::mat4 SubmarineCamera::GetProjectionMatrix(float aspectRatio) const
{
    return glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 500.0f);
}

glm::vec3 SubmarineCamera::GetPosition() const
{
    return position;
}
void SubmarineCamera::updatePosition()
{
   /* float horizontalDistance = distanceFromTarget * cos(glm::radians(pitch));
    float verticalDistance = distanceFromTarget * sin(glm::radians(pitch));
    float offsetX = horizontalDistance * sin(glm::radians(yaw+submarine->RotationYaw()));
    float offsetZ = horizontalDistance * cos(glm::radians(yaw+submarine->RotationYaw()));
    position = submarine->GetPosition() - glm::vec3(offsetX, verticalDistance, offsetZ);
    target = submarine->GetPosition();*/

    glm::vec3 direction = submarine->Direction();

    float distance = 5.0f;

    position = calculateNewPoint(submarine->GetPosition(), direction, distance);
    target = submarine->GetPosition();
}
void SubmarineCamera::SetDistanceFromTarget(float distance)
{
    distanceFromTarget = distance;
    updatePosition();
}

void SubmarineCamera::SetPitch(float pitch)
{
    this->pitch = pitch;
    updatePosition();
}

void SubmarineCamera::SetYaw(float yaw)
{
    this->yaw = yaw;
    updatePosition();
}

void SubmarineCamera::calculateZoom(float yOffset)
{
    if (distanceFromTarget > 20.0f)
        distanceFromTarget = 20.0f;
    else if (distanceFromTarget < 10.0f)
        distanceFromTarget = 10.0f;
    else
    {
        float zoomLevel = yOffset;
        distanceFromTarget -= zoomLevel;
    }
}

void SubmarineCamera::MouseControl(float xpos, float ypos)
{
}

glm::vec3 SubmarineCamera::calculateNewPoint(glm::vec3 startPoint, glm::vec3 direction, float distance) {
    glm::vec3 newPoint = startPoint - direction * distance*5.0f;
    return newPoint;
}
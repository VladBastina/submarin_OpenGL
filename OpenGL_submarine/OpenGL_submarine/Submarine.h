#pragma once
#ifndef SUBMARINE_H
#define SUBMARINE_H
#define NOMINMAX 1
#include <Windows.h>
#include <locale>
#include <codecvt>
#include "Model.h"
#include "Camera.cpp"
class SubmarineCamera;

const float maxSpeed = 10.0f;
const float accelerationRate = 0.5f;
const float decelerationRate = 0.5f;
const float rotationSpeed = 30.0f;
const float rollAngle = 10.0f;
const float maxPitchAngle = 20.0f;
const float pitchRate = 5.0f;
const float verticalSpeedModifier = 0.5f;
const float verticalSpeed = 5.0f;
const float maxDepth = -45.0f;
const float surfaceLevel = 0.0f;

class Submarine
{
public:
    Submarine();

    void ProcessInput(GLFWwindow* window, float deltaTime,SubmarineCamera* camera);
    void Render(SubmarineCamera* camera,float aspectRatio);

    glm::vec3 GetPosition() const;
    float getRotationPitch()const;
    float RotationYaw() const;
    float RotationPitch() const;
    glm::vec3 Direction();
private:
    glm::vec3 direction;
    glm::vec3 lastPosition;
    Shader* submarineShader;
    Model submarine;
    glm::vec3 position;
    float rotationYaw, rotationPitch, acceleration, currentSpeed, rotationRoll;
};
#endif
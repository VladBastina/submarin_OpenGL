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
const float maxDepth = -100.0f;
const float surfaceLevel = 0.0f;

class Submarine
{
public:
    Submarine();

    void ProcessInput(GLFWwindow* window, float deltaTime);
    void Render(SubmarineCamera* camera);

    glm::vec3 GetPosition() const;
    float RotationYaw() const;
    float RotationPitch() const;

private:
    Shader* submarineShader;
    Model submarine;
    glm::vec3 position;
    float rotationYaw, rotationPitch, acceleration, currentSpeed, rotationRoll;
};
#endif
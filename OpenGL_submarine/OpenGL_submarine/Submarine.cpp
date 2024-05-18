#pragma once

#include <Windows.h>
#include <locale>
#include <codecvt>
#include "Camera.cpp"
#include "Model.h"


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
	Model submarine;
	Submarine():position(glm::vec3(0.0f,0.0f,0.0f)), rotationYaw(0.0f), rotationPitch(0.0f),acceleration(0.0f),currentSpeed(0.0f),rotationRoll(0.0f)
	{
		wchar_t buffer[MAX_PATH];
		GetCurrentDirectoryW(MAX_PATH, buffer);

		std::wstring executablePath(buffer);
		std::wstring wscurrentPath = executablePath.substr(0, executablePath.find_last_of(L"\\/"));

		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::string currentPath = converter.to_bytes(wscurrentPath);
		submarineShader = new Shader((currentPath + "\\Shaders\\PhongLight.vs").c_str(), (currentPath + "\\Shaders\\PhongLight.fs").c_str());
		std::string submarineObjFileName = (currentPath + "\\Models\\Submarine\\submarine.obj");
		submarine = Model(submarineObjFileName);
	}

	void ProcessInput(GLFWwindow* window, float deltaTime)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			acceleration += accelerationRate * deltaTime;
			currentSpeed += acceleration * deltaTime;

			if (currentSpeed > maxSpeed)
				currentSpeed = maxSpeed;
		}
		else
		{
			if (currentSpeed > 0.0f)
			{
				currentSpeed -= decelerationRate * deltaTime;
				if (currentSpeed < 0.0f)
				{
					currentSpeed = 0.0f;
					acceleration = 0.0f;
				}
			}
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			rotationYaw -= rotationSpeed * deltaTime;
			rotationRoll = glm::mix(rotationRoll, -rollAngle, 0.1f);

		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			rotationYaw += rotationSpeed * deltaTime;
			rotationRoll = glm::mix(rotationRoll, rollAngle, 0.1f);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
		{
			rotationRoll = glm::mix(rotationRoll, 0.0f, 0.1f);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			if (rotationPitch > -maxPitchAngle)
				rotationPitch -= pitchRate * deltaTime;
		}
		else if (rotationPitch < 0.0f)
		{
			rotationPitch += pitchRate * deltaTime;
		}

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			if (rotationPitch < maxPitchAngle)
				rotationPitch += pitchRate * deltaTime;
		}
		else if (rotationPitch > 0.0f)
		{
			rotationPitch -= pitchRate * deltaTime;
		}

		if (currentSpeed > 0.0f)
		{
			glm::vec3 forwardMovement = glm::vec3(sin(glm::radians(rotationYaw)), 0.0f, cos(glm::radians(rotationYaw))) * currentSpeed * deltaTime;
			glm::vec3 verticalMovement = glm::vec3(0.0f, -sin(glm::radians(rotationPitch))*  5.0f* currentSpeed * deltaTime, 0.0f);

			position += forwardMovement + verticalMovement;

			if (position.y > surfaceLevel)
				position.y = surfaceLevel;
			if (position.y < maxDepth)
				position.y = maxDepth;
		}
	}

	void Render(Camera* pCamera)
	{
		submarineShader->Use();
		submarineShader->SetMat4("projection", pCamera->GetProjectionMatrix());
		submarineShader->SetMat4("view", pCamera->GetViewMatrix());
		glm::mat4 submarineModel = glm::mat4(1.0f);
		submarineModel = glm::translate(submarineModel, position);
		submarineModel = glm::rotate(submarineModel, glm::radians(rotationYaw), glm::vec3(0.0f, 1.0f, 0.0f));
		submarineModel = glm::rotate(submarineModel, glm::radians(rotationPitch), glm::vec3(1.0f, 0.0f, 0.0f));
		submarineModel = glm::rotate(submarineModel, glm::radians(rotationRoll), glm::vec3(0.0f, 0.0f, 1.0f));
		submarineShader->SetMat4("model", submarineModel);
		submarine.Draw(submarineShader);


	}
private:
	Shader* submarineShader;
	float rotationYaw, rotationPitch,acceleration, currentSpeed,rotationRoll;
	glm::vec3 position;
	


};

#pragma once

#include <Windows.h>
#include <locale>
#include <codecvt>
#include "Camera.cpp"
#include "Model.h"

class Submarine
{
public:
	Model submarine;
	Submarine()
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

	/*public void move()
	{
		checkInput();

	}
	public increasePosition(float dx, float dy, float dz)
	{
		this->increasePosition=
	}*/
	void Render(Camera* pCamera)
	{
		submarineShader->Use();
		submarineShader->SetMat4("projection", pCamera->GetProjectionMatrix());
		submarineShader->SetMat4("view", pCamera->GetViewMatrix());
		glm::mat4 submarineModel = glm::scale(glm::mat4(1.0), glm::vec3(1.f));
		submarineShader->SetMat4("model", submarineModel);
		submarine.Draw(submarineShader);


	}
private:
	Shader* submarineShader;

	/*const float speed = 20;
	const float turnSpeed = 5;
	float currentSpeed = 0;
	float currentTurn = 0;
	float rotX, rotY, rotZ;*/

	/*void checkInput()
	{
		float velocity = (float)(cameraSpeedFactor * deltaTime);
		switch (direction) {
		case EMovementSubmarineType::FORWARD:
			this->currentSpeed += speed;
			break;
		case EMovementSubmarineType::ROLLLEFT:
			position -= forward * velocity;
			break;
		case EMovementSubmarineType::ROLLRIGHT:
			position -= right * velocity;
			break;
		case EMovementSubmarineType::DOWN:
			position += right * velocity;
			break;
		case EMovementSubmarineType::UP:
			position += up * velocity;
			break;
		case ECameraMovementType::DOWN:
			position -= up * velocity;
			break;
		}
	}*/


};

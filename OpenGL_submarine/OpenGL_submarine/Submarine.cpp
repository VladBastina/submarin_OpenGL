#include "Submarine.h"
#include "SubmarineCamera.h"
	Submarine::Submarine() :position(glm::vec3(0.0f, 0.0f, 0.0f)), rotationYaw(0.0f), rotationPitch(0.0f), acceleration(0.0f), currentSpeed(0.0f), rotationRoll(0.0f),
		lastPosition(glm::vec3(0.0f, 0.0f, 0.5f))
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

	void Submarine::ProcessInput(GLFWwindow* window, float deltaTime, SubmarineCamera* camera)
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
			if (rotationPitch < -maxPitchAngle)
				rotationPitch = maxPitchAngle;
			rotationPitch -= pitchRate * deltaTime;
		}
		else if (rotationPitch < 0.0f)
		{
			if (rotationPitch > 0.0f)
				rotationPitch = 0.0f;
			rotationPitch += pitchRate * deltaTime;
		}

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{

			if (rotationPitch > maxPitchAngle)
				rotationPitch = maxPitchAngle;
			rotationPitch += pitchRate * deltaTime;

		}
		else if (rotationPitch > 0.0f)
		{
			if (rotationPitch < 0.0f)
				rotationPitch = 0.0f;
			rotationPitch -= pitchRate * deltaTime;
		}

		if (currentSpeed > 0.0f)
		{
			glm::vec3 forwardMovement = glm::vec3(sin(glm::radians(rotationYaw)), 0.0f, cos(glm::radians(rotationYaw))) * currentSpeed * deltaTime;
			glm::vec3 verticalMovement = glm::vec3(0.0f, -sin(glm::radians(rotationPitch)) * 1.0f * currentSpeed * deltaTime, 0.0f);

			position += forwardMovement + verticalMovement;

			if (position.y > surfaceLevel)
				position.y = surfaceLevel;
			if (position.y < maxDepth)
				position.y = maxDepth;

			if (position.x > 166.0f || position.x < -166.0f || position.z < -166.0f || position.z > 166.0f)
			{
				position = glm::vec3(0.0f, position.y, 0.0f);
			}
		}
		
	}
	/*public increasePosition(float dx, float dy, float dz)
	{
		this->increasePosition=
	}*/

	void Submarine::Render(SubmarineCamera* camera,float aspectRatio,float mixValue)
	{
		submarineShader->Use();
		submarineShader->SetMat4("projection", camera->GetProjectionMatrix(aspectRatio));
		submarineShader->SetMat4("view", camera->GetViewMatrix());
		glm::mat4 submarineModel = glm::mat4(1.0f);
		submarineModel = glm::translate(submarineModel,position);
		submarineModel = glm::rotate(submarineModel, glm::radians(rotationYaw), glm::vec3(0.0f, 1.0f, 0.0f));
		submarineModel = glm::rotate(submarineModel, glm::radians(rotationPitch), glm::vec3(1.0f, 0.0f, 0.0f));
		submarineModel = glm::rotate(submarineModel, glm::radians(rotationRoll), glm::vec3(0.0f, 0.0f, 1.0f));
		submarineShader->SetMat4("model", submarineModel);
		//submarineShader->SetVec3("lightPos", lightPos);
		submarineShader->SetVec3("lightColor", glm::vec3(0.0f, 0.0f, 0.0f));
		//submarineShader->SetVec3("viewPos", pCamera->GetPosition());
		submarineShader->SetFloat("ambientReflection", 0.3f);
		submarineShader->SetFloat("diffuseConstant", 1.0f);
		submarineShader->SetFloat("specularConstant", 1.0f);
		submarineShader->SetFloat("shininess", 1000.0f);
		submarineShader->SetVec3("fogColor", glm::vec3(0.0f, 0.12f, 0.25f));
		submarineShader->SetFloat("fogDensity", 0.0f);
		submarineShader->SetFloat("mixValue", mixValue);
		submarine.Draw(submarineShader);
	}
	glm::vec3 Submarine::GetPosition() const
	{
		return position;
	}

	float Submarine::getRotationPitch() const
	{
		return rotationPitch;
	}

	float Submarine::RotationYaw() const
	{
		return rotationYaw;
	}

	float Submarine::RotationPitch() const
	{
		return rotationPitch;
	}

	glm::vec3 Submarine::Direction()
	{
		if (lastPosition != position)
		{
			direction = glm::normalize(position - lastPosition);
			lastPosition = position;
		}
		return direction;

	}
	
	


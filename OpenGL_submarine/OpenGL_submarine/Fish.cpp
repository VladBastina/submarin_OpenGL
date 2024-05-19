#include "Fish.h"
#include "SubmarineCamera.h"

Fish::Fish()
{
	wchar_t buffer[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, buffer);

	std::wstring executablePath(buffer);
	std::wstring wscurrentPath = executablePath.substr(0, executablePath.find_last_of(L"\\/"));

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string currentPath = converter.to_bytes(wscurrentPath);
	fishShader = new Shader((currentPath + "\\Shaders\\PhongLight.vs").c_str(), (currentPath + "\\Shaders\\PhongLight.fs").c_str());
	std::string submarineObjFileName = (currentPath + "\\Models\\Fish\\fish.obj");
	fish = Model(submarineObjFileName);
}

void Fish::Render(SubmarineCamera* camera)
{
	fishShader->Use();
	fishShader->SetMat4("projection", camera->GetProjectionMatrix());
	fishShader->SetMat4("view", camera->GetViewMatrix());
	glm::mat4 fishModel = glm::mat4(1.0f);
	fishShader->SetMat4("model", fishModel);
	//submarineShader->SetVec3("lightPos", lightPos);
	fishShader->SetVec3("lightColor", glm::vec3(1.0f, 0.8f, 0.4f));
	//submarineShader->SetVec3("viewPos", pCamera->GetPosition());
	fishShader->SetFloat("ambientReflection", 0.3f);
	fishShader->SetFloat("diffuseConstant", 1.0f);
	fishShader->SetFloat("specularConstant", 1.0f);
	fishShader->SetFloat("shininess", 1000.0f);
	fish.Draw(fishShader);
}

#include "Fish.h"
#include "Submarine.h"
#include <cstdlib>
#include <ctime>
#include <random>

Fish::Fish() {
    wchar_t buffer[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, buffer);

    std::wstring executablePath(buffer);
    std::wstring wscurrentPath = executablePath.substr(0, executablePath.find_last_of(L"\\/"));

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string currentPath = converter.to_bytes(wscurrentPath);
    fishShader = new Shader((currentPath + "\\Shaders\\PhongLight.vs").c_str(), (currentPath + "\\Shaders\\PhongLight.fs").c_str());
    std::string submarineObjFileName = (currentPath + "\\Models\\Fish\\fish.obj");
    fish = Model(submarineObjFileName);

    submarineObjFileName = (currentPath + "\\Models\\Whale\\Whale.obj");
    whale = Model(submarineObjFileName);
    whaleShader = new Shader((currentPath + "\\Shaders\\PhongLight.vs").c_str(), (currentPath + "\\Shaders\\PhongLight.fs").c_str());
    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    InitializeFish();
}

void Fish::InitializeFish() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distPosX(-200.0f, 200.0f);
    std::uniform_real_distribution<float> distPosY(-40.0f, -10.0f);
    std::uniform_real_distribution<float> distPosZ(-200.0f, 200.0f);
    std::uniform_real_distribution<float> distMove(-0.1f, 0.1f);

    for (int i = 0; i < numFish; ++i) {
        // Generate random position with y < 0.0f
        glm::vec3 randomPos = glm::vec3(distPosX(gen), distPosY(gen), distPosZ(gen));
        fishPositions.push_back(randomPos);

        // Generate random movement vector
        glm::vec3 movementVector = glm::vec3(distMove(gen), distMove(gen), distMove(gen));
        fishMovementVectors.push_back(movementVector);
    }
}

void Fish::Update(float deltaTime) {
    for (int i = 0; i < numFish; ++i) {
        fishPositions[i] += fishMovementVectors[i] * deltaTime;

        // Boundary check to keep fish within a certain range
        if (fishPositions[i].x > 200.0f || fishPositions[i].x < -200.0f) {
            fishMovementVectors[i].x = -fishMovementVectors[i].x;
        }
        if (fishPositions[i].z > 200.0f || fishPositions[i].z < -200.0f) {
            fishMovementVectors[i].z = -fishMovementVectors[i].z;
        }
        if (fishPositions[i].y > -10.0f || fishPositions[i].y < -40.0f) {
            fishMovementVectors[i].y = -fishMovementVectors[i].y;
        }
    }
}

void Fish::Render(SubmarineCamera* camera, float aspectRatio,glm::vec3 lightPos,float mixValue) {
    fishShader->Use();
    fishShader->SetMat4("projection", camera->GetProjectionMatrix(aspectRatio));
    fishShader->SetMat4("view", camera->GetViewMatrix());

    for (int i = 0; i < numFish; ++i) {
        glm::mat4 fishModel = glm::mat4(1.0f);

        // Translate to the fish's position
        fishModel = glm::translate(fishModel, fishPositions[i]);
        fishModel = glm::scale(fishModel, glm::vec3(0.5f, 0.5f, 0.5f));

        // Calculate the rotation based on movement direction
        glm::vec3 direction = glm::normalize(fishMovementVectors[i]);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(up, direction));
        glm::vec3 newUp = glm::cross(direction, right);

        glm::mat4 rotationMatrix = glm::mat4(
            glm::vec4(right, 0.0f),
            glm::vec4(newUp, 0.0f),
            glm::vec4(direction, 0.0f),
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
        );

        fishModel *= rotationMatrix;

        fishShader->SetMat4("model", fishModel);
        fishShader->SetVec3("lightPos", lightPos);
        fishShader->SetVec3("viewPos", camera->GetPosition());
        fishShader->SetVec3("lightColor", glm::vec3(1.0f, 0.8f, 0.4f));
        fishShader->SetFloat("ambientReflection", 0.3f);
        fishShader->SetFloat("diffuseConstant", 1.0f);
        fishShader->SetFloat("specularConstant", 1.0f);
        fishShader->SetFloat("shininess", 1000.0f);
        fishShader->SetVec3("fogColor", glm::vec3(0.0f, 0.12f, 0.25f));
        fishShader->SetFloat("fogDensity", 0.07f);
        fishShader->SetFloat("mixValue", mixValue);
        fish.Draw(fishShader);
    }

    whaleShader->Use();
    whaleShader->SetMat4("projection", camera->GetProjectionMatrix(aspectRatio));
    whaleShader->SetMat4("view", camera->GetViewMatrix());

    glm::mat4 whaleModel = glm::mat4(1.0f);
    whaleModel = glm::translate(whaleModel, glm::vec3(100.0f, -20.0f,50.0f));
    whaleModel = glm::scale(whaleModel, glm::vec3(0.1f, 0.1f, 0.1f));
    whaleModel = glm::rotate(whaleModel, glm::pi<float>() / 2, glm::vec3(-1.0f, 0.0f, 0.0f));
    //whaleModel = glm::rotate(whaleModel, glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));

    whaleShader->SetMat4("model", whaleModel);
    whaleShader->SetVec3("lightPos", lightPos);
    whaleShader->SetVec3("viewPos", camera->GetPosition());
    whaleShader->SetVec3("lightColor", glm::vec3(1.0f, 0.8f, 0.4f));
    whaleShader->SetFloat("ambientReflection", 0.3f);
    whaleShader->SetFloat("diffuseConstant", 1.0f);
    whaleShader->SetFloat("specularConstant", 1.0f);
    whaleShader->SetFloat("shininess", 1000.0f);
    whaleShader->SetVec3("fogColor", glm::vec3(0.0f, 0.12f, 0.25f));
    whaleShader->SetFloat("fogDensity", 0.01f);
    whaleShader->SetFloat("mixValue", mixValue);

    whale.Draw(whaleShader);
}

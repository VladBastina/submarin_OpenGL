#pragma once

#include "SubmarineCamera.h"
#include "Model.h"

class Fish {
public:
    Fish();
    void Render(SubmarineCamera* camera, float aspectRatio, glm::vec3 lightPos,float mixValue);
    void Update(float deltaTime);

private:
    Shader* fishShader;
    Model fish;
    Model whale;
    Shader* whaleShader;
    std::vector<glm::vec3> fishPositions;
    std::vector<glm::vec3> fishMovementVectors;
    const int numFish = 100;
    void InitializeFish();
};



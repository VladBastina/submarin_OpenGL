#include "Shader.h"
#include "Camera.cpp"
#include "GerstnerWave.cpp"
#include "PerlinNoise.cpp"

class Ocean 
{
private:
	std::vector<float> oceanPoints;
	std::vector<unsigned int> indices;
	unsigned int oceanVAO, oceanVBO, oceanEBO;
	Shader* oceanShader;
	PerlinNoise* perlinNoise;
	std::vector<float>terrainPoints;
	std::vector<unsigned int>terrainIndices;
	unsigned int terrainVAO, terrainVBO, terrainEBO;
	Shader* terrainShader;
	std::vector<float>wallPoints;
	std::vector<unsigned int>wallIndices;
	unsigned int wallVAO, wallVBO, wallEBO;
	Shader* wallShader;


public:
	Ocean()
	{
		perlinNoise = new PerlinNoise();

		int numPointsPerSide = 0;

		for (float k = -100.0f; k < 100.0f; k += 0.1f)
		{
			numPointsPerSide++;
			for (float i = -100.0f; i < 100.0f; i += 0.1f)
			{
				oceanPoints.push_back(k);
				oceanPoints.push_back(0.0f);
				oceanPoints.push_back(i);

			}
		}

		int gridSize = numPointsPerSide - 1;

		for (int z = 0; z < gridSize; ++z) {
			for (int x = 0; x < gridSize; ++x) {
				int topLeft = z * numPointsPerSide + x;
				int topRight = topLeft + 1;
				int bottomLeft = (z + 1) * numPointsPerSide + x;
				int bottomRight = bottomLeft + 1;

				indices.push_back(topLeft);
				indices.push_back(bottomRight);
				indices.push_back(bottomLeft);

				indices.push_back(topLeft);
				indices.push_back(topRight);
				indices.push_back(bottomRight);
			}
		}

		glGenVertexArrays(1, &oceanVAO);
		glBindVertexArray(oceanVAO);

		glGenBuffers(1, &oceanVBO);
		glBindBuffer(GL_ARRAY_BUFFER, oceanVBO);
		glBufferData(GL_ARRAY_BUFFER, oceanPoints.size() * sizeof(float), oceanPoints.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &oceanEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, oceanEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

		oceanShader = new Shader("Water2.vs", "Water.fs");

		int numpoints = 0;

		for (float k = -250.0f; k < 250.0f; k += 0.5f)
		{
			numpoints++;
			for (float i = -250.0f; i < 250.0f; i +=0.5f)
			{
				double height = (perlinNoise->noise(k/10, 0.0, i/10))*10.0;
				terrainPoints.push_back(k);
				terrainPoints.push_back(height);
				terrainPoints.push_back(i);

			}
		}

		gridSize = numpoints - 1;

		for (int z = 0; z < gridSize; ++z) {
			for (int x = 0; x < gridSize; ++x) {
				int topLeft = z * numpoints+ x;
				int topRight = topLeft + 1;
				int bottomLeft = (z + 1) * numpoints + x;
				int bottomRight = bottomLeft + 1;

				terrainIndices.push_back(topLeft);
				terrainIndices.push_back(bottomRight);
				terrainIndices.push_back(bottomLeft);

				terrainIndices.push_back(topLeft);
				terrainIndices.push_back(topRight);
				terrainIndices.push_back(bottomRight);
			}
		}

		glGenVertexArrays(1, &terrainVAO);
		glBindVertexArray(terrainVAO);

		glGenBuffers(1, &terrainVBO);
		glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
		glBufferData(GL_ARRAY_BUFFER, terrainPoints.size() * sizeof(float), terrainPoints.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &terrainEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, terrainIndices.size() * sizeof(unsigned int), terrainIndices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

		unsigned int numPointVert = 0, numPointHor = 0;

		
		
		for (float i = -60.0f; i < 2.0f; i += 1.0f)
		{
			numPointVert++;
			for (float j = -105.0f; j < 105.0f; j += 1.0f)
			{
				if (i == -50.0f)
				{
					numPointHor++;
				}
				wallPoints.push_back(j);
				wallPoints.push_back(i);
				wallPoints.push_back(0.0f);
			}
		}

		gridSize = numPointVert - 1;
		unsigned int gridSize2 = numPointHor - 1;

		for (int z = 0; z < gridSize; ++z) {
			for (int x = 0; x < gridSize2; ++x) {
				int topLeft = z * numPointHor + x;
				int topRight = topLeft + 1;
				int bottomLeft = (z + 1) * numPointHor + x;
				int bottomRight = bottomLeft + 1;

				wallIndices.push_back(topLeft);
				wallIndices.push_back(bottomRight);
				wallIndices.push_back(bottomLeft);

				wallIndices.push_back(topLeft);
				wallIndices.push_back(topRight);
				wallIndices.push_back(bottomRight);
			}
		}

		glGenVertexArrays(1, &wallVAO);
		glBindVertexArray(wallVAO);

		glGenBuffers(1, &wallVBO);
		glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
		glBufferData(GL_ARRAY_BUFFER, wallPoints.size() * sizeof(float), wallPoints.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &wallEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, wallIndices.size() * sizeof(unsigned int), wallIndices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

		terrainShader = new Shader("Terrain.vs", "Terrain.fs");
		wallShader = new Shader("Wall.vs", "Wall.fs");
	}

	void RenderOcean(Camera* pCamera, glm::vec3 lightPos, glm::vec3 lightColor,double currentFrame,std::vector<GerstnerWave>waves,unsigned int skyboxTextureID,unsigned int stonesTextureID ,unsigned int causticstextureID,float mixValue)
	{
		glm::mat4 model = glm::mat4(1.0);

		glm::mat4 waterModel = glm::translate(glm::mat4(1.0), glm::vec3(pCamera->GetPosition().x, 0.0, pCamera->GetPosition().z));

		oceanShader->Use();
		glm::mat4 projection = pCamera->GetProjectionMatrix();
		glm::mat4 view = pCamera->GetViewMatrix();
		oceanShader->SetMat4("projection", projection);
		oceanShader->SetMat4("view", view);
		oceanShader->SetFloat("time", float(currentFrame));

		for (int i = 0; i < waves.size(); ++i)
		{
			std::string a = "waves[" + std::to_string(i) + "].amplitude";
			oceanShader->SetFloat(a, waves[i].amplitude);
			std::string b = "waves[" + std::to_string(i) + "].wavelength";
			oceanShader->SetFloat(b, waves[i].wavelength);
			std::string c = "waves[" + std::to_string(i) + "].speed";
			oceanShader->SetFloat(c, waves[i].speed);
			std::string d = "waves[" + std::to_string(i) + "].direction";
			oceanShader->SetFloat(d, waves[i].direction);
			std::string e = "waves[" + std::to_string(i) + "].phase";
			oceanShader->SetFloat(e, waves[i].phase);
		}

		oceanShader->SetVec3("lightPos", lightPos);
		oceanShader->SetVec3("lightColor", glm::vec3(1.0f, 0.8f, 0.4f));
		oceanShader->SetVec3("viewPos", pCamera->GetPosition());
		oceanShader->SetFloat("ambientReflection", 0.3f);
		oceanShader->SetFloat("diffuseConstant", 1.0f);
		oceanShader->SetFloat("specularConstant", 1.0f);
		oceanShader->SetFloat("shininess", 1000.0f);
		oceanShader->SetFloat("mixValue", mixValue);
		oceanShader->SetVec3("fogColor", glm::vec3(0.0f, 0.12f, 0.25f));
		oceanShader->SetFloat("fogDensity", 0.07f);

		oceanShader->SetMat4("model", waterModel);
		glBindVertexArray(oceanVAO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glActiveTexture(GL_TEXTURE0);
		oceanShader->SetInt("skybox", 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);

		glActiveTexture(GL_TEXTURE0 + 1);
		oceanShader->SetInt("floor", 1);
		glBindTexture(GL_TEXTURE_2D, stonesTextureID);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindVertexArray(0);

		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -50.0, 0.0));

		model = translationMatrix * model;

		terrainShader->Use();
		terrainShader->SetMat4("projection", projection);
		terrainShader->SetMat4("view", view);
		terrainShader->SetFloat("mixValue", mixValue);
		terrainShader->SetVec3("fogColor", glm::vec3(0.0f, 0.12f, 0.25f));
		terrainShader->SetFloat("fogDensity", 0.03f);
		terrainShader->SetVec3("viewPos", pCamera->GetPosition());

		terrainShader->SetMat4("model", model);
		glBindVertexArray(terrainVAO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glActiveTexture(GL_TEXTURE0);
		terrainShader->SetInt("texture1", 0);
		glBindTexture(GL_TEXTURE_2D, stonesTextureID);

		glActiveTexture(GL_TEXTURE0 +1);

		glBindTexture(GL_TEXTURE_2D, causticstextureID);
		terrainShader->SetInt("texture2", 1);
		glDrawElements(GL_TRIANGLES, terrainIndices.size(), GL_UNSIGNED_INT, 0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindVertexArray(0);

		translationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -100.0));

		glm::mat4 modelWall = waterModel * translationMatrix * glm::mat4(1.0f);

		wallShader->Use();
		wallShader->SetMat4("projection", projection);
		wallShader->SetMat4("view", view);
		wallShader->SetVec3("fogColor", glm::vec3(0.0f, 0.12f, 0.25f));
		wallShader->SetFloat("fogDensity", 0.1f);
		wallShader->SetVec3("viewPos", pCamera->GetPosition());
		wallShader->SetFloat("mixValue", mixValue);

		wallShader->SetMat4("model", modelWall);
		glBindVertexArray(wallVAO);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glDrawElements(GL_TRIANGLES, wallIndices.size(), GL_UNSIGNED_INT, 0);

		translationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 100.0));

		modelWall =waterModel* translationMatrix * glm::mat4(1.0f);
		wallShader->SetMat4("model", modelWall);
		glDrawElements(GL_TRIANGLES, wallIndices.size(), GL_UNSIGNED_INT, 0);

		translationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(100.0, 0.0, 0.0));
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0), glm::pi<float>() / 2, glm::vec3(0.0f, 1.0f, 0.0f));
		modelWall = waterModel * translationMatrix *rotationMatrix * glm::mat4(1.0f);
		wallShader->SetMat4("model", modelWall);
		glDrawElements(GL_TRIANGLES, wallIndices.size(), GL_UNSIGNED_INT, 0);

		translationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-100.0, 0.0, 0.0));
		rotationMatrix = glm::rotate(glm::mat4(1.0), glm::pi<float>() / 2, glm::vec3(0.0f, 1.0f, 0.0f));
		modelWall =waterModel * translationMatrix * rotationMatrix * glm::mat4(1.0f);
		wallShader->SetMat4("model", modelWall);
		glDrawElements(GL_TRIANGLES, wallIndices.size(), GL_UNSIGNED_INT, 0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindVertexArray(0);
	}

	~Ocean() 
	{
		oceanPoints.clear();
		indices.clear();
		glDeleteVertexArrays(1, &oceanVAO);
		glDeleteBuffers(1, &oceanVBO);
		std::cout << "Ocean deleted"<<std::endl;
		terrainPoints.clear();
		terrainIndices.clear();
		glDeleteVertexArrays(1, &terrainVAO);
		glDeleteBuffers(1, &terrainVBO);
		wallPoints.clear();
		wallIndices.clear();
		glDeleteVertexArrays(1, &wallVAO);
		glDeleteBuffers(1, &wallVBO);
	}
};
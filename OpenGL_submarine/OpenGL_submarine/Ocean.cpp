#include "Shader.cpp"
#include "Camera.cpp"
#include "GerstnerWave.cpp"

class Ocean 
{
private:
	std::vector<float> oceanPoints;
	std::vector<unsigned int> indices;
	unsigned int oceanVAO, oceanVBO, oceanEBO;
	Shader* oceanShader;

public:
	Ocean()
	{
		for (float k = -10.0f; k < 10.0f; k += 0.1f)
			for (float i = -10.0f; i < 10.0f; i += 0.1f)
			{
				oceanPoints.push_back(k);
				oceanPoints.push_back(0.0f);
				oceanPoints.push_back(i);

			}

		int numPointsPerSide = 200;
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
	}

	void RenderOcean(Camera* pCamera, glm::vec3 lightPos, glm::vec3 lightColor,double currentFrame,std::vector<GerstnerWave>waves)
	{
		glm::mat4 model = glm::mat4(1.0);

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
		oceanShader->SetFloat("shininess", 100.0f);

		oceanShader->SetMat4("model", model);
		glBindVertexArray(oceanVAO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	~Ocean() 
	{
		oceanPoints.clear();
		indices.clear();
		glDeleteVertexArrays(1, &oceanVAO);
		glDeleteBuffers(1, &oceanVBO);
	}
};
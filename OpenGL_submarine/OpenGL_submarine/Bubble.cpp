#pragma once

#include <GL/glew.h>
#include <string>
#include "Shader.cpp"
#include "Camera.cpp"

class Bubble
{
public:
	Bubble(unsigned int texture, float xPos, float yPos, float zPos)
		: bubbleTexture(texture), x(xPos), y(yPos), z(zPos)
	{
		bubbleShader = new Shader("Bubble.vs", "Bubble.fs");
		Initialize();
	}

	void Render(Camera* pCamera)
	{
		bubbleShader->Use();
		bubbleShader->SetMat4("projection", pCamera->GetProjectionMatrix());
		bubbleShader->SetMat4("view", pCamera->GetViewMatrix());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bubbleTexture);

		glBindVertexArray(bubbleVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	~Bubble()
	{
		glDeleteVertexArrays(1, &bubbleVAO);
		glDeleteBuffers(1, &bubbleVBO);
		delete bubbleShader;
	}

private:
	unsigned int bubbleVAO, bubbleVBO;
	unsigned int bubbleTexture;
	Shader* bubbleShader;
	float x, y, z;

	void Initialize()
	{
		// Define vertex data
		float vertices[] = {
			// Define vertex positions
		 -1.0f,  1.0f, -1.0f,  
		 -1.0f, -1.0f, -1.0f, 
		  1.0f, -1.0f, -1.0f,  

		  1.0f, -1.0f, -1.0f,  
		  1.0f,  1.0f, -1.0f,  
		 -1.0f,  1.0f, -1.0f,  
		};

		// Generate and bind VAO
		glGenVertexArrays(1, &bubbleVAO);
		glBindVertexArray(bubbleVAO);

		// Generate VBO
		glGenBuffers(1, &bubbleVBO);
		glBindBuffer(GL_ARRAY_BUFFER, bubbleVBO);

		// Copy vertex data to VBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Set vertex attribute pointers
		// Vertex positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// Vertex texture coords
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Unbind VAO and VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		std::cout << "Bubble VAO and VBO initialized successfully" << std::endl;
	}

};

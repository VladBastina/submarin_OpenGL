﻿#include "Ocean.cpp"

class SkyBox
{
public:
    SkyBox(unsigned int texture)
    {
        textureID = texture;

        float vertices[] = {
	    -1.0f, 1.0f, -1.0f,
	    -1.0f, -1.0f, -1.0f,
	     1.0f, -1.0f, -1.0f,
	     1.0f, -1.0f, -1.0f,
	     1.0f, 1.0f, -1.0f,
	     -1.0f, 1.0f, -1.0f,

	    -1.0f, -1.0f, 1.0f,
	    -1.0f, -1.0f, -1.0f,
	    -1.0f, 1.0f, -1.0f,
	    -1.0f, 1.0f, -1.0f,
	    -1.0f, 1.0f, 1.0f,
	    -1.0f, -1.0f, 1.0f,

	    1.0f, -1.0f, 1.0f,
	    1.0f, -1.0f, -1.0f,
	    1.0f, 1.0f, -1.0f,
	    1.0f, 1.0f, -1.0f,
	    1.0f, 1.0f, 1.0f,
	    1.0f, -1.0f, 1.0f,

	    -1.0f, -1.0f, 1.0f,
	    -1.0f, 1.0f, 1.0f,
	    1.0f, 1.0f, 1.0f,
	    1.0f, 1.0f, 1.0f,
	    1.0f, -1.0f, 1.0f,
	    -1.0f, -1.0f, 1.0f,

	    -1.0f, 1.0f, -1.0f,
	    -1.0f, 1.0f, 1.0f,
	    1.0f, 1.0f, 1.0f,
	    1.0f, 1.0f, 1.0f,
	    1.0f, 1.0f, -1.0f,
	    -1.0f, 1.0f, -1.0f,

	    -1.0f, -1.0f, -1.0f,
	    -1.0f, -1.0f, 1.0f,
	    1.0f, -1.0f, 1.0f,
	    1.0f, -1.0f, 1.0f,
	    1.0f, -1.0f, -1.0f,
	    -1.0f, -1.0f, -1.0f,
		};


        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        skyboxShader = new Shader("SkyBox.vs", "SkyBox.fs");
    }

    void RenderSkybox(Camera* pcamera)
    {
        glDepthFunc(GL_LEQUAL);
        skyboxShader->Use();
        glm::mat4 view = glm::mat4(glm::mat3(pcamera->GetViewMatrix()));
        skyboxShader->SetMat4("view", view);
        skyboxShader->SetMat4("projection", pcamera->GetProjectionMatrix());

        glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }

    ~SkyBox()
    {
        glDeleteVertexArrays(1, &skyboxVAO);
        glDeleteBuffers(1, &skyboxVBO);
        delete skyboxShader;
    }

private:
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int textureID;
    Shader* skyboxShader;
};
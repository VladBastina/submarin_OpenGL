#include "Ocean.cpp"

class SkyBox
{
public:
    SkyBox(unsigned int texture)
    {
        textureID = texture;

        float vertices[] = {
            // Fața frontală
            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            // Fața din spate
            -0.5f, -0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            // Fața de sus
            -0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            // Fața de jos
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,
            // Fața dreaptă
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f, -0.5f,
             // Fața stângă
             -0.5f, -0.5f, -0.5f,
             -0.5f, -0.5f,  0.5f,
             -0.5f,  0.5f,  0.5f,
             -0.5f,  0.5f,  0.5f,
             -0.5f,  0.5f, -0.5f,
             -0.5f, -0.5f, -0.5f
        };


        // Crearea și încărcarea geometriei în buffer-ele OpenGL
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // Coordonatele vertex
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        skyboxShader = new Shader("SkyBox.vs", "SkyBox.fs");
    }

    void RenderSkybox(Camera* pcamera)
    {
        glDepthMask(GL_FALSE);
        skyboxShader->Use();
        glm::mat4 view = glm::mat4(glm::mat3(pcamera->GetViewMatrix()));
        skyboxShader->SetMat4("view", view);
        skyboxShader->SetMat4("projection", pcamera->GetProjectionMatrix());
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        skyboxShader->SetInt("skybox", 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
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
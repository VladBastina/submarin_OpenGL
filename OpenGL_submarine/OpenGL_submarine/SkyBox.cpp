#include "Ocean.cpp"

class SkyBox
{
public:
    SkyBox(unsigned int texture)
    {
        textureID = texture;

        float vertices[] = {
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,

        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,

        -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f
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
        //glDepthMask(GL_FALSE);
        skyboxShader->Use();
        //glm::mat4 view = glm::mat4(glm::mat3(pcamera->GetViewMatrix()));
        skyboxShader->SetMat4("view", pcamera->GetViewMatrix());
        skyboxShader->SetMat4("projection", pcamera->GetProjectionMatrix());

        glm::mat4 model = glm::mat4(1.0f);
        skyboxShader->SetMat4("model", model);
        skyboxShader->SetVec3("color", glm::vec3(1.0f,1.0f,1.0f));

        glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        //glDepthMask(GL_TRUE);
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
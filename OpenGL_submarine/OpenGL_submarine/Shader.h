#pragma once

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm.hpp>


class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath);

    // activate the shader
    // ------------------------------------------------------------------------
    void Use() const;
    // utility uniform functions
    // ------------------------------------------------------------------------
    void SetBool(const std::string& name, bool value) const;
    // ------------------------------------------------------------------------
    void SetInt(const std::string& name, int value) const;
    // ------------------------------------------------------------------------
    void SetFloat(const std::string& name, float value) const;
    // ------------------------------------------------------------------------
    void SetMat4(const std::string& name, const glm::mat4& mat) const;

    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec3(const std::string& name, float x, float y, float z) const;

    void SetVec4(const std::string& name, const glm::vec4& value) const;
    void SetVec4(const std::string& name, float x, float y, float z, float w) const;
    unsigned int GetID() const;
    ~Shader();
private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void CheckCompileErrors(unsigned int shader, std::string type);
};


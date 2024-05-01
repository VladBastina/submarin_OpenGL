#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h> 
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h> 
#include <vector>
#include <algorithm>

#include <GL/glew.h>

#define GLM_FORCE_CTOR_INIT 
#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")


class Shader
{
public:
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(const char* vertexPath, const char* fragmentPath) {
		Init(vertexPath, fragmentPath);
	}

	~Shader() {
		if (ID != 0) // Check if the program is initialized before deleting
			glDeleteProgram(ID);
	}

	// activate the shader
	// ------------------------------------------------------------------------
	void Use() const {
		if (ID != 0) // Check if the program is initialized before using
			glUseProgram(ID);
	}

	unsigned int GetID() const { return ID; }

	// utility uniform functions
	void SetInt(const std::string& name, int value) const {
		if (ID != 0) {
			GLint loc = glGetUniformLocation(ID, name.c_str());
			if (loc != -1)
				glUniform1i(loc, value);
			else
				std::cerr << "Uniform '" << name << "' not found in shader." << std::endl;
		}
	}

	void SetFloat(const std::string& name, const float& value) const {
		if (ID != 0) {
			GLint loc = glGetUniformLocation(ID, name.c_str());
			if (loc != -1)
				glUniform1f(loc, value);
			else
				std::cerr << "Uniform '" << name << "' not found in shader." << std::endl;
		}
	}

	void SetVec3(const std::string& name, const glm::vec3& value) const {
		if (ID != 0) {
			GLint loc = glGetUniformLocation(ID, name.c_str());
			if (loc != -1)
				glUniform3fv(loc, 1, &value[0]);
			else
				std::cerr << "Uniform '" << name << "' not found in shader." << std::endl;
		}
	}

	void SetMat4(const std::string& name, const glm::mat4& mat) const {
		if (ID != 0) {
			GLint loc = glGetUniformLocation(ID, name.c_str());
			if (loc != -1)
				glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
			else
				std::cerr << "Uniform '" << name << "' not found in shader." << std::endl;
		}
	}

private:
	unsigned int ID = 0;

	void Init(const char* vertexPath, const char* fragmentPath) {
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e) {
			std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
			return;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		CheckCompileErrors(vertex, "VERTEX");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		CheckCompileErrors(fragment, "FRAGMENT");
		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		CheckCompileErrors(ID, "PROGRAM");

		// 3. delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void CheckCompileErrors(unsigned int shader, std::string type) {
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};
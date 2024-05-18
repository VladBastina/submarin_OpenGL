#include "Shader.h"
	

	Shader::Shader(const char* vertexPath, const char* fragmentPath) {
		Init(vertexPath, fragmentPath);
	}

	Shader::~Shader() {
		if (ID != 0) // Check if the program is initialized before deleting
			glDeleteProgram(ID);
	}

	// activate the shader
	// ------------------------------------------------------------------------
	void Shader::Use() const {
		if (ID != 0) // Check if the program is initialized before using
			glUseProgram(ID);
	}

	unsigned int Shader::GetID() const { return ID; }

	// utility uniform functions
	void Shader::SetInt(const std::string& name, int value) const {
		if (ID != 0) {
			GLint loc = glGetUniformLocation(ID, name.c_str());
			if (loc != -1)
				glUniform1i(loc, value);
			else
				std::cerr << "Uniform '" << name << "' not found in shader." << std::endl;
		}
	}

	void Shader::SetFloat(const std::string& name, const float& value) const {
		if (ID != 0) {
			GLint loc = glGetUniformLocation(ID, name.c_str());
			if (loc != -1)
				glUniform1f(loc, value);
			else
				std::cerr << "Uniform '" << name << "' not found in shader." << std::endl;
		}
	}

	void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
		if (ID != 0) {
			GLint loc = glGetUniformLocation(ID, name.c_str());
			if (loc != -1)
				glUniform3fv(loc, 1, &value[0]);
			else
				std::cerr << "Uniform '" << name << "' not found in shader." << std::endl;
		}
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const {
		if (ID != 0) {
			GLint loc = glGetUniformLocation(ID, name.c_str());
			if (loc != -1)
				glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
			else
				std::cerr << "Uniform '" << name << "' not found in shader." << std::endl;
		}
	}

	void Shader::Init(const char* vertexPath, const char* fragmentPath) {
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensures ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try
		{
			// Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();
		// 2. Compile shaders
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];
		// Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// Print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		// Print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// Shader Program
		this->ID = glCreateProgram();
		glAttachShader(this->ID, vertex);
		glAttachShader(this->ID, fragment);
		glLinkProgram(this->ID);
		// Print linking errors if any
		glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		// Delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	

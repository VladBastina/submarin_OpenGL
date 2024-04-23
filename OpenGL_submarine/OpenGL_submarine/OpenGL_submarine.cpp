#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h> 
#include <vector>
#include <algorithm>

#include <iostream>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <fstream>
#include "Camera.cpp"
#include "Shader.cpp"

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const float PI = 3.14159265358979323846;

void gerstnerWave(float x, float z, float& outX, float& outZ, float time) {
	// Parametrii valului
	float A = 0.1f; // Amplificarea valului
	float lambda = 1.0f; // Lungimea valului
	float omega = 2.0f * PI / lambda; // Frecvența unghiulară
	float k = 2.0f * PI / lambda; // Numărul de undă
	float phi = PI / 4.0f; // Fază inițială

	// Calculul deplasării
	float displacementX = A * cos(k * (x * cos(phi) + z * sin(phi)) - omega * time);
	float displacementZ = A * cos(k * (x * sin(phi) - z * cos(phi)) - omega * time);

	outX = x + displacementX;
	outZ = z + displacementZ;
}

struct GerstnerWave {
	float amplitude;
	float wavelength;
	float speed;
	float direction;
	float phase;
};

// Funcție pentru calculul valului Gerstner
void gerstnerWave(float x, float y, float z, float time, const GerstnerWave& wave, float& outX, float& outY, float& outZ) {
	float k = 2.0f * glm::pi<float>() / wave.wavelength;
	float omega = 2.0f * glm::pi<float>() / wave.speed;
	float displacementX = wave.amplitude * cos(glm::dot(glm::vec2(x, z), glm::vec2(cos(wave.direction), sin(wave.direction))) * k - omega * time + wave.phase);
	float displacementZ = wave.amplitude * sin(glm::dot(glm::vec2(x, z), glm::vec2(cos(wave.direction), sin(wave.direction))) * k - omega * time + wave.phase);
	float displacementY = wave.amplitude * sin(omega * time - wave.phase); // Aici se poate ajusta pentru a obține o undă înaltă și subțire pe axa y
	outX = x + displacementX;
	outY = y + displacementY;
	outZ = z + displacementZ;
}

Camera* pCamera = nullptr;

void Cleanup()
{
	delete pCamera;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
int main(int argc, char** argv);
void processInput(GLFWwindow* window);

// timing
double deltaTime = 0.0f;	// time between current frame and last frame
double lastFrame = 0.0f;

unsigned int CreateTexture(const std::string& strTexturePath);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		pCamera->ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		pCamera->ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		pCamera->ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		pCamera->ProcessKeyboard(RIGHT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		pCamera->ProcessKeyboard(UP, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		pCamera->ProcessKeyboard(DOWN, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		pCamera->Reset(width, height);

	}
}

int main(int argc, char** argv)
{
	std::string strFullExeFileName = argv[0];
	std::string strExePath;
	const size_t last_slash_idx = strFullExeFileName.rfind('\\');
	if (std::string::npos != last_slash_idx) {
		strExePath = strFullExeFileName.substr(0, last_slash_idx);
	}

	std::vector<GerstnerWave> waves = {
	{1.0f, 1.0f, 1.0f, 0.0f, 0.0f},           // Val 1 (orizontal, fără mișcare)
	{0.05f, 2.0f, 0.8f, glm::pi<float>() / 3.0f, glm::pi<float>() / 2.0f}, // Val 2 (înclinat la 60 de grade, mișcare în sens invers acelor de ceasornic)
	{2.0f, 1.5f, 1.2f, glm::pi<float>() / 6.0f, glm::pi<float>()}, // Val 3 (înclinat la 30 de grade, mișcare în sensul acelor de ceasornic)
	{0.86f, 2.5f, 0.9f, glm::pi<float>() / 4.0f, glm::pi<float>() / 4.0f}, // Val 4 (înclinat la 45 de grade, mișcare în sens invers acelor de ceasornic)
	{0.12f, 1.2f, 1.5f, glm::pi<float>() / 2.0f, glm::pi<float>() / 3.0f}, // Val 5 (vertical, mișcare în sus)
	{0.07f, 2.8f, 0.7f, glm::pi<float>() * 2.0f / 3.0f, glm::pi<float>() / 6.0f}, // Val 6 (înclinat la 120 de grade, mișcare în jos)
	{0.5f, 1.8f, 1.0f, glm::pi<float>() * 5.0f / 6.0f, glm::pi<float>() / 4.0f}, // Val 7 (înclinat la 150 de grade, mișcare în sens invers acelor de ceasornic)
	{0.09f, 2.2f, 0.8f, glm::pi<float>() * 7.0f / 6.0f, glm::pi<float>() / 2.0f}, // Val 8 (înclinat la 210 de grade, mișcare în sensul acelor de ceasornic)
	{1.11f, 1.4f, 1.3f, glm::pi<float>() * 4.0f / 3.0f, glm::pi<float>() * 5.0f / 6.0f}, // Val 9 (înclinat la 240 de grade, mișcare în jos)
	{0.13f, 2.3f, 0.6f, glm::pi<float>() * 3.0f / 2.0f, glm::pi<float>() * 7.0f / 6.0f} // Val 10 (înclinat la 270 de grade, mișcare în sus)
	};

	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Submarin", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Create camera
	pCamera = new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0, 0.0, 3.0));

	Shader oceanShader("Ocean.vs", "Ocean.fs");
	//shaderBlending.SetInt("texture1", 0);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		std::vector<float> oceanPoints;
		for (float k = -5.0f; k < 5.0f; k += 0.1f)
			for (float i = -5.0f; i < 5.0f; i += 0.1f)
				for (float j = -5.0f; i < 5.0f; i += 0.1f)
				{
					float newX=0.0f, newY=0.0f,newZ=0.0f;
					for (auto& wave : waves)
					{
						float tempX, tempY,tempZ;
						gerstnerWave(i, j,k,currentFrame, wave, tempX, tempY,tempZ);
						newX += tempX;
						newY += tempY;
						newZ += tempZ;
					}
					oceanPoints.push_back(newX/10);
					oceanPoints.push_back(newY/10);
					oceanPoints.push_back(newZ/10);
				}

		// Grass VAO si VBO
		unsigned int oceanVAO, oceanVBO;
		glGenVertexArrays(1, &oceanVAO);
		glGenBuffers(1, &oceanVBO);
		glBindVertexArray(oceanVAO);
		glBindBuffer(GL_ARRAY_BUFFER, oceanVBO);
		glBufferData(GL_ARRAY_BUFFER, oceanPoints.size() * sizeof(float), oceanPoints.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


		// input
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0);

		oceanShader.Use();
		glm::mat4 projection = pCamera->GetProjectionMatrix();
		glm::mat4 view = pCamera->GetViewMatrix();
		oceanShader.SetMat4("projection", projection);
		oceanShader.SetMat4("view", view);

		// Draw floor
		glBindVertexArray(oceanVAO);
		model = glm::mat4();
		oceanShader.SetMat4("model", model);
		glDrawArrays(GL_POINTS, 0, (100*100*3));


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();

		oceanPoints.clear();
		glDeleteVertexArrays(1, &oceanVAO);
		glDeleteBuffers(1, &oceanVBO);
	}

	Cleanup();

	// Clear floor VAO
	
	// Clear grass VAO

	// glfw: terminate, clearing all previously allocated GLFW resources
	glfwTerminate();
	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
	
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	pCamera->Reshape(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	pCamera->MouseControl((float)xpos, (float)ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yOffset)
{
	pCamera->ProcessMouseScroll((float)yOffset);
}

unsigned int CreateTexture(const std::string& strTexturePath)
{
	unsigned int textureId = -1;

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(strTexturePath.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cout << "Failed to load texture: " << strTexturePath << std::endl;
	}
	stbi_image_free(data);

	return textureId;
}

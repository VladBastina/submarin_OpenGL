#define STB_IMAGE_IMPLEMENTATION
#include "Skybox.cpp"
#include "Model.h"
#include "Submarine.cpp"

#include <irrKlang.h>
using namespace irrklang;

#pragma comment (lib, "irrKlang.lib")

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float kaValue = 0.5f;


Camera* pCamera = nullptr;
SkyBox* skybox= nullptr;

void Cleanup()
{
	delete pCamera;
}

unsigned int loadSkyboxTexture(const std::vector<std::string>& faces, std::string strExePath)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		std::string path = strExePath + "\\" + faces[i];
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
int main(int argc, char** argv);
void processInput(GLFWwindow* window);

// timing
double deltaTime = 0.0f;	// time between current frame and last frame
double lastFrame = 0.0f;

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

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		float mixValue = skybox->getMixValue();
		mixValue += 0.1f;
		if (mixValue > 1.0f)
		{
			mixValue = 1.0f;
		}
		skybox->setMixValue(mixValue);
	}

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
	{
		float mixValue = skybox->getMixValue();
		mixValue -= 0.1f;
		if (mixValue < 0.1f)
		{
			mixValue = 0.1f;
		}
		skybox->setMixValue(mixValue);
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

	std::vector<std::string> faces
	{
		"px.png",
		"nx.png",
		"py.png",
		"ny.png",
		"pz.png",
		"nz.png"
	};

	std::vector<GerstnerWave> waves = {
	{0.2f, 1.0f, 1.0f, 0.0f, 0.0f},
	{0.05f, 2.0f, 2.0f, glm::pi<float>() / 3.0f, glm::pi<float>() / 2.0f},
	{0.5f, 1.5f, 5.0f, glm::pi<float>() / 6.0f, glm::pi<float>()},
	{0.86f, 2.5f, 7.f, glm::pi<float>() / 4.0f, glm::pi<float>() / 4.0f},
	{0.12f, 1.2f, 20.0f, glm::pi<float>() / 2.0f, glm::pi<float>() / 3.0f},
	{0.07f, 2.8f, 3.0f, glm::pi<float>() * 2.0f / 3.0f, glm::pi<float>() / 6.0f},
	{0.5f, 1.8f, 1.0f, glm::pi<float>() * 5.0f / 6.0f, glm::pi<float>() / 4.0f},
	{0.09f, 2.2f, 3.0f, glm::pi<float>() * 7.0f / 6.0f, glm::pi<float>() / 2.0f},
	{0.11f, 1.4f, 1.25f, glm::pi<float>() * 4.0f / 3.0f, glm::pi<float>() * 5.0f / 6.0f},
	{0.13f, 2.3f, 10.0f, glm::pi<float>() * 3.0f / 2.0f, glm::pi<float>() * 7.0f / 6.0f}
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

	unsigned int skyboxtextureID = loadSkyboxTexture(faces,strExePath);
	unsigned int stonestextureID = CreateTexture(strExePath + "\\pietris.png");
	unsigned int causticstextureID = CreateTexture(strExePath + "\\caustic.png");

	Ocean* ocean = new Ocean();
	skybox = new SkyBox(skyboxtextureID);
	
	ISoundEngine* SoundEngine = createIrrKlangDevice();

	// Create camera
	pCamera = new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0, 0.0, 0.0));
	float lastY = pCamera->GetPosition().y;

	glm::vec3 lightPos(0.0f, 20.0f, 0.0f);
	glm::vec3 lightColor(1.0f, 0.8f, 0.4f);

	std::string seagul_sound = strExePath + "\\seagul_sound.mp3";
	std::string splash_sound = strExePath + "\\splash_water.mp3";
	std::string sonar_sound = strExePath + "\\sonar.mp3";
	std::string underwater_sound = strExePath + "\\underwater.mp3";
	bool sound = false;

	Submarine submarine;
	// render loop
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		float currentY = pCamera->GetPosition().y;

		if ((currentY < -1.0f && lastY >= -1.0f) ||
			(currentY >= -1.0f && lastY < -1.0f) ||
			(currentY <= 1.0f && lastY > 1.0f) ||
			(currentY > 1.0f && lastY <= 1.0f))
		{
			sound = false;
		}

		if (sound == false)
		{
			SoundEngine->stopAllSounds();

			if (currentY < -1.0f)
			{
				sound = true;
				SoundEngine->play2D(underwater_sound.c_str(), true);
				SoundEngine->play2D(sonar_sound.c_str(), true);
			}
			else if (currentY >= -1.0f && currentY <= 1.0f)
			{
				sound = true;
				SoundEngine->play2D(splash_sound.c_str(), true);
			}
			else
			{
				sound = true;
				SoundEngine->play2D(seagul_sound.c_str(), true);
			}
		}

		lastY = currentY;

		processInput(window);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		glm::vec3 position = pCamera->GetPosition();

		if (position.x < (-166.6f) || position.x>(166.6f) || position.z < (-166.6f) || position.z>166.6f)
		{
			pCamera->SetPosition(glm::vec3(0.0f, position.y, 0.0f));
		}

		skybox->RenderSkybox(pCamera);

		ocean->RenderOcean(pCamera, lightPos, lightColor, currentFrame, waves,skyboxtextureID,stonestextureID,causticstextureID,skybox->getMixValue());


		submarine.Render(pCamera);
	




		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	Cleanup();
	delete ocean;
	delete skybox;
	SoundEngine->drop();

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

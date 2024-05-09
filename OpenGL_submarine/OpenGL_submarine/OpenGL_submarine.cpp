#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <map>

#include "Skybox.cpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


Camera* pCamera = nullptr;

void Cleanup()
{
	delete pCamera;
}

unsigned int loadSkyboxTexture(const std::vector<std::string>& faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data != nullptr)
		{
			if(i==0)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			else if (i==1)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			else if (i == 2)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			else if (i == 3)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			else if (i == 4)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y+ i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			else if (i == 5)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Failed to load texture: " << faces[i] << std::endl;
			return 0;
		}
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
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
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"down.jpg",
		"front.jpg",
		"back.jpg"
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

	unsigned int skyboxtextureID = loadSkyboxTexture(faces);

	Ocean* ocean = new Ocean();
	SkyBox* skybox = new SkyBox(skyboxtextureID);
	
	// Create camera
	pCamera = new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0, 0.0, 3.0));

	glm::vec3 lightPos(0.0f, 5.0f, 0.0f);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skybox->RenderSkybox(pCamera);
		ocean->RenderOcean(pCamera, lightPos, glm::vec3(1.0f, 0.8f, 0.4f), currentFrame, waves);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	Cleanup();
	delete ocean;
	delete skybox;

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

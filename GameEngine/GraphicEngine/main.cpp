#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

#include "cProjectManager.h"
#include "cProjectUI.h"
#include "cShaderManager.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

}

static void error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, char* argv[]) {

	GLuint vertex_buffer = 0;
	GLuint shaderID = 0;
	GLint vpos_location = 0;
	GLint vcol_location = 0;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Game Engine", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	cShaderManager* pTheShaderManager = new cShaderManager();

	cShaderManager::cShader vertexShader01;
	cShaderManager::cShader fragmentShader01;

	vertexShader01.fileName = "assets/shaders/vertexShader01.glsl";
	fragmentShader01.fileName = "assets/shaders/fragmentShader01.glsl";

	if (!pTheShaderManager->createProgramFromFile("Shader_1", vertexShader01, fragmentShader01)) {
		std::cout << "Didn't compile shaders" << std::endl;
		std::string theLastError = pTheShaderManager->getLastError();
		std::cout << "Because: " << theLastError << std::endl;
		return -1;
	} else {
		std::cout << "Compiled shader OK." << std::endl;
	}

	pTheShaderManager->useShaderProgram("Shader_1");
	shaderID = pTheShaderManager->getIDFromFriendlyName("Shader_1");
	//glUseProgram(shaderID);

	// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup ImGui style
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Creates my Project Manager
	cProjectManager* g_ProjectManager = new cProjectManager();
	g_ProjectManager->SetShaderID(shaderID);
	
	// Creates my Project Manager UI - ImGui Window
	cProjectUI g_projectUI(g_ProjectManager);

	// ImGui Window Color
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		g_projectUI.renderUI();
		g_projectUI.renderSceneUI();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	delete g_ProjectManager;

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
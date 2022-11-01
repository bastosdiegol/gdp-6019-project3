#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "cProjectManager.h"
#include "cProjectUI.h"
#include "cShaderManager.h"
#include "cLightManager.h"
#include "cLightHelper.h"

#ifdef _DEBUG
#define DEBUG_LOG_ENABLED
#endif
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

// Global Camera Eye that will be pointing to the Selected Scene Camera
glm::vec3* g_cameraEye;
// Global Camera Eye that will be pointing to the Selected Scene Target
glm::vec3* g_cameraTarget;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	const float CAMERA_MOVE_SPEED = 1.0f;
	if (key == GLFW_KEY_A)     // Left
	{
		g_cameraEye->x -= CAMERA_MOVE_SPEED;
	}
	if (key == GLFW_KEY_D)     // Right
	{
		g_cameraEye->x += CAMERA_MOVE_SPEED;
	}
	if (key == GLFW_KEY_W)     // Forward
	{
		g_cameraEye->z += CAMERA_MOVE_SPEED;
	}
	if (key == GLFW_KEY_S)     // Backwards
	{
		g_cameraEye->z -= CAMERA_MOVE_SPEED;
	}
	if (key == GLFW_KEY_Q)     // Down
	{
		g_cameraEye->y -= CAMERA_MOVE_SPEED;
	}
	if (key == GLFW_KEY_E)     // Up
	{
		g_cameraEye->y += CAMERA_MOVE_SPEED;
	}
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

	const char* glsl_version = "#version 460";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// Create window
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Game Engine", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		return 2;
	}
	glfwSwapInterval(1);

	// Shader Management
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
		DEBUG_PRINT("Compiled shader OK.");
	}
	pTheShaderManager->useShaderProgram("Shader_1");
	shaderID = pTheShaderManager->getIDFromFriendlyName("Shader_1");
	glUseProgram(shaderID);

	// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// Setup ImGui style
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer backends
	if (!ImGui_ImplGlfw_InitForOpenGL(window, true) || !ImGui_ImplOpenGL3_Init(glsl_version)) {
		return 3;
	}

	// Creates my Project Manager
	cProjectManager* g_ProjectManager = new cProjectManager();
	g_ProjectManager->SetShaderID(shaderID);
	
	// Creates my Project Manager UI - ImGui Window
	cProjectUI g_projectUI(g_ProjectManager);

	// ImGui Window Color
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Gets Shader ID for each variable to sent to it
	GLint mvp_location						= glGetUniformLocation(shaderID, "MVP");
	GLint mModel_location					= glGetUniformLocation(shaderID, "mModel");
	GLint mView_location					= glGetUniformLocation(shaderID, "mView");
	GLint mProjection_location				= glGetUniformLocation(shaderID, "mProjection");
	GLint mModelInverseTransform_location	= glGetUniformLocation(shaderID, "mModelInverseTranspose");

	while (!glfwWindowShouldClose(window)) {

		//DrawConcentricDebugLightObjects();

		float ratio;
		int width, height;
		glm::mat4x4 matModel;
		glm::mat4x4 matProjection;
		glm::mat4x4 matView;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Renders Project Manager UI
		g_projectUI.renderUI();
		g_projectUI.renderSceneUI();
		if(g_projectUI.isMeshSelected)
			g_projectUI.renderMeshUI();
		if(g_projectUI.isLightSelected)
			g_projectUI.renderLighthUI();

		// If new Scene
		if (g_ProjectManager->isNewScene) {
			// Points Camera Eye and Target to their respective new values
			g_cameraEye = &g_ProjectManager->m_selectedScene->m_cameraEye;
			g_cameraTarget = &g_ProjectManager->m_selectedScene->m_cameraTarget;
			g_ProjectManager->isNewScene = false;
		}

		// Checks if there's a Selected Scene to be drawn
		if (g_ProjectManager->m_selectedScene != nullptr) {
			g_ProjectManager->m_lightManager->CopyLightInformationToShader(
				g_ProjectManager->m_VAOManager->m_shaderID,
				&g_ProjectManager->m_selectedScene->m_mLights);

			matView = glm::lookAt(*g_cameraEye,
								  *g_cameraTarget,
								  upVector);

			// Pass eye location to the shader
			GLint eyeLocation_UniLoc = glGetUniformLocation(shaderID, "eyeLocation");
			glUniform4f(eyeLocation_UniLoc,	g_cameraEye->x, g_cameraEye->y, g_cameraEye->z, 1.0f);

			matProjection = glm::perspective(0.6f,       // Field of view
											 ratio,
											 0.1f,
											 10000.0f);

			// Time to Draw the Meshes of Selected Scene
			std::map<std::string, cMeshObject*>::iterator itMeshes;
			itMeshes = g_ProjectManager->m_selectedScene->m_mMeshes.begin();
			// Iterates through all meshes
			for (itMeshes; itMeshes != g_ProjectManager->m_selectedScene->m_mMeshes.end(); itMeshes++) {
				cMeshObject* pCurrentMeshObject = itMeshes->second;
				// Skip this meshe if not visible
				if (!pCurrentMeshObject->m_bIsVisible)
					continue;

				glCullFace(GL_BACK);
				glEnable(GL_DEPTH_TEST);

				matModel = glm::mat4x4(1.0f);
				// Apply Position Transformation
				glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f), pCurrentMeshObject->m_position);
				// Apply Rotation Transformation
				glm::mat4 matRoationZ = glm::rotate(glm::mat4(1.0f), pCurrentMeshObject->m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
				glm::mat4 matRoationY = glm::rotate(glm::mat4(1.0f), pCurrentMeshObject->m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
				glm::mat4 matRoationX = glm::rotate(glm::mat4(1.0f), pCurrentMeshObject->m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
				// Scale the object
				float uniformScale = pCurrentMeshObject->m_scale;
				glm::mat4 matScale = glm::scale(glm::mat4(1.0f), glm::vec3(uniformScale, uniformScale, uniformScale));
				// Applying all these transformations to the Model
				matModel = matModel * matTranslation;
				matModel = matModel * matRoationX;
				matModel = matModel * matRoationY;
				matModel = matModel * matRoationZ;
				matModel = matModel * matScale;

				// Pass all the matrices to the Shader
				glUniformMatrix4fv(mModel_location, 1, GL_FALSE, glm::value_ptr(matModel));
				glUniformMatrix4fv(mView_location, 1, GL_FALSE, glm::value_ptr(matView));
				glUniformMatrix4fv(mProjection_location, 1, GL_FALSE, glm::value_ptr(matProjection));

				// Inverse transpose of a 4x4 matrix removes the right column and lower row
				// Leaving only the rotation (the upper left 3x3 matrix values)
				glm::mat4 mModelInverseTransform = glm::inverse(glm::transpose(matModel));
				glUniformMatrix4fv(mModelInverseTransform_location, 1, GL_FALSE, glm::value_ptr(mModelInverseTransform));

				// Wireframe Check
				if (pCurrentMeshObject->m_isWireframe)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				// Pass Colours to the Shader
				GLint RGBA_Colour_ULocID = glGetUniformLocation(shaderID, "RGBA_Colour");
				glUniform4f(RGBA_Colour_ULocID, pCurrentMeshObject->m_RGBA_colour.r,
					pCurrentMeshObject->m_RGBA_colour.g,
					pCurrentMeshObject->m_RGBA_colour.b,
					pCurrentMeshObject->m_RGBA_colour.w);
				// Pass the UseRGB boolean to the Shader
				GLint bUseRGBA_Colour_ULocID = glGetUniformLocation(shaderID, "bUseRGBA_Colour");
				if (pCurrentMeshObject->m_bUse_RGBA_colour)
					glUniform1f(bUseRGBA_Colour_ULocID, (GLfloat)GL_TRUE);
				else
					glUniform1f(bUseRGBA_Colour_ULocID, (GLfloat)GL_FALSE);

				// Pass DoNotLight boolean to the Shader	
				GLint bDoNotLight_Colour_ULocID = glGetUniformLocation(shaderID, "bDoNotLight");
				if (pCurrentMeshObject->m_bDoNotLight)
					glUniform1f(bDoNotLight_Colour_ULocID, (GLfloat)GL_TRUE);
				else
					glUniform1f(bDoNotLight_Colour_ULocID, (GLfloat)GL_FALSE);

				// Pass the Model we want to draw
				glBindVertexArray(pCurrentMeshObject->m_parentModel->VAO_ID);
				glDrawElements(GL_TRIANGLES,
							   pCurrentMeshObject->m_parentModel->numberOfIndices,
							   GL_UNSIGNED_INT,
							   (void*)0);
				glBindVertexArray(0);
			}
		}

		// Rendering
		ImGui::Render();
		//int display_w, display_h;
		//glfwGetFramebufferSize(window, &display_w, &display_h);
		//glViewport(0, 0, display_w, display_h);
		//glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		//glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete g_ProjectManager;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

//void DrawConcentricDebugLightObjects(void) {
//	extern bool bEnableDebugLightingObjects;
//	extern cLightManager* g_pTheLightManager;
//
//	if (!bEnableDebugLightingObjects) {
//		pDebugSphere_1->bIsVisible = false;
//		pDebugSphere_2->bIsVisible = false;
//		pDebugSphere_3->bIsVisible = false;
//		pDebugSphere_4->bIsVisible = false;
//		pDebugSphere_5->bIsVisible = false;
//		return;
//	}
//
//	pDebugSphere_1->bIsVisible = true;
//	pDebugSphere_2->bIsVisible = true;
//	pDebugSphere_3->bIsVisible = true;
//	pDebugSphere_4->bIsVisible = true;
//	pDebugSphere_5->bIsVisible = true;
//
//	cLightHelper theLightHelper;
//
//	// Move the debug sphere to where the light #0 is
//	pDebugSphere_1->position = glm::vec3(g_pTheLightManager->vecTheLights[0].position);
//	pDebugSphere_2->position = glm::vec3(g_pTheLightManager->vecTheLights[0].position);
//	pDebugSphere_3->position = glm::vec3(g_pTheLightManager->vecTheLights[0].position);
//	pDebugSphere_4->position = glm::vec3(g_pTheLightManager->vecTheLights[0].position);
//	pDebugSphere_5->position = glm::vec3(g_pTheLightManager->vecTheLights[0].position);
//
//	{
//		// Draw a bunch of concentric spheres at various "brightnesses" 
//		float distance75percent = theLightHelper.calcApproxDistFromAtten(
//			0.75f,  // 75%
//			0.001f,
//			100000.0f,
//			g_pTheLightManager->vecTheLights[0].atten.x,
//			g_pTheLightManager->vecTheLights[0].atten.y,
//			g_pTheLightManager->vecTheLights[0].atten.z);
//
//		pDebugSphere_2->scale = distance75percent;
//		pDebugSphere_2->position = glm::vec3(g_pTheLightManager->vecTheLights[0].position);
//	}
//
//	{
//		// Draw a bunch of concentric spheres at various "brightnesses" 
//		float distance50percent = theLightHelper.calcApproxDistFromAtten(
//			0.50f,  // 75%
//			0.001f,
//			100000.0f,
//			g_pTheLightManager->vecTheLights[0].atten.x,
//			g_pTheLightManager->vecTheLights[0].atten.y,
//			g_pTheLightManager->vecTheLights[0].atten.z);
//
//		pDebugSphere_3->scale = distance50percent;
//		pDebugSphere_3->position = glm::vec3(g_pTheLightManager->vecTheLights[0].position);
//	}
//
//	{
//		// Draw a bunch of concentric spheres at various "brightnesses" 
//		float distance25percent = theLightHelper.calcApproxDistFromAtten(
//			0.25f,  // 75%
//			0.001f,
//			100000.0f,
//			g_pTheLightManager->vecTheLights[0].atten.x,
//			g_pTheLightManager->vecTheLights[0].atten.y,
//			g_pTheLightManager->vecTheLights[0].atten.z);
//
//		pDebugSphere_4->scale = distance25percent;
//		pDebugSphere_4->position = glm::vec3(g_pTheLightManager->vecTheLights[0].position);
//	}
//
//	{
//		// Draw a bunch of concentric spheres at various "brightnesses" 
//		float distance5percent = theLightHelper.calcApproxDistFromAtten(
//			0.05f,  // 75%
//			0.001f,
//			100000.0f,
//			g_pTheLightManager->vecTheLights[0].atten.x,
//			g_pTheLightManager->vecTheLights[0].atten.y,
//			g_pTheLightManager->vecTheLights[0].atten.z);
//
//		pDebugSphere_5->scale = distance5percent;
//		pDebugSphere_5->position = glm::vec3(g_pTheLightManager->vecTheLights[0].position);
//	}
//	return;
//}
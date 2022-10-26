#include <imgui.h>

#include "cProjectUI.h"

void cProjectUI::renderUI() {
	// Create a window called "Project Manager", with a menu bar.
	ImGui::Begin("Project Manager", NULL, ImGuiWindowFlags_MenuBar		| 
										  ImGuiWindowFlags_NoMove		| 
										  ImGuiWindowFlags_NoTitleBar	|
										  ImGuiWindowFlags_NoResize);
	// Menu for Project Management
	// TODO: Save Scene on XML file
	if (ImGui::BeginMenuBar()) {
		if(ImGui::BeginMenu("Project")){
			if (ImGui::MenuItem("Save Scene")) {
			}
			if (ImGui::MenuItem("Quit")) {
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::TreeNodeEx("Scenes:", ImGuiTreeNodeFlags_DefaultOpen)) {
		for (int sceneIndex = 0; sceneIndex < m_projectManager->m_vScenes.size(); sceneIndex++) {
			ImGui::Bullet(); 
			if (ImGui::SmallButton(m_projectManager->m_vScenes[sceneIndex].c_str())) {
				m_projectManager->LoadScene(m_projectManager->m_vScenes[sceneIndex]);
			}
		}
		ImGui::TreePop();
	}
	
	ImGui::End();
}

void cProjectUI::renderSceneUI() {
	// Scene Widget
	ImGui::Begin("Selected Scene", NULL, ImGuiWindowFlags_MenuBar |
										 ImGuiWindowFlags_NoMove		|
										 ImGuiWindowFlags_NoTitleBar	|
										 ImGuiWindowFlags_NoResize);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Scene")) {
			// TODO: Add models using this menu in the future?
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	// Checks if there's a selected scene
	if (ImGui::TreeNodeEx("Models:", ImGuiTreeNodeFlags_DefaultOpen)) {
		if (m_projectManager->m_selectedScene != nullptr) {
			// Iterates through all models
			for (int modelIndex = 0;
				modelIndex < m_projectManager->m_selectedScene->m_vModels.size();
				modelIndex++) {
				ImGui::Bullet();
				if (ImGui::SmallButton(m_projectManager->m_selectedScene->m_vModels[modelIndex].c_str())) {
				}
			}
		} else {
			ImGui::BulletText("Empty.");
		}
		ImGui::TreePop();
	}

	ImGui::End();
}

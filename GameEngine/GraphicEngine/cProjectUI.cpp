#include <map>

#include "imgui/imgui.h"
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
			if (ImGui::MenuItem("New Scene")) {
			}
			if (ImGui::MenuItem("Quit")) {
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::TreeNodeEx("Scenes:", ImGuiTreeNodeFlags_DefaultOpen)) {
		std::map<std::string, cScene*>::iterator itScenes;
		for (itScenes = m_projectManager->m_mScenes.begin();
			 itScenes != m_projectManager->m_mScenes.end();
			 itScenes++) {
			ImGui::Bullet();
			if (ImGui::SmallButton(itScenes->first.c_str())) {
				m_projectManager->LoadScene(itScenes->first.c_str());
				m_projectManager->isNewScene = true;
			}
		}
		ImGui::TreePop();
	}
	
	ImGui::End();
}

void cProjectUI::renderSceneUI() {
	// Scene Widget
	ImGui::Begin("Selected Scene", NULL, ImGuiWindowFlags_MenuBar		|
										 ImGuiWindowFlags_NoMove		|
										 ImGuiWindowFlags_NoTitleBar	|
										 ImGuiWindowFlags_NoResize);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Scene")) {
			if (ImGui::MenuItem("Save Scene")) {
				m_projectManager->SaveSelectedScene();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	// Checks if there's a selected scene
	if (ImGui::TreeNodeEx("Meshes:", ImGuiTreeNodeFlags_DefaultOpen)) {
		if (m_projectManager->m_selectedScene != nullptr) {
			// Iterates through all models
			std::map<std::string, cMeshObject*>::iterator itMesh;
			for (itMesh = m_projectManager->m_selectedScene->m_vMeshes.begin();
				 itMesh != m_projectManager->m_selectedScene->m_vMeshes.end();
				 itMesh++) {
				ImGui::Bullet();
				if (ImGui::SmallButton(itMesh->second->m_meshName.c_str())) {
					m_projectManager->m_selectedMesh = itMesh->second;
				}
			}
		} else {
			ImGui::BulletText("Empty.");
		}
		ImGui::TreePop();
	}

	ImGui::End();
}

void cProjectUI::renderMeshUI() {
	// Mesh Widget
	ImGui::Begin("Selected Mesh", NULL, ImGuiWindowFlags_MenuBar	|
										ImGuiWindowFlags_NoMove		|
										ImGuiWindowFlags_NoTitleBar	|
										ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysUseWindowPadding);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Mesh")) {
			if (ImGui::MenuItem("Save Mesh Data")) {

			}
			// TODO: Add models using this menu in the future?
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	// Checks if the selected mesh isnt nullptr
	if (m_projectManager->m_selectedMesh != nullptr) {
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), m_projectManager->m_selectedMesh->m_meshName.c_str());
		// Variable for ImGui to control Position
		float pos3f[3] = { m_projectManager->m_selectedMesh->m_position.x,
							  m_projectManager->m_selectedMesh->m_position.y,
							  m_projectManager->m_selectedMesh->m_position.z };
		ImGui::Text("Position");
		// Position Input
		if (ImGui::InputFloat3("X Y Z##PositionInput", pos3f, "%.2f")) {
			m_projectManager->m_selectedMesh->m_position.x = pos3f[0];
			m_projectManager->m_selectedMesh->m_position.y = pos3f[1];
			m_projectManager->m_selectedMesh->m_position.z = pos3f[2];
		}
		// Position Slider
		if (ImGui::SliderFloat3("X Y Z##PositionSlider", pos3f, -100, +100, "%.2f")) {
			m_projectManager->m_selectedMesh->m_position.x = pos3f[0];
			m_projectManager->m_selectedMesh->m_position.y = pos3f[1];
			m_projectManager->m_selectedMesh->m_position.z = pos3f[2];
		}
		// Variable for ImGui to control Rotation
		float rot3f[3] = { m_projectManager->m_selectedMesh->m_rotation.x,
								  m_projectManager->m_selectedMesh->m_rotation.y,
								  m_projectManager->m_selectedMesh->m_rotation.z };
		ImGui::Text("Rotation");
		// Rotation Input
		if (ImGui::InputFloat3("X Y Z##RotationInput", rot3f, "%.2f")) {
			m_projectManager->m_selectedMesh->m_rotation.x = rot3f[0];
			m_projectManager->m_selectedMesh->m_rotation.y = rot3f[1];
			m_projectManager->m_selectedMesh->m_rotation.z = rot3f[2];
		}
		// Rotation Slider
		if (ImGui::SliderFloat3("X Y Z##RotationSlider", rot3f, -100, +100, "%.2f")) {
			m_projectManager->m_selectedMesh->m_rotation.x = rot3f[0];
			m_projectManager->m_selectedMesh->m_rotation.y = rot3f[1];
			m_projectManager->m_selectedMesh->m_rotation.z = rot3f[2];
		}
		ImGui::Text("Scale");
		// Scale Slider
		ImGui::SliderFloat("##Scale", &m_projectManager->m_selectedMesh->m_scale, -10, 10);
		ImGui::Text("Use RGB?"); ImGui::SameLine();
		// RGB Checkbox
		ImGui::Checkbox("##RGB?", &m_projectManager->m_selectedMesh->m_bUse_RGBA_colour);
		// Variable for ImGui to control RGB
		float col4f[4] = { m_projectManager->m_selectedMesh->m_RGBA_colour.r,
								  m_projectManager->m_selectedMesh->m_RGBA_colour.g,
								  m_projectManager->m_selectedMesh->m_RGBA_colour.b,
								  m_projectManager->m_selectedMesh->m_RGBA_colour.a};
		// RGB Picker
		if (ImGui::ColorEdit4("R G B A##RGB", col4f, ImGuiColorEditFlags_Float)) {
			m_projectManager->m_selectedMesh->m_RGBA_colour.r = col4f[0];
			m_projectManager->m_selectedMesh->m_RGBA_colour.g = col4f[1];
			m_projectManager->m_selectedMesh->m_RGBA_colour.b = col4f[2];
			m_projectManager->m_selectedMesh->m_RGBA_colour.a = col4f[3];
		}
		ImGui::Text("Is Wireframe?"); ImGui::SameLine();
		// isWireframe Checkbox
		ImGui::Checkbox("##Wireframe?", &m_projectManager->m_selectedMesh->m_isWireframe);
		ImGui::Text("Do not Light?"); ImGui::SameLine();
		// DoNotLight Checkbox
		ImGui::Checkbox("##Light?", &m_projectManager->m_selectedMesh->m_bDoNotLight);
		ImGui::Text("Is Visible?"); ImGui::SameLine();
		// isVisible Checkbox
		ImGui::Checkbox("##Visible?", &m_projectManager->m_selectedMesh->m_bIsVisible);
	}

	ImGui::End();
}
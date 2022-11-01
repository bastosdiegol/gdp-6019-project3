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
	// Camera Manipulation
	// First Checks if there's a selected scene
	if (m_projectManager->m_selectedScene != nullptr) {
		// Variable for ImGui to control Position
		float eye3f[3] = { m_projectManager->m_selectedScene->m_cameraEye.x,
						   m_projectManager->m_selectedScene->m_cameraEye.y,
						   m_projectManager->m_selectedScene->m_cameraEye.z };
		ImGui::Text("Camera Eye");
		// Camera Eye Input
		if (ImGui::InputFloat3("X Y Z##EyeInput", eye3f, "%.2f")) {
			m_projectManager->m_selectedScene->m_cameraEye.x = eye3f[0];
			m_projectManager->m_selectedScene->m_cameraEye.y = eye3f[1];
			m_projectManager->m_selectedScene->m_cameraEye.z = eye3f[2];
		}
		// Camera Eye Slider
		if (ImGui::SliderFloat3("X Y Z##EyeSlider", eye3f, -50, +50, "%.2f")) {
			m_projectManager->m_selectedScene->m_cameraEye.x = eye3f[0];
			m_projectManager->m_selectedScene->m_cameraEye.y = eye3f[1];
			m_projectManager->m_selectedScene->m_cameraEye.z = eye3f[2];
		}
		// Variable for ImGui to control Camera Target
		float target3f[3] = { m_projectManager->m_selectedScene->m_cameraTarget.x,
							  m_projectManager->m_selectedScene->m_cameraTarget.y,
							  m_projectManager->m_selectedScene->m_cameraTarget.z };
		ImGui::Text("Camera Target");
		// Camera Target Input
		if (ImGui::InputFloat3("X Y Z##TargetInput", target3f, "%.2f")) {
			m_projectManager->m_selectedScene->m_cameraTarget.x = target3f[0];
			m_projectManager->m_selectedScene->m_cameraTarget.y = target3f[1];
			m_projectManager->m_selectedScene->m_cameraTarget.z = target3f[2];
		}
		// Camera Target Slider
		if (ImGui::SliderFloat3("X Y Z##TargetSlider", target3f, -50, +50, "%.2f")) {
			m_projectManager->m_selectedScene->m_cameraTarget.x = target3f[0];
			m_projectManager->m_selectedScene->m_cameraTarget.y = target3f[1];
			m_projectManager->m_selectedScene->m_cameraTarget.z = target3f[2];
		}
	} else {
		ImGui::BulletText("No Scene Selected.");
	}
	// Checks if there's a selected scene
	if (ImGui::TreeNodeEx("Meshes:", ImGuiTreeNodeFlags_DefaultOpen)) {
		if (m_projectManager->m_selectedScene != nullptr) {
			// Iterates through all models
			std::map<std::string, cMeshObject*>::iterator itMesh;
			for (itMesh = m_projectManager->m_selectedScene->m_mMeshes.begin();
				 itMesh != m_projectManager->m_selectedScene->m_mMeshes.end();
				 itMesh++) {
				ImGui::Bullet();
				if (ImGui::SmallButton(itMesh->second->m_meshName.c_str())) {
					m_projectManager->m_selectedMesh = itMesh->second;
				}
			}
			// Iterates through all lights
			std::map<std::string, cLight*>::iterator itLight;
			for (itLight = m_projectManager->m_selectedScene->m_mLights.begin();
				 itLight != m_projectManager->m_selectedScene->m_mLights.end();
				 itLight++) {
				ImGui::Bullet();
				if (ImGui::SmallButton(itLight->first.c_str())) {
					m_projectManager->m_selectedLight = itLight->second;
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
										ImGuiWindowFlags_NoResize | 
										ImGuiWindowFlags_AlwaysUseWindowPadding);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Mesh")) {
			if (ImGui::MenuItem("Save Mesh Data")) {
				// TODO:: Save Mesh Data
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
		if (ImGui::SliderFloat3("X Y Z##PositionSlider", pos3f, -10, +10, "%.2f")) {
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
		if (ImGui::SliderFloat3("X Y Z##RotationSlider", rot3f, -5, +5, "%.2f")) {
			m_projectManager->m_selectedMesh->m_rotation.x = rot3f[0];
			m_projectManager->m_selectedMesh->m_rotation.y = rot3f[1];
			m_projectManager->m_selectedMesh->m_rotation.z = rot3f[2];
		}
		ImGui::Text("Scale");
		// Scale Slider
		ImGui::SliderFloat("##Scale", &m_projectManager->m_selectedMesh->m_scale, -10, 10);
		ImGui::SameLine();
		ImGui::InputFloat("##ScaleInput", &m_projectManager->m_selectedMesh->m_scale);
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
	} else {
		ImGui::BulletText("No Mesh Selected.");
	}

	ImGui::End();
}

void cProjectUI::renderLighthUI() {
	ImGui::Begin("Selected Light", NULL, ImGuiWindowFlags_MenuBar |
										 ImGuiWindowFlags_NoMove |
										 ImGuiWindowFlags_NoTitleBar |
										 ImGuiWindowFlags_NoResize | 
										 ImGuiWindowFlags_AlwaysUseWindowPadding);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Light")) {
			if (ImGui::MenuItem("Save Light Data")) {
				// TODO: Save Light Data
			}
			// TODO: Add lights using this menu in the future?
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	// Checks if the selected mesh isnt nullptr
	if (m_projectManager->m_selectedLight != nullptr) {
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), m_projectManager->m_selectedLight->m_friendlyName.c_str());
		// Light Type
		static const char* current_item; // Combo current selected item
		// Array used on the combo
		const char* lightType[3] = { "Point Light", "Spot Light", "Directional" };
		// Sets current_item same from selected light
		switch ((cLight::eLightType)m_projectManager->m_selectedLight->m_param1.x) {
		case cLight::POINT_LIGHT:
			current_item = "Point Light";
			break;
		case cLight::SPOT_LIGHT:
			current_item = "Spot Light";
			break;
		case cLight::DIRECTIONAL_LIGHT:
			current_item = "Directional";
			break;
		}
		// Light ComboBox
		ImGui::Text("Light Type");
		if (ImGui::BeginCombo("##Light Type", current_item)) {
			for (int i = 0; i < 3; i++) {
				bool is_selected = (current_item == lightType[i]);
				if (ImGui::Selectable(lightType[i], is_selected)) {
					current_item = lightType[i];
					m_projectManager->m_selectedLight->SetLightType(cLight::eLightType(i));
				}if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		// Variable for ImGui to control Position
		float pos3f[3] = { m_projectManager->m_selectedLight->m_position.x,
						   m_projectManager->m_selectedLight->m_position.y,
						   m_projectManager->m_selectedLight->m_position.z };
		ImGui::Text("Position");
		// Position Input
		if (ImGui::InputFloat3("X Y Z##PositionInput", pos3f, "%.2f")) {
			m_projectManager->m_selectedLight->m_position.x = pos3f[0];
			m_projectManager->m_selectedLight->m_position.y = pos3f[1];
			m_projectManager->m_selectedLight->m_position.z = pos3f[2];
		}
		// Position Slider
		if (ImGui::SliderFloat3("X Y Z##PositionSlider", pos3f, -10, +10, "%.2f")) {
			m_projectManager->m_selectedLight->m_position.x = pos3f[0];
			m_projectManager->m_selectedLight->m_position.y = pos3f[1];
			m_projectManager->m_selectedLight->m_position.z = pos3f[2];
		}
		ImGui::Text("Use RGB?"); ImGui::SameLine();
		// RGB Checkbox
		if (ImGui::Checkbox("##RGB?", &m_projectManager->m_selectedLight->m_useRGB)) {
			// If False - Set Light to White
			if (m_projectManager->m_selectedLight->m_useRGB == false) {
				m_projectManager->m_selectedLight->SetToWhite();
			}
		}
		// Variable for ImGui to control RGB
		float col4f[4] = { m_projectManager->m_selectedLight->m_specular.x,
						   m_projectManager->m_selectedLight->m_specular.y,
						   m_projectManager->m_selectedLight->m_specular.z,
						   m_projectManager->m_selectedLight->m_specular.w };
		// RGB Picker
		if (ImGui::ColorEdit4("R G B A##RGB", col4f, ImGuiColorEditFlags_Float)) {
			m_projectManager->m_selectedLight->m_specular.x = col4f[0];
			m_projectManager->m_selectedLight->m_specular.y = col4f[1];
			m_projectManager->m_selectedLight->m_specular.z = col4f[2];
			m_projectManager->m_selectedLight->m_specular.w = col4f[3];
		}
	} else {
		ImGui::BulletText("No Light Selected.");
	}

	ImGui::End();
}
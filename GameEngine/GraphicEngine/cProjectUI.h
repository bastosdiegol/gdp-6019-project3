#pragma once
#include "cProjectManager.h"

class cProjectUI {
private:
	cProjectManager* m_projectManager;
public:
	cProjectUI(cProjectManager* projectManager) : m_projectManager(projectManager) {}
	/// <summary>
	/// Will draw any important information related to Project Manager
	/// </summary>
	void renderUI();
	/// <summary>
	/// After a scene is being selected. This method will open the scene windows
	/// </summary>
	void renderSceneUI();
};


#pragma once
#ifndef __PLAY_SCENE2__
#define __PLAY_SCENE2__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Enemy.h"
#include "Target.h"

class PlayScene2 : public Scene
{
public:
	PlayScene2();
	~PlayScene2();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;


	void checkGuiChangs();
	void m_ImGuiKeyMap();
	void m_ImGuiSetStyle();
	void m_updateUI();

private:
	// ImGui menu variables
	bool m_exitApp = false;
	bool m_displayAbout = false;
	bool m_displayUI = true;

	bool m_changeScene;

	glm::vec2 m_mousePosition;
};

#endif /* defined (__PLAY_SCENE2__) */
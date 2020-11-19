#pragma once
#ifndef __PLAY_SCENE2__
#define __PLAY_SCENE2__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Enemy.h"
#include "Target.h"
#include "Brick.h"

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

	void initLabels();
	void initGameObjects();

	void updateLabels();
	void updateGameObjects();

private:
	// ImGui menu variables
	bool m_exitApp = false;
	bool m_displayAbout = false;
	bool m_displayUI = true;

	Brick* m_pBrickSpite;
	Target* m_pLootCrate;

	Label* m_pBrickVelocity;
	Label* m_pPPM;

	SDL_Color black = { 0,0,0,225 };
	SDL_Color white = { 225,225,225,225 };

	bool m_changeScene;

	float m_PPM = 100;

	glm::vec2 m_mousePosition;
};

#endif /* defined (__PLAY_SCENE2__) */
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
#include "Ball.h"


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
	void initMemberVariables();
	void initGameObjects();
	void initGuiSliderVariables();

	void updateLabels();
	void updateGameObjects();
	void updateOrientation(BrickOrientation);
	void checkCollision();
	void calculateAverageBrickVel();

	void resetScene();
	void changeScene();
	
	float GuiSliderFloats[5];

	bool GuiSliderBools[5];

private:
	// ImGui menu variables
	bool m_exitApp = false;
	bool m_displayAbout = false;
	bool m_displayUI = true;

	Brick* m_pBrickSprite;
	Brick* m_pBrickSpiteVertical;
	Ball* m_pBallSprite;
	Target* m_pLootCrate;

	Label* m_pBrickVelocityLabel;
	Label* m_pPPMLabel;
	Label* m_pKeepUpScoreLabel;
	Label* m_highScoreLabel;
	Label* m_pBallVelocityLabel;

	int collisionCounter;
	int ballVelocityCounter;
	int brickVelocityCounter;
	float averageBrickVelocity;
	float avgBrickVelTemp;
	float averageBallVelocity;
	float avgBallVelTemp;


	SDL_Color black = { 0,0,0,225 };
	SDL_Color white = { 225,225,225,225 };

	bool m_changeScene;

	float m_PPM = 100;

	glm::vec2 m_mousePosition;
};

#endif /* defined (__PLAY_SCENE2__) */
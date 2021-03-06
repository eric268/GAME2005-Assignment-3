#pragma once
#ifndef __START_SCENE__
#define __START_SCENE__

#include "Scene.h"
#include "Label.h"
#include "ship.h"
#include "Button.h"
#include "Enemy.h"
#include "Player.h"

class StartScene final : public Scene
{
public:
	StartScene();
	~StartScene();

	// Inherited via Scene
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
	
private:
	Label* m_pStartLabel{};
	Label* m_pInstructionsLabel{};
	Label* m_pStartLabel2{};
	Label* m_pStartLabel3{};
	Enemy* m_arrEnemies[4];

	Label* m_pName[2];
	Label* m_pButtonLabel[2];

	Player* m_pPlayer{};

	Button* m_pStartButton;

	Button* m_pScene2StartButton;

	SDL_Color white = { 225,225,225,225 };
};

#endif /* defined (__START_SCENE__) */
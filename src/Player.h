#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "Sprite.h"
#include "Label.h"
#include "Ball.h"


class Player final : public Sprite
{
public:

	Player();
	~Player();


	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stopMovingX();
	void stopMovingY();

	bool isColliding(GameObject*);
	bool isMoving(GameObject*);
	float getDistance(GameObject*);
	bool getBulletFired();
	void setBeginSimulation(bool);

	void checkCollision();
	void m_checkBounds();
	bool setBulletFired(bool);
	bool getBeginSimulation();
	float getAcceleration();
	void setAcceleration(float);
	
	//void checkCollisionWalls();

	//void playerMovement(GameObject*);

private:
	float ACCELERATION = 50.0f;
	glm::vec2 m_direction;
	bool m_beginSimulation;
	bool m_bulletFired;

};

#endif /* defined (__PLAYER__) */
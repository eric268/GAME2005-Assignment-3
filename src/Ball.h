#pragma once
#ifndef __BALL__
#define __BALL__

#include "DisplayObject.h"
#include "WallCollision.h"
class Ball final : public DisplayObject
{
public:

	Ball();
	~Ball ();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void updateVelocity();
	void calculateMomentum();
	void velocityAfterCollision(float, glm::vec2);
	void velocityAfterWallCollision();
	void changeYBrickCollision();

	//Setters
	void setCollisionType(WallCollison);
	void setDirection(glm::vec2);
	void setAcceleration(float);
	void setForce(float);
	void setMass(float);
	void setBeginSimulation(bool);
	void setWallAbsorbtion(float);
	void setMomentum(glm::vec2);
	void setBrickWeight(float);
	void setBrickVelocity(glm::vec2);
	void setBrickCollisionHappened(bool);
	void setBrickPosition(glm::vec2);

	//Getters
	WallCollison getCollisionType();
	void checkCollisionWalls();
	glm::vec2 getDirection();
	float getAcceleration();
	float getForce();
	float getMass();
	bool getBeginSimulation();
	float getWallAbsorbtion();
	glm::vec2 getMomentum();
	float getPaddleWeight();
	glm::vec2 getPaddleVelocity();
	bool getPaddleCollisionHappened();
	glm::vec2 getPaddlePosition();

private:
	glm::vec2 m_direction;
	glm::vec2 m_mousePosition;
	glm::vec2 m_previousPosition;
	glm::vec2 m_momentum;
	glm::vec2 m_BrickVelocity;
	glm::vec2 m_brickPosition;

	float m_acceleraton;
	float m_force;
	float m_mass;
	bool m_beginSimulation;
	int m_frameCount;
	float m_PPM;
	float m_wallVelAbsorbtion;
	int collisionCheckCounter;
	bool collisionJustHappened;
	bool paddleCollisionHappened;
	float m_paddleWeight;

	WallCollison collisionType;

	void m_move();
	bool m_withinBounds();
	void m_reset();

	//Constants
	const float m_gravity = 9.8f;
};

#endif /* defined (__Ball__) */
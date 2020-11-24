#pragma once
#ifndef __BRICK__
#define __BRICK__
#include "Sprite.h"
#include "BrickOrientation.h"

class Brick final : public DisplayObject
{
public:
	Brick();
	Brick(int);
	~Brick();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	glm::vec2 getDirection();
	void setDirection(glm::vec2);

	float getAcceleration();
	void setAcceleration(float);

	float getForce();
	void setForce(float);

	float getMass();
	void setMass(float);

	bool getBeginSimulation();
	void setBeginSimulation(bool);

	void handleEvents();

	void brickMovement(glm::vec2);
	void calculateVelocity();

	bool pause = false;

	BrickOrientation getOrientation();
	void setOrientation(BrickOrientation);


private: 
	glm::vec2 m_direction;
	glm::vec2 m_mousePosition;
	glm::vec2 m_previousPosition;

	float m_acceleraton;
	float m_force;
	float m_mass;
	bool m_beginSimulation;
	int m_frameCount;
	float m_PPM;
	BrickOrientation m_orientation;

	int temp = 0;


	void m_move();
	bool m_withinBounds(glm::vec2);
	void m_reset();

	//Constants
	const float m_gravity = 9.8f;
};

#endif /* defined (__BRICK__) */
#include "Ball.h"
#include "TextureManager.h"
#include "CollisionManager.h"
#include "Util.h"
#include "Game.h"
#include <cassert>
#include <iostream>

Ball::Ball()
{
	TextureManager::Instance()->load("../Assets/textures/Ball.png", "Ball");

	auto size = TextureManager::Instance()->getTextureSize("Ball");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(135.0f, 200.0f);
	getRigidBody()->velocity = glm::vec2(250.0f, 250.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);

	m_direction = glm::vec2(0.0f);
	m_previousPosition = getTransform()->position;
	m_acceleraton = 0.0f;
	m_force = 0.0f;
	m_mass = 2.0f; //In kg
	m_wallVelAbsorbtion = 0.1f;
	m_beginSimulation = false;
	m_frameCount = 0;
	m_PPM = 100;
	m_momentum = glm::vec2(0.0f);
	collisionType = NO_WALL_COLLISION;
	setType(BALL);
	collisionCheckCounter = 0;
	collisionJustHappened = false;
	m_BrickVelocity = glm::vec2(0.0f);
	m_paddleWeight = 0.0f;
	paddleCollisionHappened = true;
	m_brickPosition = glm::vec2(0.0f);
}

Ball::~Ball() = default;

void Ball::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("Ball", x, y, 0, 255, true);
}

void Ball::update()
{
	if (m_beginSimulation)
	{
		updateVelocity();
		m_move();
		calculateMomentum();
		checkCollisionWalls();
	}


}

void Ball::clean()
{
}

glm::vec2 Ball::getDirection()
{
	return m_direction;
}

void Ball::setDirection(glm::vec2 direction)
{
	m_direction = direction;
}

float Ball::getAcceleration()
{
	return m_acceleraton;
}

void Ball::setAcceleration(float acceleration)
{
	m_acceleraton = acceleration;
}

float Ball::getForce()
{
	return m_force;
}

void Ball::setForce(float force)
{
	m_force = force;
}

float Ball::getMass()
{
	return m_mass;
}

void Ball::setMass(float mass)
{
	m_mass = mass;
}

bool Ball::getBeginSimulation()
{
	return m_beginSimulation;
}

void Ball::setBeginSimulation(bool begin)
{
	m_beginSimulation = begin;
}

void Ball::checkCollisionWalls()
{
	if (getTransform()->position.x - getWidth() / 2 <= 0 && collisionJustHappened == false)
	{

		collisionType =  LEFT_WALL_COLLISION;
	}
	else if (getTransform()->position.x + getWidth() / 2 >= 800.0f && collisionJustHappened == false)
	{
		collisionType = RIGHT_WALL_COLLISION;
	}
	else if (getTransform()->position.y - getHeight() / 2 <= 0 && collisionJustHappened == false)
	{
		collisionType = CEILING_COLLISION;
	}
	else if (getTransform()->position.y + getHeight() / 2 >= 600 && collisionJustHappened == false)
	{
		collisionType = FLOOR_COLLISION;
	}
	else
		collisionType = NO_WALL_COLLISION;
}

float Ball::getWallAbsorbtion()
{
	return m_wallVelAbsorbtion;
}

void Ball::setWallAbsorbtion(float wallWeight)
{
	m_wallVelAbsorbtion = wallWeight;
}

glm::vec2 Ball::getMomentum()
{
	return m_momentum;
}

void Ball::setBrickWeight(float pW)
{
	m_paddleWeight = pW;
}

void Ball::setBrickVelocity(glm::vec2 pV)
{
	m_BrickVelocity = pV;
}

void Ball::setBrickCollisionHappened(bool pDC)
{
	paddleCollisionHappened = pDC;
}

void Ball::setBrickPosition(glm::vec2 brickPos)
{
	m_brickPosition = brickPos;
}

float Ball::getPaddleWeight()
{
	return m_paddleWeight;
}

glm::vec2 Ball::getPaddleVelocity()
{
	return m_BrickVelocity;
}

bool Ball::getPaddleCollisionHappened()
{
	return paddleCollisionHappened;
}

glm::vec2 Ball::getPaddlePosition()
{
	return m_brickPosition;
}

void Ball::setMomentum(glm::vec2 momentum)
{
	m_momentum = momentum;
}



WallCollison Ball::getCollisionType()
{
	return collisionType;
}

void Ball::setCollisionType(WallCollison collision)
{
	collisionType = collision;
}
void Ball::changeYBrickCollision()
{
	if (m_brickPosition.y > getTransform()->position.y && getRigidBody()->velocity.y > 0)
	{
		getRigidBody()->velocity.y = -getRigidBody()->velocity.y;
	}
	else if (m_brickPosition.y < getTransform()->position.y && getRigidBody()->velocity.y < 0)
	{
		getRigidBody()->velocity.y = -getRigidBody()->velocity.y;
	}
}

void Ball::updateVelocity()
{
	if (collisionType == BRICK_COLLISION)
	{
		velocityAfterCollision(m_paddleWeight, m_BrickVelocity);
		changeYBrickCollision();
		collisionType = NO_WALL_COLLISION;
	}
	
		collisionJustHappened = false;
	
		if (collisionType == LEFT_WALL_COLLISION || collisionType == RIGHT_WALL_COLLISION)
		{
			if (collisionType == LEFT_WALL_COLLISION)
			{
				getTransform()->position.x += 10;

			}
			if (collisionType == RIGHT_WALL_COLLISION)
			{
				getTransform()->position.x -= 10;
			}

			velocityAfterWallCollision();
			getRigidBody()->velocity.x = -getRigidBody()->velocity.x;
			collisionType = NO_WALL_COLLISION;
			collisionJustHappened = true;
		}
		 if (collisionType == FLOOR_COLLISION || collisionType == CEILING_COLLISION)
		{
			 if (collisionType == FLOOR_COLLISION)
			 {
				 getTransform()->position.y -= 10;
			 }
			 if (collisionType == CEILING_COLLISION)
			 {
				 getTransform()->position.y += 10;
			 }
			velocityAfterWallCollision();
			getRigidBody()->velocity.y = -getRigidBody()->velocity.y;
			collisionType = NO_WALL_COLLISION;
			collisionJustHappened = true;
		}

		collisionCheckCounter = 0;
	//}
	//else
	//{
		collisionCheckCounter++;
	//}

}

void Ball::calculateMomentum()
{
	m_momentum = (m_mass * getRigidBody()->velocity)/m_PPM;
	std::cout << Util::magnitude(m_momentum) << std::endl;
}

void Ball::velocityAfterCollision(float otherWeight, glm::vec2 otherVelocity)
{
	glm::vec2 otherObjectMomentum = otherWeight * otherVelocity;
	glm::vec2 totalMomentumBefore = m_momentum + otherObjectMomentum;
	float totalMass = m_mass + otherWeight;
	
	assert(totalMass > 0);

	glm::vec2 newVelocity = totalMomentumBefore / totalMass;
	float percentChangeVel = Util::magnitude(newVelocity) / (Util::magnitude(getRigidBody()->velocity)/m_PPM);

	getRigidBody()->velocity.x *= percentChangeVel;
	getRigidBody()->velocity.y *= percentChangeVel;
}

void Ball::velocityAfterWallCollision()
{
	getRigidBody()->velocity.x *= (1.0f - m_wallVelAbsorbtion);
	getRigidBody()->velocity.y *= (1.0f - m_wallVelAbsorbtion);
}



void Ball::m_move()
{
	float deltaTime = 1.0f / 60.0f;

	glm::vec2 pos = getTransform()->position;
	pos.x += getRigidBody()->velocity.x * deltaTime;
	pos.y += getRigidBody()->velocity.y * deltaTime;

	getTransform()->position = pos;
}

bool Ball::m_withinBounds()
{
	return false;
}

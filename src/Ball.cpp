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
	TextureManager::Instance()->load("../Assets/textures/Square.png", "Square");
	TextureManager::Instance()->load("../Assets/textures/Hexagon.png", "Hexagon");
	TextureManager::Instance()->load("../Assets/textures/Triangle.png", "Triangle");

	auto size = TextureManager::Instance()->getTextureSize("Ball");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(2.5f, 2.5f);
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
	collisionCheckCounter = 0;
	m_collisionJustHappened = true;
	m_brickVelocity = glm::vec2(0.0f);
	m_paddleWeight = 0.0f;
	paddleCollisionHappened = true;
	m_brickPosition = glm::vec2(0.0f);
	m_highScore = 0;
	m_keepUpScore = 0;
	m_brickHeight = 0.0f;
	m_brickWidth = 0.0f;

	setType(BALL);
	m_brickOrientation = HORIZONTAL;
	collisionType = NO_WALL_COLLISION;
	m_shape = CIRCLE;
}

Ball::~Ball() = default;

void Ball::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	//Change size and image depending on shape selected
	if (m_shape == CIRCLE)
	{
		TextureManager::Instance()->draw("Ball", x, y, 0, 255, true);
		auto size = TextureManager::Instance()->getTextureSize("Ball");
		setWidth(size.x);
		setHeight(size.y);
	}
	else if (m_shape == SQUARE)
	{
		TextureManager::Instance()->draw("Square", x, y, 0, 255, true);
		auto size = TextureManager::Instance()->getTextureSize("Square");
		setWidth(size.x);
		setHeight(size.y);
	}
	else if (m_shape == TRIANGLE)
	{
		TextureManager::Instance()->draw("Triangle", x, y, 0, 255, true);
		auto size = TextureManager::Instance()->getTextureSize("Triangle");
		setWidth(size.x);
		setHeight(size.y);
	}
	else if (m_shape == HEXAGON)
	{
		TextureManager::Instance()->draw("Hexagon", x, y, 0, 255, true);
		auto size = TextureManager::Instance()->getTextureSize("Hexagon");
		setWidth(size.x);
		setHeight(size.y);
	}
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
	if (getTransform()->position.x - getWidth() / 2 <= 0 && m_collisionJustHappened == false)
	{

		collisionType =  LEFT_WALL_COLLISION;
	}
	else if (getTransform()->position.x + getWidth() / 2 >= 800.0f && m_collisionJustHappened == false)
	{
		collisionType = RIGHT_WALL_COLLISION;
	}
	else if (getTransform()->position.y - getHeight() / 2 <= 0 && m_collisionJustHappened == false)
	{
		collisionType = CEILING_COLLISION;
	}
	else if (getTransform()->position.y + getHeight() / 2 >= 600 && m_collisionJustHappened == false)
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
	m_brickVelocity = pV;
}

void Ball::setBrickCollisionHappened(bool pDC)
{
	paddleCollisionHappened = pDC;
}

void Ball::setBrickPosition(glm::vec2 brickPos)
{
	m_brickPosition = brickPos;
}

void Ball::setBrickOrientation(BrickOrientation brickOrient)
{
	m_brickOrientation = brickOrient;
}

void Ball::setGravityEnabled(bool gravEnabled)
{
	m_gravityEnabled = gravEnabled;
}

void Ball::setKeepUpScore(int keepUpScore)
{
	m_keepUpScore = keepUpScore;
}

void Ball::setHighScore(int highScore)
{
	m_highScore = highScore;
}

void Ball::setBrickWidth(float brickWidth)
{
	m_brickWidth = brickWidth;
}

void Ball::setBrickHeight(float brickHeight)
{
	m_brickHeight = brickHeight;
}

void Ball::setCollisionJustHappened(bool collision)
{
	m_collisionJustHappened = collision;
}

void Ball::setShape(CollisionShape shape)
{
	m_shape = shape;
}

float Ball::getPaddleWeight()
{
	return m_paddleWeight;
}

glm::vec2 Ball::getPaddleVelocity()
{
	return m_brickVelocity;
}

bool Ball::getPaddleCollisionHappened()
{
	return paddleCollisionHappened;
}

bool Ball::getCollisionJustHappened()
{
	return m_collisionJustHappened;
}

glm::vec2 Ball::getPaddlePosition()
{
	return m_brickPosition;
}

BrickOrientation Ball::getBrickOrientation()
{
	return m_brickOrientation;
}

bool Ball::getGravityEnabled()
{
	return m_gravityEnabled;
}

int Ball::getKeepUpScore()
{
	return m_keepUpScore;
}

int Ball::getHighScore()
{
	return m_highScore;
}

float Ball::getBrickWidth()
{
	return m_brickWidth;
}

float Ball::getBrickHeight()
{
	return m_brickHeight;
}

CollisionShape Ball::getShape()
{
	return m_shape;
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
	if (m_brickPosition.y > getTransform()->position.y )
	{
		//This offsets whatever overlap there is between Brick and ball during collision
		getTransform()->position.y = m_brickPosition.y - m_brickHeight/2 - getHeight() / 2 ;

		if (getRigidBody()->velocity.y > 0)
		{
			getRigidBody()->velocity.y = -getRigidBody()->velocity.y;
		}
	}
	else if (m_brickPosition.y < getTransform()->position.y )
	{
		getTransform()->position.y = m_brickPosition.y + m_brickHeight/2 + getHeight() / 2;

		if (getRigidBody()->velocity.y < 0)
		{
			getRigidBody()->velocity.y = -getRigidBody()->velocity.y;
		}
	}
}

void Ball::changeXBrickCollision()
{
	if (m_brickPosition.x > getTransform()->position.x)
	{
		getTransform()->position.x = m_brickPosition.x - m_brickWidth / 2 - getWidth() / 2;
		if (getRigidBody()->velocity.x > 0)
		{
			getRigidBody()->velocity.x = -getRigidBody()->velocity.x;
		}

	}
	else if (m_brickPosition.x < getTransform()->position.x )
	{
		getTransform()->position.x = m_brickPosition.x + m_brickWidth / 2 + getWidth() / 2;
		if (getRigidBody()->velocity.x < 0)
		{
			getRigidBody()->velocity.x = -getRigidBody()->velocity.x;
		}
	}
}

void Ball::updateVelocity()
{
	m_collisionJustHappened = false;
	if (collisionType == BRICK_COLLISION)
	{
		m_keepUpScore++;
		velocityAfterCollision(m_paddleWeight, m_brickVelocity);
		if (m_brickOrientation == HORIZONTAL)
		{
			changeYBrickCollision();
		}
		else if (m_brickOrientation == VERTICAL)
		{
			changeXBrickCollision();
		}
		m_collisionJustHappened = true;
		collisionType = NO_WALL_COLLISION;
	}
	
		if (collisionType == LEFT_WALL_COLLISION || collisionType == RIGHT_WALL_COLLISION)
		{
			if (collisionType == LEFT_WALL_COLLISION)
			{
				float temp = getTransform()->position.x - getWidth() / 2;
				temp = 0 - temp;
				getTransform()->position.x += temp;

			}
			if (collisionType == RIGHT_WALL_COLLISION)
			{
				float temp = getTransform()->position.x + getWidth() / 2;
				temp = 800 - temp;
				getTransform()->position.x += temp;
			}

			velocityAfterWallCollision();
			getRigidBody()->velocity.x = -getRigidBody()->velocity.x;
			collisionType = NO_WALL_COLLISION;
			m_collisionJustHappened = true;
		}
		 if (collisionType == FLOOR_COLLISION || collisionType == CEILING_COLLISION)
		{
			 if (collisionType == FLOOR_COLLISION)
			 {
				 float temp = getTransform()->position.y + getHeight() / 2;
				 temp = 600 - temp;
				 getTransform()->position.y += temp;
				
				 if (m_highScore < m_keepUpScore)
				 {
					 m_highScore = m_keepUpScore;
				 }
				 m_keepUpScore = 0;
			 }
			 if (collisionType == CEILING_COLLISION)
			 {
				 float temp = getTransform()->position.y - getHeight() / 2;
				 temp = 0 - temp;
				 getTransform()->position.y += temp;
			 }
			velocityAfterWallCollision();
			getRigidBody()->velocity.y = -getRigidBody()->velocity.y;
			collisionType = NO_WALL_COLLISION;
			m_collisionJustHappened = true;
		}
}

void Ball::calculateMomentum()
{
	m_momentum = (m_mass * getRigidBody()->velocity)/m_PPM;
	//std::cout << Util::magnitude(m_momentum) << std::endl;
}

void Ball::velocityAfterCollision(float otherWeight, glm::vec2 otherVelocity)
{
	glm::vec2 otherObjectMomentum = otherWeight * otherVelocity;
	glm::vec2 totalMomentumBefore = m_momentum + otherObjectMomentum;
	float totalMass = m_mass + otherWeight;
	
	assert(totalMass > 0);

	glm::vec2 newVelocity = totalMomentumBefore / totalMass;
	//percentChangeVel = Util::magnitude(newVelocity) / (Util::magnitude(getRigidBody()->velocity)/m_PPM);

	getRigidBody()->velocity = newVelocity*m_PPM;

}

void Ball::velocityAfterWallCollision()
{
	getRigidBody()->velocity.x *= (1.0f - m_wallVelAbsorbtion);
	getRigidBody()->velocity.y *= (1.0f - m_wallVelAbsorbtion);
}

void Ball::m_move()
{
	float deltaTime = 1.0f / 60.0f;

	if (m_gravityEnabled)
	{
		getRigidBody()->acceleration.y = m_gravity;
		getRigidBody()->velocity += (getRigidBody()->acceleration * m_PPM) * deltaTime;
	}
	else if (!m_gravityEnabled)
	{
		getRigidBody()->acceleration = glm::vec2(0.0f);
	}

	glm::vec2 pos = getTransform()->position;
	pos.x += getRigidBody()->velocity.x * deltaTime;
	pos.y += getRigidBody()->velocity.y * deltaTime;

	getTransform()->position = pos;
}

bool Ball::m_withinBounds()
{
	return false;
}

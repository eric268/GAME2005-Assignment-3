#include "Player.h"
#include "TextureManager.h"
#include "Util.h"
#include "PlayScene.h"
#include "Bullet.h"
#include <vector>
#include <algorithm>

#define WIDTH 768     //Replace values with proper window size
#define HEIGHT 960   //Replace values with proper window size


Player::Player()
{
	TextureManager::Instance()->load("../Assets/textures/Space Ship.png", "Space Ship");

	auto size = TextureManager::Instance()->getTextureSize("Space Ship");
	setWidth(size.x);
	setHeight(size.y);
	m_beginSimulation = false;
	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLAYER);
	m_bulletFired = false;
}

Player::~Player()
= default;

void Player::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw("Space Ship", x, y, 0, 255, true);
}

void Player::update()
{
	const float deltaTime = 1.0f / 60.f;

		m_checkBounds();

		// Normalize direction vector
		float dirMagnitude = Util::magnitude(m_direction);
		if (dirMagnitude > 0)
		{
			getRigidBody()->acceleration = Util::normalize(m_direction * ACCELERATION);
		}
		else if (Util::magnitude(getRigidBody()->velocity) > 0)
		{
			getRigidBody()->acceleration = Util::normalize(getRigidBody()->velocity) * (-ACCELERATION) / (ACCELERATION / 2);
		}


		getRigidBody()->velocity += getRigidBody()->acceleration;

		glm::vec2 pos = getTransform()->position;
		pos.x += getRigidBody()->velocity.x * deltaTime;
		pos.y += getRigidBody()->velocity.y * deltaTime;

		getTransform()->position = pos;



}

void Player::clean()
{

}

void Player::moveLeft() {
	m_direction.x = -1;

}

void Player::moveRight() {
	m_direction.x = 1;
}

void Player::moveUp() {
	m_direction.y = -1;
}

void Player::moveDown() {
	m_direction.y = 1;
}

void Player::stopMovingX() {
	m_direction.x = 0.0f;
}

void Player::stopMovingY() {
	m_direction.y = 0.0f;
}

bool Player::isColliding(GameObject * pOther) {
	// Works for square sprites only
	float myRadius = getWidth() * 0.5f;
	float otherRadius = pOther->getWidth() * 0.5f;

	return (getDistance(pOther) <= myRadius + otherRadius);

}

bool Player::getBeginSimulation()
{
	return m_beginSimulation;
}

float Player::getAcceleration()
{
	return ACCELERATION;
}

void Player::setAcceleration(float acceleration)
{
	ACCELERATION = acceleration;
}

void Player::setBeginSimulation(bool begin)
{
	m_beginSimulation = begin;
}

void Player::m_checkBounds()
{
	if (getTransform()->position.x - getWidth() / 2 <  0)
	{
		getRigidBody()->velocity.x = 0;
		getTransform()->position.x =  getWidth()/2;
	}
	if (getTransform()->position.y - getHeight() / 2 <  0)
	{
		getRigidBody()->velocity.y = 0;
		getTransform()->position.y = getHeight() / 2;
	}
	if (getTransform()->position.x + getWidth() / 2 > 800)
	{
		getRigidBody()->velocity.x = 0;
		getTransform()->position.x = 800 - getWidth() / 2;
	}
	if (getTransform()->position.y + getHeight() / 2 > 600)
	{
		getRigidBody()->velocity.y = 0;
		getTransform()->position.y = 600 - getHeight() / 2;
	}
}

bool Player::setBulletFired(bool bulletFired)
{
	return m_bulletFired = bulletFired;
}

float Player::getDistance(GameObject * pOther) {
	glm::vec2 myPos = getTransform()->position;
	glm::vec2 otherPos = pOther->getTransform()->position;

	// Use pythagorean to calculate distance c = sqrt(a^2 + b^2)
	float a = myPos.x - otherPos.x;
	float b = myPos.y - otherPos.y;
	return sqrt(a * a + b * b);
}

bool Player::getBulletFired()
{
	return m_bulletFired;
}

//bool Player::m_Bounds(glm::vec2 myPos)
//{
//	if (myPos.x >= getWidth() / 2 && myPos.x <= 800 - getWidth() / 2)
//	{
//		if (myPos.y >= getHeight() / 2 && myPos.y <= 600 - getHeight() / 2)
//		{
//			return true;
//		}
//	}
//	return false;
//
//}






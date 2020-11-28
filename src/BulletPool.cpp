#include "BulletPool.h"
#include "Bullet.h"
#include <iostream>
#include <vector>

BulletPool::BulletPool(int size) 
{
	for (int i = 0; i < size; i++)
	{
		Bullet* bullet = new Bullet();
		inactive.push_back(bullet);
		all.push_back(bullet);
	}

	std::cout << "Bullet Pool created with size " << size << std::endl;

}

BulletPool::~BulletPool()
= default;

Bullet* BulletPool::Spawn()
{
	Bullet* bullet = NULL;
	if (inactive.size() > 0)
	{

		bullet = inactive.back();
		bullet->Reset();
		bullet->active = true;
		inactive.pop_back();
		active.push_back(bullet);
		bullet->setPlaySound(true);
		std::cout << "SPAWNED" << std::endl;
		std::cout << "Active count: " << std::to_string(active.size()) << std::endl;
	}
	else
	{
		std::cout << "ERROR: Bullet could not be spawned. Max bullets reached. " << std::endl;
	}

	return bullet;
}

Bullet* BulletPool::FireBullet(GameObject* player)
{
	Bullet* bullet = NULL;
	if (inactive.size() > 0)
	{
		bullet = inactive.back();
		bullet->Reset();
		bullet->active = true;
		inactive.pop_back();
		active.push_back(bullet);
		bullet->setPlaySound(true);
		bullet->getTransform()->position = player->getTransform()->position;
		std::cout << "SPAWNED" << std::endl;
		std::cout << "Active count: " << std::to_string(active.size()) << std::endl;
	}
	else
	{
		std::cout << "ERROR: Bullet could not be spawned. Max bullets reached. " << std::endl;
	}

	return bullet;
}

void BulletPool::Despawn(Bullet* bullet)
{
	bullet->active = false;
	inactive.push_back(bullet);

	for (std::vector<Bullet*>::iterator myiter = active.begin(); myiter != active.end(); myiter++)
	{
		if (*myiter == bullet)
		{
			active.erase(myiter);
			std::cout << "DESPAWNED" << std::endl;
			std::cout << "Active count: " << std::to_string(active.size()) << std::endl;
			return;
		}
	}

}
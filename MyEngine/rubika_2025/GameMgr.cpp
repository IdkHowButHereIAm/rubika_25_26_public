#include "GameMgr.h"

#include "Entity.h"

GameMgr::GameMgr()
{}

GameMgr::~GameMgr()
{
	for (Entity* e : Entities)
	{
		e->Destroy();
		delete e;
	}

	Entities.clear();
}

void GameMgr::Update(float deltaTime) const
{
	for (Entity* e : Entities)
	{
		e->Update(deltaTime);
	}
}

void GameMgr::Draw(sf::RenderWindow& window)
{
	for (Entity* e : Entities)
	{
		e->Draw(window);
	}
}

void GameMgr::AddEntity(Entity* entity)
{
	if (!entity)
	{
		return;
	}

	Entities.push_back(entity);
	entity->Start();
}

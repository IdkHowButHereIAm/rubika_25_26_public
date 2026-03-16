#pragma once

#include <vector>

namespace sf
{
	class RenderWindow;
};

class Entity;

class GameMgr
{
public:
	GameMgr();
	~GameMgr();

	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window);

	void AddEntity(Entity* entity);
private:
	std::vector<Entity*> Entities;
};
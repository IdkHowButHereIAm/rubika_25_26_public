#pragma once

#include <vector>

namespace sf
{
	class RenderWindow;
};

class Entity;

// This all class is temporary and will be reworked later
class GameMgr
{
public:
	GameMgr();
	~GameMgr();

	void Update(float deltaTime) const;
	void Draw(sf::RenderWindow& window);

	void AddEntity(Entity* entity);
	void DeleteEntity();
private:
	std::vector<Entity*> Entities;
};
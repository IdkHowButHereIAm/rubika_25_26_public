#pragma once

#include <Engine/Gameplay/Component/Transform/Transform.h>

#include <vector>
#include <string>

namespace sf
{
	class RenderWindow;
}

class Entity
{
public:
	Entity(std::string friendlyName = "");
	virtual ~Entity();

	virtual void Start();

	virtual void Update(float fDeltaTime);

	virtual void Destroy();

	template <typename C>
	C* AddComponent();

	template <typename C>
	C* GetComponent() const;

	void Draw(sf::RenderWindow& window) const;

protected:

	std::vector<IComponent*> Components;
	std::string FriendlyName;
};

#include "Entity.hxx"
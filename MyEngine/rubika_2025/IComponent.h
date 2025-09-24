#pragma once

#include <string>

namespace sf
{
	class RenderWindow;
};

class Entity;

class IComponent abstract
{
public:
	IComponent() = delete;
	IComponent(Entity& entity);

	virtual ~IComponent();

	virtual void Start() = 0;

	virtual void Update(float fDeltaTime) = 0;

	virtual void Draw(sf::RenderWindow& window) const = 0;

	virtual void Destroy() = 0;

protected:
	Entity& GetEntity() const;;

private:
	Entity& Parent;
};

 IComponent::IComponent(Entity& entity): Parent()
{
}

inline Entity& IComponent::GetEntity() const
{ return Parent; }

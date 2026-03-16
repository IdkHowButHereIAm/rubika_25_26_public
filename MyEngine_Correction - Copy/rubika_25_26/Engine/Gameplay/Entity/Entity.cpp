#include "Entity.h"

#include <Engine/Gameplay/Component/IComponent.h>
#include <Engine/Gameplay/Component/Transform/Transform.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Engine/Gameplay/Component/Renderer/Renderer.h>

Entity::Entity(std::string friendlyName): FriendlyName(friendlyName)
{}

Entity::~Entity()
{
	for (const IComponent* component : Components)
	{
		delete component;
	}

	Components.clear();
}

void Entity::Start()
{
	for (IComponent* c : Components)
	{
		c->Start();
	}
}

void Entity::Update(float fDeltaTime)
{
	for (IComponent* c : Components)
	{
		c->Update(fDeltaTime);
	}
}

void Entity::Destroy()
{
	for (IComponent* c : Components)
	{
		c->Destroy();
	}
}

void Entity::Draw(sf::RenderWindow& window) const
{
	Renderer* rendererComp = GetComponent<Renderer>();
	if (!rendererComp)
	{
		return;
	}

	rendererComp->Draw(window);
}

#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Transform.hpp>

#include "Entity.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"


Entity::Entity(std::string friendlyName)
{
    friendlyName = friendlyName;
    AddComponent<TransformComponent>();
    AddComponent<SpriteComponent>();
}

Entity::~Entity()
{
}

void Entity::Start()
{
}

void Entity::Update(float fDeltaTime)
{
}

void Entity::Destroy()
{
}

void Entity::Draw(sf::RenderWindow& window) const
{
}

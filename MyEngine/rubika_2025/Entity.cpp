#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Transform.hpp>

#include "Entity.h"

#include <SFML/Graphics/RenderWindow.hpp>

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
    for (const auto element : Components)
    {
        element->Update(fDeltaTime);
    }
}

void Entity::Destroy()
{
}

void Entity::Draw(sf::RenderWindow& window) const
{
    for (const auto element : Components)
    {
        element->Draw(window);
    }
}

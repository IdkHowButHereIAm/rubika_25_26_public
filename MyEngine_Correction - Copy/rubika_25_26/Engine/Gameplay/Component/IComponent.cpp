#include "IComponent.h"

#include <Engine/Gameplay/Entity/Entity.h>

IComponent::IComponent(Entity& entity): ParentEntity{ entity }
{}

IComponent::~IComponent()
{}

Entity& IComponent::GetEntity() const
{
	return ParentEntity;
}

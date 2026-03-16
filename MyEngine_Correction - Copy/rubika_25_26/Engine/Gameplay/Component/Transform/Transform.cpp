#include "Transform.h"

#include <Engine/Gameplay/Entity/Entity.h>

Transform::Transform(Entity& entity) : IComponent(entity)
{
	WorldPosition = sf::Vector2f();
	Rotation = 0.f;
	Scale = sf::Vector2f(1.f, 1.f);

	UpdateMatrix();
}


Transform::~Transform()
{}

const sf::Transform& Transform::GetMatrix() const
{
	return Matrix;
}

void Transform::SetWorldPosition(sf::Vector2f newPosition)
{
	WorldPosition = newPosition;
	UpdateMatrix();
}

sf::Vector2f Transform::GetWorldPosition() const
{
	return WorldPosition;
}

void Transform::SetRotation(float newRotation)
{
	Rotation = newRotation;
	UpdateMatrix();
}

float Transform::GetRotation() const
{
	return Rotation;
}

void Transform::SetScale(sf::Vector2f newScale)
{
	Scale = newScale;
	UpdateMatrix();
}

sf::Vector2f Transform::GetScale() const
{
	return Scale;
}

void Transform::Start()
{}

void Transform::Update(float fDeltaTime)
{}

void Transform::Destroy()
{}

void Transform::UpdateMatrix()
{
	Matrix = sf::Transform::Identity;
	Matrix.translate(WorldPosition);
	Matrix.rotate(sf::degrees(Rotation));
	Matrix.scale(Scale);
}

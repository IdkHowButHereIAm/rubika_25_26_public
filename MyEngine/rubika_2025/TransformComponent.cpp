#include "TransformComponent.h"

#include "Entity.h"

TransformComponent::TransformComponent(Entity& entity) : IComponent(entity)
{
	Position = sf::Vector2f();
	Rotation = sf::Angle();
	Scale = sf::Vector2f(1.f, 1.f);

	UpdateMatrix();
}


TransformComponent::~TransformComponent()
{}

const sf::Transform& TransformComponent::GetMatrix() const
{
	return Matrix;
}

void TransformComponent::SetPosition(const sf::Vector2f& newPosition)
{
	Position = newPosition;
	UpdateMatrix();
}

sf::Vector2f TransformComponent::GetPosition() const
{
	return Position;
}

void TransformComponent::SetRotation(const sf::Angle& newRotation)
{
	Rotation = newRotation;
	UpdateMatrix();
}

sf::Angle TransformComponent::GetRotation() const
{
	return Rotation;
}

void TransformComponent::SetScale(sf::Vector2f newScale)
{
	Scale = newScale;
	UpdateMatrix();
}

sf::Vector2f TransformComponent::GetScale() const
{
	return Scale;
}

void TransformComponent::Start()
{}

void TransformComponent::Update(float fDeltaTime)
{}

void TransformComponent::Destroy()
{}

void TransformComponent::Draw(sf::RenderWindow & window) const
{}

void TransformComponent::UpdateMatrix()
{
	Matrix = sf::Transform::Identity;
	Matrix.translate(Position);
	Matrix.rotate(Rotation);
	Matrix.scale(Scale);
}

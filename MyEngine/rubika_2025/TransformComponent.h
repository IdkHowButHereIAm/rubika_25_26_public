#pragma once

#include "IComponent.h"

#include <SFML/Graphics/Transformable.hpp>

class TransformComponent : public IComponent
{
public:
	TransformComponent(Entity& entity);
	~TransformComponent();

	const sf::Transform& GetMatrix() const;

	void SetPosition(const sf::Vector2f& newPosition);
	sf::Vector2f GetPosition() const;

	void SetRotation(const sf::Angle& newRotation);
	sf::Angle GetRotation() const;

	void SetScale(sf::Vector2f newScale);
	sf::Vector2f GetScale() const;

	virtual void Start() override;
	virtual void Update(float fDeltaTime) override;
	virtual void Destroy() override;
	virtual void Draw(sf::RenderWindow& window) const override;

private:
	sf::Transform Matrix;

	sf::Vector2f Position;
	sf::Angle Rotation;
	sf::Vector2f Scale;

	void UpdateMatrix();
};

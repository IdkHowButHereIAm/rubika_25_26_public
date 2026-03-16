#pragma once

#include <Engine/Gameplay/Component/IComponent.h>

#include <SFML/Graphics/Transform.hpp>

class Transform : public IComponent
{
public:
	Transform(Entity& entity);
	~Transform();

	struct DecomposedData
	{
		sf::Vector2f Position;
		float Rotation;
		sf::Vector2f Scale;
	};

	const sf::Transform& GetMatrix() const;

	void SetWorldPosition(sf::Vector2f newPosition);
	sf::Vector2f GetWorldPosition() const;

	void SetRotation(float newRotation);
	float GetRotation() const;

	void SetScale(sf::Vector2f newScale);
	sf::Vector2f GetScale() const;

	virtual void Start() override;
	virtual void Update(float fDeltaTime) override;
	virtual void Destroy() override;

private:
	sf::Transform Matrix;

	sf::Vector2f WorldPosition;
	float Rotation;
	sf::Vector2f Scale;

	void UpdateMatrix();
};

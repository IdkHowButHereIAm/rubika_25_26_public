#pragma once

#include "IComponent.h"
#include <string>

class SpriteComponent : public IComponent
{
public:
	SpriteComponent();
	~SpriteComponent();

	virtual void Start() override;

	virtual void Update(float fDeltaTime) override;

	virtual void Draw(sf::RenderWindow& window) const override;

	virtual void Destroy() override;

	void SetTexture(const std::string& textureName);
	void SetAnimation(const std::string& animationName);

	void PlayAnimation(bool bPause);
};
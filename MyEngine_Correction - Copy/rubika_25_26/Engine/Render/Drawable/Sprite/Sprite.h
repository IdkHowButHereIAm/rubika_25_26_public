#pragma once

#include <Engine/Render/Drawable/IDrawable.h>
#include <Engine/Render/Ressource/TextureMgr.h>

#include <SFML/Graphics/Sprite.hpp>
#include <string>


class Sprite : public IDrawable
{
public:
	Sprite();
	~Sprite();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	void Reset();

	void SetTexture(const std::string& textureName);
	void SetAnimation(const std::string& animationName);

	void EnableAnimation(bool play);

protected:

	sf::Sprite* DrawableCasted;
	AnimationData CurrentAnimationData;

	std::string CurrentTexture;
	std::string CurrentAnimation;

	int CurrentAnimationNb = 0;
	float CurrentAnimationTime = 0.f;

	bool PlayAnimation;
};
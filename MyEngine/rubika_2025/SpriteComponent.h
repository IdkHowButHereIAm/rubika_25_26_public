#pragma once

#include "IComponent.h"
#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "TextureMgr.h"

class SpriteComponent : public IComponent
{
public:
	SpriteComponent(Entity& entity);
	~SpriteComponent() override;

	virtual void Start() override;

	virtual void Update(float fDeltaTime) override;

	virtual void Destroy() override;

	virtual void Draw(sf::RenderWindow& window) const override;

	void SetTexture(const std::string& textureName);
	void SetAnimation(const std::string& animationName);
	void PlayAnimation(bool bPause);

	private:
	sTextureData m_p_texture_;
	sAnimationData animation_data_;
	float trueIndex = 0;
	bool isPlaying;
	mutable int currentAnimIndex = 0;
};

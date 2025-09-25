#include "SpriteComponent.h"

#include <rapidxml/rapidxml.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Globals.h"
#include "TextureMgr.h"


SpriteComponent::SpriteComponent(Entity& entity) : IComponent(entity)
{
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::Start()
{
}

void SpriteComponent::Update(float fDeltaTime)
{
}

void SpriteComponent::Destroy()
{
}

void SpriteComponent::Draw(sf::RenderWindow& window) const
{
    sf::Sprite sprite(m_p_texture_.Texture);
    sprite.setPosition({10.f, 50.f});
    window.draw(sprite);
}

void SpriteComponent::SetTexture(const std::string& textureName)
{
    m_p_texture_ =  Globals::Instance().TextureMgr->GetTextureData(textureName);
}

void SpriteComponent::SetAnimation(const std::string& animationName)
{
    animation_data_ = m_p_texture_.AnimationData.find(animationName)->second;
}

void SpriteComponent::PlayAnimation(bool bPause)
{
    
}
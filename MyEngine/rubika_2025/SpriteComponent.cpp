#include "SpriteComponent.h"

#include <rapidxml/rapidxml.hpp>

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
}

void SpriteComponent::SetTexture(const std::string& textureName)
{
    m_p_texture_ =  Globals::Instance().TextureMgr->GetTextureData(textureName);
}

void SpriteComponent::SetAnimation(const std::string& animationName)
{
    
}

void SpriteComponent::PlayAnimation(bool bPause)
{
    
}
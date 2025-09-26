#include "SpriteComponent.h"

#include <rapidxml/rapidxml.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Entity.h"
#include "Globals.h"
#include "TextureMgr.h"
#include "TransformComponent.h"


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
    trueIndex += fDeltaTime;
    if (trueIndex >= animation_data_.TimeBetweenAnimationInS)
    {
        trueIndex -= animation_data_.TimeBetweenAnimationInS;
        currentAnimIndex++;
    }
    currentAnimIndex = (currentAnimIndex % animation_data_.AnimationSpriteCount);
}

void SpriteComponent::Destroy()
{
}

void SpriteComponent::Draw(sf::RenderWindow& window) const
{
    
    sf::Sprite sprite(m_p_texture_.Texture);
    sprite.setTextureRect(sf::IntRect(
        {animation_data_.StartX + (animation_data_.OffsetX + animation_data_.SizeX)*currentAnimIndex, animation_data_.StartY},
        {animation_data_.SizeX, animation_data_.SizeY}));

    sf::RenderStates render_state;
    render_state.transform = GetEntity().GetComponent<TransformComponent>()->GetMatrix();

    window.draw(sprite, render_state);
    
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
    isPlaying = bPause;
}
#include "Sprite.h"

#include <Engine/Globals.h>
#include <Engine/Render/Ressource/TextureMgr.h>

Sprite::Sprite(): IDrawable(), DrawableCasted(nullptr), PlayAnimation(true)
{
	DrawableCasted = new sf::Sprite(TextureMgr::GetEmptyTexture());
	Drawable = DrawableCasted;
}

Sprite::~Sprite()
{
	if (CurrentTexture.size() != 0)
	{
		const TextureData& textureData = gData.TextureMgr->GetTextureData(CurrentTexture);
		textureData.Release();
	}

	IDrawable::~IDrawable();
}

void Sprite::Start()
{}

void Sprite::Update(float deltaTime)
{
	if (PlayAnimation)
	{
		CurrentAnimationTime += deltaTime;
		if (CurrentAnimationTime >= CurrentAnimationData.TimeBetweenAnimationInS)
		{
			++CurrentAnimationNb;
			if (CurrentAnimationNb >= CurrentAnimationData.AnimationSpriteCount)
			{
				CurrentAnimationNb = 0;
			}
			CurrentAnimationTime = 0;
		}
	}

	unsigned line = CurrentAnimationNb / CurrentAnimationData.SpriteOnLine;
	unsigned column = CurrentAnimationNb % CurrentAnimationData.SpriteOnLine;

	sf::IntRect rect;

	if (!CurrentAnimationData.IsReverted)
	{
		rect.size.y = CurrentAnimationData.SizeY;
		rect.position.y = CurrentAnimationData.StartY + line * (CurrentAnimationData.OffsetY + CurrentAnimationData.SizeY);

		rect.size.x = CurrentAnimationData.SizeX;
		rect.position.x = CurrentAnimationData.StartX + column * (CurrentAnimationData.OffsetX + CurrentAnimationData.SizeX);
	}
	else
	{
		rect.size.y = CurrentAnimationData.SizeY;
		rect.position.y = CurrentAnimationData.StartY + line * (CurrentAnimationData.OffsetY + CurrentAnimationData.SizeY);
		
		rect.size.x = -CurrentAnimationData.SizeX;
		rect.position.x = CurrentAnimationData.StartX + column * (CurrentAnimationData.OffsetX + CurrentAnimationData.SizeX) + CurrentAnimationData.SizeX;
	}

	DrawableCasted->setTextureRect(rect);
}

void Sprite::Reset()
{
	CurrentAnimationTime = 0.f;
	CurrentAnimationNb = 0;
}

void Sprite::SetTexture(const std::string& textureName)
{
	if (CurrentTexture.size() != 0)
	{
		const TextureData& textureData = gData.TextureMgr->GetTextureData(CurrentTexture);
		textureData.Release();
	}

	const TextureData& textureData = gData.TextureMgr->GetTextureData(textureName);
	DrawableCasted->setTexture(textureData.Texture);
	textureData.AddRef();

	CurrentTexture = textureName;
}

void Sprite::SetAnimation(const std::string& animationName)
{
	const TextureData& textureData = gData.TextureMgr->GetTextureData(CurrentTexture);
	CurrentAnimationData  = textureData.AnimationsData.at(animationName);

	CurrentAnimation = animationName;
}

void Sprite::EnableAnimation(bool enable)
{
	PlayAnimation = enable;
}

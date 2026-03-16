#include "StaticRectangle.h"

#include <Engine/Globals.h>
#include <Engine/Render/Ressource/TextureMgr.h>

StaticRectangle::StaticRectangle(): DrawableCasted(nullptr), CurrentTexture(""), CurrentTile(""), TileData()
{
	DrawableCasted = new sf::RectangleShape();
	Drawable = DrawableCasted;
}

StaticRectangle::~StaticRectangle()
{
	if (CurrentTexture.size() != 0)
	{
		const TextureData& textureData = gData.TextureMgr->GetTextureData(CurrentTexture);
		textureData.Release();
	}

	if (DrawableCasted)
	{
		DrawableCasted = nullptr;
	}
}

void StaticRectangle::Start()
{
	sf::IntRect rect;
	if (!TileData.IsRevertedX)
	{
		rect.size.x = TileData.SizeX;
		rect.position.x = TileData.StartX;
	}
	else
	{
		rect.size.x = -TileData.SizeX;
		rect.position.x = TileData.StartX + TileData.SizeX;
	}

	if (!TileData.IsRevertedY)
	{
		rect.size.y = TileData.SizeY;
		rect.position.y = TileData.StartY;
	}
	else
	{
		rect.size.y = -TileData.SizeY;
		rect.position.y = TileData.StartY + TileData.SizeY;
	}

	DrawableCasted->setSize(sf::Vector2f((float)TileData.SizeX, (float) TileData.SizeY));
	DrawableCasted->setTextureRect(rect);
}

void StaticRectangle::Update(float)
{}

void StaticRectangle::SetTexture(const std::string& textureName)
{
	if (CurrentTexture.size() != 0)
	{
		const TextureData& textureData = gData.TextureMgr->GetTextureData(textureName);
		textureData.Release();
	}

	const TextureData& textureData = gData.TextureMgr->GetTextureData(textureName);
	DrawableCasted->setTexture(&textureData.Texture);
	textureData.AddRef();

	CurrentTexture = textureName;
}

void StaticRectangle::SetTile(const std::string& tileNameName)
{
	const TextureData& textureData = gData.TextureMgr->GetTextureData(CurrentTexture);
	TileData = textureData.StaticTilesData.at(tileNameName);

	CurrentTile = tileNameName;
}

void StaticRectangle::SetFillColor(sf::Color color)
{
	DrawableCasted->setFillColor(color);
}

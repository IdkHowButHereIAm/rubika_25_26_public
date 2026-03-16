#pragma once

#include <Engine/Render/Drawable/IDrawable.h>
#include <Engine/Render/Ressource/TextureMgr.h>
#include <SFML/Graphics/RectangleShape.hpp>

#include <string>

class StaticRectangle : public IDrawable
{
public:
	StaticRectangle();
	~StaticRectangle();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;

	void SetTexture(const std::string& textureName);
	void SetTile(const std::string& animationName);
	void SetFillColor(sf::Color color);

protected:
	sf::RectangleShape* DrawableCasted;
	std::string CurrentTexture;
	std::string CurrentTile;

	StaticTileData TileData;
};
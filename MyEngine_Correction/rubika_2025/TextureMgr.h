#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <filesystem>
#include <unordered_map>
#include <string>

struct sAnimationData
{
	sAnimationData();

	int StartX;
	int StartY;
	int SizeX;
	int SizeY;
	int OffsetX;
	int OffsetY;
	int AnimationSpriteCount;
	int SpriteOnLine;
	bool IsReverted;
	float TimeBetweenAnimationInS;
};

/* Struct that contains any data linked to a texture.
 * From the texture itself to its metadata.
 */ 
struct sTextureData
{
	sf::Texture Texture;
	std::unordered_map<std::string, sAnimationData> AnimationData;
};

class TextureMgr final
{
public:
	/*
	 * Loads the texture using the given path, including its metadata.
	 * Obviously, this function will also store the loaded data
	 * If errors occur during the loading process, this function must return false.
	 * Return true otherwise.
	*/
	bool LoadTexture(const std::filesystem::path& path);

	/*
	 * Returns the corresponding TextureData using the name of the texture
	 */
	const sTextureData& GetTextureData(const std::string& name) const;

private:
	std::unordered_map<std::string, sTextureData> Textures;

	sTextureData MissingTexture;
};
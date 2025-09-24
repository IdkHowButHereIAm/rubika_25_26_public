#pragma once

#include <filesystem>
#include <map>
#include <SFML/Graphics/Texture.hpp>

/* Struct that contains any data linked to a texture.
 * From the texture itself to its metadata.
 */ 
struct sTextureData
{
	sf::Texture texture;

	//Parse
	sf::Vector2f position;
	sf::Vector2f size;
	sf::Vector2f offset;
	int index;
	int currentSprite;
	bool reverted;
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

	std::map<std::string, sTextureData> textures;
	sTextureData baseTexture;

	/*
	 * Returns the corresponding TextureData using the name of the texture
	 */
	const sTextureData& GetTextureData(const std::string& name) const;
	bool xml_parse(const std::filesystem::path& path);
};

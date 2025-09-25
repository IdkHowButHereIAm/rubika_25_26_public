#include "TextureMgr.h"

#include "Logger.h"
#include "Profiler.h"

#include <SFML/Graphics/Image.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <filesystem>
#include <iostream>

TextureMgr::TextureMgr()
{
	sf::Image img(sf::Vector2u(1, 1), sf::Color(255, 0, 255, 255));
	MissingTexture.Texture.loadFromImage(img);
}

TextureMgr::~TextureMgr()
{
	Textures.clear();
}

const sTextureData& TextureMgr::GetTextureData(const std::string& name) const
{
	const auto& it = Textures.find(name);
	if (it != Textures.end())
	{
		return it->second;
	}
	
	return MissingTexture;
}

bool LoadAnimationMetadata(rapidxml::xml_node<>* node, sTextureData& textureData)
{
	if (!node)
	{
		return false;
	}

	rapidxml::xml_node<>* animationNode = node->first_node();
	while (animationNode)
	{
		rapidxml::xml_attribute<>* nameAttribute = animationNode->first_attribute("Name");
		if (nameAttribute)
		{
			auto p = textureData.AnimationData.emplace(std::string(nameAttribute->value(), nameAttribute->value_size()), sAnimationData());
			if (p.second)
			{
				sAnimationData& data = p.first->second;
				if (rapidxml::xml_node<>* innerNode = animationNode->first_node("X"))
				{
					std::string value(innerNode->value(), innerNode->value_size());
					data.StartX = atoi(value.c_str());
				}

				if (rapidxml::xml_node<>* innerNode = animationNode->first_node("Y"))
				{
					std::string value(innerNode->value(), innerNode->value_size());
					data.StartY = atoi(value.c_str());
				}

				if (rapidxml::xml_node<>* innerNode = animationNode->first_node("SizeX"))
				{
					std::string value(innerNode->value(), innerNode->value_size());
					data.SizeX = atoi(value.c_str());
				}

				if (rapidxml::xml_node<>* innerNode = animationNode->first_node("SizeY"))
				{
					std::string value(innerNode->value(), innerNode->value_size());
					data.SizeY = atoi(value.c_str());
				}

				if (rapidxml::xml_node<>* innerNode = animationNode->first_node("OffsetX"))
				{
					std::string value(innerNode->value(), innerNode->value_size());
					data.OffsetX = atoi(value.c_str());
				}

				if (rapidxml::xml_node<>* innerNode = animationNode->first_node("OffsetY"))
				{
					std::string value(innerNode->value(), innerNode->value_size());
					data.OffsetY = atoi(value.c_str());
				}

				if (rapidxml::xml_node<>* innerNode = animationNode->first_node("SpriteNum"))
				{
					std::string value(innerNode->value(), innerNode->value_size());
					data.AnimationSpriteCount = atoi(value.c_str());
				}

				if (rapidxml::xml_node<>* innerNode = animationNode->first_node("SpritesOnLine"))
				{
					std::string value(innerNode->value(), innerNode->value_size());
					data.SpriteOnLine = atoi(value.c_str());
				}

				if (rapidxml::xml_node<>* innerNode = animationNode->first_node("Reverted"))
				{
					std::string value(innerNode->value(), innerNode->value_size());
					data.IsReverted = atoi(value.c_str());
				}

				if (rapidxml::xml_node<>* innerNode = animationNode->first_node("TimeBetweenAnimation"))
				{
					std::string value(innerNode->value(), innerNode->value_size());
					data.TimeBetweenAnimationInS = std::stof(value.c_str());
				}
			}
			else
			{
				char msg[128] = {};
				snprintf(msg, sizeof(msg), "LoadAnimationMetadata: Cannot add animation &s. Ignore it", nameAttribute->value());
				Logger::Error(msg);
			}
		}
		else
		{
			Logger::Error("LoadAnimationMetadata: Find a animation node with no name. Ignore it");
		}

		animationNode = animationNode->next_sibling();
	}

	return true;
}

bool LoadTextureMetadata(const std::filesystem::path& path, sTextureData& textureData)
{
	rapidxml::file<> metadataFile(path.string().c_str());
	if (metadataFile.size() == 0)
	{
		char msg[128] = {};
		snprintf(msg, sizeof(msg), "LoadTextureMetadata: Cannot open file %s", path.string().c_str());
		Logger::Error(msg);
		return false;
	}

	rapidxml::xml_document metadataXml;
	metadataXml.parse<0>(metadataFile.data());

	if (rapidxml::xml_node<>* node = metadataXml.first_node("Animations"))
	{
		if (!LoadAnimationMetadata(node, textureData))
		{
			return false;
		}
	}

	return true;
}

bool TextureMgr::LoadTexture(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path))
	{
		char msg[128] = {};
		snprintf(msg, sizeof(msg), "LoadTexture: Texture file doesn't exist %s", path.string().c_str());
		Logger::Error(msg);
		return false;
	}

	std::filesystem::path metadataPath = path;
	metadataPath = metadataPath.replace_extension(".xml");
	if (!std::filesystem::exists(metadataPath.native()))
	{
		char msg[128] = {};
		snprintf(msg, sizeof(msg), "LoadTexture: Texture metadata file doesn't exist %s", metadataPath.string().c_str());
		Logger::Error(msg);
		return false;
	}

	auto p = Textures.emplace(path.string(), sTextureData());
	if (!p.second)
	{
		Logger::Error("LoadTexture: Internal error. Cannot emplace in map");
		return false;
	}

	sTextureData& textureData = p.first->second;
	if (!textureData.Texture.loadFromFile(path.string().c_str()))
	{
		return false;
	}

	if (!LoadTextureMetadata(metadataPath, textureData))
	{
		return false;
	}

	return true;
}


sAnimationData::sAnimationData() : StartX(0), StartY(0), SizeX(0), SizeY(0),
OffsetX(0), OffsetY(0), AnimationSpriteCount(0), SpriteOnLine(0), IsReverted(false)
{}

sTextureData::sTextureData() : Texture(), AnimationData()
{}

sTextureData::~sTextureData()
{
	AnimationData.clear();
}

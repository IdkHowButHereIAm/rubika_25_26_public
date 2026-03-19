#include "TextureMgr.h"

#include <Engine/Globals.h>
#include <Engine/Task/TaskMgr.h>
#include <Engine/Profiler.h>
#include <rapidxml/rapidxml_utils.hpp>

#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>
#endif

#include <assert.h>
#include <filesystem>
#include <iostream>

TextureMgr::TextureMgr()
{}

TextureMgr::~TextureMgr()
{
	for (const auto& p : Textures)
	{
		p.second.Release();
		assert(p.second.RefCount == 0);
	}

	Textures.clear();
}

void TextureMgr::Init()
{
	gData.DebugMgr->RegisterDebugableWindow("TextureMgr", this);
}

void TextureMgr::Update()
{
	CallbacksNextFrameMutex.lock();
	std::set<std::filesystem::path> localCallkacks;
	std::swap(localCallkacks, CallbacksNextFrame);
	CallbacksNextFrameMutex.unlock();

	for (const std::filesystem::path& id : localCallkacks)
	{
		auto it = Requesting.find(id);
		if (it != Requesting.end())
		{
			const sLoadCallback& data = it->second;
			TextureData* textureData = GetTextureData(id.string());
			if (textureData)
			{
				PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLACK, "Load Texture from main");

				if (textureData->Image.getPixelsPtr())
				{
					if (!textureData->Texture.loadFromImage(textureData->Image))
					{
						assert(false);
					}
				}
				else
				{
					textureData->Texture = GetMissingTexture();
				}
				PROFILER_EVENT_END();

			}

			for (int i = 0; i < data.Callbacks.size(); ++i)
			{
				gData.TaskMgr->RegisterTask([f = data.Callbacks[i], textureData]()
					{
						f(textureData);
					}, TaskMgr::ePhase::Worker);
			}

			Requesting.erase(it);
		}
	}
}

void TextureMgr::Shut()
{
	gData.DebugMgr->UnregisterDebugableWindow("TextureMgr");
}

bool TextureMgr::LoadTexture(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path.native()))
	{
		std::cerr << "Texture file doesn't exist " << path << std::endl;
		return false;
	}

	std::filesystem::path metadataPath = path;
	metadataPath = metadataPath.replace_extension(".xml");
	if (!std::filesystem::exists(metadataPath.native()))
	{
		std::cerr << "Texture metadata file doesn't exist " << metadataPath << std::endl;
		return false;
	}

	auto p = Textures.emplace(std::piecewise_construct, 
				std::forward_as_tuple(path.string()),
				std::forward_as_tuple());

	if (!p.second)
	{
		std::cerr << "LoadTexture: Internal error. Cannot emplace in map" << std::endl;
		return false;
	}
	
	TextureData& textureData = p.first->second;
	textureData.AddRef();
	if (!textureData.Texture.loadFromFile(path.string()))
	{
		return false;
	}
	
	if (!LoadTextureMetadata(metadataPath, textureData))
	{
		return false;
	}

	return true;
}

void TextureMgr::LoadTextureAsync(const std::filesystem::path& path, TextureLoadedCallback callback)
{
	const TextureData* texture = GetTextureData(path.string());
	if (texture)
	{
		gData.TaskMgr->RegisterTask([this, path, callback]()
			{
				const TextureData* texture = GetTextureData(path.string());
				callback(texture);
			}, TaskMgr::ePhase::Worker);
		return;
	}

	auto it = Requesting.find(path);
	if (it == Requesting.end())
	{
		auto itEmplace = Requesting.emplace(std::piecewise_construct,
			std::forward_as_tuple(path),
			std::forward_as_tuple());
		if (itEmplace.second)
		{
			sLoadCallback& data = itEmplace.first->second;
			data.Callbacks.push_back(callback);

			gData.TaskMgr->RegisterTask([this, path]()
				{
					LoadTexture_Thread(path);
				}, TaskMgr::ePhase::Worker);
		}
		else
		{
			gData.TaskMgr->RegisterTask([callback]()
				{
					callback(nullptr);
				}, TaskMgr::ePhase::Worker);
		}
	}
	else
	{
		sLoadCallback& data = it->second;
		data.Callbacks.push_back(callback);
	}
}

const TextureData* TextureMgr::GetTextureData(const std::string& name) const
{
	const TextureData* ret = nullptr;

	TexturesMutex.lock();
	auto it = Textures.find(name);
	if (it != Textures.end())
	{
		ret = &it->second;
	}
	TexturesMutex.unlock();

	return ret;
}

TextureData* TextureMgr::GetTextureData(const std::string& name)
{
	TextureData* ret = nullptr;

	TexturesMutex.lock();
	auto it = Textures.find(name);
	if (it != Textures.end())
	{
		ret = &it->second;
	}
	TexturesMutex.unlock();

	return ret;
}

sf::Texture emptyTexture;

const sf::Texture& TextureMgr::GetEmptyTexture()
{
	return emptyTexture;
}

const sf::Texture& TextureMgr::GetMissingTexture()
{
	return emptyTexture;
}

bool TextureMgr::CheckTextureDependencies(const std::filesystem::path& texturePath)
{
	if (!std::filesystem::exists(texturePath))
	{
		return false;
	}

	std::filesystem::path metadataPath = texturePath;
	metadataPath = metadataPath.replace_extension(".xml");
	if (!std::filesystem::exists(metadataPath))
	{
		return false;
	}

	return true;
}

bool TextureMgr::LoadTextureAndDependencies(const std::filesystem::path& texturePath)
{
	PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLACK, "LoadTextureAndDependencies %s", texturePath.c_str());

	PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLUE, "Load Image");

	TexturesMutex.lock();
	auto p = Textures.emplace(std::piecewise_construct,
		std::forward_as_tuple(texturePath.string()),
		std::forward_as_tuple());
	TextureData& textureData = p.first->second;
	TexturesMutex.unlock();

	if (!textureData.Image.loadFromFile(texturePath.generic_string()))
	{
		PROFILER_EVENT_END();
		PROFILER_EVENT_END();
		return false;
	}
	PROFILER_EVENT_END();

	PROFILER_EVENT_BEGIN(PROFILER_COLOR_GREEN, "Load Metadata");

	std::filesystem::path metadataPath = texturePath;
	metadataPath = metadataPath.replace_extension(".xml");
	if (!LoadTextureMetadata(metadataPath, textureData))
	{
		PROFILER_EVENT_END();
		PROFILER_EVENT_END();

		return false;
	}
	PROFILER_EVENT_END();

	PROFILER_EVENT_END();
	return true;
}


bool TextureMgr::LoadTextureMetadata(const std::filesystem::path& path, TextureData& textureData)
{
	rapidxml::file<> metadataFile(path.string().c_str());
	if (metadataFile.size() == 0)
	{
		std::cerr << "LoadTextureMetadata: Cannot open file " << path << std::endl;
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

	if (rapidxml::xml_node<>* node = metadataXml.first_node("Backgrounds"))
	{
		if (!LoadStaticTileMetadata(node, textureData))
		{
			return false;
		}
	}

	return true;
}

bool TextureMgr::LoadAnimationMetadata(rapidxml::xml_node<>* node, TextureData& textureData)
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
			auto p = textureData.AnimationsData.emplace(std::string(nameAttribute->value(), nameAttribute->value_size()), AnimationData());
			if (p.second)
			{
				AnimationData& data = p.first->second;
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
				std::cerr << "LoadAnimationMetadata: Cannot add animation " << nameAttribute->value() << ". Ignore it" << std::endl;
			}
		}
		else
		{
			std::cerr << "LoadAnimationMetadata: Find a animation node with no name. Ignore it" << std::endl;
		}

		animationNode = animationNode->next_sibling();
	}

	return true;
}

bool TextureMgr::LoadStaticTileMetadata(rapidxml::xml_node<>* node, TextureData& textureData)
{
	if (!node)
	{
		return false;
	}

	rapidxml::xml_node<>* tileNode = node->first_node();
	while (tileNode)
	{
		rapidxml::xml_attribute<>* nameAttribute = tileNode->first_attribute("Name");
		if (nameAttribute)
		{
			auto p = textureData.StaticTilesData.emplace(std::string(nameAttribute->value(), nameAttribute->value_size()), StaticTileData());
			if (p.second)
			{
				StaticTileData& data = p.first->second;
				if (rapidxml::xml_node<>* innerNode = tileNode->first_node("X"))
				{
					std::string value(innerNode->value(), innerNode->value_size());
					data.StartX = atoi(value.c_str());
				}

				if (rapidxml::xml_node<>* innerNode = tileNode->first_node("Y"))
				{
					std::string value(innerNode->value(), innerNode->value_size());
					data.StartY = atoi(value.c_str());
				}

				if (rapidxml::xml_node<>* innerNode = tileNode->first_node("SizeX"))
				{
					std::string value(innerNode->value(), innerNode->value_size());
					data.SizeX = atoi(value.c_str());
				}

				if (rapidxml::xml_node<>* innerNode = tileNode->first_node("SizeY"))
				{
					std::string value(innerNode->value(), innerNode->value_size());
					data.SizeY = atoi(value.c_str());
				}

				if (rapidxml::xml_node<>* innerNode = tileNode->first_node("RevertedX"))
				{
					std::string value(innerNode->value(), innerNode->value_size());
					data.IsRevertedX = atoi(value.c_str());
				}

				if (rapidxml::xml_node<>* innerNode = tileNode->first_node("RevertedY"))
				{
					std::string value(innerNode->value(), innerNode->value_size());
					data.IsRevertedY = atoi(value.c_str());
				}
			}
			else
			{
				std::cerr << "LoadStaticTileMetadata: Cannot add static Data " << nameAttribute->value() << ". Ignore it" << std::endl;
			}
		}
		else
		{
			std::cerr << "LoadStaticTileMetadata: Find a tile node with no name. Ignore it" << std::endl;
		}

		tileNode = tileNode->next_sibling();
	}

	return true;
}

void TextureMgr::LoadTexture_Thread(const std::filesystem::path& path)
{
	PROFILER_EVENT_BEGIN(PROFILER_COLOR_RED, "Requesting %s", path.filename().c_str());

	if (!CheckTextureDependencies(path) || !LoadTextureAndDependencies(path))
	{
		TexturesMutex.lock();
		auto p = Textures.emplace(std::piecewise_construct,
			std::forward_as_tuple(path.string()),
			std::forward_as_tuple());
		TexturesMutex.unlock();
	}

	CallbacksNextFrameMutex.lock();
	CallbacksNextFrame.insert(path);
	CallbacksNextFrameMutex.unlock();

	PROFILER_EVENT_END();
}

void TextureMgr::DrawDebug()
{
#ifdef _USE_IMGUI

	const auto flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders;

	if (ImGui::BeginTable("##Textures", 5, flags))
	{
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Width");
		ImGui::TableSetupColumn("Height");
		ImGui::TableSetupColumn("RefCount");
		ImGui::TableSetupColumn("Image");
		ImGui::TableHeadersRow();

		for (const auto& [name, data] : Textures)
		{
			int count = data.RefCount;

			ImVec4 color = count > 1 ? ImVec4(255, 255, 255, 255) : ImVec4(200, 0, 0, 255);
			ImGui::PushStyleColor(ImGuiCol_Text, color);

			ImGui::TableNextColumn();
			ImGui::TextWrapped(name.c_str());
			ImGui::TableNextColumn();
			ImGui::TextWrapped("%d", data.Texture.getSize().x);
			ImGui::TableNextColumn();
			ImGui::TextWrapped("%d", data.Texture.getSize().y);
			ImGui::TableNextColumn();
			ImGui::TextWrapped("%d", count);
			ImGui::TableNextColumn();

			const sf::Vector2f size((float)data.Texture.getSize().x, (float) data.Texture.getSize().y);
			const float displayMaxSizeX = 200;
			const float displayMaxSizeY = 200;

			sf::Vector2f displaySize;
			if (size.x < displayMaxSizeX && size.y < displayMaxSizeY)
			{
				displaySize = size;
			}
			else if (size.x > size.y)
			{
				displaySize.x = displayMaxSizeX;
				displaySize.y = size.y / size.x * displayMaxSizeY;
			}
			else
			{
				displaySize.x = size.x / size.y * displayMaxSizeX;
				displaySize.y = displayMaxSizeY;
			}

			ImGui::Image(data.Texture, displaySize);

			ImGui::PopStyleColor();

			ImGui::TableNextRow();
		}
		ImGui::EndTable();
	}

#endif
}

AnimationData::AnimationData(): StartX(0), StartY(0), SizeX(0), SizeY(0),
OffsetX(0), OffsetY(0), AnimationSpriteCount(0), SpriteOnLine(0), IsReverted(false)
{}

TextureData::TextureData(): Texture(), AnimationsData(), RefCount(0)
{}

TextureData::~TextureData()
{
	AnimationsData.clear();
}

void TextureData::AddRef() const
{
	RefCount++;
}

void TextureData::Release() const
{
	assert(RefCount > 0);
	RefCount--;
}

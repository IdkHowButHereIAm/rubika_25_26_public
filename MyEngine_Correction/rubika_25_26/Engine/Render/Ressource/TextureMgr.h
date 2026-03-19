#pragma once

#include <Engine/Debug/DebugMgr.h>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <rapidxml/rapidxml.hpp>

#include <unordered_map>
#include <filesystem>
#include <string>
#include <atomic>
#include <functional>
#include <set>
#include <mutex>

struct AnimationData
{
	AnimationData();

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

struct StaticTileData
{
	int StartX;
	int StartY;
	int SizeX;
	int SizeY;
	bool IsRevertedX;
	bool IsRevertedY;
};

struct TextureData
{
	TextureData();
	~TextureData();

	sf::Texture Texture;
	sf::Image Image;
	std::unordered_map<std::string, AnimationData> AnimationsData;
	std::unordered_map<std::string, StaticTileData> StaticTilesData;

	void AddRef() const;
	void Release() const;

	friend class TextureMgr;
private:
	mutable std::atomic<int> RefCount;
};

class TextureMgr final : public IDebugable
{
public:

	TextureMgr();
	~TextureMgr();

	void Init();
	void Update();
	void Shut();

	using TextureLoadedCallback = std::function<void(const TextureData*)>;

	bool LoadTexture(const std::filesystem::path& path);
	void LoadTextureAsync(const std::filesystem::path& path, TextureLoadedCallback callback);

	const TextureData* GetTextureData(const std::string& name) const;
	TextureData* GetTextureData(const std::string& name);

	static const sf::Texture& GetEmptyTexture();
	static const sf::Texture& GetMissingTexture();

	virtual void DrawDebug() override;

private:
	std::unordered_map<std::string, TextureData> Textures;

	bool CheckTextureDependencies(const std::filesystem::path& texturePath);
	bool LoadTextureAndDependencies(const std::filesystem::path& texturePath);
	bool LoadTextureMetadata(const std::filesystem::path& path, TextureData& textureData);
	bool LoadAnimationMetadata(rapidxml::xml_node<>* node, TextureData& textureData);
	bool LoadStaticTileMetadata(rapidxml::xml_node<>* node, TextureData& textureData);

	struct sLoadCallback
	{
		std::vector<TextureLoadedCallback> Callbacks;
	};
	std::unordered_map<std::filesystem::path, sLoadCallback> Requesting;
	std::set<std::filesystem::path> CallbacksNextFrame;

	mutable std::mutex TexturesMutex;
	mutable std::mutex CallbacksNextFrameMutex;

	void LoadTexture_Thread(const std::filesystem::path& path);
};
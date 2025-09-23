#include "TextureMgr.h"
#include <filesystem>

bool TextureMgr::LoadTexture(const std::filesystem::path& path)
{
   if (  !std::filesystem::exists(path))
      return false;
   
   sTextureData tex;
   tex.texture.loadFromFile(path);
   
   textures.insert_or_assign(path.stem().string(), tex);
   return true;
}

const sTextureData& TextureMgr::GetTextureData(const std::string& name) const
{
   if (!textures.contains(name))
      return baseTexture;
   
   return textures.at(name);
}

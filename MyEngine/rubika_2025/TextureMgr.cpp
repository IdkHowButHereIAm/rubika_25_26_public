#include "TextureMgr.h"
#include <filesystem>
#include "C:\Users\p.farin\Documents\GitHub\rubika_25_26_public\MyEngine\packages\rapidxml-1.13\rapidxml_utils.hpp"

bool TextureMgr::LoadTexture(const std::filesystem::path& path)
{
   if (  !std::filesystem::exists(path))
      return false;

   if (path.extension() == ".xml")
      xml_parse(path);
   else if (path.extension() == ".png" || path.extension() == ".jpg")
   {
      sTextureData tex;
      tex.texture.loadFromFile(path);
   
      textures.insert_or_assign(path.stem().string(), tex);
      return true;
   }
   else
      return false;
}

const sTextureData& TextureMgr::GetTextureData(const std::string& name) const
{
   if (!textures.contains(name))
      return baseTexture;
   
   return textures.at(name);
}

bool TextureMgr::xml_parse(const std::filesystem::path& path)
{
   sTextureData tex;

   rapidxml::file<> xml_file(path.string().c_str());
   rapidxml::xml_document<> doc;
   doc.parse<0>(xml_file.data());

   rapidxml::xml_node<> *node = doc.first_node();
   for (rapidxml::xml_node<> *child = node->first_node(); child; child = child->next_sibling())
   {
     // tex.size = vectorf ();
      
      for (rapidxml::xml_node<> *grandChild = child->first_node(); grandChild; grandChild = grandChild->next_sibling())
      {
            printf("%s\n", grandChild->name());
            printf("%s\n", grandChild->value());
      }
   }
   
   
   //textures.insert_or_assign();
   return true;
}


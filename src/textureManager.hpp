#pragma once

#include <vector>

#include "texture.hpp"

class TextureManager
{
public:
	TextureManager();
    
	unsigned int addTextureFromFile(std::string ident, ::string filename);
	unsigned int getTextureIndex(std::string ident);
	Texture& getTextureByIndex(unsigned int index);

private:
    std::vector<Texture> _textures;

};

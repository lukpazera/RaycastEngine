
#include "textureManager.hpp"

TextureManager::TextureManager()
{
	addTextureFromFile("con1", "TexConcrete1024.png");
	addTextureFromFile("con2", "TexConcrete2_1024.png");
}

unsigned int TextureManager::addTextureFromFile(std::string ident, std::string filename)
{
	Texture newTexture = Texture(ident, filename);
	_textures.push_back(newTexture);
	return _textures.size() - 1;
}

unsigned int TextureManager::getTextureIndex(std::string ident)
{
	for (std::size_t i = 0; i < _textures.size(); ++i)
	{
		if (_textures[i].getIdent() == ident) { return i; }
	}
	return 0;
}

Texture& TextureManager::getTextureByIndex(unsigned int index)
{
	return _textures[index];
}
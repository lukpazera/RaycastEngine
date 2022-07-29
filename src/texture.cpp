
#include "texture.hpp"

Texture::Texture(std::string id, ofImage& image)
{
	_ident = id;
}

Texture::Texture(std::string id, std::string filename)
{
	_texImage.setUseTexture(true);
	_texImage.load(filename);
	_texImage.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_NEAREST);

	_ident = id;
}

std::string Texture::getIdent()
{
	return _ident;
}

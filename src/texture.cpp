
#include "texture.hpp"

Texture::Texture(ofImage& image)
{
}

Texture::Texture(std::string filename)
{
    ofLoadImage(_pixels, filename);
}

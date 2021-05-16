#pragma once

/*
 * CPU texture.
 * Since all rendering is done on CPU there is no need for
 * ofImage and using ofTexture (which is GPU one).
 *
 *
 */

#include <vector>

#include <stdio.h>
#include "ofMain.h"

class Texture
{
public:
    Texture(ofImage& image);
    Texture(std::string filename);
    
private:
    /*
     * Texture map
     */
    std::vector<char> _texture;
    ofPixels _pixels;
};

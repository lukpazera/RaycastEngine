#pragma once

/*
 * Texture
 */

#include <vector>

#include <stdio.h>
#include "ofMain.h"

class Texture
{
public:
    Texture(std::string ident, ofImage& image);
    Texture(std::string ident, std::string filename);
    
	std::string getIdent();
private:
	std::string _ident;
	ofImage _texImage;
};

#pragma once

#include <string>
#include "ofMain.h"

#include "map.hpp"
#include "player.hpp"
#include "texture.hpp"
#include "renderer.hpp"
#include "ddaRaycaster.h"


class CPURenderer : public Renderer
{
public:
    //CPURenderer();
	//~CPURenderer();

    void update() override;
	void draw() override;
	void onInit() override;
	void onResolutionChanged() override;

private:
    void _allocateRenderBuffer();
    
    ofImage _buffer;
    
    ofImage _tex;
    ofImage _texMetal;
};
#pragma once

#include <string>
#include "ofMain.h"

#include "map.hpp"
#include "player.hpp"
#include "texture.hpp"
#include "renderer.hpp"
#include "ddaRaycaster.h"


class HybridRenderer : public Renderer
{
	//using Renderer::Renderer; // this should allow for the parent base renderer class to be called.
public:
    void update() override;
	void onDraw() override;
	void onInit() override;

private:
	void _drawLevelColumn(ofVec2f eye, int column, int level);

	ofImage _tex;
	ofImage _texMetal;
};
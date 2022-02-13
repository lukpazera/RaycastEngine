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
public:
    void update() override;
	void onDraw() override;
	void onInit() override;

private:
    ofImage _tex;
    ofImage _texMetal;
};
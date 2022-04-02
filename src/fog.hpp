#pragma once

#include <string>
#include "ofMain.h"

class Fog
{
public:
    Fog();

	void setColor(ofColor color);
	void applyToColumn(float x, float y, float width, float height, float distance);

private:
	void _initTexture(ofColor color);

	ofImage _fogImage;
	float _minDistance;
	float _maxDistance;

};

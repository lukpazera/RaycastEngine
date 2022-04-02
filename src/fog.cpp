#include "fog.hpp"

Fog::Fog() :
	_minDistance(6.0),
	_maxDistance(16.0)
{    
	_initTexture(ofColor(64, 64, 64));
}

void Fog::setColor(ofColor color)
{
	_initTexture(color);
}

void Fog::applyToColumn(float x, float y, float width, float height, float distance)
{
	if (distance < _minDistance) { return; }

	distance -= _minDistance;
	float fogOpacity = distance / (_maxDistance - _minDistance);
	fogOpacity = min(fogOpacity, 1.0f);

	_fogImage.drawSubsection(x, y, width, height, fogOpacity * 256.0, 0.0, 1.0, 1.0);
}

void Fog::_initTexture(ofColor color)
{
	_fogImage.allocate(256, 1, OF_IMAGE_COLOR_ALPHA);

	ofPixels &pixels = _fogImage.getPixels();
	ofColor fogColor = color;

	for (int i = 0; i < 256; i++)
	{
		fogColor.a = unsigned char(i);
		pixels.setColor(i * 4, fogColor);
	}
	_fogImage.update();
	_fogImage.getTextureReference().setTextureMinMagFilter(GL_LINEAR, GL_NEAREST);
}

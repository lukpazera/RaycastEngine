#include "HybridRenderer.hpp"

void HybridRenderer::onInit()
{
	_tex.setUseTexture(true);
	_tex.load("TexConcrete1024.png");
	_tex.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_NEAREST);
	_texMetal.setUseTexture(true);
	_texMetal.load("TexMetal1024.png");
	_texMetal.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_NEAREST);
	skyColor = ofColor(240, 140, 100);
}

void HybridRenderer::update()
{

}

void HybridRenderer::onDraw()
{
	// Background
    ofColor groundClose(88, 96, 104);
    ofColor groundFar(16, 16, 16);
    ofBackgroundGradient(groundFar, groundClose, OF_GRADIENT_BAR);
	//ofColor sky(130, 150, 165);
	ofSetColor(skyColor);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight() / 2);
    
    int columns = renderInfo.renderResolutionX;

	ofSetColor(255, 255, 255);

    float lightIntensity = (getPlayer()->getDirection().dot(lightInfo.direction) + 1.0f) / 2.0f;

	// --- Setting up FOV processing
	// Progressing through FOV needs to go in correct direction, rendering will be flipped
	// compared to the map otherwise.
	// What we do here is we pick start and end angle and create start and end eye vectors
	// out of these.
	// We are going to cast rays between these 2 eye start and end vectors.
	float startRayAngle = (getPlayer()->getLookAtAngle() + (renderInfo.fov / 2));
	float endRayAngle = (getPlayer()->getLookAtAngle() - (renderInfo.fov / 2));
	ofVec2f eyeStart(sinf(startRayAngle), cosf(startRayAngle));
	ofVec2f eyeEnd(sinf(endRayAngle), cosf(endRayAngle));

	// The idea here is that we do not go by angle step across the FOV
	// as this creates distortion.
	// What we need to do is to go in equal distance steps along the vector that goes
	// from eyeStart to eyeEnd.
	// Here we only initialize the length of that step
	// and we'll use it later in the loop to calculate eye vectors.
	ofVec2f visionLine = eyeEnd - eyeStart;
	ofVec2f visionLineNorm = visionLine.normalized();
	float visionLineLength = visionLine.length();
	float visionLineStep = visionLineLength / float(columns);

	// Note that vision line length says how many cells fit into view horizontally.
	// We can use this to know the height of a cell in pixels from any distance.	
	
	for (int x = 0; x < columns; x++)
	{
		ofVec2f eye = eyeStart + (visionLineNorm * float(x) * visionLineStep);
		if (debugDrawing) { debugFOVPoints.push_back(eye); }
		eye.normalize();

		_drawLevelColumn(eye, x, 1);
		_drawLevelColumn(eye, x, 0);
	}
}

void HybridRenderer::_drawLevelColumn(ofVec2f eye, int column, int level)
{
	// call raycast here
	RayHit hit;

	if (!raycaster.raycast(getPlayer()->getPosition(), eye, level, hit))
	{
		return;
	}

	float distanceToSurface = hit.distance;
	float sampleX = hit.texU;
	ofVec2f normal(0, 0);
	normal = hit.normal;
	char mapElement = hit.mapElement;

	// To calculate line length can't use the distance to surface directly.
	ofVec2f alongEye(eye * distanceToSurface);

	// Store ray for debug drawing for later.
	if (debugDrawing) { debugRays.push_back(alongEye); }

	float cameraDistance = getPlayer()->getDirection().dot(alongEye);
	float medianDistance = cameraDistance;

	// Calculate height of a wall in pixels.
	// This value can be way bigger than screen Y resolution.
	int wallHeight = int(float(renderInfo.cellPixelSize) / cameraDistance);

	// minY is where we should start drawing wall from vertically in pixels
	// it's unclipped here so it'll be negative when we're very close to the wall
	// maxY is where we should stop drawing wall at vertically in pixels
	// again, unclipped so will be large number, out of the bounds of the screen
	int minY = (renderInfo.renderResolutionY - wallHeight) / 2;
	int maxY = renderInfo.renderResolutionY - minY;

	// Need to offset minY maxY based on level that we are drawing
	minY -= wallHeight * level;
	maxY -= wallHeight * level;

	// Skip drawing columns that are completely off screen.
	if (minY < 0 && maxY < 0) { return; }

	float texelSize = 1024.0f / wallHeight;

	sampleX = sampleX * 1023.0f;
	if (sampleX > 1023.0f)
	{
		sampleX = 1023.0f;
	}

	// Light intensity is currently just doing dot product between
	// intersection wall normal and light direction.
	float lightIntensity = 1.0f - ((lightInfo.direction.dot(normal) + 1.0f) / 2.0f);

	// Remap light intensity to 0.25-1.0 range.
	lightIntensity *= 1.0f;
	lightIntensity += 0.5f;

	ofImage* tex;

	if ('#' == mapElement)
	{
		tex = &_tex;
	}
	else if ('$' == mapElement)
	{
		tex = &_texMetal;
	}

	tex->drawSubsection(float(column) * renderInfo.resolutionMultiplier, minY * renderInfo.resolutionMultiplier, renderInfo.resolutionMultiplier, wallHeight * renderInfo.resolutionMultiplier,
		sampleX, 0, 1, 1024);
}


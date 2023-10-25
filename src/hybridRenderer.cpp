#include "HybridRenderer.hpp"

void HybridRenderer::onInit()
{
	_tex.setUseTexture(true);
	_tex.load("TexConcrete1024.png");
	_tex.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_NEAREST);

	_texTop.setUseTexture(true);
	_texTop.load("TexConcreteTop1024.png");
	_texTop.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_NEAREST);

	_texLow.setUseTexture(true);
	_texLow.load("TexConcreteLow1024.png");
	_texLow.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_NEAREST);

	_texMetal.setUseTexture(true);
	_texMetal.load("TexConcrete2_1024.png");
	_texMetal.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_NEAREST);

	skyColor = ofColor(240, 140, 100);
	_fog.setColor(ofColor(04, 8, 12)); // 18, 23));

	_sky.setUseTexture(true);
	_sky.load("Sky.png");
	_sky.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_NEAREST);
	_sky.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);

	_skyImageAngleRange = 2.0f * PI; // / 1.0f; // / 2.0f; // 180 degrees range
}

void HybridRenderer::update()
{

}

void HybridRenderer::onDraw()
{
	// Background
    ofColor groundClose(46, 50, 55);
    ofColor groundFar(25, 33, 36);
    ofBackgroundGradient(groundFar, groundClose, OF_GRADIENT_BAR);
	//ofColor sky(130, 150, 165);
	//ofSetColor(skyColor);
	//ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight() / 2);
    
	ofSetColor(255, 255, 255);
	_drawSky();

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

		_drawLevelColumn(eye, x, 2);
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
	lightIntensity *= 0.5f;
	lightIntensity += 0.5f;

	ofImage* tex;

	if ('#' == mapElement)
	{
		tex = &_tex;
	}
	else if ('^' == mapElement)
	{
		tex = &_texTop;
	}
	else if ('&' == mapElement)
	{
		tex = &_texLow;
	}
	else if ('$' == mapElement)
	{
		tex = &_texMetal;
	}

	float x = float(column) * renderInfo.resolutionMultiplier;
	float y = minY * renderInfo.resolutionMultiplier;
	float width = float(renderInfo.resolutionMultiplier);
	float height = float(wallHeight * renderInfo.resolutionMultiplier);
	tex->drawSubsection(x, y, width, height, sampleX, 0, 1, 1024);

	_fog.applyToColumn(x, y, width, height, cameraDistance);
}

void HybridRenderer::_drawSky()
{
	float skyboxImageRadiansRange = TWO_PI;  // skybox image covers 180 degrees
	float skyboxPixelWidth = 15360.0f;
	float fov = PI / 2.f;

	float playerAngle = TWO_PI - getPlayer()->getLookAtAngle(); // Need to reverse the angle, the sky will scroll in reverse direction otherwise

	// BG BLOCK 1
	float skySampleXOffset = fmod(playerAngle / skyboxImageRadiansRange, 1.0f); // offset into the sky texture in 0-1 range. One image covers 180 degrees (PI radians).

	float skyPixelDensity = skyboxPixelWidth / skyboxImageRadiansRange;
	
	float skySampleX = skySampleXOffset * skyboxPixelWidth; // offset into the sky texture in pixels.

	// width and height is the actual screen resolution.
	float width = renderInfo.renderResolutionX * renderInfo.resolutionMultiplier;
	float height = renderInfo.renderResolutionY * renderInfo.resolutionMultiplier;
	float targetHeight = height * 0.5f;

	float sourceWidth = skyPixelDensity * fov;
	float sourceHeight = sourceWidth * 0.25f;
	
	float targetWidth = width;

	// 256 here is a fixed offset in Y into the image. I could just as well cut these 256 pixels from the image and then use offset 0.
	_sky.drawSubsection(0, 0, targetWidth, targetHeight, skySampleX, 256, sourceWidth, sourceHeight);

	// BG BLOCK2
	float skySampleXEndOffset = skySampleX + sourceWidth; // one screen is 90 degrees, we put half of the image 
	if (skySampleXOffset > 0.75f) // Basically one sky image covers PI radians and FOV we assume is PI/2 (so we see half of skybox image on screen at the same time.)
	{
		float block1PieceOnScreen = 1.0f - skySampleXOffset; // this tells how much of skybox image1 is visible on screen, in range 0-1.0.
		float block2Offset = (block1PieceOnScreen * skyboxImageRadiansRange) / fov; // Calculate the offset as 0-1 range value in screen space. Basically this will be the offset to draw 2nd block at in 0-1 screen range.
		float targetX = block2Offset * width; // and get final value in pixels.

		_sky.drawSubsection(targetX, 0, targetWidth, targetHeight, 0, 256, sourceWidth, sourceHeight);

		/*
		std::string skyEndOffsetString = "Block1 % in FOV: " + ofToString(block1PieceOnScreen);
		ofDrawBitmapStringHighlight(skyEndOffsetString, 160, 170);

		std::string block2OffsetDisp = "Block 2 offset: " + ofToString(block2Offset);
		ofDrawBitmapStringHighlight(block2OffsetDisp, 160, 190);
		*/
	}

	/*
	std::string offsetX = "Sky Sample X: " + ofToString(skySampleX);
	ofDrawBitmapStringHighlight(offsetX, 40, 80);

	std::string sourceWidthString = ofToString(sourceWidth);
	ofDrawBitmapStringHighlight(sourceWidthString, 40, 110);

	std::string sourceHeightString = ofToString(sourceHeight);
	ofDrawBitmapStringHighlight(sourceHeightString, 160, 110);

	std::string targetWidthString = ofToString(targetWidth);
	ofDrawBitmapStringHighlight(targetWidthString, 40, 140);

	std::string targetHeightString = ofToString(targetHeight);
	ofDrawBitmapStringHighlight(targetHeightString, 160, 140);
	*/
}

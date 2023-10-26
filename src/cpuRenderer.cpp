#include "cpuRenderer.hpp"

void CPURenderer::onInit()
{
    _allocateRenderBuffer();
	_buffer.getTexture().setTextureMinMagFilter(GL_LINEAR, GL_NEAREST);

	_tex.setUseTexture(false);
	_tex.load("TexConcrete1024.png");
	_texMetal.setUseTexture(false);
	_texMetal.load("TexMetal1024.png");

}

void CPURenderer::onResolutionChanged()
{
	_allocateRenderBuffer();
}

void CPURenderer::update()
{

}

void CPURenderer::onDraw()
{
	// Background
    ofColor groundClose(88, 96, 104);
    ofColor groundFar(16, 16, 16);
    ofBackgroundGradient(groundFar, groundClose, OF_GRADIENT_BAR);
	ofColor sky(130, 150, 165);
	ofSetColor(sky);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight() / 2);
    
    int columns = renderInfo.renderResolutionX;

	_buffer.setColor(ofColor(32, 0)); // mid gray and alpha to 0
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
		ofVec2f normal(0, 0);

		// call raycast here
		RayHit hit;

		if (!raycaster.raycast(getPlayer()->getPosition(), eye, 0, hit))
		{
			continue;
		}

        float distanceToSurface = hit.distance;
        float sampleX = hit.texU;
		normal = hit.normal;
		char mapElement = hit.mapElement;

        // To calculate line length can't use the distance to surface directly.
        ofVec2f alongEye(eye * distanceToSurface);

		// Store ray for debug drawing for later.
		if (debugDrawing) { debugRays.push_back(alongEye); }

        float cameraDistance = getPlayer()->getDirection().dot(alongEye);
		float medianDistance = cameraDistance;

		// Calculate height of a wall in pixels.
		int wallHeight = int(float(renderInfo.cellPixelSize) / cameraDistance);

		// minY is where we should start drawing wall from vertically in pixels
		// it's unclipped here so it'll be negative when we're very close to the wall
		// maxY is where we should stop drawing wall at vertically in pixels
		// again, unclipped so will be large number, out of the bounds of the screen
		int minY = (renderInfo.renderResolutionY - wallHeight) / 2;
        int maxY = renderInfo.renderResolutionY - minY;

        ofColor pixelColor;
        
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

		// these are used for drawing but we still need to preserve
		// unclamped min and max Y for texture sampling.
		float drawMaxY = min(maxY, renderInfo.renderResolutionY);
		float drawMinY = max(minY, 0);

        for(int y = drawMinY; y < drawMaxY; y++)
        {            
            float sampleY = 1023.0f * ((float)(y - minY) / (float)(maxY - minY));
            if (sampleY > 1023.0f)
            {
                sampleY = 1023.0f;
            }
            
            if ('#' == mapElement)
            {
				pixelColor = _tex.getColor((int)sampleX, (int)sampleY);
                //pixelColor = ofColor(255, 230, 150);
            }
            else if ('$' == mapElement)
            {
				pixelColor = _texMetal.getColor((int)sampleX, (int)sampleY);
                //pixelColor = ofColor(200, 230, 250);
            }
			pixelColor *= lightIntensity;
            float fogAmount = medianDistance / 16.0f * 0.9f;
            
            ofColor depthColor(20, 20, 20);
            depthColor *= 0.25f;
            pixelColor.lerp(depthColor, fogAmount);
            pixelColor.a = 255;
		
			int index = (y * renderInfo.renderResolutionX + x) * 4;
			memcpy(&_buffer.getPixels()[index], &pixelColor, 4);
		}
		
    }
    _buffer.update();
	_buffer.draw(0, 0, renderInfo.outputResolutionX, renderInfo.outputResolutionY);
}

// -------- Private methods

void CPURenderer::_allocateRenderBuffer()
{
    _buffer.clear();
    _buffer.allocate(renderInfo.renderResolutionX, renderInfo.renderResolutionY, OF_IMAGE_COLOR_ALPHA);
}
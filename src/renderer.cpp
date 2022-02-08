#include "renderer.hpp"

Renderer::Renderer() :
    _map(NULL),
	_player(NULL),
	_resX(320),
	_resY(240),
    _resMultiplier(4),
	_debugDrawing(false),
    _tex1("TexConcrete1024.png")
{
    _fov = 3.14159 / 2.0f;
	_heightSquashFactor = 2.1f;
    _maxTestingDepth = 16.0f;
    
    _lightAngle = 3.14159 / 3.0;
    _lightDirection.x = sinf(_lightAngle);
    _lightDirection.y = cosf(_lightAngle);
    
	_debugRays.clear();
	_debugFOVPoints.clear();

    _allocateRenderBuffer();

	_tex.setUseTexture(false);
	_tex.load("TexConcrete1024.png");
	_texMetal.setUseTexture(false);
	_texMetal.load("TexMetal1024.png");

}

Renderer::~Renderer()
{
}

void Renderer::setDebugDrawing(bool state)
{
	_debugDrawing = state;
}

void Renderer::update()
{

}

void Renderer::draw()
{
	// Background
	ofColor sky(67, 94, 163);
	ofSetColor(sky);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight() / 2);

    //ofColor groundClose(91, 91, 91);
    //ofColor groundFar(0, 0, 0);
    //ofBackgroundGradient(groundFar, groundClose, OF_GRADIENT_BAR);
   
    
    int columns = _resX;
    float centerX = _resX / 2;
    float centerY = _resY / 2;

    _buffer.setColor(sky);

    float lightIntensity = (_player->getDirection().dot(_lightDirection) + 1.0f) / 2.0f;

	// We'll store rays in vector for later drawing on screen.
	if (_debugDrawing)
	{
		_debugRays.clear();
		_debugFOVPoints.clear();
	}

	// --- Setting up FOV processing
	// Progressing through FOV needs to go in correct direction, rendering will be flipped
	// compared to the map otherwise.
	// What we do here is we pick start and end angle and create start and end eye vectors
	// out of these.
	// We are going to cast rays between these 2 eye start and end vectors.
	float startRayAngle = (_player->getLookAtAngle() + (_fov / 2));
	float endRayAngle = (_player->getLookAtAngle() - (_fov / 2));
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

    for (int x = 0; x < columns; x++)
    {
		ofVec2f eye = eyeStart + (visionLineNorm * float(x) * visionLineStep);
		if (_debugDrawing) { _debugFOVPoints.push_back(eye); }
		eye.normalize();
		ofVec2f normal(0, 0);

		// --- Setting up DDA raycasting
		ofVec2f rayStart = _player->getPosition();
		ofVec2f rayDir = eye;
		float rayXUnitStepSize = sqrt(1 + (rayDir.y / rayDir.x) * (rayDir.y / rayDir.x));
		float rayYUnitStepSize = sqrt(1 + (rayDir.x / rayDir.y) * (rayDir.x / rayDir.y));
		ofVec2f rayUnitStepSize = ofVec2f(rayXUnitStepSize, rayYUnitStepSize); // how much we move along the ray in x and y direction.
		int rayMapX = int(rayStart.x);
		int rayMapY = int(rayStart.y);
		ofVec2f rayLengthByAxis = ofVec2f();
		int cellStepX = 0;
		int cellStepY = 0;
		ofVec2f normalX = ofVec2f();
		ofVec2f normalY = ofVec2f();
		bool hDirection = false; // which direction was the last hit in, true = horizontal

		// DDA Starting conditions
		if (rayDir.x < 0)
		{
			cellStepX = -1;
			rayLengthByAxis.x = fmod(rayStart.x, 1.0) * rayUnitStepSize.x;
			normalX.x = 1.0; // Going left on x we can only ever hit right side of the cell.
		}
		else
		{
			cellStepX = 1;
			rayLengthByAxis.x = (1.0 - fmod(rayStart.x, 1.0)) * rayUnitStepSize.x;
			normalX.x = -1.0;
		}

		if (rayDir.y < 0)
		{
			cellStepY = -1;
			rayLengthByAxis.y = fmod(rayStart.y, 1.0) * rayUnitStepSize.y;
			normalY.y = 1.0; // going up we can only hit bottom side of a cell
		}
		else
		{
			cellStepY = 1;
			rayLengthByAxis.y = (1.0 - fmod(rayStart.y, 1.0)) * rayUnitStepSize.y;
			normalY.y = -1.0;
		}

		bool wallFound = false;
		float castDistance = 0;
		char mapElement;
		float texU = 0;
		while (!wallFound && castDistance < _maxTestingDepth)
		{
			if (rayLengthByAxis.x < rayLengthByAxis.y)
				// travel in x distance
			{
				rayMapX += cellStepX;
				castDistance = rayLengthByAxis.x;
				rayLengthByAxis.x += rayUnitStepSize.x;
				normal = normalX;
				hDirection = true;
			}
			else
				// travel in y distance
			{
				rayMapY += cellStepY;
				castDistance = rayLengthByAxis.y;
				rayLengthByAxis.y += rayUnitStepSize.y;
				normal = normalY;
				hDirection = false;
			}

			mapElement = _map->getCell(rayMapX, rayMapY);
			if (mapElement == '#' || mapElement == '$')
			{
				wallFound = true;
			}
		}

		ofVec2f intersection = ofVec2f();
		if (wallFound)
		{
			intersection = rayStart + rayDir * castDistance;
			if (hDirection)
			{
				texU = fmod(intersection.y, 1.0);
			}
			else
			{
				texU = fmod(intersection.x, 1.0);
			}
		}
		else
		{
			continue;
		}

        float distanceToSurface = castDistance;
        float sampleX = texU;
        
        // To calculate line length can't use the distance to surface directly.
        ofVec2f alongEye(eye * distanceToSurface);

		// Store ray for debug drawing for later.
		if (_debugDrawing) { _debugRays.push_back(alongEye); }

        float cameraDistance = _player->getDirection().dot(alongEye);
		float medianDistance = cameraDistance;

        int height = _resY;
        
		// Height squash factor is used to compress vertical lines so walls look square rather than
		// being alongated on Y
		int minY = (float)(height / 2) - height / (medianDistance * _heightSquashFactor); 
        
        int maxY = height - minY;
        float distanceShade = 255.0 - (medianDistance * 16.0f);
        float brightnessBoost = ((16.0f - medianDistance) / 16.0f) * 0.5f + 1.0f;
        // Get color from texture.
        
        //int shade = (int)(distanceShade * sampleX);
        //if (shade < 0) {
        //    shade = 0;
        //}
        
        //ofSetColor(shade, shade, shade);
        //ofDrawRectangle(x * 4, minY * 4, 4, (maxY - minY) * 4);
        ofColor pixelColor;
        
        sampleX = sampleX * 1023.0f;
        if (sampleX > 1023.0f)
        {
            sampleX = 1023.0f;
        }
        
        int wallHeight = maxY - minY;

		// Light intensity is currently just doing dot product between
		// intersection wall normal and light direction.
		float lightIntensity = 1.0f - ((_lightDirection.dot(normal) + 1.0f) / 2.0f);

		// Remap light intensity to 0.25-1.0 range.
        lightIntensity *= 0.5f;
        lightIntensity += 0.5f;

		// these are used for drawing but we still need to preserve
		// unclamped min and max Y for texture sampling.
		float drawMaxY = min(maxY, _resY);
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
            //pixelColor = ofColor(255, 255, 255);
            //pixelColor *= (1.0f - (medianDistance / 16.0f * 0.95f)); // depth shading
            float fogAmount = medianDistance / 16.0f * 0.9f;
            
            //pixelColor *= (lightIntensity);// * 0.5f + 0.5f) ;
            //pixelColor *= brightnessBoost;
            
            ofColor depthColor(20, 20, 20);
            depthColor *= 0.25f;
            //ofColor depthShaded = pixelColor.lerp(depthColor, fogAmount);
            pixelColor.lerp(depthColor, fogAmount);
            pixelColor.a = 255;
      

			int index = (y * _resX + x) * 3;
			memcpy(&_buffer.getPixels()[index], &pixelColor, 3);

            //ofSetColor(pixelColor);
            //ofDrawRectangle(x * 2, y * 2, 2, 2);
            //_buffer.setColor(x, y, pixelColor);
			//int index = (y * _resX + x) * 3;
			
			//_buffer.getPixels()[index] = pixelColor.r;
			
			// This is drawing pixel
			//_buffer.getPixels()[index+1] = pixelColor.g;
			
			
			//_buffer.getPixels()[index+2] = pixelColor.b;

			//_buffer.getPixels().setColor(x, y, pixelColor);

            int refY = wallHeight - (y - minY) + maxY - 1;
            
            if (refY < _resY)
            {
                float gradient = (float)(y - minY)/ (float)(maxY - minY);
                
                //ofColor refPixelColor = pixelColor * 0.5f;
                //refPixelColor.a = (int)(gradient * 160.0);
                //_buffer.setColor(x, refY, refPixelColor);
                //ofSetColor(refPixelColor);
                //ofDrawRectangle(x * 2, refY * 2, 2, 2);
            }
            //ofDrawLine(x*2, y*2, x*2+1, y*2);
        }
    }
	//_buffer.setFromPixels(_pixels);
    _buffer.update();
	_buffer.draw(0, 0, ofGetWidth(), ofGetHeight());
	_drawDebug();
}

void Renderer::setMap(Map *map)
{
    _map = map;
}

void Renderer::setPlayer(Player* player)
{
    _player = player;
}

void Renderer::increaseResolution()
{
    if (_resMultiplier == 1) { return; }
    _resMultiplier /= 2;
    _allocateRenderBuffer();
}

void Renderer::decreaseResolution()
{
    if (_resMultiplier >= 16) { return; }
    _resMultiplier *= 2;
    _allocateRenderBuffer();
}

// -------- Private methods

void Renderer::_calculateRenderSize()
{
    int screenWidth = ofGetWidth();
    int screenHeight = ofGetHeight();
    _resX = screenWidth / _resMultiplier;
    _resY = screenHeight / _resMultiplier;
}

void Renderer::_allocateRenderBuffer()
{
    _calculateRenderSize();
    _buffer.clear();
    _buffer.allocate(_resX, _resY, OF_IMAGE_COLOR);
}

void Renderer::_drawDebug()
{
	if (!_debugDrawing) { return; }
	
	float cellSize = float(ofGetHeight() / _map->getHeight());
	ofVec2f p1 = _player->getPosition() * cellSize;
	for (auto it = std::begin(_debugRays); it != std::end(_debugRays); ++it)
	{
		ofVec2f p2 = ((*it * cellSize) + p1);
		ofSetColor(255, 0, 64);
		ofDrawLine(p1, p2);
	}

	for (int i = 0; i < _debugFOVPoints.size(); i+=16)
	{
		ofVec2f p = p1 + (_debugFOVPoints.at(i) * cellSize * 5);
		ofSetColor(255, 128, 255);
		ofDrawCircle(p, 1);
	}
}

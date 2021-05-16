#include "renderer.hpp"

Renderer::Renderer() :
    _map(NULL),
	_player(NULL),
	_resX(320),
	_resY(240),
    _resMultiplier(4),
    _tex1("TexConcrete1024.png")
{
    _fov = 3.14159 / 2.5f;
    _maxTestingDepth = 16.0f;
    
    _lightAngle = 3.14159 / 3.0;
    _lightDirection.x = sinf(_lightAngle);
    _lightDirection.y = cosf(_lightAngle);
    
    _allocateRenderBuffer();
}

void Renderer::update()
{
    ofColor groundClose(91, 91, 91);
    ofColor groundFar(0, 0, 0);
    ofBackgroundGradient(groundFar, groundClose, OF_GRADIENT_BAR);
    
    // Background
    ofColor sky(67, 74, 83);
    ofSetColor(sky);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight() / 2);
    
    int columns = _resX;
    float centerX = _resX / 2;
    float centerY = _resY / 2;

    _buffer.setColor(sky);
    
    float lightIntensity = (_player->getDirection().dot(_lightDirection) + 1.0f) / 2.0f;
    
    for (int x = 0; x < columns; x++)
    {
        float rayAngle = (_player->getLookAtAngle() - (_fov / 2)) + (((float)x / (float)columns) * _fov);
        float distanceToSurface = 0;
        float sampleX = 0;
        bool surfaceHit = false;
        
        ofVec2f eye(sinf(rayAngle), cosf(rayAngle));
        ofVec2f normal(0, 0);
        
        char wall = '#';
        char metalWall = '$';
        char mapElement;
        
        while(!surfaceHit && distanceToSurface < _maxTestingDepth)
        {
            distanceToSurface += 0.01f;
            
            // Test a cell in a map.
            int testX = (int)(_player->getPosition().x + (eye.x * distanceToSurface));
            int testY = (int)(_player->getPosition().y + (eye.y * distanceToSurface));
            
            // Test if we're beyond map bounds
            if (testX < 0 || testX > _map->getWidth() || testY < 0 || testY > _map->getHeight())
            {
                surfaceHit = true;
                distanceToSurface = _maxTestingDepth;
            }
            else
            {
                mapElement = _map->getCell(testX, testY);
                if (wall == mapElement || metalWall == mapElement)
                {
                    surfaceHit = true;
                    
                    // find mid point of a cell
                    float blockMidX = (float)testX + 0.5f;
                    float blockMidY = (float)testY + 0.5f;
                    
                    // this can really be a vector
                    ofVec2f wallCollisionPoint = _player->getPosition() + (eye * distanceToSurface);
                    
                    //
                    float testAngle = atan2f(wallCollisionPoint.y - blockMidY, wallCollisionPoint.x - blockMidX);
                    if(testAngle >= -3.14159f * 0.25f && testAngle < 3.14159f * 0.25f)
                    {
                        sampleX = wallCollisionPoint.y - (float)testY;
                        normal.x = -1.0f;
                    }
                    else if(testAngle >= 3.14159f * 0.25f && testAngle < 3.14159f * 0.75f)
                    {
                        sampleX = wallCollisionPoint.x - (float)testX;
                        normal.y = -1.0f;
                    }
                    else if(testAngle < -3.14159f * 0.25f && testAngle >= -3.14159f * 0.75f)
                    {
                        sampleX = wallCollisionPoint.x - (float)testX;
                        normal.y = 1.0f;
                    }
                    else if(testAngle >= 3.14159f * 0.75f || testAngle < -3.14159f * 0.75f)
                    {
                        sampleX = wallCollisionPoint.y - (float)testY;
                        normal.x = 1.0f;
                    }
                    else
                    {
                        sampleX = 0.5;
                        normal.x = 1.0f;
                    }
                    
                }
            }
        }
        
        // To calculate line length can't use the distance to surface directly.
        ofVec2f alongEye(eye * distanceToSurface);
        float cameraDistance = _player->getDirection().dot(alongEye);
        //float medianDistance = ((distanceToSurface * 0.3f)  + (cameraDistance * 0.7f));
        float distancedifference = distanceToSurface - cameraDistance;
        float medianDistance = cameraDistance + (distancedifference / 4.0f);
        
        int height = _resY; // / 2;
        
        int minY = (float)(height / 2) - height / (medianDistance * 1.4f); // 1.4 if increasing depth impression
        
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
        
        //float lightIntensity = 1.0f - ((eye.dot(normal) + 1.0f) / 2.0f);
        float lightIntensity = (eye.dot(normal)); // + 1.0f) / 2.0f);
        if (lightIntensity < 0.0f)
        {
            lightIntensity = 0.0f;
        }
        lightIntensity *= 0.75f;
        lightIntensity += 0.25f;
        
        for(int y = minY; y < maxY; y++)
        {
            if (y < 0 || y >= _resY) { continue; };
            
            float sampleY = 1023.0f * ((float)(y - minY) / (float)(maxY - minY));
            if (sampleY > 1023.0f)
            {
                sampleY = 1023.0f;
            }
            
            if (wall == mapElement)
            {
				//pixelColor = _tex.getColor((int)sampleX, (int)sampleY);
                pixelColor = ofColor(255, 230, 150);
            }
            else if (metalWall == mapElement)
            {
				//pixelColor = _texMetal.getColor((int)sampleX, (int)sampleY);
                pixelColor = ofColor(200, 230, 250);
            }
            //pixelColor = ofColor(255, 255, 255);
            //pixelColor *= (1.0f - (medianDistance / 16.0f * 0.95f)); // depth shading
            float fogAmount = medianDistance / 16.0f * 0.9f;
            
            pixelColor *= (lightIntensity);// * 0.5f + 0.5f) ;
            pixelColor *= brightnessBoost;
            
            ofColor depthColor(20, 20, 20);
            depthColor *= 0.25f;
            ofColor depthShaded = pixelColor.lerp(depthColor, fogAmount);
            pixelColor = depthShaded;
            pixelColor.a = 255;
            
            //ofSetColor(pixelColor);
            //ofDrawRectangle(x * 2, y * 2, 2, 2);
            _buffer.setColor(x, y, pixelColor);
            
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
    _buffer.update();
}

void Renderer::draw()
{
    _buffer.draw(0, 0, ofGetWidth(), ofGetHeight());
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

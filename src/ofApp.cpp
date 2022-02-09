#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	_debugDrawing = false;
	_moveSpeed = 4.0f;
	_moveForward = false;
	_moveBackward = false;
	_moveLeft = false;
	_moveRight = false;

    _player.setMap(&_map);
    
    _renderer.setMap(&_map);
    _renderer.setPlayer(&_player);
    
    _tex.load("TexConcrete1024.png");
    _texMetal.load("TexMetal1024.png");
    _bg.load("Background.png");
}

//--------------------------------------------------------------
void ofApp::update()
{
	if (_moveRight)
	{
		_player.sideStep(_moveSpeed * 0.5 * ofGetLastFrameTime());
	}
    if (_moveLeft)
	{
		_player.sideStep(-_moveSpeed * 0.5 * ofGetLastFrameTime());
	}
	if (_moveForward)
	{
		_player.move(_moveSpeed * ofGetLastFrameTime());
	}
	if (_moveBackward)
	{
		_player.move(-_moveSpeed * ofGetLastFrameTime());
	}

    _renderer.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{    
    _renderer.draw();
    
	if (_debugDrawing)
	{
		_map.draw();
		_player.draw();
	}

	int fpsval = int(ofGetFrameRate());
	std::string fps = ofToString(fpsval);
	ofDrawBitmapStringHighlight(fps, 40, 40);
	ofDrawBitmapStringHighlight(ofToString(ofGetLastFrameTime()), 40, 120);

	if (_debugDrawing)
	{
		ofSetColor(40, 255, 40);
		ofDrawRectangle(20, 70, 4 * fpsval, 20);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'd')
    {
		_moveRight = true;
    }
    else if (key == 'a')
    {
		_moveLeft = true;
    }
    else if (key == 'w')
    {
		_moveForward = true;
    }
    else if (key == 's')
    {
		_moveBackward = true;
    }
    
    // Increase/descrease resolution
    else if (key == ',')
    {
        _renderer.decreaseResolution();
    }
    else if (key == '.')
    {
        _renderer.increaseResolution();
    }

	// Toggle debug drawing
	else if (key == '1')
	{
		_debugDrawing = !_debugDrawing;
		_renderer.setDebugDrawing(_debugDrawing);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'd')
	{
		_moveRight = false;
	}
	else if (key == 'a')
	{
		_moveLeft = false;
	}
	else if (key == 'w')
	{
		_moveForward = false;
	}
	else if (key == 's')
	{
		_moveBackward = false;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	int prevx = ofGetPreviousMouseX();
	int offsetX = x - prevx;
	if (offsetX != 0)
	{
		_player.rotate(float(offsetX) * -0.01);
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

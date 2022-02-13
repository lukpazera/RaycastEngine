#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	_debugDrawing = false;
	_moveSpeed = 4.0f;
	_moveForward = false;
	_moveBackward = false;
	_moveLeft = false;
	_moveRight = false;
	_mouseWrapped = false;

    _player.setMap(&_map);
    
	_renderer.onInit();
    _renderer.setMap(&_map);
    _renderer.setPlayer(&_player);
	_renderer.setResolution(ofGetWidth(), ofGetHeight());
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
		_renderer.debugDrawing = _debugDrawing;
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
	// Wrapping mouse will call this event.
	// We don't want to process wrapped mouse coords again
	// so when the call after manual wrap happens just set wrap to false
	// and return early.
	// The next call will be processed properly.
	if (_mouseWrapped)
	{
		_mouseWrapped = false;
		return;
	}

	int prevx = ofGetPreviousMouseX();
	int offsetX = x - prevx;
	if (offsetX != 0)
	{
		_player.rotate(float(offsetX) * -0.01);
	}

// Windows specific code for handling mouse wrapping
#if defined( TARGET_WIN32 )
	if (x < 10)
	{
		SetCursorPos(x + ofGetWidth() / 2, ofGetHeight() / 2);
		_mouseWrapped = true;
	}
	else if (x > (ofGetWidth() - 10))
	{
		SetCursorPos(x - ofGetWidth() / 2, ofGetHeight() / 2);
		_mouseWrapped = true;
	}
#endif
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

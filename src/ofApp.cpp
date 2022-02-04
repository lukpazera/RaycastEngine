#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	_debugDrawing = false;
    
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
		// Decrease angle to rotate clockwise
        _player.rotate(-0.2f); // * ofGetLastFrameTime());
    }
    else if (key == 'a')
    {
		// Increase angle to rotate counterclockwise
        _player.rotate(0.2f); // * ofGetLastFrameTime());
    }
    else if (key == 'w')
    {
        _player.move(0.6f); // * ofGetLastFrameTime());
    }
    else if (key == 's')
    {
        _player.move(-0.6f);// * ofGetLastFrameTime());
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

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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

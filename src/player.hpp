#pragma once

#include <stdio.h>
#include "ofMain.h"

#include "map.hpp"

class Player
{
public:
    Player();
    
    ofVec2f getPosition() const;
    ofVec2f getDirection() const;
    float getLookAtAngle() const;
    
    void move(float distance);
	void sideStep(float distance);
    void rotate(float angle);
    
	void draw();

    void setMap(Map* map);
    
private:
    ofVec2f _playerPosition;
    ofVec2f _playerDirection;
    float _playerLookAtAngle;
    Map* _map;
};

#pragma once

#include "map.hpp"
#include "ofMain.h"


struct RayHit
{
	RayHit() : distance(0), intersection(ofVec2f()), normal(ofVec2f()), texU(0), mapElement(' ') {}

	float distance;
	ofVec2f intersection;
	ofVec2f normal;
	float texU;
	char mapElement;
};

class DDARaycaster
{
public:
	DDARaycaster();

	bool raycast(const ofVec2f rayOrigin, const ofVec2f rayDirection, RayHit& hit);
	void setMap(Map& map);
	void setMaxTestingDepth(int depth);

private:
	Map* _map;
	int _maxTestingDepth;
};


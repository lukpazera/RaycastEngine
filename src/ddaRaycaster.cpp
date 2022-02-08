#include "ddaRaycaster.h"

DDARaycaster::DDARaycaster() :
	_map(NULL),
	_maxTestingDepth(16)
{

}

bool DDARaycaster::raycast(const ofVec2f rayOrigin, const ofVec2f rayDir, RayHit& hit)
{
	if (_map == NULL) { return false; }

	// Setting up raycasting
	float rayXUnitStepSize = sqrt(1 + (rayDir.y / rayDir.x) * (rayDir.y / rayDir.x));
	float rayYUnitStepSize = sqrt(1 + (rayDir.x / rayDir.y) * (rayDir.x / rayDir.y));
	ofVec2f rayUnitStepSize = ofVec2f(rayXUnitStepSize, rayYUnitStepSize); // how much we move along the ray in x and y direction.
	int rayMapX = int(rayOrigin.x);
	int rayMapY = int(rayOrigin.y);
	ofVec2f rayLengthByAxis = ofVec2f();
	int cellStepX = 0;
	int cellStepY = 0;
	ofVec2f normalX = ofVec2f();
	ofVec2f normalY = ofVec2f();
	ofVec2f hitNormal = ofVec2f();

	bool hDirection = false; // which direction was the last hit in, true = horizontal

	// Starting conditions
	if (rayDir.x < 0)
	{
		cellStepX = -1;
		rayLengthByAxis.x = fmod(rayOrigin.x, 1.0) * rayUnitStepSize.x;
		normalX.x = 1.0; // Going left on x we can only ever hit right side of the cell.
	}
	else
	{
		cellStepX = 1;
		rayLengthByAxis.x = (1.0 - fmod(rayOrigin.x, 1.0)) * rayUnitStepSize.x;
		normalX.x = -1.0;
	}

	if (rayDir.y < 0)
	{
		cellStepY = -1;
		rayLengthByAxis.y = fmod(rayOrigin.y, 1.0) * rayUnitStepSize.y;
		normalY.y = 1.0; // going up we can only hit bottom side of a cell
	}
	else
	{
		cellStepY = 1;
		rayLengthByAxis.y = (1.0 - fmod(rayOrigin.y, 1.0)) * rayUnitStepSize.y;
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
			hitNormal = normalX;
			hDirection = true;
		}
		else
			// travel in y distance
		{
			rayMapY += cellStepY;
			castDistance = rayLengthByAxis.y;
			rayLengthByAxis.y += rayUnitStepSize.y;
			hitNormal = normalY;
			hDirection = false;
		}

		mapElement = _map->getCell(rayMapX, rayMapY);
		if (mapElement == '$' || mapElement == '#')
		{
			wallFound = true;
		}
	}
	
	if (!wallFound) { return false; }

	ofVec2f intersection = ofVec2f();
	intersection = rayOrigin + rayDir * castDistance;
	if (hDirection)
	{
		texU = fmod(intersection.y, 1.0);
	}
	else
	{
		texU = fmod(intersection.x, 1.0);
	}

	// Write results back
	hit.distance = castDistance;
	hit.intersection = intersection;
	hit.normal = hitNormal;
	hit.texU = texU;
	hit.mapElement = mapElement;

	return true;
}

void DDARaycaster::setMap(Map& map)
{
	_map = &map;
}

void DDARaycaster::setMaxTestingDepth(int depth)
{
	_maxTestingDepth = depth;
}
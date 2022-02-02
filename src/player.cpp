
#include "player.hpp"

Player::Player() :
	_map(NULL),
	_playerPosition(ofVec2f(2.5f, 2.5f)),
	_playerLookAtAngle(0.0)
{
	rotate(0);
}

ofVec2f Player::getPosition() const
{
    return _playerPosition;
}

/*
 * Returns normalized vector representing player's direction.
 */
ofVec2f Player::getDirection() const
{
    return _playerDirection;
}

/*
 * Returns player look at angle.
 * 0 angle is player looking down the map.
 */
float Player::getLookAtAngle() const
{
    return _playerLookAtAngle;
}

void Player::move(float distance)
{    
    ofVec2f newPosition = _playerPosition + (_playerDirection * distance);
    if (_map->isWall((int)newPosition.x, (int)newPosition.y))
    {
        return;
    }
    _playerPosition = newPosition;
}

/*
 * Rotates player in place by a given angle in radians.
 * Increasing angle rotates player counterclockwise.
 */
void Player::rotate(float angle)
{
    _playerLookAtAngle += angle;
    _playerDirection.x = sinf(_playerLookAtAngle);
    _playerDirection.y = cosf(_playerLookAtAngle);
}

void Player::setMap(Map* map)
{
    _map = map;
}

/*
 * Draws player on screen for debugging purposes.
 * Requires map to be initialized.
 */
void Player::draw()
{
	if (_map == NULL) { return; }

	int mapHeight = _map->getHeight();
	int screenHeight = ofGetHeight();
	float cellSize = float(screenHeight / mapHeight);

	// Draw player point
	ofVec2f playerScreenPos = ofVec2f(cellSize * _playerPosition.x, cellSize * _playerPosition.y);
	ofSetColor(255, 192, 0);
	ofDrawCircle(playerScreenPos, cellSize * 0.1f);

	// Draw player direction
	ofSetColor(80, 240, 80);
	ofVec2f dirTipScreenPos = playerScreenPos + (getDirection() * cellSize * 0.5f);
	ofDrawLine(playerScreenPos, dirTipScreenPos);
}
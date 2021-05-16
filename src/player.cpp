
#include "player.hpp"

Player::Player()
{
    
}

ofVec2f Player::getPosition() const
{
    return _playerPosition;
}

ofVec2f Player::getDirection() const
{
    return _playerDirection;
}

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

void Player::rotate(float angle)
{
    _playerLookAtAngle += angle;
    _playerDirection.x = sinf(_playerLookAtAngle);
    _playerDirection.y = cosf(_playerLookAtAngle);
}

void Player::setMap(Map* map)
{
    _map = map;
    _playerPosition.x = 2.5f;
    _playerPosition.y = 2.5f;
    _playerLookAtAngle = 0;
    rotate(0);
    
}

#pragma once
#include "cocos2d.h"

struct MovementDirection
{
	MovementDirection()
		: x(0)
		, y(0)
	{}
	MovementDirection(float dx, float dy)
		: x(dx)
		, y(dy)
	{}
	float x;
	float y;

	bool operator ==(MovementDirection const& other) const;
	bool operator !=(MovementDirection const& other) const;

};

class CMotorSystem
{
public: 

	void SetSpeed(float const& speed);
	float GetSpeed();

	void SetDirection(MovementDirection const& direction);
	MovementDirection GetDirection();

	cocos2d::Point MoveToward(cocos2d::Point posObject, float dt);
	cocos2d::Point MoveToPoint(cocos2d::Point posObject, cocos2d::Point const & posPoint);

private:
	float m_speed;
	MovementDirection m_movementDirection;

};


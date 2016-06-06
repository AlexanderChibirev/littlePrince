#include "MotorSystem.h"
#include "MathAndLogic.h"

void CMotorSystem::SetSpeed(float const & speed)
{
	m_speed = speed;
}

float CMotorSystem::GetSpeed()
{
	return m_speed;
}

void CMotorSystem::SetDirection(MovementDirection const & direction)
{
	m_movementDirection = direction;
}

MovementDirection CMotorSystem::GetDirection()
{
	return m_movementDirection;
}

cocos2d::Point CMotorSystem::MoveToPoint(cocos2d::Point posObject, cocos2d::Point const & posPoint)
{
	posObject.x += this->GetSpeed() *  SMath::Normalize(posPoint, posObject).x;
	posObject.y += this->GetSpeed() *  SMath::Normalize(posPoint, posObject).y;
	return posObject;
}


cocos2d::Point CMotorSystem::MoveToward(cocos2d::Point posObject, float dt)
{
	cocos2d::Point positionOffset(this->GetSpeed() * dt * this->GetDirection().x,
		this->GetSpeed() * dt * this->GetDirection().y);
	return posObject + positionOffset;
}

bool MovementDirection::operator==(MovementDirection const & other) const
{
	return (x == other.x) && (y == other.y);
}

bool MovementDirection::operator!=(MovementDirection const & other) const
{
	return (x != other.x) || (y != other.y);
}

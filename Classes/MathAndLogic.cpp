#include "MathAndLogic.h" 

 USING_NS_CC;

#define _USE_MATH_DEFINES


#include <math.h>

using namespace std;

float SMath::CalculateDistance(cocos2d::Vec2 const & posObject1, cocos2d::Vec2 const & posObject2)
{
	return posObject1.distance(posObject2);
}

cocos2d::Vec2 SMath::Normalize(cocos2d::Vec2 const & posObject1, cocos2d::Vec2 const & posObject2)
{
	cocos2d::Vec2 normalizeVector = posObject1 - posObject2;
	return normalizeVector.getNormalized();
}

 float SMath::CalculateAngle(cocos2d::Vec2 const & start, cocos2d::Vec2 const & end)
 {
	 return float(-atan2((end.y - start.y), (end.x - start.x)) * 180 / M_PI);
 }

#pragma once
#include "cocos2d.h" 

struct SMath
{
	static float CalculateDistance(cocos2d::Vec2 const & firstPoint, cocos2d::Vec2 const & secondPoint);
	static cocos2d::Vec2 Normalize(cocos2d::Vec2 const & firstPoint, cocos2d::Vec2 const & secondPoint);
	static float CalculateAngle(cocos2d::Vec2 const & posObject1, cocos2d::Vec2 const & posObject2);
};



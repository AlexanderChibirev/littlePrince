#pragma once
#include "cocos2d.h"

USING_NS_CC;

using namespace cocos2d;

class CCoogwheel:public cocos2d::Sprite
{
public:
	static CCoogwheel * create(const cocos2d::Vec2 & position);
	bool init(const cocos2d::Vec2 & position);
	virtual ~CCoogwheel() = default;
	virtual void update(float delta);
private:	
	void InitAnim();
	cocos2d::RefPtr<Animate> m_anim;
};
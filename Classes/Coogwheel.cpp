#include "Coogwheel.h"
#include "2d/CCSpriteFrameCache.h"
#include "2d/CCActionInterval.h"
#include "base/CCVector.h"
#include "cocos_custom.h"

CCoogwheel * CCoogwheel::create(const cocos2d::Vec2 & position)
{
	return make_cc<CCoogwheel>(position);
}

bool CCoogwheel::init(const cocos2d::Vec2 & position)
{
	if (!Sprite::init())
		return false;
	InitAnim();

	this->setPosition(position);
	return true;
}

void CCoogwheel::update(float delta)
{

}

void CCoogwheel::InitAnim()
{
	SpriteFrameCache *sharedSpriteFrameCache = SpriteFrameCache::getInstance();
	sharedSpriteFrameCache->addSpriteFramesWithFile("cogwheel.plist");
	auto animFrames = Vector<SpriteFrame*>(24);
	char str[100] = { 0 };
	for (size_t i = 1; i < 11; i++)
	{
		sprintf(str, "cogwheel_%d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	m_anim = Animate::create(animation);
	runAction(RepeatForever::create(m_anim));

}
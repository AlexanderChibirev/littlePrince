#include "EnemyPuppet.h"

USING_NS_CC;

bool CEnemyPuppet::init(IMapPhysics &mapPhysics)
{
    if (!CPuppet::init(mapPhysics))
    {
        return false;
    }


    m_body = Sprite::create("enemyFly_1.png");

	SpriteFrameCache *sharedSpriteFrameCache = SpriteFrameCache::getInstance();
	std::vector<cocos2d::RefPtr<Animate>> animationsVector;

	auto animFrames = Vector<SpriteFrame*>(20);

	char str[100] = { 0 };

	for (int i = 1; i < 8; i++)
	{
		sprintf(str, "enemyFly_%d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	auto animation = Animation::createWithSpriteFrames(animFrames, float(0.7));
	this->m_attack = Animate::create(animation);
	animationsVector.push_back(Animate::create(animation));
	animFrames.clear();


	for (int i = 1; i < 5; i++)
	{
		sprintf(str, "dead_%d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	animation = Animation::createWithSpriteFrames(animFrames, float(0.1));
	this->m_dead = Animate::create(animation);
	animationsVector.push_back(Animate::create(animation));
	animFrames.clear();

    return true;
}

cocos2d::RefPtr<cocos2d::Sprite> CEnemyPuppet::GetSprite()
{
	return m_body;
}

void CEnemyPuppet::onEnter()
{
    CPuppet::onEnter();
    addChild(m_body);
}

void CEnemyPuppet::onExit()
{
    m_body->removeFromParentAndCleanup(true);
    CPuppet::onExit();
}

cocos2d::RefPtr<Animate> CEnemyPuppet::GetAnim()
{
	return m_attack;
}

cocos2d::RefPtr<Animate> CEnemyPuppet::GetAnimDead()
{
	return m_dead;
}
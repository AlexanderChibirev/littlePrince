#pragma once
#include "cocos_custom.h"
#include "Puppet.h"
#include "cocos2d.h"
#include "Entity.h"
#include "Player.h"

class CEnemyPuppet : public CPuppet
{
public:
    bool init(IMapPhysics &mapPhysics);
    void onEnter() override;
    void onExit() override;
	cocos2d::RefPtr<Animate> GetAnim();
	cocos2d::RefPtr<Animate> GetAnimDead();
	cocos2d::RefPtr<cocos2d::Sprite> GetSprite();

private:
    cocos2d::RefPtr<cocos2d::Sprite> m_body;
	cocos2d::RefPtr<Animate> m_attack;
	cocos2d::RefPtr<Animate> m_dead;
};

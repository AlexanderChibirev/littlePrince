#pragma once
#include "cocos_custom.h"
#include "IPuppeteer.h"
#include "EnemyPuppet.h"
#include "Player.h"
#include "CustomMap.h"
#include "SimpleAudioEngine.h"

class CEnemyPuppeteer : protected IPuppeteer, CEntity
{
public:
    CEnemyPuppeteer(Player * player, CCustomMap *customMap);

    void SetPuppet(CEnemyPuppet *enemy);

protected:
    void OnEnter() override;
    void Update(float delta) override;
    void OnExit() override;
    cocos2d::Vec2 GetVelocity() const override;

private:
    void UpdateAI();
	void UpdateSound();
	Player *m_player;
	CCustomMap *m_customMap;
    CPuppetPtr m_enemy;
    float m_timeSinceLastUpdate = 0;
	float m_time = 0;
    cocos2d::Vec2 m_velocity;
	cocos2d::RefPtr<Animate> m_attack;
	cocos2d::RefPtr<Animate> m_dead;
	cocos2d::Sprite *m_sprite;
	float m_life = 100;
	float m_healthHero = 100;
	float m_timeSound = 0 ;
};

typedef std::unique_ptr<CEnemyPuppeteer> CEnemyPuppeteerPtr;

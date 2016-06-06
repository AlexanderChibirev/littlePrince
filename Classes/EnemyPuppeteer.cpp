#include "EnemyPuppeteer.h"

USING_NS_CC;

namespace
{
const float AI_UPDATE_INTERVAL = 0.5f; // in seconds.
const float NEAR_DISTANCE = 400.f;
const float ENEMY_SPEED = 100.f;
}

CEnemyPuppeteer::CEnemyPuppeteer(Player * player, CCustomMap *customMap)
	: m_player(player)
	, m_customMap(customMap)
{

}

void CEnemyPuppeteer::SetPuppet(CEnemyPuppet *enemy)
{
    m_enemy = enemy;
	m_attack = enemy->GetAnim();
	m_dead = enemy->GetAnimDead();
	m_sprite = enemy->GetSprite();
    if (m_enemy)
    {
        m_enemy->SetPuppeteer(this);
    }
}

void CEnemyPuppeteer::OnEnter()
{
}

void CEnemyPuppeteer::Update(float delta)
{
	m_sprite->runAction(RepeatForever::create(m_attack));//анимация
    m_timeSinceLastUpdate += delta;
	if (m_enemy->getBoundingBox().intersectsRect(m_player->getBoundingBox()))
	{
		if(m_player->attak)
		{
			m_life -= 2;
			m_sprite->runAction(RepeatForever::create(m_dead));
		}
		else
		{
			UpdateSound();
		}
	}
	if (m_time > AI_UPDATE_INTERVAL*50)
	{
		m_sprite->stopAllActions();
	}
    if (m_timeSinceLastUpdate > AI_UPDATE_INTERVAL)
    {
        m_timeSinceLastUpdate = 0;
        UpdateAI();
    }
	if (m_life < 0)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/deadEnemy.wav");
		m_enemy->setPosition(-1000, -1000);
	}
}

void CEnemyPuppeteer::UpdateSound()
{
	if (m_timeSound < 0.1)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/enemyAttack.wav");
		m_timeSound += 1;
	}
	else
	{
		m_timeSound += 30 * 0.020f;
	}
	if (m_timeSound > 30)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/enemyAttack.wav");
		m_timeSound = 0;
	}
	m_player->runAction(RepeatForever::create((m_player->m_enemyAttack)));
	m_healthHero -= 2;
	m_player->SetHealthPoint(m_player->GetHealthPoint() - 1);
}

void CEnemyPuppeteer::OnExit()
{
}

cocos2d::Vec2 CEnemyPuppeteer::GetVelocity() const
{
    return m_velocity;
}

void CEnemyPuppeteer::UpdateAI()
{
    Vec2 heroPos = m_player->getPosition();
    Vec2 ownPos = m_enemy->getPosition();
    Vec2 directPath = heroPos - ownPos;
    if (directPath.getLength() < NEAR_DISTANCE)
    {
        float factor = std::min<float>(ENEMY_SPEED, directPath.getLength());
        m_velocity = directPath.getNormalized() * factor;
    }
	else
	{
		m_velocity = Vec2(0, 0);
	}
}

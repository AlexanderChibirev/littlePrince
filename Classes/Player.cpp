#include "Player.h"
#include <iostream>
#include <string>
#include "SimpleAudioEngine.h"

Player::Player() = default;

void Player::initOptions()
{
	this->SetDamage(10);
	this->SetHealthPoint(100);
	this->SetSpeed(250);
	this->SetDirection(MovementDirection(0, 0));
	this->m_state = this->state::stay;
	this->m_gun = this->CurrentGun::FireGun;
	this->runAction(RepeatForever::create(m_currentState[0]));
	this->schedule(schedule_selector(Player::Update), 1 / 60);
}

void Player::initAnimations()
{
	SpriteFrameCache *sharedSpriteFrameCache = SpriteFrameCache::getInstance();
	std::vector<cocos2d::RefPtr<Animate>> animationsVector;

	auto animFrames = Vector<SpriteFrame*>(20);

	char str[100] = { 0 };
	for (int i = 1; i < 10; i++)
	{
		//survivor - idle_knife_00.png
		//idle_1.png
		sprintf(str, "idle_%d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.18f);
	this->stayWithKnife = Animate::create(animation);
	animationsVector.push_back(Animate::create(animation));
	animFrames.clear();

	for (int i = 1; i < 27; i++) //ץמהüבא ס םמזמל
	{
		sprintf(str, "run_%d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
	this->runWithKnife = Animate::create(animation);
	this->m_right = Animate::create(animation);
	animationsVector.push_back(Animate::create(animation));
	animFrames.clear();

	for (int i = 1; i < 27; i++)
	{
		sprintf(str, "runL_%d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
	this->m_left = Animate::create(animation);
	animationsVector.push_back(Animate::create(animation));

	animFrames.clear();

	for (int i = 1; i < 5; i++)
	{
		sprintf(str, "fire_%d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	animationsVector.push_back(Animate::create(animation));
	m_states.insert({ CurrentGun::FireGun, animationsVector });
	m_currentAction = m_states.find(CurrentGun::FireGun)->second[2];
	animFrames.clear();

	for (int i = 1; i < 4; i++)
	{
		sprintf(str, "fall_%d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
	this->m_fall = Animate::create(animation);
	animationsVector.push_back(Animate::create(animation));
	animFrames.clear();

	for (int i = 1; i < 4; i++) 
	{
		sprintf(str, "attackEnemy_%d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
	this->m_enemyAttack = Animate::create(animation);
	animationsVector.push_back(Animate::create(animation));
	animFrames.clear();


	for (int i = 1; i < 9; i++)
	{
		sprintf(str, "jumpHero_%d.png", i);
		SpriteFrame* frame = sharedSpriteFrameCache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	this->m_jump = Animate::create(animation);
	animationsVector.push_back(Animate::create(animation));
	animFrames.clear();
}

Player* Player::create(Joystick *moveJoy)

{
	Player* pSprite = new Player();
	pSprite->m_moveController = moveJoy;
	pSprite->initAnimations();
	pSprite->m_currentState = pSprite->m_states.find(CurrentGun::FireGun)->second;

	if (pSprite->initWithSpriteFrameName("idle_1.png"))
	{
		pSprite->autorelease();
		pSprite->initOptions();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return nullptr;
}


Vec2 Player::GetPositionHero()
{
	return { m_dx,m_dy } ;
}

void Player::SetGravityHero(float gravity)
{
	m_dy = gravity;
}

void Player::Update(float dt)
{
	double x = m_moveController->getVelocity().x;
	double y = m_moveController->getVelocity().y;
	double dx = 0;
	double dy = 0;

	auto angle = this->getRotation();
	angle = (abs(360 - angle));
	angle = (int(angle) % 360 * M_PI) / 180;
	if (m_moveController->getVelocity().x != 0)
	{
		dx = x * cos(angle); 
	}
	if (m_moveController->getVelocity().y != 0)
	{
		dy = x * sin(angle); 
	}
	m_dx = dx;
	x = dx*5 + this->getPosition().x;
	y = dy*5 + this->getPosition().y;

	this->setPosition(x, m_dy + y);
	if (GetPositionHero().x > 0 && (m_state != state::right) )
	{
		this->stopAllActions();
		this->Right();
		m_state = right;
	}
	if (GetPositionHero().x < 0 && (m_state != state::left) )
	{
		this->stopAllActions();
		this->Left();
		m_state = left;
	}
	else if (m_dx == 0 && m_state != state::stay && (m_state != state::gun))
	{
		this->stopAllActions();
		this->Stay();
		m_state = stay;
	}
	if(m_state == state::gun)
	{
		if (m_timeSinceLastUpdate > 1.5)
		{
			this->stopAllActions();
			m_state = state::stay;
			m_timeSinceLastUpdate = 0;
		}
	}
	m_timeAttak += 30 * 0.05f;
	if(m_timeAttak < 2 && m_timeAttak > 1)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/fire.wav");
	}
	if(attak == true && m_timeAttak > 30)
	{
		this->stopAllActions();
		m_state = state::stay;
		runAction(RepeatForever::create(m_currentState[0]));
		attak = false;
	}
	if(m_fallBegin)
	{
		m_timeFall += 40 * 0.003f;
		if (m_timeFall > 40)
		{
			this->stopAllActions();
			m_state = state::stay;
			m_fallBegin = false;
			m_timeFall = 0;
			runAction(RepeatForever::create(m_currentState[0]));
		}
	}
}

void Player::Shoot(cocos2d::Layer * layer)
{
	m_timeAttak = 0;
	attak = true;
	this->stopAllActions();
	m_state = state::gun;
	m_currentAction = m_states.find(CurrentGun::FireGun)->second[3];
	this->runAction(m_currentAction);
}

void Player::Stay()
{
	runAction(RepeatForever::create(m_currentState[0]));
}

void Player::Fall()
{
	m_fallBegin = true;
	runAction(RepeatForever::create(m_fall));
}


void Player::Left()
{
	runAction(RepeatForever::create( (m_left)));
}

void Player::Right()
{
	runAction(RepeatForever::create(m_right));
}

void Player::SetHPLine(cocos2d::Sprite * line)
{
	m_healthLine = line;
}

void Player::UpdateHPLine()
{
	auto health = (GetHealthPoint() > 0) ? GetHealthPoint() : 0;
	m_healthLine->setScaleY(health / 300.0f);
}

void Player::Jump()
{
	runAction(RepeatForever::create(m_jump));
}

void  Player::SetHP(float hp)
{
	SetHealthPoint(hp);
}
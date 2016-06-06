#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MainMenu.h"
#include "SimpleAudioEngine.h"
#include "Player.h"
#include "GameEndScene.h"
#include "Coogwheel.h"
#include "WinScene.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//1. super init first
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_customMap = make_node<CCustomMap>("tmx/layer_1.tmx");
	

	auto joy = Joystick::create();
	m_joy = joy;

	SpriteFrameCache *sharedSpriteFrameCache = SpriteFrameCache::getInstance();
	sharedSpriteFrameCache->addSpriteFramesWithFile("flyEnemy.plist");
	sharedSpriteFrameCache->addSpriteFramesWithFile("damageHero.plist");
	sharedSpriteFrameCache->addSpriteFramesWithFile("idleHero.plist");
	sharedSpriteFrameCache->addSpriteFramesWithFile("jumpHero.plist");
	sharedSpriteFrameCache->addSpriteFramesWithFile("fireHero.plist");
	sharedSpriteFrameCache->addSpriteFramesWithFile("deadEnemy.plist");
	sharedSpriteFrameCache->addSpriteFramesWithFile("runHero.plist");
	sharedSpriteFrameCache->addSpriteFramesWithFile("fallHero.plist");
	sharedSpriteFrameCache->addSpriteFramesWithFile("runHeroLeft.plist");
	
	m_player = Player::create(joy);
	this->m_player->setPosition((m_customMap->GetHeroWorldPosition().origin));

	addChild(m_customMap);
	InitCamera();

	m_obstacles = m_customMap->GetObcaslePosition();
	m_obstaclesOnGround = m_customMap->GetObcasleOnGround();
	m_jumpButtonPlay->addTouchEventListener(CC_CALLBACK_2(GameScene::MoveUp, this));
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	m_HPLine->initWithFile("health_line.png");
	m_player->SetHPLine(m_HPLine);
	__String *tempScore = __String::createWithFormat("%i", m_scoreKey);
	m_scoreLabel = Label::createWithTTF(tempScore->getCString(), "fonts/CyrilicOld.TTF", visibleSize.height * 0.1);
	m_scoreLabel->setColor(Color3B::BLACK);
	
	
	InitBonus();
	InitCoogwheel();
	this->addChild(m_HPLine);
	this->addChild(m_player);
	this->addChild(joy);
	this->addChild(m_scoreLabel);
	this->addChild(m_spriteKeyIcon);
	this->addChild(m_jumpButtonPlay);
	SpawnEnemies();
	this->scheduleUpdate();
	return true;
}

void GameScene::InitCamera() 
{
	cameraTarget = Sprite::create();
	cameraTarget->setPositionX(m_player->getPosition().x); // set to players x 
	cameraTarget->setPositionY(m_player->getPosition().y); // center of height 
	cameraTarget->retain();
	camera = Follow::create(cameraTarget, Rect::ZERO);
	camera->retain();
	this->runAction(camera);
}

void GameScene::InitCoogwheel() 
{
	m_obstaclesCoogwheel = m_customMap->GetObcasleCoogwheel();
	for (auto &it : m_obstaclesCoogwheel)
	{
		auto explision = CCoogwheel::create(Vec2(it.getMidX(), it.getMidY()));
		this->addChild(explision);
	}
}


void GameScene::InitBonus()
{
	m_obstaclesBonusJump = m_customMap->GetObcasleBonusJumpPosition();
	for (auto &it : m_obstaclesBonusJump)
	{
		Sprite *Bonus = cocos2d::Sprite::create();
		Bonus->initWithFile("superJump.png");
		Bonus->setPosition(Vec2(it.getMidX(), it.getMidY()));
		addChild(Bonus);
	}

	m_obstaclesBonusHP = m_customMap->GetObcasleBonusHPPosition();
	int i = 0;
	for (auto &it : m_obstaclesBonusHP)
	{
		auto BonusHp = cocos2d::Sprite::create();
		BonusHp->initWithFile("healthBonus.png");
		m_SpriteBonusHP.emplace_back(BonusHp);
		m_SpriteBonusHP[i]->setPosition(Vec2(it.getMidX(), it.getMidY()));
		addChild(m_SpriteBonusHP[i]);
		++i;
	}

	m_obstaclesKey = m_customMap->GetObcasleKey();
	i = 0;
	for (auto &it : m_obstaclesKey)
	{
		auto BonusHp = cocos2d::Sprite::create();
		BonusHp->initWithFile("key.png");
		m_SpriteKey.emplace_back(BonusHp);
		m_SpriteKey[i]->setPosition(Vec2(it.getMidX(), it.getMidY()));
		addChild(m_SpriteKey[i]);
		++i;
	}
}

void GameScene::MoveUp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType eventType)
{
	if (cocos2d::ui::Widget::TouchEventType::BEGAN == eventType && m_player->m_onGround)
	{
		m_clickedJump = true;
	}
	return;
}


bool GameScene::onTouchBegan(Touch * pTouches, Event * event)
{
	if (!m_joy->isContainTouch(pTouches))
	{
		m_player->Shoot(this);
	}
	return true;
}

void GameScene::update(float dt)
{
	UpdateCamera();
	float x = m_player->getPosition().x;
	float y = m_player->getPosition().y;
	if (m_player->m_onGround)
	{
		m_velocityGravity = 0;
		m_player->SetGravityHero(m_velocityGravity);
	}
	else
	{
		//if (jumpCheck)
		//{
		//	m_velocityGravity = 0;
		//	m_velocityGravity += (30 * 0.05f) * -1;//ускорение
		//	if (m_velocityGravity < -3)
		//	{
		//		m_velocityGravity = -3;
		//		m_player->SetGravityHero(m_velocityGravity);
		//		jumpCheck = false;
		//	}
		//	else
		//	{
		//		m_player->SetGravityHero(m_velocityGravity);
		//	}
		//}
		//else 
		//{
			m_velocityGravity = -3;
			m_player->SetGravityHero(m_velocityGravity);
		//}
	}
	if (m_clickedJump)
	{
		UpdateSound();
		m_player->Jump();
		m_velocityVec += m_superJump * 0.010f;//ускорение
		m_player->m_onGround = false;
		if(!jumpCheck)
		{
			m_player->setPosition(m_player->getPosition().x, m_player->getPosition().y + m_velocityVec);
		}
		if (m_velocityVec > m_superJump - 20)//высота прыжка
		{
			jumpCheck = true;
			m_gravity-= 10 * 0.02f;
			m_player->setPosition(m_player->getPosition().x, m_player->getPosition().y + m_gravity);
			if(m_gravity < 1)
			{
				jumpCheck = false;
				m_player->stopAllActions();
				m_player->Fall();
				m_clickedJump = false;
				m_velocityVec = 0;
				m_gravity = 3;
			}
		}
	}
	UpdateIcon(x, y);
	UpdateBonuses();
	UpdateCollisionPlayer(x, y);
	UpdateCoogwhell();
	UpdateTransitionScene();
}


void GameScene::UpdateCollisionPlayer(float x, float y)
{
	float dx = m_player->GetPositionHero().x;
	float dy = m_player->GetPositionHero().y;
	for (auto &it : m_obstacles)
	{
		if (it.intersectsRect(m_player->getBoundingBox()))
		{
			if (dx < 0 && dy < 0)
			{
				m_player->setPosition(x, y +16);//4.15 с мотором
			}
			if (dx > 0 && dy < 0)
			{
				m_player->setPosition(x, y +16);//4.15 с мотором
			}
			if (dy > 0)//если мы шли наверх
			{
				m_player->setPosition(x, y - 10);//зависит от скорости прыжка
			}
			if (dy < 0 && !m_player->m_onGround)
			{
				m_player->m_onGround = true;
				dy = 0;
				m_player->setPosition(x, y + 6);//если падаем
			}
			if (dx > 0)
			{
				m_player->setPosition(x - 18, y);//4.15 с мотором
			}
			if (dx < 0)
			{
				m_player->setPosition(x + 18, y);//4.15 с мотором
			}
		}
	}
	for (auto &it : m_obstaclesOnGround)
	{
		if (it.intersectsRect(m_player->getBoundingBox()))
		{
			m_player->m_onGround = false;
		}
	}
}

void GameScene::UpdateIcon(float x, float y)
{
	m_jumpButtonPlay->setPosition(Point(x + 400, y - 250));
	m_joy->setPosition(x - 470, y - 300);
	m_HPLine->setPosition(x + 400, y);
	m_spriteKeyIcon->setPosition(x - 50, y + 270);
	m_scoreLabel->setPosition(x, y + 270);
}
void GameScene::UpdateCamera()
{
	cameraTarget->setPositionX(m_player->getPosition().x); // set to players x 
	cameraTarget->setPositionY(m_player->getPosition().y); // center of height 
	cameraTarget->retain();

	camera = Follow::create(cameraTarget, Rect::ZERO);
	camera->retain();
	this->runAction(camera);
}

void GameScene::UpdateSound()
{
	if (m_timeSoundAttackHero = 0 < 0.1)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/jump.wav");
		m_timeSoundAttackHero += 1;
	}
	else
	{
		m_timeSoundAttackHero += 30 * 0.020f;
	}
	if (m_timeSoundAttackHero > 30)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/jump.wav");
		m_timeSoundAttackHero = 0;
	}
}



void GameScene::SpawnEnemies()
{
	for (const Vec2 &pos : m_customMap->GetEnemyWorldPositions())
	{
		auto enemy = make_node<CEnemyPuppet>(*m_customMap);
		enemy->setPosition(convertToNodeSpace(pos));
		auto puppeteer = std::make_unique<CEnemyPuppeteer>(m_player, m_customMap);
		puppeteer->SetPuppet(enemy);
		addChild(enemy);
		m_enemyPuppeteers.emplace_back(std::move(puppeteer));
	}
}

void GameScene::GoToGameOverScene()
{
	auto scene = GameEndScene::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}	

void GameScene::GoToGameWinScene()
{
	auto scene = WinScene::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}


void  GameScene::UpdateCoogwhell()
{
	for (auto &it : m_obstaclesCoogwheel)
	{
		if (it.intersectsRect(m_player->getBoundingBox()))
		{
			m_player->runAction(RepeatForever::create((m_player->m_enemyAttack)));
			m_player->SetHealthPoint(m_player->GetHealthPoint() - 1);
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
		}
	}
}

void GameScene::UpdateBonuses()
{

	for (auto &it : m_obstaclesBonusJump)
	{
		if (it.intersectsRect(m_player->getBoundingBox()))
		{
			m_superJump = 35;
		}
	}
	if (m_superJump == 35)
	{
		timeBonuse += 30 * 0.020f;
	}
	if (m_superJump == 35 && timeBonuse >30)
	{
		m_superJump = 30;
		timeBonuse = 0;
	}
	for (auto &it : m_SpriteBonusHP)
	{
		if (it->getBoundingBox().intersectsRect(m_player->getBoundingBox()))
		{
			m_player->SetHealthPoint(m_player->GetHealthPoint() + 30);
			it->removeFromParent();
			m_SpriteBonusHP.erase(remove(m_SpriteBonusHP.begin(), m_SpriteBonusHP.end(), it), m_SpriteBonusHP.end());
		}
	}
	for (auto &it : m_SpriteKey)
	{
		if (it->getBoundingBox().intersectsRect(m_player->getBoundingBox()))
		{
			//m_player->SetHealthPoint(m_player->GetHealthPoint() + 30);
			m_scoreKey++;
			__String *tempScore = __String::createWithFormat("%i", m_scoreKey);
			m_scoreLabel->setString(tempScore->getCString());
			it->removeFromParent();
			m_SpriteKey.erase(remove(m_SpriteKey.begin(), m_SpriteKey.end(), it), m_SpriteKey.end());
		}
	}

}



void GameScene::UpdateTransitionScene()
{
	if (!m_player->IsLife())
	{
		GoToGameOverScene();
	}
	m_player->UpdateHPLine();
	if (m_scoreKey == 6)
	{
		GoToGameWinScene();
	}
}
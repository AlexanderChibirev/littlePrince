#pragma once
#include "cocos2d.h"
#include "Joystick.h"
#include "Player.h"
#include "cocos_custom.h"
#include "CustomMap.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "EnemyPuppet.h"
#include "EnemyPuppeteer.h"

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);

	void update(float dt);
private:
	void GoToGameWinScene();
	void GoToGameOverScene();
	void InitBonus();
	void InitCoogwheel();
	void SpawnEnemies();
	void SetJoystick(Joystick *joy) { m_joy = joy; };
	void MoveUp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType eventType);
	bool onTouchBegan(Touch* pTouches, Event* event);
	void UpdateBonuses();
	void InitCamera();
	void UpdateCamera();
	void UpdateSound();
	void UpdateIcon(float x, float y);
	void UpdateCollisionPlayer(float x, float y);
	void UpdateCoogwhell();
	void UpdateTransitionScene();
	std::vector<CEnemyPuppeteerPtr> m_enemyPuppeteers;
	std::vector<cocos2d::Rect> m_obstaclesCoogwheel;
	std::vector<cocos2d::Rect> m_obstacles;
	std::vector<cocos2d::Rect> m_obstaclesOnGround;
	std::vector<cocos2d::Rect> m_obstaclesBonusJump;
	std::vector<cocos2d::Rect> m_obstaclesBonusHP;
	std::vector<cocos2d::Sprite*> m_SpriteBonusHP;
	std::vector<cocos2d::Rect> m_obstaclesKey;
	std::vector<cocos2d::Sprite*> m_SpriteKey;

	float m_timeSoundAttackHero = 0;
	ui::Button* m_jumpButtonPlay = ui::Button::create("jump.png", "jumpClicked.png");
	Sprite *m_HPLine = cocos2d::Sprite::create();
	CCustomMap *m_customMap = nullptr;
	Joystick *m_joy;
	Player *m_player;
	float m_timeSound = 0;
	float m_velocityVec = 30;
	Sprite *m_spriteKeyIcon = Sprite::create("key.png");
	Sprite *cameraTarget;
	Follow *camera;
	bool va = true;
	bool m_clickedJump = false;
	float m_superJump = 30;
	float timeBonuse = 0;
	unsigned m_scoreKey = 0;
	cocos2d::Label *m_scoreLabel;
	float m_gravity = 3;
	float m_velocityGravity = 0;
	bool jumpCheck = false;
};


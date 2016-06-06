#include "GameEndScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MainMenu.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* GameEndScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameEndScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameEndScene::init()
{
	//1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->scheduleOnce(schedule_selector(GameEndScene::GoToMenuScene), 10);

	auto backgrowndSprite = Sprite::create("forMenuScene/bgGameOver.png");
	backgrowndSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	this->addChild(backgrowndSprite);

	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/GameEnd.mp3", true);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);

	return true;
}

void GameEndScene::GoToMenuScene(float dt)
{
	auto scene = MainMenu::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(2.5f, scene));
}


#include "WinScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MainMenu.h"

#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* WinScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = WinScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool WinScene::init()
{
	//1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->scheduleOnce(schedule_selector(WinScene::GoToMenuScene), 2);

	auto backgrowndSprite = Sprite::create("forMenuScene/winScene.png");
	backgrowndSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	this->addChild(backgrowndSprite);

	return true;
}

void WinScene::GoToMenuScene(float dt)
{
	auto scene = MainMenu::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}


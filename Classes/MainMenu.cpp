#include "MainMenu.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "OptionalScene.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* MainMenu::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainMenu::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto closeLabel = Label::createWithTTF("Exit!", "fonts/Marker Felt.ttf", 24);
	auto closeItem = MenuItemLabel::create(closeLabel, CC_CALLBACK_1(MainMenu::OnMenuClose, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));
	auto playLabel = Label::createWithTTF("Play Now!", "fonts/Marker Felt.ttf", 60);
	auto playItem = MenuItemLabel::create(playLabel, CC_CALLBACK_1(MainMenu::OnMenuPlay, this));

	playItem->setPosition(Vec2(origin.x + 0.5f * visibleSize.width, origin.y + 0.8f * visibleSize.height));

	auto optionLabel = Label::createWithTTF("optional", "fonts/Marker Felt.ttf", 60);
	auto optionItem = MenuItemLabel::create(optionLabel, CC_CALLBACK_1(MainMenu::OnOptionsPlay, this));

	optionItem->setPosition(Vec2(origin.x + 0.5f * visibleSize.width, origin.y + 0.8f * visibleSize.height - 100));

	auto sprite = Sprite::create("forMenuScene/bgMenu.png");
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(sprite);

	auto menu = Menu::create(closeItem, playItem, optionItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/Menu.mp3", true);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);
	return true;
}

void MainMenu::GoToGameScene(cocos2d::Ref *sender)
{
	auto scene = GameScene::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}



void MainMenu::OnMenuClose(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MainMenu::OnMenuPlay(Ref *pSender)
{
	auto scene = GameScene::createScene();

	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}

void  MainMenu::OnOptionsPlay(cocos2d::Ref* pSender)
{
	auto scene = OptionalScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(3.0f, scene));
}
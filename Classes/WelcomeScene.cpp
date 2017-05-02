#include"WelcomeScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene* Welcome::createScene()
{
	auto scene = Scene::create();
	auto layer = Welcome::create();
	scene->addChild(layer);
	return scene;
}

bool Welcome::init()
{
	if (!Layer::init())
		return false;
	Size visibleSize = Director::getInstance()->getVisibleSize();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("fruit.plist");

    auto background = Sprite::create("bg.png");
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(background);


	auto redBall1 = Sprite::createWithSpriteFrameName("redBall1.png");
	redBall1->setPosition(visibleSize.width-redBall1->getContentSize().width / 2, visibleSize.height + redBall1->getContentSize().height / 2);
	redBall1->runAction(Sequence::create(DelayTime::create(0.0f),MoveBy::create(0.5,Vec2(0,-(redBall1->getContentSize().height))),NULL));
	this->addChild(redBall1);

	auto redBall2 = Sprite::createWithSpriteFrameName("redBall2.png");
	redBall2->setPosition(visibleSize.width / 3, visibleSize.height + redBall2->getContentSize().height / 2);
	redBall2->runAction(Sequence::create(DelayTime::create(0.2f), MoveBy::create(0.5, Vec2(0, -(redBall2->getContentSize().height))), NULL));
	this->addChild(redBall2);

	auto blueBall1 = Sprite::createWithSpriteFrameName("blueBall1.png");
	blueBall1->setPosition(blueBall1->getContentSize().width / 2, visibleSize.height + blueBall1->getContentSize().height / 2);
	blueBall1->runAction(Sequence::create(DelayTime::create(0.3f), MoveBy::create(0.5, Vec2(0, -(blueBall1->getContentSize().height))), NULL));
	this->addChild(blueBall1);
	auto blueBall2 = Sprite::createWithSpriteFrameName("blueBall2.png");
    blueBall2->setPosition(visibleSize.width / 3*2, visibleSize.height + blueBall2->getContentSize().height / 2);
	blueBall2->runAction(Sequence::create(DelayTime::create(0.4f), MoveBy::create(0.5, Vec2(0, -(blueBall2->getContentSize().height))), NULL));
	this->addChild(blueBall2);

	auto tree = Sprite::createWithSpriteFrameName("tree.png");
	tree->setPosition(visibleSize.width / 4 * 3, visibleSize.height + tree->getContentSize().height / 2);
	tree->runAction(Sequence::create(DelayTime::create(1.0f), MoveBy::create(0.5, Vec2(0, -(tree->getContentSize().height))), NULL));
	this->addChild(tree);

	auto fatherChrismas = Sprite::createWithSpriteFrameName("fatherChrismas.png");
	fatherChrismas->setPosition(visibleSize.width / 4 * 1, visibleSize.height + fatherChrismas->getContentSize().height / 2);
	fatherChrismas->runAction(Sequence::create(DelayTime::create(1.2f), MoveBy::create(0.5, Vec2(0, -(fatherChrismas->getContentSize().height))), NULL));
	this->addChild(fatherChrismas);

	auto snowman = Sprite::createWithSpriteFrameName("snowman.png");
	snowman->setPosition(visibleSize.width / 4 * 2, visibleSize.height + snowman->getContentSize().height / 2);
	snowman->runAction(Sequence::create(DelayTime::create(1.4f), MoveBy::create(0.5, Vec2(0, -(snowman->getContentSize().height))), NULL));
	this->addChild(snowman);
	
	auto label = Label::createWithSystemFont("START GAME", "Arial", 70);
	label->setColor(Color3B::BLACK);
	auto menuItem = MenuItemLabel::create(label, CC_CALLBACK_1(Welcome::menuCallback, this));
	auto menu = Menu::create(menuItem, NULL);
	menu->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	menu->runAction(Sequence::create(DelayTime::create(1.5), Show::create(),NULL));
	this->addChild(menu);
	return true;
}

void Welcome::menuCallback(cocos2d::Ref * sender)
{
	auto scene = NewGame::createScene();
	Director::getInstance()->replaceScene(scene);
}

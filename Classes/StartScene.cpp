#include "StartScene.h"

#pragma execution_character_set("utf-8")
USING_NS_CC;
using namespace CocosDenshion;

void StartScene::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

Scene* StartScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Point(0, 0));
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	// 'layer' is an autorelease object
	auto layer = StartScene::create(scene->getPhysicsWorld());

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

StartScene * StartScene::create(PhysicsWorld * world) {
	StartScene* pRet = new(std::nothrow) StartScene();
	if (pRet && pRet->init(world)) {
		pRet->autorelease();
		return pRet;
	}
	delete pRet;
	pRet = NULL;
	return NULL;
}

// on "init" you need to initialize your instance
bool StartScene::init(PhysicsWorld* world)
{

	if (!Layer::init())
	{
		return false;
	}
	this->setPhysicsWorld(world);
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	auto title = Sprite::create("title.png");
	title->setPosition(Vec2(visibleSize.width*0.5, visibleSize.height*0.8));
	addChild(title);

	AddStartBtn();
	AddEdge();
	ShootBullet();
	ShootBullet();
	if (GlobalVar::GlobalScore != -1) AddScore();

	return true;
}

void StartScene::AddEdge() {
	auto edgeSp = Sprite::create();
	auto boundBody = PhysicsBody::createEdgeBox(visibleSize, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	boundBody->setDynamic(false);
	edgeSp->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	edgeSp->setPhysicsBody(boundBody);
	edgeSp->getPhysicsBody()->setCollisionBitmask(0xFF);
	edgeSp->getPhysicsBody()->setContactTestBitmask(0xFF);
	this->addChild(edgeSp);
}

void StartScene::AddStartBtn() {
	auto menuItem = MenuItemImage::create(
		"Start-Btn.png",
		"Start-Btn.png",
		CC_CALLBACK_1(StartScene::ToGameSence, this));
	menuItem->setPosition(Vec2(visibleSize.width*0.5 + origin.x, visibleSize.height*0.2 + origin.y));
	menuItem->setScale(0.4f);
	auto menu = Menu::create(menuItem, NULL);
	menu->setPosition(Vec2::ZERO);

	auto physicsBody = PhysicsBody::createBox(menuItem->getContentSize(), PhysicsMaterial(0.1f, 1.0f, 0.0f));
	physicsBody->setDynamic(false);
	menuItem->setPhysicsBody(physicsBody);
	menuItem->getPhysicsBody()->setCollisionBitmask(0xFF);
	menuItem->getPhysicsBody()->setContactTestBitmask(0xFF);

	this->addChild(menu, 1);
}

void StartScene::ShootBullet() {
	auto bullet = Sprite::create("ControlCenter.png");
	auto direction = random(-360, 360);

	bullet->setPosition(
		random(0.0f + bullet->getContentSize().width, visibleSize.width - bullet->getContentSize().width),
		random(0.0f + bullet->getContentSize().height, visibleSize.height - bullet->getContentSize().height)
		);
	bullet->setPhysicsBody(PhysicsBody::createCircle(25, PhysicsMaterial(0.1f, 1.0f, 0.0f)));
	auto physicsBody = bullet->getPhysicsBody();
	physicsBody->setCollisionBitmask(0xFF);
	physicsBody->setContactTestBitmask(0xFF);
	addChild(bullet, 5);

	//将角度转换成弧度
	float radians = CC_DEGREES_TO_RADIANS(-direction);

	float y, x = 1;
	if ((-direction) == 90 || (-direction) == 270) x = 0;
	else if ((-direction) > 90 && (-direction) < 270) x = -1;

	y = tan(radians) * x;
	//向量标准化(即向量长度为1)
	Point normalizedVector = ccpNormalize(Vec2(x, y));

	//移动长度向量
	Point overShootVector = normalizedVector * 900;
	//超出屏幕的点
	Point offScreenPoint = bullet->getPosition() + overShootVector;

	bullet->setRotation(-direction);
	physicsBody->setVelocity(overShootVector);
}

void StartScene::ToGameSence(Ref* pSender) {
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameScene::createScene(), Color3B(0, 0, 0)));
}

void StartScene::AddScore() {
	
	TTFConfig ttfConfig;
	ttfConfig.fontFilePath = "fonts/arial.ttf";
	ttfConfig.fontSize = 36;

	auto score = Label::createWithTTF(ttfConfig, "GameOver");
	score->setPosition(Vec2(visibleSize.width*0.5, visibleSize.height*0.5));
	addChild(score);

	ttfConfig.fontSize = 30;
	auto scoreLable = Label::createWithTTF(ttfConfig, "");
	scoreLable->setPosition(Vec2(origin.x + visibleSize.width / 2, visibleSize.height*0.35));
	addChild(scoreLable, 100);

	std::string score_text;
	std::stringstream ss;
	ss.clear();
	ss << GlobalVar::GlobalScore;
	ss >> score_text;
	scoreLable->setString("Score: " + score_text);
	scoreLable->updateContent();
}

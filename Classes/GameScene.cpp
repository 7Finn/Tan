#include "GameScene.h"

#pragma execution_character_set("utf-8")
#define database UserDefault::getInstance()
using namespace CocosDenshion;
USING_NS_CC;


void GameScene::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

Scene* GameScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Point(0, 0));

	auto layer = GameScene::create(scene->getPhysicsWorld());

	scene->addChild(layer);

	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init(PhysicsWorld* world)
{

	if (!Layer::init())
	{
		return false;
	}

	this->setPhysicsWorld(world);
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	GlobalVar::GlobalScore = -1;
	

	addTouchListener();
	scheduleUpdate();
	addContactListener();
	addEdge();
	addPlayer();
	PlayBackgroundMusic();


	//��ʼ��ֵ
	shootInterval = 0.15f;
	creatInterval = 2.0f;
	bulletSpeed = 500.0f;
	highestScore = database->getIntegerForKey("highestScore", 0);
	score = 0;
	time = 0.0f;

	TTFConfig ttfConfig;
	ttfConfig.fontFilePath = "fonts/arial.ttf";
	ttfConfig.fontSize = 36;

	//����Label
	scoreLable = Label::createWithTTF(ttfConfig, "");
	scoreLable->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - scoreLable->getContentSize().height) - Vec2(0, 30));
	addChild(scoreLable, 100);
	updateScore(score, scoreLable, 0);

	//��߼�¼Label
	highestScoreLable = Label::createWithTTF(ttfConfig, "");
	highestScoreLable->setPosition(Vec2(origin.x + visibleSize.width - 150,
		origin.y + visibleSize.height - highestScoreLable->getContentSize().height) - Vec2(0, 30));
	addChild(highestScoreLable, 100);
	updateScore(highestScore, highestScoreLable, 1);


	schedule(schedule_selector(GameScene::createBlockC), creatInterval);
	schedule(schedule_selector(GameScene::createBlockS), creatInterval);
	return true;
}

GameScene * GameScene::create(PhysicsWorld * world) {
	GameScene* pRet = new(std::nothrow) GameScene();
	if (pRet && pRet->init(world)) {
		pRet->autorelease();
		return pRet;
	}
	delete pRet;
	pRet = NULL;
	return NULL;
}

void GameScene::addPlayer() {
	//�����̨
	player = Sprite::create("Player.png");
	player->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	player->setScale(0.6f);

	player->setPhysicsBody(PhysicsBody::createCircle(player->getContentSize().height/4, PhysicsMaterial(0.1f, 1.0f, 0.0f)));
	auto physicsBody = player->getPhysicsBody();
	physicsBody->setCategoryBitmask(0x04);
	physicsBody->setContactTestBitmask(0x04);
	physicsBody->setCollisionBitmask(0x0);
	physicsBody->setTag(50);

	addChild(player, 10);
}

void GameScene::createBlockS(float dt) {
	float tmpy = 0, tmpx = 0, tmp;
	
	tmp = random(0.0, 1.0);
	if (tmp < 0.5) {
		tmpx = random(origin.x, visibleSize.width);
		if (tmpx < visibleSize.width / 2) {
			tmpx = origin.x;
		}
		else {
			tmpx = visibleSize.width;
		}
		tmpy = random(origin.y, visibleSize.height);
	}
	else {
		tmpy = random(origin.y, visibleSize.height);
		if (tmpy < visibleSize.height / 2) {
			tmpy = origin.y;
		}
		else {
			tmpy = visibleSize.height;
		}
		tmpx = random(origin.x, visibleSize.width);
	}
	
	int num;
	if (score < 10) {
		num = random(5.0, 10.0);
	}
	else {
		num = random((float)score / 2, (float)score);
	}
	auto block = Block::createSquareBlock(num, 50, 50);
	block->setPosition(tmpx, tmpy);
	initBlockPhysicalBody(block);
	block->runAction(MoveTo::create(10.0f, player->getPosition()));
	blocks.push_back(block);
	addChild(block);
}

void GameScene::createBlockC(float dt) {
	float tmpy = 0, tmpx = 0, tmp;
	tmp = random(0.0, 1.0);
	if (tmp < 0.5) {
		tmpx = random(origin.x, visibleSize.width);
		if (tmpx < visibleSize.width / 2) {
			tmpx = origin.x;
		}
		else {
			tmpx = visibleSize.width;
		}
		tmpy = random(origin.y, visibleSize.height);
	}
	else {
		tmpy = random(origin.y, visibleSize.height);
		if (tmpy < visibleSize.height / 2) {
			tmpy = origin.y;
		}
		else {
			tmpy = visibleSize.height;
		}
		tmpx = random(origin.x, visibleSize.width);
	}
	int num;
	if (score < 10) {
		num = random(5.0, 10.0);
	}
	else {
		num = random((float)score / 2, (float)score);
	}

	auto block = Block::createCircleBlock(num, 25);
	block->setPosition(tmpx, tmpy);
	initBlockPhysicalBody(block);
	block->runAction(MoveTo::create(45.0f, player->getPosition()));
	blocks.push_back(block);
	addChild(block);
}

void GameScene::initBlockPhysicalBody(Block* block) {
	auto physicsBody = block->getPhysicsBody();
	physicsBody->setCategoryBitmask(0x06);
	physicsBody->setCollisionBitmask(0x01);
	physicsBody->setContactTestBitmask(0xFF);
	physicsBody->setTag(10);
	physicsBody->setMass(10000.0f);
	physicsBody->setMoment(PHYSICS_INFINITY);
	physicsBody->setDynamic(true);
}


void GameScene::addTouchListener() {
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);

	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);

	//��ӵ��¼��ַ�����
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event) {
	Vec2 position = touch->getLocation();

	//�����������
	Point shootVector = position - player->getPosition();
	//������׼��(����������Ϊ1)
	Point normalizedVector = ccpNormalize(shootVector);
	//�����ת�Ļ���
	float radians = atan2(normalizedVector.y, -normalizedVector.x);
	//������ת���ɽǶ�
	float degree = CC_RADIANS_TO_DEGREES(radians);

	player->setRotation(degree - 180);
	return true;
}
void GameScene::onTouchMoved(Touch *touch, Event *unused_event) {
	Vec2 position = touch->getLocation();

	//�����������
	Point shootVector = position - player->getPosition();
	//������׼��(����������Ϊ1)
	Point normalizedVector = ccpNormalize(shootVector);
	//�����ת�Ļ���
	float radians = atan2(normalizedVector.y, -normalizedVector.x);
	//������ת���ɽǶ�
	float degree = CC_RADIANS_TO_DEGREES(radians);

	player->setRotation(degree - 180);
}
void GameScene::onTouchEnded(Touch *touch, Event *unused_event) {

}

void GameScene::update(float dt) {
	time += dt;
	if (time >= shootInterval) {
		time = 0;
		fire();
	}
}

void GameScene::fire() {
	auto bullet = Sprite::create("ControlCenter.png");
	auto direction = player->getRotation();
	

	bullet->setScale(0.4f);
	bullet->setPosition(player->getPosition());
	bullet->setPhysicsBody(PhysicsBody::createCircle(25, PhysicsMaterial(0.1f, 1.0f, 0.0f)));
	auto physicsBody = bullet->getPhysicsBody();
	physicsBody->setGroup(-1);
	physicsBody->setCategoryBitmask(0x03);
	physicsBody->setCollisionBitmask(0x03);
	physicsBody->setContactTestBitmask(0xFF);
	physicsBody->setMass(1.0f);
	physicsBody->setTag(1);
	addChild(bullet, 5);
	bullets.push_back(bullet);

	//���Ƕ�ת���ɻ���
	float radians = CC_DEGREES_TO_RADIANS(-direction);


	float y, x = 1;
	if ((-direction) == 90 || (-direction) == 270) x = 0;
	else if ((-direction) > 90 && (-direction) < 270) x = -1;

	y = tan(radians) * x;
	//������׼��(����������Ϊ1)
	Point normalizedVector = ccpNormalize(Vec2(x, y));

	//�ƶ���������
	Point overShootVector = normalizedVector * 900;
	//������Ļ�ĵ�
	Point offScreenPoint = bullet->getPosition() + overShootVector;

	bullet->setRotation(-direction);
	physicsBody->setVelocity(overShootVector);

}

bool GameScene::onConcactBegan(PhysicsContact& contact) {
	auto body1 = contact.getShapeA()->getBody();
	auto body2 = contact.getShapeB()->getBody();
	auto sp1 = (Sprite*)body1->getNode();
	auto sp2 = (Sprite*)body2->getNode();
	if (sp1 && sp2) {
		int tag1 = sp1->getPhysicsBody()->getTag();
		int tag2 = sp2->getPhysicsBody()->getTag();

		//�ڵ���ǽ
		if (tag1 + tag2 == 1) {
			if (tag1 == 1) sp1->removeFromParentAndCleanup(true);
			else sp2->removeFromParentAndCleanup(true);
		}
		//�ڵ����
		if (tag1 + tag2 == 11) {
			Block* block;
			Sprite* bullet;
			if (tag1 == 10) {
				block = (Block*)sp1;
				bullet = sp2;
			}
			else {
				block = (Block*)sp2;
				bullet = sp1;
			}

			if (block->getIsMerging()) return false;

			int blockNumber = block->getNumber() - 1;
			if (blockNumber == 0) {
				// ���·���
				score++;
				updateScore(score, scoreLable, 0);
				if (score > highestScore) {
					highestScore = score;
					database->setIntegerForKey("highestScore", score);
				}
				// ������Ч
				SimpleAudioEngine::sharedEngine()->playEffect("music/BlockDestroy.mp3", false);
				removeBlock(block);
				block->removeFromParentAndCleanup(true);
			}
			else block->setNumber(blockNumber);

			Vec2 speed = bullet->getPhysicsBody()->getVelocity();
			static int speed_up = 0;
			if (speed_up != score / 10) {
				speed_up = score / 10;
				speed *= 2;
				bullet->getPhysicsBody()->setVelocity(speed);
			}
		}

		// �����뷽��
		if (tag1 + tag2 == 20) {
			Block* block1;
			Block* block2;
			block1 = (Block*)sp1;
			block2 = (Block*)sp2;
			if (block1->getIsMerging() || block2->getIsMerging()) return false;
			mergeBlock(block1, block2);
		}

		// ���������
		if (tag1 == 50) {
			Block* block2;
			block2 = (Block*)sp2;
			
			GameOver(block2);
		}
		else if (tag2 == 50) {
			Block* block1;
			block1 = (Block*)sp1;

			GameOver(block1);
		}
	}
	return true;
}

void GameScene::addContactListener() {
	auto touchListener = EventListenerPhysicsContact::create();
	touchListener->onContactBegin = CC_CALLBACK_1(GameScene::onConcactBegan, this);
	_eventDispatcher->addEventListenerWithFixedPriority(touchListener, 1);
}

void GameScene::addEdge() {
	auto edgeSp = Sprite::create();
	auto boundBody = PhysicsBody::createEdgeBox(visibleSize);
	boundBody->setDynamic(false);
	boundBody->setTag(0);
	edgeSp->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	edgeSp->setPhysicsBody(boundBody);
	edgeSp->getPhysicsBody()->setCollisionBitmask(0x0);
	edgeSp->getPhysicsBody()->setContactTestBitmask(0xFF);
	this->addChild(edgeSp);
}

void GameScene::removeBlock(Sprite* sprite) {
	Vector<Sprite*>::iterator iter;
	for (iter = blocks.begin(); iter != blocks.end(); iter++) if (*iter == sprite) break;
	if (iter != blocks.end()) blocks.erase(iter);
	
}

void GameScene::mergeBlock(Block* block1, Block* block2) {

	// �����ں�
	block1->setIsMerging(true);
	block2->setIsMerging(true);

	auto destoryBlock = CallFunc::create([this, block1, block2]() {
		int blockNumber1 = block1->getNumber();
		int blockNumber2 = block2->getNumber();

		Vec2 pos = block1->getPosition();

		// �ϳ�����
		blockType newType;
		if (block1->getType() == Square || block2->getType() == Square) newType = Square;
		else newType = Circle;

		// �Ƴ�����
		removeBlock(block1);
		removeBlock(block2);
		block1->removeFromParentAndCleanup(true);
		block2->removeFromParentAndCleanup(true);

		// �����·���
		Block *block;
		if (newType == Square) block = Block::createSquareBlock(blockNumber1 + blockNumber2, 50, 50);
		else block = Block::createCircleBlock(blockNumber1 + blockNumber2, 25);

		block->setPosition(pos);
		initBlockPhysicalBody(block);
		block->runAction(MoveTo::create(random(40.0f, 50.0f), this->player->getPosition()));
		blocks.push_back(block);
		addChild(block);
	});

	Vec2 pos1 = block1->getPosition();
	Vec2 pos2 = block2->getPosition();

	// �ƶ�����
	auto moveTo = MoveTo::create(0.5f, pos1);
	block1->stopAllActions();
	block2->stopAllActions();
	block1->runAction(moveTo);
	block2->runAction(Sequence::create(moveTo, destoryBlock, nullptr));
}

void GameScene::updateScore(int s, Label* sLabel, int flag) {
	std::string score_text;
	std::stringstream ss;
	ss.clear();
	ss << s;
	ss >> score_text;
	if (flag)
	score_text = "Best: " + score_text;
	sLabel->setString(score_text);
	sLabel->updateContent();
}


void GameScene::PlayBackgroundMusic() {
	//��ʼ���ű������֣�true��ʾѭ��
	//SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/Spectre.mp3", true);
}

void GameScene::Clear() {
	this->unschedule(schedule_selector(GameScene::createBlockC));
	this->unschedule(schedule_selector(GameScene::createBlockS));
	_eventDispatcher->removeAllEventListeners();
}

void GameScene::GameOver(Block* block) {
	block->setIsMerging(true);
	// ��ת�ؿ�ʼ����Ļص�
	auto gameEnd = CallFunc::create([this]() {
		GlobalVar::GlobalScore = score;
		Clear();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, StartScene::createScene(), Color3B(0, 0, 0)));
	});

	auto moveTo = MoveTo::create(0.5f, player->getPosition());
	block->stopAllActions();
	block->runAction(Sequence::create(moveTo, gameEnd, nullptr));
}
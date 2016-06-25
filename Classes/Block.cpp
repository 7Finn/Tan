#include"Block.h"
USING_NS_CC;

Block::Block():b_number(0), isMerging(false), b_lablenumber(NULL), b_layercolor(NULL) {}

Block* Block::createSquareBlock(int number, int width, int height) {
	Block* block = new Block();
	if (block && block->init()) {
		auto physicsBody = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(0.1f, 1.0f, 0.0f));
		block->autorelease();

		// 根据number获取图片，文字颜色
		int picNum;
		Color3B color;
		picNum = number / 10;
		if (picNum > 3) picNum = 3;
		switch (picNum) {
		case 0: color = ccc3(249, 211, 91); break;
		case 1: color = ccc3(209, 55, 55); break;
		case 2: color = ccc3(73, 185, 165); break;
		case 3: color = ccc3(72, 56, 167); break;
		}

		// 添加一个背景子精灵
		std::string fileName;
		std::stringstream ss;
		ss.clear();
		ss << picNum;
		ss >> fileName;
		fileName = "square" + fileName + ".png";
		auto background = Sprite::create(fileName);
		float scale = width / background->getContentSize().width;
		background->setScale(scale);
		block->addChild(background);

		block->setPhysicsBody(physicsBody);
		block->setType(Square);
		block->initSquareBlock(number, width, height, color);
		return block;
	}
	CC_SAFE_DELETE(block);
	return NULL;
}

Block* Block::createCircleBlock(int number, float radius) {
	Block* block = new Block();
	if (block && block->init()) {
		auto physicsBody = PhysicsBody::createCircle(radius, PhysicsMaterial(0.1f, 1.0f, 0.0f));
		block->autorelease();

		// 根据number获取图片，文字颜色
		int picNum;
		Color3B color;
		picNum = number / 10;
		if (picNum > 3) picNum = 3;
		switch (picNum) {
			case 0: color = ccc3(249, 211, 91); break;
			case 1: color = ccc3(209, 55, 55); break;
			case 2: color = ccc3(73, 185, 165); break;
			case 3: color = ccc3(72, 56, 167); break;
		}

		// 添加一个背景子精灵
		std::string fileName;
		std::stringstream ss;
		ss.clear();
		ss << picNum;
		ss >> fileName;
		fileName = "circle" + fileName + ".png";
		auto background = Sprite::create(fileName);
		float scale = 2*radius/ background->getContentSize().width ;
		background->setScale(scale);
		block->addChild(background);

		block->setPhysicsBody(physicsBody);
		block->setType(Circle);
		block->initCircleBlock(number, radius, color);
		return block;
	}
	CC_SAFE_DELETE(block);
	return NULL;
}

int Block::getNumber() {
	return b_number;
}


void Block::setNumber(int number) {
	b_number = number;
	b_lablenumber->setString("");
	if (b_number > 0) {
		b_lablenumber->setString(CCString::createWithFormat("%i", b_number)->getCString());
	}
}

void Block::initSquareBlock(int number, int width, int height, Color3B color) {
	b_number = number;
	b_lablenumber = CCLabelTTF::create("", "arial.ttf", 25);
	b_lablenumber->setColor(color);
	b_lablenumber->setPosition(this->getPosition());
	this->addChild(b_lablenumber);
	setNumber(number);
}


void Block::initCircleBlock(int number, float radius, Color3B color) {
	b_number = number;
	b_lablenumber = CCLabelTTF::create("", "arial.ttf", 25);
	b_lablenumber->setColor(color);
	b_lablenumber->setPosition(this->getPosition());
	this->addChild(b_lablenumber);
	setNumber(number);
}

void Block::initLevelBlock()
{
	content = CCSprite::create("levelUp.png");
	float scalX = 50 / content->getContentSize().width;
	float scalY = 50 / content->getContentSize().height;
	content->setScaleX(scalX);
	content->setScaleY(scalY);
	this->addChild(content);
}
